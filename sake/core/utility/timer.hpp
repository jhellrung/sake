/*******************************************************************************
 * sake/core/utility/timer.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct timer
 * struct timer_error
 *
 * timer is a simple replacement for boost::timer, when higher resolution is
 * desired and platform-specific behavior is acceptable.
 *
 * Note: If clock_gettime is used (the default for Linux), you will likely have
 * to add -lrt to your linker command.
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_TIMER_HPP
#define SAKE_CORE_UTILITY_TIMER_HPP

#include <exception>

#include <boost/config.hpp>
#include <boost/exception/errinfo_api_function.hpp>
#include <boost/exception/errinfo_errno.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/throw_exception.hpp>

#include <sake/boost_ext/exception/define_exception.hpp>



#if   !defined( SAKE_TIMER_USE_POSIX_CLOCK_GETTIME_REALTIME  ) \
   || !defined( SAKE_TIMER_USE_POSIX_CLOCK_GETTIME_MONOTONIC ) \
   || !defined( SAKE_TIMER_USE_POSIX_CLOCK_GETTIME_PROCESS   ) \
   || !defined( SAKE_TIMER_USE_POSIX_CLOCK_GETTIME_THREAD    ) \
   || !defined( SAKE_TIMER_USE_POSIX_GETTIMEOFDAY            ) \
   || !defined( SAKE_TIMER_USE_WINDOWS_QUERY_PERFORMANCE     ) \
   || !defined( SAKE_TIMER_USE_CTIME                         )

// No "clock" chosen, so determine one which is available.

#if   defined( BOOST_HAS_CLOCK_GETTIME )
#if   defined( _POSIX_MONOTONIC_CLOCK )
#define SAKE_TIMER_USE_POSIX_CLOCK_GETTIME_MONOTONIC
#elif defined( _POSIX_CPUTIME )
#define SAKE_TIMER_USE_POSIX_CLOCK_GETTIME_PROCESS
#else
#define SAKE_TIMER_USE_POSIX_CLOCK_GETTIME_REALTIME
#endif
#elif defined( BOOST_HAS_GETTIMEOFDAY )
#define SAKE_TIMER_USE_POSIX_GETTIMEOFDAY
#elif defined( _WIN32 )
#define SAKE_TIMER_USE_WINDOWS_QUERY_PERFORMANCE
#else
#define SAKE_TIMER_USE_CTIME
#endif

#elif defined( SAKE_TIMER_USE_POSIX_CLOCK_GETTIME_REALTIME  ) \
    + defined( SAKE_TIMER_USE_POSIX_CLOCK_GETTIME_MONOTONIC ) \
    + defined( SAKE_TIMER_USE_POSIX_CLOCK_GETTIME_PROCESS   ) \
    + defined( SAKE_TIMER_USE_POSIX_CLOCK_GETTIME_THREAD    ) \
    + defined( SAKE_TIMER_USE_POSIX_GETTIMEOFDAY            ) \
    + defined( SAKE_TIMER_USE_WINDOWS_QUERY_PERFORMANCE     ) \
    + defined( SAKE_TIMER_USE_CTIME                         ) \
      > 1

#error Must define at most one of SAKE_TIMER_USE_XXX.

#endif



#if   defined( SAKE_TIMER_USE_POSIX_CLOCK_GETTIME_REALTIME  ) \
   || defined( SAKE_TIMER_USE_POSIX_CLOCK_GETTIME_MONOTONIC ) \
   || defined( SAKE_TIMER_USE_POSIX_CLOCK_GETTIME_PROCESS   ) \
   || defined( SAKE_TIMER_USE_POSIX_CLOCK_GETTIME_THREAD    )
#include <errno.h>
#include <time.h>
#elif defined( SAKE_TIMER_USE_POSIX_GETTIMEOFDAY            )
#include <errno.h>
#include <sys/time.h>
#elif defined( SAKE_TIMER_USE_WINDOWS_QUERY_PERFORMANCE     )
#include <Windows.h>
#ifdef BOOST_HAS_MS_INT64
BOOST_STATIC_ASSERT((boost::is_same< LONGLONG, __int64 >::value));
#endif // #ifdef BOOST_HAS_MS_INT64
#elif defined( SAKE_TIMER_USE_CTIME                         )
#include <ctime>
#endif



namespace sake
{

/*******************************************************************************
 * struct timer
 ******************************************************************************/

struct timer
{
    timer();

    void restart();
    double elapsed() const;

private:
#if   defined( SAKE_TIMER_USE_POSIX_CLOCK_GETTIME_REALTIME  ) \
   || defined( SAKE_TIMER_USE_POSIX_CLOCK_GETTIME_MONOTONIC ) \
   || defined( SAKE_TIMER_USE_POSIX_CLOCK_GETTIME_PROCESS   ) \
   || defined( SAKE_TIMER_USE_POSIX_CLOCK_GETTIME_THREAD    )
    typedef timespec time_type;
#elif defined( SAKE_TIMER_USE_POSIX_GETTIMEOFDAY            )
    typedef timeval time_type;
#elif defined( SAKE_TIMER_USE_WINDOWS_QUERY_PERFORMANCE     )
    typedef LARGE_INTEGER time_type;
#elif defined( SAKE_TIMER_USE_CTIME                         )
    typedef std::clock_t time_type;
#endif

    static void get_time(time_type& t);
#ifdef SAKE_TIMER_USE_WINDOWS_QUERY_PERFORMANCE
    static double get_frequency();
#endif // #ifdef SAKE_TIMER_USE_WINDOWS_QUERY_PERFORMANCE
    time_type m_start;
};

/*******************************************************************************
 * struct timer_error
 ******************************************************************************/

SAKE_BOOST_EXT_DEFINE_EXCEPTION( sake, timer_error )

/*******************************************************************************
 * timer member implementations
 ******************************************************************************/

inline
timer::
timer()
{ restart(); }

inline void
timer::
restart()
{ get_time(m_start); }

inline double
timer::
elapsed() const
{
    time_type now;
    get_time(now);
#if   defined( SAKE_TIMER_USE_POSIX_CLOCK_GETTIME_REALTIME  ) \
   || defined( SAKE_TIMER_USE_POSIX_CLOCK_GETTIME_MONOTONIC ) \
   || defined( SAKE_TIMER_USE_POSIX_CLOCK_GETTIME_PROCESS   ) \
   || defined( SAKE_TIMER_USE_POSIX_CLOCK_GETTIME_THREAD    )
    return (now.tv_sec - m_start.tv_sec) + 1.0e-9 * (now.tv_nsec - m_start.tv_nsec);
#elif defined( SAKE_TIMER_USE_POSIX_GETTIMEOFDAY            )
    return (now.tv_sec - m_start.tv_sec) + 1.0e-6 * (now.tv_usec - m_start.tv_usec);
#elif defined( SAKE_TIMER_USE_WINDOWS_QUERY_PERFORMANCE     )
    static double const frequency = get_frequency();
#ifdef BOOST_HAS_MS_INT64
    return (now.QuadPart - m_start.QuadPart) / frequency;
#else // #ifdef BOOST_HAS_MS_INT64
    static double const _0x100000000 = static_cast< double >(1UL << 16)
                                     * static_cast< double >(1UL << 16);
    return (now.LowPart >= m_start.LowPart ?
                _0x100000000 * (now.HighPart - m_start.HighPart) + (now.LowPart - m_start.LowPart) :
                _0x100000000 * (now.HighPart - m_start.HighPart) - (m_start.LowPart - now.LowPart))
           / frequency;
#endif // #ifdef BOOST_HAS_MS_INT64
#elif defined( SAKE_TIMER_USE_CTIME                         )
    return static_cast< double >(now - m_start) / CLOCKS_PER_SEC;
#endif
}

inline void
timer::
get_time(time_type& t)
{
#if   defined( SAKE_TIMER_USE_POSIX_CLOCK_GETTIME_REALTIME  ) \
   || defined( SAKE_TIMER_USE_POSIX_CLOCK_GETTIME_MONOTONIC ) \
   || defined( SAKE_TIMER_USE_POSIX_CLOCK_GETTIME_PROCESS   ) \
   || defined( SAKE_TIMER_USE_POSIX_CLOCK_GETTIME_THREAD    )
#if   defined( SAKE_TIMER_USE_POSIX_CLOCK_GETTIME_REALTIME  )
    int const e = clock_gettime(CLOCK_REALTIME, &t);
#elif defined( SAKE_TIMER_USE_POSIX_CLOCK_GETTIME_MONOTONIC )
    int const e = clock_gettime(CLOCK_MONOTONIC, &t);
#elif defined( SAKE_TIMER_USE_POSIX_CLOCK_GETTIME_PROCESS   )
    int const e = clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t);
#elif defined( SAKE_TIMER_USE_POSIX_CLOCK_GETTIME_THREAD    )
    int const e = clock_gettime(CLOCK_THREAD_CPUTIME_ID, &t);
#endif
    if(e == -1)
        BOOST_THROW_EXCEPTION(timer_error()
            << boost::errinfo_api_function("int clock_gettime(clockid_t,timespec*)")
            << boost::errinfo_errno(errno));
#elif defined( SAKE_TIMER_USE_POSIX_GETTIMEOFDAY            )
    int const e = gettimeofday(&t, NULL);
    if(e == -1)
        BOOST_THROW_EXCEPTION(timer_error()
            << boost::errinfo_api_function("int gettimeofday(timeval*,timezone*)")
            << boost::errinfo_errno(errno));
#elif defined( SAKE_TIMER_USE_WINDOWS_QUERY_PERFORMANCE     )
    BOOL const b = QueryPerformanceCounter(&t);
    if(!b)
        BOOST_THROW_EXCEPTION(timer_error()
            << boost::errinfo_api_function("BOOL QueryPerformanceCounter(LARGE_INTEGER*)"));
#elif defined( SAKE_TIMER_USE_CTIME                         )
    t = std::clock();
    if(t == -1)
        BOOST_THROW_EXCEPTION(timer_error()
            << boost::errinfo_api_function("std::clock_t std::clock()"));
#endif
}

#ifdef SAKE_TIMER_USE_WINDOWS_QUERY_PERFORMANCE
inline double
timer::
get_frequency()
{
    LARGE_INTEGER frequency;
    BOOL const b = QueryPerformanceFrequency(&frequency);
    if(!b)
        BOOST_THROW_EXCEPTION(timer_error()
            << boost::errinfo_api_function("BOOL QueryPerformanceFrequency(LARGE_INTEGER*)"));
#ifdef BOOST_HAS_MS_INT64
    return static_cast< double >(frequency.QuadPart);
#else // #ifdef BOOST_HAS_MS_INT64
    double const _0x100000000 = static_cast< double >(1UL << 16)
                              * static_cast< double >(1UL << 16);
    return _0x100000000 * frequency.HighPart + frequency.LowPart;
#endif // #ifdef BOOST_HAS_MS_INT64
}
#endif // #ifdef SAKE_TIMER_USE_WINDOWS_QUERY_PERFORMANCE

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_TIMER_HPP

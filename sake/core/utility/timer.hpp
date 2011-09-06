/*******************************************************************************
 * core/utility/timer.hpp
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
#include <boost/mpl/assert.hpp>
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
BOOST_MPL_ASSERT((boost::is_same< LONGLONG, __int64 >));
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

    static void get_current_time(time_type& t);
#ifdef SAKE_TIMER_USE_WINDOWS_QUERY_PERFORMANCE
    static double get_frequency();
#endif // #ifdef SAKE_TIMER_USE_WINDOWS_QUERY_PERFORMANCE
    time_type m_start_time;
};

/*******************************************************************************
 * struct timer_error
 ******************************************************************************/

SAKE_BOOST_EXT_DEFINE_EXCEPTION( sake, timer_error )

/*******************************************************************************
 * timer member function implementations
 ******************************************************************************/

inline
timer::
timer()
{ restart(); }

inline void
timer::
restart()
{ get_current_time(m_start_time); }

inline double
timer::
elapsed() const
{
    time_type current_time;
    get_current_time(current_time);
#if   defined( SAKE_TIMER_USE_POSIX_CLOCK_GETTIME_REALTIME  ) \
   || defined( SAKE_TIMER_USE_POSIX_CLOCK_GETTIME_MONOTONIC ) \
   || defined( SAKE_TIMER_USE_POSIX_CLOCK_GETTIME_PROCESS   ) \
   || defined( SAKE_TIMER_USE_POSIX_CLOCK_GETTIME_THREAD    )
    return (current_time.tv_sec - m_start_time.tv_sec)
         + 1.0e-9 * (current_time.tv_nsec - m_start_time.tv_nsec);
#elif defined( SAKE_TIMER_USE_POSIX_GETTIMEOFDAY            )
    return (current_time.tv_sec - m_start_time.tv_sec)
         + 1.0e-6 * (current_time.tv_usec - m_start_time.tv_usec);
#elif defined( SAKE_TIMER_USE_WINDOWS_QUERY_PERFORMANCE     )
    static const double frequency = get_frequency();
#ifdef BOOST_HAS_MS_INT64
    return (current_time.QuadPart - m_start_time.QuadPart) / frequency;
#else // #ifdef BOOST_HAS_MS_INT64
    static const double double_0x100000000 = static_cast< double >(1UL << 16)
                                           * static_cast< double >(1UL << 16);
    return (current_time.LowPart >= m_start_time.LowPart ?
                double_0x100000000 * (current_time.HighPart - m_start_time.HighPart)
              + (current_time.LowPart - m_start_time.LowPart) :
                double_0x100000000 * (current_time.HighPart - m_start_time.HighPart)
              - (m_start_time.LowPart - current_time.LowPart)
           ) / frequency;
#endif // #ifdef BOOST_HAS_MS_INT64
#elif defined( SAKE_TIMER_USE_CTIME                         )
    return static_cast< double >(current_time - m_start_time) / CLOCKS_PER_SEC;
#endif
}

inline void
timer::
get_current_time(time_type& t)
{
#if   defined( SAKE_TIMER_USE_POSIX_CLOCK_GETTIME_REALTIME  ) \
   || defined( SAKE_TIMER_USE_POSIX_CLOCK_GETTIME_MONOTONIC ) \
   || defined( SAKE_TIMER_USE_POSIX_CLOCK_GETTIME_PROCESS   ) \
   || defined( SAKE_TIMER_USE_POSIX_CLOCK_GETTIME_THREAD    )
#if   defined( SAKE_TIMER_USE_POSIX_CLOCK_GETTIME_REALTIME  )
    const int e = clock_gettime(CLOCK_REALTIME, &t);
#elif defined( SAKE_TIMER_USE_POSIX_CLOCK_GETTIME_MONOTONIC )
    const int e = clock_gettime(CLOCK_MONOTONIC, &t);
#elif defined( SAKE_TIMER_USE_POSIX_CLOCK_GETTIME_PROCESS   )
    const int e = clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t);
#elif defined( SAKE_TIMER_USE_POSIX_CLOCK_GETTIME_THREAD    )
    const int e = clock_gettime(CLOCK_THREAD_CPUTIME_ID, &t);
#endif
    if(e == -1)
        BOOST_THROW_EXCEPTION(
            timer_error() << boost::errinfo_api_function("int clock_gettime(clockid_t,timespec*)")
                          << boost::errinfo_errno(errno)
        );
#elif defined( SAKE_TIMER_USE_POSIX_GETTIMEOFDAY            )
    const int e = gettimeofday(&t, NULL);
    if(e == -1)
        BOOST_THROW_EXCEPTION(
            timer_error() << boost::errinfo_api_function("int gettimeofday(timeval*,timezone*)")
                          << boost::errinfo_errno(errno)
        );
#elif defined( SAKE_TIMER_USE_WINDOWS_QUERY_PERFORMANCE     )
    const BOOL b = QueryPerformanceCounter(&t);
    if(!b)
        BOOST_THROW_EXCEPTION(
            timer_error() << boost::errinfo_api_function("BOOL QueryPerformanceCounter(LARGE_INTEGER*)")
        );
#elif defined( SAKE_TIMER_USE_CTIME                         )
    t = std::clock();
    if(t == -1)
        BOOST_THROW_EXCEPTION(
            timer_error() << boost::errinfo_api_function("std::clock_t std::clock()")
        );
#endif
}

#ifdef SAKE_TIMER_USE_WINDOWS_QUERY_PERFORMANCE
inline double
timer::
get_frequency()
{
    LARGE_INTEGER queried_frequency;
    const BOOL b = QueryPerformanceFrequency(&queried_frequency);
    if(!b)
        BOOST_THROW_EXCEPTION(
            timer_error() << boost::errinfo_api_function("BOOL QueryPerformanceFrequency(LARGE_INTEGER*)")
        );
#ifdef BOOST_HAS_MS_INT64
    return static_cast< double >(queried_frequency.QuadPart);
#else // #ifdef BOOST_HAS_MS_INT64
    const double double_0x100000000 = static_cast< double >(1UL << 16)
                                    * static_cast< double >(1UL << 16);
    return double_0x100000000 * queried_frequency.HighPart + queried_frequency.LowPart;
#endif // #ifdef BOOST_HAS_MS_INT64
}
#endif // #ifdef SAKE_TIMER_USE_WINDOWS_QUERY_PERFORMANCE

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_TIMER_HPP

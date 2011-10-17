/*******************************************************************************
 * sake/core/math/integer_difference.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct integer_difference<T>
 * struct extension::integer_difference< T, Enable = void >
 * struct default_impl::integer_difference<T>
 *
 * integer_difference<T> evaluates to an integral type which can represent the
 * "difference" between two T objects.  For builtin integral types, this will
 * generally be some wider type (if a wider type exists).
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_INTEGER_DIFFERENCE_HPP
#define SAKE_CORE_MATH_INTEGER_DIFFERENCE_HPP

#include <limits>

#include <boost/config.hpp>
#include <boost/integer.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_integral.hpp>

#include <sake/core/math/integer_difference_fwd.hpp>

namespace sake
{

/*******************************************************************************
 * struct integer_difference<T>
 ******************************************************************************/

template< class T >
struct integer_difference
    : extension::integer_difference<T>
{ };

/*******************************************************************************
 * struct extension::integer_difference< T, Enable = void >
 ******************************************************************************/

namespace extension
{

template< class T, class Enable /*= void*/ >
struct integer_difference
    : default_impl::integer_difference<T>
{ };

} // namespace extension

/*******************************************************************************
 * struct default_impl::integer_difference<T>
 ******************************************************************************/

namespace default_impl
{

namespace integer_difference_private
{

template< class T, bool = boost::is_integral<T>::value >
struct dispatch;

template< class T >
struct dispatch< T, false >
{
    BOOST_STATIC_ASSERT((std::numeric_limits<T>::is_signed));
    typedef T type;
};

template< int Bits >
struct int_t_fast
{ typedef typename boost::int_t< Bits >::fast type; };

template< class T >
struct dispatch< T, true >
{
    static int const desired_bits = std::numeric_limits<T>::digits
                                  + std::numeric_limits<T>::is_signed;
#ifndef BOOST_NO_LONG_LONG
    typedef long long widest_builtin_int_type;
#else // #ifndef BOOST_NO_LONG_LONG
    typedef long widest_builtin_int_type;
#endif // #ifndef BOOST_NO_LONG_LONG
    typedef typename boost::mpl::eval_if_c<
        (desired_bits < std::numeric_limits< widest_builtin_int_type >::digits),
        int_t_fast< desired_bits >,
        boost::mpl::identity< widest_builtin_int_type >
    >::type type;
};

} // namespace integer_difference_private

template< class T >
struct integer_difference
    : integer_difference_private::dispatch<T>
{ };

} // namespace default_impl

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_INTEGER_DIFFERENCE_HPP

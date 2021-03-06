/*******************************************************************************
 * sake/core/math/zero.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct zero_t
 * zero_t const zero
 *
 * operator<<(std::ostream& o, zero_t) -> std::ostream&
 *
 * operator[op](zero_t, zero_t) -> bool
 * operator[op](zero_t, [builtin arithmetic type] x) -> bool
 * operator[op]([builtin arithmetic type] x, zero_t) -> bool
 *
 * [op] in { ==, !=, <, >, <=, >= }
 *
 * zero_t is meant to represent a type-equivalent to the number zero ("0").  A
 * static instantiation of zero_t is provided within the sake namespace, named
 * (aptly) zero.  Thus, operators and constructors for classes can be defined in
 * terms of zero_t parameters, possibly giving efficiency improvements over
 * alternative overloads (such as using the int 0).
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_ZERO_HPP
#define SAKE_CORE_MATH_ZERO_HPP

#include <ostream>

#include <boost/mpl/int.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/type_traits/is_floating_point.hpp>
#include <boost/type_traits/is_signed.hpp>
#include <boost/type_traits/is_unsigned.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/if.hpp>
#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/mpl/uint.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/math/zero_fwd.hpp>
#include <sake/core/utility/int_tag.hpp>

namespace sake
{

namespace zero_private
{

template< class T >
inline T
as_impl();

} // namespace zero_private

template< class T >
inline T
zero_t::
as()
{ return zero_private::as_impl<T>(); }

inline std::ostream&
operator<<(std::ostream& o, sake::zero_t)
{ return o << '0'; }

inline boost:: true_type operator==(sake::zero_t, sake::zero_t) { return boost::true_type(); }
inline boost::false_type operator!=(sake::zero_t, sake::zero_t) { return boost::false_type(); }
inline boost::false_type operator< (sake::zero_t, sake::zero_t) { return boost::false_type(); }
inline boost::false_type operator> (sake::zero_t, sake::zero_t) { return boost::false_type(); }
inline boost:: true_type operator<=(sake::zero_t, sake::zero_t) { return boost::true_type(); }
inline boost:: true_type operator>=(sake::zero_t, sake::zero_t) { return boost::true_type(); }

template< class T >
inline typename boost::enable_if_c<
    boost::is_arithmetic<T>::value,
    bool
>::type
operator==(T const x, sake::zero_t)
{ return x == 0; }

template< class T >
inline typename boost::enable_if_c<
    boost::is_arithmetic<T>::value,
    bool
>::type
operator!=(T const x, sake::zero_t)
{ return x != 0; }

template< class T >
inline typename boost::enable_if_c<
    boost::is_arithmetic<T>::value,
    bool
>::type
operator==(sake::zero_t, T const x)
{ return 0 == x; }

template< class T >
inline typename boost::enable_if_c<
    boost::is_arithmetic<T>::value,
    bool
>::type
operator!=(sake::zero_t, T const x)
{ return 0 != x; }

#define define_operator( op ) \
template< class T > \
inline typename boost::enable_if_c< \
    boost_ext::mpl::or2< \
        boost::is_floating_point<T>, \
        boost::is_signed<T> \
    >::value, \
    bool \
>::type \
operator op (T const x, sake::zero_t) \
{ return x op 0; } \
template< class T > \
inline typename boost::enable_if_c< \
    boost_ext::mpl::or2< \
        boost::is_floating_point<T>, \
        boost::is_signed<T> \
    >::value, \
    bool \
>::type \
operator op (sake::zero_t, T const x) \
{ return 0 op x; }
define_operator( < )
define_operator( > )
define_operator( <= )
define_operator( >= )
#undef define_operator

template< class T >
inline typename boost::enable_if_c<
    boost::is_unsigned<T>::value,
    bool
>::type
operator<(T const /*x*/, sake::zero_t)
{ return false; }
template< class T >
inline typename boost::enable_if_c<
    boost::is_unsigned<T>::value,
    bool
>::type
operator>(T const x, sake::zero_t)
{ return x != 0; }
template< class T >
inline typename boost::enable_if_c<
    boost::is_unsigned<T>::value,
    bool
>::type
operator<=(T const x, sake::zero_t)
{ return x == 0; }
template< class T >
inline typename boost::enable_if_c<
    boost::is_unsigned<T>::value,
    bool
>::type
operator>=(T const /*x*/, sake::zero_t)
{ return true; }

template< class T >
inline typename boost::enable_if_c<
    boost::is_unsigned<T>::value,
    bool
>::type
operator<(sake::zero_t, T const x)
{ return 0 != x; }
template< class T >
inline typename boost::enable_if_c<
    boost::is_unsigned<T>::value,
    bool
>::type
operator>(sake::zero_t, T const /*x*/)
{ return false; }
template< class T >
inline typename boost::enable_if_c<
    boost::is_unsigned<T>::value,
    bool
>::type
operator<=(sake::zero_t, T const /*x*/)
{ return true; }
template< class T >
inline typename boost::enable_if_c<
    boost::is_unsigned<T>::value,
    bool
>::type
operator>=(sake::zero_t, T const x)
{ return 0 == x; }

namespace zero_private
{

template< class T >
inline T
as_dispatch(sake::int_tag<3>)
{ return T(sake::zero); }

template< class T >
inline T
as_dispatch(sake::int_tag<2>)
{ return T((boost::mpl::int_<0>())); }

template< class T >
inline T
as_dispatch(sake::int_tag<1>)
{ return T((boost_ext::mpl::uint<0>())); }

template< class T >
inline T
as_dispatch(sake::int_tag<0>)
{ return T(0); }

template< class T >
inline T
as_impl()
{
    typedef typename boost_ext::mpl::
         if_< boost_ext::is_convertible< sake::zero_t, T >,
              sake::int_tag<3> >::type::template
    else_if < boost_ext::is_convertible< boost::mpl::int_<0>, T >,
              sake::int_tag<2> >::type::template
    else_if < boost_ext::is_convertible< boost_ext::mpl::uint<0>, T >,
              sake::int_tag<1> >::type::template
    else_   < sake::int_tag<0> >::type int_tag_;
    return zero_private::as_dispatch<T>(int_tag_());
}

} // namespace zero_private

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_ZERO_HPP

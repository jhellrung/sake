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
#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/type_traits/is_floating_point.hpp>
#include <boost/type_traits/is_signed.hpp>
#include <boost/type_traits/is_unsigned.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/mpl/uint.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/math/static_intlog2.hpp>
#include <sake/core/math/zero_fwd.hpp>

namespace sake
{

namespace zero_private
{

template< class T >
struct as_dispatch_index;

template<
    class T,
    unsigned int = as_dispatch_index<T>::value
>
struct as_dispatch;

} // namespace zero_private

template< class T >
inline T
zero_t::
as()
{ return zero_private::as_dispatch<T>::apply(); }

inline std::ostream&
operator<<(std::ostream& o, sake::zero_t)
{ return o << '0'; }

inline bool operator==(sake::zero_t, sake::zero_t) { return true; }
inline bool operator!=(sake::zero_t, sake::zero_t) { return false; }
inline bool operator< (sake::zero_t, sake::zero_t) { return false; }
inline bool operator> (sake::zero_t, sake::zero_t) { return false; }
inline bool operator<=(sake::zero_t, sake::zero_t) { return true; }
inline bool operator>=(sake::zero_t, sake::zero_t) { return true; }

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
struct as_dispatch_index
{
    static unsigned int const _ =
        (1 << 3) * boost_ext::is_convertible< sake::zero_t, T >::value
      | (1 << 2) * boost_ext::is_convertible< boost_ext::mpl::uint<0>, T >::value
      | (1 << 1) * boost_ext::is_convertible< boost::mpl::int_<0>, T >::value
      | (1 << 0);
    static unsigned int const value = sake::static_intlog2_c<_>::value;
};

template< class T >
struct as_dispatch<T,3>
{
    static T apply()
    { return T(sake::zero); }
};

template< class T >
struct as_dispatch<T,2>
{
    static T apply()
    { return T(boost_ext::mpl::uint<0>()); }
};

template< class T >
struct as_dispatch<T,1>
{
    static T apply()
    { return T(boost::mpl::int_<0>()); }
};

template< class T >
struct as_dispatch<T,0>
{
    static T apply()
    { return T(0); }
};

} // namespace zero_private

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_ZERO_HPP

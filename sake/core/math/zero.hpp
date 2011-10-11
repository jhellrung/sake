/*******************************************************************************
 * sake/core/math/zero.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct zero_t
 * zero_t const zero
 * operator<<(std::ostream& out, zero_t) -> std::ostream&
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

#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/type_traits/is_floating_point.hpp>
#include <boost/type_traits/is_signed.hpp>
#include <boost/type_traits/is_unsigned.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/or.hpp>

#include <sake/core/math/zero_fwd.hpp>

namespace sake
{

inline std::ostream&
operator<<(std::ostream& o, zero_t)
{ return o << '0'; }

inline bool operator==(zero_t, zero_t) { return true; }
inline bool operator!=(zero_t, zero_t) { return false; }
inline bool operator< (zero_t, zero_t) { return false; }
inline bool operator> (zero_t, zero_t) { return false; }
inline bool operator<=(zero_t, zero_t) { return true; }
inline bool operator>=(zero_t, zero_t) { return true; }

template< class T >
inline typename boost::enable_if_c<
    boost::is_arithmetic<T>::value,
    bool
>::type
operator==(T const & x, zero_t)
{ return x == 0; }

template< class T >
inline typename boost::enable_if_c<
    boost::is_arithmetic<T>::value,
    bool
>::type
operator!=(T const & x, zero_t)
{ return x != 0; }

template< class T >
inline typename boost::enable_if_c<
    boost::is_arithmetic<T>::value,
    bool
>::type
operator==(zero_t, T const & x)
{ return 0 == x; }

template< class T >
inline typename boost::enable_if_c<
    boost::is_arithmetic<T>::value,
    bool
>::type
operator!=(zero_t, T const & x)
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
operator op (T const & x, zero_t) \
{ return x op 0; } \
template< class T > \
inline typename boost::enable_if_c< \
    boost_ext::mpl::or2< \
        boost::is_floating_point<T>, \
        boost::is_signed<T> \
    >::value, \
    bool \
>::type \
operator op (zero_t, T const & x) \
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
operator<(T const & x, zero_t)
{ return false; }
template< class T >
inline typename boost::enable_if_c<
    boost::is_unsigned<T>::value,
    bool
>::type
operator>(T const & x, zero_t)
{ return x != 0; }
template< class T >
inline typename boost::enable_if_c<
    boost::is_unsigned<T>::value,
    bool
>::type
operator<=(T const & x, zero_t)
{ return x == 0; }
template< class T >
inline typename boost::enable_if_c<
    boost::is_unsigned<T>::value,
    bool
>::type
operator>=(T const & x, zero_t)
{ return true; }

template< class T >
inline typename boost::enable_if_c<
    boost::is_unsigned<T>::value,
    bool
>::type
operator<(zero_t, T const & x)
{ return 0 != x; }
template< class T >
inline typename boost::enable_if_c<
    boost::is_unsigned<T>::value,
    bool
>::type
operator>(zero_t, T const & x)
{ return false; }
template< class T >
inline typename boost::enable_if_c<
    boost::is_unsigned<T>::value,
    bool
>::type
operator<=(zero_t, T const & x)
{ return true; }
template< class T >
inline typename boost::enable_if_c<
    boost::is_unsigned<T>::value,
    bool
>::type
operator>=(zero_t, T const & x)
{ return 0 == x; }

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_ZERO_HPP

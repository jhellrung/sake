/*******************************************************************************
 * sake/core/math/one.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct one_t
 * one_t const one
 *
 * operator<<(std::ostream& o, one_t) -> std::ostream&
 *
 * operator/(one_t, T const & x) -> result_of::invert<T>::type
 *
 * operator&(one_t, [builtin integral type] x) -> bool
 * operator&([builtin integral type] x, one_t) -> bool
 *
 * sake::one is similar to sake::zero.  It provides a type to represent the
 * numeric constant "1", which may be used for more efficient construction and
 * arithmetic operations.
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_ONE_HPP
#define SAKE_CORE_MATH_ONE_HPP

#include <ostream>

#include <boost/type_traits/is_integral.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/core/functional/operators/divides.hpp>
#include <sake/core/math/invert.hpp>
#include <sake/core/math/one_fwd.hpp>

namespace sake
{

inline std::ostream&
operator<<(std::ostream& o, sake::one_t)
{ return o << '1'; }

namespace operators
{
namespace result_of
{
namespace extension
{

template< class T >
struct divides0< sake::one_t, T >
    : sake::result_of::invert<T>
{ };

template< class T >
struct bit_and0<
    sake::one_t, T,
    typename boost::enable_if_c< boost::is_integral<T>::value >::type >
{ typedef bool type; };

template< class T >
struct bit_and1<
    T, sake::one_t,
    typename boost::enable_if_c< boost::is_integral<T>::value >::type >
{ typedef bool type; };

} // namespace extension
} // namespace result_of
} // namespace operators

template< class T >
inline typename result_of::invert<T>::type
operator/(sake::one_t, T const & x)
{ return sake::invert(x); }

template< class T >
inline typename boost::enable_if_c<
    boost::is_integral<T>::value,
    bool
>::type
operator&(sake::one_t, T const x)
{ return (1 & x) != 0; }

template< class T >
inline typename boost::enable_if_c<
    boost::is_integral<T>::value,
    bool
>::type
operator&(T const x, sake::one_t)
{ return (x & 1) != 0; }

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_ONE_HPP

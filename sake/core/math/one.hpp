/*******************************************************************************
 * sake/core/math/one.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct one_t
 * one_t const one
 *
 * operator<<(std::ostream& o, one_t) -> std::ostream&
 *
 * operator<<([builtin arithmetic type] x, one_t) -> [builtin arithmetic type]
 * operator>>([builtin arithmetic type] x, one_t) -> [builtin arithmetic type]
 * operator<<=([builtin arithmetic type]& x, one_t) -> [builtin arithmetic type]&
 * operator>>=([builtin arithmetic type]& x, one_t) -> [builtin arithmetic type]&
 *
 * operator/(one_t, T const & x) -> result_of::rcp<T>::type
 *
 * operator&([builtin integral type] x, one_t) -> bool
 *
 * sake::one is similar to sake::zero.  It provides a type to represent the
 * numeric constant "1".  It is primarily used for efficient construction and
 * arithmetic operations.
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_ONE_HPP
#define SAKE_CORE_MATH_ONE_HPP

#include <ostream>

#include <boost/config.hpp>
#include <boost/mpl/int.hpp>
#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/if.hpp>
#include <sake/boost_ext/mpl/uint.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/functional/operators/bit_and.hpp>
#include <sake/core/functional/operators/divide.hpp>
#include <sake/core/math/one_fwd.hpp>
#include <sake/core/math/rcp.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/utility/int_tag.hpp>

namespace sake
{

namespace one_private
{

template< class T >
inline T
as_impl();

} // namespace one_private

template< class T >
inline T
one_t::
as()
{ return one_private::as_impl<T>(); }

inline std::ostream&
operator<<(std::ostream& o, sake::one_t)
{ return o << '1'; }

namespace operators {
namespace result_of {
namespace extension {

template< class One, class T >
struct divide< One, T,
    typename boost::enable_if_c<
        boost::is_same<
            typename boost_ext::remove_qualifiers< One >::type,
            sake::one_t
        >::value
    >::type >
    : sake::result_of::rcp<T>
{ };

template< class T >
struct bit_and< T, sake::one_t,
    typename boost::enable_if_c< boost::is_integral<T>::value >::type >
{ typedef bool type; };

} // namespace extension
} // namespace result_of
} // namespace operators

template< class T >
inline typename boost::enable_if_c<
    boost::is_arithmetic<T>::value, T >::type
operator<<(T const x, sake::one_t)
{ return x * 2; }

template< class T >
inline typename boost::enable_if_c<
    boost::is_arithmetic<T>::value, T >::type
operator>>(T const x, sake::one_t)
{ return x / 2; }

template< class T >
inline typename boost::enable_if_c<
    boost::is_arithmetic<T>::value, T& >::type
operator<<=(T& x, sake::one_t)
{ return x *= 2; }

template< class T >
inline typename boost::enable_if_c<
    boost::is_arithmetic<T>::value, T& >::type
operator>>=(T& x, sake::one_t)
{ return x /= 2; }

#ifndef BOOST_NO_RVALUE_REFERENCES

template< class T >
inline typename sake::result_of::rcp<T>::type
operator/(sake::one_t, T&& x)
//{ return sake::rcp(sake::forward<T>(x)); }
{ return sake::functional::rcp()(sake::forward<T>(x)); }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

template< class T >
inline typename sake::result_of::rcp< T& >::type
operator/(sake::one_t, T& x)
//{ return sake::rcp(x); }
{ return sake::functional::rcp()(x); }

template< class T >
inline typename sake::result_of::rcp< T const & >::type
operator/(sake::one_t, T const & x)
//{ return sake::rcp(x); }
{ return sake::functional::rcp()(x); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

template< class T >
inline typename boost::enable_if_c<
    boost::is_integral<T>::value, bool >::type
operator&(T const x, sake::one_t)
{ return (x & 1) != 0; }

namespace one_private
{

template< class T >
inline T
as_dispatch(sake::int_tag<3>)
{ return T(sake::one); }

template< class T >
inline T
as_dispatch(sake::int_tag<2>)
{ return T((boost_ext::mpl::uint<1>())); }

template< class T >
inline T
as_dispatch(sake::int_tag<1>)
{ return T((boost::mpl::int_<1>())); }

template< class T >
inline T
as_dispatch(sake::int_tag<0>)
{ return T(1); }

template< class T >
inline T
as_impl()
{
    typedef typename boost_ext::mpl::
         if_<
        boost_ext::is_convertible< sake::one_t, T >,
        sake::int_tag<3>
    >::type::template
    else_if <
        boost_ext::is_convertible< boost_ext::mpl::uint<1>, T >,
        sake::int_tag<2>
    >::type::template
    else_if <
        boost_ext::is_convertible< boost::mpl::int_<1>, T >,
        sake::int_tag<1>
    >::type::template
    else_   <
        sake::int_tag<0>
    >::type int_tag_;
    return one_private::as_dispatch<T>(int_tag_());
}

} // namespace one_private

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_ONE_HPP

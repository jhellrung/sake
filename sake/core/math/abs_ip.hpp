/*******************************************************************************
 * sake/core/math/abs_ip.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * abs_ip(T& x) -> T&
 * struct functional::abs_ip
 *
 * Assigns its argument to its absolute value (in-place).
 *
 * sake::abs_ip(T&) is implemented in terms of
 * - std::[f]abs, if T is a builtin floating point or integral type; else
 * - T::abs_ip(), if available; else
 * - abs_ip(T&) (unqualified, hence subject to ADL), if available; else
 * - comparison with sake::zero and sake::negate_ip.
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_ABS_IP_HPP
#define SAKE_CORE_MATH_ABS_IP_HPP

#include <cmath>
#include <cstdlib>

#include <boost/config.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_signed.hpp>
#include <boost/type_traits/is_unsigned.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/is_lvalue_reference.hpp>

#include <sake/core/introspection/is_callable_function.hpp>
#include <sake/core/introspection/is_callable_member_function.hpp>
#include <sake/core/math/negate_ip.hpp>
#include <sake/core/math/zero.hpp>
#include <sake/core/utility/dispatch_priority_tag.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>
#include <sake/core/utility/workaround.hpp>

namespace sake
{

namespace abs_ip_private
{

template< class T >
inline T&
impl(T& x);

} // namespace abs_ip_private

namespace result_of
{

template< class T >
struct abs_ip
{
    BOOST_STATIC_ASSERT((boost_ext::is_lvalue_reference<T>::value));
    typedef T type;
};

} // namespace result_of

/*******************************************************************************
 * abs_ip(T& x) -> T&
 * struct functional::abs_ip
 ******************************************************************************/

namespace functional
{

struct abs_ip
{
    SAKE_RESULT_FROM_METAFUNCTION( result_of::abs_ip, 1 )

    template< class T >
    T&
    operator()(T& x) const
    { return abs_ip_private::impl(x); }

    int&
    operator()(int& x) const
    { return x = std::abs(x); }

    long&
    operator()(long& x) const
    { return x = std::abs(x); }

#ifndef BOOST_NO_LONG_LONG
    long long&
    operator()(long long& x) const
    {
#if SAKE_WORKAROUND_MSC_VERSION_LESS_EQUAL( 1500 )
        return x < 0 ? (x = -x) : x;
#else // #if SAKE_WORKAROUND_MSC_VERSION_LESS_EQUAL( 1500 )
        return x = std::abs(x);
#endif // #if SAKE_WORKAROUND_MSC_VERSION_LESS_EQUAL( 1500 )
    }
#endif // #ifndef BOOST_NO_LONG_LONG

    float&
    operator()(float& x) const
    { return x = std::fabs(x); }

    double&
    operator()(double& x) const
    { return x = std::fabs(x); }

    long double&
    operator()(long double& x) const
    { return x = std::fabs(x); }
};

} // namespace functional

functional::abs_ip const abs_ip = { };

} // namespace sake

namespace sake_abs_ip_private
{

#define SAKE_INTROSPECTION_TRAIT_NAME    is_callable
#define SAKE_INTROSPECTION_FUNCTION_NAME abs_ip
#define SAKE_INTROSPECTION_FUNCTION_ARITY_LIMITS ( 1, 1 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_FUNCTION()

template< class Result, class T >
inline Result
adl(T& x)
{ return abs_ip(x); }

} // namespace sake_abs_ip_private

namespace sake
{

namespace abs_ip_private
{

#define SAKE_INTROSPECTION_TRAIT_NAME           is_callable_mem_fun
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME abs_ip
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY_LIMITS ( 0, 0 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_MEMBER_FUNCTION()

template< class T >
inline typename boost::enable_if_c<
    boost::is_signed<T>::value,
    T&
>::type
dispatch(T& x, sake::dispatch_priority_tag<6>)
{ return x = std::abs(x); }

template< class T >
inline typename boost::enable_if_c<
    boost::is_unsigned<T>::value,
    T&
>::type
dispatch(T& x, sake::dispatch_priority_tag<5>)
{ return x; }

template< class T >
inline typename boost::enable_if_c<
    is_callable_mem_fun< T&, T& ( ) >::value,
    T&
>::type
dispatch(T& x, sake::dispatch_priority_tag<4>)
{ return x.abs_ip(); }

template< class T >
inline typename boost::enable_if_c< is_callable_mem_fun< T& >::value, T& >::type
dispatch(T& x, sake::dispatch_priority_tag<3>)
{ x.abs_ip(); return x; }

template< class T >
inline typename boost::enable_if_c<
    ::sake_abs_ip_private::is_callable< T& ( T& ) >::value,
    T&
>::type
dispatch(T& x, sake::dispatch_priority_tag<2>)
{ return ::sake_abs_ip_private::adl< T& >(x); }

template< class T >
inline typename boost::enable_if_c<
    ::sake_abs_ip_private::is_callable< void ( T& ) >::value,
    T&
>::type
dispatch(T& x, sake::dispatch_priority_tag<1>)
{ ::sake_abs_ip_private::adl< void >(x); return x; }

template< class T >
inline T&
dispatch(T& x, sake::dispatch_priority_tag<0>)
{ return x < sake::zero ? sake::negate_ip(x) : x; }

template< class T >
inline T&
impl(T& x)
{ return abs_ip_private::dispatch(x, sake::dispatch_priority_tag<6>()); }

} // namespace abs_ip_private

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_ABS_IP_HPP

/*******************************************************************************
 * sake/core/math/invert_ip.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * invert_ip(T& x) -> T&
 * struct functional::invert_ip
 *
 * Assigns its argument to its inverted value (multiplicative inverse)
 * (in-place).
 *
 * sake::invert_ip(T&) is implemented in terms of
 * - T::invert_ip(), if available; else
 * - invert_ip(T&) (unqualified, hence subject to ADL), if available; else
 * - assignment and inversion (via sake::invert).
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_NEGATE_IP_HPP
#define SAKE_CORE_MATH_NEGATE_IP_HPP

#include <boost/static_assert.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/is_lvalue_reference.hpp>

#include <sake/core/introspection/is_callable_function.hpp>
#include <sake/core/introspection/is_callable_member_function.hpp>
#include <sake/core/math/invert.hpp>
#include <sake/core/utility/dispatch_priority_tag.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>
#include <sake/core/utility/workaround.hpp>

namespace sake
{

namespace invert_ip_private
{

template< class T >
inline T&
impl(T& x);

} // namespace invert_ip_private

namespace result_of
{

template< class T >
struct invert_ip
{
    BOOST_STATIC_ASSERT((boost_ext::is_lvalue_reference<T>::value));
    typedef T type;
};

} // namespace result_of

/*******************************************************************************
 * invert_ip(T& x) -> T&
 * struct functional::invert_ip
 ******************************************************************************/

namespace functional
{

struct invert_ip
{
    SAKE_RESULT_FROM_METAFUNCTION( result_of::invert_ip, 1 )

    template< class T >
    T&
    operator()(T& x) const
    { return invert_ip_private::impl(x); }

    float&
    operator()(float& x) const
    { return x = 1/x; }
    double&
    operator()(double& x) const
    { return x = 1/x; }
    long double&
    operator()(long double& x) const
    { return x = 1/x; }
};

} // namespace functional

#ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
namespace invert_ip_adl_barrier
{ functional::invert_ip const invert_ip = { }; }
using namespace invert_ip_adl_barrier;
#else // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
functional::invert_ip const invert_ip = { };
#endif // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS

} // namespace sake

namespace sake_invert_ip_private
{

#define SAKE_INTROSPECTION_TRAIT_NAME    is_callable
#define SAKE_INTROSPECTION_FUNCTION_NAME invert_ip
#define SAKE_INTROSPECTION_FUNCTION_ARITY_LIMITS ( 1, 1 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_FUNCTION()

template< class Result, class T >
inline Result
adl(T& x)
{ return invert_ip(x); }

} // namespace sake_invert_ip_private

namespace sake
{

namespace invert_ip_private
{

#define SAKE_INTROSPECTION_TRAIT_NAME           is_callable_mem_fun
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME invert_ip
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY_LIMITS ( 0, 0 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_MEMBER_FUNCTION()

template< class T >
inline typename boost::enable_if_c<
    is_callable_mem_fun< T&, T& ( ) >::value,
    T&
>::type
dispatch(T& x, sake::dispatch_priority_tag<4>)
{ return x.invert_ip(); }

template< class T >
inline typename boost::enable_if_c< is_callable_mem_fun< T& >::value, T& >::type
dispatch(T& x, sake::dispatch_priority_tag<3>)
{ x.invert_ip(); return x; }

template< class T >
inline typename boost::enable_if_c<
    ::sake_invert_ip_private::is_callable< T& ( T& ) >::value,
    T&
>::type
dispatch(T& x, sake::dispatch_priority_tag<2>)
{ return ::sake_invert_ip_private::adl< T& >(x); }

template< class T >
inline typename boost::enable_if_c<
    ::sake_invert_ip_private::is_callable< void ( T& ) >::value,
    T&
>::type
dispatch(T& x, sake::dispatch_priority_tag<1>)
{ ::sake_invert_ip_private::adl< void >(x); return x; }

template< class T >
inline T&
dispatch(T& x, sake::dispatch_priority_tag<0>)
{ return x = sake::invert(x); }

template< class T >
inline T&
impl(T& x)
{ return invert_ip_private::dispatch(x, sake::dispatch_priority_tag<4>()); }

} // namespace invert_ip_private

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_NEGATE_IP_HPP

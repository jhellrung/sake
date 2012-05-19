/*******************************************************************************
 * sake/core/math/neg_ip.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * neg_ip(T& x) -> T&
 * struct functional::neg_ip
 *
 * Assigns its argument to its negated value (in-place).
 *
 * sake::neg_ip(T&) is implemented in terms of
 * - T::neg_ip(), if available; else
 * - neg_ip(T&) (unqualified, hence subject to ADL), if available; else
 * - assignment and negation (via unary operator-).
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_NEG_IP_HPP
#define SAKE_CORE_MATH_NEG_IP_HPP

#include <boost/static_assert.hpp>

#include <sake/boost_ext/mpl/if.hpp>
#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/is_lvalue_reference_to_nonconst.hpp>

#include <sake/core/config.hpp>
#include <sake/core/introspection/is_callable_function.hpp>
#include <sake/core/introspection/is_callable_member_function.hpp>
#include <sake/core/utility/int_tag.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>

namespace sake
{

namespace neg_ip_private
{

template< class T >
inline T&
impl(T& x);

} // namespace neg_ip_private

namespace result_of
{

template< class T >
struct neg_ip
{
    BOOST_STATIC_ASSERT((boost_ext::is_lvalue_reference_to_nonconst<T>::value));
    typedef T type;
};

} // namespace result_of

/*******************************************************************************
 * neg_ip(T& x) -> T&
 * struct functional::neg_ip
 ******************************************************************************/

namespace functional
{

struct neg_ip
{
    SAKE_RESULT_FROM_METAFUNCTION( sake::result_of::neg_ip, 1 )

    template< class T >
    T&
    operator()(T& x) const
    { return neg_ip_private::impl(x); }
};

} // namespace functional

#ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
namespace neg_ip_adl_barrier
{ sake::functional::neg_ip const neg_ip = { }; }
using namespace neg_ip_adl_barrier;
#else // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
sake::functional::neg_ip const neg_ip = { };
#endif // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS

} // namespace sake

namespace sake_neg_ip_private
{

#define SAKE_INTROSPECTION_TRAIT_NAME    is_callable
#define SAKE_INTROSPECTION_FUNCTION_NAME neg_ip
#define SAKE_INTROSPECTION_FUNCTION_ARITY_LIMITS ( 1, 1 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_FUNCTION()

template< class Result, class T >
inline Result
adl(T& x)
{ return static_cast< Result >(neg_ip(x)); }

} // namespace sake_neg_ip_private

namespace sake
{

namespace neg_ip_private
{

#define SAKE_INTROSPECTION_TRAIT_NAME           is_callable_mem_fun
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME neg_ip
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY_LIMITS ( 0, 0 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_MEMBER_FUNCTION()

template< class T >
inline T&
dispatch(T& x, sake::int_tag<4>)
{ return x.neg_ip(); }

template< class T >
inline T&
dispatch(T& x, sake::int_tag<3>)
{ x.neg_ip(); return x; }

template< class T >
inline T&
dispatch(T& x, sake::int_tag<2>)
{ return ::sake_neg_ip_private::adl< T& >(x); }

template< class T >
inline T&
dispatch(T& x, sake::int_tag<1>)
{ ::sake_neg_ip_private::adl< void >(x); return x; }

template< class T >
inline T&
dispatch(T& x, sake::int_tag<0>)
{ return x = -x; }

template< class T >
inline T&
impl(T& x)
{
    typedef typename boost_ext::mpl::
         if_<
        neg_ip_private::is_callable_mem_fun< T&, T& ( ) >,
        sake::int_tag<4>
    >::type::template
    else_if <
        neg_ip_private::is_callable_mem_fun< T& >,
        sake::int_tag<3>
    >::type::template
    else_if <
        ::sake_neg_ip_private::is_callable< T& ( T& ) >,
        sake::int_tag<2>
    >::type::template
    else_if <
        ::sake_neg_ip_private::is_callable< void ( T& ) >,
        sake::int_tag<1>
    >::type::template
    else_   <
        sake::int_tag<0>
    >::type int_tag_;
    return neg_ip_private::dispatch(x, int_tag_());
}

} // namespace neg_ip_private

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_NEG_IP_HPP

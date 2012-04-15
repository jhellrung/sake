/*******************************************************************************
 * sake/core/math/inv_ip.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * inv_ip(T& x) -> T&
 * struct functional::inv_ip
 *
 * Assigns its argument to its inverted value (multiplicative inverse)
 * (in-place).
 *
 * sake::inv_ip(T&) is implemented in terms of
 * - T::inv_ip(), if available; else
 * - inv_ip(T&) (unqualified, hence subject to ADL), if available; else
 * - assignment and inversion (via sake::inv).
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_INV_IP_HPP
#define SAKE_CORE_MATH_INV_IP_HPP

#include <boost/static_assert.hpp>

#include <sake/boost_ext/mpl/if.hpp>
#include <sake/boost_ext/mpl/uint.hpp>
#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/is_lvalue_reference_to_nonconst.hpp>

#include <sake/core/math/inv.hpp>
#include <sake/core/math/private/inv_common.hpp>
#include <sake/core/utility/int_tag.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>
#include <sake/core/utility/workaround.hpp>

namespace sake
{

namespace inv_ip_private
{

template< class T >
inline T&
impl(T& x);

} // namespace inv_ip_private

namespace result_of
{

template< class T >
struct inv_ip
{
    BOOST_STATIC_ASSERT((boost_ext::is_lvalue_reference_to_nonconst<T>::value));
    typedef T type;
};

} // namespace result_of

/*******************************************************************************
 * inv_ip(T& x) -> T&
 * struct functional::inv_ip
 ******************************************************************************/

namespace functional
{

struct inv_ip
{
    SAKE_RESULT_FROM_METAFUNCTION( sake::result_of::inv_ip, 1 )

    template< class T >
    T&
    operator()(T& x) const
    { return inv_ip_private::impl(x); }

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
namespace inv_ip_adl_barrier
{ sake::functional::inv_ip const inv_ip = { }; }
using namespace inv_ip_adl_barrier;
#else // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
sake::functional::inv_ip const inv_ip = { };
#endif // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS

} // namespace sake

namespace sake
{

namespace inv_ip_private
{

template< class T >
struct dispatch_index
    : boost_ext::mpl::
           if_< inv_ip_private::is_callable_mem_fun< T&, T& ( ) >, sake::int_tag<4> >::type::template
      else_if < inv_ip_private::is_callable_mem_fun< T&         >, sake::int_tag<3> >::type::template
      else_if < ::sake_inv_ip_private::is_callable<   T& ( T& ) >, sake::int_tag<2> >::type::template
      else_if < ::sake_inv_ip_private::is_callable< void ( T& ) >, sake::int_tag<1> >::type::template
      else_   < sake::int_tag<0> >
{ };

template< class T >
inline T&
dispatch(T& x, sake::int_tag<4>)
{ return x.inv_ip(); }

template< class T >
inline T&
dispatch(T& x, sake::int_tag<3>)
{ x.inv_ip(); return x; }

template< class T >
inline T&
dispatch(T& x, sake::int_tag<2>)
{ return ::sake_inv_ip_private::adl< T& >(x); }

template< class T >
inline T&
dispatch(T& x, sake::int_tag<1>)
{ ::sake_inv_ip_private::adl< void >(x); return x; }

template< class T >
inline T&
dispatch(T& x, sake::int_tag<0>)
{ return x = sake::inv(x); }

template< class T >
inline T&
impl(T& x)
{
    typedef typename boost_ext::mpl::
         if_<
        inv_ip_private::is_callable_mem_fun< T&, T& ( ) >,
        sake::int_tag<4>
    >::type::template
    else_if <
        inv_ip_private::is_callable_mem_fun< T& >,
        sake::int_tag<3>
    >::type::template
    else_if <
        ::sake_inv_ip_private::is_callable< T& ( T& ) >,
        sake::int_tag<2>
    >::type::template
    else_if <
        ::sake_inv_ip_private::is_callable< void ( T& ) >,
        sake::int_tag<1>
    >::type::template
    else_   <
        sake::int_tag<0>
    >::type int_tag_;
    return inv_ip_private::dispatch(x, int_tag_());
}

} // namespace inv_ip_private

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_INV_IP_HPP

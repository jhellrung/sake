/*******************************************************************************
 * sake/core/math/rcp_ip.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * rcp_ip(T& x) -> void
 * struct functional::rcp_ip
 *
 * Assigns its argument to its reciprocal value (multiplicative inverse)
 * (in-place).
 *
 * sake::rcp_ip(T&) is implemented in terms of
 * - T::rcp_ip(), if available; else
 * - rcp_ip(T&) (unqualified, hence subject to ADL), if available; else
 * - assignment and sake::rcp.
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_RCP_IP_HPP
#define SAKE_CORE_MATH_RCP_IP_HPP

#include <sake/boost_ext/mpl/if.hpp>

#include <sake/core/config.hpp>
#include <sake/core/math/rcp.hpp>
#include <sake/core/math/private/rcp_common.hpp>
#include <sake/core/move/move.hpp>
#include <sake/core/utility/int_tag.hpp>

namespace sake
{

namespace rcp_ip_private
{

template< class T >
inline void
impl(T& x);

} // namespace rcp_ip_private

/*******************************************************************************
 * rcp_ip(T& x) -> void
 * struct functional::rcp_ip
 ******************************************************************************/

namespace functional
{

struct rcp_ip
{
    typedef void result_type;

    template< class T >
    void operator()(T& x) const
    { rcp_ip_private::impl(x); }

    void operator()(float& x) const
    { x = 1/x; }
    void operator()(double& x) const
    { x = 1/x; }
    void operator()(long double& x) const
    { x = 1/x; }
};

} // namespace functional

#ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
namespace rcp_ip_adl_barrier
{ sake::functional::rcp_ip const rcp_ip = { }; }
using namespace rcp_ip_adl_barrier;
#else // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
sake::functional::rcp_ip const rcp_ip = { };
#endif // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS

} // namespace sake

namespace sake
{

namespace rcp_ip_private
{

template< class T >
inline void
dispatch(T& x, sake::int_tag<2>)
{ x.rcp_ip(); }

template< class T >
inline void
dispatch(T& x, sake::int_tag<1>)
{ ::sake_rcp_ip_private::adl(x); }

template< class T >
inline void
dispatch(T& x, sake::int_tag<0>)
{ x = sake::rcp(sake::move(x)); }

template< class T >
inline void
impl(T& x)
{
    typedef typename boost_ext::mpl::
         if_< rcp_ip_private::is_callable_mem_fun< T&, void ( ) >,
              sake::int_tag<2> >::type::template
    else_if < ::sake_rcp_ip_private::is_callable< void ( T& ) >,
              sake::int_tag<1> >::type::template
    else_   < sake::int_tag<0> >::type int_tag_;
    rcp_ip_private::dispatch(x, int_tag_());
}

} // namespace rcp_ip_private

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_RCP_IP_HPP

/*******************************************************************************
 * sake/core/math/sqr_ip.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * sqr_ip(T& x) -> void
 * struct functional::sqr_ip
 *
 * Assigns its argument to its squared value (in-place).
 *
 * sake::sqr_ip(T&) is implemented in terms of
 * - T::sqr_ip(), if available; else
 * - sqr_ip(T&) (unqualified, hence subject to ADL), if available; else
 * - T::operator*=.
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_SQR_IP_HPP
#define SAKE_CORE_MATH_SQR_IP_HPP

#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/if.hpp>

#include <sake/core/config.hpp>
#include <sake/core/math/private/sqr_common.hpp>
#include <sake/core/utility/int_tag.hpp>

namespace sake
{

namespace sqr_ip_private
{

template< class T >
inline void
impl(T& x);

} // namespace sqr_ip_private

/*******************************************************************************
 * sqr_ip(T& x) -> void
 * struct functional::sqr_ip
 ******************************************************************************/

namespace functional
{

struct sqr_ip
{
    typedef void result_type;

    template< class T >
    void operator()(T& x) const
    { sqr_ip_private::impl(x); }
};

} // namespace functional

#ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
namespace sqr_ip_adl_barrier
{ sake::functional::sqr_ip const sqr_ip = { }; }
using namespace sqr_ip_adl_barrier;
#else // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
sake::functional::sqr_ip const sqr_ip = { };
#endif // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS

} // namespace sake

namespace sake
{

namespace sqr_ip_private
{

template< class T >
inline void
dispatch(T& x, sake::int_tag<2>)
{ x.sqr_ip(); }

template< class T >
inline void
dispatch(T& x, sake::int_tag<1>)
{ ::sake_sqr_ip_private::adl(x); }

template< class T >
inline void
dispatch(T& x, sake::int_tag<0>)
{ x *= x; }

template< class T >
inline void
impl(T& x)
{
    typedef typename boost_ext::mpl::
         if_< sqr_ip_private::is_callable_mem_fun< T&, void ( ) >,
              sake::int_tag<2> >::type::template
    else_if < ::sake_sqr_ip_private::is_callable< void ( T& ) >,
              sake::int_tag<1> >::type::template
    else_   < sake::int_tag<0> >::type int_tag_;
    sqr_ip_private::dispatch(x, int_tag_());
}

} // namespace sqr_ip_private

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_SQR_IP_HPP

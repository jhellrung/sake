/*******************************************************************************
 * sake/core/math/abs_ip.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * abs_ip(T& x) -> void
 * struct functional::abs_ip
 *
 * Assigns its argument to its absolute value (in-place).
 *
 * sake::abs_ip(T&) is implemented in terms of
 * - std::[f]abs, if T is a builtin floating point or integral type; else
 * - T::abs_ip(), if available; else
 * - abs_ip(T&) (unqualified, hence subject to ADL), if available; else
 * - comparison with sake::zero and sake::neg_ip.
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_ABS_IP_HPP
#define SAKE_CORE_MATH_ABS_IP_HPP

#include <cmath>
#include <cstdlib>

#include <boost/config.hpp>
#include <boost/type_traits/is_signed.hpp>
#include <boost/type_traits/is_unsigned.hpp>

#include <sake/boost_ext/mpl/if.hpp>

#include <sake/core/config.hpp>
#include <sake/core/math/neg_ip.hpp>
#include <sake/core/math/private/abs_common.hpp>
#include <sake/core/math/zero.hpp>
#include <sake/core/utility/int_tag.hpp>

namespace sake
{

namespace abs_ip_private
{

template< class T >
inline void
impl(T& x);

} // namespace abs_ip_private

/*******************************************************************************
 * abs_ip(T& x) -> void
 * struct functional::abs_ip
 ******************************************************************************/

namespace functional
{

struct abs_ip
{
    typedef void result_type;

    template< class T >
    void operator()(T& x) const
    { abs_ip_private::impl(x); }

    void operator()(int& x) const
    { x = std::abs(x); }

    void operator()(long& x) const
    { x = std::abs(x); }

#ifndef BOOST_NO_LONG_LONG
    void operator()(long long& x) const
    {
#if SAKE_MSC_VERSION && SAKE_MSC_VERSION <= 1500
        if(x < 0)
            x = -x;
#else // #if SAKE_MSC_VERSION && SAKE_MSC_VERSION <= 1500
        x = std::abs(x);
#endif // #if SAKE_MSC_VERSION && SAKE_MSC_VERSION <= 1500
    }
#endif // #ifndef BOOST_NO_LONG_LONG

    void operator()(float& x) const
    { x = std::fabs(x); }

    void operator()(double& x) const
    { x = std::fabs(x); }

    void operator()(long double& x) const
    { x = std::fabs(x); }
};

} // namespace functional

#ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
namespace abs_ip_adl_barrier
{ sake::functional::abs_ip const abs_ip = { }; }
using namespace abs_ip_adl_barrier;
#else // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
sake::functional::abs_ip const abs_ip = { };
#endif // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS

} // namespace sake

namespace sake
{

namespace abs_ip_private
{

template< class T >
inline void
dispatch(T& x, sake::int_tag<4>)
{ x = std::abs(x); }

template< class T >
inline void
dispatch(T& /*x*/, sake::int_tag<3>)
{ }

template< class T >
inline void
dispatch(T& x, sake::int_tag<2>)
{ x.abs_ip(); }

template< class T >
inline void
dispatch(T& x, sake::int_tag<1>)
{ ::sake_abs_ip_private::adl(x); }

template< class T >
inline void
dispatch(T& x, sake::int_tag<0>)
{
    if(x < sake::zero)
        sake::neg_ip(x);
}

template< class T >
inline void
impl(T& x)
{
    typedef typename boost_ext::mpl::
         if_< boost::is_signed<T>,
              sake::int_tag<4> >::type::template
    else_if < boost::is_unsigned<T>,
              sake::int_tag<3> >::type::template
    else_if < abs_ip_private::is_callable_mem_fun< T&, void ( ) >,
              sake::int_tag<2> >::type::template
    else_if < ::sake_abs_ip_private::is_callable< void ( T& ) >,
              sake::int_tag<1> >::type::template
    else_   < sake::int_tag<0> >::type int_tag_;
    abs_ip_private::dispatch(x, int_tag_());
}

} // namespace abs_ip_private

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_ABS_IP_HPP

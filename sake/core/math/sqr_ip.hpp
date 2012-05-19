/*******************************************************************************
 * sake/core/math/sqr_ip.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * sqr_ip(T& x) -> T&
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

#include <boost/static_assert.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/if.hpp>
#include <sake/boost_ext/type_traits/is_lvalue_reference_to_nonconst.hpp>

#include <sake/core/config.hpp>
#include <sake/core/math/private/sqr_common.hpp>
#include <sake/core/utility/int_tag.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>

namespace sake
{

namespace sqr_ip_private
{

template< class T >
inline T&
impl(T& x);

} // namespace sqr_ip_private

namespace result_of
{

template< class T >
struct sqr_ip
{
    BOOST_STATIC_ASSERT((boost_ext::is_lvalue_reference_to_nonconst<T>::value));
    typedef T type;
};

} // namespace result_of

/*******************************************************************************
 * sqr_ip(T& x) -> T&
 * struct functional::sqr_ip
 ******************************************************************************/

namespace functional
{

struct sqr_ip
{
    SAKE_RESULT_FROM_METAFUNCTION( sake::result_of::sqr_ip, 1 )

    template< class T >
    T&
    operator()(T& x) const
    { return sqr_ip_private::impl(x); }
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
inline T&
dispatch(T& x, sake::int_tag<4>)
{ return x.sqr_ip(); }

template< class T >
inline T&
dispatch(T& x, sake::int_tag<3>)
{ x.sqr_ip(); return x; }

template< class T >
inline T&
dispatch(T& x, sake::int_tag<2>)
{ return ::sake_sqr_ip_private::adl< T& >(x); }

template< class T >
inline T&
dispatch(T& x, sake::int_tag<1>)
{ ::sake_sqr_ip_private::adl< void >(x); return x; }

template< class T >
inline T&
dispatch(T& x, sake::int_tag<0>)
{ return x *= x; }

template< class T >
inline T&
impl(T& x)
{
    typedef typename boost_ext::mpl::
         if_< sqr_ip_private::is_callable_mem_fun< T&, T& ( ) >, sake::int_tag<4> >::type::template
    else_if < sqr_ip_private::is_callable_mem_fun< T&         >, sake::int_tag<3> >::type::template
    else_if < ::sake_sqr_ip_private::is_callable<   T& ( T& ) >, sake::int_tag<2> >::type::template
    else_if < ::sake_sqr_ip_private::is_callable< void ( T& ) >, sake::int_tag<1> >::type::template
    else_   < sake::int_tag<0> >::type int_tag_;
    return sqr_ip_private::dispatch(x, int_tag_());
}

} // namespace sqr_ip_private

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_SQR_IP_HPP

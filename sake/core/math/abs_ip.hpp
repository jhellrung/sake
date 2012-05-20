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
 * - comparison with sake::zero and sake::neg_ip.
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_ABS_IP_HPP
#define SAKE_CORE_MATH_ABS_IP_HPP

#include <cmath>
#include <cstdlib>

#include <boost/config.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_signed.hpp>
#include <boost/type_traits/is_unsigned.hpp>

#include <sake/boost_ext/mpl/if.hpp>
#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/is_lvalue_reference_to_nonconst.hpp>

#include <sake/core/config.hpp>
#include <sake/core/math/neg_ip.hpp>
#include <sake/core/math/private/abs_common.hpp>
#include <sake/core/math/zero.hpp>
#include <sake/core/utility/int_tag.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>

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
    BOOST_STATIC_ASSERT((boost_ext::is_lvalue_reference_to_nonconst<T>::value));
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
    SAKE_RESULT_FROM_METAFUNCTION( sake::result_of::abs_ip, 1 )

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
#if SAKE_MSC_VERSION && SAKE_MSC_VERSION <= 1500
        return x < 0 ? (x = -x) : x;
#else // #if SAKE_MSC_VERSION && SAKE_MSC_VERSION <= 1500
        return x = std::abs(x);
#endif // #if SAKE_MSC_VERSION && SAKE_MSC_VERSION <= 1500
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
inline T&
dispatch(T& x, sake::int_tag<6>)
{ return x = std::abs(x); }

template< class T >
inline T&
dispatch(T& x, sake::int_tag<5>)
{ return x; }

template< class T >
inline T&
dispatch(T& x, sake::int_tag<4>)
{ return x.abs_ip(); }

template< class T >
inline T&
dispatch(T& x, sake::int_tag<3>)
{ x.abs_ip(); return x; }

template< class T >
inline T&
dispatch(T& x, sake::int_tag<2>)
{ return ::sake_abs_ip_private::adl< T& >(x); }

template< class T >
inline T&
dispatch(T& x, sake::int_tag<1>)
{ ::sake_abs_ip_private::adl< void >(x); return x; }

template< class T >
inline T&
dispatch(T& x, sake::int_tag<0>)
{ return x < sake::zero ? sake::neg_ip(x) : x; }

template< class T >
inline T&
impl(T& x)
{
    typedef typename boost_ext::mpl::
         if_<
        boost::is_signed<T>,
        sake::int_tag<6>
    >::type::template
    else_if <
        boost::is_unsigned<T>,
        sake::int_tag<5>
    >::type::template
    else_if <
        abs_ip_private::is_callable_mem_fun< T&, T& ( ) >,
        sake::int_tag<4>
    >::type::template
    else_if <
        abs_ip_private::is_callable_mem_fun< T& >,
        sake::int_tag<3>
    >::type::template
    else_if <
        ::sake_abs_ip_private::is_callable< T& ( T& ) >,
        sake::int_tag<2>
    >::type::template
    else_if <
        ::sake_abs_ip_private::is_callable< void ( T& ) >,
        sake::int_tag<1>
    >::type::template
    else_   <
        sake::int_tag<0>
    >::type int_tag_;
    return abs_ip_private::dispatch(x, int_tag_());
}

} // namespace abs_ip_private

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_ABS_IP_HPP

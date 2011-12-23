/*******************************************************************************
 * sake/core/range/begin.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * range::begin(R&& r)
 *     -> range_forward_cursor<R>::type
 * struct functional::swap
 *
 * Swaps its arguments.
 *
 * sake::swap(T0&, T1&) is implemented in terms of
 * - T0::swap(T1&), if available; else
 * - T1::swap(T0&), if available; else
 * - swap(T0&, T1&) (unqualified, hence subject to ADL), if available; else
 * - swap(T1&, T0&) (unqualified, hence subject to ADL), if available; else
 * - move construction and move assignment via sake::move.
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_SWAP_HPP
#define SAKE_CORE_UTILITY_SWAP_HPP

#include <cstddef>

#include <boost/static_assert.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/core/introspection/is_callable_function.hpp>
#include <sake/core/introspection/is_callable_member_function.hpp>
#include <sake/core/move/move.hpp>
#include <sake/core/utility/dispatch_priority_tag.hpp>
#include <sake/core/utility/workaround.hpp>

namespace sake
{

namespace swap_private
{

template< class T0, class T1 >
inline void
impl(T0& x0, T1& x1);

} // namespace swap_private

/*******************************************************************************
 * swap(T0& x0, T1& x1) -> void
 * struct functional::swap
 ******************************************************************************/

namespace functional
{

struct swap
{
    typedef void result_type;

    template< class T0, class T1 >
    void operator()(T0& x0, T1& x1) const
    { swap_private::impl(x0, x1); }

    template< class T0, std::size_t N0, class T1, std::size_t N1 >
    void operator()(T0 (&a0)[N0], T1 (&a1)[N1])
    {
        BOOST_STATIC_ASSERT((N0 == N1));
        for(std::size_t i = 0; i != N0; ++i)
            operator()(a0[i], a1[i]);
    }
};

} // namespace functional

#ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
namespace swap_adl_barrier
{ functional::swap const swap = { }; }
using namespace swap_adl_barrier;
#else // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
functional::swap const swap = { };
#endif // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS

} // namespace sake

namespace sake_swap_private
{

#define SAKE_INTROSPECTION_TRAIT_NAME    is_callable
#define SAKE_INTROSPECTION_FUNCTION_NAME swap
#define SAKE_INTROSPECTION_FUNCTION_ARITY_LIMITS ( 2, 2 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_FUNCTION()

template< class T0, class T1 >
inline void
adl(T0& x0, T1& x1)
{ swap(x0, x1); }

} // namespace sake_swap_private

namespace sake
{

namespace swap_private
{

#define SAKE_INTROSPECTION_TRAIT_NAME           is_callable_mem_fun
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME swap
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY_LIMITS ( 1, 1 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_MEMBER_FUNCTION()

template< class T0, class T1 >
typename boost::enable_if_c<
    is_callable_mem_fun< T0&, void ( T1& ) >::value
>::type
dispatch(T0& x0, T1& x1, sake::dispatch_priority_tag<4>)
{ x0.swap(x1); }

template< class T0, class T1 >
typename boost::enable_if_c<
    is_callable_mem_fun< T1&, void ( T0& ) >::value
>::type
dispatch(T0& x0, T1& x1, sake::dispatch_priority_tag<3>)
{ x1.swap(x0); }

template< class T0, class T1 >
typename boost::enable_if_c<
    ::sake_swap_private::is_callable< void ( T0&, T1& ) >::value
>::type
dispatch(T0& x0, T1& x1, sake::dispatch_priority_tag<2>)
{ ::sake_swap_private::adl(x0, x1); }

template< class T0, class T1 >
typename boost::enable_if_c<
    ::sake_swap_private::is_callable< void ( T1&, T0& ) >::value
>::type
dispatch(T0& x0, T1& x1, sake::dispatch_priority_tag<1>)
{ ::sake_swap_private::adl(x1, x0); }

template< class T0, class T1 >
inline void
dispatch(T0& x0, T1& x1, sake::dispatch_priority_tag<0>)
{
    T0 temp(sake::move(x0));
    x0 = sake::move(x1);
    x1 = sake::move(temp);
}

template< class T0, class T1 >
inline void
impl(T0& x0, T1& x1)
{ swap_private::dispatch(x0, x1, sake::dispatch_priority_tag<4>()); }

} // namespace swap_private

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_SWAP_HPP

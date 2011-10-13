/*******************************************************************************
 * sake/core/utility/swap.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * swap(T0& x0, T1& x1) -> void
 * struct functional::swap
 *
 * swap swaps its arguments.  It automatically uses ADL to find overloads of
 * swap, falling back to a default implementation if ADL fails.
 *
 * The default implementation of swap(T0& x0, T1& x1)
 * - forwards to T0::swap, if available; else
 * - forwards to T1::swap, if available; else
 * - implements swap in terms of move construction and move assignment.
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_SWAP_HPP
#define SAKE_CORE_UTILITY_SWAP_HPP

#include <cstddef>

#include <boost/static_assert.hpp>

#include <sake/boost_ext/type_traits/add_reference.hpp>

#include <sake/core/introspection/is_callable_function.hpp>
#include <sake/core/introspection/is_callable_member_function.hpp>
#include <sake/core/move/move.hpp>

namespace sake
{

namespace swap_private
{

template< class T0, class T1 >
struct impl;

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
    { swap_private::impl< T0, T1 >::apply(x0, x1); }

    template< class T0, std::size_t N0, class T1, std::size_t N1 >
    void operator()(T0 (&a0)[N0], T1 (&a1)[N1])
    {
        BOOST_STATIC_ASSERT((N0 == N1));
        for(std::size_t i = 0; i != N0; ++i)
            operator()(a0[i], a1[i]);
    }
};

} // namespace functional

functional::swap const swap = { };

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

template<
    class T0, class T1,
    bool = ::sake_swap_private::is_callable< void ( T0&, T1& ) >::value
>
struct dispatch_on_is_callable;

template<
    class T0, class T1,
    bool = is_callable_mem_fun< T0&, void ( T1& ) >::value
>
struct dispatch_on_is_callable_mem_fun01;

template<
    class T0, class T1,
    bool = is_callable_mem_fun< T1&, void ( T0& ) >::value
>
struct dispatch_on_is_callable_mem_fun10;

template< class T0, class T1 >
struct impl
    : dispatch_on_is_callable< T0, T1 >
{ };

template< class T0, class T1 >
struct dispatch_on_is_callable< T0, T1, true >
{
    static void apply(T0& x0, T1& x1)
    { ::sake_swap_private::adl(x0, x1); }
};

template< class T0, class T1 >
struct dispatch_on_is_callable< T0, T1, false >
    : dispatch_on_is_callable_mem_fun01< T0, T1 >
{ };

template< class T0, class T1 >
struct dispatch_on_is_callable_mem_fun01< T0, T1, true >
{
    static void apply(T0& x0, T1& x1)
    { x0.swap(x1); }
};

template< class T0, class T1 >
struct dispatch_on_is_callable_mem_fun01< T0, T1, false >
    : dispatch_on_is_callable_mem_fun10< T0, T1 >
{ };

template< class T0, class T1 >
struct dispatch_on_is_callable_mem_fun10< T0, T1, true >
{
    static void apply(T0& x0, T1& x1)
    { x1.swap(x0); }
};

template< class T0, class T1 >
struct dispatch_on_is_callable_mem_fun10< T0, T1, false >
{
    static void apply(T0& x0, T1& x1)
    {
        T0 temp(sake::move(x0));
        x0 = sake::move(x1);
        x1 = sake::move(temp);
    }
};

} // namespace swap_private

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_SWAP_HPP

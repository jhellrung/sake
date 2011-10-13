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
 * swap swaps its arguments, with the default implementation using move
 * construction and assignment.  It should be semantically equivalent to
 * boost::swap or an qualified call to swap.
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_SWAP_HPP
#define SAKE_CORE_UTILITY_SWAP_HPP

#include <cstddef>

#include <boost/static_assert.hpp>

#include <sake/core/introspection/is_callable_function.hpp>
#include <sake/core/move/move.hpp>

namespace sake_swap_private
{

#define SAKE_INTROSPECTION_TRAIT_NAME    is_callable
#define SAKE_INTROSPECTION_FUNCTION_NAME swap
#define SAKE_INTROSPECTION_FUNCTION_ARITY_LIMITS ( 2, 2 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_FUNCTION()

} // namespace sake_swap_private

namespace sake
{

namespace swap_private
{

template<
    class T0, class T1,
    bool = sake_swap_private::is_callable< void ( T0&, T1& ) >::value
>
struct dispatch;

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
    { sake::swap_private::dispatch< T0, T1 >::apply(x0, x1); }

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

template< class T0, class T1 >
inline void
adl(T0& x0, T1& x1)
{ swap(x0, x1); }

} // namespace sake_swap_private

namespace sake
{

namespace swap_private
{

template< class T0, class T1 >
struct dispatch< T0, T1, false >
{
    static void apply(T0& x0, T1& x1)
    {
        T0 temp(sake::move(x0));
        x0 = sake::move(x1);
        x1 = sake::move(temp);
    }
};

template< class T0, class T1 >
struct dispatch< T0, T1, true >
{
    static void apply(T0& x0, T1& x1)
    { sake_swap_private::adl(x0, x1); }
};

} // namespace swap_private

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_SWAP_HPP

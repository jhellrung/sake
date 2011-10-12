/*******************************************************************************
 * sake/core/utility/swap.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * swap(T0& x0, T1& x1) -> void
 * default_impl::swap(T& x0, ... x1) -> void
 * struct functional::swap
 *
 * swap swaps its arguments.  It should be semantically equivalent to
 * boost::swap or an qualified call to swap.
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_SWAP_HPP
#define SAKE_CORE_UTILITY_SWAP_HPP

#include <cstddef>

#include <boost/static_assert.hpp>

#include <sake/core/move/move.hpp>

namespace sake
{

namespace default_impl
{

namespace swap_private
{

// We use this wrapper template to make the following swap overload less
// preferable to std::swap and boost::swap.
template< class T >
struct wrapper
{
    typedef wrapper type;
    T& x;
    wrapper(T& x_) : x(x_) { }
};

} // namespace swap_private

template< class T >
inline void
swap(T& x0, typename swap_private::wrapper<T>::type const w1)
{
    T temp(sake::move(x0));
    x0 = sake::move(w1.x);
    w1.x = sake::move(temp);
}

} // namespace default_impl

} // namespace sake

namespace sake_swap_private
{

template< class T0, class T1 >
inline void
impl(T0& x0, T1& x1)
{
    using ::sake::default_impl::swap;
    swap(x0, x1);
}

template< class T0, std::size_t N0, class T1, std::size_t N1 >
void impl(T0 (&x0)[N0], T1 (&x1)[N1])
{
    BOOST_STATIC_ASSERT((N0 == N1));
    for(std::size_t i = 0; i != N0; ++i)
        ::sake_swap_private::impl(x0[i], x1[i]);
}

} // namespace sake_swap_private

namespace sake
{

namespace functional
{

struct swap
{
    typedef void result_type;

    template< class T0, class T1 >
    void operator()(T0& x0, T1& x1) const
    { ::sake_swap_private::impl(x0, x1); }
};

} // namespace functional

functional::swap const swap = { };

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_SWAP_HPP

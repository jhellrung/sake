/*******************************************************************************
 * sake/core/math/static_clamp.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct static_clamp<N>
 * struct static_clamp_c<N>
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_STATIC_CLAMP_HPP
#define SAKE_CORE_MATH_STATIC_CLAMP_HPP

#include <boost/static_assert.hpp>

#include <sake/core/math/unbounded_tag.hpp>

namespace sake
{

template< int L, int N, int U >
struct static_clamp_c
{
    BOOST_STATIC_ASSERT(!(U < L));
    static unsigned int const value = N < L ? L : U < N ? U : N;
    typedef static_clamp_c type;
};

template< class L, class N, class U >
struct static_clamp
    : static_clamp_c< L::value, N::value, U::value >
{ };

template< class L, class N >
struct static_clamp< L, N, sake::unbounded_tag >
    : static_clamp_c< L::value, N::value, N::value >
{ };

template< class N, class U >
struct static_clamp< sake::unbounded_tag, N, U >
    : static_clamp_c< N::value, N::value, U::value >
{ };

template< class N >
struct static_clamp< sake::unbounded_tag, N, sake::unbounded_tag >
    : static_clamp_c< N::value, N::value, N::value >
{ };

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_STATIC_CLAMP_HPP

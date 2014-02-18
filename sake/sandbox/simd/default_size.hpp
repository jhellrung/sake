/*******************************************************************************
 * sake/core/math/simd/default_size.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_SIMD_DEFAULT_SIZE_HPP
#define SAKE_CORE_MATH_SIMD_DEFAULT_SIZE_HPP

#include <cstddef>

#include <sake/core/config.hpp>

namespace sake
{

namespace simd
{

template< class T >
struct default_size
{
    static std::size_t const value = 1;
    typedef default_size type;
};

template<>
struct default_size< float >
{
#if   defined( SAKE_SIMD_AVX )
    static std::size_t const value = 8;
#elif defined( SAKE_SIMD_SSE )
    static std::size_t const value = 4;
#else
    static std::size_t const value = 1;
#endif
    typedef default_size type;
};

template<>
struct default_size< double >
{
#if   defined( SAKE_SIMD_AVX )
    static std::size_t const value = 4;
#elif defined( SAKE_SIMD_SSE ) && SAKE_SIMD_SSE >= 2
    static std::size_t const value = 2;
#else
    static std::size_t const value = 1;
#endif
};

} // namespace simd

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_SIMD_DEFAULT_SIZE_HPP

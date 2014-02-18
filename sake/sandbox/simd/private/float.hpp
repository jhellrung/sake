/*******************************************************************************
 * sake/core/math/simd/private/float.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_SIMD_PRIVATE_FLOAT_HPP
#define SAKE_CORE_MATH_SIMD_PRIVATE_FLOAT_HPP

#include <cmath>

#include <boost/integer.hpp>
#include <boost/static_assert.hpp>

#include <sake/core/config.hpp>
#include <sake/core/math/iec559_traits.hpp>

#ifdef SAKE_SIMD_SSE
#include <xmmintrin.h>
#endif // #ifdef SAKE_SIMD_SSE
#if SAKE_SIMD_SSE >= 2
#include <emmintrin.h>
#endif // #if SAKE_SIMD_SSE >= 2
#ifdef SAKE_SIMD_AVX
#include <immintrin.h>
#endif // #ifdef SAKE_SIMD_AVX

namespace sake
{

namespace simd
{

namespace float_private
{

/*******************************************************************************
 * union float_uint<T>
 ******************************************************************************/

template< class T >
union float_uint
{
    typedef T float_type;
    typedef typename boost::uint_t< sake::iec559_traits<T>::bits >::exact uint_type;
    uint_type u;
    float_type f;
};

/*******************************************************************************
 * approx_rcp(float_uint<T> x) -> T
 ******************************************************************************/

inline float
approx_rcp(float_private::float_uint< float > x)
{
#ifdef SAKE_SIMD_SSE
    _mm_store_ss(&x.f, _mm_rcp_ss(_mm_set_ss(x.f)));
    return x.f;
#else // #ifdef SAKE_SIMD_SSE
    BOOST_STATIC_ASSERT((sake::iec559_traits< float >::bits == 32));
    // TODO: Improve constant.
    x.u = 0x7E800000 - x.u;
    return x.f;
#endif // #ifdef SAKE_SIMD_SSE
};

inline double
approx_rcp(float_private::float_uint< double > x)
{
    BOOST_STATIC_ASSERT((sake::iec559_traits< double >::bits == 64));
    // TODO: Improve constant.
    x.u = 0x7FD0000000000000 - x.u;
    return x.f;
}

/*******************************************************************************
 * sqrt(float_uint<T> x) -> T
 ******************************************************************************/

inline float
sqrt(float_private::float_uint< float > x)
{
#ifdef SAKE_SIMD_SSE
    _mm_store_ss(&x.f, _mm_sqrt_ss(_mm_load_ss(&x.f)));
    return x.f;
#else // #ifdef SAKE_SIMD_SSE
    return std::sqrt(x.f);
#endif // #ifdef SAKE_SIMD_SSE
}

inline double
sqrt(float_private::float_uint< double > x)
{
#if SAKE_SIMD_SSE >= 2
    __m128d x_ = _mm_load_sd(&x.f);
    _mm_store_sd(&x.f, _mm_sqrt_sd(x_, x_));
    return x.f;
#else // #if SAKE_SIMD_SSE >= 2
    return std::sqrt(x.f);
#endif // #if SAKE_SIMD_SSE >= 2
}

/*******************************************************************************
 * approx_sqrt(float_uint<T> x) -> T
 ******************************************************************************/

inline float
approx_rsqrt(float_private::float_uint< float > x)
{
#ifdef SAKE_SIMD_SSE
    _mm_store_ss(&x.f, _mm_rsqrt_ss(_mm_load_ss(&x.f)));
    return x.f;
#else // #ifdef SAKE_SIMD_SSE
    BOOST_STATIC_ASSERT((sake::iec559_traits< float >::bits == 32));
    x.u = 0x5f375a86 - (x.u >> 1);
    return x.f;
#endif // #ifdef SAKE_SIMD_SSE
}

inline double
approx_rsqrt(float_private::float_uint< double > x)
{
    BOOST_STATIC_ASSERT((sake::iec559_traits< double >::bits == 64));
    x.u = 0x5fe6eb50c7b537a9 - (x.u >> 1);
    return x.f;
}

} // namespace float_private

} // namespace simd

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_SIMD_PRIVATE_FLOAT_HPP

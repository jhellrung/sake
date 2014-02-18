/*******************************************************************************
 * sake/core/math/simd/float.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_SIMD_FLOAT_HPP
#define SAKE_CORE_MATH_SIMD_FLOAT_HPP

#include <cstddef>

#include <limits>

#include <boost/static_assert.hpp>

#include <sake/core/math/iec559_traits.hpp>
//#include <sake/core/math/simd/aligned_tag.hpp>
//#include <sake/core/math/simd/default_size.hpp>
//#include <sake/core/math/simd/private/float.hpp>
#include <sake/core/math/sqr.hpp>
#include <sake/core/math/static_intlog2.hpp>
#include <sake/core/math/zero.hpp>
#include <sake/core/utility/using_typedef.hpp>

#include <sake/sandbox/simd/aligned_tag.hpp>
#include <sake/sandbox/simd/default_size.hpp>
#include <sake/sandbox/simd/private/float.hpp>

namespace sake
{

namespace simd
{

template< class T, std::size_t N = simd::default_size<T>::value >
struct float_;

/*******************************************************************************
 ******************************************************************************/

template< class T, std::size_t N >
inline void
swap(simd::float_<T,N> & x0, simd::float_<T,N> & x1)
{ x0.swap(x1); }

template< class T, std::size_t N >
inline void swap_if(
    simd::float_<T,N> const & b,
    simd::float_<T,N>       & x0,
    simd::float_<T,N>       & x1)
{
    simd::float_<T,N> const t = b & (x0 ^ x1);
    x0 ^= t;
    x1 ^= t;
};

template< class T, std::size_t N >
inline simd::float_<T,N>
conditional(
    simd::float_<T,N> const & b,
    simd::float_<T,N> const & t,
    simd::float_<T,N> const & f)
{ return blend(f,t,b); }

template< class T, std::size_t N >
inline simd::float_<T,N>
sign(simd::float_<T,N> const & x)
{ return x.sign(); }

template< class T, std::size_t N >
inline simd::float_<T,N>
abs(simd::float_<T,N> const & x)
{ return x.abs(); }

template< class T, std::size_t N >
inline simd::float_<T,N>
sqr(simd::float_<T,N> const & x)
{ return x * x; }

template< class T, std::size_t N >
inline simd::float_<T,N>
approx_rcp(simd::float_<T,N> const & x)
{ return x.approx_rcp(); }

template< class T, std::size_t N >
inline simd::float_<T,N>
sqrt(simd::float_<T,N> const & x)
{ return x.sqrt(); }

template< class T, std::size_t N >
inline simd::float_<T,N>
approx_rsqrt(simd::float_<T,N> const & x)
{ return x.approx_rsqrt(); }

template< class T, std::size_t N >
inline simd::float_<T,N>
refine_rsqrt(simd::float_<T,N> const & x, simd::float_<T,N> const & y)
{
    static simd::float_<T,N> const _1_5(1.5);
    static simd::float_<T,N> const _0_5(0.5);
    return y * (_1_5 - (_0_5 * x) * sake::sqr(y));
}

template< class T, std::size_t N >
inline simd::float_<T,N>
refine_rsqrt2(simd::float_<T,N> const & x_over_2, simd::float_<T,N> const & y)
{
    static simd::float_<T,N> const _1_5(1.5);
    return y * (_1_5 - x_over_2 * sake::sqr(y));
}

/*******************************************************************************
 ******************************************************************************/

template< class T >
struct float_<T,1>
{
    BOOST_STATIC_ASSERT((std::numeric_limits<T>::is_iec559));

    typedef T scalar_type;
    static std::size_t const static_size = 1;

    float_()
    { }
    explicit float_(scalar_type const x)
    { load(x); }
    explicit float_(sake::zero_t)
    { load(sake::zero); }
    explicit float_(scalar_type const * const p)
    { load(p); }
    float_(scalar_type const * const p, simd::aligned_tag)
    { load(p, simd::_aligned); }

    float_& operator=(scalar_type const x)
    { load(x); return *this; }
    float_& operator=(sake::zero_t)
    { load(sake::zero); return *this; }

    void load(scalar_type const x)
    { _.f = x; }
    void load(sake::zero_t)
    { _.f = 0; }
    void load(scalar_type const * const p)
    { _.f = *p; }
    void load(scalar_type const * const p, simd::aligned_tag)
    { _.f = *p; }

    void store(scalar_type * const p) const
    { *p = _.f; }
    void store(scalar_type * const p, simd::aligned_tag) const
    { *p = _.f; }

    void swap(float_ & other)
    {
        _.u ^= other._.u;
        other._.u ^= _.u;
        _.u ^= other._.u;
    }

    inline friend
    float_ blend(float_ const & x0, float_ const & x1, float_ const & mask)
    { return float_((~mask._.u & x0._.u) | (mask._.u & x1._.u), 0); }

#define define_op( op ) \
    float_ & operator op (float_ const & other) \
    { _.f op other._.f; return *this; }
    define_op( += )
    define_op( -= )
    define_op( *= )
    define_op( /= )
#undef define_op

#define define_op( op ) \
    float_ & operator op (float_ const & other) \
    { _.u op other._.u; return *this; }
    define_op( &= )
    define_op( |= )
    define_op( ^= )
#undef define_op

#define define_op( op ) \
    inline friend float_ operator op (float_ const & x0, float_ const & x1) \
    { return float_(static_cast< uint_type >(0) - (x0._.f op x1._.f), 0); }
    define_op( == )
    define_op( != )
    define_op( < )
    define_op( > )
    define_op( <= )
    define_op( >= )
#undef define_op

    float_ operator-() const
    { return float_(-_.f); }

#define define_op( op ) \
    inline friend float_ operator op (float_ const & x0, float_ const & x1) \
    { return float_(x0._.f op x1._.f); }
    define_op( + )
    define_op( - )
    define_op( * )
    define_op( / )
#undef define_op

    float_ operator~() const
    { return float_(~_.u); }

#define define_op( op ) \
    inline friend float_ operator op (float_ const & x0, float_ const & x1) \
    { return float_(x0._.u op x1._.u, 0); }
    define_op( & )
    define_op( | )
    define_op( ^ )
#undef define_op

    inline friend float_ and_not(float_ const & x0, float_ const & x1)
    { return float_(~x0._.u & x1._.u); }

    float_ sign() const
    { return float_(_.u & ~((static_cast< uint_type >(-1) >> 1)), 0); }

    float_ abs() const
    { return float_(_.u & (static_cast< uint_type >(-1) >> 1), 0); }

    float_ approx_rcp() const
    { return float_(float_private::approx_rcp(_)); }

    float_ sqrt() const
    { return float_(float_private::sqrt(_)); }

    float_ approx_rsqrt() const
    { return float_(float_private::approx_rsqrt(_)); }

private:
    typedef simd::float_private::float_uint<T> float_uint_type;
    SAKE_USING_TYPEDEF( typename float_uint_type, uint_type );
    float_uint_type _;

    explicit float_(uint_type const x, int)
    { _.u = x; }
};

/*******************************************************************************
 ******************************************************************************/

template< class T, std::size_t N >
struct float_
{
    typedef T scalar_type;
    static std::size_t const static_size = N;

    float_()
    { }
    explicit float_(scalar_type const x)
    { load(x); }
    explicit float_(sake::zero_t)
    { load(sake::zero); }
    explicit float_(scalar_type const * const p)
    { load(p); }
    float_(scalar_type const * const p, simd::aligned_tag)
    { load(p, simd::_aligned); }

    float_& operator=(scalar_type const x)
    { load(x); return *this; }
    float_& operator=(sake::zero_t)
    { load(sake::zero); return *this; }

    void load(scalar_type const x)
    { _0.load(x); _1.load(x); }
    void load(sake::zero_t)
    { _0.load(sake::zero); _1.load(sake::zero); }
    void load(scalar_type const * const p)
    { _0.load(p); _1.load(p + half_size); }
    void load(scalar_type const * const p, simd::aligned_tag)
    { _0.load(p, simd::_aligned); _1.load(p + half_size, simd::_aligned); }

    void store(scalar_type * const p) const
    { _0.store(p); _1.store(p + half_size); }
    void store(scalar_type * const p, simd::aligned_tag) const
    { _0.store(p, simd::_aligned); _1.store(p + half_size, simd::_aligned); }

    void swap(float_ & other)
    { _0.swap(other._0); _1.swap(other._1); }

    inline friend
    float_ blend(float_ const & x0, float_ const & x1, float_ const & mask)
    {
        return float_(
            blend(x0._0, x1._0, mask._0),
            blend(x0._1, x1._1, mask._1)
        );
    }

#define define_op( op ) \
    float_ & operator op (float_ const & other) \
    { _0 op other._0; _1 op other._1; return *this; }
    define_op( += )
    define_op( -= )
    define_op( *= )
    define_op( /= )
    define_op( &= )
    define_op( |= )
    define_op( ^= )
#undef define_op

#define define_op( op ) \
    inline friend float_ operator op (float_ const & x0, float_ const & x1) \
    { return float_(x0._0 op x1._0, x0._1 op x1._1); }
    define_op( == )
    define_op( != )
    define_op( < )
    define_op( > )
    define_op( <= )
    define_op( >= )
    define_op( + )
    define_op( - )
    define_op( * )
    define_op( / )
    define_op( & )
    define_op( | )
    define_op( ^ )
#undef define_op

    float_ operator-() const
    { return float_(-_0, -_1); }

    float_ operator~() const
    { return float_(~_0, ~_1); }

    inline friend float_ and_not(float_ const & x0, float_ const & x1)
    { return float_(and_not(x0._0, x1._0), and_not(x0._1, x1._1)); }

    float_ sign() const
    { return float_(_0.sign(), _1.sign()); }

    float_ abs() const
    { return float_(_0.abs(), _1.abs()); }

    float_ approx_rcp() const
    { return float_(_0.approx_rcp(), _1.approx_rcp()); }

    float_ sqrt() const
    { return float_(_0.sqrt(), _1.sqrt()); }

    float_ approx_rsqrt() const
    { return float_(_0.approx_sqrt(), _1.approx_sqrt()); }

private:
    static std::size_t const half_size = 1 << sake::static_intlog2_c< N-1 >::value;
    typedef simd::float_< T,     half_size > _0_type;
    typedef simd::float_< T, N - half_size > _1_type;

    _0_type _0;
    _1_type _1;

    float_(_0_type const x0, _1_type const x1)
        : _0(x0), _1(x1)
    { }
};

/*******************************************************************************
 ******************************************************************************/

#ifdef SAKE_SIMD_SSE

template<>
struct float_< float, 4 >
{
    BOOST_STATIC_ASSERT((std::numeric_limits< float >::is_iec559));
    BOOST_STATIC_ASSERT((sake::iec559_traits< float >::bits == 32));

    typedef float scalar_type;
    static std::size_t const static_size = 4;

    float_()
    { }
    explicit float_(scalar_type const x)
    { load(x); }
    explicit float_(sake::zero_t)
    { load(sake::zero); }
    explicit float_(scalar_type const * const p)
    { load(p); }
    float_(scalar_type const * const p, simd::aligned_tag)
    { load(p, simd::_aligned); }

    float_& operator=(scalar_type const x)
    { load(x); return *this; }
    float_& operator=(sake::zero_t)
    { load(sake::zero); return *this; }

    void load(scalar_type const x)
    { _ = _mm_set1_ps(x); }
    void load(sake::zero_t)
    { _ = _mm_setzero_ps(); }
    void load(scalar_type const * const p)
    { _ = _mm_loadu_ps(const_cast< scalar_type * >(p)); }
    void load(scalar_type const * const p, simd::aligned_tag)
    { _ = _mm_load_ps(const_cast< scalar_type * >(p)); }

    void store(scalar_type * const p) const
    { _mm_storeu_ps(p, _); }
    void store(scalar_type * const p, simd::aligned_tag) const
    { _mm_store_ps(p, _); }

    void swap(float_ & other)
    {
        _ = _mm_xor_ps(_, other._);
        other._ = _mm_xor_ps(other._, _);
        _ = _mm_xor_ps(_, other._);
    }

    inline friend
    float_ blend(float_ const & x0, float_ const & x1, float_ const & mask)
    {
#ifdef SAKE_SIMD_AVX
        return float_(_mm256_castps256_ps128(_mm256_blendv_ps(
            _mm256_castps128_ps256(x0._),
            _mm256_castps128_ps256(x1._),
            _mm256_castps128_ps256(mask._)
        )));
#else // #ifdef SAKE_SIMD_AVX
        return float_(_mm_or_ps(
            _mm_andnot_ps(mask._, x0._),
            _mm_and_ps(mask._, x1._)
        ));
#endif // #ifdef SAKE_SIMD_AVX
    }

#define define_op( op, mm_op ) \
    float_ & operator op (float_ const & other) \
    { _ = mm_op(_, other._); return *this; }
    define_op( +=, _mm_add_ps )
    define_op( -=, _mm_sub_ps )
    define_op( *=, _mm_mul_ps )
    define_op( /=, _mm_div_ps )
    define_op( &=, _mm_and_ps )
    define_op( |=, _mm_or_ps )
    define_op( ^=, _mm_xor_ps )
#undef define_op

#define define_op( op, mm_op ) \
    inline friend float_ operator op (float_ const & x0, float_ const & x1) \
    { return float_(mm_op(x0._, x1._)); }
    define_op( ==, _mm_cmpeq_ps )
    define_op( !=, _mm_cmpneq_ps )
    define_op( < , _mm_cmplt_ps )
    define_op( > , _mm_cmpgt_ps )
    define_op( <=, _mm_cmple_ps )
    define_op( >=, _mm_cmpge_ps )
    define_op( +, _mm_add_ps )
    define_op( -, _mm_sub_ps )
    define_op( *, _mm_mul_ps )
    define_op( /, _mm_div_ps )
    define_op( &, _mm_and_ps )
    define_op( |, _mm_or_ps )
    define_op( ^, _mm_xor_ps )
#undef define_op

    float_ operator-() const
    {
        static float_uint_type const fu_mask = { 0x80000000 };
        static __m128 const m128_mask = _mm_set1_ps(fu_mask.f);
        return float_(_mm_xor_ps(_, m128_mask));
    }

    float_ operator~() const
    {
        static float_uint_type const fu_mask = { 0xFFFFFFFF };
        static __m128 const m128_mask = _mm_set1_ps(fu_mask.f);
        return float_(_mm_xor_ps(_, m128_mask));
    }

    inline friend float_ and_not(float_ const & x0, float_ const & x1)
    { return float_(_mm_andnot_ps(x0._, x1._)); }

    float_ sign() const
    {
        static float_uint_type const fu_mask = { 0x80000000 };
        static __m128 const m128_mask = _mm_set1_ps(fu_mask.f);
        return float_(_mm_and_ps(_, m128_mask));
    }

    float_ abs() const
    {
        static float_uint_type const fu_mask = { 0x7FFFFFFF };
        static __m128 const m128_mask = _mm_set1_ps(fu_mask.f);
        return float_(_mm_and_ps(_, m128_mask));
    }

    float_ approx_rcp() const
    { return float_(_mm_rcp_ps(_)); }

    float_ sqrt() const
    { return float_(_mm_sqrt_ps(_)); }

    float_ approx_rsqrt() const
    { return float_(_mm_rsqrt_ps(_)); }

private:
    typedef float_private::float_uint< float > float_uint_type;

    __m128 _;

    explicit float_(__m128 const & x)
        : _(x)
    { }
};

#endif // #ifdef SAKE_SIMD_SSE

/*******************************************************************************
 ******************************************************************************/

#if SAKE_SIMD_SSE >= 2

template<>
struct float_< double, 2 >
{
    BOOST_STATIC_ASSERT((std::numeric_limits< double >::is_iec559));
    BOOST_STATIC_ASSERT((sake::iec559_traits< double >::bits == 64));

    typedef double scalar_type;
    static std::size_t const static_size = 2;

    float_()
    { }
    explicit float_(scalar_type const x)
    { load(x); }
    explicit float_(sake::zero_t)
    { load(sake::zero); }
    explicit float_(scalar_type const * const p)
    { load(p); }
    float_(scalar_type const * const p, simd::aligned_tag)
    { load(p, simd::_aligned); }

    float_& operator=(scalar_type const x)
    { load(x); return *this; }
    float_& operator=(sake::zero_t)
    { load(sake::zero); return *this; }

    void load(scalar_type const x)
    { _ = _mm_set1_pd(x); }
    void load(sake::zero_t)
    { _ = _mm_setzero_pd(); }
    void load(scalar_type const * const p)
    { _ = _mm_loadu_pd(const_cast< scalar_type * >(p)); }
    void load(scalar_type const * const p, simd::aligned_tag)
    { _ = _mm_load_pd(const_cast< scalar_type * >(p)); }

    void store(scalar_type * const p) const
    { _mm_storeu_pd(p, _); }
    void store(scalar_type * const p, simd::aligned_tag) const
    { _mm_store_pd(p, _); }

    void swap(float_ & other)
    {
        _ = _mm_xor_pd(_, other._);
        other._ = _mm_xor_pd(other._, _);
        _ = _mm_xor_pd(_, other._);
    }

    inline friend
    float_ blend(float_ const & x0, float_ const & x1, float_ const & mask)
    {
#ifdef SAKE_SIMD_AVX
        return float_(_mm256_castpd256_pd128(_mm256_blendv_pd(
            _mm256_castpd128_pd256(x0._),
            _mm256_castpd128_pd256(x1._),
            _mm256_castpd128_pd256(mask._)
        )));
#else // #ifdef SAKE_SIMD_AVX
        return float_(_mm_or_pd(
            _mm_andnot_pd(mask._, x0._),
            _mm_and_pd(mask._, x1._)
        ));
#endif // #ifdef SAKE_SIMD_AVX
    }

#define define_op( op, mm_op ) \
    float_ & operator op (float_ const & other) \
    { _ = mm_op(_, other._); return *this; }
    define_op( +=, _mm_add_pd )
    define_op( -=, _mm_sub_pd )
    define_op( *=, _mm_mul_pd )
    define_op( /=, _mm_div_pd )
    define_op( &=, _mm_and_pd )
    define_op( |=, _mm_or_pd )
    define_op( ^=, _mm_xor_pd )
#undef define_op

#define define_op( op, mm_op ) \
    inline friend float_ operator op (float_ const & x0, float_ const & x1) \
    { return float_(mm_op(x0._, x1._)); }
    define_op( ==, _mm_cmpeq_pd )
    define_op( !=, _mm_cmpneq_pd )
    define_op( < , _mm_cmplt_pd )
    define_op( > , _mm_cmpgt_pd )
    define_op( <=, _mm_cmple_pd )
    define_op( >=, _mm_cmpge_pd )
    define_op( +, _mm_add_pd )
    define_op( -, _mm_sub_pd )
    define_op( *, _mm_mul_pd )
    define_op( /, _mm_div_pd )
    define_op( &, _mm_and_pd )
    define_op( |, _mm_or_pd )
    define_op( ^, _mm_xor_pd )
#undef define_op

    float_ operator-() const
    {
        static float_uint_type const fu_mask = { 0x8000000000000000 };
        static __m128d const m128d_mask = _mm_set1_pd(fu_mask.f);
        return float_(_mm_xor_pd(_, m128d_mask));
    }

    float_ operator~() const
    {
        static float_uint_type const fu_mask = { 0xFFFFFFFFFFFFFFFF };
        static __m128d const m128d_mask = _mm_set1_pd(fu_mask.f);
        return float_(_mm_xor_pd(_, m128d_mask));
    }

    inline friend float_ and_not(float_ const & x0, float_ const & x1)
    { return float_(_mm_andnot_pd(x0._, x1._)); }

    float_ sign() const
    {
        static float_uint_type const fu_mask = { 0x8000000000000000 };
        static __m128d const m128d_mask = _mm_set1_pd(fu_mask.f);
        return float_(_mm_and_pd(_, m128d_mask));
    }

    float_ abs() const
    {
        static float_uint_type const fu_mask = { 0x7FFFFFFFFFFFFFFF };
        static __m128d const m128d_mask = _mm_set1_pd(fu_mask.f);
        return float_(_mm_and_pd(_, m128d_mask));
    }

    float_ approx_rcp() const
    {
#ifdef _MSC_VER
        __declspec( align( 16 ) ) float_uint_type fu[2];
#else // #ifdef _MSC_VER
        float_uint_type fu[2] __attribute__(( aligned( 16 ) ));
#endif // #ifdef _MSC_VER
        _mm_store_pd(&fu[0].f, _);
        fu[0].f = float_private::approx_rcp(fu[0]);
        fu[1].f = float_private::approx_rcp(fu[1]);
        return float_(_mm_load_pd(&fu[0].f));
    }

    float_ sqrt() const
    { return float_(_mm_sqrt_pd(_)); }

    float_ approx_rsqrt() const
    {
#ifdef _MSC_VER
        __declspec( align( 16 ) ) float_uint_type fu[2];
#else // #ifdef _MSC_VER
        float_uint_type fu[2] __attribute__(( aligned( 16 ) ));
#endif // #ifdef _MSC_VER
        _mm_store_pd(&fu[0].f, _);
        fu[0].f = float_private::approx_rsqrt(fu[0]);
        fu[1].f = float_private::approx_rsqrt(fu[1]);
        return float_(_mm_load_pd(&fu[0].f));
    }

private:
    typedef float_private::float_uint< double > float_uint_type;

    __m128d _;

    explicit float_(__m128d const & x)
        : _(x)
    { }
};

#endif // #if SAKE_SIMD_SSE >= 2

/*******************************************************************************
 ******************************************************************************/

#ifdef SAKE_SIMD_AVX

template<>
struct float_< float, 8 >
{
    BOOST_STATIC_ASSERT((std::numeric_limits< float >::is_iec559));
    BOOST_STATIC_ASSERT((sake::iec559_traits< float >::bits == 32));

    typedef float scalar_type;
    static std::size_t const static_size = 8;

    float_()
    { }
    explicit float_(scalar_type const x)
    { load(x); }
    explicit float_(sake::zero_t)
    { load(sake::zero); }
    explicit float_(scalar_type const * const p)
    { load(p); }
    float_(scalar_type const * const p, simd::aligned_tag)
    { load(p, simd::_aligned); }

    float_& operator=(scalar_type const x)
    { load(x); return *this; }
    float_& operator=(sake::zero_t)
    { load(sake::zero); return *this; }

    void load(scalar_type const x)
    { _ = _mm256_set1_ps(x); }
    void load(sake::zero_t)
    { _ = _mm256_setzero_ps(); }
    void load(scalar_type const * const p)
    { _ = _mm256_loadu_ps(const_cast< scalar_type * >(p)); }
    void load(scalar_type const * const p, simd::aligned_tag)
    { _ = _mm256_load_ps(const_cast< scalar_type * >(p)); }

    void store(scalar_type * const p) const
    { _mm256_storeu_ps(p, _); }
    void store(scalar_type * const p, simd::aligned_tag) const
    { _mm256_store_ps(p, _); }

    void swap(float_ & other)
    {
        _ = _mm256_xor_ps(_, other._);
        other._ = _mm256_xor_ps(other._, _);
        _ = _mm256_xor_ps(_, other._);
    }

    inline friend
    float_ blend(float_ const & x0, float_ const & x1, float_ const & mask)
    { return float_(_mm256_blendv_ps(x0._, x1._, mask._)); }

#define define_op( op, mm_op ) \
    float_ & operator op (float_ const & other) \
    { _ = mm_op(_, other._); return *this; }
    define_op( +=, _mm256_add_ps )
    define_op( -=, _mm256_sub_ps )
    define_op( *=, _mm256_mul_ps )
    define_op( /=, _mm256_div_ps )
    define_op( &=, _mm256_and_ps )
    define_op( |=, _mm256_or_ps )
    define_op( ^=, _mm256_xor_ps )
#undef define_op

#define define_op( op, imm ) \
    inline friend float_ operator op (float_ const & x0, float_ const & x1) \
    { return float_(_mm256_cmp_ps(x0._, x1._, imm)); }
    define_op( ==,  0 )
    define_op( !=, 12 )
    define_op( < , 17 )
    define_op( > , 30 )
    define_op( <=, 18 )
    define_op( >=, 29 )
#undef define_op

#define define_op( op, mm_op ) \
    inline friend float_ operator op (float_ const & x0, float_ const & x1) \
    { return float_(mm_op(x0._, x1._)); }
    define_op( +, _mm256_add_ps )
    define_op( -, _mm256_sub_ps )
    define_op( *, _mm256_mul_ps )
    define_op( /, _mm256_div_ps )
    define_op( &, _mm256_and_ps )
    define_op( |, _mm256_or_ps )
    define_op( ^, _mm256_xor_ps )
#undef define_op

    float_ operator-() const
    {
        static float_uint_type const fu_mask = { 0x80000000 };
        static __m256 const m256_mask = _mm256_set1_ps(fu_mask.f);
        return float_(_mm256_xor_ps(_, m256_mask));
    }

    float_ operator~() const
    {
        static float_uint_type const fu_mask = { 0xFFFFFFFF };
        static __m256 const m256_mask = _mm256_set1_ps(fu_mask.f);
        return float_(_mm256_xor_ps(_, m256_mask));
    }

    inline friend float_ and_not(float_ const & x0, float_ const & x1)
    { return float_(_mm256_andnot_ps(x0._, x1._)); }

    float_ sign() const
    {
        static float_uint_type const fu_mask = { 0x80000000 };
        static __m256 const m256_mask = _mm256_set1_ps(fu_mask.f);
        return float_(_mm256_and_ps(_, m256_mask));
    }

    float_ abs() const
    {
        static float_uint_type const fu_mask = { 0x7FFFFFFF };
        static __m256 const m256_mask = _mm256_set1_ps(fu_mask.f);
        return float_(_mm256_and_ps(_, m256_mask));
    }

    float_ approx_rcp() const
    { return float_(_mm256_rcp_ps(_)); }

    float_ sqrt() const
    { return float_(_mm256_sqrt_ps(_)); }

    float_ approx_rsqrt() const
    { return float_(_mm256_rsqrt_ps(_)); }

private:
    typedef float_private::float_uint< float > float_uint_type;

    __m256 _;

    explicit float_(__m256 const & x)
        : _(x)
    { }
};

#endif // #ifdef SAKE_SIMD_AVX

/*******************************************************************************
 ******************************************************************************/

#ifdef SAKE_SIMD_AVX

template<>
struct float_< double, 4 >
{
    BOOST_STATIC_ASSERT((std::numeric_limits< double >::is_iec559));
    BOOST_STATIC_ASSERT((sake::iec559_traits< double >::bits == 64));

    typedef double scalar_type;
    static std::size_t const static_size = 4;

    float_()
    { }
    explicit float_(scalar_type const x)
    { load(x); }
    explicit float_(sake::zero_t)
    { load(sake::zero); }
    explicit float_(scalar_type const * const p)
    { load(p); }
    float_(scalar_type const * const p, simd::aligned_tag)
    { load(p, simd::_aligned); }

    float_& operator=(scalar_type const x)
    { load(x); return *this; }
    float_& operator=(sake::zero_t)
    { load(sake::zero); return *this; }

    void load(scalar_type const x)
    { _ = _mm256_set1_pd(x); }
    void load(sake::zero_t)
    { _ = _mm256_setzero_pd(); }
    void load(scalar_type const * const p)
    { _ = _mm256_loadu_pd(const_cast< scalar_type * >(p)); }
    void load(scalar_type const * const p, simd::aligned_tag)
    { _ = _mm256_load_pd(const_cast< scalar_type * >(p)); }

    void store(scalar_type * const p) const
    { _mm256_storeu_pd(p, _); }
    void store(scalar_type * const p, simd::aligned_tag) const
    { _mm256_store_pd(p, _); }

    void swap(float_ & other)
    {
        _ = _mm256_xor_pd(_, other._);
        other._ = _mm256_xor_pd(other._, _);
        _ = _mm256_xor_pd(_, other._);
    }

    inline friend
    float_ blend(float_ const & x0, float_ const & x1, float_ const & mask)
    { return float_(_mm256_blendv_pd(x0._, x1._, mask._)); }

#define define_op( op, mm_op ) \
    float_ & operator op (float_ const & other) \
    { _ = mm_op(_, other._); return *this; }
    define_op( +=, _mm256_add_pd )
    define_op( -=, _mm256_sub_pd )
    define_op( *=, _mm256_mul_pd )
    define_op( /=, _mm256_div_pd )
    define_op( &=, _mm256_and_pd )
    define_op( |=, _mm256_or_pd )
    define_op( ^=, _mm256_xor_pd )
#undef define_op

#define define_op( op, imm ) \
    inline friend float_ operator op (float_ const & x0, float_ const & x1) \
    { return float_(_mm256_cmp_pd(x0._, x1._, imm)); }
    define_op( ==,  0 )
    define_op( !=, 12 )
    define_op( < , 17 )
    define_op( > , 30 )
    define_op( <=, 18 )
    define_op( >=, 29 )
#undef define_op

#define define_op( op, mm_op ) \
    inline friend float_ operator op (float_ const & x0, float_ const & x1) \
    { return float_(mm_op(x0._, x1._)); }
    define_op( +, _mm256_add_pd )
    define_op( -, _mm256_sub_pd )
    define_op( *, _mm256_mul_pd )
    define_op( /, _mm256_div_pd )
    define_op( &, _mm256_and_pd )
    define_op( |, _mm256_or_pd )
    define_op( ^, _mm256_xor_pd )
#undef define_op

    float_ operator-() const
    {
        static float_uint_type const fu_mask = { 0x8000000000000000 };
        static __m256d const m256d_mask = _mm256_set1_pd(fu_mask.f);
        return float_(_mm256_xor_pd(_, m256d_mask));
    }

    float_ operator~() const
    {
        static float_uint_type const fu_mask = { 0xFFFFFFFFFFFFFFFF };
        static __m256d const m256d_mask = _mm256_set1_pd(fu_mask.f);
        return float_(_mm256_xor_pd(_, m256d_mask));
    }

    inline friend float_ and_not(float_ const & x0, float_ const & x1)
    { return float_(_mm256_andnot_pd(x0._, x1._)); }

    float_ sign() const
    {
        static float_uint_type const fu_mask = { 0x8000000000000000 };
        static __m256d const m256d_mask = _mm256_set1_pd(fu_mask.f);
        return float_(_mm256_and_pd(_, m256d_mask));
    }

    float_ abs() const
    {
        static float_uint_type const fu_mask = { 0x7FFFFFFFFFFFFFFF };
        static __m256d const m256d_mask = _mm256_set1_pd(fu_mask.f);
        return float_(_mm256_and_pd(_, m256d_mask));
    }

    float_ approx_rcp() const
    {
#ifdef _MSC_VER
        __declspec( align( 16 ) ) float_uint_type fu[4];
#else // #ifdef _MSC_VER
        float_uint_type fu[4] __attribute__(( aligned( 16 ) ));
#endif // #ifdef _MSC_VER
        _mm256_store_pd(&fu[0].f, _);
        fu[0].f = float_private::approx_rcp(fu[0]);
        fu[1].f = float_private::approx_rcp(fu[1]);
        fu[2].f = float_private::approx_rcp(fu[2]);
        fu[3].f = float_private::approx_rcp(fu[3]);
        return float_(_mm256_load_pd(&fu[0].f));
    }

    float_ sqrt() const
    { return float_(_mm256_sqrt_pd(_)); }

    float_ approx_rsqrt() const
    {
#ifdef _MSC_VER
        __declspec( align( 16 ) ) float_uint_type fu[4];
#else // #ifdef _MSC_VER
        float_uint_type fu[4] __attribute__(( aligned( 16 ) ));
#endif // #ifdef _MSC_VER
        _mm256_store_pd(&fu[0].f, _);
        fu[0].f = float_private::approx_rsqrt(fu[0]);
        fu[1].f = float_private::approx_rsqrt(fu[1]);
        fu[2].f = float_private::approx_rsqrt(fu[2]);
        fu[3].f = float_private::approx_rsqrt(fu[3]);
        return float_(_mm256_load_pd(&fu[0].f));
    }

private:
    typedef float_private::float_uint< double > float_uint_type;

    __m256d _;

    explicit float_(__m256d const & x)
        : _(x)
    { }
};

#endif // #ifdef SAKE_SIMD_AVX

} // namespace simd

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_SIMD_FLOAT_HPP

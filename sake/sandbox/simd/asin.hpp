/*******************************************************************************
 * sake/core/math/simd/asin.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * asin(T const & x) -> T
 *
 * If |x| > 0.5, use
 *     asin(x) = pi/2 - 2 * asin(sqrt((1-x)/2))
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_SIMD_ASIN_HPP
#define SAKE_CORE_MATH_SIMD_ASIN_HPP

//#include <sake/core/math/simd/private/asincos.hpp>
//#include <sake/core/math/simd/scalar.hpp>

#include <sake/sandbox/simd/private/asincos.hpp>
#include <sake/sandbox/simd/scalar.hpp>

namespace sake
{

namespace simd
{

template< class T >
inline T
asin(T const & x_)
{
    typedef typename simd::scalar<T>::type scalar_type;

#define define_constant( c, x ) \
    static T const c(static_cast< scalar_type >(x));
    define_constant( _0_5, 0.5 )
    define_constant( _2, 2 )
    define_constant( pi_over_2, 3.14159265358979324/2 )
#undef define_constant

    T const s = sign(x_);
    T x = x_ ^ s;
    T const x_gt_0_5 = x > _0_5;
    T const x2 = conditional(x_gt_0_5, _0_5 - _0_5 * x, sqr(x));
    x = conditional(x_gt_0_5, sqrt(x2), x);
    T const y = x + (x * x2) * asincos_private::reduced_asin(x2);
    return s | conditional(x_gt_0_5, pi_over_2 - _2 * y, y);
}

} // namespace simd

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_SIMD_ASIN_HPP
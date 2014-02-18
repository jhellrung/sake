/*******************************************************************************
 * sake/core/math/simd/private/asincos.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * Approximates (asin(x) - x) / x^3 = (1/6) + (3/40) x^2 + ...
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_SIMD_PRIVATE_ASINCOS_HPP
#define SAKE_CORE_MATH_SIMD_PRIVATE_ASINCOS_HPP

//#include <sake/core/math/simd/scalar.hpp>
#include <sake/core/utility/type_tag.hpp>

#include <sake/sandbox/simd/scalar.hpp>

namespace sake
{

namespace simd
{

namespace asincos_private
{

#define define_constant( c, x ) \
    static T const c(static_cast< float >(x));

template< class T >
inline T
reduced_asin_taylor_polynomial(T const & x, sake::type_tag< float >)
{
    define_constant( c0, (1.)/(2.*3.) )
    define_constant( c1, (1.*3.)/(2.*4.*5.) )
    define_constant( c2, (1.*3.*5.)/(2.*4.*6.*7.) )
    define_constant( c3, (1.*3.*5.*7.)/(2.*4.*6.*8.*9.) )

    define_constant( c4, (1.*3.*5.*7.*9.)/(2.*4.*6.*8.*10.*11.) )
    define_constant( c5, (1.*3.*5.*7.*9.*11.)/(2.*4.*6.*8.*10.*12.*13.) )
    define_constant( c6, (1.*3.*5.*7.*9.*11.*13.)/(2.*4.*6.*8.*10.*12.*14.*15.) )
    define_constant( c7, (1.*3.*5.*7.*9.*11.*13.*15.)/(2.*4.*6.*8.*10.*12.*14.*16.*17.) )

#if 1
    T const c01 = c0 + c1 * x;
    T const c23 = c2 + c3 * x;
    T const c45 = c4 + c5 * x;
    T const c67 = c6 + c7 * x;
    T const x2 = x * x;
    T const c03 = c01 + c23 * x2;
    T const c47 = c45 + c67 * x2;
    T const x4 = x2 * x2;
    return c03 + c47 * x4;
#endif
}

#undef define_constant

#define define_constant( c, x ) \
    static T const c(static_cast< double >(x));

template< class T >
inline T
reduced_asin_taylor_polynomial(T const & x, sake::type_tag< double >)
{
    define_constant( c0, (1.)/(2.*3.) )
    define_constant( c1, (1.*3.)/(2.*4.*5.) )
    define_constant( c2, (1.*3.*5.)/(2.*4.*6.*7.) )
    define_constant( c3, (1.*3.*5.*7.)/(2.*4.*6.*8.*9.) )

    define_constant( c4, (1.*3.*5.*7.*9.)/(2.*4.*6.*8.*10.*11.) )
    define_constant( c5, (1.*3.*5.*7.*9.*11.)/(2.*4.*6.*8.*10.*12.*13.) )
    define_constant( c6, (1.*3.*5.*7.*9.*11.*13.)/(2.*4.*6.*8.*10.*12.*14.*15.) )
    define_constant( c7, (1.*3.*5.*7.*9.*11.*13.*15.)/(2.*4.*6.*8.*10.*12.*14.*16.*17.) )

    define_constant( c8, (1.*3.*5.*7.*9.*11.*13.*15.*17.)/(2.*4.*6.*8.*10.*12.*14.*16.*18.*19.) )
    define_constant( c9, (1.*3.*5.*7.*9.*11.*13.*15.*17.*19.)/(2.*4.*6.*8.*10.*12.*14.*16.*18.*20.*21.) )
    define_constant( cA, (1.*3.*5.*7.*9.*11.*13.*15.*17.*19.*21.)/(2.*4.*6.*8.*10.*12.*14.*16.*18.*20.*22.*23.) )
    define_constant( cB, (1.*3.*5.*7.*9.*11.*13.*15.*17.*19.*21.*23.)/(2.*4.*6.*8.*10.*12.*14.*16.*18.*20.*22.*24.*25.) )

    define_constant( cC, (1.*3.*5.*7.*9.*11.*13.*15.*17.*19.*21.*23.*25.)/(2.*4.*6.*8.*10.*12.*14.*16.*18.*20.*22.*24.*26.*27.) )
    define_constant( cD, (1.*3.*5.*7.*9.*11.*13.*15.*17.*19.*21.*23.*25.*27.)/(2.*4.*6.*8.*10.*12.*14.*16.*18.*20.*22.*24.*26.*28.*29.) )
    define_constant( cE, (1.*3.*5.*7.*9.*11.*13.*15.*17.*19.*21.*23.*25.*27.*29.)/(2.*4.*6.*8.*10.*12.*14.*16.*18.*20.*22.*24.*26.*28.*30.*31.) )
    define_constant( cF, (1.*3.*5.*7.*9.*11.*13.*15.*17.*19.*21.*23.*25.*27.*29.*31.)/(2.*4.*6.*8.*10.*12.*14.*16.*18.*20.*22.*24.*26.*28.*30.*32.*33.) )

    define_constant( cG, (1.*3.*5.*7.*9.*11.*13.*15.*17.*19.*21.*23.*25.*27.*29.*31.*33.)/(2.*4.*6.*8.*10.*12.*14.*16.*18.*20.*22.*24.*26.*28.*30.*32.*34.*35.) )
    define_constant( cH, (1.*3.*5.*7.*9.*11.*13.*15.*17.*19.*21.*23.*25.*27.*29.*31.*33.*35.)/(2.*4.*6.*8.*10.*12.*14.*16.*18.*20.*22.*24.*26.*28.*30.*32.*34.*36.*37.) )
    define_constant( cI, (1.*3.*5.*7.*9.*11.*13.*15.*17.*19.*21.*23.*25.*27.*29.*31.*33.*35.*37.)/(2.*4.*6.*8.*10.*12.*14.*16.*18.*20.*22.*24.*26.*28.*30.*32.*34.*36.*38.*39.) )
    define_constant( cJ, (1.*3.*5.*7.*9.*11.*13.*15.*17.*19.*21.*23.*25.*27.*29.*31.*33.*35.*37.*39.)/(2.*4.*6.*8.*10.*12.*14.*16.*18.*20.*22.*24.*26.*28.*30.*32.*34.*36.*38.*40.*41.) )

    define_constant( cK, (1.*3.*5.*7.*9.*11.*13.*15.*17.*19.*21.*23.*25.*27.*29.*31.*33.*35.*37.*39.*41.)/(2.*4.*6.*8.*10.*12.*14.*16.*18.*20.*22.*24.*26.*28.*30.*32.*34.*36.*38.*40.*42.*43.) )

#if 1
    T const c01 = c0 + c1 * x;
    T const c23 = c2 + c3 * x;
    T const c45 = c4 + c5 * x;
    T const c67 = c6 + c7 * x;
    T const c89 = c8 + c9 * x;
    T const cAB = cA + cB * x;
    T const cCD = cC + cD * x;
    T const cEF = cE + cF * x;
    T const cGH = cG + cH * x;
    T const cIJ = cI + cJ * x;
    T const x2 = x * x;
    T const c03 = c01 + c23 * x2;
    T const c47 = c45 + c67 * x2;
    T const c8B = c89 + cAB * x2;
    T const cCF = cCD + cEF * x2;
    T const cGJ = cGH + cIJ * x2;
    T const x4 = x2 * x2;
    T const c07 = c03 + c47 * x4;
    T const c8F = c8B + cCF * x4;
    T const cGK = cGJ + cK  * x4;
    T const x8 = x4 * x4;
    return c07 + x8 * (c8F + x8 * cGK);
#endif
}

#undef define_constant

template< class T >
inline T
reduced_asin(T const & x)
{
    typedef typename simd::scalar<T>::type scalar_type;
    return asincos_private::reduced_asin_taylor_polynomial(
        x, sake::type_tag< scalar_type >());
};

} // namespace asincos_private

} // namespace simd

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_SIMD_PRIVATE_ASINCOS_HPP

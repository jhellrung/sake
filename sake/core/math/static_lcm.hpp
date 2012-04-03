/*******************************************************************************
 * sake/core/math/static_lcm.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct static_lcm< N0, N1 >
 * struct static_lcm_c< N0, N1 >
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_STATIC_LCM_HPP
#define SAKE_CORE_MATH_STATIC_LCM_HPP

#include <sake/core/math/static_abs.hpp>
#include <sake/core/math/static_gcd.hpp>

namespace sake
{

template< int N0, int N1 >
struct static_lcm_c
{
    static int const N0_ = sake::static_abs_c< N0 >::value;
    static int const N1_ = sake::static_abs_c< N1 >::value;
    static int const value = (N0_ / sake::static_gcd_c< N0_, N1_ >::value) * N1_;
    typedef static_lcm_c type;
};

template< class N0, class N1 >
struct static_lcm
    : static_lcm_c< N0::value, N1::value >
{ };

} // namespace sake

#endif // SAKE_CORE_MATH_STATIC_LCM_HPP

/*******************************************************************************
 * sake/core/math/static_gcd.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct static_gcd< N0, N1 >
 * struct static_gcd_c< N0, N1 >
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_STATIC_GCD_HPP
#define SAKE_CORE_MATH_STATIC_GCD_HPP

#include <sake/core/math/static_abs.hpp>

namespace sake
{

namespace static_gcd_private
{

template< int N0, int N1, bool = (N0 < N1) >
struct dispatch;

template< int N0, int N1 >
struct dispatch< N0, N1, true >
    : dispatch< N1, N0, false >
{ };

template< int N0, int N1 >
struct dispatch< N0, N1, false >
    : dispatch< N1, N0 % N1, false >
{ };

template< int N >
struct dispatch< N, 0, false >
{
    static int const value = N;
    typedef dispatch type;
};

} // namespace static_gcd_private

template< int N0, int N1 >
struct static_gcd_c
    : static_gcd_private::dispatch<
          sake::static_abs_c< N0 >::value,
          sake::static_abs_c< N1 >::value
      >
{ };

template< class N0, class N1 >
struct static_gcd
    : static_gcd_c< N0::value, N1::value >
{ };

} // namespace sake

#endif // SAKE_CORE_MATH_STATIC_GCD_HPP

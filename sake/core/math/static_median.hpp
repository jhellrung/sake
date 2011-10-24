/*******************************************************************************
 * sake/core/math/static_median.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct static_median3< N0, N1, N2 >
 * struct static_median3_c< N0, N1, N2 >
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_STATIC_MEDIAN_HPP
#define SAKE_CORE_MATH_STATIC_MEDIAN_HPP

namespace sake
{

template< int N0, int N1, int N2 >
struct static_median3_c
{
    static int const value =
        N1 < N0 ?
        N2 < N1 ? N1 : N2 < N0 ? N2 : N0 :
        N2 < N1 ? N2 < N0 ? N0 : N2 : N1;
    typedef static_median3_c type;
};

template< class N0, class N1, class N2 >
struct static_median3
    : static_median3_c< N0::value, N1::value, N2::value >
{ };

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_STATIC_MEDIAN_HPP

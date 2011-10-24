/*******************************************************************************
 * sake/core/math/static_binom.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct static_binom<N,K>
 * struct static_binom_c<N,K>
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_STATIC_BINOM_HPP
#define SAKE_CORE_MATH_STATIC_BINOM_HPP

namespace sake
{

template< int N, int K >
struct static_binom_c
{
    static int const value = (N * static_binom_c< N-1, K-1 >::value) / K;
    typedef static_binom_c type;
};

template< int N >
struct static_binom_c<N,0>
{
    static int const value = 1;
    typedef static_binom_c type;
};

template< int K >
struct static_binom_c<0,K>
{
    static int const value = 0;
    typedef static_binom_c type;
};

template<>
struct static_binom_c<0,0>
{
    static int const value = 1;
    typedef static_binom_c type;
};

template< class N, class K >
struct static_binom
    : static_binom_c< N::value, K::value >
{ };

} // namespace sake

#endif // SAKE_CORE_MATH_STATIC_BINOM_HPP

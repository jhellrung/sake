/*******************************************************************************
 * sake/core/math/static_min.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct static_min< N0, N1 >
 * struct static_min_c< N0, N1 >
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_STATIC_MIN_HPP
#define SAKE_CORE_MATH_STATIC_MIN_HPP

namespace sake
{

template< int N0, int N1 >
struct static_min_c
{
    static int const value = N1 < N0 ? N1 : N0;
    typedef static_min_c type;
};

template< class N0, class N1 >
struct static_min
    : static_min_c< N0::value, N1::value >
{ };

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_STATIC_MIN_HPP

/*******************************************************************************
 * sake/core/math/static_max.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct static_max< N0, N1 >
 * struct static_max_c< N0, N1 >
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_STATIC_MAX_HPP
#define SAKE_CORE_MATH_STATIC_MAX_HPP

namespace sake
{

template< int N0, int N1 >
struct static_max_c
{
    static int const value = N1 < N0 ? N0 : N1;
    typedef static_max_c type;
};

template< class N0, class N1 >
struct static_max
    : static_max_c< N0::value, N1::value >
{ };

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_STATIC_MAX_HPP

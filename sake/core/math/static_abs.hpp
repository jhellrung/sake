/*******************************************************************************
 * sake/core/math/static_abs.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct static_abs<N>
 * struct static_abs_c<N>
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_STATIC_ABS_HPP
#define SAKE_CORE_MATH_STATIC_ABS_HPP

namespace sake
{

template< int N >
struct static_abs_c
{
    static unsigned int const value = N < 0 ? -N : N;
    typedef static_abs_c type;
};

template< class N >
struct static_abs
    : static_abs_c< N::value >
{ };

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_STATIC_ABS_HPP

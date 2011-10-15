/*******************************************************************************
 * sake/core/math/static_intlog2.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct static_intlog2<N>
 * struct static_intlog2_c<N>
 *
 * Metafunction evaluating to the floor of the base-2 logarithm of N.
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_STATIC_INTLOG2_HPP
#define SAKE_CORE_MATH_STATIC_INTLOG2_HPP

namespace sake
{

template< unsigned int N >
struct static_intlog2_c
{
    static unsigned int const value = 1 + static_intlog2_c< N/2 >::value;
    typedef static_intlog2_c type;
};

template<>
struct static_intlog2_c<1>
{
    static unsigned int const value = 0;
    typedef static_intlog2_c type;
};

template< class N >
struct static_intlog2
    : static_intlog2_c< N::value >
{ };

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_STATIC_INTLOG2_HPP

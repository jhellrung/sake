/*******************************************************************************
 * sake/core/math/static_sqr.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct static_sqr<N>
 * struct static_sqr_c<N>
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_STATIC_SQR_HPP
#define SAKE_CORE_MATH_STATIC_SQR_HPP

namespace sake
{

template< int N >
struct static_sqr_c
{
    static unsigned int const value = N * N;
    typedef static_sqr_c type;
};

template< class N >
struct static_sqr
    : static_sqr_c< N::value >
{ };

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_STATIC_SQR_HPP

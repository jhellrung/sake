/*******************************************************************************
 * sake/core/math/static_sign.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct static_sign<N>
 * struct static_sign_c<N>
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_STATIC_SIGN_HPP
#define SAKE_CORE_MATH_STATIC_SIGN_HPP

namespace sake
{

template< int N >
struct static_sign_c
{
    static int const value = (0 < N) - (N < 0);
    typedef static_sign_c type;
};

template< class N >
struct static_sign
    : static_sign_c< N::value >
{ };

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_STATIC_SIGN_HPP

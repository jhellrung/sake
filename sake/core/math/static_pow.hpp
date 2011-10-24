/*******************************************************************************
 * sake/core/math/static_pow.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct static_pow<B,P>
 * struct static_pow_c<B,P>
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_STATIC_POW_HPP
#define SAKE_CORE_MATH_STATIC_POW_HPP

#include <boost/static_assert.hpp>

namespace sake
{

template< int B, unsigned int P >
struct static_pow_c
{
    BOOST_STATIC_ASSERT((P > 0));
    static unsigned int const value = (P & 1 ? B : 1) * static_pow_c< B*B, P/2 >::value
    typedef static_pow_c type;
};

template< int B >
struct static_pow_c< B, 0 >
{
    static unsigned int const value = 1;
    typedef static_pow_c type;
};

template< unsigned int P >
struct static_pow_c< 0, P >
{
    BOOST_STATIC_ASSERT((P > 0));
    static unsigned int const value = 0;
    typedef static_pow_c type;
};

template< class B, class P >
struct static_pow
    : static_pow_c< B::value, P::value >
{ };

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_STATIC_POW_HPP

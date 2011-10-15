/*******************************************************************************
 * sake/core/math/iec559_traits.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_IEC559_TRAITS_HPP
#define SAKE_CORE_MATH_IEC559_TRAITS_HPP

#include <limits>

#include <boost/static_assert.hpp>

namespace sake
{

namespace iec559_traits_private
{

template< int N > struct exponent_bits;
template<> struct exponent_bits<  32 > { static int const value =  8; };
template<> struct exponent_bits<  64 > { static int const value = 11; };
template<> struct exponent_bits<  80 > { static int const value = 15; };
template<> struct exponent_bits< 128 > { static int const value = 15; };

} // namespace iec559_traits_private

template< class T >
struct iec559_traits
{
    BOOST_STATIC_ASSERT((std::numeric_limits<T>::is_iec559));
    BOOST_STATIC_ASSERT((std::numeric_limits<T>::radix == 2));

    static int const bits = sizeof( T ) * std::numeric_limits< unsigned char >::digits;

    static int const sign_bits = 1;
    static int const exponent_bits = iec559_traits_private::exponent_bits< bits >::value;
    static int const fraction_bits = bits - sign_bits - exponent_bits;

    static int const exponent_min = -((1 << (exponent_bits - 1)) - 2);
    static int const exponent_max = (1 << (exponent_bits - 1)) - 1;
    static int const exponent_bias = (1 << (exponent_bits - 1)) - 1;
};

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_IEC559_TRAITS_HPP

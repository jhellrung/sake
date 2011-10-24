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

template< int MaxExponent > struct exponent_bits;
// MSVC appears to have an incorrect max_exponent...
template<> struct exponent_bits<    15 > { static int const value =  5; };
template<> struct exponent_bits<    16 > { static int const value =  5; };
template<> struct exponent_bits<   127 > { static int const value =  8; };
template<> struct exponent_bits<   128 > { static int const value =  8; };
template<> struct exponent_bits<  1023 > { static int const value = 11; };
template<> struct exponent_bits<  1024 > { static int const value = 11; };
template<> struct exponent_bits< 16383 > { static int const value = 15; };
template<> struct exponent_bits< 16384 > { static int const value = 15; };

} // namespace iec559_traits_private

template< class T >
struct iec559_traits
{
    BOOST_STATIC_ASSERT((std::numeric_limits<T>::is_iec559));
    BOOST_STATIC_ASSERT((std::numeric_limits<T>::radix == 2));

    static int const sign_bits = 1;
    static int const exponent_bits =
        iec559_traits_private::exponent_bits< std::numeric_limits<T>::max_exponent >::value;
    static int const fraction_bits = std::numeric_limits<T>::digits - 1;
    static int const bits = sign_bits + exponent_bits + fraction_bits;

    static int const exponent_min = -((1 << (exponent_bits - 1)) - 2);
    static int const exponent_max = (1 << (exponent_bits - 1)) - 1;
    static int const exponent_bias = exponent_max;
};

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_IEC559_TRAITS_HPP

/*******************************************************************************
 * sake/core/math/float_ulp.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * float_ulp(T const x) -> T
 *
 * Returns the "unit in the last place" of a (positive) floating point number.
 * 1/2 * ulp of the floating point result of a floating point binary operation
 * (+, -, *, /) with round-to-nearest bounds the absolute error in the result.
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_FLOAT_ULP_HPP
#define SAKE_CORE_MATH_FLOAT_ULP_HPP

#include <limits>

#include <boost/math/special_functions/next.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_floating_point.hpp>

#include <sake/core/utility/assert.hpp>

namespace sake
{

template< class T >
inline T
float_ulp(T const x)
{
    BOOST_STATIC_ASSERT((std::numeric_limits<T>::is_specialized));
    BOOST_STATIC_ASSERT((std::numeric_limits<T>::has_infinity));
    BOOST_STATIC_ASSERT((boost::is_floating_point<T>::value));
    SAKE_ASSERT_RELATION(
        (( 0, <, x ))
        (( x, <, std::numeric_limits<T>::infinity ))
        (( x, ==, x ))
    );
    return boost::math::float_next(x) - x;
}

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_FLOAT_ULP_HPP

/*******************************************************************************
 * sake/core/math/float_ufp.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * float_ufp(T const x) -> T
 *
 * Returns the "unit in the first place" of a (positive) floating point number.
 * That is, float_ufp(x) returns a power-of-2 such that
 *     float_ufp(x) <= x < 2 * float_ufp(x)
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_FLOAT_UFP_HPP
#define SAKE_CORE_MATH_FLOAT_UFP_HPP

#include <limits>

#include <boost/static_assert.hpp>

#include <sake/core/math/float_ulp.hpp>
#include <sake/core/utility/assert.hpp>

namespace sake
{

template< class T >
inline T
float_ufp(T const x)
{
    BOOST_STATIC_ASSERT((std::numeric_limits<T>::is_iec559));
    static T const ulp_to_ufp_shifter = 1 / std::numeric_limits<T>::epsilon();
    static T const norm_min = std::numeric_limits<T>::min();
    static T const to_norm_shifter = 1 / norm_min;
    static T const adjusted_ulp_to_ufp_shifter = ulp_to_ufp_shifter * norm_min;
    SAKE_ASSERT_RELATION( to_norm_shifter * std::numeric_limits<T>::denorm_min(), >=, norm_min );
    SAKE_ASSERT_RELATION_ALL(
        (( 0, <, x ))
        (( x, <, std::numeric_limits<T>::infinity ))
        (( x, ==, x ))
    );
    return x >= norm_min ?
           ulp_to_ufp_shifter * float_ulp(x) :
           adjusted_ulp_to_ufp_shifter * float_ulp(to_norm_shifter * x);
}

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_FLOAT_UFP_HPP

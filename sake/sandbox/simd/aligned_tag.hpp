/*******************************************************************************
 * sake/core/math/simd/aligned_tag.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_SIMD_ALIGNED_TAG_HPP
#define SAKE_CORE_MATH_SIMD_ALIGNED_TAG_HPP

namespace sake
{

namespace simd
{

struct aligned_tag { };
aligned_tag const _aligned = { };

} // namespace simd

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_SIMD_ALIGNED_TAG_HPP

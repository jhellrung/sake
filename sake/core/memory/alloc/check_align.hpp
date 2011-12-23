/*******************************************************************************
 * sake/core/memory/alloc/check_align.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_MEMORY_ALLOC_CHECK_ALIGN_HPP
#define SAKE_CORE_MEMORY_ALLOC_CHECK_ALIGN_HPP

#include <cstddef>

namespace sake
{

namespace alloc
{

inline bool
check_align(std::size_t const obj_align)
{
    return 0 == obj_align
        || 0 == (obj_align & (obj_align - 1));
}

inline bool
check_align(std::size_t const obj_align, std::size_t const obj_size)
{
    return 0 == obj_align
        || (0 == (obj_align & (obj_align - 1))
         && 0 == (obj_size % obj_align));
}

} // namespace alloc

} // namespace sake

#endif // #ifndef SAKE_CORE_MEMORY_ALLOC_CHECK_ALIGN_HPP

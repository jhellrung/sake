/*******************************************************************************
 * sake/core/memory/vmem/unreserve.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * vmem::unreserve(void* p, std::size_t reserved_size) -> void
 *
 * Unreserves the specified region of pages, making the address space available
 * to the system for reuse.  p and reserved_size must be multiples of
 * vmem::get_granularity.
 ******************************************************************************/

#ifndef SAKE_CORE_MEMORY_VMEM_UNRESERVE_HPP
#define SAKE_CORE_MEMORY_VMEM_UNRESERVE_HPP

#include <cstddef>

#include <boost/config.hpp>

#include <sake/core/math/zero.hpp>
#include <sake/core/memory/vmem/get_granularity.hpp>
#include <sake/core/utility/assert.hpp>
#include <sake/core/utility/uintptr_t.hpp>

#ifndef BOOST_WINDOWS
#include <sys/mman.h>
#endif // #ifndef BOOST_WINDOWS

namespace sake
{

namespace vmem
{

#ifdef BOOST_WINDOWS
namespace windows
{
void
unreserve_helper(void* p, std::size_t reserved_size);
} // namespace windows
#endif // #ifdef BOOST_WINDOWS

inline void
unreserve(void* const p, std::size_t const reserved_size)
{
    SAKE_ASSERT_RELATION_ALL(
        (( reinterpret_cast< sake::uintptr_t >(p) % vmem::get_granularity(), ==, sake::zero ))
        (( reserved_size % vmem::get_granularity(), ==, sake::zero ))
        (( reserved_size, !=, sake::zero ))
    );
#ifdef BOOST_WINDOWS
    windows::unreserve_helper(p, reserved_size);
#else // #ifdef BOOST_WINDOWS
    SAKE_REQUIRE_RELATION( munmap(p, reserved_size), ==, sake::zero );
#endif // #ifdef BOOST_WINDOWS
}

} // namespace vmem

} // namespace sake

#endif // #ifndef SAKE_CORE_MEMORY_VMEM_UNRESERVE_HPP

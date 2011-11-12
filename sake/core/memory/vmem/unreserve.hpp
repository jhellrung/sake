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
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>

#include <sake/core/math/zero.hpp>
#include <sake/core/memory/vmem/get_granularity.hpp>
#include <sake/core/utility/assert.hpp>
#include <sake/core/utility/uintptr_t.hpp>

#ifdef BOOST_WINDOWS
#include <windows.h>
BOOST_STATIC_ASSERT((boost::is_same< LPVOID, void* >::value));
BOOST_STATIC_ASSERT((sizeof( SIZE_T ) == sizeof( std::size_t )));
#else // #ifdef BOOST_WINDOWS
#include <sys/mman.h>
#endif // #ifdef BOOST_WINDOWS

namespace sake
{

namespace vmem
{

inline void
unreserve(void* p, std::size_t reserved_size)
{
    SAKE_ASSERT_RELATION_ALL(
        (( reinterpret_cast< sake::uintptr_t >(p) % vmem::get_granularity(), ==, sake::zero ))
        (( reserved_size % vmem::get_granularity(), ==, sake::zero ))
        (( reserved_size, !=, sake::zero ))
    );

#ifdef BOOST_WINDOWS

    SAKE_REQUIRE_RELATION( VirtualFree(p, reserved_size, MEM_DECOMMIT), !=, sake::zero );
    do {
        MEMORY_BASIC_INFORMATION info;
        SAKE_REQUIRE_RELATION( VirtualQuery(p, &info, reserved_size), !=, sake::zero );
        SAKE_ASSERT_RELATION_ALL(
            (( info.BaseAddress, ==, p ))
            (( info.AllocationBase, ==, p ))
            (( info.RegionSize, <=, reserved_size ))
            (( info.State, ==, MEM_RESERVE ))
        );
        SAKE_REQUIRE_RELATION( VirtualFree(p, 0, MEM_RELEASE), !=, sake::zero );
        reserved_size -= info.RegionSize;
        p = static_cast< unsigned char* >(p) + info.RegionSize;
    } while(reserved_size != 0);

#else // #ifdef BOOST_WINDOWS

    SAKE_REQUIRE_RELATION( munmap(p, reserved_size), ==, sake::zero );

#endif // #ifdef BOOST_WINDOWS

}

} // namespace vmem

} // namespace sake

#endif // #ifndef SAKE_CORE_MEMORY_VMEM_UNRESERVE_HPP

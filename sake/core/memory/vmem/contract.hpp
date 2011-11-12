/*******************************************************************************
 * sake/core/memory/vmem/contract.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * vmem::contract(void* p, std::size_t reserved_size, std::size_t new_size) -> std::size_t
 *
 * Attempts to contract backward the reserved address space specified by
 * (p, reserved_size).  Returns the new size of the address space.
 ******************************************************************************/

#ifndef SAKE_CORE_MEMORY_VMEM_CONTRACT_HPP
#define SAKE_CORE_MEMORY_VMEM_CONTRACT_HPP

#include <cstddef>

#include <boost/config.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>

#include <sake/core/math/zero.hpp>
#include <sake/core/memory/vmem/get_granularity.hpp>
#include <sake/core/memory/vmem/unreserve.hpp>
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

inline std::size_t
contract(void* const p, std::size_t const reserved_size, std::size_t new_size)
{
    static std::size_t const gm1 = vmem::get_granularity() - 1;
    SAKE_ASSERT_RELATION_ALL(
        (( reinterpret_cast< sake::uintptr_t >(p) % vmem::get_granularity(), ==, sake::zero ))
        (( reserved_size, !=, sake::zero ))
        (( reserved_size % vmem::get_granularity(), ==, sake::zero ))
    );
    SAKE_ASSERT_RELATION( new_size, !=, sake::zero );
    new_size = (new_size + gm1) & ~gm1;
    if(reserved_size <= new_size)
        return reserved_size;
    void* p_trim = static_cast< unsigned char* >(p) + new_size;
    std::size_t trim_size = reserved_size - new_size;

#ifdef BOOST_WINDOWS

    do {
        MEMORY_BASIC_INFORMATION info;
        SAKE_REQUIRE_RELATION( VirtualQuery(p_trim, &info, trim_size), !=, sake::zero );
        SAKE_ASSERT_RELATION_ALL(
            (( info.BaseAddress, ==, p_trim ))
            (( info.AllocationBase, <=, p_trim ))
            (( info.RegionSize, <=, trim_size ))
            (( info.State, !=, MEM_FREE ))
        );
        if(info.AllocationBase == p_trim) {
            vmem::unreserve(p_trim, trim_size);
            return reserved_size - trim_size;
        }
        p_trim = static_cast< unsigned char* >(p_trim) + info.RegionSize;
        trim_size -= info.RegionSize;
    } while(trim_size != 0);
    return reserved_size;

#else // #ifdef BOOST_WINDOWS

    SAKE_REQUIRE_RELATION( munmap(p_trim, trim_size), ==, sake::zero );
    return new_size;

#endif // #ifdef BOOST_WINDOWS
}

} // namespace vmem

} // namespace sake

#endif // #ifndef SAKE_CORE_MEMORY_VMEM_CONTRACT_HPP

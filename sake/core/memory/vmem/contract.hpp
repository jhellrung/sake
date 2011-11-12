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
std::size_t
contract_helper(void* p_trim, std::size_t trim_size, std::size_t const reserved_size);
} // namespace windows
#endif // #ifdef BOOST_WINDOWS

inline std::size_t
contract(void* const p, std::size_t const reserved_size, std::size_t new_size)
{
    std::size_t const gm1 = vmem::get_granularity() - 1;
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
    return windows::contract_helper(p_trim, trim_size, reserved_size);
#else // #ifdef BOOST_WINDOWS
    SAKE_REQUIRE_RELATION( munmap(p_trim, trim_size), ==, sake::zero );
    return new_size;
#endif // #ifdef BOOST_WINDOWS

}

} // namespace vmem

} // namespace sake

#endif // #ifndef SAKE_CORE_MEMORY_VMEM_CONTRACT_HPP

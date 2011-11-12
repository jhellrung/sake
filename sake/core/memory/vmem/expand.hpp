/*******************************************************************************
 * sake/core/memory/vmem/expand.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * vmem::expand(void* p, std::size_t reserved_size, std::size_t new_size) -> std::size_t
 *
 * Attempts to contiguously expand forward the reserved address space specified
 * by (p, reserved_size).  Returns the new size of the address space.
 ******************************************************************************/

#ifndef SAKE_CORE_MEMORY_VMEM_EXPAND_HPP
#define SAKE_CORE_MEMORY_VMEM_EXPAND_HPP

#include <cstddef>

#include <boost/config.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>

#include <sake/core/math/zero.hpp>
#include <sake/core/memory/vmem/get_granularity.hpp>
#include <sake/core/utility/assert.hpp>
#include <sake/core/utility/nullptr.hpp>
#include <sake/core/utility/uintptr_t.hpp>

#ifdef BOOST_WINDOWS
#include <windows.h>
BOOST_STATIC_ASSERT((boost::is_same< LPVOID, void* >::value));
BOOST_STATIC_ASSERT((sizeof( SIZE_T ) == sizeof( std::size_t )));
#else // #ifdef BOOST_WINDOWS
#include <sys/mman.h>
#if   defined( MREMAP_MAYMOVE )
#define mremap_(p, old_size, new_size, flags) mremap(p, old_size, new_size, flags)
#elif defined( POSIX_MREMAP_MAYMOVE )
#define mremap_(p, old_size, new_size, flags) posix_mremap(p, old_size, new_size, flags)
#else // #if defined( ... )
#define mremap_(p, old_size, new_size, flags) ( MAP_FAILED )
#endif // #if defined( ... )
#endif // #ifdef BOOST_WINDOWS

namespace sake
{

namespace vmem
{

inline std::size_t
expand(void* p, std::size_t const reserved_size, std::size_t new_size)
{
    static std::size_t const gm1 = vmem::get_granularity() - 1;
    SAKE_ASSERT_RELATION_ALL(
        (( reinterpret_cast< sake::uintptr_t >(p) % vmem::get_granularity(), ==, sake::zero ))
        (( reserved_size, !=, sake::zero ))
        (( reserved_size % vmem::get_granularity(), ==, sake::zero ))
    );
    if(new_size <= reserved_size)
        return reserved_size;
    new_size = (new_size + gm1) & ~gm1;
#ifdef BOOST_WINDOWS
    p = static_cast< unsigned char* >(p) + reserved_size;
    return VirtualAlloc(p, new_size - reserved_size, MEM_RESERVE, PAGE_NOACCESS) == nullptr ? reserved_size : new_size;
#else // #ifdef BOOST_WINDOWS
    static int const flags = 0;
    return mremap_(p, reserved_size, new_size, flags) == MAP_FAILED ? reserved_size : new_size;
#endif // #ifdef BOOST_WINDOWS
}

} // namespace vmem

} // namespace sake

#ifdef mremap_
#undef mremap_
#endif // #ifdef mremap_

#endif // #ifndef SAKE_CORE_MEMORY_VMEM_EXPAND_HPP

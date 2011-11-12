/*******************************************************************************
 * sake/core/memory/vmem/decommit.hpp
 *
 * Copyright 2010, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * vmem::decommit(void* p, std::size_t size) -> void
 *
 * Decommits the specific region of pages, such that the associated physical
 * pages of memory are returned back to the system.  Note that the address space
 * is still in a reserved state.  p and size must be multiplies of
 * vmem::get_page_size.
 ******************************************************************************/

#ifndef SAKE_CORE_MEMORY_VMEM_DECOMMIT_HPP
#define SAKE_CORE_MEMORY_VMEM_DECOMMIT_HPP

#include <cstddef>

#include <boost/config.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>

#include <sake/core/math/zero.hpp>
#include <sake/core/memory/vmem/get_page_size.hpp>
#include <sake/core/utility/assert.hpp>
#include <sake/core/utility/uintptr_t.hpp>

#ifdef BOOST_WINDOWS
#include <windows.h>
BOOST_STATIC_ASSERT((boost::is_same< LPVOID, void* >::value));
BOOST_STATIC_ASSERT((sizeof( SIZE_T ) == sizeof( std::size_t )));
#else // #ifdef BOOST_WINDOWS
#include <sys/mman.h>
#if   defined( MADV_DONTNEED )
#define madvise_( p, size ) madvise( p, size, MADV_DONTNEED )
#elif defined( POSIX_MADV_DONTNEED )
#define madvise_( p, size ) posix_madvise( p, size, POSIX_MADV_DONTNEED )
#else // #if defined( ... )
#define madvise_( p, size ) ( 0 )
#endif // #if defined( ... )
#endif // #ifdef BOOST_WINDOWS

namespace sake
{

namespace vmem
{

inline void
decommit(void* const p, std::size_t const size)
{
    SAKE_ASSERT_RELATION_ALL(
        (( reinterpret_cast< sake::uintptr_t >(p) % vmem::get_page_size(), ==, sake::zero ))
        (( size, !=, sake::zero ))
        (( size % vmem::get_page_size(), ==, sake::zero ))
    );
#ifdef BOOST_WINDOWS
    SAKE_REQUIRE_RELATION( VirtualFree(p, size, MEM_DECOMMIT), !=, sake::zero );
#else // #ifdef BOOST_WINDOWS
    SAKE_REQUIRE_RELATION( madvise_(p, size), ==, sake::zero )
#endif // #ifdef BOOST_WINDOWS
}

} // namespace vmem

} // namespace sake

#ifdef madvise_
#undef madvise_
#endif // #ifdef madvise_

#endif // #ifndef SAKE_CORE_MEMORY_VMEM_DECOMMIT_HPP

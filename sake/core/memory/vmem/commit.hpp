/*******************************************************************************
 * sake/core/memory/vmem/commit.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * vmem::commit(void* p, std::size_t size) -> bool
 *
 * Commits (associates physical memory pages with) the specified region of
 * pages.  These must have been previously reserved via vmem::reserve.  p and
 * size must be multiplies of vmem::get_page_size.
 ******************************************************************************/

#ifndef SAKE_CORE_MEMORY_VMEM_COMMIT_HPP
#define SAKE_CORE_MEMORY_VMEM_COMMIT_HPP

#include <cstddef>

#include <boost/config.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>

#include <sake/core/math/zero.hpp>
#include <sake/core/memory/vmem/get_page_size.hpp>
#include <sake/core/utility/assert.hpp>
#include <sake/core/utility/nullptr.hpp>
#include <sake/core/utility/uintptr_t.hpp>

#ifdef BOOST_WINDOWS
#include <windows.h>
BOOST_STATIC_ASSERT((boost::is_same< LPVOID, void* >::value));
BOOST_STATIC_ASSERT((sizeof( SIZE_T ) == sizeof( std::size_t )));
#endif // #ifdef BOOST_WINDOWS

namespace sake
{

namespace vmem
{

inline bool
commit(void* const p, std::size_t const size)
{
    SAKE_ASSERT_RELATION_ALL(
        (( reinterpret_cast< sake::uintptr_t >(p) % vmem::get_page_size(), ==, sake::zero ))
        (( size, !=, sake::zero ))
        (( size % vmem::get_page_size(), ==, sake::zero ))
    );
#ifdef BOOST_WINDOWS
    return VirtualAlloc(p, size, MEM_COMMIT, PAGE_READWRITE) != nullptr;
#else // #ifdef BOOST_WINDOWS
    return true;
#endif // #ifdef BOOST_WINDOWS
}

} // namespace vmem

} // namespace sake

#endif // #ifndef SAKE_CORE_MEMORY_VMEM_COMMIT_HPP

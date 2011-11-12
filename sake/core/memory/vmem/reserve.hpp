/*******************************************************************************
 * sake/core/memory/vmem/reserve.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * vmem::reserve(std::size_t size) -> sake::tuple< void*, std::size_t >
 *
 * Reserves a region of address space of at least the given size (in bytes).
 ******************************************************************************/

#ifndef SAKE_CORE_MEMORY_VMEM_RESERVE_HPP
#define SAKE_CORE_MEMORY_VMEM_RESERVE_HPP

#include <cstddef>

#include <boost/config.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>

#include <sake/core/container/tuple.hpp>
#include <sake/core/math/zero.hpp>
#include <sake/core/memory/vmem/get_granularity.hpp>
#include <sake/core/utility/assert.hpp>
#include <sake/core/utility/nullptr.hpp>

#ifdef BOOST_WINDOWS
#include <windows.h>
BOOST_STATIC_ASSERT((boost::is_same< LPVOID, void* >::value));
BOOST_STATIC_ASSERT((sizeof( SIZE_T ) == sizeof( std::size_t )));
#else // #ifdef BOOST_WINDOWS
#include <sys/mman.h>
#if   defined( MAP_ANONYMOUS )
#define MAP_ANONYMOUS_ MAP_ANONYMOUS
#elif defined( MAP_ANON )
#define MAP_ANONYMOUS_ MAP_ANON
#else // #if defined( ... )
#include <fcntl.h> // open, O_RDWR
#endif // #if defined( ... )
#endif // #ifdef BOOST_WINDOWS

namespace sake
{

namespace vmem
{

inline sake::tuple< void*, std::size_t >
reserve(std::size_t size)
{
    SAKE_ASSERT_RELATION( size, !=, sake::zero );
    std::size_t const gm1 = vmem::get_granularity() - 1;
    size = (size + gm1) & ~gm1;

#ifdef BOOST_WINDOWS

    void* const p = VirtualAlloc(nullptr, size, MEM_RESERVE, PAGE_NOACCESS);
    if(!p)
        size = 0;

#else // #ifdef BOOST_WINDOWS

#ifdef MAP_ANONYMOUS_
    static int const flags = MAP_PRIVATE | MAP_ANONYMOUS_;
    static int const fildes = -1;
#else // #ifdef MAP_ANONYMOUS_
    static int const flags = MAP_PRIVATE;
    static int const fildes = open("/dev/zero", O_RDWR);
#endif // #ifdef MAP_ANONYMOUS_
    static int const offset = 0;
    void* p = mmap(nullptr, size, PROT_READ | PROT_WRITE, flags, fildes, offset);
    if(p == MAP_FAILED) {
        p = nullptr;
        size = 0;
    }

#endif // #ifdef BOOST_WINDOWS

    return sake::make_tuple(p, size);
}

} // namespace vmem

} // namespace sake

#ifdef MAP_ANONYMOUS_
#undef MAP_ANONYMOUS_
#endif // #ifdef MAP_ANONYMOUS_

#endif // #ifndef SAKE_CORE_MEMORY_VMEM_RESERVE_HPP

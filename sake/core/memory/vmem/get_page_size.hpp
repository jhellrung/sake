/*******************************************************************************
 * sake/core/memory/vmem/get_page_size.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_MEMORY_VMEM_GET_PAGE_SIZE_HPP
#define SAKE_CORE_MEMORY_VMEM_GET_PAGE_SIZE_HPP

#include <cstddef>

#include <boost/config.hpp>

#include <sake/core/math/zero.hpp>
#include <sake/core/utility/assert.hpp>

#ifdef BOOST_WINDOWS
#include <sake/core/utility/windows/get_system_info.hpp>
#else // #ifdef BOOST_WINDOWS
#include <unistd.h>
#endif // #ifdef BOOST_WINDOWS

namespace sake
{

namespace vmem
{

namespace get_page_size_private
{

inline std::size_t
helper()
{
    std::size_t const result = static_cast< std::size_t >(
#ifdef BOOST_WINDOWS
        windows::get_system_info().dwPageSize
#else // #ifdef BOOST_WINDOWS
#if   defined( _SC_PAGESIZE )
        sysconf( _SC_PAGESIZE )
#elif defined( _SC_PAGE_SIZE )
        sysconf( _SC_PAGE_SIZE )
#else // #if defined( ... )
#error One of BOOST_WINDOWS, _SC_PAGESIZE, or _SC_PAGE_SIZE must be defined.
#endif // #if defined( ... )
#endif // #ifdef BOOST_WINDOWS
    );
    SAKE_ASSERT_RELATION( result, !=, sake::zero );
    SAKE_ASSERT_RELATION( result & (result - 1), ==, sake::zero );
    return result;
}

} // namespace get_page_size_private

inline std::size_t
get_page_size()
{
    static std::size_t const result = get_page_size_private::helper();
    return result;
}

} // namespace vmem

} // namespace sake

#endif // #ifndef SAKE_CORE_MEMORY_VMEM_GET_PAGE_SIZE_HPP

/*******************************************************************************
 * sake/core/memory/vmem/get_granularity.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_MEMORY_VMEM_GET_GRANULARITY_HPP
#define SAKE_CORE_MEMORY_VMEM_GET_GRANULARITY_HPP

#include <cstddef>

#include <boost/config.hpp>

#include <sake/core/memory/vmem/get_page_size.hpp>
#include <sake/core/utility/assert.hpp>

#ifdef BOOST_WINDOWS
#include <sake/core/utility/windows/get_system_info.hpp>
#endif // #ifdef BOOST_WINDOWS

namespace sake
{

namespace vmem
{

namespace get_granularity_private
{

inline std::size_t
helper()
{
    std::size_t const result =
#ifdef BOOST_WINDOWS
        static_cast< std::size_t >(windows::get_system_info().dwAllocationGranularity);
#else // #ifdef BOOST_WINDOWS
        vmem::get_page_size();
#endif // #ifdef BOOST_WINDOWS
    SAKE_ASSERT_RELATION( result, !=, 0 );
    SAKE_ASSERT_RELATION( result & (result - 1), ==, 0 );
    SAKE_ASSERT_RELATION( result % vmem::get_page_size(), ==, 0 );
    return result;
}

} // namespace get_granularity_private

inline std::size_t
get_granularity()
{
    static std::size_t const result = get_granularity_private::helper();
    return result;
}

} // namespace vmem

} // namespace sake

#endif // #ifndef SAKE_CORE_MEMORY_VMEM_GET_GRANULARITY_HPP

/*******************************************************************************
 * libs/core/memory/vmem/windows/unreserve_helper.cpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <boost/config.hpp>

#ifdef BOOST_WINDOWS

#include <cstddef>

#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>

#include <sake/core/math/zero.hpp>
#include <sake/core/memory/vmem/unreserve.hpp>
#include <sake/core/utility/assert.hpp>

#include <windows.h>
BOOST_STATIC_ASSERT((boost::is_same< LPVOID, void* >::value));
BOOST_STATIC_ASSERT((sizeof( SIZE_T ) == sizeof( std::size_t )));

namespace sake
{

namespace vmem
{

namespace windows
{

void
unreserve_helper(void* p, std::size_t reserved_size)
{
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
}

} // namespace windows

} // namespace vmem

} // namespace sake

#endif // #ifdef BOOST_WINDOWS

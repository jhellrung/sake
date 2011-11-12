/*******************************************************************************
 * libs/core/memory/vmem/windows/contract_helper.cpp
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

std::size_t
contract_helper(void* p_trim, std::size_t trim_size, std::size_t const reserved_size)
{
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
}

} // namespace windows

} // namespace vmem

} // namespace sake

#endif // #ifdef BOOST_WINDOWS

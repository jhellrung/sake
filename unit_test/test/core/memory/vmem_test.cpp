/*******************************************************************************
 * unit_test/test/core/memory/vmem_test.cpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <cstddef>

#include <sake/core/math/zero.hpp>
#include <sake/core/memory/vmem/commit.hpp>
#include <sake/core/memory/vmem/contract.hpp>
#include <sake/core/memory/vmem/decommit.hpp>
#include <sake/core/memory/vmem/expand.hpp>
#include <sake/core/memory/vmem/get_granularity.hpp>
#include <sake/core/memory/vmem/get_page_size.hpp>
#include <sake/core/memory/vmem/reserve.hpp>
#include <sake/core/memory/vmem/unreserve.hpp>

#include <sake/boost_ext/fusion/sequence/unpack.hpp>

#include <sake/test/environment.hpp>
#include <sake/test/test.hpp>

namespace sake_unit_test
{

void vmem_test(sake::test::environment& env)
{
    std::size_t const reserve_request_size = 3 * sake::vmem::get_granularity() + 1;
    SAKE_BOOST_EXT_FUSION_UNPACK(
        ( void* const p_reserve ) ( std::size_t const reserve_size ),
        sake::vmem::reserve(reserve_request_size)
    )
    SAKE_TEST_CHECK_RELATION( env, reserve_request_size, <=, reserve_size );
    SAKE_TEST_CHECK_RELATION( env, reserve_size % sake::vmem::get_granularity(), ==, sake::zero );
    SAKE_TEST_CHECK_RELATION( env, reserve_size, ==, 4 * sake::vmem::get_granularity() );
    std::size_t const commit_size = 2 * sake::vmem::get_page_size();
    bool const commit_result = sake::vmem::commit(p_reserve, commit_size);
    SAKE_TEST_CHECK( env, commit_result );
    for(std::size_t i = 0; i != commit_size; ++i)
        static_cast< unsigned char* >(p_reserve)[i] = static_cast< unsigned char >(i);
    for(std::size_t i = 0; i != commit_size; ++i)
        SAKE_TEST_CHECK_RELATION( env,
            static_cast< unsigned char* >(p_reserve)[i],
            ==,
            static_cast< unsigned char >(i)
        );
    sake::vmem::decommit(p_reserve, commit_size);
    sake::vmem::unreserve(p_reserve, reserve_size);
}

} // namespace sake_unit_test

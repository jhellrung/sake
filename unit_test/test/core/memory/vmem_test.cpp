/*******************************************************************************
 * unit_test/test/core/memory/vmem_test.cpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <sake/core/memory/vmem/commit.hpp>
#include <sake/core/memory/vmem/contract.hpp>
#include <sake/core/memory/vmem/decommit.hpp>
#include <sake/core/memory/vmem/expand.hpp>
#include <sake/core/memory/vmem/reserve.hpp>
#include <sake/core/memory/vmem/unreserve.hpp>

#include <sake/test/environment.hpp>
#include <sake/test/test.hpp>

namespace sake_unit_test
{

void vmem_test(sake::test::environment& env)
{
    static_cast< void >(env);
}

} // namespace sake_unit_test

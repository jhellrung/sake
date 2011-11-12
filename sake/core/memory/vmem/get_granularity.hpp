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

namespace sake
{

namespace vmem
{

std::size_t
get_granularity();

} // namespace vmem

} // namespace sake

#endif // #ifndef SAKE_CORE_MEMORY_VMEM_GET_GRANULARITY_HPP

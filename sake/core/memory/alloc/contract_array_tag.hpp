/*******************************************************************************
 * sake/core/memory/alloc/contract_array_tag.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct alloc::contract_array_tag
 ******************************************************************************/

#ifndef SAKE_CORE_MEMORY_ALLOC_CONTRACT_ARRAY_TAG_HPP
#define SAKE_CORE_MEMORY_ALLOC_CONTRACT_ARRAY_TAG_HPP

namespace sake
{

namespace alloc
{

struct contract_array_tag
{ enum type { fwd, bwd, fwd_bwd, bwd_fwd }; };

} // namespace alloc

} // namespace sake

#endif // #ifndef SAKE_CORE_MEMORY_ALLOC_CONTRACT_ARRAY_TAG_HPP

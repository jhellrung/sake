/*******************************************************************************
 * sake/core/memory/alloc/error.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_MEMORY_ALLOC_ERROR_HPP
#define SAKE_CORE_MEMORY_ALLOC_ERROR_HPP

#include <sake/boost_ext/exception/define_exception.hpp>

namespace sake
{

namespace alloc
{

SAKE_BOOST_EXT_DEFINE_EXCEPTION( sake::alloc, error )

} // namespace alloc

} // namespace sake

#endif // #ifndef SAKE_CORE_MEMORY_ALLOC_ERROR_HPP

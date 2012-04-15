/*******************************************************************************
 * sake/core/memory/alloc/throw_if_false.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_MEMORY_ALLOC_THROW_IF_FALSE_HPP
#define SAKE_CORE_MEMORY_ALLOC_THROW_IF_FALSE_HPP

#include <boost/throw_exception.hpp>

#include <sake/core/memory/alloc/error.hpp>

namespace sake
{

namespace alloc
{

template< class T >
inline T
throw_if_false(T const x)
{
    if(!x)
        boost::throw_exception(sake::alloc::error());
    return x;
}

} // namespace alloc

} // namespace sake

#endif // #ifndef SAKE_CORE_MEMORY_ALLOC_THROW_IF_FALSE_HPP

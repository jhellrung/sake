/*******************************************************************************
 * sake/core/utility/dispatch_priority_tag.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_DISPATCH_PRIORITY_TAG_HPP
#define SAKE_CORE_UTILITY_DISPATCH_PRIORITY_TAG_HPP

namespace sake
{

template< int N >
struct dispatch_priority_tag
    : dispatch_priority_tag< N-1 >
{ };

template<>
struct dispatch_priority_tag<0>
{ };

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_DISPATCH_PRIORITY_TAG_HPP

/*******************************************************************************
 * sake/core/range/private/introterminal_of_begin_end.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_PRIVATE_INTROTERMINAL_OF_BEGIN_END_HPP
#define SAKE_CORE_RANGE_PRIVATE_INTROTERMINAL_OF_BEGIN_END_HPP

#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/iterator/categories.hpp>

namespace sake
{

namespace range
{

namespace private_
{

template< class Begin, class End >
struct introterminal_of_begin_end
{ typedef sake::null_introterminal_tag type; };

template< class End >
struct introterminal_of_begin_end< sake::begin_tag, End >
{ typedef sake::begin_access_introterminal_tag type; };

template< class Begin >
struct introterminal_of_begin_end< Begin, sake::end_tag >
{ typedef sake::end_access_introterminal_tag type; };

template<>
struct introterminal_of_begin_end< sake::begin_tag, sake::end_tag >
{ typedef sake::begin_access_end_access_introterminal_tag type; };

} // namespace private_

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_PRIVATE_INTROTERMINAL_OF_BEGIN_END_HPP

/*******************************************************************************
 * sake/core/range/private/introversal_from_begin_end.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_PRIVATE_INTROVERSAL_FROM_BEGIN_END_HPP
#define SAKE_CORE_RANGE_PRIVATE_INTROVERSAL_FROM_BEGIN_END_HPP

#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/iterator/categories.hpp>

namespace sake
{

namespace range
{

namespace private_
{

template< class Begin, class End >
struct introversal_from_begin_end
{ typedef sake::null_introversal_tag type; };

template< class End >
struct introversal_from_begin_end< sake::begin_tag, End >
{ typedef sake::begin_access_introversal_tag type; };

template< class Begin >
struct introversal_from_begin_end< Begin, sake::end_tag >
{ typedef sake::end_access_introversal_tag type; };

template<>
struct introversal_from_begin_end< sake::begin_tag, sake::end_tag >
{ typedef sake::begin_access_end_access_introversal_tag type; };

} // namespace private_

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_PRIVATE_INTROVERSAL_FROM_BEGIN_END_HPP

/*******************************************************************************
 * sake/core/range/archetypes/range.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct archetypes::range< T, Traversal, Access >
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_ARCHETYPES_RANGE_HPP
#define SAKE_CORE_RANGE_ARCHETYPES_RANGE_HPP

#include <boost/mpl/placeholders.hpp>

#include <sake/core/iterator/archetypes/iterator.hpp>
#include <sake/core/range/archetypes/private/traversal_base.hpp>

namespace sake
{

namespace archetypes
{

template< class T, class Traversal, class Access >
struct range
    : sake::archetypes::range_private::traversal_base<
          Traversal,
          sake::archetypes::iterator< T, Traversal, boost::mpl::_1, Access >
      >
{ };

} // namespace archetypes

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_ARCHETYPES_RANGE_HPP

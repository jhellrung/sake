/*******************************************************************************
 * sake/core/iterator/archetypes/access.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct iterator_archetypes::readable< T, Traversal = ... >
 * struct iterator_archetypes::writable< T, Traversal = ... >
 * struct iterator_archetypes::swappable< T, Traversal = ... >
 * struct iterator_archetypes::readable_writable< T, Traversal = ... >
 * struct iterator_archetypes::readable_lvalue< T, Traversal = ... >
 * struct iterator_archetypes::writable_lvalue< T, Traversal = ... >
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_ARCHETYPES_ACCESS_HPP
#define SAKE_CORE_ITERATOR_ARCHETYPES_ACCESS_HPP

#include <boost/iterator/iterator_categories.hpp>

#include <sake/core/iterator/archetypes/access_tag.hpp>
#include <sake/core/iterator/archetypes/iterator.hpp>

namespace sake
{

namespace iterator_archetypes
{

#define define_archetype( access ) \
template< class T, class Traversal = boost::incrementable_traversal_tag > \
struct access \
{ typedef sake::archetypes::iterator< \
    T, Traversal, sake::iterator_archetypes::access ## _tag > type; };
define_archetype( readable )
define_archetype( writable )
define_archetype( swappable )
define_archetype( readable_writable )
define_archetype( readable_lvalue )
define_archetype( writable_lvalue )
#undef define_archetype

} // namespace iterator_archetypes

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_ARCHETYPES_ACCESS_HPP

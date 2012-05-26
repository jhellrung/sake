/*******************************************************************************
 * sake/core/iterator/archetypes/traversal.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct iterator_archetypes::incrementable< T, Access = ... >
 * struct iterator_archetypes::single_pass< T, Access = ... >
 * struct iterator_archetypes::forward< T, Access = ... >
 * struct iterator_archetypes::bidirectional< T, Access = ... >
 * struct iterator_archetypes::random_access< T, Access = ... >
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_ARCHETYPES_TRAVERSAL_HPP
#define SAKE_CORE_ITERATOR_ARCHETYPES_TRAVERSAL_HPP

#include <boost/iterator/iterator_categories.hpp>

#include <sake/core/iterator/archetypes/access_tag.hpp>
#include <sake/core/iterator/archetypes/iterator.hpp>

namespace sake
{

namespace iterator_archetypes
{

#define define_archetype( traversal ) \
template< class T, class Access = sake::iterator_archetypes::readable_tag > \
struct traversal \
    : sake::archetypes::iterator< \
          T, Access, boost::traversal ## _traversal_tag > \
{ };
define_archetype( incrementable )
define_archetype( single_pass )
define_archetype( forward )
define_archetype( bidirectional )
define_archetype( random_access )
#undef define_archetype

} // namespace iterator_archetypes

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_ARCHETYPES_TRAVERSAL_HPP

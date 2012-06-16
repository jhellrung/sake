/*******************************************************************************
 * sake/core/iterator/archetypes/traversal.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct iterator::archetypes::incrementable< T, IV = ..., A = ... >
 * struct iterator::archetypes::single_pass< T, IV = ..., A = ... >
 * struct iterator::archetypes::forward< T, IV = ..., A = ... >
 * struct iterator::archetypes::bidirectional< T, IV = ..., A = ... >
 * struct iterator::archetypes::random_access< T, IV = ..., A = ... >
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_ARCHETYPES_TRAVERSAL_HPP
#define SAKE_CORE_ITERATOR_ARCHETYPES_TRAVERSAL_HPP

#include <sake/core/iterator/archetypes/access_tag.hpp>
#include <sake/core/iterator/archetypes/iterator.hpp>
#include <sake/core/iterator/categories.hpp>

namespace sake
{

namespace iterator
{

namespace archetypes
{

#define define_archetype( traversal ) \
template< \
    class T, \
    class Introversal = sake::null_introversal_tag, \
    class Access = sake::iterator::archetypes::readable_tag \
> \
struct traversal \
{ \
    typedef sake::archetypes::iterator< \
        T, \
        boost::traversal ## _traversal_tag, \
        Introversal, \
        Access \
    > type; \
};
define_archetype( incrementable )
define_archetype( single_pass )
define_archetype( forward )
define_archetype( bidirectional )
define_archetype( random_access )
#undef define_archetype

} // namespace archetypes

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_ARCHETYPES_TRAVERSAL_HPP

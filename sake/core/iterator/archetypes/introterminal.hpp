/*******************************************************************************
 * sake/core/iterator/archetypes/introterminal.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct iterator::archetypes::begin_detect<
 *     T, Traversal = ..., Access = ... >
 * struct iterator::archetypes::begin_access<
 *     T, Traversal = ..., Access = ... >
 * struct iterator::archetypes::end_detect<
 *     T, Traversal = ..., Access = ... >
 * struct iterator::archetypes::end_access<
 *     T, Traversal = ..., Access = ... >
 * struct iterator::archetypes::begin_detect_end_detect<
 *     T, Traversal = ..., Access = ... >
 * struct iterator::archetypes::begin_access_end_detect<
 *     T, Traversal = ..., Access = ... >
 * struct iterator::archetypes::begin_detect_end_access<
 *     T, Traversal = ..., Access = ... >
 * struct iterator::archetypes::begin_access_end_access<
 *     T, Traversal = ..., Access = ... >
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_ARCHETYPES_INTROTERMINAL_HPP
#define SAKE_CORE_ITERATOR_ARCHETYPES_INTROTERMINAL_HPP

#include <sake/core/iterator/archetypes/access_tag.hpp>
#include <sake/core/iterator/archetypes/iterator.hpp>
#include <sake/core/iterator/categories.hpp>

namespace sake
{

namespace iterator
{

namespace archetypes
{

#define define_archetype( introterminal ) \
template< \
    class T, \
    class Traversal = boost::incrementable_traversal_tag, \
    class Access = sake::iterator::archetypes::readable_tag \
> \
struct introterminal \
{ \
    typedef sake::archetypes::iterator< \
        T, \
        Traversal, \
        sake::introterminal ## _introterminal_tag, \
        Access \
    > type; \
};
define_archetype( begin_detect )
define_archetype( begin_access )
define_archetype( end_detect )
define_archetype( end_access )
define_archetype( begin_detect_end_detect )
define_archetype( begin_access_end_detect )
define_archetype( begin_detect_end_access )
define_archetype( begin_access_end_access )
#undef define_archetype

} // namespace archetypes

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_ARCHETYPES_INTROTERMINAL_HPP

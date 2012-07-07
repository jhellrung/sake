/*******************************************************************************
 * sake/core/iterator/archetypes/access.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct iterator::archetypes::readable<
 *     T, Traversal = ..., Introterminal = ... >
 * struct iterator::archetypes::writable<
 *     T, Traversal = ..., Introterminal = ... >
 * struct iterator::archetypes::swappable<
 *     T, Traversal = ..., Introterminal = ... >
 * struct iterator::archetypes::readable_writable<
 *     T, Traversal = ..., Introterminal = ... >
 * struct iterator::archetypes::readable_lvalue<
 *     T, Traversal = ..., Introterminal = ... >
 * struct iterator::archetypes::writable_lvalue<
 *     T, Traversal = ..., Introterminal = ... >
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_ARCHETYPES_ACCESS_HPP
#define SAKE_CORE_ITERATOR_ARCHETYPES_ACCESS_HPP

#include <sake/core/iterator/archetypes/access_tag.hpp>
#include <sake/core/iterator/archetypes/iterator.hpp>
#include <sake/core/iterator/categories.hpp>

namespace sake
{

namespace iterator
{

namespace archetypes
{

#define define_archetype( access ) \
template< \
    class T, \
    class Traversal = boost::incrementable_traversal_tag, \
    class Introterminal = sake::null_introterminal_tag \
> \
struct access \
{ \
    typedef sake::archetypes::iterator< \
        T, \
        Traversal, \
        Introterminal, \
        sake::iterator::archetypes::access ## _tag \
    > type; \
};
define_archetype( readable )
define_archetype( writable )
define_archetype( swappable )
define_archetype( readable_writable )
define_archetype( readable_lvalue )
define_archetype( writable_lvalue )
#undef define_archetype

} // namespace archetypes

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_ARCHETYPES_ACCESS_HPP

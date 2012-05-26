/*******************************************************************************
 * sake/core/iterator/archetypes/access_tag.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct iterator_archetypes::readable_tag
 * struct iterator_archetypes::writable_tag
 * struct iterator_archetypes::swappable_tag
 * struct iterator_archetypes::readable_writable_tag
 * struct iterator_archetypes::lvalue_tag
 * struct iterator_archetypes::readable_lvalue_tag
 * struct iterator_archetypes::writable_lvalue_tag
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_ARCHETYPES_ACCESS_TAG_HPP
#define SAKE_CORE_ITERATOR_ARCHETYPES_ACCESS_TAG_HPP

namespace sake
{

namespace iterator_archetypes
{

struct readable_tag { };
struct writable_tag { };
struct swappable_tag { };

struct readable_writable_tag
    : iterator_archetypes::readable_tag,
      iterator_archetypes::writable_tag,
      iterator_archetypes::swappable_tag
{ };

struct lvalue_tag
    : iterator_archetypes::readable_tag
{ };

typedef iterator_archetypes::lvalue_tag readable_lvalue_tag;

struct writable_lvalue_tag
    : iterator_archetypes::lvalue_tag,
      iterator_archetypes::swappable_tag
{ };

} // namespace iterator_archetypes

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_ARCHETYPES_ACCESS_TAG_HPP

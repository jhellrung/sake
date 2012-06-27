/*******************************************************************************
 * sake/core/range/archetypes/access.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct range::archetypes::readable< T, TV = ... >
 * struct range::archetypes::writable< T, TV = ... >
 * struct range::archetypes::readable_writable< T, TV = ... >
 * struct range::archetypes::readable_lvalue< T, TV = ... >
 * struct range::archetypes::writable_lvalue< T, TV = ... >
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_ARCHETYPES_ACCESS_HPP
#define SAKE_CORE_ITERATOR_ARCHETYPES_ACCESS_HPP

#include <sake/core/iterator/archetypes/access_tag.hpp>
#include <sake/core/iterator/categories.hpp>
#include <sake/core/range/archetypes/range.hpp>

namespace sake
{

namespace range
{

namespace archetypes
{

#define define_archetype( access ) \
template< \
    class T, \
    class Traversal = boost::single_pass_traversal_tag \
> \
struct access \
{ \
    typedef sake::archetypes::range< \
        T, Traversal, sake::iterator::archetypes::access ## _tag > type; \
};
define_archetype( readable )
define_archetype( writable )
define_archetype( readable_writable )
define_archetype( readable_lvalue )
define_archetype( writable_lvalue )
#undef define_archetype

} // namespace archetypes

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_ARCHETYPES_ACCESS_HPP

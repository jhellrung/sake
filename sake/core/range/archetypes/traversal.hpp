/*******************************************************************************
 * sake/core/range/archetypes/traversal.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct range::archetypes::incrementable< T, A = ... >
 * struct range::archetypes::single_pass< T, A = ... >
 * struct range::archetypes::forward< T, A = ... >
 * struct range::archetypes::bidirectional< T, A = ... >
 * struct range::archetypes::random_access< T, A = ... >
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_ARCHETYPES_TRAVERSAL_HPP
#define SAKE_CORE_RANGE_ARCHETYPES_TRAVERSAL_HPP

#include <sake/core/iterator/archetypes/access_tag.hpp>
#include <sake/core/iterator/categories.hpp>
#include <sake/core/range/archetypes/range.hpp>

namespace sake
{

namespace range
{

namespace archetypes
{

#define define_archetype( traversal ) \
template< \
    class T, \
    class Access = sake::iterator::archetypes::readable_tag \
> \
struct traversal \
{ \
    typedef sake::archetypes::range< \
        T, boost::traversal ## _traversal_tag, Access > type; \
};
define_archetype( single_pass )
define_archetype( forward )
define_archetype( bidirectional )
define_archetype( random_access )
#undef define_archetype

} // namespace archetypes

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_ARCHETYPES_TRAVERSAL_HPP

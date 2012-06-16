/*******************************************************************************
 * sake/core/iterator/archetypes/private/introversal_base.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_ARCHETYPES_PRIVATE_INTROVERSAL_BASE_HPP
#define SAKE_CORE_ITERATOR_ARCHETYPES_PRIVATE_INTROVERSAL_BASE_HPP

#include <boost/type_traits/is_same.hpp>

#include <sake/core/iterator/archetypes/private/begin_introversal_base.hpp>
#include <sake/core/iterator/archetypes/private/common_introversal_base.hpp>
#include <sake/core/iterator/archetypes/private/end_introversal_base.hpp>
#include <sake/core/iterator/archetypes/private/relax.hpp>
#include <sake/core/iterator/categories.hpp>

namespace sake
{

namespace archetypes
{

namespace iterator_private
{

template< class Derived, class Introversal, class Traversal >
struct introversal_base
    : sake::archetypes::iterator_private::begin_introversal_base<
          Derived,
          typename sake::introversal_meet<
              Introversal, sake::begin_access_introversal_tag >::type,
          boost::is_same<
              Traversal, boost::random_access_traversal_tag >::value
      >,
      sake::archetypes::iterator_private::end_introversal_base<
          Derived,
          typename sake::introversal_meet<
              Introversal, sake::end_access_introversal_tag >::type,
          boost::is_same<
              Traversal, boost::random_access_traversal_tag >::value
      >,
      sake::archetypes::iterator_private::common_introversal_base<
          Derived, Introversal >
{
    template< class Introversal_ >
    struct relax
        : sake::archetypes::iterator_private::relax< Derived, Introversal_ >
    { };
};

} // namespace iterator_private

} // namespace archetypes

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_ARCHETYPES_PRIVATE_INTROVERSAL_BASE_HPP

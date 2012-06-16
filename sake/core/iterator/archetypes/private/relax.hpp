/*******************************************************************************
 * sake/core/iterator/archetypes/private/relax.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_ARCHETYPES_PRIVATE_RELAX_HPP
#define SAKE_CORE_ITERATOR_ARCHETYPES_PRIVATE_RELAX_HPP

#include <boost/static_assert.hpp>

#include <sake/boost_ext/type_traits/is_convertible.hpp>

namespace sake
{

namespace archetypes
{

template< class T, class Traversal, class Introversal, class Access >
struct iterator;

namespace iterator_private
{

template< class Derived, class Introversal_ >
struct relax;

template<
    class T, class Traversal, class Introversal, class Access,
    class Introversal_
>
struct relax<
    sake::archetypes::iterator< T, Traversal, Introversal, Access >,
    Introversal_
>
{
    BOOST_STATIC_ASSERT((
        boost_ext::is_convertible< Introversal, Introversal_ >::value));
    typedef sake::archetypes::iterator<
        T, Traversal, Introversal_, Access > type;
};

} // namespace iterator_private

} // namespace archetypes

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_ARCHETYPES_PRIVATE_RELAX_HPP

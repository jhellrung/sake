/*******************************************************************************
 * sake/core/iterator/concepts/SinglePass.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct iterator_concepts::SinglePass<I>
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_CONCEPTS_SINGLE_PASS_HPP
#define SAKE_CORE_ITERATOR_CONCEPTS_SINGLE_PASS_HPP

#include <boost/iterator/iterator_categories.hpp>
#include <boost/static_assert.hpp>

#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/concepts/EqualityComparable.hpp>
#include <sake/core/iterator/concepts/Incrementable.hpp>
#include <sake/core/iterator/traits.hpp>

namespace sake
{

namespace iterator_concepts
{

template< class I >
struct SinglePass
    : sake::concepts::EqualityComparable<I>,
      sake::iterator_concepts::Incrementable<I>
{
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        typename sake::iterator_traversal<I>::type,
        boost::single_pass_traversal_tag
    >::value));
};

} // namespace iterator_concepts

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_CONCEPTS_SINGLE_PASS_HPP

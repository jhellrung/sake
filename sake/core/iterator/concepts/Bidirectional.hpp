/*******************************************************************************
 * sake/core/iterator/concepts/Bidirectional.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct iterator_concepts::Bidirectional<I>
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_CONCEPTS_BIDIRECTIONAL_HPP
#define SAKE_CORE_ITERATOR_CONCEPTS_BIDIRECTIONAL_HPP

#include <boost/iterator/iterator_categories.hpp>
#include <boost/static_assert.hpp>

#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/concepts/Decrementable.hpp>
#include <sake/core/iterator/concepts/Forward.hpp>
#include <sake/core/iterator/traits.hpp>

namespace sake
{

namespace iterator_concepts
{

template< class I >
struct Bidirectional
    : sake::concepts::Decrementable<I>,
      sake::iterator_concepts::Forward<I>
{
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        typename sake::iterator_traversal<I>::type,
        boost::bidirectional_traversal_tag
    >::value));
};

} // namespace iterator_concepts

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_CONCEPTS_BIDIRECTIONAL_HPP

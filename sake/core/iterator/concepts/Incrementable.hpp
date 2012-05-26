/*******************************************************************************
 * sake/core/iterator/concepts/Incrementable.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct iterator_concepts::Incrementable<I>
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_CONCEPTS_INCREMENTABLE_HPP
#define SAKE_CORE_ITERATOR_CONCEPTS_INCREMENTABLE_HPP

#include <boost/iterator/iterator_categories.hpp>
#include <boost/static_assert.hpp>

#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/concepts/CopyAssignable.hpp>
#include <sake/core/concepts/CopyConstructible.hpp>
#include <sake/core/concepts/Incrementable.hpp>
#include <sake/core/iterator/traits.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake
{

namespace iterator_concepts
{

template< class I >
struct Incrementable
    : sake::concepts::CopyConstructible<I>,
      sake::concepts::CopyAssignable<I>,
      sake::concepts::Incrementable<I>
{
private:
    typedef sake::iterator_traits<I> traits_;
public:
    SAKE_USING_TYPEDEF( typename traits_, value_type );
    SAKE_USING_TYPEDEF( typename traits_, reference );
    SAKE_USING_TYPEDEF( typename traits_, pointer );
    SAKE_USING_TYPEDEF( typename traits_, difference_type );
    SAKE_USING_TYPEDEF( typename traits_, traversal );

    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        traversal,
        boost::incrementable_traversal_tag
    >::value));
};

} // namespace iterator_concepts

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_CONCEPTS_INCREMENTABLE_HPP

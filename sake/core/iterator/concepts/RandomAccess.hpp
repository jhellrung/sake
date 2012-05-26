/*******************************************************************************
 * sake/core/iterator/concepts/RandomAccess.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct iterator_concepts::RandomAccess<I>
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_CONCEPTS_RANDOM_ACCESS_HPP
#define SAKE_CORE_ITERATOR_CONCEPTS_RANDOM_ACCESS_HPP

#include <boost/iterator/iterator_categories.hpp>
#include <boost/static_assert.hpp>

#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/concepts/RelationalComparable.hpp>
#include <sake/core/iterator/concepts/Forward.hpp>
#include <sake/core/iterator/traits.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake
{

namespace iterator_concepts
{

template< class I >
struct RandomAccess
    : sake::concepts::RelationalComparable<I>,
      sake::iterator_concepts::Bidirectional<I>
{
private:
    typedef sake::iterator_traits<I> traits_;
public:
    SAKE_USING_TYPEDEF( typename traits_, reference );
    SAKE_USING_TYPEDEF( typename traits_, difference_type );
    SAKE_USING_TYPEDEF( typename traits_, traversal );

    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        traversal,
        boost::random_access_traversal_tag
    >::value));

    BOOST_CONCEPT_USAGE( RandomAccess )
    {
        assert_result< I& >(i += n);
        assert_result< I& >(i -= n);
        assert_result<I>(ci + n);
        assert_result<I>(n + ci);
        assert_result<I>(ci - n);
        assert_result< difference_type >(ci - cj);
        assert_result< reference >(ci[n]);
    }

private:
    I i;
    I j;
    I const ci;
    I const cj;
    difference_type const n;

    template< class T >
    void assert_result(T);
};

} // namespace iterator_concepts

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_CONCEPTS_RANDOM_ACCESS_HPP

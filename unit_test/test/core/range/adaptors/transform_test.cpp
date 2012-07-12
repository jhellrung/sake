/*******************************************************************************
 * unit_test/test/core/range/adaptors/transform_test.cpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <boost/concept/assert.hpp>
#include <boost/static_assert.hpp>

#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/iterator/categories.hpp>
#include <sake/core/range/adapt/transform.hpp>
#include <sake/core/range/begin.hpp>
#include <sake/core/range/concepts/Range.hpp>
#include <sake/core/range/concepts/Readable.hpp>
#include <sake/core/range/end.hpp>
#include <sake/core/range/is_adapt_by_value.hpp>
#include <sake/core/range/static_size.hpp>
#include <sake/core/range/traits.hpp>

#include <sake/test/environment.hpp>
#include <sake/test/test.hpp>

namespace sake_unit_test
{

namespace range
{

namespace adaptors
{

namespace
{

template< int N >
struct add
{
    typedef int result_type;
    int operator()(int const x) const
    { return x + N; }
};

template< class R >
void helper(sake::test::environment& env, R r)
{
    BOOST_STATIC_ASSERT((sake::boost_ext::is_convertible<
        typename sake::range_traversal<R>::type,
        boost::random_access_traversal_tag
    >::value));
    BOOST_CONCEPT_ASSERT((sake::concepts::Range<R>));
    BOOST_CONCEPT_ASSERT((sake::range::concepts::Readable<R>));
    BOOST_STATIC_ASSERT((sake::range_is_adapt_by_value<R>::value));
    BOOST_STATIC_ASSERT((sake::range_has_static_size<R>::value));
    BOOST_STATIC_ASSERT((sake::range_static_size<R>::value == 3));
    typedef typename sake::range_iterator<R>::type iter_type;
    iter_type i = sake::range::begin(r);
    SAKE_TEST_CHECK_RELATION( env, *i, ==, 42 );
    ++i;
    SAKE_TEST_CHECK_RELATION( env, *i, ==, 43 );
    ++i;
    SAKE_TEST_CHECK_RELATION( env, *i, ==, 44 );
    ++i;
    SAKE_TEST_CHECK_RELATION( env, i, ==, sake::range::end(r) );
}

} // namespace

void transform_test(sake::test::environment& env)
{
    int a[] = { 0, 1, 2 };
    adaptors::helper(
        env,
        a | sake::range::adapt::transform(adaptors::add< 42 >())
    );
}

} // namespace adaptors

} // namespace range

} // namespace sake_unit_test

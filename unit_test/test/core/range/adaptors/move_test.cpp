/*******************************************************************************
 * unit_test/test/core/range/adaptors/move_test.cpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <boost/concept/assert.hpp>
#include <boost/static_assert.hpp>

#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/iterator/categories.hpp>
#include <sake/core/range/adapt/move.hpp>
#include <sake/core/range/begin.hpp>
#include <sake/core/range/concepts/Range.hpp>
#include <sake/core/range/concepts/Readable.hpp>
#include <sake/core/range/is_adapt_by_value.hpp>
#include <sake/core/range/static_size.hpp>
#include <sake/core/range/traits.hpp>

#include <sake/test/environment.hpp>
#include <sake/test/test.hpp>

#include "../../common/models/movable_uint.hpp"

namespace sake_unit_test
{

namespace range
{

namespace adaptors
{

namespace
{

template< class R >
void helper(R r)
{
    BOOST_CONCEPT_ASSERT((sake::concepts::Range<R>));
    BOOST_CONCEPT_ASSERT((sake::range::concepts::Readable<R>));
    BOOST_STATIC_ASSERT((sake::range_is_adapt_by_value<R>::value));
    BOOST_STATIC_ASSERT((sake::range_has_static_size<R>::value));
    BOOST_STATIC_ASSERT((sake::range_static_size<R>::value == 3));
    typedef typename sake::range_iterator<
        R, sake::end_detect_introversal_tag >::type iter_type;
    iter_type i = sake::range::begin(r, sake::end_detect_introversal_tag());
    for(; i != sake::_end; ++i)
        models::movable_uint dummy = *i;
}

} // namespace

void move_test(sake::test::environment& env)
{
    typedef models::movable_uint value_type;
    value_type a[] = { value_type(1), value_type(2), value_type(3) };
    SAKE_TEST_CHECK_RELATION( env, a[0].value, ==, 1 );
    SAKE_TEST_CHECK_RELATION( env, a[1].value, ==, 2 );
    SAKE_TEST_CHECK_RELATION( env, a[2].value, ==, 3 );
    adaptors::helper(a | sake::range::adapt::move);
    SAKE_TEST_CHECK_RELATION( env, a[0].value, ==, 0 );
    SAKE_TEST_CHECK_RELATION( env, a[1].value, ==, 0 );
    SAKE_TEST_CHECK_RELATION( env, a[2].value, ==, 0 );
}

} // namespace adaptors

} // namespace range

} // namespace sake_unit_test

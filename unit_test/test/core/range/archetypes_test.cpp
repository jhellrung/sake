/*******************************************************************************
 * unit_test/test/core/range/archetypes_test.cpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <boost/concept/assert.hpp>

#include <sake/core/range/archetypes/access.hpp>
#include <sake/core/range/archetypes/traversal.hpp>
#include <sake/core/range/concepts/Bidirectional.hpp>
#include <sake/core/range/concepts/Forward.hpp>
#include <sake/core/range/concepts/Lvalue.hpp>
#include <sake/core/range/concepts/RandomAccess.hpp>
#include <sake/core/range/concepts/Readable.hpp>
#include <sake/core/range/concepts/ReadableLvalue.hpp>
#include <sake/core/range/concepts/ReadableWritable.hpp>
#include <sake/core/range/concepts/SinglePass.hpp>
#include <sake/core/range/concepts/WritableLvalue.hpp>

#include <sake/test/environment.hpp>

namespace sake_unit_test
{

namespace range
{

namespace
{

struct X { };

BOOST_CONCEPT_ASSERT((sake::range::concepts::SinglePass<
    sake::range::archetypes::single_pass<X>::type >));
BOOST_CONCEPT_ASSERT((sake::range::concepts::Forward<
    sake::range::archetypes::forward<X>::type >));
BOOST_CONCEPT_ASSERT((sake::range::concepts::Bidirectional<
    sake::range::archetypes::bidirectional<X>::type >));
BOOST_CONCEPT_ASSERT((sake::range::concepts::RandomAccess<
    sake::range::archetypes::random_access<X>::type >));

BOOST_CONCEPT_ASSERT((sake::range::concepts::Readable<
    sake::range::archetypes::readable<X>::type >));
BOOST_CONCEPT_ASSERT((sake::range::concepts::ReadableWritable<
    sake::range::archetypes::readable_writable<X>::type >));
BOOST_CONCEPT_ASSERT((sake::range::concepts::ReadableLvalue<
    sake::range::archetypes::readable_lvalue<X>::type >));
BOOST_CONCEPT_ASSERT((sake::range::concepts::WritableLvalue<
    sake::range::archetypes::writable_lvalue<X>::type >));

} // namespace

void archetypes_test(sake::test::environment& /*env*/)
{ }

} // namespace range

} // namespace sake_unit_test

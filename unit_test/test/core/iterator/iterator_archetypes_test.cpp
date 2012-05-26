/*******************************************************************************
 * unit_test/test/core/iterator/iterator_archetypes_test.cpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <boost/concept/assert.hpp>

#include <sake/core/iterator/archetypes/access.hpp>
#include <sake/core/iterator/archetypes/traversal.hpp>
#include <sake/core/iterator/concepts/Bidirectional.hpp>
#include <sake/core/iterator/concepts/Forward.hpp>
#include <sake/core/iterator/concepts/Incrementable.hpp>
#include <sake/core/iterator/concepts/Lvalue.hpp>
#include <sake/core/iterator/concepts/RandomAccess.hpp>
#include <sake/core/iterator/concepts/Readable.hpp>
#include <sake/core/iterator/concepts/ReadableLvalue.hpp>
#include <sake/core/iterator/concepts/ReadableWritable.hpp>
#include <sake/core/iterator/concepts/SinglePass.hpp>
#include <sake/core/iterator/concepts/Swappable.hpp>
#include <sake/core/iterator/concepts/Writable.hpp>
#include <sake/core/iterator/concepts/WritableLvalue.hpp>

#include <sake/test/environment.hpp>

namespace sake_unit_test
{

namespace
{

struct X { };

BOOST_CONCEPT_ASSERT((sake::iterator_concepts::Incrementable<
    sake::iterator_archetypes::incrementable<X>::type >));
BOOST_CONCEPT_ASSERT((sake::iterator_concepts::SinglePass<
    sake::iterator_archetypes::single_pass<X>::type >));
BOOST_CONCEPT_ASSERT((sake::iterator_concepts::Forward<
    sake::iterator_archetypes::forward<X>::type >));
BOOST_CONCEPT_ASSERT((sake::iterator_concepts::Bidirectional<
    sake::iterator_archetypes::bidirectional<X>::type >));
BOOST_CONCEPT_ASSERT((sake::iterator_concepts::RandomAccess<
    sake::iterator_archetypes::random_access<X>::type >));

BOOST_CONCEPT_ASSERT((sake::iterator_concepts::Readable<
    sake::iterator_archetypes::readable<X>::type >));
BOOST_CONCEPT_ASSERT((sake::iterator_concepts::Writable<
    sake::iterator_archetypes::writable<X>::type >));
BOOST_CONCEPT_ASSERT((sake::iterator_concepts::Swappable<
    sake::iterator_archetypes::swappable<X>::type >));
BOOST_CONCEPT_ASSERT((sake::iterator_concepts::ReadableWritable<
    sake::iterator_archetypes::readable_writable<X>::type >));
BOOST_CONCEPT_ASSERT((sake::iterator_concepts::ReadableLvalue<
    sake::iterator_archetypes::readable_lvalue<X>::type >));
BOOST_CONCEPT_ASSERT((sake::iterator_concepts::WritableLvalue<
    sake::iterator_archetypes::writable_lvalue<X>::type >));

} // namespace

void iterator_archetypes_test(sake::test::environment& /*env*/)
{ }

} // namespace sake_unit_test

/*******************************************************************************
 * unit_test/test/core/iterator/archetypes_test.cpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <boost/concept/assert.hpp>

#include <sake/core/iterator/archetypes/access.hpp>
#include <sake/core/iterator/archetypes/introversal.hpp>
#include <sake/core/iterator/archetypes/traversal.hpp>
#include <sake/core/iterator/concepts/BeginAccess.hpp>
#include <sake/core/iterator/concepts/BeginDetect.hpp>
#include <sake/core/iterator/concepts/Bidirectional.hpp>
#include <sake/core/iterator/concepts/EndAccess.hpp>
#include <sake/core/iterator/concepts/EndDetect.hpp>
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

namespace iterator
{

namespace
{

struct X { };

BOOST_CONCEPT_ASSERT((sake::iterator::concepts::Incrementable<
    sake::iterator::archetypes::incrementable<X>::type >));
BOOST_CONCEPT_ASSERT((sake::iterator::concepts::SinglePass<
    sake::iterator::archetypes::single_pass<X>::type >));
BOOST_CONCEPT_ASSERT((sake::iterator::concepts::Forward<
    sake::iterator::archetypes::forward<X>::type >));
BOOST_CONCEPT_ASSERT((sake::iterator::concepts::Bidirectional<
    sake::iterator::archetypes::bidirectional<X>::type >));
BOOST_CONCEPT_ASSERT((sake::iterator::concepts::RandomAccess<
    sake::iterator::archetypes::random_access<X>::type >));

BOOST_CONCEPT_ASSERT((sake::iterator::concepts::BeginDetect<
    sake::iterator::archetypes::begin_detect<X>::type >));
BOOST_CONCEPT_ASSERT((sake::iterator::concepts::BeginAccess<
    sake::iterator::archetypes::begin_access<X>::type >));
BOOST_CONCEPT_ASSERT((sake::iterator::concepts::EndDetect<
    sake::iterator::archetypes::end_detect<X>::type >));
BOOST_CONCEPT_ASSERT((sake::iterator::concepts::EndAccess<
    sake::iterator::archetypes::end_access<X>::type >));
BOOST_CONCEPT_ASSERT((sake::iterator::concepts::BeginDetect<
    sake::iterator::archetypes::begin_detect_end_detect<X>::type >));
BOOST_CONCEPT_ASSERT((sake::iterator::concepts::EndDetect<
    sake::iterator::archetypes::begin_detect_end_detect<X>::type >));
BOOST_CONCEPT_ASSERT((sake::iterator::concepts::BeginAccess<
    sake::iterator::archetypes::begin_access_end_detect<X>::type >));
BOOST_CONCEPT_ASSERT((sake::iterator::concepts::EndDetect<
    sake::iterator::archetypes::begin_access_end_detect<X>::type >));
BOOST_CONCEPT_ASSERT((sake::iterator::concepts::BeginDetect<
    sake::iterator::archetypes::begin_detect_end_access<X>::type >));
BOOST_CONCEPT_ASSERT((sake::iterator::concepts::EndAccess<
    sake::iterator::archetypes::begin_detect_end_access<X>::type >));
BOOST_CONCEPT_ASSERT((sake::iterator::concepts::BeginAccess<
    sake::iterator::archetypes::begin_access_end_access<X>::type >));
BOOST_CONCEPT_ASSERT((sake::iterator::concepts::EndAccess<
    sake::iterator::archetypes::begin_access_end_access<X>::type >));

BOOST_CONCEPT_ASSERT((sake::iterator::concepts::Readable<
    sake::iterator::archetypes::readable<X>::type >));
BOOST_CONCEPT_ASSERT((sake::iterator::concepts::Writable<
    sake::iterator::archetypes::writable<X>::type >));
BOOST_CONCEPT_ASSERT((sake::iterator::concepts::Swappable<
    sake::iterator::archetypes::swappable<X>::type >));
BOOST_CONCEPT_ASSERT((sake::iterator::concepts::ReadableWritable<
    sake::iterator::archetypes::readable_writable<X>::type >));
BOOST_CONCEPT_ASSERT((sake::iterator::concepts::ReadableLvalue<
    sake::iterator::archetypes::readable_lvalue<X>::type >));
BOOST_CONCEPT_ASSERT((sake::iterator::concepts::WritableLvalue<
    sake::iterator::archetypes::writable_lvalue<X>::type >));

} // namespace

void archetypes_test(sake::test::environment& /*env*/)
{ }

} // namespace iterator

} // namespace sake_unit_test

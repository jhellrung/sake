/*******************************************************************************
 * unit_test/test/core/range/archetypes/SinglePass.cpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <boost/concept/assert.hpp>

#include <sake/core/range/archetypes/traversal.hpp>
#include <sake/core/range/concepts/SinglePass.hpp>

namespace sake_unit_test
{

namespace range
{

namespace
{

struct X { };

BOOST_CONCEPT_ASSERT((sake::range::concepts::SinglePass<
    sake::range::archetypes::single_pass<X>::type >));

} // namespace

} // namespace range

} // namespace sake_unit_test

/*******************************************************************************
 * sake/core/range/concepts/WritableLvalue.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct range::concepts::WritableLvalue<R>
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_CONCEPTS_WRITABLE_LVALUE_HPP
#define SAKE_CORE_RANGE_CONCEPTS_WRITABLE_LVALUE_HPP

#include <boost/concept/assert.hpp>

#include <sake/core/iterator/concepts/WritableLvalue.hpp>
#include <sake/core/range/concepts/private/Base.hpp>
#include <sake/core/range/traits.hpp>

namespace sake
{

namespace range
{

namespace concepts
{

template< class R >
struct WritableLvalue
    : sake::range::concepts::private_::Base<R>
{
    BOOST_CONCEPT_ASSERT((sake::iterator::concepts::WritableLvalue<
        typename sake::range_iterator<R>::type >));
};

} // namespace concepts

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_CONCEPTS_WRITABLE_LVALUE_HPP

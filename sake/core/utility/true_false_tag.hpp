/*******************************************************************************
 * sake/core/utility/true_false_tag.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_TRUE_FALSE_TAG_HPP
#define SAKE_CORE_UTILITY_TRUE_FALSE_TAG_HPP

#include <boost/static_assert.hpp>

#include <sake/core/utility/sizeof_t.hpp>

#define SAKE_SIZEOF_TRUE_TAG  2 // == 1 + true
#define SAKE_SIZEOF_FALSE_TAG 1 // == 1 + false

namespace sake
{

typedef sake::sizeof_t< SAKE_SIZEOF_TRUE_TAG  >  true_tag;
typedef sake::sizeof_t< SAKE_SIZEOF_FALSE_TAG > false_tag;

BOOST_STATIC_ASSERT((sizeof(  sake::true_tag ) == SAKE_SIZEOF_TRUE_TAG ));
BOOST_STATIC_ASSERT((sizeof( sake::false_tag ) == SAKE_SIZEOF_FALSE_TAG));

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_TRUE_FALSE_TAG_HPP

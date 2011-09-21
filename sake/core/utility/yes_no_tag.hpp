/*******************************************************************************
 * sake/core/utility/yes_no_tag.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_YES_NO_TAG_HPP
#define SAKE_CORE_UTILITY_YES_NO_TAG_HPP

#include <boost/static_assert.hpp>

#include <sake/core/utility/sizeof_t.hpp>

#define SAKE_SIZEOF_YES_TAG 1
#define SAKE_SIZEOF_NO_TAG  2

namespace sake
{

typedef sake::sizeof_t< SAKE_SIZEOF_YES_TAG > yes_tag;
typedef sake::sizeof_t< SAKE_SIZEOF_NO_TAG  >  no_tag;

BOOST_STATIC_ASSERT((sizeof( yes_tag ) == SAKE_SIZEOF_YES_TAG));
BOOST_STATIC_ASSERT((sizeof(  no_tag ) == SAKE_SIZEOF_NO_TAG ));

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_YES_NO_TAG_HPP

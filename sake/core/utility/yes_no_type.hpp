/*******************************************************************************
 * core/utility/yes_no_type.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_YES_NO_TYPE_HPP
#define SAKE_CORE_UTILITY_YES_NO_TYPE_HPP

#include <boost/mpl/assert.hpp>

#include <sake/core/utility/sizeof_t.hpp>

#define SAKE_SIZEOF_YES_TYPE 1
#define SAKE_SIZEOF_NO_TYPE  2

namespace sake
{

typedef sizeof_t< SAKE_SIZEOF_YES_TYPE > yes_type;
typedef sizeof_t< SAKE_SIZEOF_NO_TYPE  >  no_type;

BOOST_MPL_ASSERT_RELATION( sizeof( yes_type ), ==, SAKE_SIZEOF_YES_TYPE );
BOOST_MPL_ASSERT_RELATION( sizeof(  no_type ), ==, SAKE_SIZEOF_NO_TYPE  );

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_YES_NO_TYPE_HPP

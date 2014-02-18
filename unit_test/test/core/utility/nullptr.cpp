/*******************************************************************************
 * unit_test/test/core/utility/nullptr.cpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <boost/static_assert.hpp>

#include <sake/core/utility/nullptr.hpp>

#include <sake/test/environment.hpp>
#include <sake/test/test.hpp>

namespace
{

template< class T > int test(T);

} // namespace

namespace sake_unit_test
{

void nullptr_(sake::test::environment & env)
{
  BOOST_STATIC_ASSERT( sizeof( sake::nullptr_t ) == sizeof( void * ) );
  BOOST_STATIC_ASSERT( sizeof( ::test< void * >(nullptr) ) );
  BOOST_STATIC_ASSERT( sizeof( ::test< int * >(nullptr) ) );
  BOOST_STATIC_ASSERT( sizeof( ::test< bool >(nullptr) ) );
}

} // namespace sake_unit_test

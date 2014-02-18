/*******************************************************************************
 * unit_test/test/boost_ext/type_traits/composite_pointer_type.cpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <boost/static_assert.hpp>
#include <boost/type_traits/integral_constant.hpp>

#include <sake/boost_ext/type_traits/composite_pointer_type.hpp>

#include <sake/core/config.hpp>
#include <sake/core/expr_traits/apply.hpp>
#include <sake/core/utility/declval.hpp>
#include <sake/core/utility/identity_type.hpp>

#include <sake/test/environment.hpp>

namespace sake_unit_test
{

namespace boost_ext
{

namespace
{

bool bool_();

template< class T >
struct is_same_as
{
  template< class U, class = void >
  struct apply
    : boost::false_type
  { };
  template< class _ >
  struct apply<T,_>
    : boost::true_type
  { };
};

struct A { };
struct B : A { };

} // namespace

void common_type(sake::test::environment& /*env*/)
{

#define assert_composite_pointer_type( T0, T1 ) \
  BOOST_STATIC_ASSERT((SAKE_EXPR_APPLY( \
    is_same_as< SAKE_IDENTITY_TYPE((( sake::boost_ext::composite_pointer_type< T0, T1 >::type ))) >, \
    bool_() ? sake::declval< T0 >(), sake::declval< T1 >() \
  )))

  assert_composite_pointer_type( void *, void * );

#undef assert_composite_pointer_type

}

} // namespace boost_ext

} // namespace sake_unit_test

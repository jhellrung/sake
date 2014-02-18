/*******************************************************************************
 * unit_test/test/boost_ext/type_traits/common_type.cpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <boost/static_assert.hpp>
#include <boost/type_traits/integral_constant.hpp>

#include <sake/boost_ext/type_traits/common_type.hpp>

#include <sake/core/config.hpp>
#include <sake/core/expr_traits/apply.hpp>
#include <sake/core/expr_traits/is_void.hpp>
#include <sake/core/utility/declval.hpp>

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
struct C { operator A () const; };
struct D { operator A & () const; };
struct E { operator A const & () const; };

struct F { operator int () const; };
struct G { operator int & () const; };
struct H { operator int const & () const; };

enum I { };

} // namespace

void common_type(sake::test::environment& /*env*/)
{

  BOOST_STATIC_ASSERT((SAKE_EXPR_IS_VOID(
    bool_() ? sake::declval< void >() : sake::declval< void >() )));

#define assert_common_type( expr0, expr1, T ) \
  BOOST_STATIC_ASSERT((SAKE_EXPR_APPLY( is_same_as<T>, bool_() ? expr0 : expr1 )))

  assert_common_type( sake::declval< void >(), sake::declval< void >(), void );
  assert_common_type( sake::declval<A>(), throw 0, A );
  assert_common_type( sake::declval< A & >(), throw 0, A );
#if defined( SAKE_MSC_VERSION ) && SAKE_MSC_VERSION <= 1500
  assert_common_type( sake::declval< A const & >(), throw 0, A const & );
#else // #if defined( SAKE_MSC_VERSION ) && SAKE_MSC_VERSION <= 1500
  assert_common_type( sake::declval< A const & >(), throw 0, A );
#endif // #if defined( SAKE_MSC_VERSION ) && SAKE_MSC_VERSION <= 1500

#define assert_common_type_declval( T0, T1, T ) \
  assert_common_type( sake::declval< T0 >(), sake::declval< T1 >(), T )

  assert_common_type_declval( A, A, A );
  assert_common_type_declval( A, A &, A );
#if defined( SAKE_MSC_VERSION ) && SAKE_MSC_VERSION <= 1500
  assert_common_type_declval( A, A const &, A const & );
#else // #if defined( SAKE_MSC_VERSION ) && SAKE_MSC_VERSION <= 1500
  assert_common_type_declval( A, A const &, A );
#endif // #if defined( SAKE_MSC_VERSION ) && SAKE_MSC_VERSION <= 1500
  assert_common_type_declval( A &, A &, A & );
  assert_common_type_declval( A &, A const &, A const & );
  assert_common_type_declval( A const &, A const &, A const & );

  assert_common_type_declval( A, B, A );
  assert_common_type_declval( A, B &, A );
  assert_common_type_declval( A, B const &, A );
  assert_common_type_declval( A &, B, A );
  assert_common_type_declval( A &, B &, A & );
  assert_common_type_declval( A &, B const &, A );
#if defined( SAKE_MSC_VERSION ) && SAKE_MSC_VERSION <= 1500
  assert_common_type_declval( A const &, B, A const & );
#else // #if defined( SAKE_MSC_VERSION ) && SAKE_MSC_VERSION <= 1500
  assert_common_type_declval( A const &, B, A );
#endif // #if defined( SAKE_MSC_VERSION ) && SAKE_MSC_VERSION <= 1500
  assert_common_type_declval( A const &, B &, A const & );
  assert_common_type_declval( A const &, B const &, A const & );

  assert_common_type_declval( A, C, A );
  assert_common_type_declval( A, C &, A );
  assert_common_type_declval( A, C const &, A );
  assert_common_type_declval( A &, C, A );
  assert_common_type_declval( A &, C &, A );
  assert_common_type_declval( A &, C const &, A );
  assert_common_type_declval( A const &, C, A );
#if defined( SAKE_MSC_VERSION ) && SAKE_MSC_VERSION <= 1500
  assert_common_type_declval( A const &, C &, A const & );
  assert_common_type_declval( A const &, C const &, A const & );
#else // #if defined( SAKE_MSC_VERSION ) && SAKE_MSC_VERSION <= 1500
  assert_common_type_declval( A const &, C &, A );
  assert_common_type_declval( A const &, C const &, A );
#endif // #if defined( SAKE_MSC_VERSION ) && SAKE_MSC_VERSION <= 1500

  assert_common_type_declval( A, D, A );
  assert_common_type_declval( A, D &, A );
  assert_common_type_declval( A, D const &, A );
  assert_common_type_declval( A &, D, A & );
  assert_common_type_declval( A &, D &, A & );
  assert_common_type_declval( A &, D const &, A & );
#if defined( SAKE_MSC_VERSION ) && SAKE_MSC_VERSION <= 1500
  assert_common_type_declval( A const &, D, A & );
#else // #if defined( SAKE_MSC_VERSION ) && SAKE_MSC_VERSION <= 1500
  assert_common_type_declval( A const &, D, A const & );
#endif // #if defined( SAKE_MSC_VERSION ) && SAKE_MSC_VERSION <= 1500
  assert_common_type_declval( A const &, D &, A const & );
  assert_common_type_declval( A const &, D const &, A const & );

#if defined( SAKE_MSC_VERSION ) && SAKE_MSC_VERSION <= 1500
  assert_common_type_declval( A, E, A const & );
  assert_common_type_declval( A, E &, A const & );
  assert_common_type_declval( A, E const &, A const & );
  assert_common_type_declval( A &, E, A const & );
  assert_common_type_declval( A &, E &, A const & );
  assert_common_type_declval( A &, E const &, A const & );
#else // #if defined( SAKE_MSC_VERSION ) && SAKE_MSC_VERSION <= 1500
  assert_common_type_declval( A, E, A );
  assert_common_type_declval( A, E &, A );
  assert_common_type_declval( A, E const &, A );
  assert_common_type_declval( A &, E, A );
  assert_common_type_declval( A &, E &, A );
  assert_common_type_declval( A &, E const &, A );
#endif // #if defined( SAKE_MSC_VERSION ) && SAKE_MSC_VERSION <= 1500
  assert_common_type_declval( A const &, E, A const & );
  assert_common_type_declval( A const &, E &, A const & );
  assert_common_type_declval( A const &, E const &, A const & );

  assert_common_type_declval( int, G, int );
  assert_common_type_declval( int, G &, int );
  assert_common_type_declval( int, G const &, int );
  assert_common_type_declval( int &, G, int & );
  assert_common_type_declval( int &, G &, int & );
  assert_common_type_declval( int &, G const &, int & );
  assert_common_type_declval( int const &, G, int const & );
  assert_common_type_declval( int const &, G &, int const & );
  assert_common_type_declval( int const &, G const &, int const & );

  assert_common_type_declval( int, H, int );
  assert_common_type_declval( int, H &, int );
  assert_common_type_declval( int, H const &, int );
#if defined( SAKE_MSC_VERSION ) && SAKE_MSC_VERSION <= 1500
  assert_common_type_declval( int &, H, int & );
  assert_common_type_declval( int &, H &, int & );
  assert_common_type_declval( int &, H const &, int & );
#else // #if defined( SAKE_MSC_VERSION ) && SAKE_MSC_VERSION <= 1500
  assert_common_type_declval( int &, H, int );
  assert_common_type_declval( int &, H &, int );
  assert_common_type_declval( int &, H const &, int );
#endif // #if defined( SAKE_MSC_VERSION ) && SAKE_MSC_VERSION <= 1500
  assert_common_type_declval( int const &, H, int const & );
  assert_common_type_declval( int const &, H &, int const & );
  assert_common_type_declval( int const &, H const &, int const & );

#undef assert_common_type_declval

#undef assert_common_type

}

} // namespace boost_ext

} // namespace sake_unit_test

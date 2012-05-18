/*******************************************************************************
 * sake/core/expr_traits/is_void.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_EXPR_IS_VOID( expression )
 *
 * This evaluates (at compile-time) to true iff the given expression has type
 * void.
 ******************************************************************************/

#ifndef SAKE_CORE_EXPR_TRAITS_IS_VOID_HPP
#define SAKE_CORE_EXPR_TRAITS_IS_VOID_HPP

#include <boost/static_assert.hpp>

#include <sake/core/utility/declval.hpp>
#include <sake/core/utility/identity_type.hpp>
#include <sake/core/utility/workaround.hpp>
#include <sake/core/utility/true_false_tag.hpp>

#if SAKE_WORKAROUND_GNUC_VERSION_LESS_EQUAL( ( 4, 6, 3 ) )
#define SAKE_EXPR_IS_VOID_USE_WEAK_IMPL
#endif // SAKE_WORKAROUND_GNUC_VERSION_LESS_EQUAL( ( 4, 6, 3 ) )

#define SAKE_EXPR_IS_VOID_is_void_detector( expression ) \
    ( SAKE_SIZEOF_TRUE_TAG \
   == sizeof( ::sake::expr_is_void_private::is_void_detector( expression ) ) )

#ifdef SAKE_EXPR_IS_VOID_USE_WEAK_IMPL

#define SAKE_EXPR_IS_VOID( expression ) \
    SAKE_EXPR_IS_VOID_is_void_detector(( \
        expression, \
        ::sake::expr_is_void_private::void_detector(), \
        ::sake::expr_is_void_private::void_detector() \
    ))

#else // #ifdef SAKE_EXPR_IS_VOID_USE_WEAK_IMPL

#define SAKE_EXPR_IS_VOID( expression ) \
    ( SAKE_EXPR_IS_VOID_is_void_detector(( \
        expression, \
        ::sake::expr_is_void_private::void_detector(), \
        ::sake::expr_is_void_private::void_detector() \
    )) \
 && SAKE_EXPR_IS_VOID_is_void_detector(( \
        ::sake::expr_is_void_private::void_detector(), \
        expression, \
        ::sake::expr_is_void_private::void_detector() \
    )))

#endif // #ifdef SAKE_EXPR_IS_VOID_USE_WEAK_IMPL

namespace sake
{

namespace expr_is_void_private
{

struct void_detector
{ void_detector operator,(void_detector) const; };
struct non_void
{ non_void operator,(void_detector) const; };

struct from_any
{
#ifdef SAKE_EXPR_IS_VOID_USE_WEAK_IMPL
    template< class T > from_any(T const &);
#else // #ifdef SAKE_EXPR_IS_VOID_USE_WEAK_IMPL
    from_any(...);
#endif // #ifdef SAKE_EXPR_IS_VOID_USE_WEAK_IMPL
};
struct from_void_detector
{ from_void_detector(void_detector); };

non_void operator,(from_any, from_void_detector);
non_void operator,(from_void_detector, from_any);

sake::true_tag is_void_detector(void_detector);
template< class T > sake::false_tag is_void_detector(T const &);

namespace
{

BOOST_STATIC_ASSERT( SAKE_EXPR_IS_VOID( sake::declval< void >() ) );

struct dummy { };
template< class L, class R > struct dummy_template { };
template< class L, class R, class T >
L operator,(dummy_template<L,R>, T);
template< class T, class L, class R >
R operator,(T, dummy_template<L,R>);

#define test( T ) \
    BOOST_STATIC_ASSERT( !SAKE_EXPR_IS_VOID( (sake::declval<T>()) ) );
test( int )
test( dummy )
test( SAKE_IDENTITY_TYPE_WRAP(( dummy_template< int, int > )) )
test( SAKE_IDENTITY_TYPE_WRAP(( dummy_template< int, void > )) )
#ifndef SAKE_EXPR_IS_VOID_USE_WEAK_IMPL
test( SAKE_IDENTITY_TYPE_WRAP(( dummy_template< void, int > )) )
#endif // #ifndef SAKE_EXPR_IS_VOID_USE_WEAK_IMPL
//test( SAKE_IDENTITY_TYPE_WRAP(( dummy_template< void, void > )) )
#undef test

} // namespace

} // namespace expr_is_void_private

} // namespace sake

#endif // #ifndef SAKE_CORE_EXPR_TRAITS_IS_VOID_HPP

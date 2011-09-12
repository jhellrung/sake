/*******************************************************************************
 * core/expr_traits/is_void.hpp
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

#include <sake/core/expr_traits/is_convertible.hpp>
#include <sake/core/utility/convertible_from_any.hpp>
#include <sake/core/utility/declval.hpp>
#include <sake/core/utility/identity_type.hpp>
#include <sake/core/utility/workaround.hpp>

#if SAKE_WORKAROUND_GNUC_VERSION_LESS_EQUAL( ( 4, 4, 1 ) )

#define SAKE_EXPR_IS_VOID( expression ) \
    SAKE_EXPR_IS_CONVERTIBLE( \
        ( \
            expression, \
            ::sake::expr_is_void_private::void_detector(), \
            ::sake::expr_is_void_private::void_detector() \
        ), \
        ::sake::expr_is_void_private::void_detector \
    )

#else // #if SAKE_WORKAROUND_GNUC_VERSION_LESS_EQUAL( ( 4, 4, 1 ) )

#define SAKE_EXPR_IS_VOID( expression ) \
    ( \
        SAKE_EXPR_IS_CONVERTIBLE( \
            ( \
                expression, \
                ::sake::expr_is_void_private::void_detector(), \
                ::sake::expr_is_void_private::void_detector() \
            ), \
            ::sake::expr_is_void_private::void_detector \
        ) \
     && SAKE_EXPR_IS_CONVERTIBLE( \
            ( \
                ::sake::expr_is_void_private::void_detector(), \
                expression, \
                ::sake::expr_is_void_private::void_detector() \
            ), \
            ::sake::expr_is_void_private::void_detector \
        ) \
    )

#endif // #if SAKE_WORKAROUND_GNUC_VERSION_LESS_EQUAL( ( 4, 4, 1 ) )

namespace sake
{

namespace expr_is_void_private
{

struct non_void;

struct void_detector
{
    void_detector operator,(void_detector) const;
    friend non_void operator,(sake::convertible_from_any, void_detector);
    non_void operator,(sake::convertible_from_any) const;
};

struct non_void
{
    non_void operator,(void_detector) const;
};

BOOST_STATIC_ASSERT( SAKE_EXPR_IS_VOID( sake::declval< void >() ) );

struct dummy { };
template< class L, class R > struct dummy_tmpl { };
template< class T, class L, class R >
L operator,(T, dummy_tmpl<L,R>);
template< class L, class R, class T >
R operator,(dummy_tmpl<L,R>, T);

#define test( T ) BOOST_STATIC_ASSERT( !SAKE_EXPR_IS_VOID( (sake::declval<T>()) ) );
test( int )
test( dummy )
test( SAKE_IDENTITY_TYPE(( dummy_tmpl< int, int > )) )
test( SAKE_IDENTITY_TYPE(( dummy_tmpl< int, void > )) )
test( SAKE_IDENTITY_TYPE(( dummy_tmpl< void, int > )) )
//test( dummy_tmpl< void, void > )
#undef test

} // namespace expr_is_void_private

} // namespace sake

#endif // #ifndef SAKE_CORE_EXPR_TRAITS_IS_VOID_HPP
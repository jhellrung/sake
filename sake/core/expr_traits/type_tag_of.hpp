/*******************************************************************************
 * sake/core/expr_traits/type_tag_of.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_EXPR_TYPE_TAG_OF( expression )
 *
 * This expands to an expression with type type_tag<T>, where T is the type of
 * the given expression.  Specifically, T is a reference type if the given
 * expression is an lvalue expression, and a non-reference (non-const) type if
 * the given expression is an rvalue expression.  The given expression is not
 * evaluated.
 *
 * Note: expression must have non-void type.
 ******************************************************************************/

#ifndef SAKE_CORE_EXPR_TRAITS_TYPE_TAG_OF_HPP
#define SAKE_CORE_EXPR_TRAITS_TYPE_TAG_OF_HPP

#include <boost/static_assert.hpp>

#include <sake/core/expr_traits/private/type_tag_inducer.hpp>
#include <sake/core/utility/declval.hpp>
#include <sake/core/utility/type_tag.hpp>

#define SAKE_EXPR_TYPE_TAG_OF( expression ) \
    (false ? \
     ::sake::expr_type_tag_of_private::deduce( expression, expression ) : \
     ::sake::expr_traits_private::type_tag_inducer())

namespace sake
{

namespace expr_type_tag_of_private
{

struct rvalue_detector
{ template< class T > rvalue_detector(T const &) { } };

template< class T, class U >
inline sake::type_tag< T& >
deduce(T&, U&)
{ return sake::type_tag< T& >(); }

template< class T, class U >
inline sake::type_tag< T const & >
deduce(T const &, U&)
{ return sake::type_tag< T const & >(); }

template< class T >
inline sake::type_tag<T>
deduce(T const &, rvalue_detector)
{ return sake::type_tag<T>(); }

namespace
{

template< class T >
int assert_same(sake::type_tag<T>, sake::type_tag<T>);
struct dummy { };

#define test( T ) \
    BOOST_STATIC_ASSERT( sizeof( \
        assert_same( \
            SAKE_EXPR_TYPE_TAG_OF( sake::declval<T>() ), \
            sake::type_tag<T>() \
        ) \
    ) );
test( int )
test( int & )
test( int const & )
test( int volatile & )
test( int const volatile & )
test( dummy )
test( dummy & )
test( dummy const & )
test( dummy volatile & )
test( dummy const volatile & )
#undef test

} // namespace

} // namespace expr_type_tag_of_private

} // namespace sake

#endif // #ifndef SAKE_CORE_EXPR_TRAITS_TYPE_TAG_OF_HPP

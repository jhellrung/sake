/*******************************************************************************
 * core/expr_traits/type_tag_of.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * SAKE_EXPR_TYPE_TAG_OF( Expr )
 *
 * This expands to an expression with type type_tag<T>, where T is the type of
 * the expression Expr.  Specifically, T is a reference type if Expr is an
 * lvalue expression, and a non-reference (non-const) type if Expr is an rvalue
 * expression.  Expr is not evaluated.
 *
 * Note: Expr must have non-void type.
 ******************************************************************************/

#ifndef SAKE_CORE_EXPR_TRAITS_TYPE_TAG_OF_HPP
#define SAKE_CORE_EXPR_TRAITS_TYPE_TAG_OF_HPP

#include <boost/mpl/assert.hpp>

#include <sake/core/utility/declval.hpp>
#include <sake/core/utility/type_tag.hpp>

#define SAKE_EXPR_TYPE_TAG_OF( Expr ) \
    (false ? \
     ::sake::expr_type_tag_of_private::deduce( ( Expr ), ( Expr ) ) : \
     ::sake::expr_type_tag_of_private::inducer())

namespace sake
{

namespace expr_type_tag_of_private
{

struct inducer
{
    template< class T > operator type_tag<T>() const
    { return type_tag<T>(); }
};

template< class T, class U >
inline type_tag< T& >
deduce(T&, U&)
{ return type_tag< T& >(); }

template< class T, class U >
inline type_tag< T const & >
deduce(T const &, U&)
{ return type_tag< T const & >(); }

template< class T >
inline type_tag<T>
deduce(T const &, ...)
{ return type_tag<T>(); }

template< class T > int assert_same(type_tag<T>, type_tag<T>);
struct dummy { };

} // namespace expr_type_tag_of_private

#define test( T ) \
    BOOST_MPL_ASSERT_RELATION( \
        sizeof( expr_type_tag_of_private::assert_same( \
            SAKE_EXPR_TYPE_TAG_OF( sake::declval<T>() ), \
            type_tag<T>() \
        ) ) , \
        ==, \
        sizeof( int ) \
    );
test( int )
test( int & )
test( int const & )
test( int volatile & )
test( int const volatile & )
test( expr_type_tag_of_private::dummy )
test( expr_type_tag_of_private::dummy & )
test( expr_type_tag_of_private::dummy const & )
test( expr_type_tag_of_private::dummy volatile & )
test( expr_type_tag_of_private::dummy const volatile & )
#undef test

} // namespace sake

#endif // #ifndef SAKE_CORE_EXPR_TRAITS_TYPE_TAG_OF_HPP

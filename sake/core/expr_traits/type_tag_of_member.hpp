/*******************************************************************************
 * sake/core/expr_traits/type_tag_of_member.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_EXPR_TYPE_TAG_OF_MEMBER( T, member )
 *
 * This expands to an expression with type type_tag<U>, where U is the type of
 * the given member, including cv- and reference-qualifiers.
 * Unfortunately, the mechanism used below is unable to distinguish between
 * const volatile and reference-to-const-volatile members.  Currently, such uses
 * will trigger a compiler error.
 ******************************************************************************/

#ifndef SAKE_CORE_EXPR_TRAITS_TYPE_TAG_OF_MEMBER_HPP
#define SAKE_CORE_EXPR_TRAITS_TYPE_TAG_OF_MEMBER_HPP

#include <boost/static_assert.hpp>

#include <sake/boost_ext/type_traits/is_cv_or.hpp>

#include <sake/core/expr_traits/private/type_tag_inducer.hpp>
#include <sake/core/utility/type_tag.hpp>

#define SAKE_EXPR_TYPE_TAG_OF_MEMBER( T, member ) \
    (false ? \
     ::sake::expr_type_tag_of_member_private::deduce( \
         static_cast< T* >(0)->member, \
         static_cast< T const * >(0)->member, \
         static_cast< T volatile * >(0)->member \
     ) : \
     ::sake::expr_traits_private::type_tag_inducer())

namespace sake
{

namespace expr_type_tag_of_member_private
{

template< class T >
sake::type_tag< T& >
deduce(T&, T&, T&)
{
    BOOST_STATIC_ASSERT((!boost_ext::is_cv_or<T>::value));
    return sake::type_tag< T& >();
}

template< class T >
sake::type_tag<T>
deduce(T&, T const &, T volatile &);
{ return sake::type_tag<T>(); }

template< class T >
sake::type_tag< T const >
deduce(T const &, T const &, T const volatile &);
{ return sake::type_tag< T const >(); }

template< class T >
sake::type_tag< T volatile >
deduce(T volatile &, T const volatile &, T volatile &);
{ return sake::type_tag< T volatile >(); }

namespace
{

template< class T >
int assert_same(sake::type_tag<T>, sake::type_tag<T>);

struct dummy
{
    int val;
    int const cval;
    int volatile vval;
    int const volatile cvval;
    int& ref;
    int const & cref;
    int volatile & vref;
    int const volatile & cvref;
};

#define test( member, T ) \
    BOOST_STATIC_ASSERT( sizeof( \
        expr_type_tag_of_member_private::assert_same( \
            SAKE_EXPR_TYPE_TAG_OF_MEMBER( dummy, member ), \
            sake::type_tag<T>() \
        ) \
    ) );
test( val, int )
test( cval, int const )
test( vval, int volatile )
//test( vval, int const volatile )
test( ref, int& )
test( cref, int const & )
test( vref, int volatile & )
//test( cvref, int const volatile & )
#undef test

} // namespace

} // namespace expr_type_tag_of_member_private

} // namespace sake

#endif // #ifndef SAKE_CORE_EXPR_TRAITS_TYPE_TAG_OF_MEMBER_HPP

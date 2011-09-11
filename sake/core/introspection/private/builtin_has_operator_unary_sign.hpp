/*******************************************************************************
 * core/introspection/private/builtin_has_operator_unary_sign.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_INTROSPECTION_PRIVATE_BUILTIN_HAS_OPERATOR_UNARY_SIGN_HPP
#define SAKE_CORE_INTROSPECTION_PRIVATE_BUILTIN_HAS_OPERATOR_UNARY_SIGN_HPP

#include <boost/mpl/and.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/or.hpp>
#include <boost/type_traits/integral_promotion.hpp>
#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/type_traits/is_enum.hpp>
#include <boost/type_traits/is_pointer.hpp>

#include <sake/boost_ext/type_traits/is_convertible.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <boost/mpl/placeholders.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <sake/core/expr_traits/apply.hpp>
#include <sake/core/utility/declval.hpp>
#include <sake/core/utility/identity_type.hpp>

namespace sake
{

namespace introspection_private
{

template< class T, class Result, class ResultMetafunction >
struct builtin_has_operator_unary_sign_impl
    : boost::mpl::and_<
          boost::mpl::or_<
              boost::is_arithmetic<T>,
              boost::is_enum<T>,
              boost::is_pointer<T>
          >,
          boost_ext::is_convertible< typename boost::integral_promotion<T>::type, Result >,
          boost::mpl::apply1< ResultMetafunction, typename boost::integral_promotion<T>::type >
      >
{ };

template< class T, class Result, class ResultMetafunction >
struct builtin_has_operator_unary_sign
    : builtin_has_operator_unary_sign_impl<
          typename boost_ext::remove_qualifiers<T>::type,
          Result,
          Metafunction
      >
{ };

#define test( op, T, Result ) \
    BOOST_STATIC_ASSERT( SAKE_EXPR_APPLY( \
        SAKE_IDENTITY_TYPE(( boost::is_same< boost::mpl::_1, Result > )), \
        op sake::declval<T>() \
    ) );
test( +, bool, int )
test( +, short, int )
test( +, int, int )
test( +, unsigned int, unsigned int )
test( +, void*, void* )
test( -, bool, int )
test( -, short, int )
test( -, int, int )
test( -, unsigned int, unsigned int )
#undef test

} // namespace introspection_private

} // namespace sake

#endif // #ifndef SAKE_CORE_INTROSPECTION_PRIVATE_BUILTIN_HAS_OPERATOR_UNARY_SIGN_HPP

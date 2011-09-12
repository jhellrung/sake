/*******************************************************************************
 * core/introspection/private/builtin_has_operator_complement.hpp
 *
 * Copyright 2010, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_INTROSPECTION_PRIVATE_BUILTIN_HAS_OPERATOR_COMPLEMENT_HPP
#define SAKE_CORE_INTROSPECTION_PRIVATE_BUILTIN_HAS_OPERATOR_COMPLEMENT_HPP

#include <boost/mpl/and.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/type_traits/integral_promotion.hpp>

#include <sake/boost_ext/type_traits/is_convertible.hpp>
#include <sake/boost_ext/type_traits/is_integral_or_enum.hpp>
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
struct builtin_has_operator_complement_impl
    : boost::mpl::and_<
          boost_ext::is_integral_or_enum<T>,
          boost_ext::is_convertible< typename boost::integral_promotion<T>::type, Result >,
          boost::mpl::apply1< ResultMetafunction, typename boost::integral_promotion<T>::type >
      >
{ };

template< class T, class Result, class ResultMetafunction >
struct builtin_has_operator_complement
    : builtin_has_operator_complement_impl<
          typename boost_ext::remove_qualifiers<T>::type,
          Result,
          ResultMetafunction
      >
{ };

#define test( T, Result ) \
    BOOST_STATIC_ASSERT( SAKE_EXPR_APPLY( \
        SAKE_IDENTITY_TYPE(( boost::is_same< boost::mpl::_1, Result > )), \
        ~sake::declval<T>() \
    ) );
#ifdef _MSC_VER
#pragma warning ( push )
#pragma warning ( disable : 4804 ) // '~' : unsafe use of type 'bool' in operation
#endif // #ifdef _MSC_VER
test( bool, int )
#ifdef _MSC_VER
#pragma warning ( pop )
#endif // #ifdef _MSC_VER
test( short, int )
test( int, int )
test( long, long )
#undef test

} // namespace introspection_private

} // namespace sake

#endif // #ifndef SAKE_CORE_INTROSPECTION_PRIVATE_BUILTIN_HAS_OPERATOR_COMPLEMENT_HPP

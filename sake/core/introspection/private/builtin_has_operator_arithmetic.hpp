/*******************************************************************************
 * core/introspection/private/builtin_has_operator_arithmetic.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_INTROSPECTION_PRIVATE_BUILTIN_HAS_OPERATOR_ARITHMETIC_HPP
#define SAKE_CORE_INTROSPECTION_PRIVATE_BUILTIN_HAS_OPERATOR_ARITHMETIC_HPP

#include <boost/mpl/and.hpp>
#include <boost/mpl/apply.hpp>

#include <sake/boost_ext/type_traits/common_type.hpp>
#include <sake/boost_ext/type_traits/is_arithmetic_or_enum.hpp>
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

template< class T, class U, class Result, class ResultMetafunction >
struct builtin_has_operator_arithmetic_impl
    : boost::mpl::and_<
          boost_ext::is_arithmetic_or_enum<T>,
          boost_ext::is_arithmetic_or_enum<U>,
          boost_ext::is_convertible< typename boost_ext::common_type<T,U>::type, Result >,
          boost::mpl::apply1< ResultMetafunction, typename boost_ext::common_type<T,U>::type >
      >
{ };

template< class T, class U, class Result, class ResultMetafunction >
struct builtin_has_operator_arithmetic
    : builtin_has_operator_arithmetic_impl<
          typename boost_ext::remove_qualifiers<T>::type,
          typename boost_ext::remove_qualifiers<U>::type,
          Result,
          ResultMetafunction
      >
{ };

#define test( T, op, U, Result ) \
    BOOST_STATIC_ASSERT( SAKE_EXPR_APPLY( \
        SAKE_IDENTITY_TYPE(( boost::is_same< boost::mpl::_1, Result > )), \
        sake::declval<T>() op sake::declval<U>() \
    ) );
test( int, *, int, int )
test( int, *, long, long )
test( long, *, int, long )
test( long, *, long, long )
test( float, *, float, float )
test( float, *, double, double )
test( double, *, float, double )
test( double, *, double, double )
#undef test

} // namespace introspection_private

} // namespace sake

#endif // #ifndef SAKE_CORE_INTROSPECTION_PRIVATE_BUILTIN_HAS_OPERATOR_ARITHMETIC_HPP

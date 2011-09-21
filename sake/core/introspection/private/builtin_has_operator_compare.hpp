/*******************************************************************************
 * sake/core/introspection/private/builtin_has_operator_compare.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_INTROSPECTION_PRIVATE_BUILTIN_HAS_OPERATOR_COMPARE_HPP
#define SAKE_CORE_INTROSPECTION_PRIVATE_BUILTIN_HAS_OPERATOR_COMPARE_HPP

#include <boost/mpl/and.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/or.hpp>

#include <sake/boost_ext/type_traits/is_arithmetic_or_enum.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>
#include <sake/boost_ext/type_traits/remove_cv_signature.hpp>
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
struct builtin_has_operator_compare_dispatch
    : boost::mpl::and_<
          boost_ext::is_arithmetic_or_enum<T>,
          boost_ext::is_arithmetic_or_enum<U>,
          boost_ext::is_convertible< bool, Result >,
          boost::mpl::apply1< ResultMetafunction, bool >
      >
{ };

template< class T, class U, class Result, class ResultMetafunction >
struct builtin_has_operator_compare_dispatch< T*, U*, Result, ResultMetafunction >
    : boost::mpl::and_<
          boost::mpl::or_<
              boost_ext::is_convertible<
                  typename boost_ext::remove_cv_signature< T* >::type,
                  typename boost_ext::remove_cv_signature< U* >::type
              >,
              boost_ext::is_convertible<
                  typename boost_ext::remove_cv_signature< U* >::type,
                  typename boost_ext::remove_cv_signature< T* >::type
              >
          >,
          boost_ext::is_convertible< bool, Result >,
          boost::mpl::apply1< ResultMetafunction, bool >
      >
{ };

template< class T, class U, class Result, class ResultMetafunction >
struct builtin_has_operator_compare
    : builtin_has_operator_compare_dispatch<
          typename boost_ext::remove_qualifiers<T>::type,
          typename boost_ext::remove_qualifiers<U>::type,
          Result,
          ResultMetafunction
      >
{ };

#define test( T, op, U ) \
    BOOST_STATIC_ASSERT( SAKE_EXPR_APPLY( \
        SAKE_IDENTITY_TYPE(( boost::is_same< boost::mpl::_1, bool > )), \
        sake::declval<T>() op sake::declval<U>() \
    ) );
test( int, ==, int )
test( void*, ==, int const * )
test( int**, ==, int const * const * )
test( int, !=, int )
test( void*, !=, int const * )
test( int**, !=, int const * const * )
test( int, <, int )
test( void*, <, int const * )
test( int**, <, int const * const * )
test( int, >, int )
test( void*, >, int const * )
test( int**, >, int const * const * )
test( int, <=, int )
test( void*, <=, int const * )
test( int**, <=, int const * const * )
test( int, >=, int )
test( void*, >=, int const * )
test( int**, >=, int const * const * )
#undef test

} // namespace introspection_private

} // namespace sake

#endif // #ifndef SAKE_CORE_INTROSPECTION_PRIVATE_BUILTIN_HAS_OPERATOR_COMPARE_HPP

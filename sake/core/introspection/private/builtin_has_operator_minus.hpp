/*******************************************************************************
 * core/introspection/private/builtin_has_operator_minus.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_INTROSPECTION_PRIVATE_BUILTIN_HAS_OPERATOR_MINUS_HPP
#define SAKE_CORE_INTROSPECTION_PRIVATE_BUILTIN_HAS_OPERATOR_MINUS_HPP

#include <cstddef>

#include <boost/mpl/and.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/is_object.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_cv.hpp>

#include <sake/boost_ext/type_traits/is_integral_or_enum.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/introspection/private/builtin_has_operator_arithmetic.hpp>

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
struct builtin_has_operator_minus_dispatch
    : builtin_has_operator_arithmetic_impl< T, U, Result, ResultMetafunction >
{ };

template< class T, class U, class Result, class ResultMetafunction >
struct builtin_has_operator_minus_dispatch< T*, U, Result, ResultMetafunction >
    : boost::mpl::and_<
          boost::is_object<T>,
          boost_ext::is_integral_or_enum<U>,
          boost_ext::is_convertible< T*, Result >,
          boost::mpl::apply1< ResultMetafunction, T* >
      >
{ };

template< class T, class U, class Result, class ResultMetafunction >
struct builtin_has_operator_minus_dispatch< T, U*, Result, ResultMetafunction >
    : boost::false_type
{ };

template< class T, class U, class Result, class ResultMetafunction >
struct builtin_has_operator_minus_dispatch< T*, U*, Result, ResultMetafunction >
    : boost::mpl::and_<
          boost::is_same<
              typename boost::remove_cv<T>::type,
              typename boost::remove_cv<U>::type
          >,
          boost::is_object<T>,
          boost_ext::is_convertible< std::ptrdiff_t, Result >,
          boost::mpl::apply1< ResultMetafunction, std::ptrdiff_t >
      >
{ };

template< class T, class U, class Result, class ResultMetafunction >
struct builtin_has_operator_minus
    : builtin_has_operator_minus_dispatch<
          typename boost_ext::remove_qualifiers<T>::type,
          typename boost_ext::remove_qualifiers<U>::type,
          Result,
          ResultMetafunction
      >
{ };

#define test( T, U ) \
    BOOST_STATIC_ASSERT( SAKE_EXPR_APPLY( \
        SAKE_IDENTITY_TYPE(( boost::is_same< boost::mpl::_1, T* > )), \
        sake::declval< T* >() - sake::declval<U>() \
    ) );
test( int*, bool )
test( int*, int )
test( int*, unsigned int )
test( int const *, bool )
test( int const *, int )
test( int const *, unsigned int )
#undef test
#define test( T, U ) \
    BOOST_STATIC_ASSERT( SAKE_EXPR_APPLY( \
        SAKE_IDENTITY_TYPE(( boost::is_same< boost::mpl::_1, std::ptrdiff_t > )), \
        sake::declval< T* >() - sake::declval< U* >() \
    ) );
test( int*, int* )
test( int*, int const * )
test( int const *, int* )
test( int const *, int const * )
#undef test

} // namespace introspection_private

} // namespace sake

#endif // #ifndef SAKE_CORE_INTROSPECTION_PRIVATE_BUILTIN_HAS_OPERATOR_MINUS_HPP

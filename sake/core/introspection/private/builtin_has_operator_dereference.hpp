/*******************************************************************************
 * core/introspection/private/builtin_has_operator_dereference.hpp
 *
 * Copyright 2010, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_INTROSPECTION_PRIVATE_BUILTIN_HAS_OPERATOR_DEREFERENCE_HPP
#define SAKE_CORE_INTROSPECTION_PRIVATE_BUILTIN_HAS_OPERATOR_DEREFERENCE_HPP

#include <boost/mpl/and.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/or.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/is_object.hpp>
#include <boost/type_traits/is_function.hpp>

#include <sake/boost_ext/type_traits/add_reference.hpp>
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
struct builtin_has_operator_dereference_impl
    : boost::false_type
{ };

template< class T, class Result, class ResultMetafunction >
struct builtin_has_operator_dereference_impl< T*, Result, ResultMetafunction >
    : boost::mpl::and_<
          boost::mpl::or_<
              boost::is_object<T>,
              boost::is_function<T>
          >,
          boost_ext::is_convertible< typename boost_ext::add_reference<T>::type, Result >,
          boost::mpl::apply1< ResultMetafunction, typename boost_ext::add_reference<T>::type >
      >
{ };

template< class T, class Result, class ResultMetafunction >
struct builtin_has_operator_dereference
    : builtin_has_operator_dereference_impl<
          typename boost_ext::remove_qualifiers<T>::type,
          Result,
          ResultMetafunction
      >
{ };

#define test( T, Result ) \
    BOOST_STATIC_ASSERT( SAKE_EXPR_APPLY( \
        SAKE_IDENTITY_TYPE(( boost::is_same< boost::mpl::_1, Result > )), \
        *sake::declval<T>() \
    ) );
test( int*, int& )
test( int const *, int const & )
test( void (*)( ), void (&)( ) )
#undef test

} // namespace introspection_private

} // namespace sake

#endif // #ifndef SAKE_CORE_INTROSPECTION_PRIVATE_BUILTIN_HAS_OPERATOR_DEREFERENCE_HPP

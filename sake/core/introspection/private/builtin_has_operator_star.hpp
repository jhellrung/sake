/*******************************************************************************
 * sake/core/introspection/private/builtin_has_operator_star.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_INTROSPECTION_PRIVATE_BUILTIN_HAS_OPERATOR_STAR_HPP
#define SAKE_CORE_INTROSPECTION_PRIVATE_BUILTIN_HAS_OPERATOR_STAR_HPP

#include <boost/mpl/apply.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/is_object.hpp>
#include <boost/type_traits/is_function.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/utility/workaround.hpp>

#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <sake/boost_ext/mpl/curry_quote.hpp>
#include <sake/core/expr_traits/apply.hpp>
#include <sake/core/utility/declval.hpp>

namespace sake
{

namespace introspection_private
{

template<
    class T, class Result, class ResultPred,
    bool = boost::is_function<T>::value
>
struct builtin_has_operator_star_dispatch_on_function;

template< class T, class Result, class ResultPred >
struct builtin_has_operator_star_dispatch_on_function< T, Result, ResultPred, false >
    : boost_ext::mpl::and3<
          boost::is_object<T>,
          boost_ext::is_convertible< typename boost_ext::add_reference<T>::type, Result >,
          boost::mpl::apply1< ResultPred, typename boost_ext::add_reference<T>::type >
      >
{ };

template< class T, class Result, class ResultPred >
struct builtin_has_operator_star_dispatch_on_function< T, Result, ResultPred, true >
    : boost_ext::mpl::and2<
#if SAKE_WORKAROUND_MSC_VERSION_LESS_EQUAL( 1500 )
          boost_ext::is_convertible< T*, Result >,
          boost::mpl::apply1< ResultPred, T* >
#else // #if SAKE_WORKAROUND_MSC_VERSION_LESS_EQUAL( 1500 )
          boost_ext::is_convertible< T&, Result >,
          boost::mpl::apply1< ResultPred, T& >
#endif // #if SAKE_WORKAROUND_MSC_VERSION_LESS_EQUAL( 1500 )
      >
{ };

template< class T, class Result, class ResultPred >
struct builtin_has_operator_star_dispatch_on_pointer
    : boost::false_type
{ };

template< class T, class Result, class ResultPred >
struct builtin_has_operator_star_dispatch_on_pointer< T*, Result, ResultPred >
    : builtin_has_operator_star_dispatch_on_function< T, Result, ResultPred >
{ };

template< class T, class Result, class ResultPred >
struct builtin_has_operator_star
    : builtin_has_operator_star_dispatch_on_pointer<
          typename boost_ext::remove_qualifiers<T>::type,
          Result,
          ResultPred
      >
{ };

namespace
{

#define test( T, Result ) \
    BOOST_STATIC_ASSERT( SAKE_EXPR_APPLY( \
        boost_ext::mpl::curry_quote2< boost::is_same >::apply< Result >::type, \
        *sake::declval<T>() \
    ) );
test( int*, int& )
test( int const *, int const & )
#if SAKE_WORKAROUND_MSC_VERSION_LESS_EQUAL( 1500 )
test( void (*)( ), void (*)( ) )
#else // #if SAKE_WORKAROUND_MSC_VERSION_LESS_EQUAL( 1500 )
test( void (*)( ), void (&)( ) )
#endif // #if SAKE_WORKAROUND_MSC_VERSION_LESS_EQUAL( 1500 )
#undef test

} // namespace

} // namespace introspection_private

} // namespace sake

#endif // #ifndef SAKE_CORE_INTROSPECTION_PRIVATE_BUILTIN_HAS_OPERATOR_STAR_HPP

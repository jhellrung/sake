/*******************************************************************************
 * sake/core/introspection/has_operator_assign.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_INTROSPECTION_HAS_OPERATOR_ASSIGN_HPP
#define SAKE_CORE_INTROSPECTION_HAS_OPERATOR_ASSIGN_HPP

#include <boost/mpl/always.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/add_reference_add_const.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>
#include <sake/boost_ext/type_traits/remove_reference.hpp>

#include <sake/core/introspection/has_member_function.hpp>
#include <sake/core/introspection/is_callable_member_function.hpp>
#include <sake/core/introspection/private/builtin_has_operator_assign.hpp>
#include <sake/core/move/rv.hpp>
#include <sake/core/utility/has_private_assign.hpp>

namespace sake
{

SAKE_INTROSPECTION_DEFINE_HAS_MEMBER_FUNCTION(
    has_mem_fun_operator_assign,
    operator=
)

#define SAKE_INTROSPECTION_TRAIT_NAME           has_operator_assign
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME operator=
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_DEFAULT_SIGNATURE( T ) \
    typename ::sake::boost_ext::add_reference<T>::type \
        ( typename ::sake::boost_ext::add_reference_add_const<T>::type )
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY_LIMITS ( 1, 1 )
#define SAKE_INTROSPECTION_BUILTIN_HAS_MEMBER_FUNCTION( T, Signature, ResultPred ) \
    ::sake::introspection_private::builtin_has_operator_assign< T, Signature, ResultPred >
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_MEMBER_FUNCTION()

// Since it is not uncommon for a class to declare its copy assignment operator
// private, we take extra pains to easily support this (via extension of the
// has_private_assign metafunction) and still be able to identify move
// assignable types.

namespace ext
{

namespace has_operator_assign_private
{

template< class T, class Signature, class ResultPred >
struct impl
    : boost::false_type
{ };

template< class T, class ResultPred >
struct impl< T, void, ResultPred >
    : impl<
          T, void ( T ),
          boost::mpl::always< boost::true_type >
      >
{ };

template< class T, class Result, class ResultPred >
struct impl< T, Result ( T ), ResultPred >
    : boost_ext::mpl::and3<
          sake::has_mem_fun_operator_assign< T, T& ( SAKE_RV_REF( T ) ) >,
          boost_ext::is_convertible< T&, Result >,
          boost::mpl::apply1< ResultPred, T& >
      >
{ };

template< class T, class Result, class ResultPred >
struct impl< T const, Result ( T ), ResultPred >
    : boost::false_type
{ };

} // namespace has_operator_assign_private

template< class T, class Signature, class ResultPred >
struct has_operator_assign<
    T, Signature, ResultPred,
    typename boost::enable_if_c<
        sake::has_private_assign<
            typename boost_ext::remove_qualifiers<T>::type
        >::value
    >::type
>
    : has_operator_assign_private::impl<
          typename boost_ext::remove_reference<T>::type,
          Signature,
          ResultPred
      >
{ };

} // namespace ext

} // namespace sake

#endif // #ifndef SAKE_CORE_INTROSPECTION_HAS_OPERATOR_ASSIGN_HPP
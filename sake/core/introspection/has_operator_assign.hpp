/*******************************************************************************
 * sake/core/introspection/has_operator_assign.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct has_operator_assign<
 *     T, U = T,
 *     Result = typename boost_ext::add_reference<T>::type,
 *     ResultPred = boost::mpl::always< boost::true_type >
 * >
 ******************************************************************************/

#ifndef SAKE_CORE_INTROSPECTION_HAS_OPERATOR_ASSIGN_HPP
#define SAKE_CORE_INTROSPECTION_HAS_OPERATOR_ASSIGN_HPP

#include <boost/config.hpp>
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
#include <sake/core/move/has_move_emulation.hpp>
#include <sake/core/move/rv.hpp>
#include <sake/core/type_traits/has_private_operator_assign.hpp>

namespace sake
{

SAKE_INTROSPECTION_DEFINE_HAS_MEMBER_FUNCTION(
    has_mem_fun_operator_assign,
    operator=
)

#define SAKE_INTROSPECTION_TRAIT_NAME           has_operator_assign
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME operator=
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME_IS_OPERATOR_ASSIGN
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_DEFAULT_SIGNATURE( T ) \
    typename ::sake::boost_ext::add_reference<T>::type \
        ( typename ::sake::boost_ext::add_reference_add_const<T>::type )
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY 1
#define SAKE_INTROSPECTION_BUILTIN_HAS_MEMBER_FUNCTION( T, Signature, ResultPred ) \
    ::sake::introspection_private::builtin_has_operator_assign< T, Signature, ResultPred >
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_MEMBER_FUNCTION()

// Since it is not uncommon for a class to declare a private assignment
// operator, and since nearly every such case is to disable all assignment
// *except* perhaps move assignment, we take extra pains to address this.

namespace extension
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
#ifndef BOOST_NO_RVALUE_REFERENCES
    : boost_ext::mpl::and3<
#else // #ifndef BOOST_NO_RVALUE_REFERENCES
    : boost_ext::mpl::and4<
          sake::has_move_emulation<T>,
#endif // #ifndef BOOST_NO_RVALUE_REFERENCES
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
        sake::has_private_operator_assign<
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

} // namespace extension

} // namespace sake

#endif // #ifndef SAKE_CORE_INTROSPECTION_HAS_OPERATOR_ASSIGN_HPP

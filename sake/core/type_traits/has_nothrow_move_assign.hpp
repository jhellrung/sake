/*******************************************************************************
 * sake/core/type_traits/has_nothrow_move_assign.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct has_nothrow_move_assign<T>
 * struct extension::has_nothrow_move_assign< T, Enable = void >
 * struct default_impl::has_nothrow_move_assign<T>
 *
 * Evaluates to true only if T has a non-throwing copy constructor.
 ******************************************************************************/

#ifndef SAKE_CORE_TYPE_TRAITS_HAS_NOTHROW_MOVE_ASSIGN_HPP
#define SAKE_CORE_TYPE_TRAITS_HAS_NOTHROW_MOVE_ASSIGN_HPP

#include <boost/mpl/identity.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/type_traits/remove_cv.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/type_traits/remove_reference.hpp>

#include <sake/core/introspection/has_operator_assign.hpp>
#include <sake/core/introspection/has_type.hpp>
#include <sake/core/type_traits/has_nothrow_copy_assign.hpp>
#include <sake/core/type_traits/has_nothrow_move_assign_fwd.hpp>

namespace sake
{

SAKE_INTROSPECTION_DEFINE_HAS_TYPE(
   has_type_has_nothrow_move_assign_tag,
   has_nothrow_move_assign_tag
)

/*******************************************************************************
 * struct has_nothrow_move_assign<T>
 ******************************************************************************/

namespace has_nothrow_move_assign_private
{

template< class T >
struct impl
    : boost_ext::mpl::and2<
          sake::has_operator_assign< T&, void ( typename boost::remove_cv<T>::type ) >,
          sake::extension::has_nothrow_move_assign<T>
      >
{ };

} // namespace has_nothrow_move_assign_private

template< class T >
struct has_nothrow_move_assign
    : has_nothrow_move_assign_private::impl<
          typename boost_ext::remove_reference<T>::type >
{ };

/*******************************************************************************
 * struct extension::has_nothrow_move_assign< T, Enable = void >
 ******************************************************************************/

namespace extension
{

template< class T, class Enable /*= void*/ >
struct has_nothrow_move_assign
    : sake::default_impl::has_nothrow_move_assign<T>
{ };

} // namespace extension

/*******************************************************************************
 * struct default_impl::has_nothrow_move_assign<T>
 ******************************************************************************/

namespace default_impl
{

template< class T >
struct has_nothrow_move_assign
    : boost::mpl::if_<
          sake::has_type_has_nothrow_move_assign_tag<T>,
          sake::has_type_has_nothrow_move_assign_tag<T,
              boost::mpl::quote1< boost::mpl::identity > >,
          sake::has_nothrow_copy_assign<T>
      >::type
{ };

} // namespace default_impl

} // namespace sake

#endif // #ifndef SAKE_CORE_TYPE_TRAITS_HAS_NOTHROW_MOVE_ASSIGN_HPP

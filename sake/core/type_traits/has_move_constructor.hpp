/*******************************************************************************
 * sake/core/type_traits/has_move_constructor.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct has_move_constructor<T>
 * struct extension::has_move_constructor< T, Enable = void >
 * struct default_impl::has_move_constructor<T>
 *
 * Evaluates to true only if T has a move constructor.
 ******************************************************************************/

#ifndef SAKE_CORE_TYPE_TRAITS_HAS_MOVE_CONSTRUCTOR_HPP
#define SAKE_CORE_TYPE_TRAITS_HAS_MOVE_CONSTRUCTOR_HPP

#include <boost/mpl/identity.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/type_traits/has_trivial_copy.hpp>
#include <boost/type_traits/remove_cv.hpp>

#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/type_traits/is_reference.hpp>

#include <sake/core/introspection/has_type.hpp>
#include <sake/core/type_traits/has_copy_constructor.hpp>
#include <sake/core/type_traits/has_move_constructor_fwd.hpp>
#include <sake/core/type_traits/has_nothrow_move_constructor.hpp>

namespace sake
{

SAKE_INTROSPECTION_DEFINE_HAS_TYPE(
   has_type_has_move_constructor_tag,
   has_move_constructor_tag
)

/*******************************************************************************
 * struct has_move_constructor<T>
 ******************************************************************************/

namespace has_move_constructor_private
{

template< class T >
struct impl
    : boost_ext::mpl::or3<
          boost_ext::is_reference<T>,
          boost::has_trivial_copy_constructor<T>,
          sake::extension::has_move_constructor<T>
      >
{ };

} // namespace has_move_constructor_private

template< class T >
struct has_move_constructor
    : has_move_constructor_private::impl<
          typename boost::remove_cv<T>::type >
{ };

/*******************************************************************************
 * struct extension::has_move_constructor< T, Enable = void >
 ******************************************************************************/

namespace extension
{

template< class T, class Enable /*= void*/ >
struct has_move_constructor
    : sake::default_impl::has_move_constructor<T>
{ };

} // namespace extension

/*******************************************************************************
 * struct default_impl::has_move_constructor<T>
 ******************************************************************************/

namespace default_impl
{

template< class T >
struct has_move_constructor
    : boost::mpl::if_<
          sake::has_type_has_move_constructor_tag<T>,
          sake::has_type_has_move_constructor_tag<T,
              boost::mpl::quote1< boost::mpl::identity > >,
          boost_ext::mpl::or2<
              sake::has_nothrow_move_constructor<T>,
              sake::has_copy_constructor<T>
          >
      >::type
{ };

} // namespace default_impl

} // namespace sake

#endif // #ifndef SAKE_CORE_TYPE_TRAITS_HAS_MOVE_CONSTRUCTOR_HPP

/*******************************************************************************
 * sake/core/type_traits/has_nothrow_default_constructor.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct has_nothrow_default_constructor<T>
 * struct extension::has_nothrow_default_constructor< T, Enable = void >
 * struct default_impl::has_nothrow_default_constructor<T>
 *
 * Evaluates to true only if T has a non-throwing default constructor.
 ******************************************************************************/

#ifndef SAKE_CORE_TYPE_TRAITS_HAS_NOTHROW_DEFAULT_CONSTRUCTOR_HPP
#define SAKE_CORE_TYPE_TRAITS_HAS_NOTHROW_DEFAULT_CONSTRUCTOR_HPP

#include <boost/mpl/identity.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/type_traits/has_nothrow_constructor.hpp>
#include <boost/type_traits/has_trivial_constructor.hpp>
#include <boost/type_traits/is_pod.hpp>
#include <boost/type_traits/remove_volatile.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/type_traits/is_reference.hpp>

#include <sake/core/introspection/has_type.hpp>
#include <sake/core/type_traits/has_nothrow_default_constructor_fwd.hpp>

namespace sake
{

SAKE_INTROSPECTION_DEFINE_HAS_TYPE(
   has_type_has_nothrow_default_constructor_tag,
   has_nothrow_default_constructor_tag
)

/*******************************************************************************
 * struct has_nothrow_default_constructor<T>
 ******************************************************************************/

namespace has_nothrow_default_constructor_private
{

template< class T >
struct impl
    : boost_ext::mpl::and2<
          boost::mpl::not_< boost_ext::is_reference<T> >,
          boost_ext::mpl::or3<
              boost::has_nothrow_default_constructor<T>,
              boost::has_trivial_default_constructor<T>,
              sake::extension::has_nothrow_default_constructor<T>
          >
      >
{ };

} // namespace has_nothrow_default_constructor_private

template< class T >
struct has_nothrow_default_constructor
    : has_nothrow_default_constructor_private::impl<
          typename boost::remove_volatile<T>::type >
{ };

template< class T >
struct has_nothrow_default_constructor< T const >
    : boost_ext::mpl::and2<
          boost::mpl::not_< boost::is_pod<T> >,
          sake::has_nothrow_default_constructor<T>
      >
{ };

/*******************************************************************************
 * struct extension::has_nothrow_default_constructor< T, Enable = void >
 ******************************************************************************/

namespace extension
{

template< class T, class Enable /*= void*/ >
struct has_nothrow_default_constructor
    : sake::default_impl::has_nothrow_default_constructor<T>
{ };

} // namespace extension

/*******************************************************************************
 * struct default_impl::has_nothrow_default_constructor<T>
 ******************************************************************************/

namespace default_impl
{

template< class T >
struct has_nothrow_default_constructor
    : sake::has_type_has_nothrow_default_constructor_tag<
          T, boost::mpl::quote1< boost::mpl::identity > >
{ };

} // namespace default_impl

} // namespace sake

#endif // #ifndef SAKE_CORE_TYPE_TRAITS_HAS_NOTHROW_DEFAULT_CONSTRUCTOR_HPP

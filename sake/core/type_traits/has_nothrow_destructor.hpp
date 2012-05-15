/*******************************************************************************
 * sake/core/type_traits/has_nothrow_destructor.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct has_nothrow_destructor<T>
 * struct extension::has_nothrow_destructor< T, Enable = void >
 * struct default_impl::has_nothrow_destructor<T>
 *
 * Evaluates to true only if T has a non-throwing default constructor.
 ******************************************************************************/

#ifndef SAKE_CORE_TYPE_TRAITS_HAS_NOTHROW_DESTRUCTOR_HPP
#define SAKE_CORE_TYPE_TRAITS_HAS_NOTHROW_DESTRUCTOR_HPP

#include <boost/mpl/not.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/type_traits/has_trivial_destructor.hpp>
#include <boost/type_traits/remove_cv.hpp>

#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/type_traits/is_reference.hpp>

#include <sake/core/introspection/has_type.hpp>
#include <sake/core/type_traits/has_nothrow_destructor_fwd.hpp>

namespace sake
{

SAKE_INTROSPECTION_DEFINE_HAS_TYPE(
   has_type_has_nothrow_destructor_tag,
   has_nothrow_destructor_tag
)

/*******************************************************************************
 * struct has_nothrow_destructor<T>
 ******************************************************************************/

namespace has_nothrow_destructor_private
{

template< class T >
struct impl
    : boost_ext::mpl::or3<
          boost_ext::is_reference<T>,
          boost::has_trivial_destructor<T>,
          sake::extension::has_nothrow_destructor<T>
      >
{ };

} // namespace has_nothrow_destructor_private

template< class T >
struct has_nothrow_destructor
    : has_nothrow_destructor_private::impl<
          typename boost::remove_cv<T>::type >
{ };

/*******************************************************************************
 * struct extension::has_nothrow_destructor< T, Enable = void >
 ******************************************************************************/

namespace extension
{

template< class T, class Enable /*= void*/ >
struct has_nothrow_destructor
    : sake::default_impl::has_nothrow_destructor<T>
{ };

} // namespace extension

/*******************************************************************************
 * struct default_impl::has_nothrow_destructor<T>
 ******************************************************************************/

namespace default_impl
{

template< class T >
struct has_nothrow_destructor
    : boost::mpl::not_< sake::has_type_has_nothrow_destructor_tag<
          T, boost::mpl::quote1< boost::mpl::not_ > > >
{ };

} // namespace default_impl

} // namespace sake

#endif // #ifndef SAKE_CORE_TYPE_TRAITS_HAS_NOTHROW_DESTRUCTOR_HPP

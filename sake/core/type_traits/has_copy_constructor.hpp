/*******************************************************************************
 * sake/core/type_traits/has_copy_constructor.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct has_copy_constructor<T>
 * struct extension::has_copy_constructor< T, Enable = void >
 * struct default_impl::has_copy_constructor<T>
 *
 * Evaluates to true only if T has a copy constructor.  Since there's no way,
 * in general, to determine this in C++03, the default implementation relies on
 * boost::has_trivial_copy_constructor, and additional non-pod classes should
 * be specialized explicitly.
 ******************************************************************************/

#ifndef SAKE_CORE_TYPE_TRAITS_HAS_COPY_CONSTRUCTOR_HPP
#define SAKE_CORE_TYPE_TRAITS_HAS_COPY_CONSTRUCTOR_HPP

#include <boost/mpl/identity.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/type_traits/has_trivial_copy_constructor.hpp>
#include <boost/type_traits/remove_cv.hpp>

#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/type_traits/is_reference.hpp>

#include <sake/core/introspection/has_type.hpp>
#include <sake/core/type_traits/has_copy_constructor_fwd.hpp>
#include <sake/core/type_traits/has_nothrow_copy_constructor.hpp>

namespace sake
{

SAKE_INTROSPECTION_DEFINE_HAS_TYPE(
   has_type_has_copy_constructor_tag,
   has_copy_constructor_tag
)

/*******************************************************************************
 * struct has_copy_constructor<T>
 ******************************************************************************/

namespace has_copy_constructor_private
{

template< class T >
struct impl
    : boost_ext::mpl::or3<
          boost_ext::is_reference<T>,
          boost::has_trivial_copy_constructor<T>,
          sake::extension::has_copy_constructor<T>
      >
{ };

} // namespace has_copy_constructor_private

template< class T >
struct has_copy_constructor
    : has_copy_constructor_private::impl<
          typename boost::remove_cv<T>::type >
{ };

/*******************************************************************************
 * struct extension::has_copy_constructor< T, Enable = void >
 ******************************************************************************/

namespace extension
{

template< class T, class Enable /*= void*/ >
struct has_copy_constructor
    : sake::default_impl::has_copy_constructor<T>
{ };

} // namespace extension

/*******************************************************************************
 * struct default_impl::has_copy_constructor<T>
 ******************************************************************************/

namespace default_impl
{

template< class T >
struct has_copy_constructor
    : boost_ext::mpl::or2<
          sake::has_type_has_copy_constructor_tag<
              T, boost::mpl::quote1< boost::mpl::identity > >,
          sake::has_nothrow_copy_constructor<T>
      >
{ };

} // namespace default_impl

} // namespace sake

#endif // #ifndef SAKE_CORE_TYPE_TRAITS_HAS_COPY_CONSTRUCTOR_HPP

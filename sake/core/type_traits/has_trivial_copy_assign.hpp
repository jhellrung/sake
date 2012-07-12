/*******************************************************************************
 * sake/core/type_traits/has_trivial_copy_assign.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct has_trivial_copy_assign<T>
 * struct extension::has_trivial_copy_assign< T, Enable = void >
 * struct default_impl::has_trivial_copy_assign<T>
 *
 * Evaluates to true only if T has a trivial copy assignment operator.
 ******************************************************************************/

#ifndef SAKE_CORE_TYPE_TRAITS_HAS_TRIVIAL_COPY_ASSIGN_HPP
#define SAKE_CORE_TYPE_TRAITS_HAS_TRIVIAL_COPY_ASSIGN_HPP

#include <boost/mpl/identity.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/type_traits/has_trivial_assign.hpp>
#include <boost/type_traits/integral_constant.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/type_traits/is_reference.hpp>

#include <sake/core/introspection/has_operator_assign.hpp>
#include <sake/core/introspection/has_type.hpp>
#include <sake/core/type_traits/fwd.hpp>

namespace sake
{

SAKE_INTROSPECTION_DEFINE_HAS_TYPE(
   has_type_has_trivial_copy_assign_tag,
   has_trivial_copy_assign_tag
)

/*******************************************************************************
 * struct has_trivial_copy_assign<T>
 ******************************************************************************/

namespace has_trivial_copy_assign_private
{

template< class T >
struct helper
    : boost_ext::mpl::and2<
          sake::has_operator_assign< T&, void ( T const & ) >,
          boost_ext::mpl::or2<
              boost::has_trivial_assign<T>,
              sake::extension::has_trivial_copy_assign<T>
          >
      >
{ };

} // namespace has_trivial_copy_assign_private

template< class T >
struct has_trivial_copy_assign
    : boost_ext::mpl::and2<
          boost::mpl::not_< boost_ext::is_reference<T> >,
          has_trivial_copy_assign_private::helper<T>
      >
{ };

template< class T >
struct has_trivial_copy_assign< T const >
    : boost::false_type
{ };

/*******************************************************************************
 * struct extension::has_trivial_copy_assign< T, Enable = void >
 ******************************************************************************/

namespace extension
{

template< class T, class Enable /*= void*/ >
struct has_trivial_copy_assign
    : sake::default_impl::has_trivial_copy_assign<T>
{ };

} // namespace extension

/*******************************************************************************
 * struct default_impl::has_trivial_copy_assign<T>
 ******************************************************************************/

namespace default_impl
{

template< class T >
struct has_trivial_copy_assign
    : sake::has_type_has_trivial_copy_assign_tag<
          T, boost::mpl::quote1< boost::mpl::identity > >
{ };

} // namespace default_impl

} // namespace sake

#endif // #ifndef SAKE_CORE_TYPE_TRAITS_HAS_TRIVIAL_COPY_ASSIGN_HPP

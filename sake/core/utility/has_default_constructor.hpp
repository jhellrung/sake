/*******************************************************************************
 * sake/core/utility/has_default_constructor.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct has_default_constructor<T>
 * struct extension::has_default_constructor< T, Enable = void >
 * struct default_impl::has_default_constructor<T>
 *
 * Evaluates to true only if T has a default constructor.  Since there's no way,
 * in general, to determine this in C++03, the default implementation relies on
 * boost::has_trivial_constructor, and additional non-pod classes should be
 * specialized explicitly.
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_HAS_DEFAULT_CONSTRUCTOR_HPP
#define SAKE_CORE_UTILITY_HAS_DEFAULT_CONSTRUCTOR_HPP

#include <boost/mpl/identity.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/type_traits/has_trivial_constructor.hpp>
#include <boost/type_traits/is_class.hpp>
#include <boost/type_traits/is_union.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/remove_volatile.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/mpl/or.hpp>

#include <sake/core/introspection/has_isc.hpp>
#include <sake/core/utility/has_default_constructor_fwd.hpp>

namespace sake
{

SAKE_INTROSPECTION_DEFINE_HAS_ISC(
    has_isc_has_default_constructor,
    has_default_constructor
)

/*******************************************************************************
 * struct has_default_constructor<T>
 ******************************************************************************/

template< class T >
struct has_default_constructor
    : extension::has_default_constructor<
          typename boost::remove_volatile<T>::type
      >
{ };

template< class T >
struct has_default_constructor< T const >
    : boost_ext::mpl::and2<
          boost_ext::mpl::or2<
              boost::is_class<T>,
              boost::is_union<T>
          >,
          sake::has_default_constructor<T>
      >
{ };

/*******************************************************************************
 * struct extension::has_default_constructor< T, Enable = void >
 ******************************************************************************/

namespace extension
{

template< class T, class Enable >
struct has_default_constructor< T, Enable /*= void*/ >
    : default_impl::has_default_constructor<T>
{ };

} // namespace extension

/*******************************************************************************
 * struct default_impl::has_default_constructor<T>
 ******************************************************************************/

namespace default_impl
{

template< class T >
struct has_default_constructor
    : boost_ext::mpl::or2<
          boost::has_trivial_constructor<
              typename boost::remove_cv<T>::type
          >,
          sake::has_isc_has_default_constructor<
              T,
              boost::mpl::quote1< boost::mpl::identity >
          >
      >
{ };

} // namespace default_impl

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_HAS_DEFAULT_CONSTRUCTOR_HPP

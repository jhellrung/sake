/*******************************************************************************
 * sake/core/iterator/private/adaptor/converting_constructor_enable.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_PRIVATE_ADAPTOR_CONVERTING_CONSTRUCTOR_ENABLE_HPP
#define SAKE_CORE_ITERATOR_PRIVATE_ADAPTOR_CONVERTING_CONSTRUCTOR_ENABLE_HPP

#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/mpl/at.hpp>
#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>
#include <sake/boost_ext/type_traits/propagate_const.hpp>

#include <sake/core/iterator/keyword.hpp>

namespace sake
{

namespace iterator_adaptor_private
{

template< class Params, class D, class P >
struct converting_constructor_enable_member
    : boost_ext::is_convertible<
          typename boost_ext::add_reference<
              typename boost_ext::propagate_const< D,
                  typename boost_ext::mpl::at<
                      P, sake::iterator_keyword::tag::member,
                      void
                  >::type
              >::type
          >::type,
          typename boost_ext::mpl::at<
              Params, sake::iterator_keyword::tag::member,
              void
          >::type
      >
{ };

template< class Params, class D, class P >
struct converting_constructor_enable_chained_base
    : boost_ext::is_convertible<
          typename boost_ext::propagate_const< D,
              typename boost_ext::mpl::at<
                  P, sake::iterator_keyword::tag::chained_base,
                  void
              >::type
          >::type &,
          typename boost_ext::mpl::at<
              Params, sake::iterator_keyword::tag::chained_base,
              void
          >::type
      >
{ };

template< class Base, class Params, class D, class B, class P >
struct converting_constructor_enable
    : boost_ext::mpl::and3<
          boost_ext::is_convertible< B const &, Base >,
          iterator_adaptor_private::converting_constructor_enable_member<
              Params, D, P >,
          iterator_adaptor_private::converting_constructor_enable_chained_base<
              Params, D, P >
      >
{ };

template< class Base, class Params, class D, class B, class P >
struct converting_constructor_enabler
    : boost::enable_if_c<
          iterator_adaptor_private::converting_constructor_enable<
              Base, Params, D, B, P
          >::value
      >
{ };

} // namespace iterator_adaptor_private

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_PRIVATE_ADAPTOR_CONVERTING_CONSTRUCTOR_ENABLE_HPP

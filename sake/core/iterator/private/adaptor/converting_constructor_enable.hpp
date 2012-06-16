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
#include <sake/boost_ext/type_traits/add_reference_add_const.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>
#include <sake/boost_ext/type_traits/is_convertible_wndp2bp.hpp>

#include <sake/core/iterator/adaptor_fwd.hpp>
#include <sake/core/iterator/keyword.hpp>

namespace sake
{

namespace iterator
{

namespace adaptor_private
{

template< class Params, class P >
struct converting_constructor_enable_member
    : boost_ext::is_convertible<
          typename boost_ext::add_reference_add_const<
              typename boost_ext::mpl::at<
                  P, sake::iterator::keyword::tag::member,
                  void
              >::type
          >::type,
          typename boost_ext::mpl::at<
              Params, sake::iterator::keyword::tag::member,
              void
          >::type
      >
{ };

template< class Params, class P >
struct converting_constructor_enable_chained_base
    : boost_ext::is_convertible<
          typename boost_ext::add_reference_add_const<
              typename boost_ext::mpl::at<
                  P, sake::iterator::keyword::tag::chained_base,
                  void
              >::type
          >::type,
          typename boost_ext::mpl::at<
              Params, sake::iterator::keyword::tag::chained_base,
              void
          >::type
      >
{ };

template< class I, class Params, class J, class P >
struct converting_constructor_enable
    : boost_ext::mpl::and3<
          boost_ext::is_convertible_wndp2bp< J const &, I >,
          converting_constructor_enable_member< Params, P >,
          converting_constructor_enable_chained_base< Params, P >
      >
{ };

template< class I, class Params, class J, class P >
struct converting_constructor_enabler
    : boost::enable_if_c< converting_constructor_enable<
          I, Params, J, P >::value >
{ };

} // namespace adaptor_private

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_PRIVATE_ADAPTOR_CONVERTING_CONSTRUCTOR_ENABLE_HPP

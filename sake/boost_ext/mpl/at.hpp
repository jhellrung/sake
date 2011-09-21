/*******************************************************************************
 * sake/boost_ext/mpl/at.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * boost_ext::mpl::at< Sequence, Key >
 * boost_ext::mpl::at< Sequence, Key, Default >
 *
 * For some reason, the boost::mpl::at<...> only takes 2 parameters, even though
 * it's documented to take 3 (useful for accessing associative sequences).  This
 * implements that functionality.
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_MPL_AT_HPP
#define SAKE_BOOST_EXT_MPL_AT_HPP

#include <boost/mpl/at.hpp>
#include <boost/mpl/void.hpp>

namespace sake
{

namespace boost_ext
{

namespace mpl
{

template< class Sequence, class Key, class Default = boost::mpl::void_ >
struct at;

namespace at_private
{

template< class T, class Default >
struct select
{ typedef T type; };

template< class Default >
struct select< boost::mpl::void_, Default >
{ typedef Default type; };

} // namespace at_private

template< class Sequence, class Key, class Default >
struct at
    : at_private::select<
          typename boost::mpl::at< Sequence, Key >::type,
          Default
      >
{ };

} // namespace mpl

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_MPL_AT_HPP

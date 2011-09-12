/*******************************************************************************
 * boost_ext/mpl/find_index.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::mpl::find_index< Sequence, T >
 *
 * This is similar to boost::mpl::find, except it evaluates to a Boost.MPL
 * integral constant rather than an iterator.
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_MPL_FIND_INDEX_HPP
#define SAKE_BOOST_EXT_MPL_FIND_INDEX_HPP

#include <boost/mpl/begin_end.hpp>
#include <boost/mpl/distance.hpp>
#include <boost/mpl/find.hpp>

namespace sake
{

namespace boost_ext
{

namespace mpl
{

template< class Sequence, class T >
struct find_index
    : boost::mpl::distance<
          typename boost::mpl::begin< Sequence >::type,
          typename boost::mpl::find< Sequence, T >::type
      >
{ };

} // namespace mpl

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_MPL_FIND_INDEX_HPP

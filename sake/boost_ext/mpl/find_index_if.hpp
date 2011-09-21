/*******************************************************************************
 * sake/boost_ext/mpl/find_index_if.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::mpl::find_index_if< Sequence, F >
 *
 * This is similar to boost::mpl::find_if, except it evaluates to a Boost.MPL
 * integral constant rather than an iterator.
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_MPL_FIND_INDEX_IF_HPP
#define SAKE_BOOST_EXT_MPL_FIND_INDEX_IF_HPP

#include <boost/mpl/begin_end.hpp>
#include <boost/mpl/distance.hpp>
#include <boost/mpl/find_if.hpp>

namespace sake
{

namespace boost_ext
{

namespace mpl
{

template< class Sequence, class F >
struct find_index_if
    : boost::mpl::distance<
          typename boost::mpl::begin< Sequence >::type,
          typename boost::mpl::find_if< Sequence, F >::type
      >
{ };

} // namespace mpl

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_MPL_FIND_INDEX_HPP

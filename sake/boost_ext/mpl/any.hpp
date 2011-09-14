/*******************************************************************************
 * boost_ext/mpl/any.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::mpl::any< Sequence, F >
 *
 * This is a metafunction which determines whether any element of the given
 * Boost.MPL sequence satisifies the given Boost.MPL metafunction.
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_MPL_ANY_HPP
#define SAKE_BOOST_EXT_MPL_ANY_HPP

#include <boost/mpl/begin_end.hpp>
#include <boost/mpl/find_if.hpp>
#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_same.hpp>

namespace sake
{

namespace boost_ext
{

namespace mpl
{

template< class Sequence, class F >
struct any
    : boost::mpl::not_<
          boost::is_same<
              typename boost::mpl::find_if< Sequence, F >::type,
              typename boost::mpl::end< Sequence >::type
          >
      >
{ };

} // namespace mpl

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_MPL_ANY_HPP

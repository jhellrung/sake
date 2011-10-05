/*******************************************************************************
 * sake/boost_ext/mpl/all.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::mpl::all< Sequence, F >
 *
 * This is a metafunction which determines whether all elements of the given
 * Boost.MPL sequence satisify the given Boost.MPL metafunction.
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_MPL_ALL_HPP
#define SAKE_BOOST_EXT_MPL_ALL_HPP

#include <boost/mpl/begin_end.hpp>
#include <boost/mpl/find_if.hpp>
#include <boost/mpl/lambda.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/protect.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/type_traits/is_same.hpp>

#include <sake/boost_ext/mpl/compose.hpp>

namespace sake
{

namespace boost_ext
{

namespace mpl
{

template< class Sequence, class F >
struct all
    : boost::is_same<
          typename boost::mpl::find_if<
              Sequence,
              boost_ext::mpl::compose<
                  boost::mpl::quote1< boost::mpl::not_ >,
                  boost::mpl::protect< typename boost::mpl::lambda<F>::type >
              >
          >::type,
          typename boost::mpl::end< Sequence >::type
      >
{ };

} // namespace mpl

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_MPL_ALL_HPP

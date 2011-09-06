/*******************************************************************************
 * boost_ext/mpl/abs.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::mpl::abs<N>
 *
 * boost_ext::mpl::abs is a Boost.MPL Metafunction which operators on Boost.MPL
 * Integral Constants.  abs computes the absolute value of its argument.
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_MPL_ABS_HPP
#define SAKE_BOOST_EXT_MPL_ABS_HPP

#include <boost/mpl/assert.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/less.hpp>
#include <boost/mpl/negate.hpp>

namespace sake
{

namespace boost_ext
{

namespace mpl
{

template< class N >
struct abs
    : boost::mpl::eval_if_c<
          boost::mpl::less< N, boost::mpl::int_<0> >::type::value,
          boost::mpl::negate<N>,
          boost::mpl::identity<N>
      >
{ };

BOOST_MPL_ASSERT_RELATION( boost_ext::mpl::abs< boost::mpl::int_<0> >::type::value, ==, 0 );
BOOST_MPL_ASSERT_RELATION( boost_ext::mpl::abs< boost::mpl::int_<+1> >::type::value, ==, +1 );
BOOST_MPL_ASSERT_RELATION( boost_ext::mpl::abs< boost::mpl::int_<-1> >::type::value, ==, +1 );

} // namespace mpl

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_MPL_ABS_HPP

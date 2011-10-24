/*******************************************************************************
 * sake/boost_ext/mpl/lcm.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::mpl::lcm< N0, N1 >
 *
 * A Boost.MPL metafunction operating on Boost.MPL integral constants which
 * computes the least common multiple of its arguments.
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_MPL_LCM_HPP
#define SAKE_BOOST_EXT_MPL_LCM_HPP

#include <boost/mpl/divides.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/times.hpp>
#include <boost/static_assert.hpp>

#include <sake/boost_ext/mpl/abs.hpp>
#include <sake/boost_ext/mpl/gcd.hpp>

namespace sake
{

namespace boost_ext
{

namespace mpl
{

namespace lcm_private
{

template< class N0, class N1 >
struct impl
    : boost::mpl::times<
          typename boost::mpl::divides<
              N0,
              typename gcd_private::dispatch< N0, N1 >::type
          >::type,
          N1
      >
{ };

} // namespace lcm_private

template< class N0, class N1 >
struct lcm
    : lcm_private::impl<
          typename boost_ext::mpl::abs< N0 >::type,
          typename boost_ext::mpl::abs< N1 >::type
      >
{ };

BOOST_MPL_ASSERT_RELATION( (boost_ext::mpl::lcm< boost::mpl::int_<1>, boost::mpl::int_<1> >::type::value), ==,  1 );
BOOST_MPL_ASSERT_RELATION( (boost_ext::mpl::lcm< boost::mpl::int_<1>, boost::mpl::int_<2> >::type::value), ==,  2 );
BOOST_MPL_ASSERT_RELATION( (boost_ext::mpl::lcm< boost::mpl::int_<2>, boost::mpl::int_<3> >::type::value), ==,  6 );
BOOST_MPL_ASSERT_RELATION( (boost_ext::mpl::lcm< boost::mpl::int_<2>, boost::mpl::int_<4> >::type::value), ==,  4 );
BOOST_MPL_ASSERT_RELATION( (boost_ext::mpl::lcm< boost::mpl::int_<4>, boost::mpl::int_<6> >::type::value), ==, 12 );

} // namespace mpl

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_MPL_LCM_HPP

/*******************************************************************************
 * boost_ext/mpl/gcd_lcm.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::mpl::gcd< N0, N1 >
 * struct boost_ext::mpl::lcm< N0, N1 >
 *
 * boost_ext::mpl::gcd and boost_ext::mpl::lcm are Boost.MPL metafunctions which
 * operator on Boost.MPL integral constants.  gcd computes the greatest common
 * divisor of its arguments; lcm computes the least common multiple of its
 * arguments.
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_MPL_GCD_LCM_HPP
#define SAKE_BOOST_EXT_MPL_GCD_LCM_HPP

#include <boost/mpl/assert.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/less.hpp>
#include <boost/mpl/modulus.hpp>
#include <boost/mpl/times.hpp>

#include <sake/boost_ext/mpl/abs.hpp>

namespace sake
{

namespace boost_ext
{

namespace mpl
{

template< class N0, class N1 > struct gcd;
template< class N0, class N1 > struct lcm;

namespace gcd_private
{

template<
    class N0, class N1,
    bool = boost::mpl::less< N0, N1 >::value,
    bool = boost::mpl::equal_to< N1, boost::mpl:int_<0> >::value
>
struct dispatch;

template< class N0, class N1 >
struct dispatch< N0, N1, true, false >
    : dispatch< N1, N0, false >
{ };

template< class N0, class N1 >
struct dispatch< N0, N1, false, false >
    : dispatch< N1, typename boost::mpl::modulus< N0, N1 >::type, false >
{ };

template< class N0, class N1 >
struct dispatch< N0, N1, false, true >
{
    BOOST_MPL_ASSERT_NOT((boost::mpl::equal_to< N0, boost::mpl::int_<0> >));
    typedef N0 type;
};

} // namespace gcd_private

template< class N0, class N1 >
struct gcd
    : gcd_private::dispatch<
          typename boost_ext::mpl::abs< N0 >::type,
          typename boost_ext::mpl::abs< N1 >::type
      >
{ };

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

BOOST_MPL_ASSERT_RELATION( (boost_ext::mpl::gcd< boost::mpl::int_<1>, boost::mpl::int_<1> >::type::value), ==, 1 );
BOOST_MPL_ASSERT_RELATION( (boost_ext::mpl::gcd< boost::mpl::int_<1>, boost::mpl::int_<2> >::type::value), ==, 1 );
BOOST_MPL_ASSERT_RELATION( (boost_ext::mpl::gcd< boost::mpl::int_<2>, boost::mpl::int_<3> >::type::value), ==, 1 );
BOOST_MPL_ASSERT_RELATION( (boost_ext::mpl::gcd< boost::mpl::int_<2>, boost::mpl::int_<4> >::type::value), ==, 2 );
BOOST_MPL_ASSERT_RELATION( (boost_ext::mpl::gcd< boost::mpl::int_<4>, boost::mpl::int_<6> >::type::value), ==, 2 );

BOOST_MPL_ASSERT_RELATION( (boost_ext::mpl::lcm< boost::mpl::int_<1>, boost::mpl::int_<1> >::type::value), ==,  1 );
BOOST_MPL_ASSERT_RELATION( (boost_ext::mpl::lcm< boost::mpl::int_<1>, boost::mpl::int_<2> >::type::value), ==,  2 );
BOOST_MPL_ASSERT_RELATION( (boost_ext::mpl::lcm< boost::mpl::int_<2>, boost::mpl::int_<3> >::type::value), ==,  6 );
BOOST_MPL_ASSERT_RELATION( (boost_ext::mpl::lcm< boost::mpl::int_<2>, boost::mpl::int_<4> >::type::value), ==,  4 );
BOOST_MPL_ASSERT_RELATION( (boost_ext::mpl::lcm< boost::mpl::int_<4>, boost::mpl::int_<6> >::type::value), ==, 12 );

} // namespace mpl

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_MPL_GCD_LCM_HPP

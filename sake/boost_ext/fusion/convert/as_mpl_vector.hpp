/*******************************************************************************
 * sake/boost_ext/fusion/convert/as_mpl_vector.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::fusion::result_of::as_mpl_vector< Sequence >
 *
 * This is a workaround when needing to apply boost::mpl algorithms to "adapted"
 * Boost.Fusion sequences (all "native" Boost.Fusion sequences can be made
 * conforming Boost.MPL sequences via inclusion of <boost/fusion/mpl.hpp>).
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_FUSION_CONVERT_AS_MPL_VECTOR_HPP
#define SAKE_BOOST_EXT_FUSION_CONVERT_AS_MPL_VECTOR_HPP

#include <boost/config.hpp>
#include <boost/fusion/iterator/equal_to.hpp>
#include <boost/fusion/iterator/next.hpp>
#include <boost/fusion/iterator/value_of.hpp>
#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/sequence/intrinsic/end.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/vector/vector0.hpp>

#include <sake/boost_ext/mpl/vector.hpp>

namespace sake
{

namespace boost_ext
{

namespace fusion
{

namespace result_of
{

namespace as_mpl_vector_private
{

template<
    class I, class E, class V,
    bool = boost::fusion::result_of::equal_to<I,E>::value
>
struct iterate;

} // namespace as_mpl_vector_private

template< class Sequence >
struct as_mpl_vector
    : as_mpl_vector_private::iterate<
          typename boost::fusion::result_of::begin< Sequence >::type,
          typename boost::fusion::result_of::end< Sequence >::type,
#ifndef BOOST_NO_VARIADIC_TEMPLATES
          boost_ext::mpl::vector<>
#else // #ifndef BOOST_NO_VARIADIC_TEMPLATES
          boost::mpl::vector0<>
#endif // #ifndef BOOST_NO_VARIADIC_TEMPLATES
      >
{ };

namespace as_mpl_vector_private
{

template< class I, class E, class V >
struct iterate< I,E,V, false >
    : iterate<
          typename boost::fusion::result_of::next<I>::type,
          E,
          typename boost::mpl::push_back<
              V, typename boost::fusion::result_of::value_of<I>::type >::type
      >
{ };

template< class I, class E, class V >
struct iterate< I,E,V, true >
{ typedef V type; };

} // namespace as_mpl_vector_private

} // namespace result_of

} // namespace fusion

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_FUSION_CONVERT_AS_MPL_VECTOR_HPP

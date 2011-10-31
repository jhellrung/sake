/*******************************************************************************
 * sake/boost_ext/fusion/convert/as_mpl_sequence.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::fusion::result_of::as_mpl_sequence< Sequence >
 *
 * This is a workaround when needing to apply boost::mpl algorithms to "adapted"
 * Boost.Fusion sequences (all "native" Boost.Fusion sequences can be made
 * conforming Boost.MPL sequences via inclusion of <boost/fusion/mpl.hpp>).
 *
 * If Sequence already happens to be a Boost.MPL sequence, then this amounts to
 * an identity operation.  Otherwise, Sequence is converted into a Boost.MPL
 * vector via boost_ext::fusion::result_of::as_mpl_vector.
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_FUSION_CONVERT_AS_MPL_SEQUENCE_HPP
#define SAKE_BOOST_EXT_FUSION_CONVERT_AS_MPL_SEQUENCE_HPP

#include <boost/mpl/is_sequence.hpp>

#include <sake/boost_ext/fusion/convert/as_mpl_vector.hpp>

namespace sake
{

namespace boost_ext
{

namespace fusion
{

namespace result_of
{

namespace as_mpl_sequence_private
{

template< class Sequence, bool = boost::mpl::is_sequence< Sequence >::value >
struct dispatch;

template< class Sequence >
struct dispatch< Sequence, false >
    : boost_ext::fusion::result_of::as_mpl_vector< Sequence >
{ };

template< class Sequence >
struct dispatch< Sequence, true >
{ typedef Sequence type; };

} // namespace detail_as_mpl_vector

template< class Sequence >
struct as_mpl_sequence
    : as_mpl_sequence_private::dispatch< Sequence >
{ };

} // namespace result_of

} // namespace fusion

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_FUSION_CONVERT_AS_MPL_SEQUENCE_HPP

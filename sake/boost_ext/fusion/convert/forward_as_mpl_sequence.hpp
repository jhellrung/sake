/*******************************************************************************
 * sake/boost_ext/fusion/convert/forward_as_mpl_sequence.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::fusion::result_of::forward_as_mpl_sequence< Sequence >
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_FUSION_CONVERT_FORWARD_AS_MPL_SEQUENCE_HPP
#define SAKE_BOOST_EXT_FUSION_CONVERT_FORWARD_AS_MPL_SEQUENCE_HPP

#include <boost/mpl/transform_view.hpp>

#include <sake/boost_ext/fusion/convert/as_mpl_sequence.hpp>
#include <sake/boost_ext/mpl/curry_quote.hpp>
#include <sake/boost_ext/type_traits/add_rvalue_reference.hpp>
#include <sake/boost_ext/type_traits/propagate_qualifiers.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

namespace sake
{

namespace boost_ext
{

namespace fusion
{

namespace result_of
{

template< class Sequence >
struct forward_as_mpl_sequence
{
    typedef boost::mpl::transform_view<
        typename boost_ext::fusion::result_of::as_mpl_sequence<
            typename boost_ext::remove_qualifiers< Sequence >::type >::type,
        typename boost_ext::mpl::curry_quote2<
            boost_ext::propagate_qualifiers >::apply<
            typename boost_ext::add_rvalue_reference< Sequence >::type >::type
    > type;
};

} // namespace result_of

} // namespace fusion

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_FUSION_CONVERT_FORWARD_AS_MPL_SEQUENCE_HPP

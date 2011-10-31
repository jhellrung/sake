/*******************************************************************************
 * sake/boost_ext/fusion/support/forward_sequence_is_convertible_wndp2bp.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_FUSION_SUPPORT_FORWARD_SEQUENCE_IS_CONVERTIBLE_WNDP2BP_HPP
#define SAKE_BOOST_EXT_FUSION_SUPPORT_FORWARD_SEQUENCE_IS_CONVERTIBLE_WNDP2BP_HPP

#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/transform_view.hpp>

#include <sake/boost_ext/fusion/convert/as_mpl_sequence.hpp>
#include <sake/boost_ext/mpl/sequence_is_convertible_wndp2bp.hpp>
#include <sake/boost_ext/type_traits/add_rvalue_reference.hpp>
#include <sake/boost_ext/type_traits/propagate_qualifiers.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

namespace sake
{

namespace boost_ext
{

namespace fusion
{

namespace traits
{

template< class FromSequence, class ToSequence >
struct forward_sequence_is_convertible_wndp2bp
    : boost_ext::mpl::sequence_is_convertible_wndp2bp<
          boost::mpl::transform_view<
              typename boost_ext::fusion::result_of::as_mpl_sequence<
                  typename boost_ext::remove_qualifiers< FromSequence >::type
              >::type,
              boost_ext::propagate_qualifiers<
                  typename boost_ext::add_rvalue_reference< FromSequence >::type,
                  boost::mpl::_1
              >
          >,
          typename boost_ext::fusion::result_of::as_mpl_sequence< ToSequence >::type
      >
{ };

} // namespace traits

} // namespace fusion

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_FUSION_SUPPORT_FORWARD_SEQUENCE_IS_CONVERTIBLE_WNDP2BP_HPP

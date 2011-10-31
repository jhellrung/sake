/*******************************************************************************
 * sake/boost_ext/fusion/support/sequence_is_convertible_wnrbt.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_FUSION_SUPPORT_SEQUENCE_IS_CONVERTIBLE_WNRBT_HPP
#define SAKE_BOOST_EXT_FUSION_SUPPORT_SEQUENCE_IS_CONVERTIBLE_WNRBT_HPP

#include <sake/boost_ext/fusion/convert/as_mpl_sequence.hpp>
#include <sake/boost_ext/mpl/sequence_is_convertible_wnrbt.hpp>

namespace sake
{

namespace boost_ext
{

namespace fusion
{

namespace traits
{

template< class FromSequence, class ToSequence >
struct sequence_is_convertible_wnrbt
    : boost_ext::mpl::sequence_is_convertible_wnrbt<
          typename boost_ext::fusion::result_of::as_mpl_sequence< FromSequence >::type,
          typename boost_ext::fusion::result_of::as_mpl_sequence< ToSequence >::type
      >
{ };

} // namespace traits

} // namespace fusion

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_FUSION_SUPPORT_SEQUENCE_IS_CONVERTIBLE_WNRBT_HPP

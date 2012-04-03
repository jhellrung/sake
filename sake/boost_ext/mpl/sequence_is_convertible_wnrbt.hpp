/*******************************************************************************
 * sake/boost_ext/mpl/sequence_is_convertible_wnrbt.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_MPL_SEQUENCE_IS_CONVERTIBLE_WNRBT_HPP
#define SAKE_BOOST_EXT_MPL_SEQUENCE_IS_CONVERTIBLE_WNRBT_HPP

#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/unpack_args.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/mpl/zip_view.hpp>

#include <sake/boost_ext/mpl/all.hpp>
#include <sake/boost_ext/mpl/and.hpp>

#include <sake/core/utility/is_convertible_wnrbt.hpp>

namespace sake
{

namespace boost_ext
{

namespace mpl
{

template< class FromSequence, class ToSequence >
struct sequence_is_convertible_wnrbt
    : boost_ext::mpl::and2<
          boost::mpl::equal_to<
              boost::mpl::size< FromSequence >,
              boost::mpl::size< ToSequence >
          >,
          boost_ext::mpl::all<
              boost::mpl::zip_view< boost::mpl::vector2< FromSequence, ToSequence > >,
              boost::mpl::unpack_args< boost::mpl::quote2< sake::is_convertible_wnrbt > >
          >
      >
{ };

} // namespace mpl

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_MPL_SEQUENCE_IS_CONVERTIBLE_WNRBT_HPP

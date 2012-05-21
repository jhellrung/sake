/*******************************************************************************
 * sake/boost_ext/mpl/common_result_type.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::mpl::has_common_result_type< Sequence >
 * struct boost_ext::mpl::common_result_type< Sequence >
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_MPL_COMMON_RESULT_TYPE_HPP
#define SAKE_BOOST_EXT_MPL_COMMON_RESULT_TYPE_HPP

#include <boost/mpl/fold.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/type_traits/is_void.hpp>

#include <sake/boost_ext/type_traits/common_result_type.hpp>

#include <sake/core/utility/convertible_to_any.hpp>

namespace sake
{

namespace boost_ext
{

namespace mpl
{

template< class Sequence >
struct common_result_type
    : boost::mpl::fold<
          Sequence,
          sake::convertible_to_any,
          boost::mpl::quote2< boost_ext::common_result_type >
      >
{ };

template< class Sequence >
struct has_common_result_type
    : boost::is_void<
          typename boost_ext::mpl::common_result_type< Sequence >::type
      >
{ };

} // namespace mpl

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_MPL_COMMON_RESULT_TYPE_HPP

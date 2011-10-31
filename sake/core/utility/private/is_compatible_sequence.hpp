/*******************************************************************************
 * sake/core/utility/private/is_compatible_sequence.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_PRIVATE_IS_COMPATIBLE_SEQUENCE_HPP
#define SAKE_CORE_UTILITY_PRIVATE_IS_COMPATIBLE_SEQUENCE_HPP

#include <boost/mpl/not.hpp>
#include <boost/mpl/size.hpp>
#include <boost/type_traits/is_base_of.hpp>

#include <sake/boost_ext/fusion/support/forward_sequence_is_convertible_wnrbt.hpp>
#include <sake/boost_ext/fusion/support/is_sequence_with_size.hpp>
#include <sake/boost_ext/fusion/support/is_sequence_with_category.hpp>
#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

namespace sake
{

namespace utility_private
{

template<
    class T, class Sequence,
    class Sequence_ = typename boost_ext::remove_qualifiers< Sequence >::type
>
struct is_compatible_sequence
    : boost_ext::mpl::and4<
          boost::mpl::not_< boost::is_base_of< T, Sequence_ > >,
          boost_ext::fusion::traits::is_sequence_with_size_c<
              Sequence_,
              boost::mpl::size< typename T::values_type >::value
          >,
          boost_ext::fusion::traits::is_random_access_sequence< Sequence_ >,
          boost_ext::fusion::traits::forward_sequence_is_convertible_wnrbt<
              Sequence,
              typename T::values_type
          >
      >
{ };

} // namespace utility_private

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_PRIVATE_IS_COMPATIBLE_SEQUENCE_HPP

/*******************************************************************************
 * sake/core/data_structures/tuple/private/sequence_constructor_enable.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_DATA_STRUCTURES_TUPLE_PRIVATE_SEQUENCE_CONSTRUCTOR_ENABLE_HPP
#define SAKE_CORE_DATA_STRUCTURES_TUPLE_PRIVATE_SEQUENCE_CONSTRUCTOR_ENABLE_HPP

#include <boost/mpl/equal.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/fusion/convert/forward_as_mpl_sequence.hpp>
#include <sake/boost_ext/fusion/support/is_sequence_with_category.hpp>
#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/is_base_of_sans_qualifiers.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>
#include <sake/boost_ext/type_traits/remove_rvalue_reference.hpp>

#include <sake/core/utility/is_convertible_wnrbt.hpp>

namespace sake
{

namespace tuple_private
{

template< class T, class Sequence >
struct sequence_constructor_enable_helper
    : boost::mpl::equal<
          typename boost_ext::fusion::result_of::
              forward_as_mpl_sequence< Sequence >::type,
          typename T::value_types,
          boost::mpl::quote2< sake::is_convertible_wnrbt >
      >
{ };

template< class T, class Sequence >
struct sequence_constructor_enable
    : boost_ext::mpl::and3<
          boost_ext::not_is_base_of_sans_qualifiers< T, Sequence >,
          boost_ext::fusion::traits::is_random_access_sequence<
              typename boost_ext::remove_qualifiers< Sequence >::type >,
          sequence_constructor_enable_helper< T, Sequence >
      >
{ };

template< class T, class Sequence >
struct sequence_constructor_enabler
    : boost::enable_if_c< sequence_constructor_enable<
          T, typename boost_ext::remove_rvalue_reference< Sequence >::type
      >::value >
{ };

} // namespace tuple_private

} // namespace sake

#endif // #ifndef SAKE_CORE_DATA_STRUCTURES_TUPLE_PRIVATE_SEQUENCE_CONSTRUCTOR_ENABLE_HPP

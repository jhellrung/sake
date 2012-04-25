/*******************************************************************************
 * sake/core/data_structures/tuple/private/operator_assign_enable.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_DATA_STRUCTURES_TUPLE_PRIVATE_OPERATOR_ASSIGN_ENABLE_HPP
#define SAKE_CORE_DATA_STRUCTURES_TUPLE_PRIVATE_OPERATOR_ASSIGN_ENABLE_HPP

#include <boost/mpl/equal.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/fusion/convert/forward_as_mpl_sequence.hpp>
#include <sake/boost_ext/fusion/support/is_sequence_with_category.hpp>
#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/is_base_of_sans_qualifiers.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/introspection/has_operator_assign.hpp>

namespace sake
{

namespace tuple_private
{

template< class T, class U >
struct has_operator_assign
    : sake::has_operator_assign<
          typename boost_ext::add_reference<T>::type,
          void ( U )
      >
{ };

template< class T, class Sequence >
struct operator_assign_enable_helper
    : boost::mpl::equal<
          typename T::values_type,
          typename boost_ext::fusion::result_of::
              forward_as_mpl_sequence< Sequence >::type,
          boost::mpl::quote2< sake::tuple_private::has_operator_assign >
      >
{ };

template< class T, class Sequence >
struct operator_assign_enable
    : boost_ext::mpl::and3<
          boost_ext::not_is_base_of_sans_qualifiers< T, Sequence >,
          boost_ext::fusion::traits::is_random_access_sequence<
              typename boost_ext::remove_qualifiers< Sequence >::type >,
          operator_assign_enable_helper< T, Sequence >
      >
{ };

template< class T, class Sequence >
struct operator_assign_enabler
    : boost::enable_if_c< operator_assign_enable< T, Sequence >::value, T& >
{ };

} // namespace tuple_private

} // namespace sake

#endif // #ifndef SAKE_CORE_DATA_STRUCTURES_TUPLE_PRIVATE_OPERATOR_ASSIGN_ENABLE_HPP

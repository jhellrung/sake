/*******************************************************************************
 * sake/boost_ext/fusion/support/is_sequence_with_category.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::fusion::traits::is_sequence_with_category< T, Tag = void >
 * struct boost_ext::fusion::traits::is_forward_sequence<T>
 * struct boost_ext::fusion::traits::is_bidirectional_sequence<T>
 * struct boost_ext::fusion::traits::is_random_access_sequence<T>
 * struct boost_ext::fusion::traits::is_associative_sequence<T>
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_FUSION_SUPPORT_IS_SEQUENCE_WITH_CATEGORY_HPP
#define SAKE_BOOST_EXT_FUSION_SUPPORT_IS_SEQUENCE_WITH_CATEGORY_HPP

#include <boost/fusion/support/category_of.hpp>
#include <boost/fusion/support/is_sequence.hpp>
#include <boost/type_traits/integral_constant.hpp>

#include <sake/boost_ext/type_traits/is_convertible.hpp>

namespace sake
{

namespace boost_ext
{

namespace fusion
{

namespace traits
{

namespace is_sequence_with_category_private
{

template<
    class T, class Tag,
    bool = boost::fusion::traits::is_sequence<T>::value
>
struct dispatch;

template< class T, class Tag >
struct dispatch< T, Tag, false >
    : boost::false_type
{ };

template< class T, class Tag >
struct dispatch< T, Tag, true >
    : boost_ext::is_convertible<
          typename boost::fusion::traits::category_of<T>::type,
          Tag
      >
{ };

} // namespace is_sequence_with_category_private

template< class T, class Tag = void >
struct is_sequence_with_category
    : is_sequence_with_category_private::dispatch< T, Tag >
{ };

template< class T >
struct is_sequence_with_category< T, void >
    : boost::fusion::traits::is_sequence<T>
{ };

template< class T >
struct is_forward_sequence
    : is_sequence_with_category< T, boost::fusion::forward_traversal_tag >
{ };

template< class T >
struct is_bidirectional_sequence
    : is_sequence_with_category< T, boost::fusion::bidirectional_traversal_tag >
{ };

template< class T >
struct is_random_access_sequence
    : is_sequence_with_category< T, boost::fusion::random_access_traversal_tag >
{ };

template< class T >
struct is_associative_sequence
    : is_sequence_with_category< T, boost::fusion::associative_tag >
{ };

} // namespace traits

} // namespace fusion

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_FUSION_SUPPORT_IS_SEQUENCE_WITH_CATEGORY_HPP

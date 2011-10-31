/*******************************************************************************
 * sake/boost_ext/fusion/support/is_sequence_with_size.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::fusion::traits::is_sequence_with_size< T, N = void >
 * struct boost_ext::fusion::traits::is_sequence_with_size_c<T,N>
 * struct boost_ext::fusion::traits::is_empty<T>
 * struct boost_ext::fusion::traits::is_singleton<T>
 * struct boost_ext::fusion::traits::is_pair<T>
 * struct boost_ext::fusion::traits::is_triple<T>
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_FUSION_SUPPORT_IS_SEQUENCE_WITH_SIZE_HPP
#define SAKE_BOOST_EXT_FUSION_SUPPORT_IS_SEQUENCE_WITH_SIZE_HPP

#include <boost/fusion/sequence/intrinsic/size.hpp>
#include <boost/fusion/support/is_sequence.hpp>

namespace sake
{

namespace boost_ext
{

namespace fusion
{

namespace traits
{

namespace is_sequence_with_size_private
{

template<
    class T, int N,
    bool = boost::fusion::traits::is_sequence<T>::value
>
struct dispatch;

template< class T, int N >
struct dispatch< T, N, false >
    : boost::false_type
{ };

template< class T, int N >
struct dispatch< T, N, true >
{
    static const bool value = N == boost::fusion::result_of::size<T>::type::value;
    typedef dispatch type;
};

} // namespace is_sequence_with_size_private

template< class T, int N >
struct is_sequence_with_size_c
    : is_sequence_with_size_private::dispatch<T,N>
{ };

template< class T, class N = void >
struct is_sequence_with_size
    : is_sequence_with_size_c< T, N::value >
{ };

template< class T >
struct is_sequence_with_size< T, void >
    : boost::fusion::traits::is_sequence<T>
{ };

template< class T >
struct is_empty
    : is_sequence_with_size_c<T,0>
{ };

template< class T >
struct is_singleton
    : is_sequence_with_size_c<T,1>
{ };

template< class T >
struct is_pair
    : is_sequence_with_size_c<T,2>
{ };

template< class T >
struct is_triple
    : is_sequence_with_size_c<T,3>
{ };

} // namespace traits

} // namespace fusion

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_FUSION_SUPPORT_IS_SEQUENCE_WITH_SIZE_HPP

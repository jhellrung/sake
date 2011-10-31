/*******************************************************************************
 * sake/boost_ext/mpl/reverse_adjacent_find_index.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::mpl::reverse_adjacent_find_index< Sequence, F >
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_MPL_REVERSE_ADJACENT_FIND_INDEX_HPP
#define SAKE_BOOST_EXT_MPL_REVERSE_ADJACENT_FIND_INDEX_HPP

#include <boost/mpl/apply.hpp>
#include <boost/mpl/begin_end.hpp>
#include <boost/mpl/next_prior.hpp>
#include <boost/mpl/size.hpp>

namespace sake
{

namespace boost_ext
{

namespace mpl
{

namespace reverse_adjacent_find_index_private
{

template<
    class Sequence, class F,
    int N = boost::mpl::size< Sequence >::value
>
struct dispatch;

template< class I, class F, int N, int K = N-1 >
struct iterate0;

template<
    class I, class F, int N, int K,
    class J = typename boost::mpl::prior<I>::type,
    bool = boost::mpl::apply2<
               F,
               typename boost::mpl::deref<J>::type,
               typename boost::mpl::deref<I>::type
           >::type::value
>
struct iterate1;

template< class Sequence, class F >
struct dispatch< Sequence, F, 0 >
{
    static int const value = 0;
    typedef dispatch type;
};

template< class Sequence, class F >
struct dispatch< Sequence, F, 1 >
{
    static int const value = 1;
    typedef dispatch type;
};

template< class Sequence, class F, int N >
struct dispatch
    : iterate0<
          typename boost::mpl::prior<
              typename boost::mpl::end< Sequence >::type
          >::type,
          F,
          N
      >
{ };

template< class I, class F, int N >
struct iterate0<I,F,N,0>
{
    static int const value = N;
    typedef iterate0 type;
};

template< class I, class F, int N, int K >
struct iterate0
    : iterate1<I,F,N,K>
{ };

template< class I, class F, int N, int K, class J >
struct iterate1< I,F,N,K, J, false >
    : iterate0<J,F,N,K-1>
{ };

template< class I, class F, int N, int K, class J >
struct iterate1< I,F,N,K, J, true >
{
    static int const value = K - 1;
    typedef iterate1 type;
};

} // namespace reverse_adjacent_find_index_private

template< class Sequence, class F >
struct reverse_adjacent_find_index
    : reverse_adjacent_find_index_private::dispatch< Sequence, F >
{ };

} // namespace mpl

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_MPL_REVERSE_ADJACENT_FIND_INDEX_HPP

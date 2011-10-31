/*******************************************************************************
 * sake/boost_ext/mpl/find_index.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::mpl::find_index< Sequence, T >
 *
 * This is similar to boost::mpl::find, except it evaluates to a Boost.MPL
 * integral constant rather than an iterator.
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_MPL_FIND_INDEX_HPP
#define SAKE_BOOST_EXT_MPL_FIND_INDEX_HPP

#include <boost/mpl/begin_end.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/next.hpp>

namespace sake
{

namespace boost_ext
{

namespace mpl
{

namespace find_index_private
{

template< class I, class E, class T, int K = 0 >
struct iterate0;

template<
    class I, class E, class T, int K,
    class U = typename boost::mpl::deref<I>::type
>
struct iterate1;

template< class I, class E, class T, int K >
struct iterate0
    : iterate1<I,E,T,K>
{ };

template< class E, class T, int K >
struct iterate0<E,E,T,K>
{
    static int const value = K;
    typedef iterate0 type;
};

template< class I, class E, class T, int K, class U >
struct iterate1
    : iterate0< typename boost::mpl::next<I>::type, E, T, K+1 >
{ };

template< class I, class E, class T, int K >
struct iterate1<I,E,T,K,T>
{
    static int const value = K;
    typedef iterate1 type;
};

} // namespace find_index_private

template< class Sequence, class T >
struct find_index
    : find_index_private::iterate0<
          typename boost::mpl::begin< Sequence >::type,
          typename boost::mpl::end< Sequence >::type,
          T
      >
{ };

} // namespace mpl

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_MPL_FIND_INDEX_HPP

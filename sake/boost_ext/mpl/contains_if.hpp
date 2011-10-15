/*******************************************************************************
 * sake/boost_ext/mpl/contains_if.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::mpl::contains_if< Sequence, Pred >
 *
 * Evaluates to true iff one or more elements in Sequence satisfy Pred.
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_MPL_CONTAINS_IF_HPP
#define SAKE_BOOST_EXT_MPL_CONTAINS_IF_HPP

#include <boost/mpl/apply.hpp>
#include <boost/mpl/begin_end.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/next.hpp>

#include <sake/boost_ext/mpl/or.hpp>

namespace sake
{

namespace boost_ext
{

namespace mpl
{

namespace contains_if_private
{

template< class I, class E, class F >
struct iterate
    : boost_ext::mpl::or2<
          boost::mpl::apply1< F, typename boost::mpl::deref<I>::type >,
          iterate< typename boost::mpl::next<I>::type, E, F >
      >
{ };

template< class E, class F >
struct iterate<E,E,F>
    : boost::mpl::false_
{ };

} // namespace contains_if_private

template< class Sequence, class Pred >
struct contains_if
    : contains_if_private::iterate<
          typename boost::mpl::begin< Sequence >::type,
          typename boost::mpl::end< Sequence >::type,
          Pred
      >
{ };

} // namespace mpl

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_MPL_CONTAINS_IF_HPP

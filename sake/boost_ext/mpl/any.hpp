/*******************************************************************************
 * sake/boost_ext/mpl/any.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::mpl::any< Sequence, F >
 *
 * This is a metafunction which determines whether any element of the given
 * Boost.MPL sequence satisifies the given Boost.MPL metafunction.
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_MPL_ANY_HPP
#define SAKE_BOOST_EXT_MPL_ANY_HPP

#include <boost/mpl/apply.hpp>
#include <boost/mpl/begin_end.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/next.hpp>
#include <boost/type_traits/integral_constant.hpp>

#include <sake/boost_ext/mpl/or.hpp>

namespace sake
{

namespace boost_ext
{

namespace mpl
{

template< class Sequence, class F >
struct any;

namespace any_private
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
    : boost::false_type
{ };

} // namespace any_private

template< class Sequence, class F >
struct any
    : any_private::iterate<
          typename boost::mpl::begin< Sequence >::type,
          typename boost::mpl::end< Sequence >::type,
          F
      >
{ };

} // namespace mpl

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_MPL_ANY_HPP

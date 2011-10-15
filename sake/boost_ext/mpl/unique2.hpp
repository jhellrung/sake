/*******************************************************************************
 * sake/boost_ext/mpl/unique2.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::mpl::unique2<
 *     Sequence,
 *     Pred     = boost::mpl::quote2< boost::is_same >,
 *     Inserter = boost::mpl::back_inserter< boost::mpl::vector0<> >
 * >
 *
 * boost_ext::mpl::unique2 is similar to boost::mpl::unique, except it removes
 * *all* duplicates, not just consecutive ones.  Unfortunately, this requires
 * a quadratic algorithm.
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_MPL_UNIQUE2_HPP
#define SAKE_BOOST_EXT_MPL_UNIQUE2_HPP

#include <boost/mpl/back_inserter.hpp>
#include <boost/mpl/copy.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/inserter.hpp>
#include <boost/mpl/lambda.hpp>
#include <boost/mpl/vector/vector0.hpp>
#include <boost/mpl/vector/vector10.hpp>

#include <sake/boost_ext/mpl/contains_if.hpp>

namespace sake
{

namespace boost_ext
{

namespace mpl
{

namespace unique2_private
{

template< class Pred, class U >
struct pred_binder
{
    template< class T >
    struct apply
        : boost::mpl::apply2< Pred, T, U >
    { };
};

template< class Pred, class Operation >
struct insert_operation
{
    template< class S, class T >
    struct apply
        : boost::mpl::eval_if_c<
              boost_ext::mpl::contains_if< S, pred_binder< Pred, T > >::value,
              boost::mpl::identity<S>,
              boost::mpl::apply2< Operation, S, T >
          >
    { };
};

} // namespace unique2_private

template<
    class Sequence,
    class Pred     = boost::mpl::quote2< boost::is_same >,
    class Inserter = boost::mpl::back_inserter< boost::mpl::vector0<> >
>
struct unique2
    : boost::mpl::copy<
          Sequence,
          boost::mpl::inserter<
              typename Inserter::state,
              unique2_private::insert_operation<
                  typename boost::mpl::lambda< Pred >::type,
                  typename Inserter::operation
              >
          >
      >
{ };

} // namespace mpl

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_MPL_UNIQUE2_HPP

/*******************************************************************************
 * sake/core/range/algorithm/accumulate.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_ALGORITHM_ACCUMULATE_HPP
#define SAKE_CORE_RANGE_ALGORITHM_ACCUMULATE_HPP

#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/range/traits.hpp>
#include <sake/core/range/traits_fwd.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>

namespace sake
{

namespace range
{

namespace algorithm
{

namespace result_of
{

template< class R, class T = void, class F = void >
struct accumulate
    : boost_ext::remove_qualifiers<T>
{ };

template< class R >
struct accumulate< R, void, void >
    : sake::range_value< typename boost_ext::remove_qualifiers<R>::type >
{ };

} // namespace result_of

namespace functional
{

struct accumulate
{
    SAKE_RESULT_FROM_METAFUNCTION(
        sake::range::algorithm::result_of::accumulate, (1,3) )

    template< class R >
    typename result< accumulate ( R const & ) >::type
    operator()(R const & r) const
    { throw 0; }

    template< class R, class T >
    typename result< accumulate ( R const &, T ) >::type
    operator()(R const & r, T const x) const
    { throw 0; }

    template< class R, class T, class F >
    typename result< accumulate ( R const &, T, F ) >::type
    operator()(R const & r, T const x, F f) const
    { throw 0; }
};

} // namespace functional

static sake::range::algorithm::functional::accumulate const accumulate = { };

} // namespace algorithm

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_ALGORITHM_ACCUMULATE_HPP

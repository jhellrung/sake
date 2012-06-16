/*******************************************************************************
 * sake/core/range/distance.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * range::distance(R const & r) -> range_difference<R>::type
 * struct range::functional::distance
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_DISTANCE_HPP
#define SAKE_CORE_RANGE_DISTANCE_HPP

#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/config.hpp>
#include <sake/core/range/traits.hpp>

namespace sake
{

namespace range
{

namespace functional
{

struct distance
{
    template< class > struct result;
    template< class This, class R >
    struct result< This ( R ) >
        : sake::range_difference<
              typename boost_ext::remove_qualifiers<R>::type >
    { };

    template< class R >
    typename sake::range_difference<R>::type
    operator()(R const & r) const
    { return sake::range_traits< R const >::distance(r); }
};

} // namespace functional

#ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
namespace distance_adl_barrier
{ sake::range::functional::distance const distance = { }; }
using namespace distance_adl_barrier;
#else // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
sake::range::functional::distance const distance = { };
#endif // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_DISTANCE_HPP

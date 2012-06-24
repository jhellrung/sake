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
#include <sake/core/range/traits_fwd.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>

namespace sake
{

namespace range
{

namespace result_of
{

template< class R >
struct distance
    : sake::range_difference<
          typename boost_ext::remove_qualifiers<R>::type const >
{ };

} // namespace result_of

namespace functional
{

struct distance
{
    SAKE_RESULT_FROM_METAFUNCTION( sake::range::result_of::distance, 1 )

    template< class R >
    typename sake::range::result_of::distance< R const & >::type
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

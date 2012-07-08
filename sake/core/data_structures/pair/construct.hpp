/*******************************************************************************
 * sake/core/data_structures/pair/construct.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * construct::pair(T0&& x0, T1&& x1)
 *     -> construct::result_of::pair< T0, T1 >::type
 ******************************************************************************/

#ifndef SAKE_CORE_DATA_STRUCTURES_PAIR_CONSTRUCT_HPP
#define SAKE_CORE_DATA_STRUCTURES_PAIR_CONSTRUCT_HPP

#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/config.hpp>
#include <sake/core/data_structures/pair/pair.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/ref/wrapped_parameter_to_reference.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>

namespace sake
{

namespace construct
{

namespace result_of
{

template< class T0, class T1 >
struct pair
{
    typedef sake::pair<
        typename sake::wrapped_parameter_to_reference<
            typename boost_ext::remove_qualifiers< T0 >::type >::type,
        typename sake::wrapped_parameter_to_reference<
            typename boost_ext::remove_qualifiers< T1 >::type >::type
    > type;
};

} // namespace result_of

namespace functional
{

struct pair
{
    SAKE_RESULT_FROM_METAFUNCTION( sake::construct::result_of::pair, 2 )

    template< class T0, class T1 >
    typename result< pair ( T0, T1 ) >::type
    operator()(SAKE_FWD_REF( T0 ) x0, SAKE_FWD_REF( T1 ) x1) const
    {
        return typename result< pair ( T0, T1 ) >::type(
            sake::forward< T0 >(x0), sake::forward< T1 >(x1));
    }
};

} // namespace functional

#ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
namespace pair_adl_barrier
{ sake::construct::functional::pair const pair = { }; }
using namespace pair_adl_barrier;
#else // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
sake::construct::functional::pair const pair = { };
#endif // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS

} // namespace construct

} // namespace sake

#endif // #ifndef SAKE_CORE_DATA_STRUCTURES_PAIR_CONSTRUCT_HPP

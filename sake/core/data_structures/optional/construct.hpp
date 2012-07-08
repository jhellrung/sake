/*******************************************************************************
 * sake/core/data_structures/optional/construct.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * construct::optional(T&& x) -> construct::result_of::optional<T>::type
 ******************************************************************************/

#ifndef SAKE_CORE_DATA_STRUCTURES_OPTIONAL_CONSTRUCT_HPP
#define SAKE_CORE_DATA_STRUCTURES_OPTIONAL_CONSTRUCT_HPP

#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/config.hpp>
#include <sake/core/data_structures/optional/optional.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/ref/wrapped_parameter_to_reference.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>

namespace sake
{

namespace construct
{

namespace result_of
{

template< class T >
struct optional
{ typedef sake::optional< typename sake::wrapped_parameter_to_reference<
    typename boost_ext::remove_qualifiers<T>::type >::type > type; };

} // namespace result_of

namespace functional
{

struct optional
{
    SAKE_RESULT_FROM_METAFUNCTION( sake::construct::result_of::optional, 1 )

    template< class T >
    typename result< optional ( T ) >::type
    operator()(SAKE_FWD_REF( T ) x) const
    { return typename result< optional ( T ) >::type(sake::forward<T>(x)); }
};

} // namespace functional

#ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
namespace optional_adl_barrier
{ sake::construct::functional::optional const optional = { }; }
using namespace optional_adl_barrier;
#else // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
sake::construct::functional::optional const optional = { };
#endif // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS

} // namespace construct

} // namespace sake

#endif // #ifndef SAKE_CORE_DATA_STRUCTURES_OPTIONAL_CONSTRUCT_HPP

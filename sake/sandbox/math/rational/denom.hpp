/*******************************************************************************
 * sake/core/math/rational/denom.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_RATIONAL_DENOM_HPP
#define SAKE_CORE_MATH_RATIONAL_DENOM_HPP

#include <sake/boost_ext/type_traits/add_rvalue_reference.hpp>
#include <sake/boost_ext/type_traits/propagate_qualifiers.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/config.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>

namespace sake
{

namespace result_of
{

template< class T >
struct denom
    : boost_ext::propagate_qualifiers<
          typename boost_ext::add_rvalue_reference<T>::type,
          typename boost_ext::remove_qualifiers<T>::type::integer_type
      >
{ };

} // namespace result_of

namespace functional
{

struct denom
{
    SAKE_RESULT_FROM_METAFUNCTION( sake::result_of::denom, 1 )

    template< class T >
    inline typename sake::result_of::denom< SAKE_FWD_PARAM( T ) >::type
    operator()(SAKE_FWD_REF( T ) x) const
    {
        typedef typename sake::result_of::denom< SAKE_FWD_PARAM( T ) >::type type;
        return static_cast< type >(x.denom());
    }
};

} // namespace functional

#ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
namespace denom_adl_barrier
{ sake::functional::denom const denom = { }; }
using namespace denom_adl_barrier;
#else // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
sake::functional::denom const denom = { };
#endif // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_RATIONAL_DENOM_HPP

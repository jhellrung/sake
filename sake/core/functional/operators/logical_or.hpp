/*******************************************************************************
 * sake/core/functional/operators/logical_or.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * operators::logical_or(T0&& x0, T1&& x1)
 *     -> operators::result_of::logical_or< T0, T1 >::type
 * struct operators::functional::logical_or
 *
 * operators::or_(T0&& x0, T1&& x1)
 *     -> operators::result_of::or_< T0, T1 >::type
 * struct operators::functional::or_
 *
 * struct operators::result_of::logical_or< T0, T1 >
 * struct operators::result_of::extension::logical_or< T0, T1, Enable = void >
 * struct operators::result_of::default_impl::logical_or< T0, T1 >
 ******************************************************************************/

#ifndef SAKE_CORE_FUNCTIONAL_OPERATORS_LOGICAL_OR_HPP
#define SAKE_CORE_FUNCTIONAL_OPERATORS_LOGICAL_OR_HPP

#define SAKE_OPERATORS_NAME logical_or
#define SAKE_OPERATORS_OP   ||
#include <sake/core/functional/operators/private/binary.ipp>

#include <sake/core/config.hpp>
#include <sake/core/functional/operators/fwd.hpp>

namespace sake
{

namespace operators
{

namespace result_of
{

template< class T0, class T1 >
struct or_
    : logical_or< T0, T1 >
{ };

} // namespace result_of

#ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
namespace or_adl_barrier
{ sake::operators::functional::or_ const or_ = { }; }
using namespace or_adl_barrier;
#else // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
sake::operators::functional::or_ const or_ = { };
#endif // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS

} // namespace operators

} // namespace sake

#endif // #ifndef SAKE_CORE_FUNCTIONAL_OPERATORS_LOGICAL_OR_HPP

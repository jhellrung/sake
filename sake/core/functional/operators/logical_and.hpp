/*******************************************************************************
 * sake/core/functional/operators/logical_and.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * operators::logical_and(T0&& x0, T1&& x1)
 *     -> operators::result_of::logical_and< T0, T1 >::type
 * struct operators::functional::logical_and
 *
 * operators::and_(T0&& x0, T1&& x1)
 *     -> operators::result_of::and_< T0, T1 >::type
 * struct operators::functional::and_
 *
 * struct operators::result_of::logical_and< T0, T1 >
 * struct operators::result_of::extension::logical_and< T0, T1, Enable = void >
 * struct operators::result_of::default_impl::logical_and< T0, T1 >
 ******************************************************************************/

#ifndef SAKE_CORE_FUNCTIONAL_OPERATORS_LOGICAL_AND_HPP
#define SAKE_CORE_FUNCTIONAL_OPERATORS_LOGICAL_AND_HPP

#define SAKE_OPERATORS_NAME logical_and
#define SAKE_OPERATORS_OP   &&
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
struct and_
    : logical_and< T0, T1 >
{ };

} // namespace result_of

#ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
namespace and_adl_barrier
{ sake::operators::functional::and_ const and_ = { }; }
using namespace and_adl_barrier;
#else // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
sake::operators::functional::and_ const and_ = { };
#endif // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS

} // namespace operators

} // namespace sake

#endif // #ifndef SAKE_CORE_FUNCTIONAL_OPERATORS_LOGICAL_AND_HPP

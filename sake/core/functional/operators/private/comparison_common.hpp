/*******************************************************************************
 * sake/core/functional/operators/private/comparison_common.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_FUNCTIONAL_OPERATORS_PRIVATE_COMPARISON_COMMON_HPP
#define SAKE_CORE_FUNCTIONAL_OPERATORS_PRIVATE_COMPARISON_COMMON_HPP

#include <boost/logic/tribool.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/type_traits/integral_constant.hpp>

#include <sake/core/math/indeterminate_fwd.hpp>

namespace sake
{

namespace operators
{

namespace result_of
{

namespace default_impl
{

typedef boost::mpl::vector7<
    bool,
    boost::logic::tribool,
    sake::functional::indeterminate,
    boost::true_type,
    boost::false_type,
    boost::mpl::true_,
    boost::mpl::false_
> comparison_result_types;

} // namespace default_impl

} // namespace result_of

} // namespace operators

} // namespace sake

#endif // #ifndef SAKE_CORE_FUNCTIONAL_OPERATORS_PRIVATE_COMPARISON_COMMON_HPP

/*******************************************************************************
 * sake/core/math/private/sign_common.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_PRIVATE_SIGN_COMMON_HPP
#define SAKE_CORE_MATH_PRIVATE_SIGN_COMMON_HPP

#include <boost/mpl/int.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/type_traits/integral_constant.hpp>

#include <sake/core/math/fuzzy_sign_t.hpp>
#include <sake/core/math/indeterminate.hpp>
#include <sake/core/math/sign_t.hpp>

namespace sake
{

namespace result_of
{

namespace default_impl
{

typedef boost::mpl::vector10<
    int,
    sake::sign_t,
    sake::fuzzy_sign_t,
    boost::integral_constant< int, -1 >,
    boost::integral_constant< int,  0 >,
    boost::integral_constant< int, +1 >,
    boost::mpl::int_<-1>,
    boost::mpl::int_< 0>,
    boost::mpl::int_<+1>,
    sake::functional::indeterminate
> sign_result_types;

} // namespace default_impl

} // namespace result_of

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_PRIVATE_SIGN_COMMON_HPP

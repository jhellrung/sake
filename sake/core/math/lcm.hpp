/*******************************************************************************
 * sake/core/math/lcm.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * lcm(T0 x0, T1 x1) -> result_of::lcm< T0, T1 >::type
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_LCM_HPP
#define SAKE_CORE_MATH_LCM_HPP

#include <sake/boost_ext/type_traits/common_type.hpp>

#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>
#include <sake/core/functional/result_from_metafunction.hpp>
#include <sake/core/math/abs_ip.hpp>
#include <sake/core/math/gcd.hpp>
#include <sake/core/move/move.hpp>

namespace sake
{

namespace result_of
{

template< class T0, class T1 >
struct lcm
    : boost_ext::common_type<
          typename boost_ext::remove_qualifiers< T0 >::type,
          typename boost_ext::remove_qualifiers< T1 >::type
      >
{ };

} // namespace result_of

namespace functional
{

struct lcm
{
    SAKE_RESULT_FROM_METAFUNCTION( result_of::lcm, 2 )

    template< class T0, class T1 >
    typename result_of::lcm< T0, T1 >::type
    operator()(T0 x0, T1 x1) const
    {
        sake::abs_ip(x0);
        sake::abs_ip(x1);
        return x1 < x0 ?
               sake::move(x0) * (sake::move(x1) / sake::gcd(x1, x0)) :
               (sake::move(x0) / sake::gcd(x0, x1)) * sake::move(x1);
    }
};

} // namespace functional

functional::lcm const lcm = { };

} // namespace sake

#endif // SAKE_CORE_MATH_LCM_HPP

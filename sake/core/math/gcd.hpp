/*******************************************************************************
 * sake/core/math/gcd.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * gcd(T0 x0, T1 x1) -> result_of::gcd< T0, T1 >::type
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_GCD_HPP
#define SAKE_CORE_MATH_GCD_HPP

#include <sake/boost_ext/type_traits/common_type.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/math/abs_ip.hpp>
#include <sake/core/math/zero.hpp>
#include <sake/core/move/move.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>

namespace sake
{

namespace gcd_private
{

template<
    class T0, class T1,
    class T = typename boost_ext::common_type< T0, T1 >::type
>
struct narrow;

template< class T >
inline T
helper(T& x0, T& x1);

} // namespace gcd_private

namespace result_of
{

template< class T0, class T1 >
struct gcd
    : boost_ext::common_type<
          typename boost_ext::remove_qualifiers< T0 >::type,
          typename boost_ext::remove_qualifiers< T1 >::type
      >
{ };

} // namespace result_of

namespace functional
{

struct gcd
{
    SAKE_RESULT_FROM_METAFUNCTION( result_of::gcd, 2 )

    template< class T0, class T1 >
    typename result_of::gcd< T0, T1 >::type
    operator()(T0 x0, T1 x1) const
    {
        typedef typename gcd_private::narrow< T0, T1 >::type narrow_type;
        sake::abs_ip(x0);
        sake::abs_ip(x1);
        if(x1 == sake::zero)
            return sake::move(x0);
        else if(x0 == sake::zero)
            return sake::move(x1);
        else if(x1 < x0) {
            narrow_type x0_(sake::move(x0) % x1);
            narrow_type x1_(sake::move(x1));
            return gcd_private::helper(x1_, x0_);
        }
        else {
            narrow_type x1_(sake::move(x1) % x0);
            narrow_type x0_(sake::move(x0));
            return gcd_private::helper(x0_, x1_);
        }
    }
};

} // namespace functional

functional::gcd const gcd = { };

namespace gcd_private
{

template< class T0, class T1, class T >
struct narrow
{ typedef T type; };

template< class T >
struct narrow< T, T, T >
{ typedef T type; };

template< class T0, class T1 >
struct narrow< T0, T1, T0 >
{ typedef T1 type; };

template< class T0, class T1 >
struct narrow< T0, T1, T1 >
{ typedef T0 type; };


template< class T >
inline T
helper(T& x0, T& x1)
{
    // Euclidean Algorithm
    while(!(x1 == sake::zero)) {
        T y = sake::move(x0) % x1;
        x0 = sake::move(x1);
        x1 = sake::move(y);
    }
    return x0;
}

} // namespace gcd_private

} // namespace sake

#endif // SAKE_CORE_MATH_GCD_HPP

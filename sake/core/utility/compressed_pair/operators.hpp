/*******************************************************************************
 * sake/core/utility/compressed_pair/operators.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct operators::result_of::extension::equal0< ... >
 * struct operators::result_of::extension::not_equal0< ... >
 * struct operators::result_of::extension::less0< ... >
 * struct operators::result_of::extension::greater0< ... >
 * struct operators::result_of::extension::less_equal0< ... >
 * struct operators::result_of::extension::greater_equal0< ... >
 * struct result_of::extension::cmp0< ... >
 *
 * operator==(compressed_pair< T0, T1 > const & x, compressed_pair< U0, U1 > const & y) -> ...
 * operator!=(compressed_pair< T0, T1 > const & x, compressed_pair< U0, U1 > const & y) -> ...
 * operator<(compressed_pair< T0, T1 > const & x, compressed_pair< U0, U1 > const & y) -> ...
 * operator>(compressed_pair< T0, T1 > const & x, compressed_pair< U0, U1 > const & y) -> ...
 * operator<=(compressed_pair< T0, T1 > const & x, compressed_pair< U0, U1 > const & y) -> ...
 * operator>=(compressed_pair< T0, T1 > const & x, compressed_pair< U0, U1 > const & y) -> ...
 * cmp(compressed_pair< T0, T1 > const & x, compressed_pair< U0, U1 > const & y) -> ...
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_COMPRESSED_PAIR_OPERATORS_HPP
#define SAKE_CORE_UTILITY_COMPRESSED_PAIR_OPERATORS_HPP

#include <sake/boost_ext/type_traits/add_reference_add_const.hpp>

#include <sake/core/functional/operators/equal.hpp>
#include <sake/core/functional/operators/greater.hpp>
#include <sake/core/functional/operators/greater_equal.hpp>
#include <sake/core/functional/operators/less.hpp>
#include <sake/core/functional/operators/less_equal.hpp>
#include <sake/core/functional/operators/logical_and.hpp>
#include <sake/core/functional/operators/logical_not.hpp>
#include <sake/core/functional/operators/logical_or.hpp>
#include <sake/core/functional/operators/not_equal.hpp>
#include <sake/core/math/cmp_fwd.hpp>
#include <sake/core/math/zero.hpp>
#include <sake/core/utility/compressed_pair/fwd.hpp>

namespace sake
{

namespace operators {
namespace result_of {
namespace extension {

template< class T0, class T1, class U0, class U1 >
struct equal0< sake::compressed_pair<T0,T1>, sake::compressed_pair<U0,U1>, void >
    : operators::result_of::and_<
          typename operators::result_of::equal<
              typename boost_ext::add_reference_add_const< T0 >::type,
              typename boost_ext::add_reference_add_const< U0 >::type
          >::type,
          typename operators::result_of::equal<
              typename boost_ext::add_reference_add_const< T1 >::type,
              typename boost_ext::add_reference_add_const< U1 >::type
          >::type
      >
{ };

template< class T0, class T1, class U0, class U1 >
struct not_equal0< sake::compressed_pair<T0,T1>, sake::compressed_pair<U0,U1>, void >
    : operators::result_of::or_<
          typename operators::result_of::not_equal<
              typename boost_ext::add_reference_add_const< T0 >::type,
              typename boost_ext::add_reference_add_const< U0 >::type
          >::type,
          typename operators::result_of::not_equal<
              typename boost_ext::add_reference_add_const< T1 >::type,
              typename boost_ext::add_reference_add_const< U1 >::type
          >::type
      >
{ };

template< class T0, class T1, class U0, class U1 >
struct less0< sake::compressed_pair<T0,T1>, sake::compressed_pair<U0,U1>, void >
    : operators::result_of::or_<
          typename operators::result_of::less<
              typename boost_ext::add_reference_add_const< T0 >::type,
              typename boost_ext::add_reference_add_const< U0 >::type
          >::type,
          typename operators::result_of::and_<
              typename operators::result_of::not_<
                  typename operators::result_of::less<
                      typename boost_ext::add_reference_add_const< U0 >::type,
                      typename boost_ext::add_reference_add_const< T0 >::type
                  >::type
              >::type
              typename operators::result_of::less<
                  typename boost_ext::add_reference_add_const< T1 >::type,
                  typename boost_ext::add_reference_add_const< U1 >::type
              >::type
          >::type
      >
{ };

template< class T0, class T1, class U0, class U1 >
struct greater0< sake::compressed_pair<T0,T1>, sake::compressed_pair<U0,U1>, void >
    : operators::result_of::or_<
          typename operators::result_of::greater<
              typename boost_ext::add_reference_add_const< T0 >::type,
              typename boost_ext::add_reference_add_const< U0 >::type
          >::type,
          typename operators::result_of::and_<
              typename operators::result_of::not_<
                  typename operators::result_of::greater<
                      typename boost_ext::add_reference_add_const< U0 >::type,
                      typename boost_ext::add_reference_add_const< T0 >::type
                  >::type
              >::type
              typename operators::result_of::greater<
                  typename boost_ext::add_reference_add_const< T1 >::type,
                  typename boost_ext::add_reference_add_const< U1 >::type
              >::type
          >::type
      >
{ };

template< class T0, class T1, class U0, class U1 >
struct less_equal0< sake::compressed_pair<T0,T1>, sake::compressed_pair<U0,U1>, void >
    : operators::result_of::and_<
          typename operators::result_of::less_equal<
              typename boost_ext::add_reference_add_const< T0 >::type,
              typename boost_ext::add_reference_add_const< U0 >::type
          >::type,
          typename operators::result_of::or_<
              typename operators::result_of::not_<
                  typename operators::result_of::less_equal<
                      typename boost_ext::add_reference_add_const< U0 >::type,
                      typename boost_ext::add_reference_add_const< T0 >::type
                  >::type
              >::type
              typename operators::result_of::less_equal<
                  typename boost_ext::add_reference_add_const< T1 >::type,
                  typename boost_ext::add_reference_add_const< U1 >::type
              >::type
          >::type
      >
{ };

template< class T0, class T1, class U0, class U1 >
struct greater_equal0< sake::compressed_pair<T0,T1>, sake::compressed_pair<U0,U1>, void >
    : operators::result_of::and_<
          typename operators::result_of::greater_equal<
              typename boost_ext::add_reference_add_const< T0 >::type,
              typename boost_ext::add_reference_add_const< U0 >::type
          >::type,
          typename operators::result_of::or_<
              typename operators::result_of::not_<
                  typename operators::result_of::greater_equal<
                      typename boost_ext::add_reference_add_const< U0 >::type,
                      typename boost_ext::add_reference_add_const< T0 >::type
                  >::type
              >::type
              typename operators::result_of::greater_equal<
                  typename boost_ext::add_reference_add_const< T1 >::type,
                  typename boost_ext::add_reference_add_const< U1 >::type
              >::type
          >::type
      >
{ };

} // namespace extension
} // namespace result_of
} // namespace operators

namespace result_of {
namespace extension {

template< class T0, class T1, class U0, class U1 >
struct cmp0< sake::compressed_pair<T0,T1>, sake::compressed_pair<U0,U1>, void >
    : boost_ext::common_type<
          typename result_of::cmp<
              typename boost_ext::add_reference_add_const< T0 >::type,
              typename boost_ext::add_reference_add_const< U0 >::type
          >::type,
          typename result_of::cmp<
              typename boost_ext::add_reference_add_const< T1 >::type,
              typename boost_ext::add_reference_add_const< U1 >::type
          >::type
      >
{ };

} // namespace extension
} // namespace result_of

namespace compressed_pair_adl
{

template< class T0, class T1, class U0, class U1 >
inline typename operators::result_of::equal<
    sake::compressed_pair<T0,T1>, sake::compressed_pair<U0,U1> >::type
operator==(sake::compressed_pair<T0,T1> const & x, sake::compressed_pair<U0,U1> const & y)
{ return x.first() == y.first() && x.second() == y.second(); }

template< class T0, class T1, class U0, class U1 >
inline typename operators::result_of::not_equal<
    sake::compressed_pair<T0,T1>, sake::compressed_pair<U0,U1> >::type
operator!=(sake::compressed_pair<T0,T1> const & x, sake::compressed_pair<U0,U1> const & y)
{ return x.first() != y.first() || x.second() != y.second(); }

template< class T0, class T1, class U0, class U1 >
inline typename operators::result_of::less<
    sake::compressed_pair<T0,T1>, sake::compressed_pair<U0,U1> >::type
operator<(sake::compressed_pair<T0,T1> const & x, sake::compressed_pair<U0,U1> const & y)
{ return x.first() < y.first() || (!(y.first() < x.first()) && x.second() < y.second()); }

template< class T0, class T1, class U0, class U1 >
inline typename operators::result_of::less<
    sake::compressed_pair<T0,T1>, sake::compressed_pair<U0,U1> >::type
operator>(sake::compressed_pair<T0,T1> const & x, sake::compressed_pair<U0,U1> const & y)
{ return x.first() > y.first() || (!(y.first() > x.first()) && x.second() > y.second()); }

template< class T0, class T1, class U0, class U1 >
inline typename operators::result_of::less<
    sake::compressed_pair<T0,T1>, sake::compressed_pair<U0,U1> >::type
operator<=(sake::compressed_pair<T0,T1> const & x, sake::compressed_pair<U0,U1> const & y)
{ return x.first() <= y.first() && (!(y.first() <= x.first()) || x.second() <= y.second()); }

template< class T0, class T1, class U0, class U1 >
inline typename operators::result_of::less<
    sake::compressed_pair<T0,T1>, sake::compressed_pair<U0,U1> >::type
operator>=(sake::compressed_pair<T0,T1> const & x, sake::compressed_pair<U0,U1> const & y)
{ return x.first() >= y.first() && (!(y.first() >= x.first()) || x.second() >= y.second()); }

template< class T0, class T1, class U0, class U1 >
inline typename result_of::cmp<
    sake::compressed_pair<T0,T1>, sake::compressed_pair<U0,U1> >::type
cmp(sake::compressed_pair< T0, T1 > const & x, sake::compressed_pair< U0, U1 > const & y)
{
    typename result_of::cmp<
        typename boost_ext::add_reference_add_const< T0 >::type,
        typename boost_ext::add_reference_add_const< U0 >::type
    >::type const s0 = sake::cmp(x.first(), y.first());
    return s0 == sake::zero ? sake::cmp(x.second(), y.second()) : s0;
}

} // namespace compressed_pair_adl

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_COMPRESSED_PAIR_OPERATORS_HPP

/*******************************************************************************
 * sake/core/data_structures/pair/operators.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct operators::result_of::extension::equal< ... >
 * struct operators::result_of::extension::not_equal< ... >
 * struct operators::result_of::extension::less< ... >
 * struct operators::result_of::extension::less_equal< ... >
 * struct operators::result_of::extension::greater< ... >
 * struct operators::result_of::extension::greater_equal< ... >
 * struct result_of::extension::cmp< ... >
 *
 * operator==(pair<T0,T1> const & x, pair<U0,U1> const & y) -> ...
 * operator!=(pair<T0,T1> const & x, pair<U0,U1> const & y) -> ...
 * operator<(pair<T0,T1> const & x, pair<U0,U1> const & y) -> ...
 * operator<=(pair<T0,T1> const & x, pair<U0,U1> const & y) -> ...
 * operator>(pair<T0,T1> const & x, pair<U0,U1> const & y) -> ...
 * operator>=(pair<T0,T1> const & x, pair<U0,U1> const & y) -> ...
 * cmp(pair<T0,T1> const & x, pair<U0,U1> const & y) -> ...
 ******************************************************************************/

#ifndef SAKE_CORE_DATA_STRUCTURES_PAIR_OPERATORS_HPP
#define SAKE_CORE_DATA_STRUCTURES_PAIR_OPERATORS_HPP

#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/type_traits/add_reference_add_const.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/data_structures/pair/fwd.hpp>
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

namespace sake
{

namespace operators
{

namespace result_of
{

namespace extension
{

template< class T0, class T1, class U0, class U1 >
struct equal< sake::pair<T0,T1>, sake::pair<U0,U1>, void >
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
struct not_equal< sake::pair<T0,T1>, sake::pair<U0,U1>, void >
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
struct less< sake::pair<T0,T1>, sake::pair<U0,U1>, void >
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

} // namespace extension

} // namespace result_of

} // namespace operators

namespace result_of
{

namespace extension
{

template< class T0, class T1, class U0, class U1 >
struct cmp< sake::pair<T0,T1>, sake::pair<U0,U1>, void >
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

template< class T, class U >
struct cmp< T, U, typename sake::pair_private::enable_binary<T,U>::type >
    : result_of::cmp<
          typename boost_ext::remove_qualifiers<T>::type,
          typename boost_ext::remove_qualifiers<U>::type
      >
{ };

} // namespace extension

} // namespace result_of

namespace pair_adl
{

template< class T0, class T1, class U0, class U1 >
inline typename operators::result_of::equal<
    sake::pair<T0,T1>,
    sake::pair<U0,U1>
>::type
operator==(sake::pair<T0,T1> const & x, sake::pair<U0,U1> const & y)
{ return x.first == y.first && x.second == y.second; }

template< class T0, class T1, class U0, class U1 >
inline typename operators::result_of::not_equal<
    sake::pair<T0,T1>,
    sake::pair<U0,U1>
>::type
operator!=(sake::pair<T0,T1> const & x, sake::pair<U0,U1> const & y)
{ return x.first != y.first || x.second != y.second; }

template< class T0, class T1, class U0, class U1 >
inline typename operators::result_of::less<
    sake::pair<T0,T1>,
    sake::pair<U0,U1>
>::type
operator<(sake::pair<T0,T1> const & x, sake::pair<U0,U1> const & y)
{ return x.first < y.first || (!(y.first < x.first) && x.second < y.second); }

template< class T0, class T1, class U0, class U1 >
inline typename result_of::cmp<
    sake::pair<T0,T1>,
    sake::pair<U0,U1>
>::type
cmp(sake::pair< T0, T1 > const & x, sake::pair< U0, U1 > const & y)
{
    typename result_of::cmp<
        typename boost_ext::add_reference_add_const< T0 >::type,
        typename boost_ext::add_reference_add_const< U0 >::type
    >::type const s0 = sake::cmp(x.first, y.first);
    return s0 == sake::zero ? sake::cmp(x.second, y.second) : s0;
}

} // namespace pair_adl

} // namespace sake

#endif // #ifndef SAKE_CORE_DATA_STRUCTURES_PAIR_OPERATORS_HPP

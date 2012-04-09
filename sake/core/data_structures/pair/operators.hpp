/*******************************************************************************
 * sake/core/data_structures/pair/operators.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct operators::result_of::extension::equal< T, U, ... >
 * struct operators::result_of::extension::not_equal< T, U, ... >
 * struct operators::result_of::extension::less< T, U, ... >
 * struct operators::result_of::extension::less_equal< T, U, ... >
 * struct operators::result_of::extension::greater< T, U, ... >
 * struct operators::result_of::extension::greater_equal< T, U, ... >
 * struct result_of::extension::compare< T, U, ... >
 *
 * operator==(pair<T0,T1> const & x, pair<U0,U1> const & y) -> ...
 * operator!=(pair<T0,T1> const & x, pair<U0,U1> const & y) -> ...
 * operator<(pair<T0,T1> const & x, pair<U0,U1> const & y) -> ...
 * operator<=(pair<T0,T1> const & x, pair<U0,U1> const & y) -> ...
 * operator>(pair<T0,T1> const & x, pair<U0,U1> const & y) -> ...
 * operator>=(pair<T0,T1> const & x, pair<U0,U1> const & y) -> ...
 * compare(pair<T0,T1> const & x, pair<U0,U1> const & y) -> ...
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
#include <sake/core/math/compare_fwd.hpp>
#include <sake/core/math/zero.hpp>

namespace sake
{

namespace pair_private
{

template< class T, class U >
struct enable_binary
    : boost::enable_if_c<
          sake::is_pair< typename boost_ext::remove_qualifiers<T>::type >::value
       && sake::is_pair< typename boost_ext::remove_qualifiers<U>::type >::value
      >
{ };

} // namespace pair_private

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

#if 0
template< class T, class U >
struct equal< T, U, typename sake::pair_private::enable_binary<T,U>::type >
    : operators::result_of::equal<
          typename boost_ext::remove_qualifiers<T>::type,
          typename boost_ext::remove_qualifiers<U>::type
      >
{ };

template< class T, class U >
struct not_equal< T, U, typename sake::pair_private::enable_binary<T,U>::type >
    : operators::result_of::not_equal<
          typename boost_ext::remove_qualifiers<T>::type,
          typename boost_ext::remove_qualifiers<U>::type
      >
{ };

template< class T, class U >
struct less< T, U, typename sake::pair_private::enable_binary<T,U>::type >
    : operators::result_of::less<
          typename boost_ext::remove_qualifiers<T>::type,
          typename boost_ext::remove_qualifiers<U>::type
      >
{ };

template< class T, class U >
struct less_equal< T, U, typename sake::pair_private::enable_binary<T,U>::type >
    : operators::result_of::less_equal<
          typename boost_ext::remove_qualifiers<T>::type,
          typename boost_ext::remove_qualifiers<U>::type
      >
{ };
#endif

} // namespace extension

} // namespace result_of

} // namespace operators

namespace result_of
{

namespace extension
{

template< class T0, class T1, class U0, class U1 >
struct compare< sake::pair<T0,T1>, sake::pair<U0,U1>, void >
    : boost_ext::common_type<
          typename result_of::compare<
              typename boost_ext::add_reference_add_const< T0 >::type,
              typename boost_ext::add_reference_add_const< U0 >::type
          >::type,
          typename result_of::compare<
              typename boost_ext::add_reference_add_const< T1 >::type,
              typename boost_ext::add_reference_add_const< U1 >::type
          >::type
      >
{ };

template< class T, class U >
struct compare< T, U, typename sake::pair_private::enable_binary<T,U>::type >
    : result_of::compare<
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
inline typename result_of::compare<
    sake::pair<T0,T1>,
    sake::pair<U0,U1>
>::type
compare(sake::pair< T0, T1 > const & x, sake::pair< U0, U1 > const & y)
{
    typename result_of::compare<
        typename boost_ext::add_reference_add_const< T0 >::type,
        typename boost_ext::add_reference_add_const< U0 >::type
    >::type const s0 = sake::compare(x.first, y.first);
    return s0 == sake::zero ? sake::compare(x.second, y.second) : s0;
}

} // namespace pair_adl

} // namespace sake

#endif // #ifndef SAKE_CORE_DATA_STRUCTURES_PAIR_OPERATORS_HPP

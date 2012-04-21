/*******************************************************************************
 * sake/core/data_structures/optional/operators.hpp
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
 * operator==(optional<T> const & x, optional<U> const & y) -> ...
 * operator!=(optional<T> const & x, optional<U> const & y) -> ...
 * operator<(optional<T> const & x, optional<U> const & y) -> ...
 * operator>(optional<T> const & x, optional<U> const & y) -> ...
 * operator<=(optional<T> const & x, optional<U> const & y) -> ...
 * operator>=(optional<T> const & x, optional<U> const & y) -> ...
 * cmp(optional<T> const & x, optional<U> const & y) -> ...
 ******************************************************************************/

#ifndef SAKE_CORE_DATA_STRUCTURES_OPTIONAL_OPERATORS_HPP
#define SAKE_CORE_DATA_STRUCTURES_OPTIONAL_OPERATORS_HPP

#include <sake/boost_ext/type_traits/common_type.hpp>
#include <sake/boost_ext/type_traits/add_reference_add_const.hpp>
#include <sake/boost_ext/type_traits/add_pointer_remove_reference_add_const.hpp>

#include <sake/core/data_structures/optional/fwd.hpp>
#include <sake/core/functional/operators/equal.hpp>
#include <sake/core/functional/operators/greater.hpp>
#include <sake/core/functional/operators/greater_equal.hpp>
#include <sake/core/functional/operators/less.hpp>
#include <sake/core/functional/operators/less_equal.hpp>
#include <sake/core/functional/operators/not_equal.hpp>
#include <sake/core/math/cmp_fwd.hpp>
#include <sake/core/math/sign_t.hpp>

namespace sake
{

namespace operators {
namespace result_of {
namespace extension {

template< class T, class U >
struct equal0< sake::optional<T>, sake::optional<U>, void >
    : boost_ext::common_type<
          typename sake::operators::result_of::equal<
              typename boost_ext::add_reference_add_const<T>::type,
              typename boost_ext::add_reference_add_const<U>::type
          >::type,
          bool
      >
{ };

template< class T, class U >
struct not_equal0< sake::optional<T>, sake::optional<U>, void >
    : boost_ext::common_type<
          typename sake::operators::result_of::not_equal<
              typename boost_ext::add_reference_add_const<T>::type,
              typename boost_ext::add_reference_add_const<U>::type
          >::type,
          bool
      >
{ };

template< class T, class U >
struct less0< sake::optional<T>, sake::optional<U>, void >
    : boost_ext::common_type<
          typename sake::operators::result_of::less<
              typename boost_ext::add_reference_add_const<T>::type,
              typename boost_ext::add_reference_add_const<U>::type
          >::type,
          bool
      >
{ };

template< class T, class U >
struct greater0< sake::optional<T>, sake::optional<U>, void >
    : boost_ext::common_type<
          typename sake::operators::result_of::greater<
              typename boost_ext::add_reference_add_const<T>::type,
              typename boost_ext::add_reference_add_const<U>::type
          >::type,
          bool
      >
{ };

template< class T, class U >
struct less_equal0< sake::optional<T>, sake::optional<U>, void >
    : boost_ext::common_type<
          typename sake::operators::result_of::less_equal<
              typename boost_ext::add_reference_add_const<T>::type,
              typename boost_ext::add_reference_add_const<U>::type
          >::type,
          bool
      >
{ };

template< class T, class U >
struct greater_equal0< sake::optional<T>, sake::optional<U>, void >
    : boost_ext::common_type<
          typename sake::operators::result_of::greater_equal<
              typename boost_ext::add_reference_add_const<T>::type,
              typename boost_ext::add_reference_add_const<U>::type
          >::type,
          bool
      >
{ };

} // namespace extension
} // namespace result_of
} // namespace operators

namespace result_of {
namespace extension {

template< class T, class U >
struct cmp0< sake::optional<T>, sake::optional<U>, void >
    : boost_ext::common_type<
          typename sake::result_of::cmp<
              typename boost_ext::add_reference_add_const<T>::type,
              typename boost_ext::add_reference_add_const<U>::type
          >::type,
          sake::sign_t
      >
{ };

} // namespace extension
} // namespace result_of

namespace pair_adl
{

template< class T, class U >
inline typename sake::operators::result_of::equal<
    sake::optional<T>, sake::optional<U> >::type
operator==(sake::optional<T> const & x, sake::optional<U> const & y)
{
    typename boost_ext::add_pointer_remove_reference_add_const<T>::type const p = x.get_ptr();
    typename boost_ext::add_pointer_remove_reference_add_const<U>::type const q = y.get_ptr();
    return p == q ? true : p && q ? *p == *q : false;
}

template< class T, class U >
inline typename sake::operators::result_of::not_equal<
    sake::optional<T>, sake::optional<U> >::type
operator!=(sake::optional<T> const & x, sake::optional<U> const & y)
{
    typename boost_ext::add_pointer_remove_reference_add_const<T>::type const p = x.get_ptr();
    typename boost_ext::add_pointer_remove_reference_add_const<U>::type const q = y.get_ptr();
    return p != q ? p && q ? *p != *q : true : false;
}

template< class T, class U >
inline typename sake::operators::result_of::less<
    sake::optional<T>, sake::optional<U> >::type
operator<(sake::optional<T> const & x, sake::optional<U> const & y)
{
    typename boost_ext::add_pointer_remove_reference_add_const<T>::type const p = x.get_ptr();
    typename boost_ext::add_pointer_remove_reference_add_const<U>::type const q = y.get_ptr();
    return q ? p ? *p < *q : true : false;
}

template< class T, class U >
inline typename sake::operators::result_of::less<
    sake::optional<T>, sake::optional<U> >::type
operator>(sake::optional<T> const & x, sake::optional<U> const & y)
{
    typename boost_ext::add_pointer_remove_reference_add_const<T>::type const p = x.get_ptr();
    typename boost_ext::add_pointer_remove_reference_add_const<U>::type const q = y.get_ptr();
    return p ? q ? *p > *q : true : false;
}

template< class T, class U >
inline typename sake::operators::result_of::less<
    sake::optional<T>, sake::optional<U> >::type
operator<=(sake::optional<T> const & x, sake::optional<U> const & y)
{
    typename boost_ext::add_pointer_remove_reference_add_const<T>::type const p = x.get_ptr();
    typename boost_ext::add_pointer_remove_reference_add_const<U>::type const q = y.get_ptr();
    return p ? q ? *p <= *q : false : true;
}

template< class T, class U >
inline typename sake::operators::result_of::less<
    sake::optional<T>, sake::optional<U> >::type
operator>=(sake::optional<T> const & x, sake::optional<U> const & y)
{
    typename boost_ext::add_pointer_remove_reference_add_const<T>::type const p = x.get_ptr();
    typename boost_ext::add_pointer_remove_reference_add_const<U>::type const q = y.get_ptr();
    return q ? p ? *p >= *q : false : true;
}

template< class T, class U >
inline typename sake::result_of::cmp<
    sake::optional<T>, sake::optional<U> >::type
cmp(sake::pair< T0, T1 > const & x, sake::pair< U0, U1 > const & y)
{
    typename boost_ext::add_pointer_remove_reference_add_const<T>::type const p = x.get_ptr();
    typename boost_ext::add_pointer_remove_reference_add_const<U>::type const q = y.get_ptr();
    return p ?
           q ? sake::cmp(*p,*q)
             : sake::sign_t(sake::positive_sign_t) :
           q ? sake::sign_t(sake::negative_sign_t)
             : sake::sign_t(sake::zero_sign_t);
}

} // namespace pair_adl

} // namespace sake

#endif // #ifndef SAKE_CORE_DATA_STRUCTURES_OPTIONAL_OPERATORS_HPP

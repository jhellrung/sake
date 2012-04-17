/*******************************************************************************
 * sake/core/utility/ref/fwd.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_REF_FWD_HPP
#define SAKE_CORE_UTILITY_REF_FWD_HPP

#include <boost/type_traits/integral_constant.hpp>
#include <boost/utility/enable_if.hpp>

//#include <sake/core/range/cursor_fwd.hpp>
//#include <sake/core/range/is_view_fwd.hpp>
//#include <sake/core/range/static_size_fwd.hpp>
#include <sake/core/utility/is_convertible_wnrbt_fwd.hpp>
#include <sake/core/utility/ref/tag.hpp>

namespace sake
{

namespace reference_wrapper_adl
{

template< class T, class Tags = sake::ref_tag::default_tags >
class reference_wrapper;

} // namespace reference_wrapper_adl

using reference_wrapper_adl::reference_wrapper;

template< class T >
struct is_reference_wrapper;

template< class T >
struct unwrap_reference;

template< class T >
struct is_wrapped_parameter;

namespace extension
{
template< class T, class Enable = void >
struct unwrap_reference;
template< class T, class Enable = void >
struct is_wrapped_parameter;
} // namespace extension

namespace default_impl
{
template< class T >
struct unwrap_reference;
template< class T >
struct is_wrapped_parameter;
} // namespace default_impl

#if 0
/*******************************************************************************
 * struct extension::range_mutable_cursor< T, Introversal, ... >
 * struct extension::range_const_cursor< T, Introversal, ... >
 * struct extension::range_is_view< T, ... >
 * struct extension::range_static_size< T, ... >
 ******************************************************************************/

namespace extension
{

template< class T, class Introversal >
struct range_mutable_cursor<
    T, Introversal,
    typename boost::enable_if_c< sake::is_reference_wrapper<T>::value >::type
>
    : sake::range_cursor< typename sake::unwrap_reference<T>::type, Introversal >
{ };

template< class T, class Introversal >
struct range_const_cursor<
    T, Introversal,
    typename boost::enable_if_c< sake::is_reference_wrapper<T>::value >::type
>
    : sake::range_cursor< typename sake::unwrap_reference<T>::type, Introversal >
{ };

template< class T >
struct range_is_view<
    T,
    typename boost::enable_if_c< sake::is_reference_wrapper<T>::value >::type
>
    : boost::true_type
{ };

template< class T >
struct range_static_size<
    T,
    typename boost::enable_if_c< sake::is_reference_wrapper<T>::value >::type
>
    : sake::range_static_size< typename sake::unwrap_reference<T>::type >
{ };

} // namespace extension
#endif // #if 0

/*******************************************************************************
 * struct extension::is_convertible_wnrbt< T, U, ... >
 ******************************************************************************/

namespace extension
{

template< class T, class U >
struct is_convertible_wnrbt<
    T, U,
    typename boost::enable_if_c< sake::is_reference_wrapper<T>::value >::type
>
    : sake::is_convertible_wnrbt<
          typename sake::unwrap_reference<T>::type &,
          U
      >
{ };

} // namespace extension

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_REF_FWD_HPP

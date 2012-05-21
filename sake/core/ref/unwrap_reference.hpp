/*******************************************************************************
 * sake/core/ref/unwrap_reference.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct unwrap_reference<T>
 * struct extension::unwrap_reference< T, Enable = void >
 * struct default_impl::unwrap_reference<T>
 *
 * This is a metafunction evaluating to the underlying referent if T is some
 * kind of reference wrapper, and to T otherwise.  A sake::reference_wrapper and
 * boost::reference_wrapper are examples of reference wrappers.
 ******************************************************************************/

#ifndef SAKE_CORE_REF_UNWRAP_REFERENCE_HPP
#define SAKE_CORE_REF_UNWRAP_REFERENCE_HPP

#include <sake/core/ref/fwd.hpp>

namespace sake
{

/*******************************************************************************
 * struct unwrap_reference<T>
 ******************************************************************************/

template< class T >
struct unwrap_reference
    : extension::unwrap_reference<T>
{ };

namespace unwrap_reference_private
{

template<
    class Q, class T,
    class U = typename sake::unwrap_reference<T>::type
>
struct helper
{ typedef U type; };

template< class Q, class T >
struct helper< Q, T, T >
{ typedef Q type; };

} // namespace unwrap_reference_private

template< class T >
struct unwrap_reference< T const >
    : unwrap_reference_private::helper< T const, T >
{ };

template< class T >
struct unwrap_reference< T volatile >
    : unwrap_reference_private::helper< T volatile, T >
{ };

template< class T >
struct unwrap_reference< T const volatile >
    : unwrap_reference_private::helper< T const volatile, T >
{ };

/*******************************************************************************
 * struct extension::unwrap_reference< T, Enable = void >
 ******************************************************************************/

namespace extension
{

template< class T, class Enable /*= void*/ >
struct unwrap_reference
    : default_impl::unwrap_reference<T>
{ };

} // namespace extension

/*******************************************************************************
 * struct default_impl::unwrap_reference<T>
 ******************************************************************************/

namespace default_impl
{

template< class T >
struct unwrap_reference
{ typedef T type; };

} // namespace default_impl

} // namespace sake

#endif // #ifndef SAKE_CORE_REF_UNWRAP_REFERENCE_HPP

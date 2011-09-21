/*******************************************************************************
 * sake/core/ref/unwrap_reference.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct unwrap_reference<T>
 *
 * This is a metafunction evaluating to the underlying referent if T is some
 * kind of reference wrapper, and to T otherwise.  A sake::reference_wrapper and
 * boost::reference_wrapper are examples of reference wrappers.
 ******************************************************************************/

#ifndef SAKE_CORE_REF_UNWRAP_REFERENCE_HPP
#define SAKE_CORE_REF_UNWRAP_REFERENCE_HPP

#include <sake/core/ref/ref_fwd.hpp>
#include <sake/core/utility/extension.hpp>

namespace sake
{

namespace no_ext
{
template< class T > struct unwrap_reference { typedef T type; };
} // namespace no_ext

SAKE_EXTENSION_UNARY_CLASS( unwrap_reference )

namespace detail_unwrap_reference
{

template< class QualifiedT, class T, class Unwrapped = typename unwrap_reference<T>::type >
struct unwrap_reference_helper { typedef Unwrapped type; };
template< class QualifiedT, class T >
struct unwrap_reference_helper< QualifiedT, T, T > { typedef QualifiedT type; };

} // namespace detail_unwrap_reference

template< class T >
struct unwrap_reference : ext::unwrap_reference<T> { };

template< class T >
struct unwrap_reference< const T >
    : detail_unwrap_reference::unwrap_reference_helper< const T, T >
{ };

template< class T >
struct unwrap_reference< volatile T >
    : detail_unwrap_reference::unwrap_reference_helper< volatile T, T >
{ };

template< class T >
struct unwrap_reference< const volatile T >
    : detail_unwrap_reference::unwrap_reference_helper< const volatile T, T >
{ };

} // namespace sake

#endif // #ifndef SAKE_CORE_REF_UNWRAP_REFERENCE_HPP

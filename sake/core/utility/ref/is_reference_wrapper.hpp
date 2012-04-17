/*******************************************************************************
 * sake/core/utility/ref/is_reference_wrapper.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct is_reference_wrapper<T>
 *
 * This is a metafunction which determines whether the given type is a reference
 * wrapper.  It is assumed that T is a reference wrapper iff T differs from
 * unwrap_reference<T>::type.
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_REF_IS_REFERENCE_WRAPPER_HPP
#define SAKE_CORE_UTILITY_REF_IS_REFERENCE_WRAPPER_HPP

#include <boost/type_traits/integral_constant.hpp>

#include <sake/core/utility/ref/fwd.hpp>
#include <sake/core/utility/ref/unwrap_reference.hpp>

namespace sake
{

template< class T >
struct is_reference_wrapper;

namespace is_reference_wrapper_private
{

template<
    class T,
    class Unwrapped = typename sake::unwrap_reference<T>::type
>
struct dispatch
    : boost::true_type
{ };

template< class T >
struct dispatch<T,T>
    : boost::false_type
{ };

} // namespace is_reference_wrapper_private

template< class T >
struct is_reference_wrapper
    : is_reference_wrapper_private::dispatch<T>
{ };

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_REF_IS_REFERENCE_WRAPPER_HPP

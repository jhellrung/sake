/*******************************************************************************
 * sake/core/ref/wrapped_parameter_to_reference.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct wrapped_parameter_to_reference<T>
 ******************************************************************************/

#ifndef SAKE_CORE_REF_WRAPPED_PARAMETER_TO_REFERENCE_HPP
#define SAKE_CORE_REF_WRAPPED_PARAMETER_TO_REFERENCE_HPP

#include <sake/core/ref/is_wrapped_parameter.hpp>
#include <sake/core/ref/wrapper_to_reference.hpp>

namespace sake
{

namespace wrapped_parameter_to_reference_private
{

template< class T, bool = sake::is_wrapped_parameter<T>::value >
struct dispatch;

template< class T >
struct dispatch< T, false >
{ typedef T type; };

template< class T >
struct dispatch< T, true >
    : sake::wrapper_to_reference<T>
{ };

} // namespace wrapped_parameter_to_reference_private

template< class T >
struct wrapped_parameter_to_reference
    : wrapped_parameter_to_reference_private::dispatch<T>
{ };

} // namespace sake

#endif // #ifndef SAKE_CORE_REF_WRAPPED_PARAMETER_TO_REFERENCE_HPP

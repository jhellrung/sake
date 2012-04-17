/*******************************************************************************
 * sake/core/utility/ref/wrapper_to_reference.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct wrapper_to_reference<T>
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_REF_WRAPPER_TO_REFERENCE_HPP
#define SAKE_CORE_UTILITY_REF_WRAPPER_TO_REFERENCE_HPP

#include <sake/core/utility/ref/unwrap_reference.hpp>

namespace sake
{

namespace wrapper_to_reference_private
{

template< class T, class T_ = typename sake::unwrap_reference<T>::type >
struct dispatch
{ typedef T_& type; };

template< class T >
struct dispatch<T,T>
{ typedef T type; };

} // namespace wrapper_to_reference_private

template< class T >
struct wrapper_to_reference
    : wrapper_to_reference_private::dispatch<T> { };

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_REF_WRAPPER_TO_REFERENCE_HPP

/*******************************************************************************
 * core/utility/rebind_pointer.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct rebind_pointer<P,T>
 * struct ext::rebind_pointer< P, T, Enable = void >
 * struct no_ext::rebind_pointer<P,T>
 *
 * This rebinds a pointer or smart pointer to a different pointee type.  Smart
 * pointers need to either be a template instantiation of the form smart_ptr<T>
 * or smart_ptr<T,A>; or have a nested class template rebind; or explicitly
 * specialize ext::rebind_pointer.
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_REBIND_POINTER_HPP
#define SAKE_CORE_UTILITY_REBIND_POINTER_HPP

#include <sake/core/introspection/has_template_rebind.hpp>
#include <sake/core/utility/extension.hpp>

namespace sake
{

template< class P, class T >
struct rebind_pointer;

namespace no_ext
{

namespace rebind_pointer_private
{

template< class P, class T, bool = sake::has_template_rebind<P>::value >
struct dispatch;

template< class P, class T >
struct dispatch< P, T, true >
{ typedef typename P::template rebind<T>::type type; };

template< template< class U > class P, class U, class T >
struct dispatch< P<U>, T, false >
{ typedef P<T> type; };

template< template< class U, class A > class P, class U, class A, class T >
struct dispatch< P<U,A>, T, false >
{ typedef P<T,A> type; };

} // namespace rebind_pointer_private

template< class P, class T >
struct rebind_pointer
    : rebind_pointer_private::dispatch<P,T>
{ };

template< class U, class T >
struct rebind_pointer< U*, T >
{ typedef T* type; };

} // namespace no_ext

SAKE_EXTENSION_CLASS( rebind_pointer, 2 )

template< class P, class T >
struct rebind_pointer
    : ext::rebind_pointer<P,T>
{ };

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_REBIND_POINTER_HPP

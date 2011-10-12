/*******************************************************************************
 * sake/core/utility/rebind_pointer.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct rebind_pointer<P,T>
 * struct extension::rebind_pointer< P, T, Enable = void >
 * struct default_impl::rebind_pointer<P,T>
 *
 * This rebinds a pointer or smart pointer to a different pointee type.  Smart
 * pointers need to either be a template instantiation of the form smart_ptr<T>
 * or smart_ptr<T,A>; or have a nested class template rebind; or explicitly
 * specialize extension::rebind_pointer.
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_REBIND_POINTER_HPP
#define SAKE_CORE_UTILITY_REBIND_POINTER_HPP

#include <sake/core/introspection/has_template_rebind.hpp>
#include <sake/core/utility/rebind_pointer_fwd.hpp>

namespace sake
{

/*******************************************************************************
 * struct rebind_pointer<P,T>
 ******************************************************************************/

template< class P, class T >
struct rebind_pointer
    : extension::rebind_pointer<P,T>
{ };

template< class U, class T >
struct rebind_pointer< U*, T >
{ typedef T* type; };

/*******************************************************************************
 * struct extension::rebind_pointer< P, T, Enable = void >
 ******************************************************************************/

namespace extension
{

template< class P, class T, class Enable /*= void*/ >
struct rebind_pointer
    : default_impl::rebind_pointer<P,T>
{ };

} // namespace extension

/*******************************************************************************
 * struct default_impl::rebind_pointer<P,T>
 ******************************************************************************/

namespace default_impl
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

} // namespace default_impl

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_REBIND_POINTER_HPP

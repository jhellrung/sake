/*******************************************************************************
 * sake/core/memory/alloc/default_impl/value_traits.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct alloc::default_impl::value_traits<A>
 ******************************************************************************/

#ifndef SAKE_CORE_MEMORY_ALLOC_DEFAULT_IMPL_VALUE_TRAITS_HPP
#define SAKE_CORE_MEMORY_ALLOC_DEFAULT_IMPL_VALUE_TRAITS_HPP

#include <boost/type_traits/is_void.hpp>

#include <sake/core/introspection/has_type_const_pointer.hpp>
#include <sake/core/introspection/has_type_pointer.hpp>
#include <sake/core/memory/alloc/has_template_value_traits.hpp>
#include <sake/core/memory/alloc/rebind.hpp>
#include <sake/core/utility/rebind_pointer.hpp>

namespace sake
{

namespace alloc
{

namespace default_impl
{

namespace value_traits_private
{

template<
    class A,
    bool = alloc::has_template_value_traits<A>::value
>
struct dispatch;

} // namespace value_traits_private

template< class A, class T >
struct value_traits
    : value_traits_private::dispatch<A>::template apply<T>
{ };

namespace value_traits_private
{

template< class A >
struct dispatch< A, true >
{
    template< class T >
    struct apply
        : A::template value_traits<T>
    { };
};

template< class A, class T, bool = sake::has_type_pointer<A>::value >
struct pointer_dispatch;

template< class A, class T >
struct pointer_dispatch< A, T, true >
{ typedef typename A::pointer type; };

template< class A, class T >
struct pointer_dispatch< A, T, false >
{ typedef T* type; };

template< class A, class T, bool = sake::has_type_const_pointer<A>::value >
struct const_pointer_dispatch;

template< class A, class T >
struct const_pointer_dispatch< A, T, true >
{ typedef typename A::const_pointer type; };

template< class A, class T >
struct const_pointer_dispatch< A, T, false >
    : sake::rebind_pointer<
          typename traits_private::pointer_dispatch<A,T>::type,
          T const
      >
{ };

template< class A >
struct dispatch< A, false >
{
    template< class T >
    struct apply
    {
        typedef T value_type;
        typedef typename pointer_dispatch<
            typename alloc::rebind<A,T>::type, T
        >::type pointer;
        typedef typename const_pointer_dispatch<
            typename alloc::rebind<A,T>::type, T
        >::type const_pointer;
    };
};

} // namespace value_traits_private

} // namespace default_impl

} // namespace alloc

} // namespace sake

#endif // #ifndef SAKE_CORE_MEMORY_ALLOC_DEFAULT_IMPL_VALUE_TRAITS_HPP

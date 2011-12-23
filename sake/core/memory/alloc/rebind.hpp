/*******************************************************************************
 * sake/core/memory/alloc/rebind.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef BOOST_PP_IS_ITERATING

#ifndef SAKE_CORE_MEMORY_ALLOC_REBIND_HPP
#define SAKE_CORE_MEMORY_ALLOC_REBIND_HPP

#include <boost/config.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>

#include <sake/core/introspection/has_template_rebind.hpp>
#include <sake/core/memory/alloc/rebind.hpp>

#ifndef SAKE_ALLOC_REBIND_MAX_AUX_TEMPLATE_PARAMS
#define SAKE_ALLOC_REBIND_MAX_AUX_TEMPLATE_PARAMS 8
#endif // #ifndef SAKE_ALLOC_REBIND_MAX_AUX_TEMPLATE_PARAMS

namespace sake
{

namespace alloc
{

template< class A, class T >
struct rebind
    : extension::rebind<A,T>
{ };

namespace extension
{

template< class A, class T, class Enable /*= void*/ >
struct rebind
    : default_impl<A,T>
{ };

} // namespace extension

namespace default_impl
{

namespace rebind_private
{

template<
    class A, class T,
    bool = sake::has_template_rebind<A>::value
>
struct dispatch;

} // namespace rebind_private

template< class A, class T >
struct rebind
    : rebind_private::dispatch<A,T>
{ };

namespace rebind_private
{

template< class A, class T >
struct dispatch< A, T, true >
{ typedef typename A::template rebind<T>::other type; };

template< class A, class T >
struct dispatch< A, T, false >
{ typedef void type; };

#ifndef BOOST_NO_VARIADIC_TEMPLATES

template< template< class U, class... _ > class A, class U, class... _, class T >
struct dispatch< A< U, _... >, T, false >
{ typedef A< T, _... > type; };

#else // #ifndef BOOST_NO_VARIADIC_TEMPLATES

#define BOOST_PP_ITERATION_LIMITS ( 0, SAKE_ALLOC_REBIND_MAX_AUX_TEMPLATE_PARAMS )
#define BOOST_PP_FILENAME_1       <sake/core/memory/alloc/rebind.hpp>
#include BOOST_PP_ITERATE()

#endif // #ifndef BOOST_NO_VARIADIC_TEMPLATES

} // namespace rebind_private

} // namespace default_impl

} // namespace alloc

} // namespace sake

#endif // #ifndef SAKE_CORE_MEMORY_ALLOC_REBIND_HPP

#else // #ifndef BOOST_PP_IS_ITERATING

#define N BOOST_PP_ITERATION()

template<
    template< class U BOOST_PP_ENUM_TRAILING_PARAMS( N, class _ ) > class A,
    class U BOOST_PP_ENUM_TRAILING_PARAMS( N, class _ ),
    class T
>
struct dispatch< A< U BOOST_PP_ENUM_TRAILING_PARAMS( N, _ ) >, T, false >
{ typedef A< T BOOST_PP_ENUM_TRAILING_PARAMS( N, _ ) > type; };

#undef N

#endif // #ifndef BOOST_PP_IS_ITERATING

/*******************************************************************************
 * sake/core/introspection/has_member_function.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_INTROSPECTION_DEFINE_HAS_MEMBER_FUNCTION( trait, name )
 *
 * Expands to define a metafunction trait which determines if a given type has
 * a member function of the given name.
 *
 * Example:
 *
 * SAKE_INTROSPECTION_DEFINE_HAS_MEMBER_FUNCTION( has_mem_fun_xxx, xxx )
 *
 * expands to a metafunction definition equivalent to
 *
 * template< class T, class Signature >
 * struct has_mem_fun_xxx
 * {
 *     static bool const value = [true iff T has a member function named xxx
 *                                with signature Signature];
 *     typedef has_mem_fun_xxx type;
 * };
 ******************************************************************************/

#ifndef BOOST_PP_IS_ITERATING

#ifndef SAKE_CORE_INTROSPECTION_HAS_MEMBER_FUNCTION_HPP
#define SAKE_CORE_INTROSPECTION_HAS_MEMBER_FUNCTION_HPP

#include <boost/config.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>

#include <sake/core/introspection/fwd.hpp>
#include <sake/core/utility/yes_no_tag.hpp>

#define SAKE_INTROSPECTION_DEFINE_HAS_MEMBER_FUNCTION( trait, name ) \
template< class T, class Signature > \
class trait \
{ \
    template< class T_ > static ::sake::yes_tag test( \
        typename ::sake::has_member_function_private::sfinae< T, Signature >::template \
        _< &T_::name >* = 0); \
    template< class T_ > static ::sake::no_tag test(...); \
public: \
    static bool const value = sizeof( ::sake::yes_tag ) == sizeof( test<T>(0) ); \
    typedef trait type; \
};

namespace sake
{

namespace has_member_function_private
{

template< class T, class Signature >
struct sfinae;

template< class T, class R >
struct sfinae
{ template< R (T::*) > struct _; };

template< class T, class R >
struct sfinae< T const, R >
{ template< R (T::*) > struct _; };

template< class T, class R >
struct sfinae< T volatile, R >
{ template< R (T::*) > struct _; };

template< class T, class R >
struct sfinae< T const volatile, R >
{ template< R (T::*) > struct _; };

#ifndef BOOST_NO_VARIADIC_TEMPLATES

template< class T, class R, class... U >
struct sfinae< T, R ( U... ) >
{ template< R (T::*)( U... ) > struct _; };

template< class T, class R, class... U >
struct sfinae< T const, R ( U... ) >
{ template< R (T::*)( U... ) const > struct _; };

template< class T, class R, class... U >
struct sfinae< T volatile, R ( U... ) >
{ template< R (T::*)( U... ) volatile > struct _; };

template< class T, class R, class... U >
struct sfinae< T const volatile, R ( U... ) >
{ template< R (T::*)( U... ) const volatile > struct _; };

#else // #ifndef BOOST_NO_VARIADIC_TEMPLATES

#define BOOST_PP_ITERATION_LIMITS ( 0, SAKE_INTROSPECTION_DEFAULT_MAX_ARITY )
#define BOOST_PP_FILENAME_1       <sake/core/introspection/has_member_function.hpp>
#include BOOST_PP_ITERATE()

#endif // #ifndef BOOST_NO_VARIADIC_TEMPLATES

} // namespace has_member_function_private

} // namespace sake

#endif // #ifndef SAKE_CORE_INTROSPECTION_HAS_MEMBER_FUNCTION_HPP

#else // #ifndef BOOST_PP_IS_ITERATING

#define N BOOST_PP_ITERATION()

#define comma_class_T0N BOOST_PP_ENUM_TRAILING_PARAMS( N, class T )
#define T0N             BOOST_PP_ENUM_PARAMS( N, T )

template< class T, class R comma_class_T0N >
struct sfinae< T, R ( T0N ) >
{ template< R (T::*)( T0N ) > struct _; };

template< class T, class R comma_class_T0N >
struct sfinae< T const, R ( T0N ) >
{ template< R (T::*)( T0N ) const > struct _; };

template< class T, class R comma_class_T0N >
struct sfinae< T volatile, R ( T0N ) >
{ template< R (T::*)( T0N ) volatile > struct _; };

template< class T, class R comma_class_T0N >
struct sfinae< T const volatile, R ( T0N ) >
{ template< R (T::*)( T0N ) const volatile > struct _; };

#undef comma_class_T0N
#undef T0N

#undef N

#endif // #ifndef BOOST_PP_IS_ITERATING

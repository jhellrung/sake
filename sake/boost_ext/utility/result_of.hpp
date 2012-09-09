/*******************************************************************************
 * sake/boost_ext/utility/result_of.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::result_of< F ( T... ) >
 *
 * This should be nearly identical to boost::result_of, but it implements SFINAE
 * for invalid call signatures and also (for better or worse) does not simply
 * default to void for nullary call signatures. Relevant links:
 *
 * http://www.boost.org/libs/utility/utility.htm#result_of
 * http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2003/n1454.html
 * http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2011/n3276.pdf
 ******************************************************************************/

#ifndef BOOST_PP_IS_ITERATING

#ifndef SAKE_BOOST_EXT_UTILITY_RESULT_OF_HPP
#define SAKE_BOOST_EXT_UTILITY_RESULT_OF_HPP

#include <boost/config.hpp>
#include <boost/function_types/is_function.hpp>
#include <boost/function_types/is_function_pointer.hpp>
#include <boost/function_types/result_type.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/if.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/introspection/has_type_result_type.hpp>
#include <sake/core/introspection/is_callable.hpp>
#include <sake/core/utility/declval.hpp>
#include <sake/core/utility/int_tag.hpp>

#ifndef SAKE_RESULT_OF_MAX_ARITY
#define SAKE_RESULT_OF_MAX_ARITY 8
#endif // #ifndef SAKE_RESULT_OF_MAX_ARITY

namespace sake
{

namespace boost_ext
{

namespace result_of_private
{

template< class Signature, class = void >
struct sfinae
{ };

#ifdef BOOST_NO_DECLTYPE_N3276

template< class F_ >
struct dispatch_index
{
    static const int value = boost_ext::mpl::
         if_< boost::function_types::is_function< F_ >,
              sake::int_tag<2> >::type::template
    else_if < boost::function_types::is_function_pointer< F_ >,
              sake::int_tag<2> >::type::template
    else_if < sake::has_type_result_type< F_ >,
              sake::int_tag<1> >::type::template
    else_   < sake::int_tag<0> >::type::value;
};

template< class F_, int = dispatch_index< F_ >::value >
struct dispatch;

template< class F_ >
struct dispatch< F_, 2 >
{
    template< class Signature >
    struct apply
    { typedef typename boost::function_types::result_type< F_ >::type type; };
};

template< class F_ >
struct dispatch< F_, 1 >
{
    template< class Signature >
    struct apply
    { typedef typename F_::result_type type; };
};

template< class F_ >
struct dispatch< F_, 0 >
{
    template< class Signature >
    struct apply
        : F_::template result< Signature >
    { };
};

#endif // #ifdef BOOST_NO_DECLTYPE_N3276

#ifndef BOOST_NO_VARIADIC_TEMPLATES

#ifndef BOOST_NO_DECLTYPE_N3276

template< class F, class... T >
struct sfinae<
    F ( T... ),
    typename result_of_private::always_void<
        decltype( sake::declval<F>() ( sake::declval<T>()... ) ) >::type
>
{ typedef decltype( sake::declval<F>() ( sake::declval<T>()... ) ) type; };

#else // #ifndef BOOST_NO_DECLTYPE_N3276

template< class F, class... T >
struct sfinae<
    F ( T... ),
    typename boost::enable_if_c<
        sake::is_callable< F, void ( T... ) >::value >::type
>
    : result_of_private::dispatch<
          typename boost_ext::remove_qualifiers<F>::type >::template
      apply< F ( T... ) >
{ };

#endif // #ifndef BOOST_NO_DECLTYPE_N3276

#else // #ifndef BOOST_NO_VARIADIC_TEMPLATES

#define BOOST_PP_ITERATION_LIMITS ( 0, SAKE_RESULT_OF_MAX_ARITY )
#define BOOST_PP_FILENAME_1       <sake/boost_ext/utility/result_of.hpp>
#include BOOST_PP_ITERATE()

#endif // #ifndef BOOST_NO_VARIADIC_TEMPLATES

} // namespace result_of_private

template< class Signature >
struct result_of
    : result_of_private::sfinae< Signature >
{ };

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_UTILITY_RESULT_OF_HPP

#else // #ifndef BOOST_PP_IS_ITERATING

#define N BOOST_PP_ITERATION()

#define comma_class_T0N BOOST_PP_ENUM_TRAILING_PARAMS( N, class T )
#define T0N BOOST_PP_ENUM_PARAMS( N, T )
#define declval_T0N \
    BOOST_PP_ENUM_BINARY_PARAMS( N, sake::declval< T, >() BOOST_PP_INTERCEPT )

#ifndef BOOST_NO_DECLTYPE_N3276

template< class F comma_class_T0N >
struct sfinae<
    F ( T0N ),
    typename result_of_private::always_void<
        decltype( sake::declval<F>() ( declval_T0N ) ) >::type
>
{ typedef decltype( sake::declval<F>() ( declval_T0N ) ) type; };

#else // #ifndef BOOST_NO_DECLTYPE_N3276

template< class F comma_class_T0N >
struct sfinae<
    F ( T0N ),
    typename boost::enable_if_c<
        sake::is_callable< F, void ( T0N ) >::value >::type
>
    : result_of_private::dispatch<
          typename boost_ext::remove_qualifiers<F>::type >::template
      apply< F ( T0N ) >
{ };

#endif // #ifndef BOOST_NO_DECLTYPE_N3276

#undef class_T0N
#undef T0N
#undef declval_T0N

#undef N

#endif // #ifndef BOOST_PP_IS_ITERATING

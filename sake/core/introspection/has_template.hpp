/*******************************************************************************
 * sake/core/introspection/has_template.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_INTROSPECTION_DEFINE_HAS_TEMPLATE[_Z]( [z,] trait, name )
 * #define SAKE_INTROSPECTION_DEFINE_HAS_TEMPLATE_PARAMS[_Z]( [z,] trait, name, params )
 *
 * Expands to define a metafunction trait which determines if a given type has
 * a nested class template of the given name.
 *
 * Example:
 *
 * SAKE_INTROSPECTION_DEFINE_HAS_TEMPLATE( has_template_xxx, xxx )
 *
 * expands to a metafunction definition equivalent to
 *
 * template< class T >
 * struct has_template_xxx
 * {
 *     static bool const value = [true iff T has a nested class template named
 *                                xxx taking only type template parameters];
 *     typedef has_type_xxx type;
 * };
 *
 * Example:
 *
 * SAKE_INTROSPECTION_DEFINE_HAS_TEMPLATE_PARAMS( has_template_xxx, xxx, 2 )
 *
 * expands to a metafunction definition equivalent to
 *
 * template< class T >
 * struct has_template_xxx
 * {
 *     static bool const value = [true iff T has a nested class template named
 *                                xxx taking 2 type template parameters];
 *     typedef has_type_xxx type;
 * };
 *
 * Example:
 *
 * SAKE_INTROSPECTION_DEFINE_HAS_TEMPLATE_PARAMS(
 *     has_template_xxx,
 *     xxx,
 *     ( class U0 ) ( int U1 )
 * )
 *
 * expands to a metafunction definition equivalent to
 *
 * template< class T >
 * struct has_template_xxx
 * {
 *     static bool const value = [true iff T has a nested class template named
 *                                xxx taking 2 template parameters, the first
 *                                being a type parameter and the second being an
 *                                integral parameter];
 *     typedef has_type_xxx type;
 * };
 *
 * TODO: Can this be simplified via variadic templates?
 ******************************************************************************/

#ifndef BOOST_PP_IS_ITERATING

#ifndef SAKE_CORE_INTROSPECTION_HAS_TEMPLATE_HPP
#define SAKE_CORE_INTROSPECTION_HAS_TEMPLATE_HPP

#include <boost/preprocessor/arithmetic/dec.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/detail/is_unary.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/deduce_z.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/seq/enum.hpp>

#include <sake/core/utility/true_false_tag.hpp>

#ifndef SAKE_INTROSPECTION_HAS_TEMPLATE_MAX_ARITY
#define SAKE_INTROSPECTION_HAS_TEMPLATE_MAX_ARITY 8
#endif // #ifndef SAKE_INTROSPECTION_HAS_TEMPLATE_MAX_ARITY

#define SAKE_INTROSPECTION_DEFINE_HAS_TEMPLATE( trait, name ) \
    SAKE_INTROSPECTION_DEFINE_HAS_TEMPLATE_Z( BOOST_PP_DEDUCE_Z(), trait, name )

#define SAKE_INTROSPECTION_DEFINE_HAS_TEMPLATE_Z( z, trait, name ) \
template< class T > \
class trait \
{ \
    BOOST_PP_CAT( BOOST_PP_REPEAT_, z ) ( \
        SAKE_INTROSPECTION_HAS_TEMPLATE_MAX_ARITY, \
        SAKE_INTROSPECTION_HAS_TEMPLATE_test_sfinae_n, \
        name \
    ) \
    template< class T_ > static ::sake::false_tag test(...); \
public: \
    static bool const value = SAKE_SIZEOF_TRUE_TAG == sizeof( test<T>(0) ); \
    typedef trait type; \
};

#define SAKE_INTROSPECTION_DEFINE_HAS_TEMPLATE_PARAMS( trait, name, params ) \
    SAKE_INTROSPECTION_DEFINE_HAS_TEMPLATE_PARAMS_Z( BOOST_PP_DEDUCE_Z(), trait, name, params )

#define SAKE_INTROSPECTION_DEFINE_HAS_TEMPLATE_PARAMS_Z( z, trait, name, params ) \
template< class T > \
class trait \
{ \
    template< template< SAKE_INTROSPECTION_HAS_TEMPLATE_params( params ) > class U > struct sfinae; \
    template< class T_ > static ::sake::true_tag  test(sfinae< T_::template name >*); \
    template< class T_ > static ::sake::false_tag test(...); \
public: \
    static bool const value = SAKE_SIZEOF_TRUE_TAG == sizeof( test<T>(0) ); \
    typedef trait type; \
};

#define SAKE_INTROSPECTION_HAS_TEMPLATE_test_sfinae_n( z, n, data ) \
    template< class T_ > static ::sake::true_tag \
    test(::sake::has_template_private::sfinae ## n < T_::template data >*);

#define SAKE_INTROSPECTION_HAS_TEMPLATE_params( params ) \
    BOOST_PP_CAT( SAKE_INTROSPECTION_HAS_TEMPLATE_params_, BOOST_PP_IS_UNARY( params ) ) ( params )
#define SAKE_INTROSPECTION_HAS_TEMPLATE_params_0( params ) \
    BOOST_PP_ENUM_PARAMS( params, class U BOOST_PP_INTERCEPT )
#define SAKE_INTROSPECTION_HAS_TEMPLATE_params_1( params ) \
    BOOST_PP_SEQ_ENUM( params )

namespace sake
{

namespace has_template_private
{

#define BOOST_PP_FILENAME_1       <sake/core/introspection/has_template.hpp>
#define BOOST_PP_ITERATION_LIMITS ( 0, SAKE_INTROSPECTION_HAS_TEMPLATE_MAX_ARITY - 1 )
#include BOOST_PP_ITERATE()

} // namespace has_template_private

} // namespace sake

#endif // #ifndef SAKE_CORE_INTROSPECTION_HAS_TEMPLATE_HPP

#else // #ifndef BOOST_PP_IS_ITERATING

#define N BOOST_PP_ITERATION()

template< template< BOOST_PP_ENUM_PARAMS( BOOST_PP_INC( N ), class T ) > class U >
struct BOOST_PP_CAT( sfinae, N ) ;

#endif // #ifndef BOOST_PP_IS_ITERATING

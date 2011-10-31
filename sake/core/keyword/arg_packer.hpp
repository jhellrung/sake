/*******************************************************************************
 * sake/core/keyword/arg_packer.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef BOOST_PP_IS_ITERATING

#ifndef SAKE_CORE_KEYWORD_ARG_PACKER_HPP
#define SAKE_CORE_KEYWORD_ARG_PACKER_HPP

#include <boost/config.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_void.hpp>

#include <sake/boost_ext/mpl/vector.hpp>

#include <sake/core/keyword/arg_pack.hpp>
#include <sake/core/keyword/fwd.hpp>
#include <sake/core/keyword/satisfies.hpp>
#include <sake/core/keyword/tag_args.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/utility/workaround.hpp>

namespace sake
{

namespace keyword
{

template< class P >
struct arg_packer
{
    BOOST_STATIC_ASSERT((boost::mpl::is_sequence<P>::value));

#ifndef BOOST_NO_VARIADIC_TEMPLATES

    template< class... A >
    struct enable
    {
        typedef typename keyword::tag_args<
            boost_ext::mpl::vector< A... >, P
        >::type tagged_args;
        static bool const value = keyword::satisfies< tagged_args, P >::value;
        typedef enable type;
    };

    template< class > struct result;

    template< class This, class... A >
    struct result< This ( A... ) >
    {
        typedef typename keyword::tag_args<
            boost_ext::mpl::vector< A... >, P
        >::type tagged_args;
        BOOST_STATIC_ASSERT((!boost::is_void< tagged_args >::value));
        BOOST_STATIC_ASSERT((keyword::satisfies< tagged_args, P >::value));
        typedef typename keyword::arg_pack_from_mpl_vector< tagged_args >::type type;
    };

    template< class... A >
    typename result< arg_packer const ( SAKE_FWD2_PARAM( A )... ) >::type
    operator()(SAKE_FWD2_REF( A )... a) const
    {
        typedef typename result<
            arg_packer const ( SAKE_FWD2_PARAM( A )... )
        >::type result_type;
        return result_type(sake::forward<A>(a)...);
    }

#else // #ifndef BOOST_NO_VARIADIC_TEMPLATES

#if SAKE_WORKAROUND_MSC_VERSION_LESS_EQUAL( 1500 )
#pragma warning( push )
#pragma warning( disable : 4348 ) // 'sake::keyword::arg_packer<P>::enable' : redefinition of default parameter : parameter n
#endif // #if SAKE_WORKAROUND_MSC_VERSION_LESS_EQUAL( 1500 )
    template< BOOST_PP_ENUM_BINARY_PARAMS(
        SAKE_KEYWORD_MAX_ARITY, class A, = void BOOST_PP_INTERCEPT ) >
    struct enable;
#if SAKE_WORKAROUND_MSC_VERSION_LESS_EQUAL( 1500 )
#pragma warning( pop )
#endif // #if SAKE_WORKAROUND_MSC_VERSION_LESS_EQUAL( 1500 )

    template< class > struct result;

#define FWD2_PARAM_An( z, n, data )  SAKE_FWD2_PARAM( A ## n )
#define FWD2_REF_An_an( z, n, data ) SAKE_FWD2_REF( A ## n ) a ## n
#define forward_An_an( z, n, data )  sake::forward< A ## n >(a ## n)

#define BOOST_PP_ITERATION_LIMITS ( 1, SAKE_KEYWORD_MAX_ARITY )
#define BOOST_PP_FILENAME_1       <sake/core/keyword/arg_packer.hpp>
#include BOOST_PP_ITERATE()

#undef FWD2_PARAM_An
#undef FWD2_REF_An_an
#undef forward_An_an

#endif // #ifndef BOOST_NO_VARIADIC_TEMPLATES
    
};

} // namespace keyword

} // namespace sake

#endif // #ifndef SAKE_CORE_KEYWORD_ARG_PACKER_HPP

#else // #ifndef BOOST_PP_IS_ITERATING

#define N BOOST_PP_ITERATION()

#define class_A0N        BOOST_PP_ENUM_PARAMS( N, class A )
#define comma_class_A0N  BOOST_PP_ENUM_TRAILING_PARAMS( N, class A )
#define A0N              BOOST_PP_ENUM_PARAMS( N, A )
#define vectorN          BOOST_PP_CAT( boost::mpl::vector, N )
#define FWD2_PARAM_A0N   BOOST_PP_ENUM( N, FWD2_PARAM_An, ~ )
#define FWD2_REF_A0N_a0N BOOST_PP_ENUM( N, FWD2_REF_An_an, ~ )
#define forward_A0N_a0N  BOOST_PP_ENUM( N, forward_An_an, ~ )

    template< class_A0N >
#if N == SAKE_KEYWORD_MAX_ARITY
    struct enable
#else // #if N == SAKE_KEYWORD_MAX_ARITY
    struct enable< A0N >
#endif // #if N == SAKE_KEYWORD_MAX_ARITY
    {
        typedef typename keyword::tag_args< vectorN< A0N >, P >::type tagged_args;
        static bool const value = keyword::satisfies< tagged_args, P >::value;
        typedef enable type;
    };

    template< class This comma_class_A0N >
    struct result< This ( A0N ) >
    {
        typedef typename keyword::tag_args< vectorN< A0N >, P >::type tagged_args;
        BOOST_STATIC_ASSERT((!boost::is_void< tagged_args >::value));
        BOOST_STATIC_ASSERT((keyword::satisfies< tagged_args, P >::value));
        typedef typename keyword::arg_pack_from_mpl_vector< tagged_args >::type type;
    };

    template< class_A0N >
    typename result< arg_packer const ( FWD2_PARAM_A0N ) >::type
    operator()(FWD2_REF_A0N_a0N) const
    {
        typedef typename result< arg_packer const ( FWD2_PARAM_A0N ) >::type result_type;
        return result_type(forward_A0N_a0N);
    }

#undef class_A0N
#undef comma_class_A0N
#undef A0N
#undef vectorN
#undef FWD2_PARAM_A0N
#undef FWD2_REF_A0N_a0N
#undef forward_A0N_a0N

#undef N

#endif // #ifndef BOOST_PP_IS_ITERATING

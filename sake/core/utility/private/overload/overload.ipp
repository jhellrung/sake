/*******************************************************************************
 * sake/core/utility/private/overload/overload.ipp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_OVERLOAD_ITERATION_DEPTH
#include <sake/core/utility/private/overload/define_iteration_depth.ipp>
#endif // #ifndef SAKE_OVERLOAD_ITERATION_DEPTH



#if BOOST_PP_ITERATION_DEPTH() == SAKE_OVERLOAD_ITERATION_DEPTH



#ifndef SAKE_OVERLOAD_T
#define SAKE_OVERLOAD_T T
#endif // #ifndef SAKE_OVERLOAD_T
#ifndef SAKE_OVERLOAD_X
#define SAKE_OVERLOAD_X x
#endif // #ifndef SAKE_OVERLOAD_X



#if defined( SAKE_OVERLOAD_FUNCTION_NAME ) \
  + defined( SAKE_OVERLOAD_CTOR_NAME ) \
 != 1
#error Exactly one of the SAKE_OVERLOAD_XXX_NAME macros must be defined.
#endif // #if ... != 1



#ifndef SAKE_OVERLOAD_DECLARE_TEMPLATE_PARAMS
#define SAKE_OVERLOAD_DECLARE_TEMPLATE_PARAMS( r, n, classT_tuple ) \
    BOOST_PP_TUPLE_REM_CTOR( n, classT_tuple )
#endif // #ifndef SAKE_OVERLOAD_DECLARE_TEMPLATE_PARAMS

#ifndef SAKE_OVERLOAD_DECLARE_PARAMS
#define SAKE_OVERLOAD_DECLARE_PARAMS( r, n, T_tuple, Tx_tuple ) \
    BOOST_PP_TUPLE_REM_CTOR( n, Tx_tuple )
#endif // #ifndef SAKE_OVERLOAD_DECLARE_PARAMS

#ifndef SAKE_OVERLOAD_DECLARATION_SUFFIX
#define SAKE_OVERLOAD_DECLARATION_SUFFIX
#endif // SAKE_OVERLOAD_DECLARATION_SUFFIX



#ifndef SAKE_OVERLOAD_BODY
#error Must define SAKE_OVERLOAD_BODY.
#endif // #ifndef SAKE_OVERLOAD_BODY



#ifndef SAKE_OVERLOAD_MIN_ARITY
#define SAKE_OVERLOAD_MIN_ARITY 1
#endif // #ifndef SAKE_OVERLOAD_MIN_ARITY

#if !defined( SAKE_OVERLOAD_PERFECT_MAX_ARITY ) \
 && !defined( SAKE_OVERLOAD_FWD2_MAX_ARITY ) \
 && !defined( SAKE_OVERLOAD_FWD_MAX_ARITY )
#error At least one of the SAKE_OVERLOAD_XXX_MAX_ARITY macros must be defined.
#endif // #if ...

#ifndef SAKE_OVERLOAD_PERFECT_MAX_ARITY
#define SAKE_OVERLOAD_PERFECT_MAX_ARITY 0
#endif // #ifndef SAKE_OVERLOAD_PERFECT_MAX_ARITY

#ifndef SAKE_OVERLOAD_FWD2_MAX_ARITY
#define SAKE_OVERLOAD_FWD2_MAX_ARITY 0
#endif // #ifndef SAKE_OVERLOAD_FWD2_MAX_ARITY

#ifndef SAKE_OVERLOAD_FWD_MAX_ARITY
#define SAKE_OVERLOAD_FWD_MAX_ARITY 0
#endif // #ifndef SAKE_OVERLOAD_FWD_MAX_ARITY

#define SAKE_OVERLOAD_max_arity SAKE_OVERLOAD_PERFECT_MAX_ARITY
#if SAKE_OVERLOAD_max_arity < SAKE_OVERLOAD_FWD2_MAX_ARITY
#undef SAKE_OVERLOAD_max_arity
#define SAKE_OVERLOAD_max_arity SAKE_OVERLOAD_FWD2_MAX_ARITY
#endif // #if SAKE_OVERLOAD_max_arity < SAKE_OVERLOAD_FWD2_MAX_ARITY
#if SAKE_OVERLOAD_max_arity < SAKE_OVERLOAD_FWD_MAX_ARITY
#undef SAKE_OVERLOAD_max_arity
#define SAKE_OVERLOAD_max_arity SAKE_OVERLOAD_FWD_MAX_ARITY
#endif // #if SAKE_OVERLOAD_max_arity < SAKE_OVERLOAD_FWD_MAX_ARITY

#if SAKE_OVERLOAD_max_arity < SAKE_OVERLOAD_MIN_ARITY
#error At least one of SAKE_OVERLOAD_XXX_MAX_ARITY must be >= SAKE_OVERLOAD_MIN_ARITY.
#endif // #if SAKE_OVERLOAD_max_arity < SAKE_OVERLOAD_MIN_ARITY



#ifdef SAKE_OVERLOAD_CTOR_NAME

#ifdef SAKE_OVERLOAD_DEFINE_RESULT
#error Must define SAKE_OVERLOAD_DEFINE_RESULT only with SAKE_OVERLOAD_FUNCTION_NAME.
#endif // #ifdef SAKE_OVERLOAD_DEFINE_RESULT
#ifdef SAKE_OVERLOAD_RESULT
#error Must define SAKE_OVERLOAD_RESULT only with SAKE_OVERLOAD_FUNCTION_NAME.
#endif // #ifdef SAKE_OVERLOAD_RESULT
#ifdef SAKE_OVERLOAD_RESULT_TYPE
#error Must define SAKE_OVERLOAD_RESULT_TYPE only with SAKE_OVERLOAD_FUNCTION_NAME.
#endif // #ifdef SAKE_OVERLOAD_RESULT_TYPE

#ifdef SAKE_OVERLOAD_CTOR_INIT_LIST
#define SAKE_OVERLOAD_colon_ctor_init_list( r, n, T_tuple, x_tuple, forward_x_tuple ) \
    : SAKE_OVERLOAD_CTOR_INIT_LIST( r, n, T_tuple, x_tuple, forward_x_tuple )
#else // #ifdef SAKE_OVERLOAD_CTOR_INIT_LIST
#define SAKE_OVERLOAD_colon_ctor_init_list( r, n, T_tuple, x_tuple, forward_x_tuple )
#endif // #ifdef SAKE_OVERLOAD_CTOR_INIT_LIST

#ifdef SAKE_OVERLOAD_ENABLE
#define SAKE_OVERLOAD_comma_enable_if_enable( r, n, T_tuple ) \
    , typename ::boost::enable_if_c< SAKE_OVERLOAD_ENABLE( r, n, T_tuple ) ::value >::type* = 0
#else // #ifdef SAKE_OVERLOAD_ENABLE
#define SAKE_OVERLOAD_comma_enable_if_enable( r, n, T_tuple )
#endif // #ifdef SAKE_OVERLOAD_ENABLE

#define SAKE_OVERLOAD_definition_helper( r, n, T_tuple, Tx_tuple, x_tuple, forward_x_tuple ) \
    SAKE_OVERLOAD_CTOR_NAME ( \
        SAKE_OVERLOAD_DECLARE_PARAMS( r, n, T_tuple, Tx_tuple ) \
        SAKE_OVERLOAD_comma_enable_if_enable( r, n, T_tuple ) \
    ) \
    SAKE_OVERLOAD_DECLARATION_SUFFIX \
    SAKE_OVERLOAD_colon_ctor_init_list( r, n, T_tuple, x_tuple, forward_x_tuple ) \
    { SAKE_OVERLOAD_BODY( r, n, T_tuple, x_tuple, forward_x_tuple ) }

#else // #ifdef SAKE_OVERLOAD_CTOR_NAME

#ifdef SAKE_OVERLOAD_CTOR_INIT_LIST
#error Must define SAKE_OVERLOAD_CTOR_INIT_LIST only with SAKE_OVERLOAD_CTOR_NAME.
#endif // #ifdef SAKE_OVERLOAD_CTOR_INIT_LIST

#ifdef SAKE_OVERLOAD_RESULT
#ifdef SAKE_OVERLOAD_RESULT_TYPE
#error Must not define both SAKE_OVERLOAD_RESULT and SAKE_OVERLOAD_RESULT_TYPE.
#endif // #ifdef SAKE_OVERLOAD_RESULT_TYPE
#else // #ifdef SAKE_OVERLOAD_RESULT
#ifndef SAKE_OVERLOAD_RESULT_TYPE
#define SAKE_OVERLOAD_RESULT_TYPE void
#endif // #ifndef SAKE_OVERLOAD_RESULT_TYPE
#endif // #ifdef SAKE_OVERLOAD_RESULT

#ifdef SAKE_OVERLOAD_ENABLE
#ifdef SAKE_OVERLOAD_RESULT
#define SAKE_OVERLOAD_enable_if_enable_result_type( r, n, T_tuple ) \
    typename ::boost::lazy_enable_if_c< \
        SAKE_OVERLOAD_ENABLE( r, n, T_tuple ) ::value, \
        SAKE_OVERLOAD_RESULT( r, n, T_tuple ) \
    >::type
#else // #ifdef SAKE_OVERLOAD_RESULT
#define SAKE_OVERLOAD_enable_if_enable_result_type( r, n, T_tuple ) \
    typename ::boost::enable_if_c< \
        SAKE_OVERLOAD_ENABLE( r, n, T_tuple ) ::value, \
        SAKE_OVERLOAD_RESULT_TYPE \
    >::type
#endif // #ifdef SAKE_OVERLOAD_RESULT
#else // #ifdef SAKE_OVERLOAD_ENABLE
#ifdef SAKE_OVERLOAD_RESULT
#define SAKE_OVERLOAD_enable_if_enable_result_type( r, n, T_tuple ) \
    typename SAKE_OVERLOAD_RESULT( r, n, T_tuple ) ::type
#else // #ifdef SAKE_OVERLOAD_RESULT
#define SAKE_OVERLOAD_enable_if_enable_result_type( r, n, T_tuple ) \
    SAKE_OVERLOAD_RESULT_TYPE
#endif // #ifdef SAKE_OVERLOAD_RESULT
#endif // #ifdef SAKE_OVERLOAD_ENABLE

#define SAKE_OVERLOAD_definition_helper( r, n, T_tuple, Tx_tuple, x_tuple, forward_x_tuple ) \
    inline SAKE_OVERLOAD_enable_if_enable_result_type( r, n, T_tuple ) \
    SAKE_OVERLOAD_FUNCTION_NAME ( SAKE_OVERLOAD_DECLARE_PARAMS( r, n, T_tuple, Tx_tuple ) ) \
    SAKE_OVERLOAD_DECLARATION_SUFFIX \
    { SAKE_OVERLOAD_BODY( r, n, T_tuple, x_tuple, forward_x_tuple ) }

#ifdef SAKE_OVERLOAD_DEFINE_RESULT

#ifndef SAKE_OVERLOAD_DECLARE_RESULT_TEMPLATE_PARAMS
#define SAKE_OVERLOAD_DECLARE_RESULT_TEMPLATE_PARAMS( r, n, classT_tuple ) \
    class This, BOOST_PP_TUPLE_REM_CTOR( n, classT_tuple )
#endif // #ifndef SAKE_OVERLOAD_DECLARE_RESULT_TEMPLATE_PARAMS
#ifndef SAKE_OVERLOAD_RESULT_SPECIALIZATION
#define SAKE_OVERLOAD_RESULT_SPECIALIZATION( r, n, T_tuple ) \
    result< This T_tuple >
#endif // #ifndef SAKE_OVERLOAD_RESULT_SPECIALIZATION

    template< class > struct result;

#define BOOST_PP_ITERATION_LIMITS ( SAKE_OVERLOAD_MIN_ARITY, SAKE_OVERLOAD_max_arity )
#include <sake/core/utility/private/overload/define_filename_x.ipp>
#include BOOST_PP_ITERATE()

#undef SAKE_OVERLOAD_DEFINE_RESULT
#undef SAKE_OVERLOAD_DECLARE_RESULT_TEMPLATE_PARAMS
#undef SAKE_OVERLOAD_RESULT_SPECIALIZATION

#else // #ifdef SAKE_OVERLOAD_DEFINE_RESULT

#ifdef SAKE_OVERLOAD_DECLARE_RESULT_TEMPLATE_PARAMS
#error Must define SAKE_OVERLOAD_DECLARE_RESULT_TEMPLATE_PARAMS only with SAKE_OVERLOAD_DEFINE_RESULT.
#endif // #ifdef SAKE_OVERLOAD_DECLARE_RESULT_TEMPLATE_PARAMS
#ifdef SAKE_OVERLOAD_RESULT_SPECIALIZATION
#error Must define SAKE_OVERLOAD_RESULT_SPECIALIZATION only with SAKE_OVERLOAD_DEFINE_RESULT.
#endif // #ifdef SAKE_OVERLOAD_RESULT_SPECIALIZATION

#endif // #ifdef SAKE_OVERLOAD_DEFINE_RESULT

#endif // #ifdef SAKE_OVERLOAD_CTOR_NAME



#ifndef BOOST_NO_RVALUE_REFERENCES

#define BOOST_PP_ITERATION_LIMITS ( SAKE_OVERLOAD_MIN_ARITY, SAKE_OVERLOAD_max_arity )
#include <sake/core/utility/private/overload/define_filename_x.ipp>
#include BOOST_PP_ITERATE()

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

#define SAKE_OVERLOAD_definition( r, rvT_seq ) \
    SAKE_OVERLOAD_definition_impl( r, \
        BOOST_PP_SEQ_SIZE( rvT_seq ), \
        ( BOOST_PP_SEQ_FOR_EACH_I_R( r, SAKE_OVERLOAD_comma_remove_rvref_elem, ~, rvT_seq ) ), \
        ( BOOST_PP_SEQ_FOR_EACH_I_R( r, SAKE_OVERLOAD_comma_elem_xi, ~, rvT_seq ) ), \
        ( BOOST_PP_ENUM( BOOST_PP_SEQ_SIZE( rvT_seq ), SAKE_OVERLOAD_AS_LVALUE_xn, ~ ) ), \
        ( BOOST_PP_ENUM( BOOST_PP_SEQ_SIZE( rvT_seq ), SAKE_OVERLOAD_forward_Tn_xn, ~ ) ) \
    )

#define SAKE_OVERLOAD_definition_impl( r, n, T_tuple, Tx_tuple, x_tuple, forward_x_tuple ) \
    template< SAKE_OVERLOAD_DECLARE_TEMPLATE_PARAMS( r, \
        n, ( BOOST_PP_ENUM_PARAMS( n, class SAKE_OVERLOAD_T ) ) ) > \
    SAKE_OVERLOAD_definition_helper( r, n, T_tuple, Tx_tuple, x_tuple, forward_x_tuple )

#define BOOST_PP_ITERATION_LIMITS ( SAKE_OVERLOAD_MIN_ARITY, SAKE_OVERLOAD_max_arity )
#include <sake/core/utility/private/overload/define_filename_x.ipp>
#include BOOST_PP_ITERATE()

#undef SAKE_OVERLOAD_definition
#undef SAKE_OVERLOAD_definition_impl

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES



#undef SAKE_OVERLOAD_definition_helper

#undef SAKE_OVERLOAD_colon_ctor_init_list
#undef SAKE_OVERLOAD_comma_enable_if_enable

#undef SAKE_OVERLOAD_enable_if_enable_result_type

#undef SAKE_OVERLOAD_max_arity

#undef SAKE_OVERLOAD_DECLARE_TEMPLATE_PARAMS
#undef SAKE_OVERLOAD_DECLARE_PARAMS
#undef SAKE_OVERLOAD_DECLARATION_SUFFIX
#undef SAKE_OVERLOAD_RESULT
#undef SAKE_OVERLOAD_RESULT_TYPE
#undef SAKE_OVERLOAD_ENABLE
#undef SAKE_OVERLOAD_FUNCTION_NAME
#undef SAKE_OVERLOAD_CTOR_NAME
#undef SAKE_OVERLOAD_CTOR_INIT_LIST
#undef SAKE_OVERLOAD_BODY
#undef SAKE_OVERLOAD_MIN_ARITY
#undef SAKE_OVERLOAD_PERFECT_MAX_ARITY
#undef SAKE_OVERLOAD_FWD2_MAX_ARITY
#undef SAKE_OVERLOAD_FWD_MAX_ARITY

#undef SAKE_OVERLOAD_T
#undef SAKE_OVERLOAD_X



#undef SAKE_OVERLOAD_ITERATION_DEPTH



#else // #if BOOST_PP_ITERATION_DEPTH() == SAKE_OVERLOAD_ITERATION_DEPTH



#if BOOST_PP_ITERATION_DEPTH() != SAKE_OVERLOAD_ITERATION_DEPTH + 1
#error Inconsistency between BOOST_PP_ITERATION_DEPTH() and SAKE_OVERLOAD_ITERATION_DEPTH.
#endif // #if BOOST_PP_ITERATION_DEPTH() != SAKE_OVERLOAD_ITERATION_DEPTH + 1



#define SAKE_OVERLOAD_N BOOST_PP_ITERATION()



#ifdef SAKE_OVERLOAD_DEFINE_RESULT



template< SAKE_OVERLOAD_DECLARE_RESULT_TEMPLATE_PARAMS( 1, // r
    SAKE_OVERLOAD_N, ( SAKE_OVERLOAD_class_T0N ) ) >
struct SAKE_OVERLOAD_RESULT_SPECIALIZATION( 1, // r
    SAKE_OVERLOAD_N, ( SAKE_OVERLOAD_T0N ) )
{
#ifdef SAKE_OVERLOAD_ENABLE
    BOOST_STATIC_ASSERT(( SAKE_OVERLOAD_ENABLE( 1, // r
        SAKE_OVERLOAD_N, ( SAKE_OVERLOAD_T0N ) ) ::value ));
#endif // #ifdef SAKE_OVERLOAD_ENABLE
#ifdef SAKE_OVERLOAD_RESULT
    typedef typename SAKE_OVERLOAD_RESULT( 1, // r
        SAKE_OVERLOAD_N, ( SAKE_OVERLOAD_T0N ) ) ::type type;
#else // #ifdef SAKE_OVERLOAD_RESULT
    typedef SAKE_OVERLOAD_RESULT_TYPE type;
#endif // #ifdef SAKE_OVERLOAD_RESULT
};



#else // #ifdef SAKE_OVERLOAD_DEFINE_RESULT



#ifndef BOOST_NO_RVALUE_REFERENCES

template< SAKE_OVERLOAD_DECLARE_TEMPLATE_PARAMS( 1, // r
    SAKE_OVERLOAD_N, ( SAKE_OVERLOAD_class_T0N ) ) >
SAKE_OVERLOAD_definition_helper( 1, // r
    SAKE_OVERLOAD_N,
    ( SAKE_OVERLOAD_T0N ),
    ( SAKE_OVERLOAD_T0N_x0N ),
    ( SAKE_OVERLOAD_x0N ),
    ( SAKE_OVERLOAD_forward_T0N_x0N )
)

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

#if SAKE_OVERLOAD_N <= SAKE_OVERLOAD_PERFECT_MAX_ARITY
BOOST_PP_SEQ_FOR_EACH_PRODUCT(
    SAKE_OVERLOAD_definition,
    BOOST_PP_REPEAT( SAKE_OVERLOAD_N, SAKE_OVERLOAD_perfect_template_param_seq, ~ )
)
#else // #if SAKE_OVERLOAD_N <= SAKE_OVERLOAD_PERFECT_MAX_ARITY
#if SAKE_OVERLOAD_N <= SAKE_OVERLOAD_FWD_MAX_ARITY
SAKE_OVERLOAD_definition( 1, // r
    BOOST_PP_REPEAT( SAKE_OVERLOAD_N, SAKE_OVERLOAD_fwd_template_param, ~ ) )
#endif // #if SAKE_OVERLOAD_N <= SAKE_OVERLOAD_FWD_MAX_ARITY
#if SAKE_OVERLOAD_N <= SAKE_OVERLOAD_FWD2_MAX_ARITY
SAKE_OVERLOAD_definition( 1, // r
    BOOST_PP_REPEAT( SAKE_OVERLOAD_N, SAKE_OVERLOAD_fwd2_template_param, ~ ) )
#endif // #if SAKE_OVERLOAD_N <= SAKE_OVERLOAD_FWD2_MAX_ARITY
#endif // #if SAKE_OVERLOAD_N <= SAKE_OVERLOAD_PERFECT_MAX_ARITY

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES



#endif // #ifdef SAKE_OVERLOAD_DEFINE_RESULT



#undef SAKE_OVERLOAD_N



#endif // #if BOOST_PP_ITERATION_DEPTH() == SAKE_OVERLOAD_ITERATION_DEPTH

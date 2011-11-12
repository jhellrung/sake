/*******************************************************************************
 * sake/core/utility/assert.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * SAKE_ASSERT( expression )
 * SAKE_ASSERT_ALL[_R]( [r,] subexpression_seq )
 * SAKE_ASSERT_ANY[_R]( [r,] subexpression_seq )
 * SAKE_ASSERT_RELATION( lhs, op, rhs )
 * SAKE_ASSERT_RELATION_ALL[_R]( [r,] relation_seq )
 * SAKE_ASSERT_RELATION_ANY[_R]( [r,] relation_seq )
 *
 * SAKE_REQUIRE( expression )
 * SAKE_REQUIRE_ALL[_R]( [r,] subexpression_seq )
 * SAKE_REQUIRE_ANY[_R]( [r,] subexpression_seq )
 * SAKE_REQUIRE_RELATION( lhs, op, rhs )
 * SAKE_REQUIRE_RELATION_ALL[_R]( [r,] relation_seq )
 * SAKE_REQUIRE_RELATION_ANY[_R]( [r,] relation_seq )
 *
 * struct assert_failure_action::functional::abort
 * struct assert_failure_action::functional::terminate
 * struct assert_failure_action::functional::exit
 * ... assert_failure_action::abort
 * ... assert_failure_action::terminate
 * ... assert_failure_action::exit
 *
 * These macros provide replacements for the well-known and idiomatic assert
 * macro in <cassert>.
 *
 * SAKE_ASSERT* vs SAKE_REQUIRE*
 * -----------------------------
 * The SAKE_ASSERT* macros can be globally disabled via preprocessor macro
 * defines.  By default, NDEBUG controls whether any SAKE_ASSERT* macro produces
 * executable code or not (same as for assert).  Defining SAKE_DEBUG forces all
 * SAKE_ASSERT* macros to be enabled, while defining SAKE_NDEBUG forces all
 * SAKE_ASSERT* macros to be disabled, independent of whether NDEBUG is defined
 * or not.
 *
 * In contrast, the SAKE_REQUIRE* macros are always enabled.
 *
 * Otherwise, corresponding SAKE_ASSERT* and SAKE_REQUIRE* macros have the same
 * semantics.
 *
 * Failure Action
 * --------------
 * Each macro calls a global function object (the failure action) upon failure
 * of the assertion.  By default, this amounts to printing a message to the
 * ostream given by the preprocessor macro SAKE_ASSERT_OSTREAM (which defaults
 * to if not defined otherwise std::cerr) and calling std::abort.  One of the
 * three provided failure actions may be specified explicitly by defining one of
 * SAKE_ASSERT_ON_FAILURE_{ABORT,TERMINATE,EXIT}, which call
 * assert_failure_action::{abort,terminate,exit}, respectively.  A custom
 * failure action may be used by defining the macro SAKE_ASSERT_FAILURE_ACTION
 * as the name of the global function object.  Any custom failure actions must
 * have the same set of operator() overloads as assert_failure_action::{abort,
 * terminate,exit}.
 *
 * SAKE_ASSERT*
 * ------------
 * SAKE_ASSERT( expression ) evalutes the given expression in a boolean context
 * and fails (calls the failure action) if the result is false.
 *
 * SAKE_ASSERT_ALL( subexpression_seq ) (where subexpression_seq is a
 * Boost.Preprocessor seq) is semantically equivalent to SAKE_ASSERT(
 * subexpression_seq[0] && ... && subexpression_seq[n-1] ), except it will
 * identify the first subexpression which evaluated to false.
 *
 * SAKE_ASSERT_ANY( subexpression_seq ) (where subexpression_seq is a
 * Boost.Preprocessor seq) is semantically equivalent to SAKE_ASSERT(
 * subexpression_seq[0] || ... || subexpression_seq[n-1] ).
 *
 * SAKE_ASSERT_RELATION( lhs, op, rhs ) (where op is one of ==, !=, <, >, <=,
 * >=) is semantically equivalent to SAKE_ASSERT( lhs op rhs ), except it will
 * evaluate lhs and rhs and pass these to the failure action.  This
 * usually means that the results of evaluating lhs and rhs are converted to
 * strings (via the stream operator operator<<), giving more helpful debugging
 * information.  Note that lhs and rhs may be evaluated more than once.
 *
 * SAKE_ASSERT_RELATION_ALL( relation_seq ) (where relation_seq is a
 * Boost.Preprocessor seq of Boost.Preprocessor 3-tuples that each look like
 * ( lhs, op, rhs )) is semantically equivalent to SAKE_ASSERT( ( lhs[0] op[0]
 * rhs[0] ) && ... && ( lhs[n-1] op[n-1] rhs[n-1] ) ), except it will identify
 * the first subexpression i which evaluated to false and provide the
 * evaluations of lhs[i] and rhs[i].
 *
 * SAKE_ASSERT_RELATION_ANY( relation_seq ) (where relation_seq is a
 * Boost.Preprocessor seq of Boost.Preprocessor 3-tuples that each look like
 * ( lhs, op, rhs )) is semantically equivalent to SAKE_ASSERT( ( lhs[0] op[0]
 * rhs[0] ) || ... || ( lhs[n-1] op[n-1] rhs[n-1] ) ), except it will provide
 * evaluations of all the lhs[i]s and rhs[i]s.
 ******************************************************************************/

#ifndef BOOST_PP_IS_ITERATING

#ifndef BOOST_PP_IS_SELFISH

#ifndef SAKE_CORE_UTILITY_ASSERT_HPP
#define SAKE_CORE_UTILITY_ASSERT_HPP

#include <cstdlib>

#include <exception>
#include <iostream>
#include <sstream>

#include <boost/current_function.hpp>
#include <boost/preprocessor/control/expr_if.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/iteration/self.hpp>
#include <boost/preprocessor/repetition/deduce_r.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/seq/enum.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/is_floating_point.hpp>
#include <boost/type_traits/is_signed.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/mpl/or.hpp>

#include <sake/core/utility/debug.hpp>
#include <sake/core/utility/ostreamable.hpp>



#ifndef SAKE_ASSERT_MAX_ARITY
#define SAKE_ASSERT_MAX_ARITY 8
#endif // #ifndef SAKE_ASSERT_MAX_ARITY



#ifndef SAKE_ASSERT_FAILURE_ACTION
#if defined( SAKE_ASSERT_ON_FAILURE_ABORT )
#define SAKE_ASSERT_FAILURE_ACTION ::sake::assert_failure_action::abort
#elif defined( SAKE_ASSERT_ON_FAILURE_TERMINATE )
#define SAKE_ASSERT_FAILURE_ACTION ::sake::assert_failure_action::terminate
#elif defined( SAKE_ASSERT_ON_FAILURE_EXIT )
#define SAKE_ASSERT_FAILURE_ACTION ::sake::assert_failure_action::exit
#else
#define SAKE_ASSERT_FAILURE_ACTION ::sake::assert_failure_action::abort
#endif
#endif // #ifndef SAKE_ASSERT_FAILURE_ACTION



#ifndef SAKE_ASSERT_OSTREAM
#define SAKE_ASSERT_OSTREAM ::std::cerr
#endif // #ifndef SAKE_ASSERT_OSTREAM



#ifndef SAKE_ASSERT_FAILURE_MACRO
#define SAKE_ASSERT_FAILURE_MACRO( data, info_seq ) \
    BOOST_PP_TUPLE_ELEM( 3, 0, data ) ( \
        BOOST_PP_TUPLE_ELEM( 3, 1, data ), \
        BOOST_PP_TUPLE_ELEM( 3, 2, data ), \
        BOOST_PP_SEQ_ENUM( info_seq ) \
    )
#endif // #ifndef SAKE_ASSERT_FAILURE_MACRO



/*******************************************************************************
 * SAKE_ASSERT( expression )
 * SAKE_ASSERT_RELATION( lhs, op, rhs )
 ******************************************************************************/

#ifdef SAKE_DEBUG

#define SAKE_ASSERT( expression ) \
    SAKE_ASSERT_EXT( \
        expression, \
        SAKE_ASSERT_FAILURE_MACRO, \
        ( SAKE_ASSERT_FAILURE_ACTION, SAKE_ASSERT_OSTREAM, "SAKE_ASSERT" ) \
    )
#define SAKE_ASSERT_RELATION( lhs, op, rhs ) \
    SAKE_ASSERT_EXT_RELATION( \
        lhs, op, rhs, \
        SAKE_ASSERT_FAILURE_MACRO, \
        ( SAKE_ASSERT_FAILURE_ACTION, SAKE_ASSERT_OSTREAM, "SAKE_ASSERT_RELATION" ) \
    )

#else // #ifdef SAKE_DEBUG

#define SAKE_ASSERT( expression )            static_cast< void >(sizeof( expression ? 0 : 0 ))
#define SAKE_ASSERT_RELATION( lhs, op, rhs ) static_cast< void >(sizeof( lhs op rhs ? 0 : 0 ))

#endif // #ifdef SAKE_DEBUG

/*******************************************************************************
 * SAKE_ASSERT_ALL( subexpression_seq )
 * SAKE_ASSERT_ANY( subexpression_seq )
 * SAKE_ASSERT_RELATION_ALL( relation_seq )
 * SAKE_ASSERT_RELATION_ANY( relation_seq )
 ******************************************************************************/

#define SAKE_ASSERT_ALL( subexpression_seq ) \
    SAKE_ASSERT_ALL_R( BOOST_PP_DEDUCE_R(), subexpression_seq )
#define SAKE_ASSERT_ANY( subexpression_seq ) \
    SAKE_ASSERT_ANY_R( BOOST_PP_DEDUCE_R(), subexpression_seq )
#define SAKE_ASSERT_RELATION_ALL( relation_seq ) \
    SAKE_ASSERT_RELATION_ALL_R( BOOST_PP_DEDUCE_R(), relation_seq )
#define SAKE_ASSERT_RELATION_ANY( relation_seq ) \
    SAKE_ASSERT_RELATION_ANY_R( BOOST_PP_DEDUCE_R(), relation_seq )

/*******************************************************************************
 * SAKE_ASSERT_ALL_R( r, subexpression_seq )
 * SAKE_ASSERT_ANY_R( r, subexpression_seq )
 * SAKE_ASSERT_RELATION_ALL_R( r, relation_seq )
 * SAKE_ASSERT_RELATION_ANY_R( r, relation_seq )
 ******************************************************************************/

#ifdef SAKE_DEBUG

#define SAKE_ASSERT_ALL_R( r, subexpression_seq ) \
    SAKE_ASSERT_EXT_ALL_R( r, \
        subexpression_seq, \
        SAKE_ASSERT_FAILURE_MACRO, \
        ( SAKE_ASSERT_FAILURE_ACTION, SAKE_ASSERT_OSTREAM, "SAKE_ASSERT_ALL" ) \
    )
#define SAKE_ASSERT_ANY_R( r, subexpression_seq ) \
    SAKE_ASSERT_EXT_ANY_R( r, \
        subexpression_seq, \
        SAKE_ASSERT_FAILURE_MACRO, \
        ( SAKE_ASSERT_FAILURE_ACTION, SAKE_ASSERT_OSTREAM, "SAKE_ASSERT_ANY" ) \
    )
#define SAKE_ASSERT_RELATION_ALL_R( r, relation_seq ) \
    SAKE_ASSERT_EXT_RELATION_ALL_R( r, \
        relation_seq, \
        SAKE_ASSERT_FAILURE_MACRO, \
        ( SAKE_ASSERT_FAILURE_ACTION, SAKE_ASSERT_OSTREAM, "SAKE_ASSERT_RELATION_ALL" ) \
    )
#define SAKE_ASSERT_RELATION_ANY_R( r, relation_seq ) \
    SAKE_ASSERT_EXT_RELATION_ANY_R( r, \
        relation_seq, \
        SAKE_ASSERT_FAILURE_MACRO, \
        ( SAKE_ASSERT_FAILURE_ACTION, SAKE_ASSERT_OSTREAM, "SAKE_ASSERT_RELATION_ANY" ) \
    )

#else // #ifdef SAKE_DEBUG

#define SAKE_ASSERT_ALL_R( r, subexpression_seq ) \
    static_cast< void >(sizeof( BOOST_PP_SEQ_FOR_EACH_I_R( r, SAKE_ASSERT_andor_subexpression, &&, subexpression_seq ) ? 0 : 0 ))
#define SAKE_ASSERT_ANY_R( r, subexpression_seq ) \
    static_cast< void >(sizeof( BOOST_PP_SEQ_FOR_EACH_I_R( r, SAKE_ASSERT_andor_subexpression, ||, subexpression_seq ) ? 0 : 0 ))
#define SAKE_ASSERT_RELATION_ALL_R( r, relation_seq ) \
    static_cast< void >(sizeof( BOOST_PP_SEQ_FOR_EACH_I_R( r, SAKE_ASSERT_andor_lhs_op_rhs, &&, relation_seq ) ? 0 : 0 ))
#define SAKE_ASSERT_RELATION_ANY_R( r, relation_seq ) \
    static_cast< void >(sizeof( BOOST_PP_SEQ_FOR_EACH_I_R( r, SAKE_ASSERT_andor_lhs_op_rhs, ||, relation_seq ) ? 0 : 0 ))

#endif // #ifdef SAKE_DEBUG

/*******************************************************************************
 * SAKE_REQUIRE( expression )
 * SAKE_REQUIRE_RELATION( lhs, op, rhs )
 ******************************************************************************/

#define SAKE_REQUIRE( expression ) \
    SAKE_ASSERT_EXT( \
        expression, \
        SAKE_ASSERT_FAILURE_MACRO, \
        ( SAKE_ASSERT_FAILURE_ACTION, SAKE_ASSERT_OSTREAM, "SAKE_REQUIRE" ) \
    )
#define SAKE_REQUIRE_RELATION( lhs, op, rhs ) \
    SAKE_ASSERT_EXT_RELATION( \
        lhs, op, rhs, \
        SAKE_ASSERT_FAILURE_MACRO, \
        ( SAKE_ASSERT_FAILURE_ACTION, SAKE_ASSERT_OSTREAM, "SAKE_REQUIRE_RELATION" ) \
    )

/*******************************************************************************
 * SAKE_REQUIRE_ALL( subexpression_seq )
 * SAKE_REQUIRE_ANY( subexpression_seq )
 * SAKE_REQUIRE_RELATION_ALL( relation_seq )
 * SAKE_REQUIRE_RELATION_ANY( relation_seq )
 ******************************************************************************/

#define SAKE_REQUIRE_ALL( subexpression_seq ) \
    SAKE_REQUIRE_ALL_R( BOOST_PP_DEDUCE_R(), subexpression_seq )
#define SAKE_REQUIRE_ANY( subexpression_seq ) \
    SAKE_REQUIRE_ANY_R( BOOST_PP_DEDUCE_R(), subexpression_seq )
#define SAKE_REQUIRE_RELATION_ALL( relation_seq ) \
    SAKE_REQUIRE_RELATION_ALL_R( BOOST_PP_DEDUCE_R(), relation_seq )
#define SAKE_REQUIRE_RELATION_ANY( relation_seq ) \
    SAKE_REQUIRE_RELATION_ANY_R( BOOST_PP_DEDUCE_R(), relation_seq )

/*******************************************************************************
 * SAKE_REQUIRE_ALL_R( r, subexpression_seq )
 * SAKE_REQUIRE_ANY_R( r, subexpression_seq )
 * SAKE_REQUIRE_RELATION_ALL_R( r, relation_seq )
 * SAKE_REQUIRE_RELATION_ANY_R( r, relation_seq )
 ******************************************************************************/

#define SAKE_REQUIRE_ALL_R( r, subexpression_seq ) \
    SAKE_ASSERT_EXT_ALL_R( r, \
        subexpression_seq, \
        SAKE_ASSERT_FAILURE_MACRO, \
        ( SAKE_ASSERT_FAILURE_ACTION, SAKE_ASSERT_OSTREAM, "SAKE_REQUIRE_ALL" ) \
    )
#define SAKE_REQUIRE_ANY_R( r, subexpression_seq ) \
    SAKE_ASSERT_EXT_ANY_R( r, \
        subexpression_seq, \
        SAKE_ASSERT_FAILURE_MACRO, \
        ( SAKE_ASSERT_FAILURE_ACTION, SAKE_ASSERT_OSTREAM, "SAKE_REQUIRE_ANY" ) \
    )
#define SAKE_REQUIRE_RELATION_ALL_R( r, relation_seq ) \
    SAKE_ASSERT_EXT_RELATION_ALL_R( r, \
        relation_seq, \
        SAKE_ASSERT_FAILURE_MACRO, \
        ( SAKE_ASSERT_FAILURE_ACTION, SAKE_ASSERT_OSTREAM, "SAKE_REQUIRE_RELATION_ALL" ) \
    )
#define SAKE_REQUIRE_RELATION_ANY_R( r, relation_seq ) \
    SAKE_ASSERT_EXT_RELATION_ANY_R( r, \
        relation_seq, \
        SAKE_ASSERT_FAILURE_MACRO, \
        ( SAKE_ASSERT_FAILURE_ACTION, SAKE_ASSERT_OSTREAM, "SAKE_REQUIRE_RELATION_ANY" ) \
    )

/*******************************************************************************
 * SAKE_ASSERT_EXT_ALL( subexpression_seq, failure_macro, data )
 * SAKE_ASSERT_EXT_ANY( subexpression_seq, failure_macro, data )
 * SAKE_ASSERT_EXT_RELATION_ALL( relation_seq, failure_macro, data )
 * SAKE_ASSERT_EXT_RELATION_ANY( relation_seq, failure_macro, data )
 ******************************************************************************/

#define SAKE_ASSERT_EXT_ALL( subexpression_seq, failure_macro, data ) \
    SAKE_ASSERT_EXT_ALL_R( BOOST_PP_DEDUCE_R(), subexpression_seq, failure_macro, data )
#define SAKE_ASSERT_EXT_ANY( subexpression_seq, failure_macro, data ) \
    SAKE_ASSERT_EXT_ANY_R( BOOST_PP_DEDUCE_R(), subexpression_seq, failure_macro, data )
#define SAKE_ASSERT_EXT_RELATION_ALL( relation_seq, failure_macro, data ) \
    SAKE_ASSERT_EXT_RELATION_ALL_R( BOOST_PP_DEDUCE_R(), relation_seq, failure_macro, data )
#define SAKE_ASSERT_EXT_RELATION_ANY( relation_seq, failure_macro, data ) \
    SAKE_ASSERT_EXT_RELATION_ANY_R( BOOST_PP_DEDUCE_R(), relation_seq, failure_macro, data )

/*******************************************************************************
 * SAKE_ASSERT_EXT( expression, failure_macro, data )
 ******************************************************************************/

#define SAKE_ASSERT_EXT( expression, failure_macro, data ) \
    static_cast< void >( expression ? 0 : ( failure_macro( \
        data, \
        ( BOOST_PP_STRINGIZE( expression ) ) \
        ( BOOST_PP_STRINGIZE( __FILE__ ) ) \
        ( BOOST_CURRENT_FUNCTION ) \
        ( __LINE__ ) \
    ), 0 ) )

/*******************************************************************************
 * SAKE_ASSERT_EXT_ALL_R( r, subexpression_seq, failure_macro, data )
 ******************************************************************************/

#define SAKE_ASSERT_EXT_ALL_R( r, subexpression_seq, failure_macro, data ) \
    static_cast< void >( true BOOST_PP_SEQ_FOR_EACH_I_R( r, \
        SAKE_ASSERT_ALL_and_subexpression_fail, \
        ( failure_macro, \
          data, \
          BOOST_PP_STRINGIZE( BOOST_PP_SEQ_FOR_EACH_I_R( r, \
              SAKE_ASSERT_andor_subexpression, &&, subexpression_seq ) ) ), \
        subexpression_seq \
    ) )
#define SAKE_ASSERT_ALL_and_subexpression_fail( r, data, i, elem ) \
    && ( elem ? true : ( BOOST_PP_TUPLE_ELEM( 3, 0, data ) ( \
        BOOST_PP_TUPLE_ELEM( 3, 1, data ), \
        ( BOOST_PP_TUPLE_ELEM( 3, 2, data ) ) \
        ( BOOST_PP_STRINGIZE( __FILE__ ) ) \
        ( BOOST_CURRENT_FUNCTION ) \
        ( __LINE__ ) \
        ( i ) \
        ( BOOST_PP_STRINGIZE( elem ) ) \
    ), false ) )

/*******************************************************************************
 * SAKE_ASSERT_EXT_ANY_R( r, subexpression_seq, failure_macro, data )
 ******************************************************************************/

#define SAKE_ASSERT_EXT_ANY_R( r, subexpression_seq, failure_macro, data ) \
    SAKE_ASSERT_EXT( \
        BOOST_PP_SEQ_FOR_EACH_I_R( r, SAKE_ASSERT_andor_subexpression, ||, subexpression_seq ), \
        failure_macro, \
        data \
    )

/*******************************************************************************
 * SAKE_ASSERT_EXT_RELATION( lhs, op, rhs, failure_macro, data )
 ******************************************************************************/

#define SAKE_ASSERT_EXT_RELATION( lhs, op, rhs, failure_macro, data ) \
    static_cast< void >( (lhs) op (rhs) ? 0 : ( failure_macro( \
        data, \
        ( BOOST_PP_STRINGIZE( (lhs) op (rhs) ) ) \
        ( BOOST_PP_STRINGIZE( __FILE__ ) ) \
        ( BOOST_CURRENT_FUNCTION ) \
        ( __LINE__ ) \
        ( lhs ) \
        ( BOOST_PP_STRINGIZE( op ) ) \
        ( rhs ) \
    ), 0 ) )

/*******************************************************************************
 * SAKE_ASSERT_EXT_RELATION_ALL_R( r, relation_seq, failure_macro, data )
 ******************************************************************************/

#define SAKE_ASSERT_EXT_RELATION_ALL_R( r, relation_seq, failure_macro, data ) \
    static_cast< void >( true BOOST_PP_SEQ_FOR_EACH_I_R( r, \
        SAKE_ASSERT_RELATION_ALL_and_subexpression_fail, \
        ( failure_macro, \
          data, \
          BOOST_PP_STRINGIZE( BOOST_PP_SEQ_FOR_EACH_I_R( r, \
              SAKE_ASSERT_andor_lhs_op_rhs, &&, relation_seq ) ) ), \
        relation_seq \
    ) )
#define SAKE_ASSERT_RELATION_ALL_and_subexpression_fail( r, data, i, elem ) \
    SAKE_ASSERT_RELATION_ALL_and_subexpression_fail_impl( \
        r, data, i, \
        BOOST_PP_TUPLE_ELEM( 3, 0, elem ), \
        BOOST_PP_TUPLE_ELEM( 3, 1, elem ), \
        BOOST_PP_TUPLE_ELEM( 3, 2, elem ) \
    )
#define SAKE_ASSERT_RELATION_ALL_and_subexpression_fail_impl( r, data, i, lhs, op, rhs ) \
    && ( (lhs) op (rhs) ? true : ( BOOST_PP_TUPLE_ELEM( 3, 0, data ) ( \
        BOOST_PP_TUPLE_ELEM( 3, 1, data ), \
        ( BOOST_PP_TUPLE_ELEM( 3, 2, data ) ) \
        ( BOOST_PP_STRINGIZE( __FILE__ ) ) \
        ( BOOST_CURRENT_FUNCTION ) \
        ( __LINE__ ) \
        ( i ) \
        ( BOOST_PP_STRINGIZE( (lhs) op (rhs) ) ) \
        ( lhs ) \
        ( BOOST_PP_STRINGIZE( op ) ) \
        ( rhs ) \
    ), false ) )

/*******************************************************************************
 * SAKE_ASSERT_EXT_RELATION_ANY_R( r, relation_seq, failure_macro, data )
 ******************************************************************************/

#define SAKE_ASSERT_EXT_RELATION_ANY_R( r, relation_seq, failure_macro, data ) \
    SAKE_ASSERT_EXT_RELATION_ANY_R_impl( \
        r, relation_seq, failure_macro, data, \
        BOOST_PP_SEQ_FOR_EACH_I_R( r, SAKE_ASSERT_andor_lhs_op_rhs, ||, relation_seq ) \
    )
#define SAKE_ASSERT_EXT_RELATION_ANY_R_impl( r, relation_seq, failure_macro, data, expression ) \
    static_cast< void >( expression ? 0 : ( failure_macro( \
        data, \
        ( BOOST_PP_STRINGIZE( expression ) ) \
        ( BOOST_PP_STRINGIZE( __FILE__ ) ) \
        ( BOOST_CURRENT_FUNCTION ) \
        ( __LINE__ ) \
        BOOST_PP_SEQ_FOR_EACH_R( r, \
            SAKE_ASSERT_RELATION_ANY_lhs_op_rhs, \
            ~, \
            relation_seq \
        ) \
    ), 0 ) )
#define SAKE_ASSERT_RELATION_ANY_lhs_op_rhs( r, data, elem ) \
    ( BOOST_PP_TUPLE_ELEM( 3, 0, elem ) ) \
    ( BOOST_PP_STRINGIZE( BOOST_PP_TUPLE_ELEM( 3, 1, elem ) ) ) \
    ( BOOST_PP_TUPLE_ELEM( 3, 2, elem ) )

/*******************************************************************************
 * SAKE_ASSERT helper macros
 ******************************************************************************/

#define SAKE_ASSERT_andor_subexpression( r, data, i, elem ) \
    BOOST_PP_EXPR_IF( i, data ) (elem)
#define SAKE_ASSERT_andor_lhs_op_rhs( r, data, i, elem ) \
    BOOST_PP_EXPR_IF( i, data ) \
    ((BOOST_PP_TUPLE_ELEM( 3, 0, elem )) \
      BOOST_PP_TUPLE_ELEM( 3, 1, elem ) \
     (BOOST_PP_TUPLE_ELEM( 3, 2, elem )))
#define SAKE_ASSERT_comma_lhs_op_rhs( r, data, elem ) \
    , BOOST_PP_TUPLE_ELEM( 3, 0, elem ) \
    , BOOST_PP_STRINGIZE( BOOST_PP_TUPLE_ELEM( 3, 1, elem ) ) \
    , BOOST_PP_TUPLE_ELEM( 3, 2, elem )

namespace sake
{

namespace assert_failure_action
{

namespace functional
{

namespace print_private
{

template< class LHS, class RHS >
struct dispatch_long;
template< class LHS, class RHS >
struct dispatch_ulong;
template< class LHS, class RHS >
struct dispatch_ldouble;

template<
    class LHS, class RHS,
    bool = dispatch_long< LHS, RHS >::value,
    bool = dispatch_ulong< LHS, RHS >::value,
    bool = dispatch_ldouble< LHS, RHS >::value
>
struct dispatch_builtin_type;

template<
    class LHS, class RHS,
    class T = typename dispatch_builtin_type< LHS, RHS >::type
>
struct dispatch;

} // namespace print_private

struct print
{
    typedef void result_type;

    void operator()(
        std::ostream& o,
        char const * const macro, char const * const expression,
        char const * const filename, char const * const function, unsigned int const line_number,
        char const * const message) const;

    void operator()(
        std::ostream& o,
        char const * const macro, char const * const expression,
        char const * const filename, char const * const function, unsigned int const line_number) const;

    void operator()(
        std::ostream& o,
        char const * const macro, char const * const expression,
        char const * const filename, char const * const function, unsigned int const line_number,
        unsigned int const subexpression_index, char const * const subexpression) const;

    template< class LHS, class RHS >
    void operator()(
        std::ostream& o,
        char const * const macro, char const * const expression,
        char const * const filename, char const * const function, unsigned int const line_number,
        unsigned int const subexpression_index, char const * const subexpression,
        LHS const & lhs, char const * const op, RHS const & rhs) const
    {
        print_private::dispatch< LHS, RHS >::apply(o,
            macro, expression, filename, function, line_number,
            subexpression_index, subexpression,
            lhs, op, rhs
        );
    }

#define BOOST_PP_ITERATION_LIMITS ( 1, SAKE_ASSERT_MAX_ARITY )
#define BOOST_PP_FILENAME_1 <sake/core/utility/assert.hpp>
#include BOOST_PP_ITERATE()

};

#define failure_action         abort
#define failure_action_abort() std::abort()
#define BOOST_PP_INDIRECT_SELF <sake/core/utility/assert.hpp>
#include BOOST_PP_INCLUDE_SELF()

#define failure_action         terminate
#define failure_action_abort() std::terminate()
#define BOOST_PP_INDIRECT_SELF <sake/core/utility/assert.hpp>
#include BOOST_PP_INCLUDE_SELF()

#define failure_action         exit
#define failure_action_abort() std::exit( EXIT_FAILURE )
#define BOOST_PP_INDIRECT_SELF <sake/core/utility/assert.hpp>
#include BOOST_PP_INCLUDE_SELF()

namespace print_private
{

template< class T >
void
apply_builtin(
    std::ostream& o,
    char const * const macro, char const * const expression,
    char const * const filename, char const * const function, unsigned int const line_number,
    T const lhs, char const * op, T const rhs);

template< class T >
void
apply_builtin(
    std::ostream& o,
    char const * const macro, char const * const expression,
    char const * const filename, char const * const function, unsigned int const line_number,
    unsigned int const subexpression_index, char const * const subexpression,
    T const lhs, char const * const op, T const rhs);

template< class LHS, class RHS >
struct dispatch_long
    : boost_ext::mpl::and6<
          boost::integral_constant< bool, (sizeof( LHS ) <= sizeof( long )) >,
          boost::integral_constant< bool, (sizeof( RHS ) <= sizeof( long )) >,
          boost::is_integral< LHS >,
          boost::is_integral< RHS >,
          boost::is_signed< LHS >,
          boost::is_signed< RHS >
      >
{ };

template< class LHS, class RHS >
struct dispatch_ulong
    : boost_ext::mpl::and6<
          boost::integral_constant< bool, (sizeof( LHS ) <= sizeof( unsigned long )) >,
          boost::integral_constant< bool, (sizeof( RHS ) <= sizeof( unsigned long )) >,
          boost::is_integral< LHS >,
          boost::is_integral< RHS >,
          boost::mpl::not_< boost::is_signed< LHS > >,
          boost::mpl::not_< boost::is_signed< RHS > >
      >
{ };

template< class LHS, class RHS >
struct dispatch_ldouble
    : boost_ext::mpl::and2<
          boost::is_floating_point< LHS >,
          boost::is_floating_point< RHS >
      >
{ };

template< class LHS, class RHS >
struct dispatch_builtin_type< LHS, RHS, false, false, false >
{ typedef void type; };
template< class LHS, class RHS >
struct dispatch_builtin_type< LHS, RHS, true, false, false >
{ typedef long type; };
template< class LHS, class RHS >
struct dispatch_builtin_type< LHS, RHS, false, true, false >
{ typedef unsigned long type; };
template< class LHS, class RHS >
struct dispatch_builtin_type< LHS, RHS, false, false, true >
{ typedef long double type; };

template< class LHS, class RHS >
struct dispatch< LHS, RHS, void >
{
    static void apply(
        std::ostream& o,
        char const * const macro, char const * const expression,
        char const * const filename, char const * const function, unsigned int const line_number,
        LHS const & lhs, char const * const op, RHS const & rhs)
    {
        std::ostringstream message(std::ostringstream::out);
        message << "{ " << expression << " } == "
                   "{ " << sake::make_ostreamable(lhs)
                        << ' ' << op << ' '
                        << sake::make_ostreamable(rhs) << " }";
        functional::print()(o,
            macro, expression, filename, function, line_number,
            message.str().c_str()
        );
    }

    static void apply(
        std::ostream& o,
        char const * const macro, char const * const expression,
        char const * const filename, char const * const function, unsigned int const line_number,
        unsigned int const subexpression_index, char const * const subexpression,
        LHS const & lhs, char const * const op, RHS const & rhs)
    {
        std::ostringstream message(std::ostringstream::out);
        message << "{ " << subexpression << " } == "
                   "{ " << sake::make_ostreamable(lhs)
                        << ' ' << op << ' '
                        << sake::make_ostreamable(rhs) << " } "
                   "(subexpression " << subexpression_index << " within { " << expression << " })";
        functional::print()(o,
            macro, expression, filename, function, line_number,
            message.str().c_str()
        );
    }
};

template< class LHS, class RHS, class T >
struct dispatch
{
    static void apply(
        std::ostream& o,
        char const * const macro, char const * const expression,
        char const * const filename, char const * const function, unsigned int const line_number,
        LHS const lhs, char const * const op, RHS const rhs)
    {
        apply_builtin<T>(o,
            macro, expression, filename, function, line_number,
            lhs, op, rhs
        );
    }

    static void apply(
        std::ostream& o,
        char const * const macro, char const * const expression,
        char const * const filename, char const * const function, unsigned int const line_number,
        unsigned int const subexpression_index, char const * const subexpression,
        LHS const lhs, char const * const op, RHS const rhs)
    {
        apply_builtin<T>(o,
            macro, expression, filename, function, line_number,
            subexpression_index, subexpression,
            lhs, op, rhs
        );
    }
};

} // namespace print_private

} // namespace functional

functional::print const print = { };
functional::abort const abort = { };
functional::terminate const terminate = { };
functional::exit const exit = { };

} // namespace assert_failure_action

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_ASSERT_HPP

#else // #ifndef BOOST_PP_IS_SELFISH

struct failure_action
{
    typedef void result_type;

    void operator()(
        std::ostream& o,
        char const * const macro, char const * const expression,
        char const * const filename, char const * const function, unsigned int const line_number) const;

    void operator()(
        std::ostream& o,
        char const * const macro, char const * const expression,
        char const * const filename, char const * const function, unsigned int const line_number,
        unsigned int const subexpression_index, char const * const subexpression) const;

    template< class LHS, class RHS >
    void operator()(
        std::ostream& o,
        char const * const macro, char const * const expression,
        char const * const filename, char const * const function, unsigned int const line_number,
        unsigned int const subexpression_index, char const * const subexpression,
        LHS const & lhs, char const * const op, RHS const & rhs) const
    {
        functional::print()(o,
            macro, expression, filename, function, line_number,
            subexpression_index, subexpression,
            lhs, op, rhs
        );
        failure_action_abort();
    }

#define BOOST_PP_ITERATION_LIMITS ( 1, SAKE_ASSERT_MAX_ARITY )
#define BOOST_PP_FILENAME_1 <sake/core/utility/assert.hpp>
#include BOOST_PP_ITERATE()

};

#undef failure_action
#undef failure_action_abort

#endif // #ifndef BOOST_PP_IS_SELFISH

#else // #ifndef BOOST_PP_IS_ITERATING

#define N BOOST_PP_ITERATION()

#define class_LHSn_class_RHSn( z, n, data ) class LHS ## n, class RHS ## n
#define LHSn_lhsn_opn_RHSn_rhsn( z, n, data ) \
    LHS ## n const & lhs ## n, \
    char const * const op ## n, \
    RHS ## n const & rhs ## n
#define or_stream_lhsn_opn_rhsn( z, n, data ) \
    BOOST_PP_EXPR_IF( n, << " || " ) \
    << sake::make_ostreamable( lhs ## n ) \
    << ' ' << op ## n << ' ' \
    << sake::make_ostreamable( rhs ## n )
#define lhsn_opn_rhsn( z, n, data ) lhs ## n, op ## n, rhs ## n

    template< BOOST_PP_ENUM( N, class_LHSn_class_RHSn, ~ ) >
    void operator()(
        std::ostream& o,
        char const * const macro, char const * const expression,
        char const * const filename, char const * const function, unsigned int const line_number,
        BOOST_PP_ENUM( N, LHSn_lhsn_opn_RHSn_rhsn, ~ )) const
    {

#ifdef failure_action

        functional::print()(o,
            macro, expression, filename, function, line_number,
            BOOST_PP_ENUM( N, lhsn_opn_rhsn, ~ )
        );
        failure_action_abort();

#else // #ifdef failure_action

#if N == 1
        print_private::dispatch< LHS0, RHS0 >::apply(o,
            macro, expression, filename, function, line_number,
            lhs0, op0, rhs0
        );
#else // #if N == 1
        std::ostringstream message(std::ostringstream::out);
        message << "{ " << expression << " } == "
                   "{ " BOOST_PP_REPEAT( N, or_stream_lhsn_opn_rhsn, ~ ) << " }";
        operator()(o,
            macro, expression, filename, function, line_number,
            message.str().c_str()
        );
#endif // #if N == 1

#endif // #ifdef failure_action

    }

#undef class_LHSn_class_RHSn
#undef LHSn_lhsn_opn_RHSn_rhsn
#undef or_stream_lhsn_opn_rhsn
#undef lhsn_opn_rhsn

#undef N

#endif // #ifndef BOOST_PP_IS_ITERATING

/*******************************************************************************
 * test/test.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * SAKE_TEST_XXX( env, expression )
 * SAKE_TEST_XXX_ALL( env, subexpression_seq )
 * SAKE_TEST_XXX_ANY( env, subexpression_seq )
 * SAKE_TEST_XXX_RELATION( env, lhs, op, rhs )
 * SAKE_TEST_XXX_RELATION_ALL( env, relation_seq )
 * SAKE_TEST_XXX_RELATION_ANY( env, relation_seq )
 * (where XXX in { WARN, CHECK, REQUIRE })
 *
 * Each macro evaluates the given boolean expression and fails (via a call to
 * env.fail) if the expression evaluates to false.  They should only be used
 * within a scope executed by a test::environment object.
 *
 * - SAKE_TEST_WARN should be used when a false expression does not
 * (necessarily) indicate an error but the host still wishes to signal a
 * warning.
 * - SAKE_TEST_CHECK should be used when a false expression indicates an error,
 * but subsequent execution is still possible.  In other words, the error is
 * recoverable.
 * - SAKE_TEST_REQUIRE should be used when a false expression indicates a
 * non-recoverable error.  A SAKE_TEST_REQUIRE failure throws an exception, thus
 * terminating the current test scope and notifying the enclosing
 * test::environment object.
 *
 * The macros other than SAKE_TEST_XXX are semantically equilavent to
 * SAKE_TEST_XXX.  They are only provided to allow one to easily log more
 * detailed information concerning the failure.
 ******************************************************************************/

#ifndef SAKE_TEST_TEST_HPP
#define SAKE_TEST_TEST_HPP

#include <boost/current_function.hpp>
#include <boost/preprocessor/control/expr_if.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/tuple/elem.hpp>

/*******************************************************************************
 * SAKE_TEST_XXX( env, expression )
 ******************************************************************************/

#define SAKE_TEST_WARN( env, expression ) \
    SAKE_TEST_XXX_impl( \
        env, \
        sake::test::environment::fail_level_warn, \
        "SAKE_TEST_WARN", expression \
    )
#define SAKE_TEST_CHECK( env, expression ) \
    SAKE_TEST_XXX_impl( \
        env, \
        sake::test::environment::fail_level_check, \
        "SAKE_TEST_CHECK", expression \
    )
#define SAKE_TEST_REQUIRE( env, expression ) \
    SAKE_TEST_XXX_impl( \
        env, \
        sake::test::environment::fail_level_require, \
        "SAKE_TEST_REQUIRE", expression \
    )

#define SAKE_TEST_XXX_impl( env, fail_level, macro, expression ) \
    static_cast< void >( ( expression ) ? 0 : ( \
        env.fail( \
            fail_level, macro, BOOST_PP_STRINGIZE( expression ), \
            BOOST_PP_STRINGIZE( __FILE__ ), BOOST_CURRENT_FUNCTION, __LINE__ \
        ), 0 \
    ) )

/*******************************************************************************
 * SAKE_TEST_XXX_ALL( env, subexpression_seq )
 ******************************************************************************/

#define SAKE_TEST_WARN_ALL( env, subexpression_seq ) \
    SAKE_TEST_XXX_ALL_impl( \
        env, \
        sake::test::environment::fail_level_warn, \
        "SAKE_TEST_WARN_ALL", subexpression_seq \
    )
#define SAKE_TEST_CHECK_ALL( env, subexpression_seq ) \
    SAKE_TEST_XXX_ALL_impl( \
        env, \
        sake::test::environment::fail_level_check, \
        "SAKE_TEST_CHECK_ALL", subexpression_seq \
    )
#define SAKE_TEST_REQUIRE_ALL( env, subexpression_seq ) \
    SAKE_TEST_XXX_ALL_impl( \
        env, \
        sake::test::environment::fail_level_require, \
        "SAKE_TEST_REQUIRE_ALL", subexpression_seq \
    )

#define SAKE_TEST_XXX_ALL_impl( env, fail_level, macro, subexpression_seq ) \
    SAKE_TEST_XXX_ALL_impl2( \
        env, fail_level, macro, \
        BOOST_PP_STRINGIZE( BOOST_PP_SEQ_FOR_EACH_I( \
            SAKE_TEST_XXX_andor_subexpression, \
            &&, \
            subexpression_seq \
        ) ), \
        subexpression_seq \
    )
#define SAKE_TEST_XXX_ALL_impl2( env, fail_level, macro, expression, subexpression_seq ) \
    static_cast< void >( BOOST_PP_SEQ_FOR_EACH_I( \
        SAKE_TEST_XXX_ALL_and_subexpression_fail, \
        ( env, fail_level, macro, expression ), \
        subexpression_seq \
    ) )
#define SAKE_TEST_XXX_ALL_and_subexpression_fail( r, data, i, elem ) \
    BOOST_PP_EXPR_IF( i, && ) \
    ( ( elem ) ? true : ( BOOST_PP_TUPLE_ELEM( 4, 0, data ) .fail( \
        BOOST_PP_TUPLE_ELEM( 4, 1, data ), \
        BOOST_PP_TUPLE_ELEM( 4, 2, data ), \
        BOOST_PP_TUPLE_ELEM( 4, 3, data ), \
        BOOST_PP_STRINGIZE( __FILE__ ), \
        BOOST_CURRENT_FUNCTION, \
        __LINE__, \
        i, \
        BOOST_PP_STRINGIZE( elem ) \
    ), false ) )

/*******************************************************************************
 * SAKE_TEST_XXX_ANY( env, subexpression_seq )
 ******************************************************************************/

#define SAKE_TEST_WARN_ANY( env, subexpression_seq ) \
    SAKE_TEST_XXX_ANY_impl( \
        env, \
        sake::test::environment::fail_level_warn, \
        "SAKE_TEST_WARN_ANY", subexpression_seq \
    )
#define SAKE_TEST_CHECK_ANY( env, subexpression_seq ) \
    SAKE_TEST_XXX_ANY_impl( \
        env, \
        sake::test::environment::fail_level_check, \
        "SAKE_TEST_CHECK_ANY", subexpression_seq \
    )
#define SAKE_TEST_REQUIRE_ANY( env, subexpression_seq ) \
    SAKE_TEST_XXX_ANY_impl( \
        env, \
        sake::test::environment::fail_level_require, \
        "SAKE_TEST_REQUIRE_ANY", subexpression_seq \
    )

#define SAKE_TEST_XXX_ANY_impl( env, fail_level, macro, subexpression_seq ) \
    SAKE_TEST_XXX_impl( \
        env, fail_level, macro, \
        BOOST_PP_SEQ_FOR_EACH_I( \
            SAKE_TEST_XXX_andor_subexpression, \
            ||, \
            subexpression_seq \
        ) \
    )

/*******************************************************************************
 * SAKE_TEST_XXX_RELATION( env, lhs, op, rhs )
 ******************************************************************************/

#define SAKE_TEST_WARN_RELATION( env, lhs, op, rhs ) \
    SAKE_TEST_XXX_RELATION_impl( \
        env, \
        sake::test::environment::fail_level_warn, \
        "SAKE_TEST_WARN_RELATION", lhs, op, rhs \
    )
#define SAKE_TEST_CHECK_RELATION( env, lhs, op, rhs ) \
    SAKE_TEST_XXX_RELATION_impl( \
        env, \
        sake::test::environment::fail_level_check, \
        "SAKE_TEST_CHECK_RELATION", lhs, op, rhs \
    )
#define SAKE_TEST_REQUIRE_RELATION( env, lhs, op, rhs ) \
    SAKE_TEST_XXX_RELATION_impl( \
        env, \
        sake::test::environment::fail_level_require, \
        "SAKE_TEST_REQUIRE_RELATION", lhs, op, rhs \
    )

#define SAKE_TEST_XXX_RELATION_impl( env, fail_level, macro, lhs, op, rhs ) \
    static_cast< void >( ( lhs op rhs ) ? 0 : ( \
        env.fail( \
            fail_level, macro, BOOST_PP_STRINGIZE( lhs op rhs ), \
            BOOST_PP_STRINGIZE( __FILE__ ), BOOST_CURRENT_FUNCTION, __LINE__, \
            lhs, BOOST_PP_STRINGIZE( op ), rhs \
        ), 0 \
    ) )

/*******************************************************************************
 * SAKE_TEST_XXX_RELATION_ALL( env, relation_seq )
 ******************************************************************************/

#define SAKE_TEST_WARN_RELATION_ALL( env, relation_seq ) \
    SAKE_TEST_XXX_RELATION_ALL_impl( \
        env, \
        sake::test::environment::fail_level_warn, \
        "SAKE_TEST_WARN_RELATION_ALL", relation_seq \
    )
#define SAKE_TEST_CHECK_RELATION_ALL( env, relation_seq ) \
    SAKE_TEST_XXX_RELATION_ALL_impl( \
        env, \
        sake::test::environment::fail_level_check, \
        "SAKE_TEST_CHECK_RELATION_ALL", relation_seq \
    )
#define SAKE_TEST_REQUIRE_RELATION_ALL( env, relation_seq ) \
    SAKE_TEST_XXX_RELATION_ALL_impl( \
        env, \
        sake::test::environment::fail_level_require, \
        "SAKE_TEST_REQUIRE_RELATION_ALL", relation_seq \
    )

#define SAKE_TEST_XXX_RELATION_ALL_impl( env, fail_level, macro, relation_seq ) \
    SAKE_TEST_XXX_RELATION_ALL_impl2( \
        env, fail_level, macro, \
        BOOST_PP_STRINGIZE( BOOST_PP_SEQ_FOR_EACH_I( \
            SAKE_TEST_XXX_RELATION_andor_lhs_op_rhs, \
            &&, \
            relation_seq \
        ) ), \
        relation_seq \
    )
#define SAKE_TEST_XXX_RELATION_ALL_impl2( env, fail_level, macro, expression, relation_seq ) \
    static_cast< void >( BOOST_PP_SEQ_FOR_EACH_I( \
        SAKE_TEST_XXX_RELATION_ALL_and_relation_fail, \
        ( env, fail_level, macro, expression ), \
        relation_seq \
    ) )
#define SAKE_TEST_XXX_RELATION_ALL_and_relation_fail( r, data, i, elem ) \
    SAKE_TEST_XXX_RELATION_ALL_and_relation_fail_impl( \
        BOOST_PP_TUPLE_ELEM( 4, 0, data ), \
        BOOST_PP_TUPLE_ELEM( 4, 1, data ), \
        BOOST_PP_TUPLE_ELEM( 4, 2, data ), \
        BOOST_PP_TUPLE_ELEM( 4, 3, data ), \
        i, \
        BOOST_PP_TUPLE_ELEM( 3, 0, elem ), \
        BOOST_PP_TUPLE_ELEM( 3, 1, elem ), \
        BOOST_PP_TUPLE_ELEM( 3, 2, elem ) \
    )
#define SAKE_TEST_XXX_RELATION_ALL_and_relation_fail_impl( env, fail_level, macro, expression, i, lhs, op, rhs ) \
    BOOST_PP_EXPR_IF( i, && ) \
    ( ( lhs op rhs ) ? true : ( env .fail( \
        fail_level, macro, expression, \
        BOOST_PP_STRINGIZE( __FILE__ ), BOOST_CURRENT_FUNCTION, __LINE__, \
        i, BOOST_PP_STRINGIZE( lhs op rhs ), \
        lhs, BOOST_PP_STRINGIZE( op ), rhs \
    ), false ) )

/*******************************************************************************
 * SAKE_TEST_XXX_RELATION_ANY( env, relation_seq )
 ******************************************************************************/

#define SAKE_TEST_WARN_RELATION_ANY( env, relation_seq ) \
    SAKE_TEST_XXX_RELATION_ANY_impl( \
        env, \
        sake::test::environment::fail_level_warn, \
        "SAKE_TEST_WARN_RELATION_ANY", relation_seq \
    )
#define SAKE_TEST_CHECK_RELATION_ANY( env, relation_seq ) \
    SAKE_TEST_XXX_RELATION_ANY_impl( \
        env, \
        sake::test::environment::fail_level_check, \
        "SAKE_TEST_CHECK_RELATION_ANY", relation_seq \
    )
#define SAKE_TEST_REQUIRE_RELATION_ANY( env, relation_seq ) \
    SAKE_TEST_XXX_RELATION_ANY_impl( \
        env, \
        sake::test::environment::fail_level_require, \
        "SAKE_TEST_REQUIRE_RELATION_ANY", relation_seq \
    )

#define SAKE_TEST_XXX_RELATION_ANY_impl( env, fail_level, macro, relation_seq ) \
    SAKE_TEST_XXX_RELATION_ANY_impl2( \
        env, fail_level, macro, \
        BOOST_PP_SEQ_FOR_EACH_I( \
            SAKE_TEST_XXX_RELATION_andor_lhs_op_rhs, \
            ||, \
            relation_seq \
        ), \
        relation_seq \
    )
#define SAKE_TEST_XXX_RELATION_ANY_impl2( env, fail_level, macro, expression, relation_seq ) \
    static_cast< void >( ( expression ) ? 0 : ( env .fail( \
        fail_level, macro, BOOST_PP_STRINGIZE( expression ), \
        BOOST_PP_STRINGIZE( __FILE__ ), BOOST_CURRENT_FUNCTION, __LINE__ \
        BOOST_PP_SEQ_FOR_EACH( SAKE_TEST_XXX_RELATION_comma_lhs_op_rhs, ~, relation_seq ) \
    ), 0 ) )

/*******************************************************************************
 * SAKE_TEST_XXX helper macros
 ******************************************************************************/

#define SAKE_TEST_XXX_andor_subexpression( r, data, i, elem ) \
    BOOST_PP_EXPR_IF( i, data ) elem
#define SAKE_TEST_XXX_RELATION_andor_lhs_op_rhs( r, data, i, elem ) \
    BOOST_PP_EXPR_IF( i, data ) \
    BOOST_PP_TUPLE_ELEM( 3, 0, elem ) \
    BOOST_PP_TUPLE_ELEM( 3, 1, elem ) \
    BOOST_PP_TUPLE_ELEM( 3, 2, elem )
#define SAKE_TEST_XXX_RELATION_comma_lhs_op_rhs( r, data, elem ) \
    , BOOST_PP_TUPLE_ELEM( 3, 0, elem ) \
    , BOOST_PP_STRINGIZE( BOOST_PP_TUPLE_ELEM( 3, 1, elem ) ) \
    , BOOST_PP_TUPLE_ELEM( 3, 2, elem )

#endif // #ifndef SAKE_TEST_TEST_HPP

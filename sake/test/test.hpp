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
 * but subsequent execution is still possible.  In other words, the error is not
 * critical.
 * - SAKE_TEST_REQUIRE should be used when a false expression indicates a
 * critical, non-recoverable error.  A SAKE_TEST_REQUIRE failure throws an
 * exception, thus terminating the current test scope and notifying the
 * enclosing test::environment object.
 *
 * The macros other than SAKE_TEST_XXX are semantically equilavent to
 * SAKE_TEST_XXX.  They are only provided to allow one to easily log more
 * detailed information concerning the failure.
 ******************************************************************************/

#ifndef SAKE_TEST_TEST_HPP
#define SAKE_TEST_TEST_HPP

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/seq/enum.hpp>
#include <boost/preprocessor/tuple/elem.hpp>

#include <sake/core/utility/assert.hpp>

#define SAKE_TEST_FAILURE_MACRO( data, info_seq ) \
    BOOST_PP_TUPLE_ELEM( 3, 0, data ) .fail( \
        BOOST_PP_CAT( \
            ::sake::test::environment::fail_level_, \
            BOOST_PP_TUPLE_ELEM( 3, 1, data ) \
        ), \
        BOOST_PP_TUPLE_ELEM( 3, 2, data ), \
        BOOST_PP_SEQ_ENUM( info_seq ) \
    )

/*******************************************************************************
 * SAKE_TEST_XXX( env, expression )
 ******************************************************************************/

#define SAKE_TEST_WARN( env, expression ) \
    SAKE_ASSERT_EXT( \
        expression, \
        SAKE_TEST_FAILURE_MACRO, \
        ( env, warn, "SAKE_TEST_WARN" ) \
    )
#define SAKE_TEST_CHECK( env, expression ) \
    SAKE_ASSERT_EXT( \
        expression, \
        SAKE_TEST_FAILURE_MACRO, \
        ( env, check, "SAKE_TEST_CHECK" ) \
    )
#define SAKE_TEST_REQUIRE( env, expression ) \
    SAKE_ASSERT_EXT( \
        expression, \
        SAKE_TEST_FAILURE_MACRO, \
        ( env, require, "SAKE_TEST_REQUIRE" ) \
    )

/*******************************************************************************
 * SAKE_TEST_XXX_ALL( env, subexpression_seq )
 ******************************************************************************/

#define SAKE_TEST_WARN_ALL( env, subexpression_seq ) \
    SAKE_ASSERT_EXT_ALL( \
        subexpression_seq, \
        SAKE_TEST_FAILURE_MACRO, \
        ( env, warn, "SAKE_TEST_WARN_ALL" ) \
    )
#define SAKE_TEST_CHECK_ALL( env, subexpression_seq ) \
    SAKE_ASSERT_EXT_ALL( \
        subexpression_seq, \
        SAKE_TEST_FAILURE_MACRO, \
        ( env, check, "SAKE_TEST_CHECK_ALL" ) \
    )
#define SAKE_TEST_REQUIRE_ALL( env, subexpression_seq ) \
    SAKE_ASSERT_EXT_ALL( \
        subexpression_seq, \
        SAKE_TEST_FAILURE_MACRO, \
        ( env, require, "SAKE_TEST_REQUIRE_ALL" ) \
    )

/*******************************************************************************
 * SAKE_TEST_XXX_ANY( env, subexpression_seq )
 ******************************************************************************/

#define SAKE_TEST_WARN_ANY( env, subexpression_seq ) \
    SAKE_ASSERT_EXT_ANY( \
        subexpression_seq, \
        SAKE_TEST_FAILURE_MACRO, \
        ( env, warn, "SAKE_TEST_WARN_ANY" ) \
    )
#define SAKE_TEST_CHECK_ANY( env, subexpression_seq ) \
    SAKE_ASSERT_EXT_ANY( \
        subexpression_seq, \
        SAKE_TEST_FAILURE_MACRO, \
        ( env, check, "SAKE_TEST_CHECK_ANY" ) \
    )
#define SAKE_TEST_REQUIRE_ANY( env, subexpression_seq ) \
    SAKE_ASSERT_EXT_ANY( \
        subexpression_seq, \
        SAKE_TEST_FAILURE_MACRO, \
        ( env, require, "SAKE_TEST_REQUIRE_ANY" ) \
    )

/*******************************************************************************
 * SAKE_TEST_XXX_RELATION( env, lhs, op, rhs )
 ******************************************************************************/

#define SAKE_TEST_WARN_RELATION( env, lhs, op, rhs ) \
    SAKE_ASSERT_EXT_RELATION( \
        lhs, op, rhs, \
        SAKE_TEST_FAILURE_MACRO, \
        ( env, warn, "SAKE_TEST_WARN_RELATION" ) \
    )
#define SAKE_TEST_CHECK_RELATION( env, lhs, op, rhs ) \
    SAKE_ASSERT_EXT_RELATION( \
        lhs, op, rhs, \
        SAKE_TEST_FAILURE_MACRO, \
        ( env, check, "SAKE_TEST_CHECK_RELATION" ) \
    )
#define SAKE_TEST_REQUIRE_RELATION( env, lhs, op, rhs ) \
    SAKE_ASSERT_EXT_RELATION( \
        lhs, op, rhs, \
        SAKE_TEST_FAILURE_MACRO, \
        ( env, require, "SAKE_TEST_REQUIRE_RELATION" ) \
    )

/*******************************************************************************
 * SAKE_TEST_XXX_RELATION_ALL( env, relation_seq )
 ******************************************************************************/

#define SAKE_TEST_WARN_RELATION_ALL( env, relation_seq ) \
    SAKE_ASSERT_EXT_RELATION_ALL( \
        relation_seq, \
        SAKE_TEST_FAILURE_MACRO, \
        ( env, warn, "SAKE_TEST_WARN_RELATION_ALL" ) \
    )
#define SAKE_TEST_CHECK_RELATION_ALL( env, relation_seq ) \
    SAKE_ASSERT_EXT_RELATION_ALL( \
        relation_seq, \
        SAKE_TEST_FAILURE_MACRO, \
        ( env, check, "SAKE_TEST_CHECK_RELATION_ALL" ) \
    )
#define SAKE_TEST_REQUIRE_RELATION_ALL( env, relation_seq ) \
    SAKE_ASSERT_EXT_RELATION_ALL( \
        relation_seq, \
        SAKE_TEST_FAILURE_MACRO, \
        ( env, require, "SAKE_TEST_REQUIRE_RELATION_ALL" ) \
    )

/*******************************************************************************
 * SAKE_TEST_XXX_RELATION_ANY( env, relation_seq )
 ******************************************************************************/

#define SAKE_TEST_WARN_RELATION_ANY( env, relation_seq ) \
    SAKE_ASSERT_EXT_RELATION_ANY( \
        relation_seq, \
        SAKE_TEST_FAILURE_MACRO, \
        ( env, warn, "SAKE_TEST_WARN_RELATION_ANY" ) \
    )
#define SAKE_TEST_CHECK_RELATION_ANY( env, relation_seq ) \
    SAKE_ASSERT_EXT_RELATION_ANY( \
        relation_seq, \
        SAKE_TEST_FAILURE_MACRO, \
        ( env, check, "SAKE_TEST_CHECK_RELATION_ANY" ) \
    )
#define SAKE_TEST_REQUIRE_RELATION_ANY( env, relation_seq ) \
    SAKE_ASSERT_EXT_RELATION_ANY( \
        relation_seq, \
        SAKE_TEST_FAILURE_MACRO, \
        ( env, require, "SAKE_TEST_REQUIRE_RELATION_ANY" ) \
    )

#endif // #ifndef SAKE_TEST_TEST_HPP

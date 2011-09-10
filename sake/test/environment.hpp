/*******************************************************************************
 * test/environment.hpp
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
 * struct test::environment
 *
 * This handles the testing environment for Sake's unit tests.
 ******************************************************************************/

#ifndef BOOST_PP_IS_ITERATING

#ifndef SAKE_TEST_ENVIRONMENT_HPP
#define SAKE_TEST_ENVIRONMENT_HPP

#include <cstring>

#include <sstream>
#include <string>

#include <boost/current_function.hpp>
#include <boost/preprocessor/control/expr_if.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/tuple/elem.hpp>

#include <sake/core/utility/ostreamable.hpp>

#ifndef SAKE_TEST_ENVIRONMENT_MAX_ARITY
#define SAKE_TEST_ENVIRONMENT_MAX_ARITY 8
#endif // #ifndef SAKE_TEST_ENVIRONMENT_MAX_ARITY

namespace sake
{

namespace test
{

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
        ), \
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
        BOOST_PP_STRINGIZE( __FILE__ ), BOOST_CURRENT_FUNCTION, __LINE__, \
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

/*******************************************************************************
 * struct environment
 ******************************************************************************/

struct environment
{
    enum e_fail_level
    {
        fail_level_warn,
        fail_level_check,
        fail_level_require
    };
    enum e_log_level
    {
        log_level_cross_scope = 0,
        log_level_warn,
        log_level_check,
        log_level_require,
        log_level_exception
    };

    environment();
    environment(environment const & other);
    ~environment();

    environment& operator=(environment other);
    void swap(environment& other);

    void parse_command_line(int argc, char* argv[]);

    void operator()(char const* local_scope_name, void (*p_f)( environment& ));
    void operator()(char const* local_scope_name, void (*p_f)( environment&, void* ), void* p);

    void fail(
        e_fail_level fail_level,
        char const * macro, char const * expression,
        char const * filename, char const * function, unsigned int line_number);
    void fail(
        e_fail_level fail_level,
        char const * macro, char const * expression,
        char const * filename, char const * function, unsigned int line_number,
        unsigned int subexpression_index, char const * subexpression);
    template< class LHS, class RHS >
    void fail(
        e_fail_level fail_level,
        char const * macro, char const * expression,
        char const * filename, char const * function, unsigned int line_number,
        LHS const & lhs, char const * op, RHS const & rhs);
    template< class LHS, class RHS >
    void fail(
        e_fail_level fail_level,
        char const * macro, char const * expression,
        char const * filename, char const * function, unsigned int line_number,
        unsigned int subexpression_index, char const * subexpression,
        LHS const & lhs, char const * op, RHS const & rhs);
#if SAKE_TEST_ENVIRONMENT_MAX_ARITY >= 2
    // template< class LHS0, class RHS0, ... >
    // void fail(
    //     e_fail_level fail_level,
    //     char const * macro, char const * expression,
    //     char const * filename, char const * function, unsigned int line_number,
    //     LHS0 const & lhs0, char const * op0, RHS0 const & rhs0,
    //     ...);
#define BOOST_PP_ITERATION_LIMITS ( 2, SAKE_TEST_ENVIRONMENT_MAX_ARITY )
#define BOOST_PP_FILENAME_1 <sake/test/environment.hpp>
#include BOOST_PP_ITERATE()
#endif // #if SAKE_TEST_ENVIRONMENT_MAX_ARITY >= 2

    int main_return_value() const;

private:
    struct impl;
    impl* mp_impl;

    static void apply(environment& this_, void* p_f)
    { (*static_cast< void (*)( environment& ) >(p_f))(this_); }

    void fail(
        e_fail_level fail_level,
        char const * macro, char const * expression,
        char const * filename, char const * function, unsigned int line_number,
        char const * message);
};

/*******************************************************************************
 * environment (inline) member function implementations
 ******************************************************************************/

inline void
environment::
operator()(char const * local_scope_name, void (*p_f)( environment& ))
{ operator()(local_scope_name, &apply, static_cast< void* >(p_f)); }

inline void
environment::
fail(
    e_fail_level fail_level,
    char const * macro, char const * expression,
    char const * filename, char const * function, unsigned int line_number)
{
    std::string message;
    message.reserve(2 + std::strlen(expression) + 2);
    message += "{ ";
    message += expression;
    message += " }";
    fail(
        fail_level, macro, expression,
        filename, function, line_number,
        message.c_str()
    );
}

inline void
environment::
fail(
    e_fail_level fail_level,
    char const * macro, char const * expression,
    char const * filename, char const * function, unsigned int line_number,
    unsigned int subexpression_index, char const * subexpression)
{
    std::ostringstream o(std::ostringstream::out);
    o << "{ " << subexpression << " } "
         "(subexpression " << subexpression_index << " within { " << expression << " })";
    fail(
        fail_level, macro, expression,
        filename, function, line_number,
        o.str().c_str()
    );
}

template< class LHS, class RHS >
inline void
environment::
fail(
    e_fail_level fail_level,
    char const * macro, char const * expression,
    char const * filename, char const * function, unsigned int line_number,
    LHS const & lhs, char const * op, RHS const & rhs)
{
    std::ostringstream o(std::ostringstream::out);
    o << "{ " << expression << " } == "
         "{ " << sake::make_ostreamable(lhs) << ' ' << op << ' ' << sake::make_ostreamable(rhs) << " }";
    fail(
        fail_level, macro, expression,
        filename, function, line_number,
        o.str().c_str()
    );
}

template< class LHS, class RHS >
inline void
environment::
fail(
    e_fail_level fail_level,
    char const * macro, char const * expression,
    char const * filename, char const * function, unsigned int line_number,
    unsigned int subexpression_index, char const * subexpression,
    LHS const & lhs, char const * op, RHS const & rhs)
{
    std::ostringstream o(std::ostringstream::out);
    o << "{ " << subexpression << " } == "
         "{ " << sake::make_ostreamable(lhs) << ' ' << op << ' ' << sake::make_ostreamable(rhs) << " } "
         "(subexpression " << subexpression_index << " within { " << expression << " })";
    fail(
        fail_level, macro, expression,
        filename, function, line_number,
        o.str().c_str()
    );
}

} // namespace test

} // namespace sake

#endif // #ifndef SAKE_TEST_ENVIRONMENT_HPP

#else // #ifndef BOOST_PP_IS_ITERATING

#define n BOOST_PP_ITERATION()

#define class_LHSn_class_RHSn( z, n, data ) class LHS ## n, class RHS ## n
#define LHSn_lhsn_opn_RHSn_rhsn( z, n, data ) \
    LHS ## n const & lhs ## n, \
    char const * const op ## n, \
    RHS ## n const & rhs ## n
#define or_if_stream_lhsn_opn_rhsn( z, n, data ) \
    << BOOST_PP_EXPR_IF( n, " || " << ) \
    sake::make_ostreamable( lhs ## n ) << ' ' << op ## n << ' ' << sake::make_ostreamable( rhs ## n )

    template< BOOST_PP_ENUM( n, class_LHSn_class_RHSn, ~ ) >
    void fail(
        e_fail_level fail_level,
        char const * macro, char const * expression,
        char const * filename, char const * function, unsigned int line_number,
        BOOST_PP_ENUM( n, LHSn_lhsn_opn_RHSn_rhsn, ~ ) )
    {
        std::ostringstream o(std::ostringstream::out);
        o << "{ " << expression << " } == "
             "{ " << BOOST_PP_REPEAT( n, or_if_stream_lhsn_opn_rhsn, ~ ) << " }";
        fail(
            fail_level, macro, expression,
            filename, function, line_number,
            o.str().c_str()
        );
    }

#undef class_LHSn_class_RHSn
#undef LHSn_lhsn_opn_RHSn_rhsn
#undef or_if_stream_lhsn_opn_rhsn

#undef n

#endif // #ifndef BOOST_PP_IS_ITERATING

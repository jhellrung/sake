/*******************************************************************************
 * test/environment.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct test::environment
 *
 * SAKE_TEST_XXX( env, expression )
 * SAKE_TEST_XXX_RELATION( env, lhs, op, rhs )
 * (where XXX in { WARN, CHECK, REQUIRE })
 *
 * This handles the testing environment for Sake's unit tests.
 ******************************************************************************/

#ifndef SAKE_TEST_ENVIRONMENT_HPP
#define SAKE_TEST_ENVIRONMENT_HPP

#include <cstring>

#include <sstream>
#include <string>

#include <boost/current_function.hpp>
// #include <boost/preprocessor/control/expr_if.hpp>
// #include <boost/preprocessor/iteration/iterate.hpp>
// #include <boost/preprocessor/iteration/local.hpp>
// #include <boost/preprocessor/repetition/enum.hpp>
// #include <boost/preprocessor/repetition/repeat.hpp>
// #include <boost/preprocessor/seq/for_each.hpp>
// #include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/stringize.hpp>
// #include <boost/preprocessor/tuple/elem.hpp>

#include <sake/core/utility/ostreamable.hpp>

namespace sake
{

namespace test
{

/*******************************************************************************
 * SAKE_TEST_XXX( env, expression )
 ******************************************************************************/

#define SAKE_TEST_WARN( env, expression ) \
    SAKE_TEST_XXX( \
        env, \
        sake::test::environment::fail_level_warn, \
        "SAKE_TEST_WARN", expression \
    )
#define SAKE_TEST_CHECK( env, expression ) \
    SAKE_TEST_XXX( \
        env, \
        sake::test::environment::fail_level_check, \
        "SAKE_TEST_CHECK", expression \
    )
#define SAKE_TEST_REQUIRE( env, expression ) \
    SAKE_TEST_XXX( \
        env, \
        sake::test::environment::fail_level_require, \
        "SAKE_TEST_REQUIRE", expression \
    )

#define SAKE_TEST_XXX( env, fail_level, macro, expression ) \
    static_cast< void >( ( expression ) ? 0 : ( \
        env.fail( \
            fail_level, \
            macro, BOOST_PP_STRINGIZE( expression ), \
            BOOST_PP_STRINGIZE( __FILE__ ), BOOST_CURRENT_FUNCTION, __LINE__ \
        ), 0 \
    ) )

/*******************************************************************************
 * SAKE_TEST_XXX_RELATION( env, lhs, op, rhs )
 ******************************************************************************/

#define SAKE_TEST_WARN_RELATION( env, lhs, op, rhs ) \
    SAKE_TEST_XXX_RELATION( \
        env, \
        sake::test::environment::fail_level_warn, \
        "SAKE_TEST_WARN_RELATION", lhs, op, rhs \
    )
#define SAKE_TEST_CHECK_RELATION( env, lhs, op, rhs ) \
    SAKE_TEST_XXX_RELATION( \
        env, \
        sake::test::environment::fail_level_check, \
        "SAKE_TEST_CHECK_RELATION", lhs, op, rhs \
    )
#define SAKE_TEST_REQUIRE_RELATION( env, lhs, op, rhs ) \
    SAKE_TEST_XXX_RELATION( \
        env, \
        sake::test::environment::fail_level_require, \
        "SAKE_TEST_REQUIRE_RELATION", lhs, op, rhs \
    )

#define SAKE_TEST_XXX_RELATION( env, fail_level, macro, lhs, op, rhs ) \
    static_cast< void >( ( lhs op rhs ) ? 0 : ( \
        env.fail( \
            fail_level, \
            macro, BOOST_PP_STRINGIZE( lhs op rhs ), \
            BOOST_PP_STRINGIZE( __FILE__ ), BOOST_CURRENT_FUNCTION, __LINE__, \
            lhs, BOOST_PP_STRINGIZE( op ), rhs \
        ), 0 \
    ) )

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
        fail_level,
        macro, expression,
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
        fail_level,
        macro, expression,
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
        fail_level,
        macro, expression,
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
        fail_level,
        macro, expression,
        filename, function, line_number,
        o.str().c_str()
    );
}

} // namespace test

} // namespace sake

#endif // #ifndef SAKE_TEST_ENVIRONMENT_HPP

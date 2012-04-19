/*******************************************************************************
 * sake/test/environment.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct test::environment
 *
 * This handles the testing environment for Sake's unit tests.
 ******************************************************************************/

#ifndef BOOST_PP_IS_ITERATING

#ifndef SAKE_TEST_ENVIRONMENT_HPP
#define SAKE_TEST_ENVIRONMENT_HPP

#include <cstring>

#include <iosfwd>
#include <sstream>
#include <string>

#include <boost/preprocessor/control/expr_if.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>

#include <sake/core/utility/assert.hpp>

#ifndef SAKE_TEST_ENVIRONMENT_MAX_ARITY
#define SAKE_TEST_ENVIRONMENT_MAX_ARITY 8
#endif // #ifndef SAKE_TEST_ENVIRONMENT_MAX_ARITY

namespace sake
{

namespace test
{

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
        log_level_info = 0,
        log_level_cross_scope,
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

    std::ostream& log() const;
    unsigned int current_log_level() const;
    unsigned int current_depth() const;

    void report() const;
    int main_return_value() const;

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
        unsigned int subexpression_index, char const * subexpression,
        LHS const & lhs, char const * op, RHS const & rhs);
    // template< class LHS0, class RHS0, ... >
    // void fail(
    //     e_fail_level fail_level,
    //     char const * macro, char const * expression,
    //     char const * filename, char const * function, unsigned int line_number,
    //     LHS0 const & lhs0, char const * op0, RHS0 const & rhs0,
    //     ...);
#define BOOST_PP_ITERATION_LIMITS ( 1, SAKE_TEST_ENVIRONMENT_MAX_ARITY )
#define BOOST_PP_FILENAME_1 <sake/test/environment.hpp>
#include BOOST_PP_ITERATE()

private:
    struct impl;
    impl* mp_impl;

    static void apply(environment& this_, void* p_f)
    { (**static_cast< void (**)( environment& ) >(p_f))(this_); }

    void fail(e_fail_level fail_level, char const * message);
};

/*******************************************************************************
 * environment (inline) member definitions
 ******************************************************************************/

inline void
environment::
operator()(char const * local_scope_name, void (*p_f)( environment& ))
{ operator()(local_scope_name, &apply, static_cast< void* >(&p_f)); }

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
    std::ostringstream message(std::ostringstream::out);
    sake::assert_failure_action::print(
        message,
        macro, expression, filename, function, line_number,
        subexpression_index, subexpression,
        lhs, op, rhs
    );
    fail(fail_level, message.str().c_str());
}

} // namespace test

} // namespace sake

#endif // #ifndef SAKE_TEST_ENVIRONMENT_HPP

#else // #ifndef BOOST_PP_IS_ITERATING

#define N BOOST_PP_ITERATION()

#define class_LHSn_class_RHSn( z, n, data ) class LHS ## n, class RHS ## n
#define LHSn_lhsn_opn_RHSn_rhsn( z, n, data ) \
    LHS ## n const & lhs ## n, \
    char const * const op ## n, \
    RHS ## n const & rhs ## n
#define lhsn_opn_rhsn( z, n, data ) lhs ## n, op ## n, rhs ## n

    template< BOOST_PP_ENUM( N, class_LHSn_class_RHSn, ~ ) >
    void fail(
        e_fail_level fail_level,
        char const * macro, char const * expression,
        char const * filename, char const * function, unsigned int line_number,
        BOOST_PP_ENUM( N, LHSn_lhsn_opn_RHSn_rhsn, ~ ) )
    {
        std::ostringstream message(std::ostringstream::out);
        sake::assert_failure_action::print(
            message,
            macro, expression, filename, function, line_number,
            BOOST_PP_ENUM( N, lhsn_opn_rhsn, ~ )
        );
        fail(fail_level, message.str().c_str());
    }

#undef class_LHSn_class_RHSn
#undef LHSn_lhsn_opn_RHSn_rhsn
#undef lhsn_opn_rhsn

#undef N

#endif // #ifndef BOOST_PP_IS_ITERATING

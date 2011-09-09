/*******************************************************************************
 * test/environment.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct test::environment
 * struct test::environment::scope_error
 *
 * This handles the testing environment for Sake's unit tests.
 ******************************************************************************/

#ifndef SAKE_TEST_ENVIRONMENT_HPP
#define SAKE_TEST_ENVIRONMENT_HPP

#include <cstring>

#include <sstream>
#include <string>

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
        unsigned int condition_index, char const * condition);
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
        unsigned int condition_index, char const * condition,
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
    unsigned int condition_index, char const * condition)
{
    std::ostringstream o(std::ostringstream::out);
    o << "{ " << condition << " } "
         "(condition " << condition_index << " within { " << expression << " })";
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
    o << "{ " << expression << " } == { " << lhs << ' ' << op << ' ' << rhs << " }";
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
    unsigned int condition_index, char const * condition,
    LHS const & lhs, char const * op, RHS const & rhs)
{
    std::ostringstream o(std::ostringstream::out);
    o << "{ " << condition << " } == { " << lhs << ' ' << op << ' ' << rhs << " } "
         "(condition " << condition_index << " within { " << expression << " })";
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

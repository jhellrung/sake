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

namespace sake
{

namespace test
{

struct environment
{
    environment();
    environment(environment const & other);
    ~environment();

    environment& operator=(environment other);
    void swap(environment& other);

    void parse_command_line(int argc, char* argv[]);

    void operator()(char const* p_local_scope, void (*p_f)( environment& ));
    void operator()(char const* p_local_scope, void (*p_f)( environment&, void* ), void* p);

    int main_return_value() const;

private:
    struct impl;
    impl* mp_impl;

    static void apply(environment& this_, void* p_f)
    { (*static_cast< void (*)( environment& ) >(p_f))(this_); }
};

/*******************************************************************************
 * environment (inline) member function implementations
 ******************************************************************************/

inline void
environment::
operator()(char const * p_local_scope, void (*p_f)( environment& ))
{ operator()(p_local_scope, &apply, static_cast< void* >(p_f)); }

} // namespace test

} // namespace sake

#endif // #ifndef SAKE_TEST_ENVIRONMENT_HPP

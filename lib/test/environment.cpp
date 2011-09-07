/*******************************************************************************
 * lib/test/environment.cpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <cassert>

#include <algorithm>
#include <iostream>
#include <list>

#include <sake/test/environment.hpp>

namespace sake
{

namespace test
{

/*******************************************************************************
 * struct environment::impl
 ******************************************************************************/

struct environment::impl
{
    std::ostream* p_out;
    std::list< char const* > scope_seq;

    impl();
    ~impl();

    void parse_command_line(int argc, char* argv[]);

    void operator()(
        environment& env,
        char const * p_scope_name,
        void (*p_f)( environment&, void* ),
        void* p);

    bool excluded(char const * p_name) const;

    void scope_enter(char const * p_name);
    void scope_exit(char const * p_name);

    int main_return_value() const;
};

/*******************************************************************************
 * environment and environment::impl member function implementations
 ******************************************************************************/

environment::
environment()
    : mp_impl(new impl)
{ }
inline
environment::impl::
impl()
    : p_out(&std::cout)
{ }

environment::
environment(environment const & other)
    : mp_impl(new impl(*other.mp_impl))
{ }

environment::
~environment()
{ delete mp_impl; }
inline
environment::impl::
~impl()
{ assert(scope_seq.empty()); }

environment&
environment::
operator=(environment other)
{
    swap(other);
    return *this;
}

void
environment::
swap(environment& other)
{ std::swap(mp_impl, other.mp_impl); }

void
environment::
parse_command_line(int argc, char* argv[])
{ mp_impl->parse_command_line(argc, argv); }
inline void
environment::impl::
parse_command_line(int argc, char* argv[])
{
    static_cast<void>(argc);
    static_cast<void>(argv);
}

namespace
{

void apply(environment& env, void* p_f)
{ (*static_cast< void (*)( environment& ) >(p_f))(env); }

} // namespace

void
environment::
operator()(char const * p_scope_name, void (*p_f)( environment& ))
{ operator()(p_scope_name, &sake::test::apply, static_cast< void* >(p_f)); }

void
environment::
operator()(char const * p_scope_name, void (*p_f)( environment&, void* ), void* p)
{ mp_impl->operator()(*this, p_scope_name, p_f, p); }
inline void
environment::impl::
operator()(
    environment& this_,
    char const * p_scope_name,
    void (*p_f)( environment&, void* ),
    void* p)
{
    // TODO: Check whether to include this scope.

    scope_seq.push_back(p_scope_name);
    struct scope_exit_type
    {
        impl& this_;
        ~scope_exit_type()
        {
            if(this_.p_out) {
                *this_.p_out << "Exiting scope " << this_.scope_seq.back() << "..." << std::endl;
            }
            this_.scope_seq.pop_back();
        }
    } _scope_exit = { *this };

    if(p_out) {
        *p_out << "Entering scope " << p_scope_name << "..." << std::endl;
    }

    try {
        (*p_f)(this_, p);
    }
    catch(...) {
        if(p_out) {
            *p_out << "*** Exception thrown! ***" << std::endl;
        }
        // TODO: Check whether to absorb or rethrow exception.
    }
}

int
environment::
main_return_value() const
{ return mp_impl->main_return_value(); }
inline int
environment::impl::
main_return_value() const
{
    // TODO: Indicate error.
    return 0;
}

} // namespace test

} // namespace sake

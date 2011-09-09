/*******************************************************************************
 * lib/test/environment.cpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <cassert>
#include <cstring>

#include <algorithm>
#include <exception>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <utility>

#include <boost/exception/diagnostic_information.hpp>
#include <boost/exception/exception.hpp>

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
    std::ostream* p_log;
    bool default_enable;
    unsigned int default_log_level;

    struct scope_data;
    std::map< std::string, scope_data > data_of_scope_name;

    std::pair< std::string const, scope_data >* p_current_scope;

    impl();
    ~impl();

    void parse_command_line(int argc, char* argv[]);

    void operator()(
        environment& env,
        char const * const local_scope_name,
        void (*p_f)( environment&, void* ),
        void* const p);

    void fail(
        e_fail_level const fail_level,
        char const * const macro, char const * const expression,
        char const * const filename, char const * const function, unsigned int const line_number,
        char const * const message);

    int main_return_value() const;
};

/*******************************************************************************
 * struct environment::impl::scope_data
 ******************************************************************************/

struct environment::impl::scope_data
{
    bool enable;
    unsigned int log_level;
    bool rethrow_exception;
    std::string aux_arg;

    unsigned int depth;

    bool n_exception;
    unsigned int n_fail_warn;
    unsigned int n_fail_check;
    unsigned int n_fail_require;

    scope_data()
        : enable(true),
          log_level(0),
          rethrow_exception(false),
          depth(0),
          n_exception(false),
          n_fail_warn(0),
          n_fail_check(0),
          n_fail_require(0)
    { }
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
    : p_log(&std::cout),
      default_enable(true),
      default_log_level(0),
      p_current_scope(0)
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
{ assert(p_current_scope == 0); }

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

void
environment::
operator()(char const * local_scope_name, void (*p_f)( environment&, void* ), void* p)
{ mp_impl->operator()(*this, local_scope_name, p_f, p); }
inline void
environment::impl::
operator()(
    environment& this_,
    char const * const local_scope_name,
    void (*p_f)( environment&, void* ),
    void* const p)
{
    assert(local_scope_name);
    assert(*local_scope_name);
    assert(p_f);

    struct raii_type
    {
        impl& this_;
        std::pair< std::string const, scope_data >* const p_outer_scope;
        char const * const local_scope_name;
        ~raii_type()
        {
            if(this_.p_current_scope == p_outer_scope)
                return;
            if(this_.p_log && this_.p_current_scope->second.log_level <= log_level_cross_scope)
                try {
                    *this_.p_log << std::setw(this_.p_current_scope->second.depth) << ""
                                 << "Exiting local scope \"" << local_scope_name << "\"..."
                                 << std::endl;
                }
                catch(...)
                { }
            this_.p_current_scope = p_outer_scope;
        }
    } raii = { *this, p_current_scope, local_scope_name };

    // Construct scope_name.
    std::string scope_name;
    if(p_current_scope) {
        scope_name.reserve(p_current_scope->first.size() + 1 + std::strlen(local_scope_name));
        scope_name += p_current_scope->first;
        scope_name += '.';
        scope_name += local_scope_name;
    }
    else
        scope_name = local_scope_name;

    // Return if this scope is not enabled.
    std::map< std::string, scope_data >::iterator it = data_of_scope_name.find(scope_name);
    if(!(it == data_of_scope_name.end() ? default_enable : it->second.enable))
        return;

    // If necessary, construct a scope_data object for this scope.
    if(it == data_of_scope_name.end()) {
        it = data_of_scope_name.insert(std::make_pair(scope_name, scope_data())).first;
        it->second.log_level = p_current_scope ? p_current_scope->second.log_level : default_log_level;
    }
    it->second.depth = p_current_scope ? p_current_scope->second.depth + 1 : 0;
    p_current_scope = &*it;

    if(p_log && p_current_scope->second.log_level <= log_level_cross_scope)
        *p_log << std::setw(p_current_scope->second.depth) << ""
               << "Entering local scope \"" << local_scope_name << "\"..."
               << std::endl;

    try {
        try {
            (*p_f)(this_, p);
        }
        catch(boost::exception& e) {
            if(p_current_scope->second.rethrow_exception)
                throw;
            if(p_log && p_current_scope->second.log_level <= log_level_exception)
                *p_log << std::setw(p_current_scope->second.depth) << ""
                       << "*** boost::exception thrown ***\n"
                       << boost::diagnostic_information(e)
                       << std::endl;
        }
        catch(std::exception& e) {
            if(p_current_scope->second.rethrow_exception)
                throw;
            if(p_log && p_current_scope->second.log_level <= log_level_exception)
                *p_log << std::setw(p_current_scope->second.depth) << ""
                       << "*** std::exception thrown ***\n"
                       << boost::diagnostic_information(e)
                       << std::endl;
        }
        catch(...) {
            if(p_current_scope->second.rethrow_exception)
                throw;
            if(p_log && p_current_scope->second.log_level <= log_level_exception)
                *p_log << std::setw(p_current_scope->second.depth) << ""
                       << "*** boost::exception thrown ***\n"
                       << "(boost::diagnostic_information unavailable)"
                       << std::endl;
        }
    }
    catch(...) {
        p_current_scope->second.n_exception = true;
        if(p_current_scope->second.rethrow_exception)
            throw;
    }
}

void
environment::
fail(
    e_fail_level fail_level,
    char const * macro, char const * expression,
    char const * filename, char const * function, unsigned int line_number,
    char const * message)
{ mp_impl->fail(fail_level, macro, expression, filename, function, line_number, message); }
inline void
environment::impl::
fail(
    e_fail_level const fail_level,
    char const * const macro, char const * const expression,
    char const * const filename, char const * const function, unsigned int const line_number,
    char const * const message)
{
    scope_data& data = p_current_scope->second;
    unsigned int log_level_fail = 0;
    switch(fail_level) {
    case fail_level_warn:
        ++data.n_fail_warn;
        log_level_fail = log_level_warn;
        break;
    case fail_level_check:
        ++data.n_fail_check;
        log_level_fail = log_level_check;
        break;
    case fail_level_require:
        ++data.n_fail_require;
        log_level_fail = log_level_require;
        break;
    }
    if(p_log && data.log_level <= log_level_fail)
        *p_log << std::setw(p_current_scope->second.depth) << ""
               << macro << " failure: " << message
               << " [" << filename << ':' << function << ':' << line_number << ']'
               << std::endl;
    if(fail_level == fail_level_require) {
        // TODO: throw a custom exception to force the scope to exit early
        return;
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
    std::map< std::string, scope_data >::const_iterator it = data_of_scope_name.begin();
    for(; it != data_of_scope_name.end(); ++it) {
        scope_data const & data = it->second;
        if(data.n_exception || data.n_fail_check != 0 || data.n_fail_require != 0)
            return 1;
    }
    return 0;
}

} // namespace test

} // namespace sake

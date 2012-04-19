/*******************************************************************************
 * libs/test/environment.cpp
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
#include <limits>
#include <map>
#include <sstream>
#include <string>
#include <utility>

#include <boost/exception/diagnostic_information.hpp>
#include <boost/exception/exception.hpp>

#include <sake/core/utility/assert.hpp>
#include <sake/core/utility/onstream.hpp>
#include <sake/core/utility/timer.hpp>

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
    struct fail_require_exception { };

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

    std::ostream& log() const;
    unsigned int current_log_level() const;
    unsigned int current_depth() const;

    void report() const;
    int main_return_value() const;

    void fail(e_fail_level const fail_level, char const* const message);
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
    double elapsed_time;

    unsigned int n_fail_warn;
    unsigned int n_fail_check;
    unsigned int n_fail_require;
    unsigned int n_throw_exception;

    scope_data()
        : enable(true),
          log_level(0),
          rethrow_exception(false),
          depth(0),
          elapsed_time(0),
          n_fail_warn(0),
          n_fail_check(0),
          n_fail_require(0),
          n_throw_exception(0)
    { }
};

/*******************************************************************************
 * environment and environment::impl member definitions
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

    struct exit_scope
    {
        impl& this_;
        std::pair< std::string const, scope_data >* const p_outer_scope;
        char const * const local_scope_name;
        ~exit_scope()
        {
            if(this_.p_current_scope == p_outer_scope)
                return;
            if(this_.p_log && this_.p_current_scope->second.log_level <= log_level_cross_scope)
                try {
                    *this_.p_log << std::setw(2 * this_.p_current_scope->second.depth) << ""
                                 << "Exiting local scope " << local_scope_name << "..."
                                 << std::endl;
                }
                catch(...)
                { }
            this_.p_current_scope = p_outer_scope;
        }
    } _exit_scope = { *this, p_current_scope, local_scope_name };
    static_cast<void>(_exit_scope);

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
    std::map< std::string, scope_data >::iterator i = data_of_scope_name.find(scope_name);
    if(!(i == data_of_scope_name.end() ? default_enable : i->second.enable))
        return;

    // If necessary, construct a scope_data object for this scope.
    if(i == data_of_scope_name.end()) {
        i = data_of_scope_name.insert(std::make_pair(scope_name, scope_data())).first;
        i->second.log_level = p_current_scope ? p_current_scope->second.log_level : default_log_level;
    }
    unsigned int const depth = p_current_scope ? p_current_scope->second.depth + 1 : 0;
    assert(i->second.depth == 0 || i->second.depth == depth);
    i->second.depth = depth;
    p_current_scope = &*i;
    unsigned int const log_level = p_current_scope->second.log_level;
    bool const rethrow_exception = p_current_scope->second.rethrow_exception;
    assert(p_current_scope->second.depth == depth);

    if(p_log && log_level <= log_level_cross_scope)
        *p_log << std::setw(2 * depth) << ""
               << "Entering local scope " << local_scope_name << "..."
               << std::endl;

    try {
        try {
            sake::timer timer;
            struct update_elapsed_time
            {
                sake::timer const & timer;
                double& elapsed_time;
                ~update_elapsed_time()
                { elapsed_time += timer.elapsed(); }
            } _update_elapsed_time = { timer, p_current_scope->second.elapsed_time };
            static_cast<void>(_update_elapsed_time);
            (*p_f)(this_, p);
        }
        catch(fail_require_exception) { }
        catch(boost::exception& e) {
            if(rethrow_exception)
                throw;
            if(p_log && log_level <= log_level_exception)
                *p_log << std::setw(2 * depth) << "" << "*** boost::exception thrown ***\n"
                       << std::setw(2 * depth) << "" << boost::diagnostic_information(e)
                       << std::endl;
        }
        catch(std::exception& e) {
            if(rethrow_exception)
                throw;
            if(p_log && log_level <= log_level_exception)
                *p_log << std::setw(2 * depth) << "" << "*** std::exception thrown ***\n"
                       << std::setw(2 * depth) << "" << boost::diagnostic_information(e)
                       << std::endl;
        }
        catch(...) {
            if(rethrow_exception)
                throw;
            if(p_log && log_level <= log_level_exception)
                *p_log << std::setw(2 * depth) << "" << "*** unknown exception thrown ***\n"
                       << std::setw(2 * depth) << "" << "(boost::diagnostic_information unavailable)"
                       << std::endl;
        }
    }
    catch(...) {
        ++p_current_scope->second.n_throw_exception;
        if(rethrow_exception)
            throw;
    }
}

std::ostream&
environment::
log() const
{ return mp_impl->log(); }
inline std::ostream&
environment::impl::
log() const
{ return p_log ? *p_log : sake::nout; }

unsigned int
environment::
current_log_level() const
{ return mp_impl->current_log_level(); }
inline unsigned int
environment::impl::
current_log_level() const
{
    return p_current_scope ?
           p_current_scope->second.log_level :
           default_log_level;
}

unsigned int
environment::
current_depth() const
{ return mp_impl->current_depth(); }
inline unsigned int
environment::impl::
current_depth() const
{
    return p_current_scope ?
           p_current_scope->second.depth :
           std::numeric_limits< unsigned int >::max();
}

void
environment::
report() const
{ mp_impl->report(); }
inline void
environment::impl::
report() const
{
    assert(p_current_scope == 0);
    if(!p_log)
        return;
    std::map< std::string, scope_data >::const_iterator i = data_of_scope_name.begin();
    double elapsed_time = 0;
    unsigned int n_fail_warn = 0;
    unsigned int n_fail_check = 0;
    unsigned int n_fail_require = 0;
    unsigned int n_throw_exception = 0;
    *p_log << "[Showing # of failed warn(s), check(s), require(s), exception(s)]"
           << std::endl;
    for(; i != data_of_scope_name.end(); ++i) {
        scope_data const & data = i->second;
        elapsed_time += data.elapsed_time;
        n_fail_warn += data.n_fail_warn;
        n_fail_check += data.n_fail_check;
        n_fail_require += data.n_fail_require;
        n_throw_exception += data.n_throw_exception;
        *p_log << i->first << " (" << data.elapsed_time << " s) : "
               << data.n_fail_warn << ' '
               << data.n_fail_check << ' '
               << data.n_fail_require << ' '
               << data.n_throw_exception
               << std::endl;
    }
    *p_log << "CUMULATIVE (" << elapsed_time << " s) : "
           << n_fail_warn << ' '
           << n_fail_check << ' '
           << n_fail_require << ' '
           << n_throw_exception
           << std::endl;
}

int
environment::
main_return_value() const
{ return mp_impl->main_return_value(); }
inline int
environment::impl::
main_return_value() const
{
    assert(p_current_scope == 0);
    std::map< std::string, scope_data >::const_iterator i = data_of_scope_name.begin();
    for(; i != data_of_scope_name.end(); ++i) {
        scope_data const & data = i->second;
        if(data.n_fail_check != 0 || data.n_fail_require != 0 || data.n_throw_exception != 0)
            return 1;
    }
    return 0;
}

void
environment::
fail(e_fail_level fail_level, char const * message)
{ mp_impl->fail(fail_level, message); }
inline void
environment::impl::
fail(e_fail_level const fail_level, char const* const message)
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
    if(p_log && data.log_level <= log_level_fail) {
        *p_log << std::setw(2 * data.depth) << message;
        p_log->flush();
    }
    if(fail_level == fail_level_require)
        throw fail_require_exception();
}

void
environment::
fail(
    e_fail_level fail_level,
    char const * macro, char const * expression,
    char const * filename, char const * function, unsigned int line_number)
{
    std::ostringstream message(std::ostringstream::out);
    sake::assert_failure_action::print(
        message,
        macro, expression, filename, function, line_number
    );
    fail(fail_level, message.str().c_str());
}

void
environment::
fail(
    e_fail_level fail_level,
    char const * macro, char const * expression,
    char const * filename, char const * function, unsigned int line_number,
    unsigned int subexpression_index, char const * subexpression)
{
    std::ostringstream message(std::ostringstream::out);
    sake::assert_failure_action::print(
        message,
        macro, expression, filename, function, line_number,
        subexpression_index, subexpression
    );
    fail(fail_level, message.str().c_str());
}

} // namespace test

} // namespace sake

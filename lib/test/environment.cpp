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
    struct scope_data;

    std::ostream* p_out;
    bool default_enable;
    std::map< std::string, scope_data > data_of_scope;

    std::string current_scope;
    unsigned int current_scope_depth;
    scope_data* p_current_scope_data;

    impl();
    ~impl();

    void parse_command_line(int argc, char* argv[]);

    void operator()(
        environment& env,
        char const * p_local_scope,
        void (*p_f)( environment&, void* ),
        void* p);

    int main_return_value() const;
};

/*******************************************************************************
 * struct environment::impl::scope_data
 ******************************************************************************/

struct environment::impl::scope_data
{
    struct config_type
    {
        bool enable;
        bool rethrow_exception;
        std::string params;
        config_type()
            : enable(true),
              rethrow_exception(false)
        { }
    } config;
    struct result_type
    {
        bool n_exception;
        unsigned int n_fail_warn;
        unsigned int n_fail_check;
        unsigned int n_fail_require;
        result_type()
            : n_exception(false),
              n_fail_warn(0),
              n_fail_check(0),
              n_fail_require(0)
        { }
    } result;
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
    : p_out(&std::cout),
      default_enable(true),
      current_scope_depth(0),
      p_current_scope_data(0)
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
{
    assert(current_scope.empty());
    assert(current_scope_depth == 0);
    assert(p_current_scope_data == 0);
}

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
operator()(char const * p_local_scope, void (*p_f)( environment&, void* ), void* p)
{ mp_impl->operator()(*this, p_local_scope, p_f, p); }
inline void
environment::impl::
operator()(
    environment& this_,
    char const * p_local_scope,
    void (*p_f)( environment&, void* ),
    void* p)
{
    assert(p_local_scope);
    assert(*p_local_scope);
    assert(p_f);

    std::string saved_current_scope;
    current_scope.swap(saved_current_scope);
    scope_data* const saved_p_current_scope_data = p_current_scope_data;

    struct raii_type
    {
        impl& this_;
        std::string& saved_current_scope;
        scope_data* const saved_p_current_scope_data;
        char const * const p_local_scope;
        bool restore_scope_depth;
        ~raii_type()
        {
            this_.current_scope.swap(saved_current_scope);
            this_.p_current_scope_data = saved_p_current_scope_data;
            if(restore_scope_depth) {
                assert(this_.current_scope_depth > 0);
                --this_.current_scope_depth;
                if(this_.p_out)
                    try {
                        *this_.p_out << std::setw(this_.current_scope_depth) << ""
                                     << "Exiting local scope \"" << p_local_scope << "\"..."
                                     << std::endl;
                    }
                    catch(...)
                    { }
            }
        }
    } raii = {
        *this,
        saved_current_scope,
        saved_p_current_scope_data,
        p_local_scope,
        false
    };

    // Update current_scope.
    if(saved_current_scope.empty())
        current_scope = p_local_scope;
    else {
        current_scope.reserve(saved_current_scope.size() + 1 + std::strlen(p_local_scope));
        current_scope = saved_current_scope + '.' + p_local_scope;
    }

    // If scope is disabled, return.
    std::map< std::string, scope_data >::iterator it = data_of_scope.find(current_scope);
    if(!(it == data_of_scope.end() ? default_enable : it->second.config.enable))
        return;

    // Enter local scope.
    if(p_out)
        *p_out << std::setw(current_scope_depth) << ""
               << "Entering local scope \"" << p_local_scope << "\"..."
               << std::endl;
    ++current_scope_depth;
    raii.restore_scope_depth = true;

    // If necessary, construct scope_data object for the current scope.
    if(it == data_of_scope.end())
        it = data_of_scope.insert(std::make_pair(current_scope, scope_data())).first;
    p_current_scope_data = &it->second;

    try {
        try {
            (*p_f)(this_, p);
        }
        catch(boost::exception& e) {
            if(p_current_scope_data->config.rethrow_exception)
                throw;
            if(p_out) {
                *p_out << std::setw(current_scope_depth) << ""
                       << "*** boost::exception thrown ***"
                       << std::endl;
                *p_out << boost::diagnostic_information(e) << std::endl;
            }
        }
        catch(std::exception& e) {
            if(p_current_scope_data->config.rethrow_exception)
                throw;
            if(p_out) {
                *p_out << std::setw(current_scope_depth) << ""
                       << "*** std::exception thrown ***"
                       << std::endl;
                *p_out << boost::diagnostic_information(e) << std::endl;
            }
        }
        catch(...) {
            if(p_current_scope_data->config.rethrow_exception)
                throw;
            if(p_out) {
                *p_out << std::setw(current_scope_depth) << ""
                       << "*** Unknown exception thrown ***"
                       << std::endl;
                *p_out << "[boost::diagnostic_information unavailable]" << std::endl;
            }
        }
    }
    catch(...) {
        p_current_scope_data->result.n_exception = true;
        if(p_current_scope_data->config.rethrow_exception)
            throw;
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
    std::map< std::string, scope_data >::const_iterator it = data_of_scope.begin();
    for(; it != data_of_scope.end(); ++it) {
        scope_data::result_type const & result = it->second.result;
        if(result.n_exception || result.n_fail_check != 0 || result.n_fail_require != 0)
            return 1;
    }
    return 0;
}

} // namespace test

} // namespace sake

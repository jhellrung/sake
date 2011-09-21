/*******************************************************************************
 * lib/core/utility/assert.cpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef BOOST_PP_IS_SELFISH

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <exception>
#include <iostream>
#include <limits>
//#include <sstream>
#include <string>

//#include <boost/preprocessor/iteration/self.hpp>

#include <sake/core/utility/assert.hpp>

namespace sake
{

namespace assert_failure_action
{

namespace functional
{

void
print::
operator()(
    std::ostream& o,
    char const * const macro, char const * const /*expression*/,
    char const * const filename, char const * const function, unsigned int const line_number,
    char const * const message) const
{
    std::cout.flush();
    std::cerr.flush();
    o << macro << " failure: " << message
      << " [" << filename << ':' << function << ':' << line_number << ']'
      << std::endl;
}

void
print::
operator()(
    std::ostream& o,
    char const * const macro, char const * const expression,
    char const * const filename, char const * const function, unsigned int const line_number) const
{
    std::string message;
    message.reserve((2 + 2) + std::strlen(expression));
    message.operator+=("{ ")
           .operator+=(expression)
           .operator+=(" }");
    operator()(o,
        macro, expression, filename, function, line_number,
        message.c_str()
    );
}

void
print::
operator()(
    std::ostream& o,
    char const * const macro, char const * const expression,
    char const * const filename, char const * const function, unsigned int const line_number,
    unsigned int const subexpression_index, char const * const subexpression) const
{
#if 1
    static const int uint_digits10 = (std::numeric_limits< unsigned int >::digits + 2)/3;
    std::string message;
    message.reserve(
        (2 + 18 + uint_digits10 + 10 + 3)
      + std::strlen(subexpression)
      + std::strlen(expression)
    );
    char subexpression_index_buffer[uint_digits10 + 1];
#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4996 ) // 'sprintf': This function or variable may
                                  // be unsafe. Consider using sprintf_s
                                  // instead. To disable deprecation, use
                                  // _CRT_SECURE_NO_WARNINGS. See online help
                                  // for details.
#endif // #ifdef _MSC_VER
    std::sprintf(subexpression_index_buffer, "%u", subexpression_index);
#ifdef _MSC_VER
#pragma warning( pop )
#endif // #ifdef _MSC_VER
    message.operator+=("{ ")
           .operator+=(subexpression)
           .operator+=(" } (subexpression ")
           .operator+=(subexpression_index_buffer)
           .operator+=(" within { ")
           .operator+=(expression)
           .operator+=(" })");
    operator()(o,
        macro, expression, filename, function, line_number,
        message.c_str()
    );
#else // #if 1
    std::ostringstream message(std::ostringstream::out);
    message << "{ " << subexpression << " } "
               "(subexpression " << subexpression_index << " within { " << expression << " })";
    operator()(o,
        macro, expression, filename, function, line_number,
        message.str().c_str()
    );
#endif // #if 1
}

#define failure_action         abort
#define failure_action_abort() std::abort()
//#define BOOST_PP_INDIRECT_SELF "assert.cpp"
//#include BOOST_PP_INCLUDE_SELF()
#include "assert.ipp"
#undef failure_action
#undef failure_action_abort

#define failure_action         terminate
#define failure_action_abort() std::terminate()
//#define BOOST_PP_INDIRECT_SELF "assert.cpp"
//#include BOOST_PP_INCLUDE_SELF()
#include "assert.ipp"
#undef failure_action
#undef failure_action_abort

#define failure_action         exit
#define failure_action_abort() std::exit( EXIT_FAILURE )
//#define BOOST_PP_INDIRECT_SELF "assert.cpp"
//#include BOOST_PP_INCLUDE_SELF()
#include "assert.ipp"
#undef failure_action
#undef failure_action_abort

} // namespace functional

} // namespace assert_failure_action

} // namespace sake

#else // #ifndef BOOST_PP_IS_SELFISH

void
failure_action::
operator()(
    std::ostream& o,
    char const * const macro, char const * const expression,
    char const * const filename, char const * const function, unsigned int const line_number) const
{
    functional::print()(o,
        macro, expression, filename, function, line_number
    );
    failure_action_abort();
}

void
failure_action::
operator()(
    std::ostream& o,
    char const * const macro, char const * const expression,
    char const * const filename, char const * const function, unsigned int const line_number,
    unsigned int const subexpression_index, char const * const subexpression) const
{
    functional::print()(o,
        macro, expression, filename, function, line_number,
        subexpression_index, subexpression
    );
    failure_action_abort();
}

#endif // #ifndef BOOST_PP_IS_SELFISH

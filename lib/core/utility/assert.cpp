/*******************************************************************************
 * lib/core/utility/assert.cpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef BOOST_PP_IS_SELFISH

#include <cstdlib>
#include <cstring>

#include <exception>
#include <iostream>
#include <sstream>
#include <string>

#include <boost/preprocessor/iteration/self.hpp>

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
    char const * const macro, char const * const expression,
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
    message.operator+=("{ ").operator+=(expression).operator+=(" }");
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
    std::ostringstream message(std::ostringstream::out);
    message << "{ " << subexpression << " } "
               "(subexpression " << subexpression_index << " within { " << expression << " })";
    operator()(o,
        macro, expression, filename, function, line_number,
        message.str().c_str()
    );
}

#define failure_action         abort
#define failure_action_abort() std::abort()
#define BOOST_PP_INDIRECT_SELF "assert.cpp"
#include BOOST_PP_INCLUDE_SELF()
#undef failure_action
#undef failure_action_abort

#define failure_action         terminate
#define failure_action_abort() std::terminate()
#define BOOST_PP_INDIRECT_SELF "assert.cpp"
#include BOOST_PP_INCLUDE_SELF()
#undef failure_action
#undef failure_action_abort

#define failure_action         exit
#define failure_action_abort() std::exit( EXIT_FAILURE )
#define BOOST_PP_INDIRECT_SELF "assert.cpp"
#include BOOST_PP_INCLUDE_SELF()
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

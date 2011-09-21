/*******************************************************************************
 * libs/core/utility/assert.cpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <exception>
#include <iostream>
#include <limits>
//#include <sstream>
#include <string>

#include <sake/core/utility/assert.hpp>
#include <sake/core/utility/type_tag.hpp>

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

namespace print_private
{

inline char const *
sprintf_format(sake::type_tag< unsigned long >)
{ return "%lu%"; }
inline char const *
sprintf_format(sake::type_tag< long >)
{ return "%li%"; }
inline char const *
sprintf_format(sake::type_tag< long double >)
{ return "%lf"; }

template< class T >
void
apply_builtin(
    std::ostream& o,
    char const * const macro, char const * const expression,
    char const * const filename, char const * const function, unsigned int const line_number,
    T const lhs, char const * op, T const rhs)
{
#if 1
    static const int T_digits10 = 4 + (sizeof( unsigned long ) + sizeof( long double ))
                                     *(std::numeric_limits< unsigned char >::digits + 2)/3;
    std::string message;
    message.reserve(
        (2 + 8 + T_digits10 + 1 + 1 + T_digits10 + 2)
      + std::strlen(expression)
      + std::strlen(op)
    );
    char lhs_buffer[T_digits10 + 1];
    char rhs_buffer[T_digits10 + 1];
#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4996 ) // 'sprintf': This function or variable may
                                  // be unsafe. Consider using sprintf_s
                                  // instead. To disable deprecation, use
                                  // _CRT_SECURE_NO_WARNINGS. See online help
                                  // for details.
#endif // #ifdef _MSC_VER
    std::sprintf(lhs_buffer, sprintf_format(sake::type_tag<T>()), lhs);
    std::sprintf(rhs_buffer, sprintf_format(sake::type_tag<T>()), rhs);
#ifdef _MSC_VER
#pragma warning( pop )
#endif // #ifdef _MSC_VER
    message.operator+=("{ ")
           .operator+=(expression)
           .operator+=(" } == { ")
           .operator+=(lhs_buffer)
           .operator+=(' ')
           .operator+=(op)
           .operator+=(' ')
           .operator+=(rhs_buffer)
           .operator+=(" }");
    functional::print()(o,
        macro, expression, filename, function, line_number,
        message.c_str()
    );
#else // #if 1
    std::ostringstream message(std::ostringstream::out);
    message << "{ " << expression << " } == "
               "{ " << sake::make_ostreamable(lhs)
                    << ' ' << op << ' '
                    << sake::make_ostreamable(rhs) << " }";
    functional::print()(o,
        macro, expression, filename, function, line_number,
        message.str().c_str()
    );
#endif // #if 1
}

#define explicit_instantiation( T ) \
template void \
apply_builtin<T>( \
    std::ostream& o, \
    char const * const macro, char const * const expression, \
    char const * const filename, char const * const function, unsigned int const line_number, \
    T const lhs, char const * op, T const rhs);
explicit_instantiation( long )
explicit_instantiation( unsigned long )
explicit_instantiation( long double )
#undef explicit_instantiation

template< class T >
void
apply_builtin(
    std::ostream& o,
    char const * const macro, char const * const expression,
    char const * const filename, char const * const function, unsigned int const line_number,
    unsigned int const subexpression_index, char const * const subexpression,
    T const lhs, char const * const op, T const rhs)
{
#if 1
    static const int uint_digits10 = (std::numeric_limits< unsigned int >::digits + 2)/3;
    static const int T_digits10 = 4 + (sizeof( unsigned long ) + sizeof( long double ))
                                     *(std::numeric_limits< unsigned char >::digits + 2)/3;
    std::string message;
    message.reserve(
        (2 + 8 + T_digits10 + 1 + 1 + T_digits10 + 18 + uint_digits10 + 10 + 3)
      + std::strlen(subexpression)
      + std::strlen(op)
      + std::strlen(expression)
    );
    char subexpression_index_buffer[uint_digits10 + 1];
    char lhs_buffer[T_digits10 + 1];
    char rhs_buffer[T_digits10 + 1];
#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4996 ) // 'sprintf': This function or variable may
                                  // be unsafe. Consider using sprintf_s
                                  // instead. To disable deprecation, use
                                  // _CRT_SECURE_NO_WARNINGS. See online help
                                  // for details.
#endif // #ifdef _MSC_VER
    std::sprintf(subexpression_index_buffer, "%u", subexpression_index);
    std::sprintf(lhs_buffer, sprintf_format(sake::type_tag<T>()), lhs);
    std::sprintf(rhs_buffer, sprintf_format(sake::type_tag<T>()), rhs);
#ifdef _MSC_VER
#pragma warning( pop )
#endif // #ifdef _MSC_VER
    message.operator+=("{ ")
           .operator+=(subexpression)
           .operator+=(" } == { ")
           .operator+=(lhs_buffer)
           .operator+=(' ')
           .operator+=(op)
           .operator+=(' ')
           .operator+=(rhs_buffer)
           .operator+=(" } (subexpression ")
           .operator+=(subexpression_index_buffer)
           .operator+=(" within { ")
           .operator+=(expression)
           .operator+=(" })");
    functional::print()(o,
        macro, expression, filename, function, line_number,
        message.c_str()
    );
#else // #if 1
    std::ostringstream message(std::ostringstream::out);
    message << "{ " << subexpression << " } == "
               "{ " << sake::make_ostreamable(lhs)
                    << ' ' << op << ' '
                    << sake::make_ostreamable(rhs) << " } "
               "(subexpression " << subexpression_index << " within { " << expression << " })";
    functional::print()(o,
        macro, expression, filename, function, line_number,
        message.str().c_str()
    );
#endif // #if 1
}

#define explicit_instantiation( T ) \
template void \
apply_builtin<T>( \
    std::ostream& o, \
    char const * const macro, char const * const expression, \
    char const * const filename, char const * const function, unsigned int const line_number, \
    unsigned int const subexpression_index, char const * const subexpression, \
    T const lhs, char const * const op, T const rhs);
explicit_instantiation( long )
explicit_instantiation( unsigned long )
explicit_instantiation( long double )
#undef explicit_instantiation

} // namespace print_private

#define failure_action         abort
#define failure_action_abort() std::abort()
#include "private/assert.ipp"

#define failure_action         terminate
#define failure_action_abort() std::terminate()
#include "private/assert.ipp"

#define failure_action         exit
#define failure_action_abort() std::exit( EXIT_FAILURE )
#include "private/assert.ipp"

} // namespace functional

} // namespace assert_failure_action

} // namespace sake

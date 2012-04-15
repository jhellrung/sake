/*******************************************************************************
 * libs/core/utility/assert.cpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <cassert>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <exception>
#include <iostream>
#include <limits>
//#include <sstream>
#include <string>

#include <boost/preprocessor/seq/elem.hpp>
#include <boost/preprocessor/seq/for_each_product.hpp>

#include <sake/core/math/zero.hpp>
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
    static std::size_t const uint_max_length = (std::numeric_limits< unsigned int >::digits + 2)/3;
    std::string message;
    message.reserve(
        (2 + 18 + uint_max_length + 10 + 3)
      + std::strlen(subexpression)
      + std::strlen(expression)
    );
    char uint_buffer[uint_max_length + 1];
#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4996 ) // 'sprintf': This function or variable may
                                  // be unsafe. Consider using sprintf_s
                                  // instead. To disable deprecation, use
                                  // _CRT_SECURE_NO_WARNINGS. See online help
                                  // for details.
#endif // #ifdef _MSC_VER
    std::sprintf(uint_buffer, "%u", subexpression_index);
#ifdef _MSC_VER
#pragma warning( pop )
#endif // #ifdef _MSC_VER
    message.operator+=("{ ")
           .operator+=(subexpression)
           .operator+=(" } (subexpression ")
           .operator+=(uint_buffer)
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

template< class T >
struct sprintf_dispatch;

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4996 ) // 'sprintf': This function or variable may
                                  // be unsafe. Consider using sprintf_s
                                  // instead. To disable deprecation, use
                                  // _CRT_SECURE_NO_WARNINGS. See online help
                                  // for details.
#endif // #ifdef _MSC_VER

template<>
struct sprintf_dispatch< long >
{
    static std::size_t const max_length = 1 + (std::numeric_limits< long >::digits + 2)/3;
    static void apply(char (&buffer)[max_length+1], long const x)
    {
        int const n = std::sprintf(buffer, "%li", x);
        static_cast< void >(n);
        assert(0 < n && n <= static_cast< int >(max_length));
    }
};

template<>
struct sprintf_dispatch< unsigned long >
{
    static std::size_t const max_length = (std::numeric_limits< unsigned long >::digits + 2)/3;
    static void apply(char (&buffer)[max_length+1], unsigned long const x)
    {
        int const n = std::sprintf(buffer, "%lu", x);
        static_cast< void >(n);
        assert(0 < n && n <= static_cast< int >(max_length));
    }
};

template<>
struct sprintf_dispatch< long double >
{
    static std::size_t const max_length =
        4 + sizeof( long double ) * (std::numeric_limits< unsigned char >::digits + 2)/3;
    static void apply(char (&buffer)[max_length+1], long double const x)
    {
        int const n = std::sprintf(buffer, "%Lf", x);
        static_cast< void >(n);
        assert(0 < n && n <= static_cast< int >(max_length));
    }
};

template<>
struct sprintf_dispatch< void const * >
{
    static std::size_t const max_length =
        // Assume hexadecimal output.
        2 + sizeof( void const * ) * std::numeric_limits< unsigned char >::digits / 4;
    static void apply(char (&buffer)[max_length+1], void const * const x)
    {
        int const n = std::sprintf(buffer, "%p", x);
        static_cast< void >(n);
        assert(0 < n && n <= static_cast< int >(max_length));
    }
};

template<>
struct sprintf_dispatch< sake::zero_t >
{
    static std::size_t const max_length = 1;
    static void apply(char (&buffer)[max_length+1], sake::zero_t)
    { buffer[0] = '0'; }
};

#ifdef _MSC_VER
#pragma warning( pop )
#endif // #ifdef _MSC_VER

void
apply_special_helper(
    std::ostream& o,
    char const * const macro, char const * const expression,
    char const * const filename, char const * const function, unsigned int const line_number,
    char const * const lhs, char const * const op, char const * const rhs,
    std::string& message)
{
    message.operator+=("{ ")
           .operator+=(expression)
           .operator+=(" } == { ")
           .operator+=(lhs)
           .operator+=(' ')
           .operator+=(op)
           .operator+=(' ')
           .operator+=(rhs)
           .operator+=(" }");
    sake::assert_failure_action::functional::print()(o,
        macro, expression, filename, function, line_number,
        message.c_str()
    );
}

void
apply_special_helper(
    std::ostream& o,
    char const * const macro, char const * const expression,
    char const * const filename, char const * const function, unsigned int const line_number,
    unsigned int const subexpression_index, char const * const subexpression,
    char const * const lhs, char const * const op, char const * const rhs,
    std::string& message)
{
    static std::size_t const uint_max_length = (std::numeric_limits< unsigned int >::digits + 2)/3;
    char uint_buffer[uint_max_length + 1];
#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4996 ) // 'sprintf': This function or variable may
                                  // be unsafe. Consider using sprintf_s
                                  // instead. To disable deprecation, use
                                  // _CRT_SECURE_NO_WARNINGS. See online help
                                  // for details.
#endif // #ifdef _MSC_VER
    std::sprintf(uint_buffer, "%u", subexpression_index);
#ifdef _MSC_VER
#pragma warning( pop )
#endif // #ifdef _MSC_VER
    message.operator+=("{ ")
           .operator+=(subexpression)
           .operator+=(" } == { ")
           .operator+=(lhs)
           .operator+=(' ')
           .operator+=(op)
           .operator+=(' ')
           .operator+=(rhs)
           .operator+=(" } (subexpression ")
           .operator+=(uint_buffer)
           .operator+=(" within { ")
           .operator+=(expression)
           .operator+=(" })");
    sake::assert_failure_action::functional::print()(o,
        macro, expression, filename, function, line_number,
        message.c_str()
    );
}

template< class LHS, class RHS >
void
apply_special(
    std::ostream& o,
    char const * const macro, char const * const expression,
    char const * const filename, char const * const function, unsigned int const line_number,
    LHS const lhs, char const * const op, RHS const rhs)
{
#if 1
    static std::size_t const lhs_max_length = sprintf_dispatch< LHS >::max_length;
    static std::size_t const rhs_max_length = sprintf_dispatch< RHS >::max_length;
    std::string message;
    message.reserve(
        (2 + 8 + lhs_max_length + 1 + 1 + rhs_max_length + 2)
      + std::strlen(expression)
      + std::strlen(op)
    );
    char lhs_buffer[lhs_max_length + 1];
    char rhs_buffer[rhs_max_length + 1];
    sprintf_dispatch< LHS >::apply(lhs_buffer, lhs);
    sprintf_dispatch< RHS >::apply(rhs_buffer, rhs);
    print_private::apply_special_helper(o,
        macro, expression, filename, function, line_number,
        lhs_buffer, op, rhs_buffer,
        message
    );
#else // #if 1
    std::ostringstream message(std::ostringstream::out);
    message << "{ " << expression << " } == "
               "{ " << lhs << ' ' << op << ' ' << rhs << " }";
    sake::assert_failure_action::functional::print()(o,
        macro, expression, filename, function, line_number,
        message.str().c_str()
    );
#endif // #if 1
}

template< class LHS, class RHS >
void
apply_special(
    std::ostream& o,
    char const * const macro, char const * const expression,
    char const * const filename, char const * const function, unsigned int const line_number,
    unsigned int const subexpression_index, char const * const subexpression,
    LHS const lhs, char const * const op, RHS const rhs)
{
#if 1
    static std::size_t const uint_max_length = (std::numeric_limits< unsigned int >::digits + 2)/3;
    static std::size_t const lhs_max_length = sprintf_dispatch< LHS >::max_length;
    static std::size_t const rhs_max_length = sprintf_dispatch< RHS >::max_length;
    std::string message;
    message.reserve(
        (2 + 8 + lhs_max_length + 1 + 1 + rhs_max_length + 18 + uint_max_length + 10 + 3)
      + std::strlen(subexpression)
      + std::strlen(op)
      + std::strlen(expression)
    );
    char lhs_buffer[lhs_max_length + 1];
    char rhs_buffer[rhs_max_length + 1];
    sprintf_dispatch< LHS >::apply(lhs_buffer, lhs);
    sprintf_dispatch< RHS >::apply(rhs_buffer, rhs);
    print_private::apply_special_helper(o,
        macro, expression, filename, function, line_number,
        subexpression_index, subexpression,
        lhs_buffer, op, rhs_buffer,
        message
    );
#else // #if 1
    std::ostringstream message(std::ostringstream::out);
    message << "{ " << subexpression << " } == "
               "{ " << lhs << ' ' << op << ' ' << rhs << " } "
               "(subexpression " << subexpression_index << " within { " << expression << " })";
    sake::assert_failure_action::functional::print()(o,
        macro, expression, filename, function, line_number,
        message.str().c_str()
    );
#endif // #if 1
}

#define special_types \
    ( long ) \
    ( unsigned long ) \
    ( long double ) \
    ( void const * ) \
    ( sake::zero_t )
#define explicit_instantiation( r, LHS_RHS ) \
    explicit_instantiation_impl( \
        BOOST_PP_SEQ_ELEM( 0, LHS_RHS ), \
        BOOST_PP_SEQ_ELEM( 1, LHS_RHS ) \
    )
#define explicit_instantiation_impl( LHS, RHS ) \
template void \
apply_special< LHS, RHS >( \
    std::ostream& o, \
    char const * const macro, char const * const expression, \
    char const * const filename, char const * const function, unsigned int const line_number, \
    LHS const lhs, char const * const op, RHS const rhs); \
template void \
apply_special< LHS, RHS >( \
    std::ostream& o, \
    char const * const macro, char const * const expression, \
    char const * const filename, char const * const function, unsigned int const line_number, \
    unsigned int const subexpression_index, char const * const subexpression, \
    LHS const lhs, char const * const op, RHS const rhs);
BOOST_PP_SEQ_FOR_EACH_PRODUCT(
    explicit_instantiation,
    ( special_types ) ( special_types )
)
#undef special_types
#undef explicit_instantiation
#undef explicit_instantiation_impl

} // namespace print_private

#define failure_action         abort
#define failure_action_abort() std::abort()
#include "private/assert/define_failure_action_apply.ipp"

#define failure_action         terminate
#define failure_action_abort() std::terminate()
#include "private/assert/define_failure_action_apply.ipp"

#define failure_action         exit
#define failure_action_abort() std::exit( EXIT_FAILURE )
#include "private/assert/define_failure_action_apply.ipp"

} // namespace functional

} // namespace assert_failure_action

} // namespace sake

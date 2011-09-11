/*******************************************************************************
 * core/utility/explicit_operator_bool.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_EXPLICIT_OPERATOR_BOOL()
 *
 * This macro should be used in place of the declaration "explicit operator
 * bool()" when defining an explicit bool conversion operator to maintain
 * portability between C++03 and C++11.  See also
 *     http://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Safe_bool
 * In a post to the Boost developer's mailing list, Andrey Semashev showed the
 * technique of defining a private nested struct and returning the address of
 * one of *its* members.  Thus, no need to provide a member of the immediately
 * enclosing class.
 *
 * Example:
 *
 * struct X
 * {
 *     SAKE_EXPLICIT_OPERATOR_BOOL() const
 *     { return ...; }
 * };
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_EXPLICIT_OPERATOR_BOOL_HPP
#define SAKE_CORE_UTILITY_EXPLICIT_OPERATOR_BOOL_HPP

#include <boost/config.hpp>



#ifndef BOOST_NO_EXPLICIT_CONVERSION_OPERATORS

#define SAKE_EXPLICIT_OPERATOR_BOOL() \
explicit operator bool()

#else // #ifndef BOOST_NO_EXPLICIT_CONVERSION_OPERATORS

#ifndef SAKE_NO_EXPLICIT_OPERATOR_BOOL

// On some platforms (so I'm told), the address of the first member of a struct
// is numerically equal to 0, so is converted to false in a boolean context.
// Hence the need for a second member variable in
// _sake_explicit_operator_bool_struct.
#define SAKE_EXPLICIT_OPERATOR_BOOL() \
struct _sake_explicit_operator_bool_struct { int padding; int member; }; \
typedef int (_sake_explicit_operator_bool_struct::*_sake_explicit_operator_bool_result_type); \
operator _sake_explicit_operator_bool_result_type() const \
{ return _sake_explicit_operator_bool_impl() ? &_sake_explicit_operator_bool_struct::member : 0; } \
bool _sake_explicit_operator_bool_impl()

#else // #ifndef SAKE_NO_EXPLICIT_OPERATOR_BOOL

#define SAKE_EXPLICIT_OPERATOR_BOOL() \
operator bool()

#endif // #ifndef SAKE_NO_EXPLICIT_OPERATOR_BOOL

#endif // #ifndef BOOST_NO_EXPLICIT_CONVERSION_OPERATORS

#endif // #ifndef SAKE_CORE_UTILITY_EXPLICIT_OPERATOR_BOOL_HPP

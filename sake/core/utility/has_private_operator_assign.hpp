/*******************************************************************************
 * sake/core/utility/has_private_operator_assign.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct has_private_operator_assign<T>
 *
 * This is a metafunction which should evaluate to true iff T has a private
 * assignment operator.  This is automatically correctly deduced if T uses
 * SAKE_NON_COPYABLE or SAKE_NON_COPY_ASSIGNABLE or derives from
 * non_copyable_base or non_copy_assignable_base, but other types will need to
 * explicitly extend this metafunction.
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_HAS_PRIVATE_ASSIGN_HPP
#define SAKE_CORE_UTILITY_HAS_PRIVATE_ASSIGN_HPP

#include <boost/type_traits/integral_constant.hpp>

#include <sake/core/introspection/has_isc.hpp>
#include <sake/core/utility/extension.hpp>

namespace sake
{

SAKE_INTROSPECTION_DEFINE_HAS_ISC(
    has_isc_has_private_operator_assign,
    has_private_operator_assign
)

namespace no_ext
{

namespace has_private_operator_assign_private
{

template< class T, bool = has_isc_has_private_operator_assign<T>::value >
struct dispatch;

template< class T >
struct dispatch< T, false >
    : boost::false_type
{ };

template< class T >
struct dispatch< T, true >
{
    static bool const value = T::has_private_operator_assign;
    typedef dispatch type;
};

} // namespace has_private_operator_assign_private

template< class T >
struct has_private_operator_assign
    : has_private_operator_assign_private::dispatch<T>
{ };

} // namespace no_ext

SAKE_EXTENSION_UNARY_CLASS( has_private_operator_assign )

template< class T >
struct has_private_operator_assign
    : ext::has_private_operator_assign<T>
{ };

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_HAS_PRIVATE_ASSIGN_HPP

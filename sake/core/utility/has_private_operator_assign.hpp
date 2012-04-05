/*******************************************************************************
 * sake/core/utility/has_private_operator_assign.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct has_private_operator_assign<T>
 * struct extension::has_private_operator_assign< T, Enable = void >
 * struct default_impl::has_private_operator_assign<T>
 *
 * This is a metafunction which should evaluate to true iff T has a private
 * assignment operator.  This is automatically correctly deduced if T uses
 * SAKE_NON_COPYABLE or SAKE_NON_COPY_ASSIGNABLE or derives from
 * non_copyable_base or non_copy_assignable_base, but other types will need to
 * explicitly extend this metafunction.
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_HAS_PRIVATE_ASSIGN_HPP
#define SAKE_CORE_UTILITY_HAS_PRIVATE_ASSIGN_HPP

#include <boost/mpl/identity.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/type_traits/integral_constant.hpp>

#include <sake/core/introspection/has_isc.hpp>
#include <sake/core/utility/has_private_operator_assign_fwd.hpp>

namespace sake
{

SAKE_INTROSPECTION_DEFINE_HAS_ISC(
    has_isc_has_private_operator_assign,
    has_private_operator_assign
)

/*******************************************************************************
 * struct has_private_operator_assign<T>
 ******************************************************************************/

template< class T >
struct has_private_operator_assign
    : extension::has_private_operator_assign<T>
{ };

/*******************************************************************************
 * struct extension::has_private_operator_assign< T, Enable = void >
 ******************************************************************************/

namespace extension
{

template< class T, class Enable /*= void*/ >
struct has_private_operator_assign
    : default_impl::has_private_operator_assign<T>
{ };

} // namespace extension

/*******************************************************************************
 * struct default_impl::has_private_operator_assign<T>
 ******************************************************************************/

namespace default_impl
{

template< class T >
struct has_private_operator_assign
    : sake::has_isc_has_private_operator_assign<
          T, boost::mpl::quote1< boost::mpl::identity > >
{ };

} // namespace default_impl

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_HAS_PRIVATE_ASSIGN_HPP

/*******************************************************************************
 * sake/core/utility/hash_value.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * hash_value(T const &) -> std::size_t
 * struct functional::hash_value
 * struct functional::hash
 *
 * Returns the hash value of its argument.
 *
 * sake::hash_value(T const &) is implemented in terms of
 * - T::hash_value(), if available; else
 * - hash_value(T const &) (unqualified, hence subject to ADL), if available;
 *   else
 * - boost::hash<T>()(T const &).
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_HASH_VALUE_HPP
#define SAKE_CORE_UTILITY_HASH_VALUE_HPP

#include <cstddef>

#include <boost/functional/hash.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/core/introspection/is_callable_function.hpp>
#include <sake/core/introspection/is_callable_member_function.hpp>
#include <sake/core/utility/dispatch_priority_tag.hpp>
#include <sake/core/utility/workaround.hpp>

namespace sake
{

namespace hash_value_private
{

template< class T >
inline std::size_t
impl(T const & x);

} // namespace hash_value_private

/*******************************************************************************
 * hash_value(T const & x) -> std::size_t
 * struct functional::hash_value
 * struct functional::hash
 ******************************************************************************/

namespace functional
{

struct hash_value
{
    typedef std::size_t result_type;

    template< class T >
    std::size_t operator()(T const & x) const
    { return hash_value_private::impl(x); }
};

typedef hash_value hash;

} // namespace functional

#ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
namespace hash_value_adl_barrier
{ functional::hash_value const hash_value = { }; }
using namespace hash_value_adl_barrier;
#else // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
functional::hash_value const hash_value = { };
#endif // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS

} // namespace sake

namespace sake_hash_value_private
{

#define SAKE_INTROSPECTION_TRAIT_NAME    is_callable
#define SAKE_INTROSPECTION_FUNCTION_NAME hash_value
#define SAKE_INTROSPECTION_FUNCTION_ARITY_LIMITS ( 1, 1 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_FUNCTION()

template< class T >
inline std::size_t
adl(T const & x)
{ return hash_value(x); }

} // namespace sake_hash_value_private

namespace sake
{

namespace hash_value_private
{

#define SAKE_INTROSPECTION_TRAIT_NAME           is_callable_mem_fun
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME hash_value
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY_LIMITS ( 0, 0 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_MEMBER_FUNCTION()

template< class T >
typename boost::enable_if_c<
    is_callable_mem_fun< T const &, std::size_t ( ) >::value,
    std::size_t
>::type
dispatch(T const & x, sake::dispatch_priority_tag<2>)
{ return x.hash_value(); }

template< class T >
typename boost::enable_if_c<
    ::sake_hash_value_private::is_callable< std::size_t ( T const & ) >::value,
    std::size_t
>::type
dispatch(T const & x, sake::dispatch_priority_tag<1>)
{ return ::sake_hash_value_private::adl(x); }

template< class T >
inline std::size_t
dispatch(T const & x, sake::dispatch_priority_tag<0>)
{ return boost::hash<T>()(x); }

template< class T >
inline std::size_t
impl(T const & x)
{ return hash_value_private::dispatch(x, sake::dispatch_priority_tag<2>()); }

} // namespace hash_value_private

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_HASH_VALUE_HPP

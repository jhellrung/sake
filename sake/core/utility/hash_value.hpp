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
 * sake::hash_value(T) is implemented in terms of
 * - T::hash_value(), if available; else
 * - hash_value(T) (unqualified, hence subject to ADL), if available;
 *   else
 * - boost::hash<T>()(T).
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_HASH_VALUE_HPP
#define SAKE_CORE_UTILITY_HASH_VALUE_HPP

#include <cstddef>

#include <boost/functional/hash.hpp>

#include <sake/boost_ext/mpl/if.hpp>

#include <sake/core/config.hpp>
#include <sake/core/introspection/is_callable_function.hpp>
#include <sake/core/introspection/is_callable_member_function.hpp>
#include <sake/core/utility/int_tag.hpp>

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
{ sake::functional::hash_value const hash_value = { }; }
using namespace hash_value_adl_barrier;
#else // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
sake::functional::hash_value const hash_value = { };
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
inline std::size_t
dispatch(T const & x, sake::int_tag<2>)
{ return x.hash_value(); }

template< class T >
inline std::size_t
dispatch(T const & x, sake::int_tag<1>)
{ return ::sake_hash_value_private::adl(x); }

template< class T >
inline std::size_t
dispatch(T const & x, sake::int_tag<0>)
{ return boost::hash<T>()(x); }

template< class T >
inline std::size_t
impl(T const & x)
{
    typedef typename boost_ext::mpl::
         if_<
        hash_value_private::is_callable_mem_fun< T const &, std::size_t ( ) >,
        sake::int_tag<2>
    >::type::template
    else_if <
        ::sake_hash_value_private::is_callable< std::size_t ( T const & ) >,
        sake::int_tag<1>
    >::type::template
    else_   <
        sake::int_tag<0>
    >::type int_tag_;
    return hash_value_private::dispatch(x, int_tag_());
}

} // namespace hash_value_private

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_HASH_VALUE_HPP

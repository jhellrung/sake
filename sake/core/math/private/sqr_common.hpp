/*******************************************************************************
 * sake/core/math/private/sqr_common.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_PRIVATE_SQR_COMMON_HPP
#define SAKE_CORE_MATH_PRIVATE_SQR_COMMON_HPP

#include <sake/core/introspection/is_callable_function.hpp>
#include <sake/core/introspection/is_callable_member_function.hpp>

namespace sake
{

namespace sqr_ip_private
{

#define SAKE_INTROSPECTION_TRAIT_NAME           is_callable_mem_fun
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME sqr_ip
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY_LIMITS ( 0, 0 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_MEMBER_FUNCTION()

} // namespace sqr_ip_private

} // namespace sake

namespace sake_sqr_ip_private
{

#define SAKE_INTROSPECTION_TRAIT_NAME    is_callable
#define SAKE_INTROSPECTION_FUNCTION_NAME sqr_ip
#define SAKE_INTROSPECTION_FUNCTION_ARITY_LIMITS ( 1, 1 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_FUNCTION()

template< class T >
inline void
adl(T& x)
{ sqr_ip(x); }

} // namespace sake_sqr_ip_private

#endif // #ifndef SAKE_CORE_MATH_PRIVATE_SQR_COMMON_HPP

/*******************************************************************************
 * sake/core/introspection/is_callable_mem_fun_get.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct is_callable_mem_fun_get<
 *     T,
 *     Signature = void,
 *     ResultMetafunction = boost::mpl::always< boost::true_type >
 * >
 ******************************************************************************/

#ifndef SAKE_CORE_INTROSPECTION_IS_CALLABLE_MEM_FUN_GET_HPP
#define SAKE_CORE_INTROSPECTION_IS_CALLABLE_MEM_FUN_GET_HPP

#include <sake/core/introspection/is_callable_member_function.hpp>

namespace sake
{

#define SAKE_INTROSPECTION_TRAIT_NAME           is_callable_mem_fun_get
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME get
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_MEMBER_FUNCTION()

} // namespace sake

#endif // #ifndef SAKE_CORE_INTROSPECTION_IS_CALLABLE_MEM_FUN_GET_HPP

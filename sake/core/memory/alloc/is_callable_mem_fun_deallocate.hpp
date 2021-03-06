/*******************************************************************************
 * sake/core/memory/alloc/is_callable_mem_fun_deallocate.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_MEMORY_ALLOC_IS_CALLABLE_MEM_FUN_DEALLOCATE_HPP
#define SAKE_CORE_MEMORY_ALLOC_IS_CALLABLE_MEM_FUN_DEALLOCATE_HPP

#include <sake/core/introspection/is_callable_member_function.hpp>

namespace sake
{

namespace alloc
{

#define SAKE_INTROSPECTION_TRAIT_NAME           is_callable_mem_fun_deallocate
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME deallocate
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY_LIMITS ( 2, 2 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_MEMBER_FUNCTION()

} // namespace alloc

} // namespace sake

#endif // #ifndef SAKE_CORE_MEMORY_ALLOC_IS_CALLABLE_MEM_FUN_DEALLOCATE_HPP

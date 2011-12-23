/*******************************************************************************
 * sake/core/introspection/has_type_const_pointer.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct has_type_const_pointer<T>
 ******************************************************************************/

#ifndef SAKE_CORE_INTROSPECTION_HAS_TYPE_CONST_POINTER_HPP
#define SAKE_CORE_INTROSPECTION_HAS_TYPE_CONST_POINTER_HPP

#include <sake/core/introspection/has_type.hpp>

namespace sake
{

SAKE_INTROSPECTION_DEFINE_HAS_TYPE( has_type_const_pointer, const_pointer )

} // namespace sake

#endif // #ifndef SAKE_CORE_INTROSPECTION_HAS_TYPE_CONST_POINTER_HPP

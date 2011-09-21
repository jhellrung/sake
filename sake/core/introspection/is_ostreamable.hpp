/*******************************************************************************
 * sake/core/introspection/is_ostreamable.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct is_ostreamable<T>
 *
 * is_ostreamable is a metafunction which returns true if
 *     declval< std::ostream& >() << declval<T>()
 * is a syntactically valid expression.
 ******************************************************************************/

#ifndef SAKE_CORE_INTROSPECTION_IS_OSTREAMABLE_HPP
#define SAKE_CORE_INTROSPECTION_IS_OSTREAMABLE_HPP

#include <iosfwd>

#include <sake/core/introspection/has_operator_shift_left.hpp>

namespace sake
{

template< class T >
struct is_ostreamable
    : sake::has_operator_shift_left< std::ostream&, T, std::ostream& >
{ };

} // namespace sake

#endif // #ifndef SAKE_CORE_INTROSPECTION_IS_OSTREAMABLE_HPP

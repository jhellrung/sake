/*******************************************************************************
 * sake/core/introspection/is_istreamable.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct is_istreamable<T>
 *
 * is_istreamable is a metafunction which returns true if
 *     declval< std::ostream& >() >> declref<T>()
 * is a syntactically valid expression.
 ******************************************************************************/

#ifndef SAKE_CORE_INTROSPECTION_IS_ISTREAMABLE_HPP
#define SAKE_CORE_INTROSPECTION_IS_ISTREAMABLE_HPP

#include <iosfwd>

#include <sake/boost_ext/type_traaits/add_lvalue_reference.hpp>

#include <sake/core/introspection/has_operator_shift_right.hpp>

namespace sake
{

template< class T >
struct is_istreamable
    : sake::has_operator_shift_right<
          std::ostream&,
          typename boost_ext::add_lvalue_reference<T>::type,
          std::ostream&
      >
{ };

} // namespace sake

#endif // #ifndef SAKE_CORE_INTROSPECTION_IS_ISTREAMABLE_HPP

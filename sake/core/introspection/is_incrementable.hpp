/*******************************************************************************
 * sake/core/introspection/is_incrementable.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_INTROSPECTION_IS_INCREMENTABLE_HPP
#define SAKE_CORE_INTROSPECTION_IS_INCREMENTABLE_HPP

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/remove_reference.hpp>

#include <sake/core/introspection/has_operator_post_increment.hpp>
#include <sake/core/introspection/has_operator_pre_increment.hpp>

namespace sake
{

template< class T >
struct is_incrementable
    : boost_ext::mpl::and2<
          has_operator_pre_increment<
              typename boost_ext::add_reference<T>::type,
              typename boost_ext::add_reference<T>::type
          >,
          has_operator_post_increment<
              typename boost_ext::add_reference<T>::type,
              typename boost_ext::remove_reference<T>::type
          >
      >
{ };

} // namespace sake

#endif // #ifndef SAKE_CORE_INTROSPECTION_IS_INCREMENTABLE_HPP

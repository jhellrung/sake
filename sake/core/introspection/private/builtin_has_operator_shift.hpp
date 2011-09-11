/*******************************************************************************
 * core/introspection/private/builtin_has_operator_shift.hpp
 *
 * Copyright 2010, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_INTROSPECTION_PRIVATE_BUILTIN_HAS_OPERATOR_SHIFT_HPP
#define SAKE_CORE_INTROSPECTION_PRIVATE_BUILTIN_HAS_OPERATOR_SHIFT_HPP

#include <boost/mpl/and.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/type_traits/is_integral.hpp>

#include <sake/boost_ext/type_traits/is_convertible.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

namespace sake
{

namespace introspection_private
{

template<
    class T, class U, class Result, class ResultCond,
    class T_ = typename boost_ext::remove_qualifiers<T>::type,
    class U_ = typename boost_ext::remove_qualifiers<U>::type
>
struct builtin_has_operator_shift
    : boost::mpl::and_<
          boost::is_integral< T_ >,
          boost::is_integral< U_ >,
          boost_ext::is_convertible< T_, Result >,
          boost::mpl::apply1< ResultCond, T_ >
      >
{ };

} // namespace introspection_private

} // namespace sake

#endif // #ifndef SAKE_CORE_INTROSPECTION_PRIVATE_BUILTIN_HAS_OPERATOR_SHIFT_HPP

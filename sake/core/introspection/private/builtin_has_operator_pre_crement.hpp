/*******************************************************************************
 * sake/core/introspection/private/builtin_has_operator_pre_crement.hpp
 *
 * Copyright 2010, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_INTROSPECTION_PRIVATE_BUILTIN_HAS_OPERATOR_PRE_CREMENT_HPP
#define SAKE_CORE_INTROSPECTION_PRIVATE_BUILTIN_HAS_OPERATOR_PRE_CREMENT_HPP

#include <boost/mpl/apply.hpp>
#include <boost/type_traits/integral_constant.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/introspection/private/builtin_has_operator_crement.hpp>

namespace sake
{

namespace introspection_private
{

template< class T, class Result, class ResultPred >
struct builtin_has_operator_pre_crement
    : boost::false_type
{ };

template< class T, class Result, class ResultPred >
struct builtin_has_operator_pre_crement< T&, Result, ResultPred >
    : boost_ext::mpl::and3<
          introspection_private::builtin_has_operator_crement< T& >,
          boost_ext::is_convertible< T&, Result >,
          boost::mpl::apply1< ResultPred, T& >
      >
{ };

} // namespace introspection_private

} // namespace sake

#endif // #ifndef SAKE_CORE_INTROSPECTION_PRIVATE_BUILTIN_HAS_OPERATOR_PRE_CREMENT_HPP

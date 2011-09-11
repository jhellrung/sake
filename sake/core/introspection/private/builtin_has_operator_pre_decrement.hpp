/*******************************************************************************
 * core/introspection/private/builtin_has_operator_pre_decrement.hpp
 *
 * Copyright 2010, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_INTROSPECTION_PRIVATE_BUILTIN_HAS_OPERATOR_PRE_DECREMENT_HPP
#define SAKE_CORE_INTROSPECTION_PRIVATE_BUILTIN_HAS_OPERATOR_PRE_DECREMENT_HPP

#include <boost/type_traits/integral_constant.hpp>

#include <sake/core/introspection/private/builtin_has_operator_pre_crement.hpp>

namespace sake
{

namespace introspection_private
{

template< class T, class Result, class ResultMetafunction >
struct builtin_has_operator_pre_decrement
    : boost::false_type
{ };

template< class T, class Result, class ResultMetafunction >
struct builtin_has_operator_pre_decrement< T&, Result, ResultMetafunction >
    : builtin_has_operator_pre_crement< T&, Result, ResultMetafunction >
{ };

template< class Result, class ResultMetafunction >
struct builtin_has_operator_pre_decrement< bool&, Result, ResultMetafunction >
    : boost::false_type
{ };

} // namespace introspection_private

} // namespace sake

#endif // #ifndef SAKE_CORE_INTROSPECTION_PRIVATE_BUILTIN_HAS_OPERATOR_PRE_DECREMENT_HPP

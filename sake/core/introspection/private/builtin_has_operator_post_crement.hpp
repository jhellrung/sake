/*******************************************************************************
 * core/introspection/detail/builtin_has_operator_post_crement.hpp
 *
 * Copyright 2010, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_INTROSPECTION_PRIVATE_BUILTIN_HAS_OPERATOR_POST_CREMENT_HPP
#define SAKE_CORE_INTROSPECTION_PRIVATE_BUILTIN_HAS_OPERATOR_POST_CREMENT_HPP

#include <boost/mpl/and.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/type_traits/integral_constant.hpp>

#include <sake/boost_ext/type_traits/is_convertible.hpp>
#include <sake/core/introspection/private/builtin_has_operator_crement.hpp>
#include <sake/core/utility/workaround.hpp>

namespace sake
{

namespace introspection_private
{

template< class T, class Result, class ResultMetafunction >
struct builtin_has_operator_post_crement
    : boost::false_type
{ };

template< class T, class Result, class ResultMetafunction >
struct builtin_has_operator_post_crement< T&, Result, ResultMetafunction >
    : boost::mpl::and_<
          introspection_private::builtin_has_operator_crement< T& >,
          boost_ext::is_convertible< T, Result >,
          boost::mpl::apply1< ResultMetafunction, T >
      >
{ };

// For some reason, the type of declval< bool& >()++ is bool& on MSVC9.
#if SAKE_WORKAROUND_MSVC_VERSION_LESS_EQUAL( 1500 )

template< class Result, class ResultMetafunction >
struct builtin_has_operator_post_crement< bool&, Result, ResultMetafunction >
    : boost::mpl::and_<
          boost_ext::is_convertible< bool&, Result >,
          boost::mpl::apply1< ResultMetafunction, bool& >
      >
{ };

#endif // #if SAKE_WORKAROUND_MSVC_VERSION_LESS_EQUAL( 1500 )

} // namespace introspection_private

} // namespace sake

#endif // #ifndef SAKE_CORE_INTROSPECTION_PRIVATE_BUILTIN_HAS_OPERATOR_POST_CREMENT_HPP

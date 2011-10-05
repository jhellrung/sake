/*******************************************************************************
 * sake/core/introspection/private/builtin_has_operator_op_assign.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_INTROSPECTION_PRIVATE_BUILTIN_HAS_OPERATOR_OP_ASSIGN_HPP
#define SAKE_CORE_INTROSPECTION_PRIVATE_BUILTIN_HAS_OPERATOR_OP_ASSIGN_HPP

#include <boost/mpl/always.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/type_traits/integral_constant.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>

namespace sake
{

namespace introspection_private
{

template< template< class, class, class, class > class BuiltinHasOperatorOp >
struct builtin_has_operator_op_assign
{
    template< class T, class U, class Result, class ResultPred >
    struct apply
        : boost::false_type
    { };

    template< class T, class U, class Result, class ResultPred >
    struct apply< T const &, U, Result, ResultPred >
        : boost::false_type
    { };

    template< class T, class U, class Result, class ResultPred >
    struct apply< T&, U, Result, ResultPred >
        : boost_ext::mpl::and3<
              BuiltinHasOperatorOp< T, U, T&, boost::mpl::always< boost::true_type > >,
              boost_ext::is_convertible< T&, Result >,
              boost::mpl::apply1< ResultPred, T& >
          >
    { };
};

} // namespace introspection_private

} // namespace sake

#endif // #ifndef SAKE_CORE_INTROSPECTION_PRIVATE_BUILTIN_HAS_OPERATOR_OP_ASSIGN_HPP

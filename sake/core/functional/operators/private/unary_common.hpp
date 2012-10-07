/*******************************************************************************
 * sake/core/functional/operators/private/unary_common.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_FUNCTIONAL_OPERATORS_PRIVATE_UNARY_COMMON_HPP
#define SAKE_CORE_FUNCTIONAL_OPERATORS_PRIVATE_UNARY_COMMON_HPP

#include <boost/logic/tribool.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/integral_promotion.hpp>

#include <sake/boost_ext/type_traits/has_qualifier.hpp>

#include <sake/core/math/indeterminate_fwd.hpp>

namespace sake
{

namespace operators
{

namespace result_of
{

namespace default_impl
{

template< class T >
struct unary_result_types
{
    BOOST_STATIC_ASSERT((!boost_ext::has_qualifier<T>::value));
    typedef boost::mpl::vector6<
        T,
        T&, // op++, op--
        bool, // logical
        boost::logic::tribool, // logical
        sake::functional::indeterminate, // logical
        typename boost::integral_promotion<T>::type // op~
    > type;
};

} // namespace default_impl

} // namespace result_of

} // namespace operators

} // namespace sake

#endif // #ifndef SAKE_CORE_FUNCTIONAL_OPERATORS_PRIVATE_UNARY_COMMON_HPP

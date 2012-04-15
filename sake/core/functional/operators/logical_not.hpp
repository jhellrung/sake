/*******************************************************************************
 * sake/core/functional/operators/logical_not.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * operators::logical_not(T&& x)
 *     -> operators::result_of::logical_not<T>::type
 * struct operators::functional::logical_not
 *
 * operators::not_(T&& x)
 *     -> operators::result_of::not_<T>::type
 * struct operators::functional::not_
 *
 * struct operators::result_of::logical_not<T>
 * struct operators::result_of::extension::logical_not< T, Enable = void >
 * struct operators::result_of::default_impl::logical_not<T>
 ******************************************************************************/

#ifndef SAKE_CORE_FUNCTIONAL_OPERATORS_LOGICAL_NOT_HPP
#define SAKE_CORE_FUNCTIONAL_OPERATORS_LOGICAL_NOT_HPP

#define SAKE_OPERATORS_NAME logical_not
#define SAKE_OPERATORS_OP   !
#include <sake/core/functional/operators/private/unary.ipp>

namespace sake
{

namespace operators
{

namespace result_of
{

template< class T >
struct not_
    : logical_not<T>
{ };

} // namespace result_of

namespace functional
{

typedef logical_not not_;

} // namespace functional

sake::operators::functional::not_ const not_ = { };

} // namespace operators

} // namespace sake

#endif // #ifndef SAKE_CORE_FUNCTIONAL_OPERATORS_LOGICAL_NOT_HPP

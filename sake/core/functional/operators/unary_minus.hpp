/*******************************************************************************
 * sake/core/functional/operators/unary_minus.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * operators::unary_minus(T const & x)
 *     -> operators::result_of::unary_minus<T>::type
 * struct operators::functional::unary_minus
 *
 * struct operators::result_of::unary_minus<T>
 * struct operators::result_of::extension::unary_minus< T, Enable = void >
 * struct operators::result_of::default_impl::unary_minus<T>
 ******************************************************************************/

#ifndef SAKE_CORE_FUNCTIONAL_OPERATORS_UNARY_MINUS_HPP
#define SAKE_CORE_FUNCTIONAL_OPERATORS_UNARY_MINUS_HPP

#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/utility/result_from_metafunction.hpp>

namespace sake
{

namespace operators
{

namespace result_of
{

template< class T >
struct unary_minus;

namespace extension
{
template< class T, class Enable = void >
struct unary_minus;
} // namespace extension

namespace default_impl
{
template< class T >
struct unary_minus;
} // namespace default_impl

/*******************************************************************************
 * struct operators::result_of::unary_minus<T>
 ******************************************************************************/

template< class T >
struct unary_minus
    : extension::unary_minus<
          typename boost_ext::remove_qualifiers<T>::type
      >
{ };

/*******************************************************************************
 * struct operators::result_of::extension::unary_minus< T, Enable = void >
 ******************************************************************************/

namespace extension
{

template< class T, class Enable /*= void*/ >
struct unary_minus
    : default_impl::unary_minus<T>
{ };

} // namespace extension

/*******************************************************************************
 * struct operators::result_of::default_impl::unary_minus<T>
 ******************************************************************************/

namespace default_impl
{

template< class T >
struct unary_minus
    : boost_ext::remove_qualifiers<T>
{ };

} // namespace default_impl

} // namespace result_of

/*******************************************************************************
 * operators::unary_minus(T const & x)
 *     -> operators::result_of::unary_minus<T>::type
 * struct operators::functional::unary_minus
 ******************************************************************************/

namespace functional
{

struct unary_minus
{
    SAKE_RESULT_FROM_METAFUNCTION( result_of::unary_minus, 1 )

    template< class T >
    typename result_of::unary_minus<T>::type
    operator()(T const & x) const
    { return -x; }
};

} // namespace functional

functional::unary_minus const unary_minus = { };

} // namespace operators

} // namespace sake

#endif // #ifndef SAKE_CORE_FUNCTIONAL_OPERATORS_UNARY_MINUS_HPP

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

#include <boost/config.hpp>

#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>
#include <sake/boost_ext/type_traits/remove_rvalue_reference.hpp>

#include <sake/core/expr_traits/best_conversion.hpp>
#include <sake/core/functional/operators/private/logical_common.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/utility/declval.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>

namespace sake
{

namespace operators
{

namespace result_of
{

template< class T >
struct logical_not;

namespace extension
{
template< class T, class Enable = void >
struct logical_not;
} // namespace extension

namespace default_impl
{
template< class T >
struct logical_not;
} // namespace default_impl

/*******************************************************************************
 * struct operators::result_of::logical_not<T>
 ******************************************************************************/

template< class T >
struct logical_not
    : extension::logical_not<T>
{ };

/*******************************************************************************
 * struct operators::result_of::extension::logical_not< T, Enable = void >
 ******************************************************************************/

namespace extension
{

template< class T, class Enable /*= void*/ >
struct logical_not
    : default_impl::logical_not<T>
{ };

} // namespace extension

/*******************************************************************************
 * struct operators::result_of::default_impl::logical_not<T>
 ******************************************************************************/

namespace default_impl
{

template< class T >
struct logical_not
{
    SAKE_EXPR_BEST_CONVERSION_TYPEDEF(
        typename !sake::declval<T>(),
        default_impl::logical_result_types,
        type
    );
}

} // namespace default_impl

} // namespace result_of

/*******************************************************************************
 * operators::logical_not(T&& x)
 *     -> operators::result_of::logical_not<T>::type
 * struct operators::functional::logical_not
 ******************************************************************************/

namespace functional
{

struct logical_not
{
    SAKE_RESULT_FROM_METAFUNCTION( result_of::logical_not, 1 )

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class T >
    typename result_of::logical_not<T>::type
    operator()(T&& x) const
    { return !sake::forward<T>(x); }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class T >
    typename result_of::logical_not<
        typename boost_ext::remove_rvalue_reference< T& >::type
    >::type
    operator()(T& x) const
    { return !x; }

    template< class T >
    typename result_of::logical_not< T const & >::type
    operator()(T const & x) const
    { return !x; }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

};

} // namespace functional

functional::logical_not const logical_not = { };

/*******************************************************************************
 * struct operators::result_of::not_<T>
 * operators::not_(T&& x)
 *     -> operators::result_of::not_<T>::type
 * struct operators::functional::not_
 ******************************************************************************/

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

functional::not_ const not_ = { };

} // namespace operators

} // namespace sake

#endif // #ifndef SAKE_CORE_FUNCTIONAL_OPERATORS_LOGICAL_NOT_HPP

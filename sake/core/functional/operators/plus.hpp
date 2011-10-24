/*******************************************************************************
 * sake/core/functional/operators/plus.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * operators::plus(T0&& x0, T1&& x1)
 *     -> operators::result_of::plus< T0, T1 >::type
 * struct operators::functional::plus
 *
 * struct operators::result_of::plus< T0, T1 >
 * struct operators::result_of::extension::plus0< T0, T1, Enable = void >
 * struct operators::result_of::extension::plus1< T0, T1, Enable = void >
 * struct operators::result_of::default_impl::plus< T0, T1 >
 ******************************************************************************/

#ifndef SAKE_CORE_FUNCTIONAL_OPERATORS_PLUS_HPP
#define SAKE_CORE_FUNCTIONAL_OPERATORS_PLUS_HPP

#include <sake/boost_ext/type_traits/common_type.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/move/forward.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>

namespace sake
{

namespace operators
{

namespace result_of
{

template< class T0, class T1 = T0 >
struct plus;

namespace extension
{
template< class T0, class T1, class Enable = void >
struct plus0;
template< class T0, class T1, class Enable = void >
struct plus1;
} // namespace extension

namespace default_impl
{
template< class T0, class T1 = T0 >
struct plus;
} // namespace default_impl

/*******************************************************************************
 * struct operators::result_of::plus< T0, T1 = T0 >
 ******************************************************************************/

template< class T0, class T1 >
struct plus
    : extension::plus0< T0, T1 >
{ };

/*******************************************************************************
 * struct operators::result_of::extension::plus0< T0, T1, Enable = void >
 * struct operators::result_of::extension::plus1< T0, T1, Enable = void >
 ******************************************************************************/

namespace extension
{

template< class T0, class T1, class Enable >
struct plus0
    : extension::plus1< T0, T1 >
{ };

template< class T0, class T1, class Enable >
struct plus1
    : default_impl::plus< T0, T1 >
{ };

} // namespace extension

/*******************************************************************************
 * struct operators::result_of::default_impl::plus< T0, T1 = T0 >
 ******************************************************************************/

namespace default_impl
{

namespace plus_private
{

template< class T0, class T1 >
struct dispatch
    : boost_ext::common_type< T0, T1 >
{ };

template< class T0, class T1 >
struct dispatch< T0*, T1 >
{ typedef T0* type; };

template< class T0, class T1 >
struct dispatch< T0, T1* >
{ typedef T1* type; };

template< class T0, class T1 >
struct dispatch< T0*, T1* >
{ typedef void type; };

} // namespace plus_private

template< class T0, class T1 >
struct plus
    : plus_private::dispatch<
          typename boost_ext::remove_qualifiers< T0 >::type,
          typename boost_ext::remove_qualifiers< T1 >::type
      >
{ };

} // namespace default_impl

} // namespace result_of

/*******************************************************************************
 * operators::plus(T0&& x0, T1&& x1)
 *     -> operators::result_of::plus< T0, T1 >::type
 * struct operators::functional::plus
 ******************************************************************************/

namespace functional
{

struct plus
{
    SAKE_RESULT_FROM_METAFUNCTION( result_of::plus, 2 )

    template< class T0, class T1 >
    typename result_of::plus< SAKE_FWD_PARAM( T0 ), SAKE_FWD_PARAM( T1 ) >::type
    operator()(SAKE_FWD_REF( T0 ) x0, SAKE_FWD_REF( T1 ) x1) const
    { return sake::forward< T0 >(x0) + sake::forward< T1 >(x1); }
};

} // namespace functional

functional::plus const plus = { };

} // namespace operators

} // namespace sake

#endif // #ifndef SAKE_CORE_FUNCTIONAL_OPERATORS_PLUS_HPP

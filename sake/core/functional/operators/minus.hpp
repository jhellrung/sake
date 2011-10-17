/*******************************************************************************
 * sake/core/functional/operators/minus.ipp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * operators::minus(T0 const & x0, T1 const & x1)
 *     -> operators::result_of::minus< T0, T1 >::type
 * struct operators::functional::minus
 *
 * struct operators::result_of::minus< T0, T1 >
 * struct operators::result_of::extension::minus0< T0, T1, Enable = void >
 * struct operators::result_of::extension::minus1< T0, T1, Enable = void >
 * struct operators::result_of::default_impl::minus< T0, T1 >
 ******************************************************************************/

#ifndef SAKE_CORE_FUNCTIONAL_OPERATORS_MINUS_HPP
#define SAKE_CORE_FUNCTIONAL_OPERATORS_MINUS_HPP

#include <cstddef>

#include <sake/boost_ext/type_traits/common_type.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/utility/result_from_metafunction.hpp>

namespace sake
{

namespace operators
{

namespace result_of
{

template< class T0, class T1 = T0 >
struct minus;

namespace extension
{
template< class T0, class T1, class Enable = void >
struct minus0;
template< class T0, class T1, class Enable = void >
struct minus1;
} // namespace extension

namespace default_impl
{
template< class T0, class T1 = T0 >
struct minus;
} // namespace default_impl

/*******************************************************************************
 * struct operators::result_of::minus< T0, T1 = T0 >
 ******************************************************************************/

template< class T0, class T1 >
struct minus
    : extension::minus0<
          typename boost_ext::remove_qualifiers< T0 >::type,
          typename boost_ext::remove_qualifiers< T1 >::type
      >
{ };

/*******************************************************************************
 * struct operators::result_of::extension::minus0< T0, T1, Enable = void >
 * struct operators::result_of::extension::minus1< T0, T1, Enable = void >
 ******************************************************************************/

namespace extension
{

template< class T0, class T1, class Enable >
struct minus0
    : extension::minus1< T0, T1 >
{ };

template< class T0, class T1, class Enable >
struct minus1
    : default_impl::minus< T0, T1 >
{ };

} // namespace extension

/*******************************************************************************
 * struct operators::result_of::default_impl::minus< T0, T1 = T0 >
 ******************************************************************************/

namespace default_impl
{

namespace minus_private
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
{ typedef void type; };

template< class T0, class T1 >
struct dispatch< T0*, T1* >
{ typedef std::ptrdiff_t type; };

} // namespace minus_private

template< class T0, class T1 >
struct minus
    : minus_private::dispatch<
          typename boost_ext::remove_qualifiers< T0 >::type,
          typename boost_ext::remove_qualifiers< T1 >::type
      >
{ };

} // namespace default_impl

} // namespace result_of

/*******************************************************************************
 * operators::minus(T0 const & x0, T1 const & x1)
 *     -> operators::result_of::minus< T0, T1 >::type
 * struct operators::functional::minus
 ******************************************************************************/

namespace functional
{

struct minus
{
    SAKE_RESULT_FROM_METAFUNCTION( result_of::minus, 2 )

    template< class T0, class T1 >
    typename result_of::minus< T0, T1 >::type
    operator()(T0 const & x0, T1 const & x1) const
    { return x0 - x1; }
};

} // namespace functional

functional::minus const minus = { };

} // namespace operators

} // namespace sake

#endif // #ifndef SAKE_CORE_FUNCTIONAL_OPERATORS_MINUS_HPP

/*******************************************************************************
 * sake/core/functional/operators/bracket.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * operators::bracket(T0&& x0, T1&& x1)
 *     -> operators::result_of::bracket< T0, T1 >::type
 * struct operators::functional::bracket
 *
 * struct operators::result_of::bracket< T0, T1 >
 * struct operators::result_of::extension::bracket< T0, T1, Enable = void >
 * struct operators::result_of::default_impl::bracket< T0, T1 >
 ******************************************************************************/

#ifndef SAKE_CORE_FUNCTIONAL_OPERATORS_BRACKET_HPP
#define SAKE_CORE_FUNCTIONAL_OPERATORS_BRACKET_HPP

#define SAKE_OPERATORS_NAME          bracket
#define SAKE_OPERATORS_APPLY( x, y ) x[y]
#define SAKE_OPERATORS_NO_DEFINE_RESULT_OF_DEFAULT_IMPL_IMPL
#include <sake/core/functional/operators/private/binary.ipp>

#include <boost/type_traits/remove_const.hpp>

#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>
#include <sake/boost_ext/type_traits/remove_reference.hpp>

#include <sake/core/iterator/is_iterator.hpp>
#include <sake/core/iterator/traits.hpp>
#include <sake/core/range/traits.hpp>

namespace sake {
namespace operators {
namespace result_of {
namespace default_impl {
namespace bracket_private {

template< class T0, class T1 >
struct dispatch_index< T0, T1* >

template<
    class T0, class T1,
    bool = sake::is_iterator< typename boost::remove_const< T0 >::type >::value
>
struct dispatch;

template< class T0, class T1 >
struct dispatch< T0, T1, true >
    : sake::iterator_reference< typename boost::remove_const< T0 >::type >
{ };

template< class T0, class T1 >
struct dispatch< T0, T1*, false >
    : dispatch< T1*, T0 >
{ };

template< class T0, class T1 >
struct dispatch< T0, T1, false >
    : sake::range_reference< T0 >
{ };

template< class T0, class T1 >
struct impl
    : dispatch<
          typename boost_ext::remove_reference< T0 >::type,
          typename boost_ext::remove_qualifiers< T1 >::type
      >
{ };

} // namespace bracket_private
} // namespace default_impl
} // namespace result_of
} // namespace operators
} // namespace sake

#endif // #ifndef SAKE_CORE_FUNCTIONAL_OPERATORS_BRACKET_HPP

/*******************************************************************************
 * sake/core/iterator/private/facade/function_prototype.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_ITERATOR_FACADE_function_prototype( prefix, result, name )
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_PRIVATE_FACADE_FUNCTION_PROTOTYPE_HPP
#define SAKE_CORE_ITERATOR_PRIVATE_FACADE_FUNCTION_PROTOTYPE_HPP

#include <boost/mpl/apply.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>

#define SAKE_ITERATOR_FACADE_function_prototype( prefix, result, name ) \
template< class Derived0, class V0, class R0, class F0, class T0, class I0, class B0, \
          class Derived1, class V1, class R1, class F1, class T1, class I1, class B1 > \
prefix typename ::sake::iterator_facade_private:: \
    lazy_enable_if_is_interconvertible< Derived0, Derived1, result >::type \
name(::sake::iterator_facade< Derived0, V0, R0, F0, T0, I0, B0 > const & i0, \
     ::sake::iterator_facade< Derived1, V1, R1, F1, T1, I1, B1 > const & i1)

namespace sake
{

namespace iterator_facade_private
{

template< class T, class U, class Result >
struct lazy_enable_if_is_interconvertible
    : boost::lazy_enable_if_c<
          boost_ext::mpl::or2<
              boost_ext::is_convertible<T,U>,
              boost_ext::is_convertible<U,T>
          >::value,
          boost::mpl::apply2< Result, T, U >
      >
{ };

} // namespace iterator_facade_private

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_PRIVATE_FACADE_FUNCTION_PROTOTYPE_HPP

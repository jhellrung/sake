/*******************************************************************************
 * sake/core/utility/declval.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * declval<T>()  -> T
 *
 * declcval<T>() -> boost::add_const<T>::type
 * declvval<T>() -> boost::add_volatile<T>::type
 * declcvval<T>() -> boost::add_cv<T>::type
 *
 * declref<T>()  -> boost_ext::add_reference<T>::type
 * declcref<T>() -> boost_ext::add_reference_add_const<T>::type
 * declvref<T>() -> boost_ext::add_reference_add_volatile<T>::type
 * declcvref<T>() -> boost_ext::add_reference_add_cv<T>::type
 *
 * declval can be used to syntactically represent an "rvalue correct" instance
 * of a type.
 *
 * This is useful for concept checking classes and template metaprogramming.
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_DECLVAL_HPP
#define SAKE_CORE_UTILITY_DECLVAL_HPP

#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/add_cv.hpp>
#include <boost/type_traits/add_volatile.hpp>

#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/add_reference_add_const.hpp>
#include <sake/boost_ext/type_traits/add_reference_add_cv.hpp>
#include <sake/boost_ext/type_traits/add_reference_add_volatile.hpp>

namespace sake
{

template< class T > T declval();

template< class T > typename boost::add_const<T>::type declcval();
template< class T > typename boost::add_volatile<T>::type declvval();
template< class T > typename boost::add_cv<T>::type declcvval();

template< class T > typename boost_ext::add_reference<T>::type declref();
template< class T > typename boost_ext::add_reference_add_const<T>::type declcref();
template< class T > typename boost_ext::add_reference_add_volatile<T>::type declvref();
template< class T > typename boost_ext::add_reference_add_cv<T>::type declcvref();

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_DECLVAL_HPP

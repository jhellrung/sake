/*******************************************************************************
 * sake/core/utility/declval.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * declval<T>()       -> T
 * declval_const<T>() -> boost::add_const<T>::type
 * declval_ref<T>()   -> boost_ext::add_reference<T>::type
 * declval_cref<T>()  -> boost_ext::add_reference_add_const<T>::type
 *
 * declval can be used to syntactically represent an "rvalue correct" instance
 * of a type.
 *
 * This is useful for concept checking classes and template metaprogramming.
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_DECLVAL_HPP
#define SAKE_CORE_UTILITY_DECLVAL_HPP

#include <boost/type_traits/add_const.hpp>

#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/add_reference_add_const.hpp>

namespace sake
{

template< class T > T declval();
template< class T > typename boost::add_const<T>::type declval_const();
template< class T > typename boost_ext::add_reference<T>::type declval_ref();
template< class T > typename boost_ext::add_reference_add_const<T>::type declval_cref();

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_DECLVAL_HPP

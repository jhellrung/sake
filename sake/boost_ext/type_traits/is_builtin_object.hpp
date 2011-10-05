/*******************************************************************************
 * sake/boost_ext/type_traits/is_builtin_object.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::is_builtin_object<T>
 *
 * is_builtin_object is a Boost.MPL metafunction that evaluates to true if its
 * argument is an object type that is neither a class type nor a union type.
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_TYPE_TRAITS_IS_BUILTIN_OBJECT_HPP
#define SAKE_BOOST_EXT_TYPE_TRAITS_IS_BUILTIN_OBJECT_HPP

#include <boost/type_traits/is_class.hpp>
#include <boost/type_traits/is_object.hpp>
#include <boost/type_traits/is_union.hpp>

namespace sake
{

namespace boost_ext
{

template< class T >
struct is_builtin_object
{
    static bool const value =
        boost::is_object<T>::value
    && !boost::is_class<T>::value
    && !boost::is_union<T>::value;
    typedef is_builtin_object type;
};

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_TYPE_TRAITS_IS_BUILTIN_HPP

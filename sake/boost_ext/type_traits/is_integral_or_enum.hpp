/*******************************************************************************
 * sake/boost_ext/type_traits/is_integral_or_enum.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::is_integral_or_enum<T>
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_TYPE_TRAITS_IS_INTEGRAL_OR_ENUM_HPP
#define SAKE_BOOST_EXT_TYPE_TRAITS_IS_INTEGRAL_OR_ENUM_HPP

#include <boost/type_traits/is_enum.hpp>
#include <boost/type_traits/is_integral.hpp>

namespace sake
{

namespace boost_ext
{

template< class T >
struct is_integral_or_enum
{
    static bool const value =
        boost::is_integral<T>::value
     || boost::is_enum<T>::value;
    typedef is_integral_or_enum type;
};

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_TYPE_TRAITS_IS_INTEGRAL_OR_ENUM_HPP

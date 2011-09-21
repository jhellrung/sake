/*******************************************************************************
 * sake/boost_ext/type_traits/is_convertible.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::is_convertible< From, To >
 *
 * This *should* be nearly identical to boost::is_convertible, though this
 * implementation will be consistent with SAKE_EXPR_IS_CONVERTIBLE.  One major
 * difference is that is_convertible< From, void > is always true.
 * Additionally, this version of is_convertible is specialized for void types
 * (void is only convertible to void), and it addresses some potential compiler
 * warnings.
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_TYPE_TRAITS_IS_CONVERTIBLE_HPP
#define SAKE_BOOST_EXT_TYPE_TRAITS_IS_CONVERTIBLE_HPP

#include <boost/type_traits/integral_constant.hpp>

#include <sake/core/expr_traits/is_convertible.hpp>
#include <sake/core/utility/declval.hpp>

namespace sake
{

namespace boost_ext
{

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4244 ) // warning C4244: 'argument' : conversion from 'From' to 'To', possible loss of data
#pragma warning( disable : 4800 ) // warning C4800: 'argument *' : forcing value to bool 'true' or 'false' (performance warning)
#endif // #ifdef _MSC_VER

template< class From, class To >
struct is_convertible
{
    static const bool value = SAKE_EXPR_IS_CONVERTIBLE( sake::declval< From >(), To );
    typedef is_convertible type;
};

#ifdef _MSC_VER
#pragma warning( pop )
#endif // #ifdef _MSC_VER

template<> struct is_convertible< void, void > : boost::true_type { };
template<> struct is_convertible< void, void const > : boost::true_type { };
template<> struct is_convertible< void, void volatile > : boost::true_type { };
template<> struct is_convertible< void, void const volatile > : boost::true_type { };
template<> struct is_convertible< void const, void > : boost::true_type { };
template<> struct is_convertible< void const, void const > : boost::true_type { };
template<> struct is_convertible< void const, void volatile > : boost::true_type { };
template<> struct is_convertible< void const, void const volatile > : boost::true_type { };
template<> struct is_convertible< void volatile, void > : boost::true_type { };
template<> struct is_convertible< void volatile, void const > : boost::true_type { };
template<> struct is_convertible< void volatile, void volatile > : boost::true_type { };
template<> struct is_convertible< void volatile, void const volatile > : boost::true_type { };
template<> struct is_convertible< void const volatile, void > : boost::true_type { };
template<> struct is_convertible< void const volatile, void const > : boost::true_type { };
template<> struct is_convertible< void const volatile, void volatile > : boost::true_type { };
template<> struct is_convertible< void const volatile, void const volatile > : boost::true_type { };

template< class To > struct is_convertible< void , To > : boost::false_type { };
template< class To > struct is_convertible< void const, To > : boost::false_type { };
template< class To > struct is_convertible< void volatile, To > : boost::false_type { };
template< class To > struct is_convertible< void const volatile, To > : boost::false_type { };

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_TYPE_TRAITS_IS_CONVERTIBLE_HPP

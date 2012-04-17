/*******************************************************************************
 * sake/core/utility/memberwise/private/is_assignable.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_MEMBERWISE_PRIVATE_IS_ASSIGNABLE_HPP
#define SAKE_CORE_UTILITY_MEMBERWISE_PRIVATE_IS_ASSIGNABLE_HPP

#include <boost/config.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/is_class.hpp>

namespace sake
{

namespace memberwise_private
{

template< class T >
struct is_assignable
    : boost::true_type
{ };

template< class T >
struct is_assignable< T const >
    : boost::is_class<T>
{ };

template< class T >
struct is_assignable< T& >
    : boost::false_type
{ };

#ifndef BOOST_NO_RVALUE_REFERENCES

template< class T >
struct is_assignable< T&& >
    : boost::false_type
{ };

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

} // namespace memberwise_private

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_MEMBERWISE_PRIVATE_IS_ASSIGNABLE_HPP

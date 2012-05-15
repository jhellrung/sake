/*******************************************************************************
 * sake/core/utility/memberwise/private/member_copy_assign_enable.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_MEMBERWISE_PRIVATE_MEMBER_COPY_ASSIGN_ENABLE_HPP
#define SAKE_CORE_UTILITY_MEMBERWISE_PRIVATE_MEMBER_COPY_ASSIGN_ENABLE_HPP

#include <cstddef>

#include <boost/config.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/is_class.hpp>

#include <sake/boost_ext/mpl/and.hpp>

#include <sake/core/introspection/has_operator_assign.hpp>

namespace sake
{

namespace memberwise_private
{

template< class T >
struct member_copy_assign_enable
    : sake::has_operator_assign< T&, T& ( T const & ) >
{ };

template< class T >
struct member_copy_assign_enable< T const >
    : boost_ext::mpl::and2<
          boost::is_class<T>,
          sake::has_operator_assign< T const &, T const & ( T const & ) >
      >
{ };

template< class T, std::size_t N >
struct member_copy_assign_enable< T[N] >
    : member_copy_assign_enable<T>
{ };

template< class T >
struct member_copy_assign_enable< T& >
    : boost::false_type
{ };

#ifndef BOOST_NO_RVALUE_REFERENCES

template< class T >
struct member_copy_assign_enable< T&& >
    : boost::false_type
{ };

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

} // namespace memberwise_private

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_MEMBERWISE_PRIVATE_MEMBER_COPY_ASSIGN_ENABLE_HPP

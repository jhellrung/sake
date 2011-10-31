/*******************************************************************************
 * sake/core/introspection/has_mem_fun_operator.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * class has_mem_fun_operator< T, U >
 ******************************************************************************/

#ifndef SAKE_CORE_INTROSPECTION_HAS_MEM_FUN_OPERATOR_HPP
#define SAKE_CORE_INTROSPECTION_HAS_MEM_FUN_OPERATOR_HPP

#include <sake/core/utility/yes_no_tag.hpp>

namespace sake
{

namespace has_mem_fun_operator_private
{

template< class T, class U >
struct sfinae
{ template< U (T::*)( ) > struct _; };
template< class T, class U >
struct sfinae< T const, U >
{ template< U (T::*)( ) const > struct _; };
template< class T, class U >
struct sfinae< T volatile, U >
{ template< U (T::*)( ) volatile > struct _; };
template< class T, class U >
struct sfinae< T const volatile, U >
{ template< U (T::*)( ) const volatile > struct _; };

} // namespace has_mem_fun_operator_private

template< class T, class U >
class has_mem_fun_operator
{
    template< class T_ > static sake::yes_tag test(
        typename has_mem_fun_operator_private::sfinae<T,U>::template _< &T_::operator U >*);
    template< class T_ > static sake::no_tag test(...);
public:
    static bool const value = sizeof( sake::yes_tag ) == sizeof( test<T>(0) );
    typedef has_mem_fun_operator type;
};

} // namespace sake

#endif // #ifndef SAKE_CORE_INTROSPECTION_HAS_MEM_FUN_OPERATOR_HPP

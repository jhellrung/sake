/*******************************************************************************
 * sake/core/type_traits/fwd.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_TYPE_TRAITS_FWD_HPP
#define SAKE_CORE_TYPE_TRAITS_FWD_HPP

namespace sake
{

template< class T >
struct has_default_constructor;
template< class T >
struct has_copy_constructor;
template< class T >
struct has_move_constructor;

template< class T >
struct has_nothrow_default_constructor;
template< class T >
struct has_nothrow_copy_constructor;
template< class T >
struct has_nothrow_move_constructor;
template< class T >
struct has_nothrow_copy_assign;
template< class T >
struct has_nothrow_move_assign;
template< class T >
struct has_nothrow_destructor;

template< class T >
struct has_trivial_default_constructor;
template< class T >
struct has_trivial_copy_constructor;
template< class T >
struct has_trivial_copy_assign;
template< class T >
struct has_trivial_destructor;

template< class T >
struct has_private_operator_assign;

namespace extension
{

template< class T, class Enable = void >
struct has_default_constructor;
template< class T, class Enable = void >
struct has_copy_constructor;
template< class T, class Enable = void >
struct has_move_constructor;

template< class T, class Enable = void >
struct has_nothrow_default_constructor;
template< class T, class Enable = void >
struct has_nothrow_copy_constructor;
template< class T, class Enable = void >
struct has_nothrow_move_constructor;
template< class T, class Enable = void >
struct has_nothrow_copy_assign;
template< class T, class Enable = void >
struct has_nothrow_move_assign;
template< class T, class Enable = void >
struct has_nothrow_destructor;

template< class T, class Enable = void >
struct has_trivial_default_constructor;
template< class T, class Enable = void >
struct has_trivial_copy_constructor;
template< class T, class Enable = void >
struct has_trivial_copy_assign;
template< class T, class Enable = void >
struct has_trivial_destructor;

template< class T, class Enable = void >
struct has_private_operator_assign;

} // namespace extension

namespace default_impl
{

template< class T >
struct has_default_constructor;
template< class T >
struct has_copy_constructor;
template< class T >
struct has_move_constructor;

template< class T >
struct has_nothrow_default_constructor;
template< class T >
struct has_nothrow_copy_constructor;
template< class T >
struct has_nothrow_move_constructor;
template< class T >
struct has_nothrow_copy_assign;
template< class T >
struct has_nothrow_move_assign;
template< class T >
struct has_nothrow_destructor;

template< class T >
struct has_trivial_default_constructor;
template< class T >
struct has_trivial_copy_constructor;
template< class T >
struct has_trivial_copy_assign;
template< class T >
struct has_trivial_destructor;

template< class T >
struct has_private_operator_assign;

} // namespace default_impl

} // namespace sake

#endif // #ifndef SAKE_CORE_TYPE_TRAITS_FWD_HPP

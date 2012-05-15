/*******************************************************************************
 * sake/core/type_traits/has_nothrow_destructor_fwd.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_TYPE_TRAITS_HAS_NOTHROW_DESTRUCTOR_FWD_HPP
#define SAKE_CORE_TYPE_TRAITS_HAS_NOTHROW_DESTRUCTOR_FWD_HPP

namespace sake
{

template< class T >
struct has_nothrow_destructor;

namespace extension
{
template< class T, class Enable = void >
struct has_nothrow_destructor;
} // namespace extension

namespace default_impl
{
template< class T >
struct has_nothrow_destructor;
} // namespace default_impl

} // namespace sake

#endif // #ifndef SAKE_CORE_TYPE_TRAITS_HAS_NOTHROW_DESTRUCTOR_FWD_HPP

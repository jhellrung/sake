/*******************************************************************************
 * sake/core/iterator/traits_fwd.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_TRAITS_FWD_HPP
#define SAKE_CORE_ITERATOR_TRAITS_FWD_HPP

namespace sake
{

template< class I >
struct iterator_traits;

template< class I >
struct iterator_value
{ typedef typename sake::iterator_traits<I>::value_type type; };
template< class I >
struct iterator_reference
{ typedef typename sake::iterator_traits<I>::reference type; };
template< class I >
struct iterator_pointer
{ typedef typename sake::iterator_traits<I>::pointer type; };
template< class I >
struct iterator_difference
{ typedef typename sake::iterator_traits<I>::difference_type type; };
template< class I >
struct iterator_traversal
{ typedef typename sake::iterator_traits<I>::traversal type; };

template< class I >
struct iterator_introversal
{
    typedef typename sake::iterator_traits<I>::introversal type;
    static unsigned int const value = type::value;
};

template< class I, class Introversal = sake::null_introversal_tag >
struct iterator_relax;

namespace extension
{
template< class I, class Enable = void >
struct iterator_traits;
} // namespace extension

namespace default_impl
{
template< class I, class Introversal = void >
struct iterator_traits;
} // namespace default_impl

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_TRAITS_FWD_HPP

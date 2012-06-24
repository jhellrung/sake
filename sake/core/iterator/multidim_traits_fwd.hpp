/*******************************************************************************
 * sake/core/iterator/multidim_traits_fwd.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_MULTIDIM_TRAITS_FWD_HPP
#define SAKE_CORE_ITERATOR_MULTIDIM_TRAITS_FWD_HPP

namespace sake
{

template< class I >
struct iterator_multidim_traits;

template< class I >
struct iterator_multidim_enable
{
    static bool const value =
        sake::iterator_multidim_traits<I>::enable_tag::value;
    typedef iterator_multidim_enable type;
};

template< class I >
struct iterator_multidim_outer
{ typedef typename sake::iterator_multidim_traits<I>::outer_iterator type; };

template< class I, class Introversal, class Outer = void >
struct iterator_multidim_inner;

namespace extension
{
template< class I, class Enable = void >
struct iterator_multidim_traits;
} // namespace extension

namespace default_impl
{
template< class I >
struct iterator_multidim_traits;
} // namespace default_impl

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_MULTIDIM_TRAITS_FWD_HPP

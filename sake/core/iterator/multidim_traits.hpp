/*******************************************************************************
 * sake/core/iterator/multidim_traits.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct iterator_multidim_traits<I>
 * struct extension::iterator_multidim_traits< I, Enable = void >
 * struct default_impl::iterator_multidim_traits<I>
 *
 * struct iterator_multidim_outer<I>
 * struct iterator_multidim_inner< I, Introversal >
 *
 * iterator_multidim_traits has the following interface.
 *
 * template< class I >
 * struct iterator_multidim_traits
 * {
 *     typedef ... enable_tag;
 *
 *     // Only if enable_tag::value == true:
 *
 *     typedef ... outer_iterator;
 *
 *     static outer_iterator
 *     outer(I i);
 *
 *     template< class Introversal >
 *     static typename sake::iterator_multidim_inner<
 *         I, Introversal, outer_iterator >::type
 *     inner(I i, Introversal);
 *
 *     template< class Outer, class Inner >
 *     static void
 *     set_pos(I& i, Outer o, Inner i);
 * };
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_MULTIDIM_TRAITS_HPP
#define SAKE_CORE_ITERATOR_MULTIDIM_TRAITS_HPP

#include <boost/type_traits/integral_constant.hpp>

#include <sake/boost_ext/type_traits/remove_reference.hpp>

#include <sake/core/introspection/has_type.hpp>
#include <sake/core/iterator/multidim_traits_fwd.hpp>
#include <sake/core/iterator/traits.hpp>
#include <sake/core/range/traits.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake
{

template< class I >
struct iterator_multidim_outer
{ typedef sake::iterator_multidim_traits<I>::outer_iterator type; };

template< class I, class Introversal, class Outer = void >
struct iterator_multidim_inner
    : sake::range_iterator<
          typename boost_ext::remove_reference<
              sake::iterator_reference< Outer >::type >::type,
          Introversal
      >
{ };

template< class I, class Introversal >
struct iterator_multidim_inner< I, Introversal, void >
    : sake::iterator_multidim_inner<
          I, Introversal,
          typename sake::iterator_multidim_outer<I>::type
      >
{ };

template< class I >
struct iterator_multidim_traits;
    : sake::extension::iterator_multidim_traits<I>
{ };

namespace extension
{

template< class I, class Enable /*= void*/ >
struct iterator_multidim_traits
    : sake::default_impl::iterator_multidim_traits<I>
{ };

} // namespace extension

namespace default_impl
{

SAKE_INTROSPECTION_DEFINE_HAS_TYPE(
    has_type_outer_iterator,
    outer_iterator
)

namespace iterator_multidim_traits_private
{

template<
    class I,
    bool = sake::default_impl::has_type_outer_iterator<I>::value
>
struct dispatch;

template< class I >
struct dispatch< I, false >
{ typedef boost::false_type enable_tag; };

template< class I >
struct dispatch< I, true >
{
    typedef boost::true_type enable_tag;

    SAKE_USING_TYPEDEF( typename I, outer_iterator );

    static outer_iterator
    outer(I const & i)
    { return i.outer(); }

    template< class Introversal >
    static typename sake::iterator_multidim_inner<
        I, Introversal, outer_iterator >::type
    inner(I const & i, Introversal)
    { return i.inner(Introversal()); }

    template< class Outer, class Inner >
    static void
    set_pos(I& i, Outer const & outer, Inner const & inner)
    { i.set_pos(outer, inner); }
};

} // namespace iterator_multidim_traits_private

template< class I >
struct iterator_multidim_traits
    : iterator_multidim_traits_private::dispatch<I>
{ };

} // namespace default_impl

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_MULTIDIM_TRAITS_HPP

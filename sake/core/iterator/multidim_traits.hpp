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
 * struct iterator_multidim_enable<I>
 * struct iterator_multidim_outer<I>
 * struct iterator_multidim_inner< I, Introterminal >
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
 *     template< class Introterminal >
 *     static typename sake::iterator_multidim_inner<
 *         I, Introterminal, outer_iterator >::type
 *     inner(I i, Introterminal);
 *
 *     template< class Outer, class Inner >
 *     static void
 *     at_ip(I& i, Outer j, Inner k);
 *
 *     template< class Outer, class Inner, class Introterminal >
 *     static typename sake::iterator_relax< I, Introterminal >::type
 *     at(I i, Outer j, Inner k, Introterminal);
 * };
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_MULTIDIM_TRAITS_HPP
#define SAKE_CORE_ITERATOR_MULTIDIM_TRAITS_HPP

#include <boost/type_traits/integral_constant.hpp>

#include <sake/boost_ext/type_traits/remove_reference.hpp>

#include <sake/core/introspection/has_type.hpp>
#include <sake/core/iterator/categories.hpp>
#include <sake/core/iterator/multidim_traits_fwd.hpp>
#include <sake/core/iterator/traits.hpp>
#include <sake/core/iterator/traits_fwd.hpp>
#include <sake/core/range/traits.hpp>
#include <sake/core/range/traits_fwd.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake
{

template<
    class I,
    class Introterminal /*= sake::null_introterminal_tag*/,
    class Outer /*= void*/
>
struct iterator_multidim_inner
    : sake::range_iterator<
          typename boost_ext::remove_reference<
              typename sake::iterator_reference< Outer >::type >::type
      >
{ };

template< class I, class Introterminal >
struct iterator_multidim_inner< I, Introterminal, void >
    : sake::iterator_multidim_inner<
          I, Introterminal,
          typename sake::iterator_multidim_outer<I>::type
      >
{ };

namespace iterator_multidim_traits_private
{

template<
    class I,
    bool = sake::extension::iterator_multidim_traits<I>::enable_tag::value
>
struct dispatch;

template< class I >
struct dispatch< I, false >
    : sake::extension::iterator_multidim_traits<I>
{ };

template< class I >
struct dispatch< I, true >
    : sake::extension::iterator_multidim_traits<I>
{
private:
    typedef sake::extension::iterator_multidim_traits<I> extension_traits_;
public:
    SAKE_USING_TYPEDEF( typename extension_traits_, outer_iterator );

    static typename sake::iterator_multidim_inner<
        I, sake::null_introterminal_tag, outer_iterator >::type
    inner(I const & i)
    { return inner(i, sake::null_introterminal_tag()); }
    template< class Introterminal >
    static typename sake::iterator_multidim_inner<
        I, Introterminal, outer_iterator >::type
    inner(I const & i, Introterminal)
    { return extension_traits_::inner(i, Introterminal()); }

    template< class Outer, class Inner >
    static typename sake::iterator_relax<I>::type
    at(I const & i, Outer const & j, Inner const & k)
    { return at(i, j, k, sake::null_introterminal_tag()); }
    template< class Outer, class Inner, class Introterminal >
    static typename sake::iterator_relax< I, Introterminal >::type
    at(I const & i, Outer const & j, Inner const & k, Introterminal)
    { return extension_traits_::at(i, j, k, Introterminal()); }
};

} // namespace iterator_multidim_traits_private

template< class I >
struct iterator_multidim_traits
    : iterator_multidim_traits_private::dispatch<I>
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

    template< class Introterminal >
    static typename sake::iterator_multidim_inner<
        I, Introterminal, outer_iterator >::type
    inner(I const & i, Introterminal)
    { return i.inner(Introterminal()); }

    template< class Outer, class Inner >
    static void
    at_ip(I& i, Outer const & j, Inner const & k)
    { i.at_ip(j,k); }

    template< class Outer, class Inner, class Introterminal >
    static typename sake::iterator_relax< I, Introterminal >::type
    at(I const & i, Outer const & j, Inner const & k, Introterminal)
    { return i.at(j, k, Introterminal()); }
};

} // namespace iterator_multidim_traits_private

template< class I >
struct iterator_multidim_traits
    : iterator_multidim_traits_private::dispatch<I>
{ };

} // namespace default_impl

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_MULTIDIM_TRAITS_HPP

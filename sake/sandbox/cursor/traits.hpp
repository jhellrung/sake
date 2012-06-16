/*******************************************************************************
 * sake/core/cursor/traits.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct cursor_traits<C>
 *
 * struct cursor_introversal<C>
 * struct cursor_begin_introversal<C>
 * struct cursor_end_introversal<C>
 ******************************************************************************/

#ifndef SAKE_CORE_CURSOR_TRAITS_HPP
#define SAKE_CORE_CURSOR_TRAITS_HPP

#include <boost/static_assert.hpp>

#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/cursor/categories.hpp>
#include <sake/core/cursor/default_impl/at_begin.hpp>
#include <sake/core/cursor/default_impl/at_end.hpp>
#include <sake/core/cursor/default_impl/begin.hpp>
#include <sake/core/cursor/default_impl/end.hpp>
#include <sake/core/cursor/traits_fwd.hpp>
#include <sake/core/introspection/has_type.hpp>

namespace sake
{

template< class C >
struct cursor_introversal
{
    typedef typename sake::cursor_traits<C>::introversal type;
    static unsigned int const value = type::value;
};

template< class C >
struct cursor_begin_introversal
    : sake::cursor_introversal_meet<
          typename cursor_introversal<C>::type,
          sake::begin_access_introversal_tag
      >
{ };

template< class C >
struct cursor_end_introversal
    : sake::cursor_introversal_meet<
          typename cursor_introversal<C>::type,
          sake::end_access_introversal_tag
      >
{ };

template< class C >
struct cursor_traits
    : sake::extension::cursor_traits<C>
{ };

namespace extension
{

template< class C, class Enable /*= void*/ >
struct cursor_traits
    : sake::default_impl::cursor_traits<C>
{ };

} // namespace extension

namespace default_impl
{

SAKE_INTROSPECTION_DEFINE_HAS_TYPE( has_type_cursor_introversal, cursor_introversal )

namespace cursor_traits_private
{

template< class Tag >
struct begin_base_index
{
    static int const value =
        boost_ext::is_convertible< Tag, sake::begin_access_introversal_tag >::value
      + boost_ext::is_convertible< Tag, sake::begin_detect_introversal_tag >::value;
};

template< class Tag >
struct end_base_index
{
    static int const value =
        boost_ext::is_convertible< Tag, sake::end_access_introversal_tag >::value
      + boost_ext::is_convertible< Tag, sake::end_detect_introversal_tag >::value;
};

template<
    class C,
    bool = sake::default_impl::has_type_cursor_introversal<C>::value
>
struct dispatch_on_has_type;

template<
    class C,
    class Tag = typename C::cursor_introversal,
    int = begin_base_index< Tag >::value
>
struct begin_base;

template<
    class C,
    class Tag = typename C::cursor_introversal,
    int = end_base_index< Tag >::value
>
struct end_base;

template< class C >
struct dispatch_on_has_type< C, false >
{ typedef sake::null_introversal_tag introversal; };

template< class C >
struct dispatch_on_has_type< C, true >
    : begin_base<C>
{ typedef typename C::cursor_introversal introversal; };

template< class C, class Tag >
struct begin_base< C, Tag, 2 >
    : end_base< C, Tag >
{
    static bool at_begin(C const & c)
    { return sake::cursor::default_impl::at_begin(c); }
    static C begin(C const & c)
    { return sake::cursor::default_impl::begin(c); }
};

template< class C, class Tag >
struct begin_base< C, Tag, 1 >
    : end_base< C, Tag >
{
    static bool at_begin(C const & c)
    { return sake::cursor::default_impl::at_begin(c); }
};

template< class C, class Tag >
struct begin_base< C, Tag, 0 >
    : end_base< C, Tag >
{ };


template< class C, class Tag >
struct end_base< C, Tag, 2 >
{
    static bool at_end(C const & c)
    { return sake::cursor::default_impl::at_end(c); }
    static C end(C const & c)
    { return sake::cursor::default_impl::end(c); }
};

template< class C, class Tag >
struct end_base< C, Tag, 1 >
{
    static bool at_end(C const & c)
    { return sake::cursor::default_impl::at_end(c); }
};

template< class C, class Tag >
struct end_base< C, Tag, 0 >
{ };


} // namespace cursor_traits_private

template< class C, class Tag /*= void*/ >
struct cursor_traits
    : cursor_traits_private::begin_base< C, Tag >
{
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        Tag, sake::null_introversal_tag >::value));
    typedef Tag introversal;
};

template< class C >
struct cursor_traits< C, void >
    : cursor_traits_private::dispatch_on_has_type<C>
{ };

} // namespace default_impl

} // namespace sake

#endif // #ifndef SAKE_CORE_CURSOR_INTROVERSAL_FWD_HPP

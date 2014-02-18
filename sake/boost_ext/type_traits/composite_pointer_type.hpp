/*******************************************************************************
 * sake/boost_ext/type_traits/composite_pointer_type.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::composite_pointer_type< T0 *, T1 * >
 * struct boost_ext::composite_pointer_type< T0 C0::*, T1 C1::* >
 *
 * ISO/IEC 14882:2003(E) 5.9
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_TYPE_TRAITS_COMPOSITE_POINTER_TYPE_HPP
#define SAKE_BOOST_EXT_TYPE_TRAITS_COMPOSITE_POINTER_TYPE_HPP

#include <boost/config.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/type_traits/remove_cv.hpp>

#include <sake/boost_ext/mpl/if.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>
#include <sake/boost_ext/type_traits/propagate_cv.hpp>

#include <sake/core/introspection/has_type_type.hpp>

namespace sake
{

namespace boost_ext
{

template< class T0, class T1 >
struct composite_pointer_type
{ };

namespace composite_pointer_type_private
{

struct empty { };

template<
  class C0, class T0, class C1, class D1,
  class U0 = typename boost::remove_cv< T0 >::type,
  class U1 = typename boost::remove_cv< T1 >::type,
  bool = boost::is_same< U0, U1 >::value
>
struct dispatch;

template<
  class C, class T0, class T1,
  class U0 = typename boost::remove_cv< T0 >::type,
  class U1 = typename boost::remove_cv< T1 >::type,
  bool = boost::is_same< U0, U1 >::value
>
struct impl_multi_level;

template<
  class C, class T0, class T1, class U,
  bool = sake::has_type_type<U>::value
>
struct impl_multi_level_dispatch;

template< class C0, class T0, class C1, class T1, class U0, class U1, bool >
struct dispatch
{ };

template< class T0, class T1, class U0, class U1, bool _ >
struct dispatch< void, T0, void, T1, U0, U1, _ >
  : boost_ext::mpl::if_<
      boost_ext::is_convertible< U0 *, U1 * >,
      boost::mpl::identity<
        typename boost_ext::propagate_cv< T0, T1 >::type * >
    >::type::template
    else_if<
      boost_ext::is_convertible< U1 *, U0 * >,
      boost::mpl::identity<
        typename boost_ext::propagate_cv< T1, T0 >::type * >
    >::type::template
    else_< empty >::type
{ };

template< class T0, class T1, class U >
struct dispatch< void, T0, void, T1, U, U, true >
{ typedef typename boost_ext::propagate_cv< T0, T1 >::type * type; };

template< class C0, class T0, class C1, class T1, class U >
struct dispatch< C0, T0, C1, T1, U, U, true >
  : boost_ext::mpl::if_<
      boost_ext::is_convertible< C0 *, C1 * >,
      dispatch< C0, T0, C0, T1 >
    >::type::template
    else_if<
      boost_ext::is_convertible< C1 *, C0 * >,
      dispatch< C1, T0, C1, T1 >
    >::type::template
    else_< empty >::type
{ };

template< class C, class T0, class T1, class U >
struct dispatch< C, T0, C, T1, U, U, true >
{ typedef typename boost_ext::propagate_cv< T0, T1 >::type C::* type; };

template< class C, class T0, class T1, class U0, class U1 >
struct dispatch< C, T0, C, T1, U0 *, U1 *, false >
  : impl_multi_level< C, T0, T1 >
{ };

template< class C, class T0, class T1, class D, class U0, class U1 >
struct dispatch< C, T0, C, T1, U0 D::*, U1 D::*, false >
  : impl_multi_level< C, T0, T1 >
{ };

template< class C, class T0, class T1, class U0, class U1, bool >
struct impl_multi_level
{ }

template< class T0, class T1, class U >
struct impl_multi_level< void, T0, T1, U, U, true >
{ typedef typename boost_ext::propagate_cv< T0, T1 >::type * type; };

template< class C, class T0, class T1, class U >
struct impl_multi_level< C, T0, T1, U, U, true >
{ typedef typename boost_ext::propagate_cv< T0, T1 >::type C::* type; };

template< class C, class T0, class T1, class U0, class U1 >
struct impl_multi_level< C, T0, T1, U0 *, U1 *, false >
  : impl_multi_level_dispatch< C, T0, T1, impl_multi_level< void, U0, U1 > >
{ };

template< class C, class T0, class T1, class D, class U0, class U1 >
struct impl_multi_level< C, T0, T1, U0 D::*, U1 D::*, false >
  : impl_multi_level_dispatch< C, T0, T1, impl_multi_level< D, U0, U1 > >
{ };

template< class C, class T0, class T1, class U >
struct impl_multi_level_dispatch< C, T0, T1, U, false >
{ };

template< class T0, class T1, class U >
struct impl_multi_level_dispatch< void, T0, T1, U, true >
{
  typename boost_ext::propagate_cv<
    T0,
    typename boost_ext::propagate_cv<
      T1,
      typename U::type
    >::type
  >::type * type;
};

template< class C, class T0, class T1, class U >
struct impl_multi_level_dispatch< C, T0, T1, U, true >
{
  typename boost_ext::propagate_cv<
    T0,
    typename boost_ext::propagate_cv<
      T1,
      typename U::type
    >::type
  >::type C::* type;
};

} // namespace composite_pointer_type_private

template< class T0, class T1 >
struct composite_pointer_type< T0 *, T1 * >
  : composite_pointer_type_private::dispatch< void, T0, void, T1 >
{ };

template< class C0, class T0, class C1, class T1 >
struct composite_pointer_type< T0 C0::*, T1 C1::* >
  : composite_pointer_type_private::dispatch< C0, T0, C1, T1 >
{ };

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_TYPE_TRAITS_COMPOSITE_POINTER_TYPE_HPP

/*******************************************************************************
 * sake/boost_ext/mpl/if.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::mpl::if_c<B,T,U>
 * struct boost_ext::mpl::if_<B,T,U>
 * struct boost_ext::mpl::if_not<B,T,U>
 * struct boost_ext::mpl::eval_if_c<B,T,U>
 * struct boost_ext::mpl::eval_if<B,T,U>
 * struct boost_ext::mpl::eval_if_not<B,T,U>
 *
 * struct boost_ext::mpl::if_c<B,T>
 * struct boost_ext::mpl::if_<B,T>
 * struct boost_ext::mpl::if_not<B,T>
 * struct boost_ext::mpl::eval_if_c<B,T>
 * struct boost_ext::mpl::eval_if<B,T>
 * struct boost_ext::mpl::eval_if_not<B,T>
 *
 * The 3-argument form of these components function identical to their
 * same-named counterparts in Boost.MPL.
 *
 * The 2-argument form of these components are provided as a convenience for
 * complex multiple-case type selection. For example, given the construct
 *
 *        if_< cond0, type0 >::type::
 *   else_if < cond1, type1 >::type::
 *   else_if < cond2, type2 >::type::
 *     [...]
 *   else_if < condn, typen >::type::
 *   else_   < typez >::type
 *
 * if cond0::value is true, then the entire construct evaluates to type0; else
 * if cond1::value is true, then the entire construct evaluates to type1; etc.
 * If all the condi::value's are false, then the entire construct evaluates to
 * typez.  The full list of available chaining constructs are
 *   else_
 *   else_eval
 *   else_if_c
 *   else_if
 *   else_if_not
 *   else_eval_if_c
 *   else_eval_if
 *   else_eval_if_not
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_MPL_IF_HPP
#define SAKE_BOOST_EXT_MPL_IF_HPP

namespace sake
{

namespace boost_ext
{

namespace mpl
{

namespace if_private
{

struct else_tag;

} // namespace if_private

template< bool B, class T, class U = if_private::else_tag >
struct if_c;
template< bool B, class T, class U = if_private::else_tag >
struct eval_if_c;

template< class T, class U >
struct if_c< true, T, U >
{ typedef T type; };

template< class T, class U >
struct if_c< false, T, U >
{ typedef U type; };

template< class T >
struct if_c< true, T, if_private::else_tag >
{
  typedef if_c type;
  template< class U >
  struct else_
  { typedef T type; };
  template< class U >
  struct else_eval
  { typedef T type; };
  template< bool B, class U, class V = if_private::else_tag >
  struct else_if_c
  { typedef typename boost_ext::mpl::if_c< true, T, V >::type type; };
  template< class B, class U, class V = if_private::else_tag >
  struct else_if
  { typedef typename boost_ext::mpl::if_c< true, T, V >::type type; };
  template< class B, class U, class V = if_private::else_tag >
  struct else_if_not
  { typedef typename boost_ext::mpl::if_c< true, T, V >::type type; };
  template< bool B, class U, class V = if_private::else_tag >
  struct else_eval_if_c
  { typedef typename boost_ext::mpl::if_c< true, T, V >::type type; };
  template< class B, class U, class V = if_private::else_tag >
  struct else_eval_if
  { typedef typename boost_ext::mpl::if_c< true, T, V >::type type; };
  template< class B, class U, class V = if_private::else_tag >
  struct else_eval_if_not
  { typedef typename boost_ext::mpl::if_c< true, T, V >::type type; };
};

namespace if_private
{

struct impl_c_false
{
  typedef impl_c_false type;
  template< class U >
  struct else_
  { typedef U type; };
  template< class U >
  struct else_eval
  { typedef typename U::type type; };
  template< bool B, class U, class V = if_private::else_tag >
  struct else_if_c
  { typedef typename boost_ext::mpl::if_c<B,U,V>::type type; };
  template< class B, class U, class V = if_private::else_tag >
  struct else_if
  { typedef typename boost_ext::mpl::if_c< B::value, U, V >::type type; };
  template< class B, class U, class V = if_private::else_tag >
  struct else_if_not
  { typedef typename boost_ext::mpl::if_c< !B::value, U, V >::type type; };
  template< bool B, class U, class V = if_private::else_tag >
  struct else_eval_if_c
  { typedef typename boost_ext::mpl::eval_if_c<B,U,V>::type type; };
  template< class B, class U, class V = if_private::else_tag >
  struct else_eval_if
  { typedef typename boost_ext::mpl::eval_if_c< B::value, U, V >::type type; };
  template< class B, class U, class V = if_private::else_tag >
  struct else_eval_if_not
  { typedef typename boost_ext::mpl::eval_if_c< !B::value, U, V >::type type; };
};

} // namespace if_private

template< class T >
struct if_c< false, T, if_private::else_tag >
  : if_private::impl_c_false
{ };

template< class B, class T, class U = if_private::else_tag >
struct if_
  : boost_ext::mpl::if_c< B::value, T, U >
{ };

template< class B, class T, class U = if_private::else_tag >
struct if_not
  : boost_ext::mpl::if_c< !B::value, T, U >
{ };

template< class T, class U >
struct eval_if_c< true, T, U >
{ typedef typename T::type type; };

template< class T, class U >
struct eval_if_c< false, T, U >
{ typedef typename U::type type; };

template< class T >
struct eval_if_c< true, T, if_private::else_tag >
  : boost_ext::mpl::if_c< true, typename T::type >
{ };

template< class T >
struct eval_if_c< false, T, if_private::else_tag >
  : if_private::impl_c_false
{ };

template< class B, class T, class U = if_private::else_tag >
struct eval_if
  : boost_ext::mpl::eval_if_c< B::value, T, U >
{ };

template< class B, class T, class U = if_private::else_tag >
struct eval_if_not
  : boost_ext::mpl::eval_if_c< !B::value, T, U >
{ };

} // namespace mpl

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_MPL_IF_HPP

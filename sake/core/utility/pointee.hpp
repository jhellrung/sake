/*******************************************************************************
 * sake/core/utility/pointee.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct pointee<P>
 * struct extension::pointee< P, Enable = void >
 * struct default_impl::pointee<P>
 *
 * This is a metafunction giving the type the given (possibly smart) pointer
 * or iterator refers to.  This includes a const qualifier if the pointer or
 * iterator has only const access to the pointee object.
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_POINTEE_HPP
#define SAKE_CORE_UTILITY_POINTEE_HPP

#include <sake/boost_ext/mpl/if.hpp>
#include <sake/boost_ext/type_traits/add_const_if.hpp>

#include <sake/core/introspection/has_type_element_type.hpp>
#include <sake/core/introspection/has_type_type.hpp>
#include <sake/core/introspection/has_type_value_type.hpp>
#include <sake/core/introspection/is_incrementable.hpp>
#include <sake/core/iterator/is_const_iterator.hpp>
#include <sake/core/iterator/traits.hpp>
#include <sake/core/utility/int_tag.hpp>

namespace sake
{

/*******************************************************************************
 * struct pointee<P>
 ******************************************************************************/

template< class P >
struct pointee
  : extension::pointee<P>
{ };

template< class T >
struct pointee< T * >
{ typedef T type; };

/*******************************************************************************
 * struct extension::pointee< P, Enable = void >
 ******************************************************************************/

namespace extension
{

template< class P, class Enable /*= void*/ >
struct pointee
  : default_impl::pointee<P>
{ };

} // namespace extension

/*******************************************************************************
 * struct default_impl::pointee<P>
 ******************************************************************************/

namespace default_impl
{

namespace pointee_private
{

template< class P >
struct dispatch_index
{
  static int const value = boost_ext::mpl::
       if_< sake::is_incrementable<P>,      // iterators
            sake::int_tag<4> >::type::template
  else_if < sake::has_type_element_type<P>, // Boost-style smart pointers
            sake::int_tag<3> >::type::template
  else_if < sake::has_type_value_type<P>,   // best guess for everything else
            sake::int_tag<2> >::type::template
  else_if < sake::has_type_type<P>,         // last ditch effort
            sake::int_tag<1> >::type::template
  else_   < sake::int_tag<0> >::type::value;
};

template<
  class P,
  int = dispatch_index<P>::value
>
struct dispatch;

template< class I >
struct dispatch<I,4>
  : boost_ext::add_const_if_c<
      sake::is_const_iterator<I>::value,
      typename sake::iterator_value<I>::type
    >
{ };

template< class P >
struct dispatch<P,3>
{ typedef typename P::element_type type; };

template< class P >
struct dispatch<P,2>
{ typedef typename P::value_type type; };

template< class P >
struct dispatch<P,1>
{ typedef typename P::type type; };

template< class P >
struct dispatch<P,0>
{ };

} // namespace pointee_private

template< class P >
struct pointee
  : pointee_private::dispatch<P>
{ };

} // namespace default_impl

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_POINTEE_HPP

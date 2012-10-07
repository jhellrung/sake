/*******************************************************************************
 * sake/core/range/basic/construct/subrange.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * range::basic::construct::subrange(I i,J j)
 *   -> range::basic::construct::result_of::subrange<I,J>::type
 * range::basic::construct::subrange(begin_tag, I i)
 *   -> range::basic::construct::result_of::subrange< begin_tag, I >::type
 * range::basic::construct::subrange(I i, end_tag)
 *   -> range::basic::construct::result_of::subrange< I, end_tag >::type
 * range::basic::construct::subrange(I i, N)
 *   -> range::basic::construct::result_of::subrange<I,N>::type
 * range::basic::construct::subrange_c<N>(I i)
 *   -> range::basic::construct::result_of::subrange_c<I,N>::type
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_CONSTRUCT_SUBRANGE_HPP
#define SAKE_CORE_RANGE_CONSTRUCT_SUBRANGE_HPP

#include <cstddef>

#include <boost/static_assert.hpp>
#include <boost/type_traits/integral_constant.hpp>

#include <sake/boost_ext/type_traits/common_type.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/config.hpp>
#include <sake/core/introspection/has_isc_value.hpp>
#include <sake/core/iterator/adapt_introterminal.hpp>
#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/iterator/categories.hpp>
#include <sake/core/range/basic/subrange.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>

namespace sake
{

namespace range
{

namespace basic
{

namespace construct
{

namespace result_of
{

template< class T, class U >
struct subrange;

template< class I, std::size_t N >
struct subrange_c;

} // namespace result_of

namespace functional
{

struct subrange
{
  SAKE_RESULT_FROM_METAFUNCTION(
    sake::range::basic::construct::result_of::subrange, 2 )

  template< class T, class U >
  typename result< subrange ( T, U ) >::type
  operator()(T const & x, U const & y) const
  { return typename result< subrange ( T, U ) >::type(x,y); }
};

} // namespace functional

#ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
namespace subrange_adl_barrier
{ sake::range::basic::construct::functional::subrange const subrange = { }; }
using namespace subrange_adl_barrier;
#else // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
sake::range::basic::construct::functional::subrange const subrange = { };
#endif // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS

template< std::size_t N, class I >
inline typename sake::range::basic::construct::
  result_of::subrange_c<I,N>::type
subrange_c(I const & i)
{
  return typename sake::range::basic::construct::
    result_of::subrange_c<I,N>::type(i);
}

namespace result_of
{

namespace subrange_private
{

template< class I, class U, bool = sake::has_isc_value<U>::value >
struct dispatch1;

template< class T, class U >
struct dispatch0
  : subrange_private::dispatch1<T,U>
{ };

template< class I >
struct dispatch0< sake::begin_tag, I >
{
  BOOST_STATIC_ASSERT((boost_ext::is_convertible<
    typename sake::iterator_introterminal<I>::type,
    sake::begin_access_introterminal_tag
  >::value));
  typedef sake::range::basic::subrange<
    typename sake::iterator::adapt_introterminal<
      I,
      sake::begin_access_introterminal_tag,
      sake::begin_access_introterminal_tag
    >::type
  > type;
};

template< class I >
struct dispatch0< I, sake::end_tag >
{
  BOOST_STATIC_ASSERT((boost_ext::is_convertible<
    typename sake::iterator_introterminal<I>::type,
    sake::end_access_introterminal_tag
  >::value));
  typedef sake::range::basic::subrange<
    typename sake::iterator::adapt_introterminal<
      I,
      sake::end_access_introterminal_tag,
      sake::end_access_introterminal_tag
    >::type
  > type;
};

template< class I, class J >
struct dispatch1< I, J, false >
{
  typedef sake::range::basic::subrange<
    typename sake::iterator::adapt_introterminal<
      typename boost_ext::common_type<I,J>::type,
      sake::null_introterminal_tag
    >::type
  > type;
};

template< class I, class N >
struct dispatch0< I, N, true >
{
  typedef sake::range::basic::subrange<
    typename sake::iterator::adapt_introterminal<
      I, sake::null_introterminal_tag >::type,
    boost::integral_constant< std::size_t, N::value >
  > type;
};

} // namespace subrange_private

template< class T, class U >
struct subrange
  : subrange_private::dispatch0<
      typename boost_ext::remove_qualifiers<T>::type,
      typename boost_ext::remove_qualifiers<U>::type
    >
{ };

template< class I, std::size_t N >
struct subrange_c
  : sake::range::basic::construct::result_of::subrange<
      I, boost::integral_constant< std::size_t, N > >
{ };

} // namespace result_of

} // namespace basic

} // namespace construct

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_CONSTRUCT_SUBRANGE_HPP

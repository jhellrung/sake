/*******************************************************************************
 * sake/core/iterator/distance.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * iterator::distance(I0 i0, I1 i1)
 *   -> iterator::result_of::distance< I0, I1 >::type
 * struct iterator::functional::distance
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_DISTANCE_HPP
#define SAKE_CORE_ITERATOR_DISTANCE_HPP

#include <boost/mpl/not.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/static_assert.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/mpl/if.hpp>
#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/type_traits/common_result_type.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/config.hpp>
#include <sake/core/introspection/has_operator_minus.hpp>
#include <sake/core/iterator/begin.hpp>
#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/iterator/categories.hpp>
#include <sake/core/iterator/distance_fwd.hpp>
#include <sake/core/iterator/end.hpp>
#include <sake/core/iterator/facade_fwd.hpp>
#include <sake/core/iterator/multidim_traits.hpp>
#include <sake/core/iterator/traits.hpp>
#include <sake/core/iterator/traits_fwd.hpp>
#include <sake/core/range/distance.hpp>
#include <sake/core/range/distance_fwd.hpp>
#include <sake/core/range/traits.hpp>
#include <sake/core/range/traits_fwd.hpp>
#include <sake/core/utility/int_tag.hpp>
#include <sake/core/utility/is_template_base_of.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>

namespace sake
{

namespace iterator
{

namespace result_of
{

template< class I0, class I1 = I0 >
struct distance;

} // namespace result_of

namespace distance_private
{

template< class I0, class I1 >
struct dispatch_index;

template<
  class I0, class I1,
  int = dispatch_index< I0, I1 >::value
>
struct dispatch;

} // namespace distance_private

namespace functional
{

struct distance
{
  SAKE_RESULT_FROM_METAFUNCTION( sake::iterator::result_of::distance, 2 )

  template< class I0, class I1 >
  typename result< distance ( I0, I1 ) >::type
  operator()(I0 const & i0, I1 const & i1) const
  { return distance_private::dispatch< I0, I1 >::apply(i0, i1); }
};

} // namespace functional

#ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
namespace distance_adl_barrier
{ sake::iterator::functional::distance const distance = { }; }
using namespace distance_adl_barrier;
#else // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
sake::iterator::functional::distance const distance = { };
#endif // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS

namespace result_of
{

template< class I0, class I1 /*= I0*/ >
struct distance
  : sake::iterator::distance_private::dispatch<
      typename boost_ext::remove_qualifiers< I0 >::type,
      typename boost_ext::remove_qualifiers< I1 >::type
    >
{ };

} // namespace result_of

namespace distance_private
{

template< class I, class Pred >
struct has_operator_minus_helper
  : boost_ext::mpl::or2<
      boost_ext::is_convertible<
        typename sake::iterator_traversal<I>::type,
        boost::random_access_traversal_tag
      >,
      sake::is_template_base_of2< sake::iterator::facade, I, Pred >
    >
{ };

template< class I0, class I1 >
struct dispatch_index
{
  typedef typename boost_ext::common_result_type< I0, I1 >::type common_type_;
  typedef typename sake::iterator_difference< common_type_ >::type type;
  static int const value = boost_ext::mpl::
  if_<
    has_operator_minus_helper<
      common_type_,
      sake::has_operator_minus< boost::mpl::_1, boost::mpl::_1, type >
    >,
    sake::int_tag<2>
  >::type::template
  else_if<
    sake::iterator_multidim_enable< common_type_ >,
    sake::int_tag<1>
  >::type::template
  else_<
    sake::int_tag<0>
  >::type::value;
};

template< class I >
struct dispatch_index< sake::begin_tag, I >
{
  BOOST_STATIC_ASSERT((boost_ext::is_convertible<
    typename sake::iterator_introterminal<I>::type,
    sake::begin_detect_introterminal_tag
  >::value));
  typedef typename sake::iterator_difference<I>::type type;
  static int const value = boost_ext::mpl::
  if_<
    has_operator_minus_helper<
      I, sake::has_operator_minus< boost::mpl::_1, sake::begin_tag, type > >,
    sake::int_tag<3>
  >::type::template
  else_if<
    has_operator_minus_helper<
      I, sake::has_operator_minus< sake::begin_tag, boost::mpl::_1, type > >,
    sake::int_tag<2>
  >::type::template
  else_if<
    boost_ext::mpl::and2<
      boost_ext::is_convertible<
        typename sake::iterator_introterminal<I>::type,
        sake::begin_access_introterminal_tag
      >,
      boost_ext::mpl::or3<
        boost::mpl::not_< boost_ext::is_convertible<
          typename sake::iterator_traversal<I>::type,
          boost::bidirectional_traversal_tag
        > >,
        has_operator_minus_helper<
          I, sake::has_operator_minus< boost::mpl::_1, boost::mpl::_1, type > >,
        sake::iterator_multidim_enable<I>
      >
    >,
    sake::int_tag<1>
  >::type::template
  else_<
    sake::int_tag<0>
  >::type::value;
};

template< class I >
struct dispatch_index< I, sake::begin_tag >
{
  BOOST_STATIC_ASSERT((boost_ext::is_convertible<
    typename sake::iterator_introterminal<I>::type,
    sake::begin_detect_introterminal_tag
  >::value));
  typedef typename sake::iterator_difference<I>::type type;
  static int const value = boost_ext::mpl::
  if_<
    has_operator_minus_helper<
      I, sake::has_operator_minus< sake::begin_tag, boost::mpl::_1, type > >,
    sake::int_tag<1>
  >::type::template
  else_<
    sake::int_tag<0>
  >::type::value;
};

template< class I >
struct dispatch_index< I, sake::end_tag >
{
  BOOST_STATIC_ASSERT((boost_ext::is_convertible<
    typename sake::iterator_introterminal<I>::type,
    sake::end_detect_introterminal_tag
  >::value));
  typedef typename sake::iterator_difference<I>::type type;
  static int const value = boost_ext::mpl::
  if_<
    has_operator_minus_helper<
      I, sake::has_operator_minus< sake::end_tag, boost::mpl::_1, type > >,
    sake::int_tag<3>
  >::type::template
  else_if<
    has_operator_minus_helper<
      I, sake::has_operator_minus< boost::mpl::_1, sake::end_tag, type > >,
    sake::int_tag<2>
  >::type::template
  else_if<
    boost_ext::mpl::and2<
      boost_ext::is_convertible<
        typename sake::iterator_introterminal<I>::type,
        sake::end_access_introterminal_tag
      >,
      boost_ext::mpl::or2<
        has_operator_minus_helper<
          I, sake::has_operator_minus< boost::mpl::_1, boost::mpl::_1, type > >,
        sake::iterator_multidim_enable<I>
      >
    >,
    sake::int_tag<1>
  >::type::template
  else_<
    sake::int_tag<0>
  >::type::value;
};

template< class I >
struct dispatch_index< sake::end_tag, I >
{
  BOOST_STATIC_ASSERT((boost_ext::is_convertible<
    typename sake::iterator_introterminal<I>::type,
    sake::end_detect_introterminal_tag
  >::value));
  typedef typename sake::iterator_difference<I>::type type;
  static int const value = boost_ext::mpl::
  if_<
    has_operator_minus_helper<
      I, sake::has_operator_minus< boost::mpl::_1, sake::end_tag, type > >,
    sake::int_tag<1>
  >::type::template
  else_<
    sake::int_tag<0>
  >::type::value;
};

template< class I0, class I1 >
struct dispatch< I0, I1, 2 >
{
  typedef typename boost_ext::common_result_type< I0, I1 >::type common_type_;
  typedef typename sake::iterator_difference< common_type_ >::type type;
  static type apply(I0 const & i0, I1 const & i1)
  { return i1 - i0; }
};

template< class I0, class I1 >
struct dispatch< I0, I1, 1 >
{
  typedef typename boost_ext::common_result_type< I0, I1 >::type common_type_;
  typedef typename sake::iterator_difference< common_type_ >::type type;
  static type apply(I0 const & i0, I1 const & i1)
  {
    typedef sake::iterator_multidim_traits< I0 > traits0;
    typedef sake::iterator_multidim_traits< I1 > traits1;
    typename traits0::outer_iterator j0 = traits0::outer(i0);
    typename traits1::outer_iterator j1 = traits1::outer(i1);
    if(j0 == j1)
      return sake::iterator::distance(
        traits0::inner(i0), traits1::inner(i1));
    type result = sake::iterator::distance(
      traits0::inner(i0, sake::end_access_introterminal_tag()), sake::_end);
    while(++j0 != j1)
      result += sake::range::functional::distance()(*j0);
    result += sake::iterator::distance(
      sake::range_traits< typename sake::iterator_value<
        typename traits0::outer_iterator >::type const >::begin(*j0),
      traits1::inner(i1)
    );
    return result;
  }
};

template< class I0, class I1 >
struct dispatch< I0, I1, 0 >
{
  typedef typename boost_ext::common_result_type< I0, I1 >::type common_type_;
  typedef typename sake::iterator_difference< common_type_ >::type type;
  static type apply(I0 i0, I1 const i1)
  {
    type n = 0;
    for(; i0 != i1; ++i0, ++n);
    return n;
  }
};

template< class I >
struct dispatch< sake::begin_tag, I, 3 >
{
  typedef typename sake::iterator_difference<I>::type type;
  static type apply(sake::begin_tag, I const & i)
  { return i - sake::_begin; }
  static type apply(I const & i, sake::begin_tag)
  { return -(i - sake::_begin); }
};

template< class I >
struct dispatch< sake::begin_tag, I, 2 >
  : dispatch< I, sake::begin_tag >
{ };

template< class I >
struct dispatch< sake::begin_tag, I, 1 >
{
  typedef typename sake::iterator_difference<I>::type type;
  static type apply(sake::begin_tag, I const & i)
  { return sake::iterator::distance(sake::iterator::begin(i), i); }
  static type apply(I const & i, sake::begin_tag)
  { return sake::iterator::distance(i, sake::iterator::begin(i)); }
};

template< class I >
struct dispatch< sake::begin_tag, I, 0 >
{
  typedef typename sake::iterator_difference<I>::type type;
  BOOST_STATIC_ASSERT((boost_ext::is_convertible<
    typename sake::iterator_traversal<I>::type,
    boost::bidirectional_traversal_tag
  >::value));
  static type apply(sake::begin_tag, I i)
  {
    type n = 0;
    for(; i != sake::_begin; --i, ++n);
    return n;
  }
  static type apply(I i, sake::begin_tag)
  {
    type n = 0;
    for(; i != sake::_begin; --i, --n);
    return n;
  }
};

template< class I >
struct dispatch< I, sake::begin_tag, 1 >
{
  typedef typename sake::iterator_difference<I>::type type;
  static type apply(I const & i, sake::begin_tag)
  { return sake::_begin - i; }
  static type apply(sake::begin_tag, I const & i)
  { return -(sake::_begin - i); }
};

template< class I >
struct dispatch< I, sake::begin_tag, 0 >
  : dispatch< sake::begin_tag, I >
{ };

template< class I >
struct dispatch< I, sake::end_tag, 3 >
{
  typedef typename sake::iterator_difference<I>::type type;
  static type apply(I const & i, sake::end_tag)
  { return sake::_end - i; }
  static type apply(sake::end_tag, I const & i)
  { return -(sake::_end - i); }
};

template< class I >
struct dispatch< I, sake::end_tag, 2 >
  : dispatch< sake::end_tag, I >
{ };

template< class I >
struct dispatch< I, sake::end_tag, 1 >
{
  typedef typename sake::iterator_difference<I>::type type;
  static type apply(I const & i, sake::end_tag)
  { return sake::iterator::distance(i, sake::iterator::end(i)); }
  static type apply(sake::end_tag, I const & i)
  { return sake::iterator::distance(sake::iterator::end(i), i); }
};

template< class I >
struct dispatch< I, sake::end_tag, 0 >
{
  typedef typename sake::iterator_difference<I>::type type;
  static type apply(I i, sake::end_tag)
  {
    type n = 0;
    for(; i != sake::_end; ++i, ++n);
    return n;
  }
  static type apply(sake::end_tag, I i)
  {
    type n = 0;
    for(; i != sake::_end; ++i, --n);
    return n;
  }
};

template< class I >
struct dispatch< sake::end_tag, I, 1 >
{
  typedef typename sake::iterator_difference<I>::type type;
  static type apply(sake::end_tag, I const & i)
  { return i - sake::_end; }
  static type apply(I const & i, sake::end_tag)
  { return -(i - sake::_end); }
};

template< class I >
struct dispatch< sake::end_tag, I, 0 >
  : dispatch< I, sake::end_tag >
{ };

} // namespace distance_private

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_DISTANCE_HPP

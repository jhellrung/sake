/*******************************************************************************
 * sake/core/range/basic/sub.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * range::sub(I i,J j) -> range::result_of::sub<I,J>::type
 * range::sub(begin_tag, I i) -> range::result_of::sub< begin_tag, I >::type
 * range::sub(I i, end_tag) -> range::result_of::sub< I, end_tag >::type
 * range::sub(I i, N) -> range::result_of::sub<I,N>::type
 * range::sub_c<N>(I i) -> range::result_of::sub_c<I,N>::type
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_BASIC_SUB_HPP
#define SAKE_CORE_RANGE_BASIC_SUB_HPP

#include <cstddef>

#include <boost/static_assert.hpp>
#include <boost/type_traits/integral_constant.hpp>

#include <sake/boost_ext/type_traits/common_type.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/introspection/has_isc_value.hpp>
#include <sake/core/iterator/adapt_introversal.hpp>
#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/iterator/categories.hpp>
#include <sake/core/range/basic/subrange.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>

namespace sake
{

namespace range
{

namespace result_of
{

template< class I, class J >
struct sub;

template< class I, std::size_t N >
struct sub_c;

} // namespace result_of

namespace functional
{

struct sub
{
    SAKE_RESULT_FROM_METAFUNCTION( sake::range::result_of::sub, 2 )

    template< class I, class J >
    typename result< sub ( I, J ) >::type
    operator()(I const & i, J const & j) const
    { return typename result< sub ( I, J ) >::type(i,j); }
};

} // namespace functional

static sake::range::functional::sub const sub = { };

template< std::size_t N, class I >
inline typename sake::range::result_of::sub_c<I,N>::type
sub_c(I const & i)
{ return typename sake::range::result_of::sub_c<I,N>::type(i); }

namespace result_of
{

namespace sub_private
{

template< class I, class J, bool = sake::has_isc_value<J>::value >
struct dispatch1;

template< class I, class J >
struct dispatch0
    : sub_private::dispatch1<I,J>
{ };

template< class I >
struct dispatch0< sake::begin_tag, I >
{
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        typename sake::iterator_introversal<I>::type,
        sake::begin_access_introversal_tag
    >::value));
    typedef sake::range::basic::subrange<
        typename sake::iterator::adapt_introversal<
            I,
            sake::begin_access_introversal_tag,
            sake::begin_access_introversal_tag
        >::type
    > type;
};

template< class I >
struct dispatch0< I, sake::end_tag >
{
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        typename sake::iterator_introversal<I>::type,
        sake::end_access_introversal_tag
    >::value));
    typedef sake::range::basic::subrange<
        typename sake::iterator::adapt_introversal<
            I,
            sake::end_access_introversal_tag,
            sake::end_access_introversal_tag
        >::type
    > type;
};

template< class I, class J >
struct dispatch1< I, J, false >
{
    typedef sake::range::basic::subrange<
        typename sake::iterator::adapt_introversal<
            typename boost_ext::common_type<I,J>::type,
            sake::null_introversal_tag
        >::type
    > type;
};

template< class I, class N >
struct dispatch0< I, N, true >
{
    typedef sake::range::basic::subrange<
        typename sake::iterator::adapt_introversal<
            I, sake::null_introversal_tag >::type,
        boost::integral_constant< std::size_t, N::value >
    > type;
};

} // namespace sub_private

template< class I, class J >
struct sub
    : sub_private::dispatch0<
          typename boost_ext::remove_qualifiers<I>::type,
          typename boost_ext::remove_qualifiers<J>::type
      >
{ };

template< class I, std::size_t N >
struct sub_c
    : sake::range::result_of::sub<
          I, boost::integral_constant< std::size_t, N > >
{ };

} // namespace result_of

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_BASIC_SUB_HPP

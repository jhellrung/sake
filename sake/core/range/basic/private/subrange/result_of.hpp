/*******************************************************************************
 * sake/core/range/basic/private/subrange/result_of.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_BASIC_PRIVATE_SUBRANGE_RESULT_OF_HPP
#define SAKE_CORE_RANGE_BASIC_PRIVATE_SUBRANGE_RESULT_OF_HPP

#include <cstddef>

#include <boost/concept/assert.hpp>
#include <boost/type_traits/integral_constant.hpp>

#include <sake/boost_ext/type_traits/common_type.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/introspection/has_isc_value.hpp>
#include <sake/core/iterator/adapt_introversal.hpp>
#include <sake/core/iterator/adapt_introversal_fwd.hpp>
#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/iterator/categories.hpp>
#include <sake/core/iterator/concepts/Iterator.hpp>
#include <sake/core/range/basic/fwd.hpp>

namespace sake
{

namespace range
{

namespace result_of
{

template< class Begin, class End >
struct sub;
template< class I, std::size_t N >
struct sub_c;

namespace sub_private
{

template< class Begin, class End, bool = sake::has_isc_value< End >::value >
struct dispatch;

template< class Begin, class End >
struct dispatch< Begin, End, false >
{
    BOOST_CONCEPT_ASSERT((sake::concepts::Iterator< Begin >));
    BOOST_CONCEPT_ASSERT((sake::concepts::Iterator< End >));
    typedef sake::range::basic::subrange<
        typename sake::iterator::adapt_introversal<
            typename boost_ext::common_type< Begin, End >::type,
            sake::null_introversal_tag
        >::type
    > type;
};

template< class I >
struct dispatch< sake::begin_tag, I, false >
{
    BOOST_CONCEPT_ASSERT((sake::concepts::Iterator<I>));
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
struct dispatch< I, sake::end_tag, false >
{
    BOOST_CONCEPT_ASSERT((sake::concepts::Iterator<I>));
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

template< class I, class N >
struct dispatch< I, N, true >
    : sake::range::result_of::sub_c< I, N::value >
{ };

} // namespace sub_private

template< class Begin, class End >
struct sub
    : sub_private::dispatch<
          typename boost_ext::remove_qualifiers< Begin >::type,
          typename boost_ext::remove_qualifiers< End >::type
      >
{ };

template< class I, std::size_t N >
struct sub_c
{
    typedef sake::range::basic::subrange<
        typename sake::iterator::adapt_introversal<
            I, sake::null_introversal_tag >::type,
        boost::integral_constant< std::size_t, N >
    > type;
};

} // namespace result_of

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_BASIC_PRIVATE_SUBRANGE_RESULT_OF_HPP

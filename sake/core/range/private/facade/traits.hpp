/*******************************************************************************
 * sake/core/range/private/facade/traits.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_PRIVATE_FACADE_TRAITS_HPP
#define SAKE_CORE_RANGE_PRIVATE_FACADE_TRAITS_HPP

#include <boost/concept/assert.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/make_unsigned.hpp>

#include <sake/boost_ext/mpl/at.hpp>
#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>
#include <sake/boost_ext/type_traits/is_convertible_wndp2bp.hpp>
#include <sake/boost_ext/type_traits/remove_reference.hpp>

#include <sake/core/iterator/categories.hpp>
#include <sake/core/iterator/traits.hpp>
#include <sake/core/range/concepts/Range.hpp>
#include <sake/core/range/keyword.hpp>
#include <sake/core/utility/is_convertible_wnrbt.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake
{

namespace range
{

namespace facade_adl
{

namespace private_
{

template< class Params >
struct traits
{
    BOOST_STATIC_ASSERT((boost::mpl::is_sequence< Params >::value));

    typedef typename boost_ext::mpl::at<
        Params, sake::range::keyword::tag::iterator >::type iterator;
    typedef typename boost_ext::mpl::at<
        Params, sake::range::keyword::tag::const_iterator,
        iterator
    >::type const_iterator;
    BOOST_STATIC_ASSERT((
        boost_ext::is_convertible_wndp2bp< iterator, const_iterator >::value));

    typedef sake::iterator_traits< iterator > iterator_traits_;
    typedef sake::iterator_traits< const_iterator > const_iterator_traits_;

    SAKE_USING_TYPEDEF( typename iterator_traits_, value_type );
    BOOST_STATIC_ASSERT((boost::is_same<
        value_type, typename const_iterator_traits_::value_type >::value));

    SAKE_USING_TYPEDEF( typename iterator_traits_, reference );
    typedef typename const_iterator_traits_::reference const_reference;
    BOOST_STATIC_ASSERT((
        sake::is_convertible_wnrbt< reference, const_reference >::value));

    SAKE_USING_TYPEDEF( typename iterator_traits_, difference_type );
    BOOST_STATIC_ASSERT((boost::is_integral< difference_type >::value));
    BOOST_STATIC_ASSERT((boost::is_same<
        difference_type,
        typename const_iterator_traits_::difference_type
    >::value));

    typedef typename boost::make_unsigned< difference_type >::type size_type;

    typedef typename iterator_traits_::traversal range_traversal;
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        range_traversal, boost::single_pass_traversal_tag >::value));
    BOOST_STATIC_ASSERT((boost::is_same<
        range_traversal, typename const_iterator_traits_::traversal >::value));

    typedef typename boost_ext::mpl::or2<
        boost_ext::is_convertible<
            range_traversal,
            boost::bidirectional_traversal_tag
        >,
        boost_ext::mpl::at<
            Params, sake::range::keyword::tag::back_enable,
            boost::false_type
        >
    >::type back_enable;

    typedef typename boost_ext::mpl::or2<
        boost_ext::is_convertible<
            range_traversal,
            boost::forward_traversal_tag
        >,
        boost_ext::mpl::at<
            Params, sake::range::keyword::tag::distance_enable,
            boost::false_type
        >
    >::type distance_enable;

    typedef typename boost_ext::mpl::or2<
        boost_ext::is_convertible<
            range_traversal,
            boost::random_access_traversal_tag
        >,
        boost_ext::mpl::at<
            Params, sake::range::keyword::tag::size_enable,
            boost::false_type
        >
    >::type size_enable;
};

} // namespace private_

} // namespace facade_adl

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_PRIVATE_FACADE_TRAITS_HPP

/*******************************************************************************
 * sake/core/range/private/adaptor/traits.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_PRIVATE_ADAPTOR_TRAITS_HPP
#define SAKE_CORE_RANGE_PRIVATE_ADAPTOR_TRAITS_HPP

#include <boost/concept/assert.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/has_key.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/map/map10.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/static_assert.hpp>

#include <sake/boost_ext/mpl/at.hpp>
#include <sake/boost_ext/mpl/lazy_at.hpp>
#include <sake/boost_ext/type_traits/is_reference.hpp>
#include <sake/boost_ext/type_traits/remove_reference.hpp>

#include <sake/core/iterator/traits.hpp>
#include <sake/core/range/concepts/Range.hpp>
#include <sake/core/range/facade.hpp>
#include <sake/core/range/keyword.hpp>
#include <sake/core/range/traits.hpp>
#include <sake/core/range/traits_fwd.hpp>
#include <sake/core/utility/template_keyword.hpp>

namespace sake
{

namespace range
{

namespace adaptor_private
{

template< class Derived, class R, class Params >
struct traits
{
    typedef typename boost_ext::remove_reference<R>::type base_;
    BOOST_CONCEPT_ASSERT((sake::concepts::Range< base_ >));
    BOOST_STATIC_ASSERT((boost::mpl::is_sequence< Params >::value));

    typedef typename boost_ext::mpl::lazy_at<
        Params, sake::range::keyword::tag::iterator,
        sake::range_iterator< base_ >
    >::type iterator;

    typedef typename boost_ext::mpl::lazy_at<
        Params, sake::range::keyword::tag::const_iterator,
        boost::mpl::eval_if_c<
            boost_ext::is_reference<R>::value,
            boost::mpl::identity< iterator >,
            sake::range_iterator< typename boost::add_const<R>::type >
        >
    >::type const_iterator;

    typedef typename boost_ext::mpl::lazy_at<
        Params, sake::range::keyword::tag::distance_enable,
        sake::range_distance_enable< base_ >
    >::type distance_enable;

    typedef typename boost_ext::mpl::lazy_at<
        Params, sake::range::keyword::tag::size_enable,
        sake::range_size_enable< base_ >
    >::type size_enable;

    typedef boost::mpl::map4<
        sake::range::keyword::iterator< iterator >,
        sake::range::keyword::const_iterator< const_iterator >,
        sake::range::keyword::distance_enable< distance_enable >,
        sake::range::keyword::size_enable< size_enable >
    > nominal_facade_param_types;
    typedef typename sake::lazy_insert_keyword_value_if_c<
        boost::mpl::has_key<
            Params, sake::range::keyword::tag::chained_base >::value,
        nominal_facade_param_types,
        sake::range::keyword::chained_base,
        boost_ext::mpl::at<
            Params, sake::range::keyword::tag::chained_base >
    >::type facade_param_types;

    typedef sake::range::facade< Derived, facade_param_types > facade_;
};

} // namespace adaptor_private

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_PRIVATE_ADAPTOR_TRAITS_HPP

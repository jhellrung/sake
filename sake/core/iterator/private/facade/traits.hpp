/*******************************************************************************
 * sake/core/iterator/private/facade/traits.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_PRIVATE_FACADE_TRAITS_HPP
#define SAKE_CORE_ITERATOR_PRIVATE_FACADE_TRAITS_HPP

#include <cstddef>

#include <boost/mpl/is_sequence.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/remove_const.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/mpl/at.hpp>
#include <sake/boost_ext/mpl/lazy_at.hpp>
#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/iterator/categories.hpp>
#include <sake/core/iterator/keyword.hpp>
#include <sake/core/utility/void.hpp>

namespace sake
{

namespace iterator
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
      Params, sake::iterator::keyword::tag::value,
      void
    >::type facade_value_type;
    typedef typename boost::remove_const< facade_value_type >::type value_type;
    typedef typename boost_ext::mpl::lazy_at<
        Params, sake::iterator::keyword::tag::reference,
        boost_ext::add_reference< facade_value_type >
    >::type reference;
    typedef typename boost_ext::mpl::at<
        Params, sake::iterator::keyword::tag::difference,
        std::ptrdiff_t
    >::type difference_type;

    typedef typename boost_ext::mpl::at<
        Params, sake::iterator::keyword::tag::traversal,
        boost::incrementable_traversal_tag
    >::type iterator_traversal;

    typedef typename boost_ext::mpl::at<
        Params, sake::iterator::keyword::tag::introterminal,
        sake::null_introterminal_tag
    >::type iterator_introterminal;

    typedef typename boost_ext::mpl::or2<
        boost_ext::is_convertible<
            iterator_traversal,
            boost::random_access_traversal_tag
        >,
        boost_ext::mpl::and2<
            boost_ext::is_convertible<
                iterator_traversal,
                boost::forward_traversal_tag
            >,
            boost_ext::mpl::at<
                Params, sake::iterator::keyword::tag::difference_enable,
                boost::false_type
            >
        >
    >::type difference_enable;

    typedef typename boost_ext::mpl::or2<
        difference_enable,
        boost_ext::mpl::and2<
            boost_ext::is_convertible<
                iterator_traversal,
                boost::forward_traversal_tag
            >,
            boost_ext::mpl::at<
                Params, sake::iterator::keyword::tag::compare_enable,
                boost::false_type
            >
        >
    >::type compare_enable;
};

} // namespace private_

} // namespace facade_adl

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_PRIVATE_FACADE_TRAITS_HPP

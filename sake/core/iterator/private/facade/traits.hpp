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

#include <boost/iterator/iterator_categories.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/remove_const.hpp>

#include <sake/boost_ext/mpl/at.hpp>
#include <sake/boost_ext/mpl/lazy_at.hpp>
#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/cursor/categories.hpp>
#include <sake/core/iterator/keyword.hpp>
#include <sake/core/utility/void.hpp>

namespace sake
{

namespace iterator_facade_adl
{

namespace private_
{

template< class Params >
struct traits
{
    typedef typename boost_ext::mpl::at<
      Params, sake::iterator_keyword::tag::value,
      void
    >::type facade_value_type;
    typedef typename boost::remove_const< facade_value_type >::type value_type;
    typedef typename boost_ext::mpl::lazy_at<
        Params, sake::iterator_keyword::tag::reference,
        boost_ext::add_reference< facade_value_type >
    >::type reference;
    typedef typename boost_ext::mpl::at<
        Params, sake::iterator_keyword::tag::difference,
        std::ptrdiff_t
    >::type difference_type;

    typedef typename boost_ext::mpl::at<
        Params, sake::iterator_keyword::tag::traversal,
        boost::incrementable_traversal_tag
    >::type iterator_traversal;

    typedef typename boost_ext::mpl::at<
        Params, sake::iterator_keyword::tag::introversal,
        sake::null_introversal_tag
    >::type cursor_introversal;

    typedef typename boost_ext::mpl::or2<
        boost_ext::is_convertible<
            iterator_traversal,
            boost::random_access_traversal_tag
        >,
        boost_ext::mpl::at<
            Params, sake::iterator_keyword::tag::difference_enable,
            boost::false_type
        >
    >::type difference_enable;
    typedef typename boost_ext::mpl::or2<
        difference_enable,
        boost_ext::mpl::at<
            Params, sake::iterator_keyword::tag::compare_enable,
            boost::false_type
        >
    >::type compare_enable;
};

} // namespace private_

} // namespace iterator_facade_adl

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_PRIVATE_FACADE_TRAITS_HPP

/*******************************************************************************
 * sake/core/iterator/private/adaptor/traits.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_PRIVATE_ADAPTOR_TRAITS_HPP
#define SAKE_CORE_ITERATOR_PRIVATE_ADAPTOR_TRAITS_HPP

#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/has_key.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/map/map10.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/static_assert.hpp>

#include <sake/boost_ext/mpl/at.hpp>
#include <sake/boost_ext/mpl/lazy_at.hpp>
#include <sake/boost_ext/type_traits/add_const_if.hpp>
#include <sake/boost_ext/type_traits/is_reference.hpp>
#include <sake/boost_ext/type_traits/is_cv_or.hpp>

#include <sake/core/cursor/traits.hpp>
#include <sake/core/introspection/has_operator_less.hpp>
#include <sake/core/introspection/has_operator_minus.hpp>
#include <sake/core/iterator/facade.hpp>
#include <sake/core/iterator/is_const_iterator.hpp>
#include <sake/core/iterator/keyword.hpp>
#include <sake/core/iterator/traits.hpp>
#include <sake/core/utility/is_template_base_of.hpp>
#include <sake/core/utility/template_keyword.hpp>

namespace sake
{

namespace iterator_adaptor_private
{

template< class Derived, class Base, class Params >
struct traits
{
    BOOST_STATIC_ASSERT((!boost_ext::is_reference< Base >::value));
    BOOST_STATIC_ASSERT((!boost_ext::is_cv_or< Base >::value));

    typedef typename sake::iterator_value< Base >::type base_value_type;

    typedef typename boost_ext::mpl::lazy_at<
        Params, sake::iterator_keyword::tag::value,
        boost_ext::add_const_if_c<
            sake::is_const_iterator< Base >::value,
            base_value_type
        >
    >::type facade_value_type;

    typedef typename boost_ext::mpl::lazy_at<
        Params, sake::iterator_keyword::tag::reference,
        boost::mpl::eval_if_c<
            boost::mpl::has_key<
                Params, sake::iterator_keyword::tag::value >::value,
            boost::mpl::identity< facade_value_type & >,
            sake::iterator_reference< Base >
        >
    >::type reference;

    typedef typename boost_ext::mpl::lazy_at<
        Params, sake::iterator_keyword::tag::difference,
        sake::iterator_difference< Base >
    >::type difference_type;

    typedef typename boost_ext::mpl::lazy_at<
        Params, sake::iterator_keyword::tag::traversal,
        sake::iterator_traversal< Base >
    >::type iterator_traversal;

    typedef typename boost_ext::mpl::lazy_at<
        Params, sake::iterator_keyword::tag::introversal,
        sake::cursor_introversal< Base >
    >::type cursor_introversal;

    typedef typename boost_ext::mpl::at<
        Params, sake::iterator_keyword::tag::compare_enable,
        sake::is_template_base_of2<
            sake::iterator_facade, Base,
            sake::has_operator_less< boost::mpl::_1 >
        >
    >::type compare_enable;
    typedef typename boost_ext::mpl::at<
        Params, sake::iterator_keyword::tag::difference_enable,
        sake::is_template_base_of2<
            sake::iterator_facade, Base,
            sake::has_operator_minus< boost::mpl::_1 >
        >
    >::type difference_enable;

    typedef boost::mpl::map7<
        sake::iterator_keyword::value< facade_value_type >,
        sake::iterator_keyword::reference< reference >,
        sake::iterator_keyword::difference< difference_type >,
        sake::iterator_keyword::traversal< iterator_traversal >,
        sake::iterator_keyword::introversal< cursor_introversal >,
        sake::iterator_keyword::compare_enable< compare_enable >,
        sake::iterator_keyword::difference_enable< difference_enable >
    > nominal_param_types;
    typedef typename sake::lazy_insert_keyword_value_if_c<
        boost::mpl::has_key<
            Params, sake::iterator_keyword::tag::chained_base >::value,
        nominal_param_types,
        sake::iterator_keyword::chained_base,
        boost_ext::mpl::at<
            Params, sake::iterator_keyword::tag::chained_base >
    >::type param_types;

    typedef sake::iterator_facade< Derived, param_types > iterator_facade_;
};

} // namespace iterator_adaptor_private

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_PRIVATE_ADAPTOR_TRAITS_HPP

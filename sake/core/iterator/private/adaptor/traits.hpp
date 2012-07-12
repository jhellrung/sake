/*******************************************************************************
 * sake/core/iterator/private/adaptor/traits.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_PRIVATE_ADAPTOR_TRAITS_HPP
#define SAKE_CORE_ITERATOR_PRIVATE_ADAPTOR_TRAITS_HPP

#include <boost/concept/assert.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/has_key.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/map/map10.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/static_assert.hpp>

#include <sake/boost_ext/mpl/at.hpp>
#include <sake/boost_ext/mpl/lazy_at.hpp>
#include <sake/boost_ext/type_traits/add_const_if.hpp>
#include <sake/boost_ext/type_traits/is_reference.hpp>
#include <sake/boost_ext/type_traits/is_cv_or.hpp>
#include <sake/boost_ext/type_traits/remove_reference.hpp>

#include <sake/core/introspection/has_operator_less.hpp>
#include <sake/core/introspection/has_operator_minus.hpp>
#include <sake/core/iterator/concepts/fwd.hpp>
#include <sake/core/iterator/concepts/Iterator.hpp>
#include <sake/core/iterator/facade.hpp>
#include <sake/core/iterator/is_const_iterator.hpp>
#include <sake/core/iterator/keyword.hpp>
#include <sake/core/iterator/traits.hpp>
#include <sake/core/utility/is_template_base_of.hpp>
#include <sake/core/utility/template_keyword.hpp>

namespace sake
{

namespace iterator
{

namespace adaptor_private
{

template< class Derived, class I, class Params >
struct traits
{
    BOOST_STATIC_ASSERT((!boost_ext::is_reference<I>::value));
    BOOST_STATIC_ASSERT((!boost_ext::is_cv_or<I>::value));
    BOOST_CONCEPT_ASSERT((sake::concepts::Iterator<I>));
    BOOST_STATIC_ASSERT((boost::mpl::is_sequence< Params >::value));

    typedef typename sake::iterator_value<I>::type base_value_type;

    typedef typename boost_ext::mpl::lazy_at<
        Params, sake::iterator::keyword::tag::value,
        boost_ext::remove_reference<
            typename boost_ext::mpl::lazy_at<
                Params, sake::iterator::keyword::tag::reference,
                boost_ext::add_const_if_c<
                    sake::is_const_iterator<I>::value,
                    base_value_type
                >
            >::type
        >
    >::type facade_value_type;

    typedef typename boost_ext::mpl::lazy_at<
        Params, sake::iterator::keyword::tag::reference,
        boost::mpl::eval_if<
            boost::mpl::has_key<
                Params, sake::iterator::keyword::tag::value >,
            boost::mpl::identity< facade_value_type & >,
            sake::iterator_reference<I>
        >
    >::type reference;

    typedef typename boost_ext::mpl::lazy_at<
        Params, sake::iterator::keyword::tag::difference,
        sake::iterator_difference<I>
    >::type difference_type;

    typedef typename boost_ext::mpl::lazy_at<
        Params, sake::iterator::keyword::tag::traversal,
        sake::iterator_traversal<I>
    >::type iterator_traversal;

    typedef typename boost_ext::mpl::lazy_at<
        Params, sake::iterator::keyword::tag::introterminal,
        sake::iterator_introterminal<I>
    >::type iterator_introterminal;

    typedef typename boost_ext::mpl::lazy_at<
        Params, sake::iterator::keyword::tag::compare_enable,
        sake::is_template_base_of2<
            sake::iterator::facade, I,
            sake::has_operator_less< boost::mpl::_1 >
        >
    >::type compare_enable;
    typedef typename boost_ext::mpl::lazy_at<
        Params, sake::iterator::keyword::tag::difference_enable,
        sake::is_template_base_of2<
            sake::iterator::facade, I,
            sake::has_operator_minus<
                boost::mpl::_1, boost::mpl::_1, difference_type >
        >
    >::type difference_enable;

    typedef boost::mpl::map7<
        sake::iterator::keyword::value< facade_value_type >,
        sake::iterator::keyword::reference< reference >,
        sake::iterator::keyword::difference< difference_type >,
        sake::iterator::keyword::traversal< iterator_traversal >,
        sake::iterator::keyword::introterminal< iterator_introterminal >,
        sake::iterator::keyword::compare_enable< compare_enable >,
        sake::iterator::keyword::difference_enable< difference_enable >
    > nominal_facade_param_types;
    typedef typename sake::lazy_insert_keyword_value_if_c<
        boost::mpl::has_key<
            Params, sake::iterator::keyword::tag::chained_base >::value,
        nominal_facade_param_types,
        sake::iterator::keyword::chained_base,
        boost_ext::mpl::at<
            Params, sake::iterator::keyword::tag::chained_base >
    >::type facade_param_types;

    typedef sake::iterator::facade< Derived, facade_param_types > facade_;
};

} // namespace adaptor_private

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_PRIVATE_ADAPTOR_TRAITS_HPP

/*******************************************************************************
 * core/iterator/private/category.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_PRIVATE_CATEGORY_HPP
#define SAKE_CORE_ITERATOR_PRIVATE_CATEGORY_HPP

#include <iterator>

#include <boost/iterator/iterator_categories.hpp>
#include <boost/static_assert.hpp>

#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>

namespace sake
{

namespace iterator_private
{

struct input_output_iterator_tag
    : std::input_iterator_tag
{
    operator std::output_iterator_tag() const
    { return std::output_iterator_tag(); }
};

template< class Category, class Traversal >
struct category_with_traversal
    : Category, Traversal
{ };

template< class Traversal, class Value >
struct category
{
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        Traversal, boost::incrementable_traversal_tag
    >::value));
    BOOST_STATIC_ASSERT((boost_ext::mpl::or2<
        boost_ext::is_convertible< Traversal, std::input_iterator_tag >,
        boost_ext::is_convertible< Traversal, std::output_iterator_tag >
    >::value));
    typedef Traversal type;
};

template< class Value >
struct category< boost::incrementable_traversal_tag, Value >
{
    typedef category_with_traversal<
        std::output_iterator_tag,
        boost::incrementable_traversal_tag
    > type;
};

template< class Value >
struct category< boost::single_pass_traversal_tag, Value >
{
    typedef category_with_traversal<
        input_output_iterator_tag,
        boost::single_pass_traversal_tag
    > type;
};

template< class Value >
struct category< boost::single_pass_traversal_tag, Value const >
{
    typedef category_with_traversal<
        std::input_iterator_tag,
        boost::single_pass_traversal_tag
    > type;
};

template< class Value >
struct category< boost::forward_traversal_tag, Value >
{
    typedef category_with_traversal<
        std::forward_iterator_tag,
        boost::forward_traversal_tag
    > type;
};

template< class Value >
struct category< boost::bidirectional_traversal_tag, Value >
{
    typedef category_with_traversal<
        std::bidirectional_iterator_tag,
        boost::bidirectional_traversal_tag
    > type;
};

template< class Value >
struct category< boost::random_access_traversal_tag, Value >
{
    typedef category_with_traversal<
        std::random_access_iterator_tag,
        boost::random_access_traversal_tag
    > type;
};

} // namespace iterator_private

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_PRIVATE_CATEGORY_HPP

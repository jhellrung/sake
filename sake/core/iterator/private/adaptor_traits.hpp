/*******************************************************************************
 * core/iterator/private/adaptor_traits.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_PRIVATE_ADAPTOR_TRAITS_HPP
#define SAKE_CORE_ITERATOR_PRIVATE_ADAPTOR_TRAITS_HPP

#include <boost/iterator/iterator_traits.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>

#include <sake/boost_ext/type_traits/add_const_if.hpp>

#include <sake/core/cursor/introversal.hpp>
#include <sake/core/iterator/facade.hpp>
#include <sake/core/utility/default_tag.hpp>

namespace sake
{

namespace iterator_adaptor_private
{

template<
    class Derived,
    class IteratorBase,
    class Value, class Reference, class Difference,
    class Traversal, class Introversal,
    class ChainedBase
>
struct traits
{
    typedef typename sake::lazy_replace_default_tag<
        Value,
        boost::iterator_value< IteratorBase >
    >::type facade_value_type;
    typedef typename sake::lazy_replace_default_tag<
        Reference,
        boost::mpl::eval_if_c<
            sake::is_default_tag< Value >::value,
            boost::iterator_reference< IteratorBase >,
            boost::mpl::identity< Value& >
        >
    >::type reference;
    typedef typename sake::lazy_replace_default_tag<
        Difference,
        boost::iterator_difference< IteratorBase >
    >::type difference_type;
    typedef typename sake::lazy_replace_default_tag<
        Traversal,
        boost::iterator_traversal< IteratorBase >
    >::type traversal_type;
    typedef typename sake::lazy_replace_default_tag<
        Introversal,
        sake::cursor_introversal< IteratorBase >
    >::type introversal_type;
    typedef sake::iterator_facade<
        Derived,
        facade_value_type, reference, difference_type,
        traversal_type, introversal_type,
        ChainedBase
    > iterator_facade_;
};

} // namespace iterator_adaptor_private

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_PRIVATE_ADAPTOR_TRAITS_HPP

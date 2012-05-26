/*******************************************************************************
 * sake/core/iterator/concepts/Forward.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct iterator_concepts::Forward<I>
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_CONCEPTS_FORWARD_HPP
#define SAKE_CORE_ITERATOR_CONCEPTS_FORWARD_HPP

#include <boost/iterator/iterator_categories.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/is_signed.hpp>

#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/concepts/DefaultConstructible.hpp>
#include <sake/core/iterator/concepts/SinglePass.hpp>
#include <sake/core/iterator/traits.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake
{

namespace iterator_concepts
{

template< class I >
struct Forward
    : sake::concepts::DefaultConstructible<I>,
      sake::iterator_concepts::SinglePass<I>
{
private:
    typedef sake::iterator_traits<I> traits_;
public:
    SAKE_USING_TYPEDEF( typename traits_, difference_type );
    SAKE_USING_TYPEDEF( typename traits_, traversal );

    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        traversal,
        boost::forward_traversal_tag
    >::value));

    BOOST_STATIC_ASSERT((boost::is_integral< difference_type >::value));
    BOOST_STATIC_ASSERT((boost::is_signed< difference_type >::value));
};

} // namespace iterator_concepts

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_CONCEPTS_FORWARD_HPP

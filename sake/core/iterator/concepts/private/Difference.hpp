/*******************************************************************************
 * sake/core/iterator/concepts/private/Difference.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_CONCEPTS_PRIVATE_DIFFERENCE_HPP
#define SAKE_CORE_ITERATOR_CONCEPTS_PRIVATE_DIFFERENCE_HPP

#include <boost/concept/usage.hpp>

#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/config.hpp>
#include <sake/core/iterator/traits.hpp>
#include <sake/core/iterator/traits_fwd.hpp>

namespace sake
{

namespace iterator
{

namespace concepts
{

namespace private_
{

template<
    class I, class Tag,
    bool = boost_ext::is_convertible<
               typename sake::iterator_traversal<I>::type,
               boost::random_access_traversal_tag
           >::value
>
struct Difference;

template< class I, class Tag >
struct Difference< I, Tag, false >
{ };

template< class I, class Tag >
struct Difference< I, Tag, true >
{
    typedef typename sake::iterator_difference<I>::type difference_type;

    BOOST_CONCEPT_USAGE( Difference )
    {
        assert_result(i - Tag());
        assert_result(Tag() - i);
    }

private:
    I const i;

    void assert_result(difference_type);

    SAKE_SUPPRESS_WARNING_UNINIITIALIZED_IN_CONCEPT_CHECKING_CLASS( Difference )
};

} // namespace private_

} // namespace concepts

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_CONCEPTS_PRIVATE_DIFFERENCE_HPP

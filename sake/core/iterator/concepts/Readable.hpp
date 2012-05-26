/*******************************************************************************
 * sake/core/iterator/concepts/Readable.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct iterator_concepts::Readable<I>
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_CONCEPTS_READABLE_HPP
#define SAKE_CORE_ITERATOR_CONCEPTS_READABLE_HPP

#include <boost/concept/usage.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_void.hpp>

#include <sake/boost_ext/type_traits/is_cv_or.hpp>
#include <sake/boost_ext/type_traits/is_reference.hpp>

#include <sake/core/config.hpp>
#include <sake/core/concepts/CopyAssignable.hpp>
#include <sake/core/concepts/CopyConstructible.hpp>
#include <sake/core/iterator/traits.hpp>

namespace sake
{

namespace iterator_concepts
{

template< class I >
struct Readable
    : sake::concepts::CopyConstructible<I>,
      sake::concepts::CopyAssignable<I>
{
    typedef typename sake::iterator_value<I>::type value_type;
    BOOST_STATIC_ASSERT((!boost_ext::is_reference< value_type >::value));
    BOOST_STATIC_ASSERT((!boost_ext::is_cv_or< value_type >::value));
    BOOST_STATIC_ASSERT((!boost::is_void< value_type >::value));

    BOOST_CONCEPT_USAGE( Readable )
    {
        assert_result(*i);
    }

private:
    I const i;

    void assert_result(value_type const &);

    SAKE_SUPPRESS_WARNING_UNINIITIALIZED_IN_CONCEPT_CHECKING_CLASS( Readable )
};

} // namespace iterator_concepts

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_CONCEPTS_READABLE_HPP

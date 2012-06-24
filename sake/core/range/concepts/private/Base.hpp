/*******************************************************************************
 * sake/core/range/concepts/private/Base.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_CONCEPTS_PRIVATE_BASE_HPP
#define SAKE_CORE_RANGE_CONCEPTS_PRIVATE_BASE_HPP

// #include <boost/static_assert.hpp>

// #include <sake/boost_ext/type_traits/is_convertible.hpp>

// #include <sake/core/concepts/CopyAssignable.hpp>
// #include <sake/core/concepts/CopyConstructible.hpp>
// #include <sake/core/concepts/Dereferenceable.hpp>
// #include <sake/core/concepts/Incrementable.hpp>
// #include <sake/core/iterator/categories.hpp>
#include <sake/core/iterator/traits.hpp>
#include <sake/core/range/traits.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake
{

namespace range
{

namespace concepts
{

namespace private_
{

template< class R >
struct Base
{
private:
    typedef sake::range_traits<R> traits_;
public:
    SAKE_USING_TYPEDEF( typename traits_, iterator );
    SAKE_USING_TYPEDEF( typename traits_, size_type );
    typedef sake::iterator_traits< iterator > iterator_traits;

    // SAKE_USING_TYPEDEF( typename traits_, value_type );
    // SAKE_USING_TYPEDEF( typename traits_, reference );
    // SAKE_USING_TYPEDEF( typename traits_, pointer );
    // SAKE_USING_TYPEDEF( typename traits_, difference_type );
    // SAKE_USING_TYPEDEF( typename traits_, traversal );
    // SAKE_USING_TYPEDEF( typename traits_, introversal );

    // BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        // traversal, boost::incrementable_traversal_tag >::value));
    // BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        // introversal, sake::null_introversal_tag >::value));

    // template< class Introversal >
    // struct relax
    // { typedef typename traits_::template relax< Introversal >::type type; };

    // BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        // I, typename relax< introversal >::type >::value));
    // BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        // typename relax< introversal >::type, I >::value));
};

} // namespace private_

} // namespace concepts

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_CONCEPTS_PRIVATE_BASE_HPP

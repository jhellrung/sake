/*******************************************************************************
 * sake/core/iterator/concepts/private/Base.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_CONCEPTS_PRIVATE_BASE_HPP
#define SAKE_CORE_ITERATOR_CONCEPTS_PRIVATE_BASE_HPP

#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>

#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/concepts/CopyAssignable.hpp>
#include <sake/core/concepts/CopyConstructible.hpp>
#include <sake/core/concepts/Dereferenceable.hpp>
#include <sake/core/concepts/Incrementable.hpp>
#include <sake/core/iterator/categories.hpp>
#include <sake/core/iterator/traits.hpp>
#include <sake/core/iterator/traits_fwd.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake
{

namespace iterator
{

namespace concepts
{

namespace private_
{

template< class I >
struct Base
    : sake::concepts::CopyConstructible<I>,
      sake::concepts::CopyAssignable<I>,
      sake::concepts::Dereferenceable< I const >,
      sake::concepts::Incrementable<I>
{
private:
    typedef sake::iterator_traits<I> traits_;
public:
    SAKE_USING_TYPEDEF( typename traits_, value_type );
    SAKE_USING_TYPEDEF( typename traits_, reference );
    SAKE_USING_TYPEDEF( typename traits_, pointer );
    SAKE_USING_TYPEDEF( typename traits_, difference_type );
    SAKE_USING_TYPEDEF( typename traits_, traversal );
    SAKE_USING_TYPEDEF( typename traits_, introterminal );

    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        traversal, boost::incrementable_traversal_tag >::value));
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        introterminal, sake::null_introterminal_tag >::value));

    template< class Introterminal >
    struct relax
    { typedef typename traits_::template relax< Introterminal >::type type; };

    BOOST_STATIC_ASSERT((
        boost::is_same< I, typename relax< introterminal >::type >::value));
};

} // namespace private_

} // namespace concepts

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_CONCEPTS_PRIVATE_BASE_HPP

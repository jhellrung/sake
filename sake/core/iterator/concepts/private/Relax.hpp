/*******************************************************************************
 * sake/core/iterator/concepts/private/Relax.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_CONCEPTS_PRIVATE_RELAX_HPP
#define SAKE_CORE_ITERATOR_CONCEPTS_PRIVATE_RELAX_HPP

#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>

#include <sake/core/iterator/concepts/private/IntroterminalBase_fwd.hpp>
#include <sake/core/iterator/concepts/private/TraversalBase.hpp>
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

template<
    class I, class Introterminal,
    class J = typename sake::iterator_relax< I, Introterminal >::type
>
struct Relax
    : sake::iterator::concepts::private_::TraversalBase<J>,
      sake::iterator::concepts::private_::IntroterminalBase< J, Introterminal >
{
private:
    typedef sake::iterator_traits<I> traits_;
    typedef sake::iterator_traits<J> relax_traits;
public:
    SAKE_USING_TYPEDEF( typename traits_, value_type );
    SAKE_USING_TYPEDEF( typename traits_, reference );
    SAKE_USING_TYPEDEF( typename traits_, difference_type );
    SAKE_USING_TYPEDEF( typename traits_, traversal );
#define assert_is_same( type ) \
    BOOST_STATIC_ASSERT((boost::is_same< \
        typename relax_traits::type, type >::value));
    assert_is_same( value_type )
    assert_is_same( reference )
    assert_is_same( difference_type )
    assert_is_same( traversal )
#undef assert_is_same
    SAKE_USING_TYPEDEF( typename traits_, introterminal );
    BOOST_STATIC_ASSERT((!boost::is_same<
        typename relax_traits::introterminal, introterminal >::value));
};

template< class I, class Introterminal >
struct Relax< I, Introterminal, I >
{ };

} // namespace private_

} // namespace concepts

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_CONCEPTS_PRIVATE_RELAX_HPP

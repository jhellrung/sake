/*******************************************************************************
 * sake/core/iterator/concepts/private/TraversalBase.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_CONCEPTS_PRIVATE_TRAVERSAL_BASE_HPP
#define SAKE_CORE_ITERATOR_CONCEPTS_PRIVATE_TRAVERSAL_BASE_HPP

#include <boost/concept/usage.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_signed.hpp>

#include <sake/boost_ext/mpl/if.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/concepts/Decrementable.hpp>
#include <sake/core/concepts/EqualityComparable.hpp>
#include <sake/core/config.hpp>
#include <sake/core/iterator/categories.hpp>
#include <sake/core/iterator/concepts/private/Base.hpp>
#include <sake/core/iterator/traits.hpp>
#include <sake/core/iterator/traits_fwd.hpp>
#include <sake/core/utility/int_tag.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake
{

namespace iterator
{

namespace concepts
{

namespace private_
{

template< class Traversal >
struct TraversalBase_index
{
#define is_convertible_( x ) \
    boost_ext::is_convertible< Traversal, boost::x ## _traversal_tag >
    static int const value = boost_ext::mpl::
         if_< is_convertible_( random_access ),
              sake::int_tag<4> >::type::template
    else_if < is_convertible_( bidirectional ),
              sake::int_tag<3> >::type::template
    else_if < is_convertible_( forward ),
              sake::int_tag<2> >::type::template
    else_if < is_convertible_( single_pass ),
              sake::int_tag<1> >::type::template
    else_   < sake::int_tag<0> >::type::value;
#undef is_convertible_
};

template<
    class I,
    class Traversal = typename sake::iterator_traversal<I>::type,
    int = TraversalBase_index< Traversal >::value
>
struct TraversalBase;

// Incrementable
template< class I, class Traversal >
struct TraversalBase< I, Traversal, 0 >
    : sake::iterator::concepts::private_::Base<I>
{
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        typename sake::iterator_traversal<I>::type,
        boost::incrementable_traversal_tag
    >::value));
};

// SinglePass
template< class I, class Traversal >
struct TraversalBase< I, Traversal, 1 >
    : sake::iterator::concepts::private_::TraversalBase<
          I, boost::incrementable_traversal_tag >,
      sake::concepts::EqualityComparable<I>
{
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        typename sake::iterator_traversal<I>::type,
        boost::single_pass_traversal_tag
    >::value));
};

// Forward
template< class I, class Traversal >
struct TraversalBase< I, Traversal, 2 >
    : sake::iterator::concepts::private_::TraversalBase<
          I, boost::single_pass_traversal_tag >
{
private:
    typedef sake::iterator_traits<I> traits_;
public:
    SAKE_USING_TYPEDEF( typename traits_, difference_type );
    SAKE_USING_TYPEDEF( typename traits_, traversal );
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        traversal, boost::forward_traversal_tag >::value));
    BOOST_STATIC_ASSERT((boost::is_signed< difference_type >::value));
};

// Bidirectional
template< class I, class Traversal >
struct TraversalBase< I, Traversal, 3 >
    : sake::iterator::concepts::private_::TraversalBase<
          I, boost::forward_traversal_tag >,
      sake::concepts::Decrementable<I>
{
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        typename sake::iterator_traversal<I>::type,
        boost::bidirectional_traversal_tag
    >::value));
};

// RandomAccess
template< class I, class Traversal >
struct TraversalBase< I, Traversal, 4 >
    : sake::iterator::concepts::private_::TraversalBase<
          I, boost::bidirectional_traversal_tag >
{
private:
    typedef sake::iterator_traits<I> traits_;
public:
    SAKE_USING_TYPEDEF( typename traits_, reference );
    SAKE_USING_TYPEDEF( typename traits_, difference_type );
    SAKE_USING_TYPEDEF( typename traits_, traversal );

    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        traversal, boost::random_access_traversal_tag >::value));

    BOOST_CONCEPT_USAGE( TraversalBase )
    {
        assert_result< I& >(i += n);
        assert_result< I& >(i -= n);
        assert_result<I>(ci + n);
        assert_result<I>(n + ci);
        assert_result<I>(ci - n);
        assert_result< difference_type >(ci - cj);
        assert_result< reference >(ci[n]);
    }

private:
    I i;
    I j;
    I const ci;
    I const cj;
    difference_type const n;

    template< class T >
    void assert_result(T);

    SAKE_SUPPRESS_WARNING_UNINIITIALIZED_IN_CONCEPT_CHECKING_CLASS( TraversalBase )
};

} // namespace private_

} // namespace concepts

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_CONCEPTS_PRIVATE_TRAVERSAL_BASE_HPP

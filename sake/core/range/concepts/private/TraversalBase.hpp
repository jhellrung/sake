/*******************************************************************************
 * sake/core/range/concepts/private/TraversalBase.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_CONCEPTS_PRIVATE_TRAVERSAL_BASE_HPP
#define SAKE_CORE_RANGE_CONCEPTS_PRIVATE_TRAVERSAL_BASE_HPP

#include <boost/concept/usage.hpp>
#include <boost/static_assert.hpp>

#include <sake/boost_ext/mpl/if.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/config.hpp>
#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/iterator/categories.hpp>
#include <sake/core/range/concepts/fwd.hpp>
#include <sake/core/range/concepts/private/Base.hpp>
#include <sake/core/range/traits.hpp>
#include <sake/core/utility/int_tag.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake
{

namespace range
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

// SinglePass
template< class R, class Traversal >
struct TraversalBase< R, Traversal, 1 >
    : sake::range::concepts::private_::Base<R>
{
private:
    typedef sake::range_traits<R> traits_;
public:
    SAKE_USING_TYPEDEF( typename traits_, reference );
    SAKE_USING_TYPEDEF( typename traits_, traversal );
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        traversal, boost::single_pass_traversal_tag >::value));

    BOOST_CONCEPT_USAGE( TraversalBase )
    {
        assert_result(traits_::at(r, sake::_begin));
    }
private:
    R r;

    void assert_result(reference);

    SAKE_SUPPRESS_WARNING_UNINIITIALIZED_IN_CONCEPT_CHECKING_CLASS( TraversalBase )
};

// Forward
template< class R, class Traversal >
struct TraversalBase< R, Traversal, 2 >
    : sake::range::concepts::private_::TraversalBase<
          R, boost::single_pass_traversal_tag >
{
private:
    typedef sake::range_traits<R> traits_;
public:
    SAKE_USING_TYPEDEF( typename traits_, traversal );
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        traversal, boost::forward_traversal_tag >::value));
    BOOST_STATIC_ASSERT((traits_::distance_enable_tag::value));
};

// Bidirectional
template< class R, class Traversal >
struct TraversalBase< R, Traversal, 3 >
    : sake::range::concepts::private_::TraversalBase<
          R, boost::forward_traversal_tag >
{
private:
    typedef sake::range_traits<R> traits_;
public:
    SAKE_USING_TYPEDEF( typename traits_, reference );
    SAKE_USING_TYPEDEF( typename traits_, traversal );
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        traversal, boost::bidirectional_traversal_tag >::value));

    BOOST_CONCEPT_USAGE( TraversalBase )
    {
        assert_result(traits_::at(r, sake::_end));
    }
private:
    R r;

    void assert_result(reference);

    SAKE_SUPPRESS_WARNING_UNINIITIALIZED_IN_CONCEPT_CHECKING_CLASS( TraversalBase )
};

// RandomAccess
template< class R, class Traversal >
struct TraversalBase< R, Traversal, 4 >
    : sake::range::concepts::private_::TraversalBase<
          R, boost::bidirectional_traversal_tag >
{
private:
    typedef sake::range_traits<R> traits_;
public:
    SAKE_USING_TYPEDEF( typename traits_, reference );
    SAKE_USING_TYPEDEF( typename traits_, size_type );
    SAKE_USING_TYPEDEF( typename traits_, traversal );
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        traversal, boost::random_access_traversal_tag >::value));

    BOOST_STATIC_ASSERT((traits_::size_enable_tag::value));

    BOOST_CONCEPT_USAGE( TraversalBase )
    {
        assert_result(traits_::at(r,n));
    }
private:
    R r;
    size_type const n;

    void assert_result(reference);

    SAKE_SUPPRESS_WARNING_UNINIITIALIZED_IN_CONCEPT_CHECKING_CLASS( TraversalBase )
};

} // namespace private_

} // namespace concepts

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_CONCEPTS_PRIVATE_TRAVERSAL_BASE_HPP

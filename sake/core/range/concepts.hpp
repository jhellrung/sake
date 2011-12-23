/*******************************************************************************
 * sake/core/range/concepts.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct concepts::[Traversal][Access]Range<R[,V]>
 *
 * [Traversal] in { SinglePass, Forward, Bidirectional, RandomAccess }
 * [Access]    in { (empty), Readable, Writable, Swappable, ReadableWritable,
 *                  LValue }
 *
 * These are range concept checking classes that combine several commonly
 * occurring concept combinations.
 ******************************************************************************/

#ifndef BOOST_PP_IS_ITERATING

#ifndef SAKE_CORE_RANGE_CONCEPTS_HPP
#define SAKE_CORE_RANGE_CONCEPTS_HPP

#include <boost/concept/assert.hpp>
#include <boost/concept/usage.hpp>
#include <boost/iterator/iterator_concepts.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/seq/elem.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/range/concepts.hpp>
#include <boost/range/iterator.hpp>
#include <boost/range/value_type.hpp>

namespace sake
{

namespace concepts
{

#define Traversals \
    ( SinglePass ) \
    ( Forward ) \
    ( Bidirectional ) \
    ( RandomAccess )

#define BOOST_PP_ITERATION_LIMITS ( 0, BOOST_PP_SEQ_SIZE( Traversals ) - 1 )
#define BOOST_PP_FILENAME_1       <sake/core/range/concepts.hpp>
#include BOOST_PP_ITERATE()

#undef Traversals

} // namespace concepts

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_CONCEPTS_HPP

#else // #ifndef BOOST_PP_IS_ITERATING

#define N BOOST_PP_ITERATION()

#define Traversal BOOST_PP_SEQ_ELEM( N, Traversals )

#define boost_TraversalRangeConcept boost:: BOOST_PP_CAT( Traversal, RangeConcept )

#define TraversalRange                 BOOST_PP_CAT( Traversal, Range )
#define TraversalReadableRange         BOOST_PP_CAT( Traversal, ReadableRange )
#define TraversalWritableRange         BOOST_PP_CAT( Traversal, WritableRange )
#define TraversalSwappableRange        BOOST_PP_CAT( Traversal, SwappableRange )
#define TraversalReadableWritableRange BOOST_PP_CAT( Traversal, ReadableWritableRange )
#define TraversalLvalueRange           BOOST_PP_CAT( Traversal, LvalueRange )

template< class R >
struct TraversalRange
    : boost_TraversalRangeConcept<R>
{ };

template< class R >
struct TraversalReadableRange
    : boost_TraversalRangeConcept<R>
{
    BOOST_CONCEPT_ASSERT((boost_concepts::ReadableIteratorConcept<
        typename boost::range_iterator<R>::type
    >));
};

template< class R, class V = typename boost::range_value<R>::type >
struct TraversalWritableRange
    : boost_TraversalRangeConcept<R>
{
    BOOST_CONCEPT_ASSERT((boost_concepts::WritableIteratorConcept<
        typename boost::range_iterator<R>::type, V
    >));
};

template< class R >
struct TraversalSwappableRange
    : boost_TraversalRangeConcept<R>
{
    BOOST_CONCEPT_ASSERT((boost_concepts::SwappableIteratorConcept<
        typename boost::range_iterator<R>::type
    >));
};

template< class R, class V = typename boost::range_value<R>::type >
struct TraversalReadableWritableRange
    : boost_TraversalRangeConcept<R>
{
    BOOST_CONCEPT_ASSERT((boost_concepts::ReadableIteratorConcept<
        typename boost::range_iterator<R>::type
    >));
    BOOST_CONCEPT_ASSERT((boost_concepts::WritableIteratorConcept<
        typename boost::range_iterator<R>::type, V
    >));
};

template< class R >
struct TraversalLvalueRange
    : boost_TraversalRangeConcept<R>
{
    BOOST_CONCEPT_ASSERT((boost_concepts::LvalueIteratorConcept<
        typename boost::range_iterator<R>::type
    >));
};

#undef TraversalRange
#undef TraversalReadableRange
#undef TraversalWritableRange
#undef TraversalSwappableRange
#undef TraversalReadableWritableRange
#undef TraversalLvalueRange

#undef boost_TraversalRangeConcept

#undef Traversal

#undef N

#endif // #ifndef BOOST_PP_IS_ITERATING

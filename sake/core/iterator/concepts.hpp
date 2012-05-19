/*******************************************************************************
 * sake/core/iterator/concepts.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct concepts::[Traversal][Access]Iterator<I>
 *
 * [Traversal] in { Incrementable, SinglePass, Forward, Bidirectional,
 *                  RandomAccess }
 * [Access]    in { (empty), Readable, Writable, Swappable, ReadableWritable,
 *                  LValue }
 *
 * These are iterator concept checking classes that combine several commonly
 * occuring concept combinations.
 ******************************************************************************/

#ifndef BOOST_PP_IS_ITERATING

#ifndef SAKE_CORE_ITERATOR_CONCEPTS_HPP
#define SAKE_CORE_ITERATOR_CONCEPTS_HPP

#include <boost/config.hpp>
#include <boost/concept/assert.hpp>
#include <boost/concept/usage.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/seq/elem.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/tuple/elem.hpp>

#include <sake/core/utility/workaround.hpp>

//#if BOOST_VERSION <= 104900
// && SAKE_WORKAROUND_GNUC_VERSION_GREATER_EQUAL( (4,6,3) )
#if BOOST_VERSION <= 104900 && defined( __GNUC__ )
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#endif // #if ...
#include <boost/iterator/iterator_concepts.hpp>
#if BOOST_VERSION <= 104900 && defined( __GNUC__ )
#pragma GCC diagnostic pop
#endif // #if ...

namespace sake
{

namespace concepts
{

#define Traversals \
    (( Incrementable, IncrementableIterator  )) \
    (( SinglePass,    SinglePassIterator     )) \
    (( Forward,       ForwardTraversal       )) \
    (( Bidirectional, BidirectionalTraversal )) \
    (( RandomAccess,  RandomAccessTraversal  ))

#define BOOST_PP_ITERATION_LIMITS ( 0, BOOST_PP_SEQ_SIZE( Traversals ) - 1 )
#define BOOST_PP_FILENAME_1       <sake/core/iterator/concepts.hpp>
#include BOOST_PP_ITERATE()

#undef Traversals

} // namespace concepts

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_CONCEPTS_HPP

#else // #ifndef BOOST_PP_IS_ITERATING

#define N BOOST_PP_ITERATION()

#define Traversal_boost_Traversal  BOOST_PP_SEQ_ELEM( N, Traversals )
#define Traversal                  BOOST_PP_TUPLE_ELEM( 2, 0, Traversal_boost_Traversal )
#define boost_Traversal            BOOST_PP_TUPLE_ELEM( 2, 1, Traversal_boost_Traversal )

#define boost_TraversalConcept boost_concepts:: BOOST_PP_CAT( boost_Traversal, Concept )

#define TraversalIterator                 BOOST_PP_CAT( Traversal, Iterator )
#define TraversalReadableIterator         BOOST_PP_CAT( Traversal, ReadableIterator )
#define TraversalWritableIterator         BOOST_PP_CAT( Traversal, WritableIterator )
#define TraversalSwappableIterator        BOOST_PP_CAT( Traversal, SwappableIterator )
#define TraversalReadableWritableIterator BOOST_PP_CAT( Traversal, ReadableWritableIterator )
#define TraversalLValueIterator           BOOST_PP_CAT( Traversal, LValueIterator )

template< class I >
struct TraversalIterator
    : boost_TraversalConcept<I>
{ };

template< class I >
struct TraversalReadableIterator
    : boost_TraversalConcept<I>
{ BOOST_CONCEPT_ASSERT((boost_concepts::ReadableIteratorConcept<I>)); };

template< class I, class Value = typename boost::iterator_value<I>::type >
struct TraversalWritableIterator
    : boost_TraversalConcept<I>
{ BOOST_CONCEPT_ASSERT((boost_concepts::WritableIteratorConcept< I, Value >)); };

template< class I >
struct TraversalSwappableIterator
    : boost_TraversalConcept<I>
{ BOOST_CONCEPT_ASSERT((boost_concepts::SwappableIteratorConcept<I>)); };

template< class I, class Value = typename boost::iterator_value<I>::type >
struct TraversalReadableWritableIterator
    : boost_TraversalConcept<I>
{
    BOOST_CONCEPT_ASSERT((boost_concepts::ReadableIteratorConcept<I>));
    BOOST_CONCEPT_ASSERT((boost_concepts::WritableIteratorConcept< I, Value >));
};

template< class I >
struct TraversalLValueIterator
    : boost_TraversalConcept<I>
{ BOOST_CONCEPT_ASSERT((boost_concepts::LvalueIteratorConcept<I>)); };

#undef TraversalIterator
#undef TraversalReadableIterator
#undef TraversalWritableIterator
#undef TraversalSwappableIterator
#undef TraversalReadableWritableIterator
#undef TraversalLValueIterator

#undef boost_TraversalConcept

#undef Traversal
#undef boost_Traversal

#undef N

#endif // #ifndef BOOST_PP_IS_ITERATING

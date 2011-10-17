/*******************************************************************************
 * sake/core/iterator/archetypes.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct archetypes::[traversal]_[access]_iterator
 *
 * [traversal] in {incrementable, single_pass, forward, bidirectional,
 *                 random_access }
 * [access]    in {readable, writable, readable_writable, readable_lvalue,
 *                 writable_lvalue }
 ******************************************************************************/

#ifndef BOOST_PP_IS_ITERATING

#ifndef SAKE_CORE_ITERATOR_ARCHETYPES_HPP
#define SAKE_CORE_ITERATOR_ARCHETYPES_HPP

#include <boost/iterator/iterator_archetypes.hpp>
#include <boost/iterator/iterator_categories.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/seq/cat.hpp>
#include <boost/preprocessor/seq/elem.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/tuple/elem.hpp>

#include <sake/core/iterator/private/category.hpp>

namespace sake
{

namespace archetypes
{

#define traversal_seq \
    ( incrementable ) \
    ( single_pass ) \
    ( forward ) \
    ( bidirectional ) \
    ( random_access )
#define access_seq \
    (( readable                , void const )) \
    (( writable                , void )) \
    (( readable_writable       , void )) \
    (( readable_lvalue         , void const )) \
    (( writable_lvalue         , void )) \
    (( readable_writable_lvalue, void )) \

#define BOOST_PP_ITERATION_LIMITS ( 0, BOOST_PP_SEQ_SIZE( traversal_seq ) - 1 )
#define BOOST_PP_FILENAME_1       <sake/core/iterator/archetypes.hpp>
#include BOOST_PP_ITERATE()

#undef traversal_seq
#undef access_seq

} // namespace archetypes

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_ARCHETYPES_HPP

#elif BOOST_PP_ITERATION_DEPTH() == 1 // #ifndef BOOST_PP_IS_ITERATING

#define N1 BOOST_PP_FRAME_ITERATION( 1 )

#define traversal           BOOST_PP_SEQ_ELEM( N1, SAKE_traversal_seq )
#define boost_traversal_tag boost:: BOOST_PP_CAT( traversal, _traversal_tag )

#define BOOST_PP_ITERATION_LIMITS ( 0, BOOST_PP_SEQ_SIZE( access_seq ) - 1 )
#define BOOST_PP_FILENAME_2       <sake/core/iterator/archetypes.hpp>
#include BOOST_PP_ITERATE()

#undef traversal
#undef boost_traversal_tag

#undef N1

#else // #ifndef BOOST_PP_IS_ITERATING

#define N2 BOOST_PP_FRAME_ITERATION( 2 )

#define access_constness BOOST_PP_SEQ_ELEM( N2, access_seq )
#define access           BOOST_PP_TUPLE_ELEM( 2, 0, access_pair )
#define constness        BOOST_PP_TUPLE_ELEM( 2, 1, access_pair )

template< class T >
struct BOOST_PP_SEQ_CAT( ( traversal ) ( _ ) ( access ) ( _iterator ) )
    : boost::iterator_archetype<
          T,
          boost::iterator_archetypes:: BOOST_PP_CAT( access, _iterator_t ),
          boost_traversal_tag
      >
{
    typedef typename sake::iterator_private::category<
        boost_traversal_tag,
        constness
    >::type iterator_category;
};

#undef access_constness
#undef access
#undef constness

#undef N2

#endif // #ifndef BOOST_PP_IS_ITERATING

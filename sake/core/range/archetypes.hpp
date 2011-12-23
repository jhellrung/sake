/*******************************************************************************
 * sake/core/range/archetypes.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct archetypes::[traversal]_[access]_range<T>
 * struct archetypes::[traversal]_[access]_static_size_range<T,N>
 *
 * [traversal] in { single_pass, forward, bidirectional, random_access }
 * [access]    in { readable, writable, readable_writable, readable_lvalue,
 *                  writable_lvalue }
 ******************************************************************************/

#ifndef BOOST_PP_IS_ITERATING

#ifndef SAKE_CORE_RANGE_ARCHETYPES_HPP
#define SAKE_CORE_RANGE_ARCHETYPES_HPP

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/seq/cat.hpp>
#include <boost/preprocessor/seq/elem.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/type_traits/integral_constant.hpp>

#include <sake/core/iterator/archetypes.hpp>
#include <sake/core/range/traits/static_size_fwd.hpp>
#include <sake/core/utility/non_copyable.hpp>

namespace sake
{

#define traversals \
    ( single_pass ) \
    ( forward ) \
    ( bidirectional ) \
    ( random_access )
#define accesses \
    ( readable ) \
    ( writable ) \
    ( readable_writable ) \
    ( readable_lvalue ) \
    ( writable_lvalue )

#define BOOST_PP_ITERATION_LIMITS ( 0, BOOST_PP_SEQ_SIZE( traversals ) - 1 )
#define BOOST_PP_FILENAME_1       <sake/core/range/archetypes.hpp>
#include BOOST_PP_ITERATE()

#undef traversals
#undef accesses

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_ARCHETYPES_HPP

#elif BOOST_PP_ITERATION_DEPTH() == 1 // #ifndef BOOST_PP_IS_ITERATING

#define N1 BOOST_PP_FRAME_ITERATION( 1 )

#define traversal BOOST_PP_SEQ_ELEM( N1, traversals )

#define BOOST_PP_ITERATION_LIMITS ( 0, BOOST_PP_SEQ_SIZE( accesses ) - 1 )
#define BOOST_PP_FILENAME_2       <sake/core/range/archetypes.hpp>
#include BOOST_PP_ITERATE()

#undef traversal

#undef N1

#else // #ifndef BOOST_PP_IS_ITERATING

#define N2 BOOST_PP_FRAME_ITERATION( 2 )

#define access BOOST_PP_SEQ_ELEM( N2, accesses )

#define traversal_access                   BOOST_PP_SEQ_CAT( ( traversal ) ( _ ) ( access ) )
#define traversal_access_range             BOOST_PP_CAT( traversal_access, _range )
#define traversal_access_iterator          BOOST_PP_CAT( traversal_access, _iterator )
#define traversal_access_static_size_range BOOST_PP_CAT( traversal_access, _static_size_range )

namespace archetypes
{

template< class T >
struct traversal_access_range
{
    typedef traversal_access_iterator<T> iterator;
    typedef iterator const_iterator;
    iterator begin() const;
    iterator end() const;
    SAKE_NON_COPYABLE( traversal_access_range )
};

template< class T, int N >
struct traversal_access_static_size_range
    : traversal_access_range<T>
{ };

} // namespace archetypes

namespace extension
{

template< class T, int N >
struct range_static_size< archetypes::traversal_access_static_size_range<T,N> >
    : boost::integral_constant< int, N >
{ };

} // namespace extension

#undef traversal_access
#undef traversal_access_range
#undef traversal_access_iterator
#undef traversal_access_static_size_range

#undef access

#undef N2

#endif // #ifndef BOOST_PP_IS_ITERATING

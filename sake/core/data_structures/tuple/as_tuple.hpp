/*******************************************************************************
 * sake/core/data_structures/tuple/as_tuple.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef BOOST_PP_IS_ITERATING

#ifndef SAKE_CORE_DATA_STRUCTURES_TUPLE_AS_TUPLE_HPP
#define SAKE_CORE_DATA_STRUCTURES_TUPLE_AS_TUPLE_HPP

#include <boost/fusion/sequence/intrinsic/size.hpp>
#include <boost/fusion/sequence/intrinsic/value_at.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum.hpp>

#include <sake/boost_ext/move/forward.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>
#include <sake/core/data_structures/tuple/tuple.hpp>

namespace sake
{

namespace result_of
{

namespace as_tuple_private
{

template<
    class Sequence,
    int = boost::fusion::result_of::size< Sequence >::value
>
struct dispatch;

#define result_of_value_at_c_Sequence_n( z, n, data ) \
    typename boost::fusion::result_of::value_at_c< Sequence, n >::type

#define BOOST_PP_ITERATION_LIMITS ( 0, SAKE_TUPLE_MAX_SIZE )
#define BOOST_PP_FILENAME_1       <sake/core/data_structures/as_tuple.hpp>
#include BOOST_PP_ITERATE()

#undef result_of_value_at_c_Sequence_n

} // namespace as_tuple_private

template< class Sequence >
struct as_tuple
    : as_tuple_private::dispatch<
          typename boost_ext::remove_qualifiers< Sequence >::type
      >
{ };

} // namespace result_of

template< class Sequence >
inline typename result_of::as_tuple< SAKE_FWD_PARAM( Sequence ) >::type
as_tuple(SAKE_FWD_REF( Sequence ) s)
{
    typedef typename result_of::as_tuple< SAKE_FWD_PARAM( Sequence ) >::type result_type;
    return result_type(boost_ext::forward< Sequence >(s));
}

} // namespace sake

#endif // #ifndef SAKE_CORE_DATA_STRUCTURES_TUPLE_AS_TUPLE_HPP

#else // #ifndef BOOST_PP_IS_ITERATING

#define N BOOST_PP_ITERATION()

template< class Sequence >
struct dispatch< Sequence, N >
{ typedef sake::tuple< BOOST_PP_ENUM( N, result_of_value_at_c_Sequence_n, ~ ) > type; };

#undef N

#endif // #ifndef BOOST_PP_IS_ITERATING

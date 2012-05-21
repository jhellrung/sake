/*******************************************************************************
 * sake/boost_ext/fusion/convert/as_array.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * boost_ext::fusion::as_array(Sequence const & seq)
 *     -> fusion::result_of::as_array< Sequence >::type
 *
 * boost_ext::fusion::as_array converts a Boost.Fusion sequence into a
 * boost::array with a value_type that all types in the Boost.Fusion sequence
 * are convertible to.  Attempting to convert a Boost.Fusion sequence whose
 * types are not inter-convertible in at least one direction will give a
 * compile-time error.  This implementation assumes that convertibility is
 * transitive.
 ******************************************************************************/

#ifndef BOOST_PP_IS_ITERATING

#ifndef SAKE_BOOST_EXT_FUSION_CONVERT_AS_ARRAY_HPP
#define SAKE_BOOST_EXT_FUSION_CONVERT_AS_ARRAY_HPP

#include <cstddef>

#include <boost/fusion/mpl.hpp>
#include <boost/fusion/sequence/intrinsic/at_c.hpp>
#include <boost/fusion/sequence/intrinsic/size.hpp>
#include <boost/fusion/support/is_sequence.hpp>
#include <boost/mpl/size.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_void.hpp>

#include <sake/boost_ext/array.hpp>
#include <sake/boost_ext/fusion/convert/as_mpl_vector.hpp>
#include <sake/boost_ext/fusion/support/is_sequence_with_category.hpp>
#include <sake/boost_ext/mpl/all.hpp>
#include <sake/boost_ext/mpl/common_result_type.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>

#ifndef SAKE_FUSION_AS_ARRAY_LIMIT
#define SAKE_FUSION_AS_ARRAY_LIMIT 8
#endif // #ifndef SAKE_FUSION_AS_ARRAY_LIMIT

namespace sake
{

namespace boost_ext
{

namespace fusion
{

namespace result_of
{

template< class Sequence >
struct as_array
{
    BOOST_STATIC_ASSERT((boost_ext::fusion::traits::is_random_access_sequence< Sequence >::value));
    typedef typename boost_ext::fusion::result_of::as_mpl_vector< Sequence >::type mpl_vector_type;
    typedef typename boost_ext::mpl::common_result_type< mpl_vector_type >::type value_type;
    BOOST_STATIC_ASSERT(!(boost::is_void< value_type >::value));
    BOOST_STATIC_ASSERT((boost_ext::mpl::all<
        mpl_vector_type,
        boost_ext::is_convertible< boost::mpl::_1, value_type >
    >::value));
    typedef boost::array<
        value_type,
        boost::mpl::size< mpl_vector_type >::value
    > type;
};

template< class T, std::size_t N >
struct as_array< boost::array<T,N> >
{ typedef boost::array<T,N> type; };

} // namespace result_of

namespace as_array_private
{

template<
    class Sequence,
    int = boost::fusion::size< Sequence >::value
>
struct dispatch;

#define at_c_n_s( z, n, data ) boost::fusion::at_c<n>(s)

#define BOOST_PP_ITERATION_LIMITS ( 1, SAKE_FUSION_AS_ARRAY_LIMIT )
#define BOOST_PP_FILENAME_1       <sake/boost_ext/fusion/convert/as_array.hpp>
#include BOOST_PP_ITERATE()

#undef at_c_n_s

} // namespace as_array_private

template< class Sequence >
inline typename boost_ext::fusion::result_of::as_array< Sequence >::type
as_array(Sequence const & s)
{ return as_array_private::dispatch< Sequence >::apply(s); }

template< class T, std::size_t N >
inline boost::array<T,N> const &
as_array(boost::array<T,N> const & a)
{ return a; }

} // namespace fusion

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_FUSION_CONVERT_AS_ARRAY_HPP

#else // #ifndef BOOST_PP_IS_ITERATING

#define N BOOST_PP_ITERATION()

template< class Sequence >
struct dispatch< Sequence, N >
{
    typedef typename boost_ext::fusion::result_of::as_array< Sequence >::type result_type;
    static result_type apply(Sequence const & s)
    {
        result_type a = {{ BOOST_PP_ENUM( N, at_c_n_s, ~ ) }};
        return a;
    }
};

#undef N

#endif // #ifndef BOOST_PP_IS_ITERATING

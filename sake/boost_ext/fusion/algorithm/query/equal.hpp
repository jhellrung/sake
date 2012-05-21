/*******************************************************************************
 * sake/boost_ext/fusion/algorithm/query/equal.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * boost_ext::fusion::equal(Sequence0 const & s0, Sequence1 const & s1)
 *     -> boost_ext::fusion::result_of::equal< Sequence0, Sequence1 >::type
 * boost_ext::fusion::equal(Sequence0 const & s0, Sequence1 const & s1, Equal equal_)
 *     -> boost_ext::fusion::result_of::equal< Sequence0, Sequence1, Equal >::type
 * boost_ext::fusion::equal(I0 const & i0, I1 const & i1)
 *     -> boost_ext::fusion::result_of::equal< I0, I1 >::type
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_FUSION_ALGORITHM_QUERY_EQUAL_HPP
#define SAKE_BOOST_EXT_FUSION_ALGORITHM_QUERY_EQUAL_HPP

#include <boost/fusion/iterator/deref.hpp>
#include <boost/fusion/iterator/equal_to.hpp>
#include <boost/fusion/iterator/next.hpp>
#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/sequence/intrinsic/size.hpp>
#include <boost/fusion/support/is_sequence.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/static_assert.hpp>
#include <boost/utility/result_of.hpp>

#include <sake/boost_ext/type_traits/common_result_type.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/functional/operators/equal.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>

namespace sake
{

namespace boost_ext
{

namespace fusion
{

namespace equal_private
{

template<
    class T0, class T1, class Equal,
    bool = boost::fusion::traits::is_sequence< T0 >::value,
    bool = boost::fusion::traits::is_sequence< T1 >::value
>
struct dispatch0;

template<
    class Sequence0, class Sequence1, class Equal,
    int N0 = boost::fusion::result_of::size< Sequence0 >::value,
    int N1 = boost::fusion::result_of::size< Sequence1 >::value
>
struct dispatch1;

template< int N, class I0, class I1, class Equal >
struct iterate_dispatch;

template< int N, class I0, class I1, class Equal >
inline typename iterate_dispatch< N, I0, I1, Equal >::type
iterate(I0 const & i0, I1 const & i1, Equal const & equal_)
{ return iterate_dispatch< N, I0, I1, Equal >::apply(i0, i1, equal_); }

template< class I0, class I1, class Equal >
struct dispatch0< I0, I1, Equal, false, false >
{
    typedef typename boost::result_of< Equal ( I0, I1 ) >::type type;
    static type apply(I0 const & i0, I1 const & i1, Equal equal_)
    { return equal_(i0, i1); }
};

template< class I0, class I1 >
struct dispatch0< I0, I1, sake::operators::functional::equal, false, false >
{
    typedef boost::mpl::bool_<
        boost::fusion::result_of::equal_to< I0, I1 >::type::value
    > type;
    static type apply(I0 const & /*i0*/, I1 const & /*i1*/, sake::operators::functional::equal)
    { return type(); }
};

template< class Sequence0, class Sequence1, class Equal >
struct dispatch0< Sequence0, Sequence1, Equal, true, true >
    : dispatch1< Sequence0, Sequence1, Equal >
{ };

template< class Sequence0, class Sequence1, class Equal, int N0, int N1 >
struct dispatch1
{
    typedef boost::mpl::false_ type;
    static type apply(Sequence0 const & /*s0*/, Sequence1 const & /*s1*/, Equal const & /*equal_*/)
    {
        BOOST_STATIC_ASSERT((N0 != N1));
        return type();
    }
};

template< class Sequence0, class Sequence1, class Equal, int N >
struct dispatch1< Sequence0, Sequence1, Equal, N, N >
{
    typedef typename iterate_dispatch<
        N,
        typename boost::fusion::result_of::begin< Sequence0 const >::type,
        typename boost::fusion::result_of::begin< Sequence1 const >::type,
        Equal
    >::type type;
    static type apply(Sequence0 const & s0, Sequence1 const & s1, Equal const & equal_)
    { return equal_private::iterate<N>(boost::fusion::begin(s0), boost::fusion::begin(s1), equal_); }
};

template< class I0, class I1, class Equal >
struct iterate_dispatch< 0, I0, I1, Equal >
{
    typedef boost::mpl::true_ type;
    static type apply(I0 const & /*i0*/, I1 const & /*i1*/, Equal const & /*equal_*/)
    { return type(); }
};

template< class I0, class I1, class Equal >
struct iterate_dispatch< 1, I0, I1, Equal >
{
    typedef typename boost::result_of< Equal (
        typename boost::fusion::result_of::deref< I0 >::type,
        typename boost::fusion::result_of::deref< I1 >::type
    ) >::type type;
    static type apply(I0 const & i0, I1 const & i1, Equal equal_)
    { return equal_(*i0, *i1); }
};

template< int N, class I0, class I1, class Equal >
struct iterate_dispatch
{
    typedef typename boost::result_of< Equal (
        typename boost::fusion::result_of::deref< I0 >::type,
        typename boost::fusion::result_of::deref< I1 >::type
    ) >::type curr_type;
    typedef typename iterate_dispatch<
        N-1,
        typename boost::fusion::result_of::next< I0 >::type,
        typename boost::fusion::result_of::next< I1 >::type,
        Equal
    >::type next_type;
    typedef typename boost_ext::common_result_type< curr_type, next_type >::type type;
    static type apply(I0 const & i0, I1 const & i1, Equal equal_)
    {
        curr_type const curr_result = equal_(*i0, *i1);
        return !curr_result ?
               curr_result :
               equal_private::iterate< N-1 >(
                   boost::fusion::next(i0),
                   boost::fusion::next(i1),
                   equal_
               );
    }
};

} // namespace equal_private

namespace result_of
{

template<
    class T0, class T1,
    class Equal = sake::operators::functional::equal
>
struct equal
    : equal_private::dispatch0<
          typename boost_ext::remove_qualifiers< T0 >::type,
          typename boost_ext::remove_qualifiers< T1 >::type,
          typename boost_ext::remove_qualifiers< Equal >::type
      >
{ };

} // namespace result_of

namespace functional
{

struct equal
{
    SAKE_RESULT_FROM_METAFUNCTION( boost_ext::fusion::result_of::equal, (2,3) )

    template< class T0, class T1 >
    typename boost_ext::fusion::result_of::equal< T0, T1 >::type
    operator()(T0 const & x0, T1 const & x1) const
    { return operator()(x0, x1, sake::operators::equal); }

    template< class T0, class T1, class Equal >
    typename boost_ext::fusion::result_of::equal< T0, T1, Equal >::type
    operator()(T0 const & x0, T1 const & x1, Equal const & equal_) const
    { return equal_private::dispatch0< T0, T1, Equal >::apply(x0, x1, equal_); }
};

} // namespace functional

boost_ext::fusion::functional::equal const equal = { };

} // namespace fusion

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_FUSION_ALGORITHM_QUERY_EQUAL_HPP

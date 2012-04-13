/*******************************************************************************
 * sake/boost_ext/fusion/algorithm/query/not_equal.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * boost_ext::fusion::not_equal(Sequence0 const & s0, Sequence1 const & s1)
 *     -> boost_ext::fusion::result_of::not_equal< Sequence0, Sequence1 >::type
 * boost_ext::fusion::not_equal(Sequence0 const & s0, Sequence1 const & s1, NotEqual not_equal_)
 *     -> boost_ext::fusion::result_of::not_equal< Sequence0, Sequence1, NotEqual >::type
 * boost_ext::fusion::not_equal(I0 const & i0, I1 const & i1)
 *     -> boost_ext::fusion::result_of::not_equal< I0, I1 >::type
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_FUSION_ALGORITHM_QUERY_NOT_EQUAL_HPP
#define SAKE_BOOST_EXT_FUSION_ALGORITHM_QUERY_NOT_EQUAL_HPP

#include <boost/fusion/iterator/deref.hpp>
#include <boost/fusion/iterator/equal_to.hpp>
#include <boost/fusion/iterator/next.hpp>
#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/sequence/intrinsic/size.hpp>
#include <boost/fusion/support/is_sequence.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/static_assert.hpp>
#include <boost/utility/result_of.hpp>

#include <sake/boost_ext/type_traits/common_type.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/functional/operators/not_equal.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>

namespace sake
{

namespace boost_ext
{

namespace fusion
{

namespace not_equal_private
{

template<
    class T0, class T1, class NotEqual,
    bool = boost::fusion::traits::is_sequence< T0 >::value,
    bool = boost::fusion::traits::is_sequence< T1 >::value
>
struct dispatch0;

template<
    class Sequence0, class Sequence1, class NotEqual,
    int N0 = boost::fusion::result_of::size< Sequence0 >::value,
    int N1 = boost::fusion::result_of::size< Sequence1 >::value
>
struct dispatch1;

template< int N, class I0, class I1, class NotEqual >
struct iterate_dispatch;

template< int N, class I0, class I1, class NotEqual >
inline typename iterate_dispatch< N, I0, I1, NotEqual >::type
iterate(I0 const & i0, I1 const & i1, NotEqual const & not_equal_)
{ return iterate_dispatch< N, I0, I1, NotEqual >::apply(i0, i1, not_equal_); }

template< class I0, class I1, class NotEqual >
struct dispatch0< I0, I1, NotEqual, false, false >
{
    typedef typename boost::result_of< NotEqual ( I0, I1 ) >::type type;
    static type apply(I0 const & i0, I1 const & i1, NotEqual not_equal_)
    { return not_equal_(i0, i1); }
};

template< class I0, class I1 >
struct dispatch0< I0, I1, sake::operators::functional::not_equal, false, false >
{
    typedef boost::mpl::bool_<
        !boost::fusion::result_of::equal_to< I0, I1 >::type::value
    > type;
    static type apply(I0 const & /*i0*/, I1 const & /*i1*/, sake::operators::functional::not_equal)
    { return type(); }
};

template< class Sequence0, class Sequence1, class NotEqual >
struct dispatch0< Sequence0, Sequence1, NotEqual, true, true >
    : dispatch1< Sequence0, Sequence1, NotEqual >
{ };

template< class Sequence0, class Sequence1, class NotEqual, int N0, int N1 >
struct dispatch1
{
    typedef boost::mpl::true_ type;
    static type apply(Sequence0 const & /*s0*/, Sequence1 const & /*s1*/, NotEqual const & /*not_equal_*/)
    {
        BOOST_STATIC_ASSERT((N0 != N1));
        return type();
    }
};

template< class Sequence0, class Sequence1, class NotEqual, int N >
struct dispatch1< Sequence0, Sequence1, NotEqual, N, N >
{
    typedef typename iterate_dispatch<
        N,
        typename boost::fusion::result_of::begin< Sequence0 const >::type,
        typename boost::fusion::result_of::begin< Sequence1 const >::type,
        NotEqual
    >::type type;
    static type apply(Sequence0 const & s0, Sequence1 const & s1, NotEqual const & not_equal_)
    { return not_equal_private::iterate<N>(boost::fusion::begin(s0), boost::fusion::begin(s1), not_equal_); }
};

template< class I0, class I1, class NotEqual >
struct iterate_dispatch< 0, I0, I1, NotEqual >
{
    typedef boost::mpl::false_ type;
    static type apply(I0 const & /*i0*/, I1 const & /*i1*/, NotEqual const & /*not_equal_*/)
    { return type(); }
};

template< class I0, class I1, class NotEqual >
struct iterate_dispatch< 1, I0, I1, NotEqual >
{
    typedef typename boost::result_of< NotEqual (
        typename boost::fusion::result_of::deref< I0 >::type,
        typename boost::fusion::result_of::deref< I1 >::type
    ) >::type type;
    static type apply(I0 const & i0, I1 const & i1, NotEqual not_equal_)
    { return not_equal_(*i0, *i1); }
};

template< int N, class I0, class I1, class NotEqual >
struct iterate_dispatch
{
    typedef typename boost::result_of< NotEqual (
        typename boost::fusion::result_of::deref< I0 >::type,
        typename boost::fusion::result_of::deref< I1 >::type
    ) >::type curr_type;
    typedef typename iterate_dispatch<
        N-1,
        typename boost::fusion::result_of::next< I0 >::type,
        typename boost::fusion::result_of::next< I1 >::type,
        NotEqual
    >::type next_type;
    typedef typename boost_ext::common_type< curr_type, next_type >::type type;
    static type apply(I0 const & i0, I1 const & i1, NotEqual not_equal_)
    {
        BOOST_STATIC_ASSERT((boost_ext::is_convertible< curr_type, bool >::value));
        curr_type const curr_result = not_equal_(*i0, *i1);
        return curr_result ?
               curr_result :
               not_equal_private::iterate< N-1 >(
                   boost::fusion::next(i0),
                   boost::fusion::next(i1),
                   not_equal_
               );
    }
};

} // namespace not_equal_private

namespace result_of
{

template<
    class T0, class T1,
    class NotEqual = sake::operators::functional::not_equal
>
struct not_equal
    : not_equal_private::dispatch0<
          typename boost_ext::remove_qualifiers< T0 >::type,
          typename boost_ext::remove_qualifiers< T1 >::type,
          typename boost_ext::remove_qualifiers< NotEqual >::type
      >
{ };

} // namespace result_of

namespace functional
{

struct not_equal
{
    SAKE_RESULT_FROM_METAFUNCTION( result_of::not_equal, (2,3) )

    template< class T0, class T1 >
    typename result_of::not_equal< T0, T1 >::type
    operator()(T0 const & x0, T1 const & x1) const
    { return operator()(x0, x1, sake::operators::not_equal); }

    template< class T0, class T1, class NotEqual >
    typename result_of::not_equal< T0, T1, NotEqual >::type
    operator()(T0 const & x0, T1 const & x1, NotEqual const & not_equal_) const
    { return not_equal_private::dispatch0< T0, T1, NotEqual >::apply(x0, x1, not_equal_); }
};

} // namespace functional

functional::not_equal const not_equal = { };

} // namespace fusion

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_FUSION_ALGORITHM_QUERY_NOT_EQUAL_HPP

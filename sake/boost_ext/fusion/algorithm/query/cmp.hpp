/*******************************************************************************
 * sake/boost_ext/fusion/algorithm/query/cmp.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * boost_ext::fusion::cmp(Sequence0 const & s0, Sequence1 const & s1)
 *     -> boost_ext::fusion::result_of::cmp< Sequence0, Sequence1 >::type
 * boost_ext::fusion::cmp(Sequence0 const & s0, Sequence1 const & s1, Cmp cmp_)
 *     -> boost_ext::fusion::result_of::cmp< Sequence0, Sequence1, Cmp >::type
 * boost_ext::fusion::cmp(I0 const & i0, I1 const & i1)
 *     -> boost_ext::fusion::result_of::cmp< I0, I1 >::type
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_FUSION_ALGORITHM_QUERY_CMP_HPP
#define SAKE_BOOST_EXT_FUSION_ALGORITHM_QUERY_CMP_HPP

#include <boost/fusion/iterator/deref.hpp>
#include <boost/fusion/iterator/distance.hpp>
#include <boost/fusion/iterator/next.hpp>
#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/sequence/intrinsic/size.hpp>
#include <boost/mpl/int.hpp>

#include <sake/boost_ext/type_traits/common_result_type.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>
#include <sake/boost_ext/utility/result_of.hpp>

#include <sake/core/config.hpp>
#include <sake/core/math/cmp.hpp>
#include <sake/core/math/sign_t.hpp>
#include <sake/core/math/static_cmp.hpp>
#include <sake/core/math/static_min.hpp>
#include <sake/core/math/static_sign.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>

namespace sake
{

namespace boost_ext
{

namespace fusion
{

namespace cmp_private
{

template<
    class T0, class T1, class Cmp,
    bool = boost::fusion::traits::is_sequence< T0 >::value,
    bool = boost::fusion::traits::is_sequence< T1 >::value
>
struct dispatch;

template< int N, int S, class I0, class I1, class Cmp >
struct iterate_dispatch;

template< int N, int S, class I0, class I1, class Cmp >
inline typename iterate_dispatch< N, S, I0, I1, Cmp >::type
iterate(I0 const & i0, I1 const & i1, Cmp const & cmp_)
{ return iterate_dispatch< N, S, I0, I1, Cmp >::apply(i0, i1, cmp_); }

template< class I0, class I1, class Cmp >
struct dispatch< I0, I1, Cmp, false, false >
{
    typedef typename boost_ext::result_of< Cmp ( I0, I1 ) >::type type;
    static type apply(I0 const & i0, I1 const & i1, Cmp cmp_)
    { return cmp_(i0, i1); }
};

template< class I0, class I1 >
struct dispatch< I0, I1, sake::functional::cmp, false, false >
{
    typedef boost::mpl::int_<
        sake::static_sign_c<
            boost::fusion::result_of::distance< I0, I1 >::type::value
        >::value
    > type;
    static type apply(I0 const & /*i0*/, I1 const & /*i1*/, sake::functional::cmp)
    { return type(); }
};

template< class Sequence0, class Sequence1, class Cmp >
struct dispatch< Sequence0, Sequence1, Cmp, true, true >
{
    static int const N0 = boost::fusion::result_of::size< Sequence0 >::value;
    static int const N1 = boost::fusion::result_of::size< Sequence1 >::value;
    static int const N = sake::static_min_c< N0, N1 >::value;
    static int const S = sake::static_cmp_c< N0, N1 >::value;
    typedef typename cmp_private::iterate_dispatch<
        N, S,
        typename boost::fusion::result_of::begin< const Sequence0 >::type,
        typename boost::fusion::result_of::begin< const Sequence1 >::type,
        Cmp
    >::type type;
    static type apply(Sequence0 const & s0, Sequence1 const & s1, Cmp const & cmp_)
    { return cmp_private::iterate<N,S>(boost::fusion::begin(s0), boost::fusion::begin(s1), cmp_); }
};

template< int S, class I0, class I1, class Cmp >
struct iterate_dispatch< 0, S, I0, I1, Cmp >
{
    typedef sake::sign_t type;
    static type apply(I0 const & /*i0*/, I1 const & /*i1*/, Cmp const &)
    { return type(S); }
};

template< int S, class I0, class I1, class Cmp >
struct iterate_dispatch< 1, S, I0, I1, Cmp >
{
    typedef typename boost_ext::result_of< Cmp (
        typename boost::fusion::result_of::deref< I0 >::type,
        typename boost::fusion::result_of::deref< I1 >::type
    ) >::type type;
    static type apply(I0 const & i0, I1 const & i1, Cmp cmp_)
    {
        type const s = cmp_(*i0, *i1);
        return s == sake::zero ? type(S) : s;
    };
};

template< int N, int S, class I0, class I1, class Cmp >
struct iterate_dispatch
{
    typedef typename boost_ext::result_of< Cmp (
        typename boost::fusion::result_of::deref< I0 >::type,
        typename boost::fusion::result_of::deref< I1 >::type
    ) >::type curr_type;
    typedef typename cmp_private::iterate_dispatch<
        N-1, S,
        typename boost::fusion::result_of::next< I0 >::type,
        typename boost::fusion::result_of::next< I1 >::type,
        Cmp
    >::type next_type;
    typedef typename boost_ext::common_result_type< curr_type, next_type >::type type;
    static type apply(I0 const & i0, I1 const & i1, Cmp cmp_)
    {
        curr_type const s = cmp_(*i0, *i1);
        return s == sake::zero ?
               cmp_private::iterate< N-1, S >(
                   boost::fusion::next(i0),
                   boost::fusion::next(i1),
                   cmp_
               ) : s;
    }
};

} // namespace cmp_private

namespace result_of
{

template<
    class Sequence0, class Sequence1,
    class Cmp = sake::functional::cmp
>
struct cmp
    : cmp_private::dispatch<
          typename boost_ext::remove_qualifiers< Sequence0 >::type,
          typename boost_ext::remove_qualifiers< Sequence1 >::type,
          typename boost_ext::remove_qualifiers< Cmp >::type
      >
{ };

} // namespace result_of

namespace functional
{

struct cmp
{
    SAKE_RESULT_FROM_METAFUNCTION( boost_ext::fusion::result_of::cmp, (2,3) )

    template< class T0, class T1 >
    typename boost_ext::fusion::result_of::cmp< T0, T1 >::type
    operator()(T0 const & x0, T1 const & x1) const
    { return operator()(x0, x1, sake::cmp); }

    template< class T0, class T1, class Cmp >
    typename boost_ext::fusion::result_of::cmp< T0, T1, Cmp >::type
    operator()(T0 const & x0, T1 const & x1, Cmp const & cmp_) const
    { return cmp_private::dispatch< T0, T1, Cmp >::apply(x0, x1, cmp_); }
};

} // namespace functional

#ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
namespace cmp_adl_barrier
{ boost_ext::fusion::functional::cmp const cmp = { }; }
using namespace cmp_adl_barrier;
#else // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
boost_ext::fusion::functional::cmp const cmp = { };
#endif // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS

} // namespace fusion

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_FUSION_ALGORITHM_QUERY_CMP_HPP

/*******************************************************************************
 * sake/core/math/binom.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * binom(N n, K k[, I i]) -> result_of::binom<N,K[,I]>::type
 * binom<K>(N n[, I i]) -> result_of::binom<N,K[,I]>::type
 * binom_c<K>(N n[, I i]) -> result_of::binom_c<N,K[,I]>::type
 * struct functional::binom<>
 * struct functional::binom<K>
 * struct functional::binom_c<K>
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_STATIC_BINOM_HPP
#define SAKE_CORE_MATH_STATIC_BINOM_HPP

#include <boost/mpl/integral_c.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_empty.hpp>
#include <boost/type_traits/is_signed.hpp>
#include <boost/type_traits/make_unsigned.hpp>

#include <sake/boost_ext/mpl/uint.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>
#include <sake/boost_ext/type_traits/remove_rvalue_reference.hpp>

#include <sake/core/functional/operators/divide.hpp>
#include <sake/core/functional/operators/multiply.hpp>
#include <sake/core/introspection/has_isc_value.hpp>
#include <sake/core/math/one.hpp>
#include <sake/core/math/zero.hpp>
#include <sake/core/move/move.hpp>
#include <sake/core/move/rv_cast.hpp>
#include <sake/core/utility/assert.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>

namespace sake
{

namespace binom_private
{

template<
    class N, class K,
    bool = sake::has_isc_value<K>::value
>
struct dispatch;

template< class N, unsigned int K >
struct dispatch_c;

} // namespace binom_private

namespace result_of
{

template< class N, class K, class I = void >
struct binom
    : binom_private::dispatch<
          typename boost_ext::remove_qualifiers<N>::type,
          typename boost_ext::remove_qualifiers<K>::type
      >
{ };

template< class N, unsigned int K, class I = void >
struct binom_c
    : binom_private::dispatch_c<
          typename boost_ext::remove_qualifiers<N>::type,
          K
      >
{ };

} // namespace result_of

namespace functional
{

template< class K = void >
struct binom
{
    template< class > struct result;
    template< class This, class N >
    struct result< This ( N ) >
        : result_of::binom<N,K>
    { };
    template< class This, class N, class I >
    struct result< This ( N, I ) >
        : result_of::binom<N,K>
    { };

    template< class N >
    typename result_of::binom<N,K>::type
    operator()(N n) const
    { return binom_private::dispatch_c< N, K::value >::apply(n); }
    template< class N, class I >
    typename result_of::binom<N,K>::type
    operator()(N n, I i) const
    { return binom_private::dispatch_c< N, K::value >::apply(n,i); }
};

template<>
struct binom< void >
{
    SAKE_RESULT_FROM_METAFUNCTION( result_of::binom, 2 )

    template< class N, class K >
    typename result_of::binom<N,K>::value
    operator()(N n, K k) const
    { return binom_private::dispatch<N,K>::apply(n,k); }
    template< class N, class K, class I >
    typename result_of::binom<N,K>::value
    operator()(N n, K k, I i) const
    { return binom_private::dispatch<N,K>::apply(n,k,i); }
};

template< unsigned int K >
struct binom_c
{
    template< class > struct result;
    template< class This, class N >
    struct result< This ( N ) >
        : result_of::binom_c<N,K>
    { };
    template< class This, class N, class I >
    struct result< This ( N, I ) >
        : result_of::binom_c<N,K>
    { };

    template< class N >
    typename result_of::binom_c<N,K>::type
    operator()(N n) const
    { return binom_private::dispatch_c<N,K>::apply(n); }
    template< class N, class I >
    typename result_of::binom_c<N,K>::type
    operator()(N n, I i) const
    { return binom_private::dispatch_c<N,K>::apply(n,i); }
};

} // namespace functional

template< class N, class K >
inline typename result_of::binom<N,K>::type
binom(N n, K k)
{ return binom_private::dispatch<N,K>::apply(n,k); }
template< class N, class K, class I >
inline typename result_of::binom<N,K>::type
binom(N n, K k, I i)
{ return binom_private::dispatch<N,K>::apply(n,k,i); }

template< class K, class N >
inline typename result_of::binom_c< N, K::value >::type
binom(N n)
{ return binom_private::dispatch_c< N, K::value >::apply(n); }
template< class K, class N, class I >
inline typename result_of::binom_c< N, K::value >::type
binom(N n, I i)
{ return binom_private::dispatch_c< N, K::value >::apply(n,i); }

template< unsigned int K, class N >
inline typename result_of::binom_c<N,K>::type
binom_c(N n)
{ return binom_private::dispatch_c<N,K>::apply(n); }
template< unsigned int K, class N, class I >
inline typename result_of::binom_c<N,K>::type
binom_c(N n, I i)
{ return binom_private::dispatch_c<N,K>::apply(n,i); }

namespace binom_private
{

// The whole purpose of dispatch_0 is to intelligently cast arguments to
// unsigned integral types to avoid signed/unsigned comparison warnings.
template<
    class N, class K,
    bool = boost::is_signed<N>::value,
    bool = boost::is_signed<K>::value
>
struct dispatch_0;

template< class N, class K >
struct dispatch_0< N, K, true, true >
{
    typedef N type;
    typedef typename boost::make_unsigned<N>::type uN;
    typedef typename boost::make_unsigned<K>::type uK;
    static type apply(N const n, K const k)
    {
        SAKE_ASSERT_RELATION_ALL(
            (( n, >=, sake::zero ))
            (( k, >=, sake::zero ))
        );
        uN un = static_cast< uN >(n);
        uK uk = static_cast< uK >(k);
        return static_cast< type >(dispatch_0<uN,uK>::apply(un, uk));
    }
    template< class I >
    static type apply(N const n, K const k, I& i)
    {
        SAKE_ASSERT_RELATION_ALL(
            (( n, >=, sake::zero ))
            (( k, >=, sake::zero ))
        );
        uN un = static_cast< uN >(n);
        uK uk = static_cast< uK >(k);
        return static_cast< type >(dispatch_0<uN,uK>::apply(un, uk, i));
    }
};

template< class N, class K >
struct dispatch_0< N, K, true, false >
{
    typedef N type;
    typedef typename boost::make_unsigned<N>::type uN;
    static type apply(N const n, K& k)
    {
        SAKE_ASSERT_RELATION( n, >=, sake::zero );
        uN un = static_cast< uN >(n);
        return static_cast< type >(dispatch_0<uN,K>::apply(un, k));
    }
    template< class I >
    static type apply(N const n, K& k, I& i)
    {
        SAKE_ASSERT_RELATION( n, >=, sake::zero );
        uN un = static_cast< uN >(n);
        return static_cast< type >(dispatch_0<uN,K>::apply(un, k, i));
    }
};

template< class N, class K >
struct dispatch_0< N, K, false, true >
{
    typedef N type;
    typedef typename boost::make_unsigned<K>::type uK;
    static type apply(N& n, K const k)
    {
        SAKE_ASSERT_RELATION( k, >=, sake::zero );
        uK uk = static_cast< uK >(k);
        return dispatch_0<N,uK>::apply(n, uk);
    }
    template< class I >
    static type apply(N& n, K const k, I& i)
    {
        SAKE_ASSERT_RELATION( k, >=, sake::zero );
        uK uk = static_cast< uK >(k);
        return dispatch_0<N,uK>::apply(n, uk, i);
    }
};

template< class N, class K >
struct dispatch_0< N, K, false, false >
{
    typedef N type;
    static type apply(N& n, K& k)
    {
        N r = sake::one.as<N>();
        return apply(n,k,r);
    }
    template< class I >
    static type apply(N& n, K& k, I& i)
    {
        N r(sake::move(i));
        return apply(n,k,r);
    }
    static type apply(N& n, K& k, N& r)
    {
        SAKE_ASSERT_RELATION_ALL(
            (( n, >=, sake::zero ))
            (( k, >=, sake::zero ))
            (( k, <=, n ))
        );
        if((n >> sake::one) < k) {
            K nmk = n - sake::move(k);
            k = sake::move(nmk);
        }
        n -= k;
        for(unsigned int j = 0; j != k;)
            (r *= ++n) /= ++j;
        return sake::move(r);
    }
};

template< class N, class K >
struct dispatch< N, K, false >
    : dispatch_0<N,K>
{ };

template< class N, class K >
struct dispatch< N, K, true >
{
    BOOST_STATIC_ASSERT((boost::is_empty<K>::value));
    typedef typename dispatch_c< N, K::value >::type type;
    static type apply(N& n, K /*k*/)
    { return dispatch_c< N, K::value >::apply(n); }
    template< class I >
    static type apply(N& n, K /*k*/, I& i)
    { return dispatch_c< N, K::value >::apply(n,i); }
};

template< class X, class N, unsigned int K, unsigned int J >
struct iterate_c;

template< class N >
struct dispatch_c<N,0>
{
    typedef N type;
    static type apply(N& n)
    {
        SAKE_ASSERT_RELATION( n, >=, sake::zero );
        return sake::one.as<N>();
    }
    template< class I >
    static type apply(N& n, I& i)
    {
        SAKE_ASSERT_RELATION( n, >=, sake::zero );
        return N(sake::move(i));
    }
};

template< class N >
struct dispatch_c<N,1>
{
    typedef N type;
    static type apply(N& n)
    {
        SAKE_ASSERT_RELATION( n, >=, sake::zero );
        return sake::move(n);
    }
    template< class I >
    static type apply(N& n, I& /*i*/)
    { return apply(n); }
};

template< class N, unsigned int K >
struct dispatch_c
{
    typedef typename iterate_c<N,N,K,2>::type type;
    static type apply(N& n)
    {
        SAKE_ASSERT_RELATION( n, >=, sake::zero );
        n -= boost_ext::mpl::uint< K-1 >();
        N x = n;
        return iterate_c<N,N,K,2>::apply(x, ++n);
    }
    template< class I >
    static type apply(N& n, I& /*i*/)
    { return apply(n); }
};

template< class X, class N, unsigned int K, unsigned int J >
struct iterate_c
{
    typedef typename boost_ext::remove_rvalue_reference<
        typename operators::result_of::divide<
            typename operators::result_of::multiply< X, N& >::type,
            boost_ext::mpl::uint<J>
        >::type
    >::type X_;
    typedef typename iterate_c<X_,N,K,J+1>::type type;
    static type apply(X& x, N& n)
    {
        X_ x_ = SAKE_RV_CAST(sake::move(x) * n) / boost_ext::mpl::uint<J>();
        return iterate_c<X_,N,K,J+1>::apply(x_, ++n);
    }
};

template< class X, class N, unsigned int K >
struct iterate_c<X,N,K,K>
{
    typedef typename boost_ext::remove_rvalue_reference<
        typename operators::result_of::divide<
            typename operators::result_of::multiply< X, N& >::type,
            boost_ext::mpl::uint<K>
        >::type
    >::type type;
    static type apply(X& x, N& n)
    { return SAKE_RV_CAST(sake::move(x) * n) / boost_ext::mpl::uint<K>(); }
};

} // namespace binom_private

} // namespace sake

#endif // SAKE_CORE_MATH_STATIC_BINOM_HPP

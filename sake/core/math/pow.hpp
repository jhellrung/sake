/*******************************************************************************
 * sake/core/math/pow.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * pow(B b, P p[, I i]) -> result_of::pow<B,P[,I]>::type
 * pow<P>(B b[, I i]) -> result_of::pow<B,P[,I]>::type
 * pow_c<P>(B b[, I i]) -> result_of::pow_c<B,P[,I]>::type
 * struct functional::pow<>
 * struct functional::pow<P>
 * struct functional::pow_c<P>
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_POW_HPP
#define SAKE_CORE_MATH_POW_HPP

#include <boost/config.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_empty.hpp>
#include <boost/type_traits/is_signed.hpp>

#include <sake/boost_ext/type_traits/common_type.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/functional/operators/multiply.hpp>
#include <sake/core/introspection/has_isc_value.hpp>
#include <sake/core/math/inv.hpp>
#include <sake/core/math/inv_ip.hpp>
#include <sake/core/math/neg_ip.hpp>
#include <sake/core/math/one.hpp>
#include <sake/core/math/sqr.hpp>
#include <sake/core/math/sqr_ip.hpp>
#include <sake/core/math/static_intlog2.hpp>
#include <sake/core/math/zero.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/move.hpp>
#include <sake/core/move/rv_cast.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>

namespace sake
{

namespace pow_private
{

template< class B, class P >
struct dispatch_index;

template<
    class B, class P,
    unsigned int = dispatch_index<B,P>::value
>
struct dispatch;

template< class B, int P, bool = (P < 0), bool = (P & 1) >
struct dispatch_c;

} // namespace pow_private

namespace result_of
{

template< class B, class P, class I = void >
struct pow
    : pow_private::dispatch<
          typename boost_ext::remove_qualifiers<B>::type,
          typename boost_ext::remove_qualifiers<P>::type
      >
{ };

template< class B, int P, class I = void >
struct pow_c
    : pow_private::dispatch_c<
          typename boost_ext::remove_qualifiers<B>::type,
          P
      >
{ };

} // namespace result_of

namespace functional
{

template< class P = void >
struct pow
{
    template< class > struct result;
    template< class This, class B >
    struct result< This ( B ) >
        : result_of::pow<B,P>
    { };
    template< class This, class B, class I >
    struct result< This ( B, I ) >
        : result_of::pow<B,P>
    { };

    template< class B >
    typename result_of::pow<B,P>::type
    operator()(B b) const
    { return pow_private::dispatch_c< B, P::value >::apply(b); }
    template< class B, class I >
    typename result_of::pow<B,P>::type
    operator()(B b, I i) const
    { return pow_private::dispatch_c< B, P::value >::apply(b,i); }
};

template<>
struct pow< void >
{
    SAKE_RESULT_FROM_METAFUNCTION( result_of::pow, 2 )

    template< class B, class P >
    typename result_of::pow<B,P>::value
    operator()(B b, P p) const
    { return pow_private::dispatch<B,P>::apply(b,p); }
    template< class B, class P, class I >
    typename result_of::pow<B,P>::value
    operator()(B b, P p, I i) const
    { return pow_private::dispatch<B,P>::apply(b,p,i); }
};

template< int P >
struct pow_c
{
    template< class > struct result;
    template< class This, class B >
    struct result< This ( B ) >
        : result_of::pow_c<B,P>
    { };
    template< class This, class B, class I >
    struct result< This ( B, I ) >
        : result_of::pow_c<B,P>
    { };

    template< class B >
    typename result_of::pow_c<B,P>::type
    operator()(B b) const
    { return pow_private::dispatch_c<B,P>::apply(b); }
    template< class B, class I >
    typename result_of::pow_c<B,P>::type
    operator()(B b, I i) const
    { return pow_private::dispatch_c<B,P>::apply(b,i); }
};

} // namespace functional

template< class B, class P >
inline typename result_of::pow<B,P>::type
pow(B b, P p)
{ return pow_private::dispatch<B,P>::apply(b,p); }
template< class B, class P, class I >
inline typename result_of::pow<B,P>::type
pow(B b, P p, I i)
{ return pow_private::dispatch<B,P>::apply(b,p,i); }

template< class P, class B >
inline typename result_of::pow_c< B, P::value >::type
pow(B b)
{ return pow_private::dispatch_c< B, P::value >::apply(b); }
template< class P, class B, class I >
inline typename result_of::pow_c< B, P::value >::type
pow(B b, I i)
{ return pow_private::dispatch_c< B, P::value >::apply(b,i); }

template< int P, class B >
inline typename result_of::pow_c<B,P>::type
pow_c(B b)
{ return pow_private::dispatch_c<B,P>::apply(b); }
template< int P, class B, class I >
inline typename result_of::pow_c<B,P>::type
pow_c(B b, I i)
{ return pow_private::dispatch_c<B,P>::apply(b,i); }

namespace pow_private
{

using boost_ext::mpl::uint;

template< class B, class P >
struct dispatch_index
{
    static unsigned int const value = boost_ext::mpl::
             if_< sake::has_isc_value<P>, uint<2> >::type::template
        else_if < boost::is_signed<P>, uint<1> >::type::template
        else_   < uint<0> >::type::value;
};

template< class B, class P >
struct dispatch<B,P,2>
{
    BOOST_STATIC_ASSERT((boost::is_empty<P>::value));
    typedef typename dispatch_c< B, P::value >::type type;
    static type apply(B& b, P /*p*/)
    { return dispatch_c< B, P::value >::apply(b); }
    template< class I >
    static type apply(B& b, P /*p*/, I& i)
    { return dispatch_c< B, P::value >::apply(b,i); }
};

template< class B, class P >
struct dispatch<B,P,1>
{
    typedef B type;
    static type apply(B& b, P& p)
    {
        B r = sake::one.as<B>();
        return apply(b,p,r);
    }
    template< class I >
    static type apply(B& b, P& p, I& i)
    {
        bool const inv_ = p < sake::zero;
        if(inv_)
            sake::neg_ip(p);
        b = dispatch<B,P,0>::apply(b,p,i);
        return inv_ ? sake::inv(sake::move(b)) : sake::move(b);
    }
};

template< class B, class P >
struct dispatch<B,P,0>
{
    typedef B type;
    static type apply(B& b, P& p)
    {
        B r = sake::one.as<B>();
        return apply(b,p,r);
    }
    template< class I >
    static type apply(B& b, P& p, I& i)
    {
        B r(sake::move(i));
        return apply(b,p,r);
    }
    static type apply(B& b, P& p, B& r)
    {
        for(; sake::zero < p; sake::sqr_ip(b), p >>= sake::one)
            if(p & sake::one)
                r *= b;
        return sake::move(r);
    }
};

template< int P, class B >
struct dispatch_c< B, P, false, false >
{
    typedef typename result_of::sqr<B>::type sqr_type;
    typedef typename dispatch_c< sqr_type, P/2 >::type type;
    static type apply(B& b)
    {
        sqr_type b2 = sake::sqr(sake::move(b));
        return dispatch_c< sqr_type, P/2 >::apply(b2);
    }
    template< class I >
    static type apply(B& b, I& i)
    {
        sqr_type b2 = sake::sqr(sake::move(b));
        return dispatch_c< sqr_type, P/2 >::apply(b2,i);
    }
};

template< int P, class B >
struct dispatch_c< B, P, false, true >
{
    typedef typename result_of::sqr< B& >::type sqr_type;
    typedef typename operators::result_of::multiply<
        B, typename dispatch_c< sqr_type, P/2 >::type
    >::type type;
    static type apply(B& b)
    {
        sqr_type b2 = sake::sqr(b);
        return sake::move(b) 
             * SAKE_RV_CAST((dispatch_c< sqr_type, P/2 >::apply(b2)));
    }
    template< class I >
    static type apply(B& b, I& i)
    {
        sqr_type b2 = sake::sqr(b);
        return sake::move(b)
             * SAKE_RV_CAST((dispatch_c< sqr_type, P/2 >::apply(b2,i)));
    }
};

template< int P, class B, bool _ >
struct dispatch_c< B, P, true, _ >
{
    typedef typename result_of::inv< typename dispatch_c<B,-P>::type >::type type;
    static type apply(B& b)
    { return sake::inv(SAKE_RV_CAST((dispatch_c<B,-P>::apply(b)))); }
    template< class I >
    static type apply(B& b, I& i)
    { return sake::inv(SAKE_RV_CAST((dispatch_c<B,-P>::apply(b,i)))); }
};

template< class B >
struct dispatch_c< B, 3, false, true >
{
    typedef typename result_of::sqr< B& >::type sqr_type;
    typedef typename operators::result_of::multiply< B, sqr_type >::type type;
    static type apply(B& b)
    {
        sqr_type b2 = sake::sqr(b);
        return sake::move(b) * sake::move(b2);
    }
    template< class I >
    static type apply(B& b, I& /*i*/)
    { return apply(b); }
};

template< class B >
struct dispatch_c< B, 2, false, false >
{
    typedef typename result_of::sqr<B>::type type;
    static type apply(B& b)
    { return sake::sqr(sake::move(b)); }
    template< class I >
    static type apply(B& b, I& /*i*/)
    { return apply(b); }
};

template< class B >
struct dispatch_c< B, 1, false, true >
{
    typedef B type;
    static type apply(B& b)
    { return sake::move(b); }
    template< class I >
    static type apply(B& b, I& /*i*/)
    { return apply(b); }
};

template< class B >
struct dispatch_c< B, 0, false, false >
{
    typedef B type;
    static type apply(B& /*b*/)
    { return sake::one.as<B>(); }
    template< class I >
    static type apply(B& /*b*/, I& i)
    { return B(sake::move(i)); }
};

} // namespace pow_private

} // namespace sake

#endif // SAKE_CORE_MATH_POW_HPP

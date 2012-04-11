/*******************************************************************************
 * sake/core/math/intlog2.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * intlog2(T const & x) -> result_of::intlog2<T>::type
 * intlog2_nothrow(T const & x) -> result_of::intlog2<T>::type
 *
 * struct result_of::intlog2<T>
 * struct result_of::extension::intlog2< T, Enable = void >
 * struct result_of::default_impl::intlog2<T>
 *
 * Returns the floored base-2 logarithm of x.  If x is nonpositive, intlog2
 * throws an exception.
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_INTLOG2_HPP
#define SAKE_CORE_MATH_INTLOG2_HPP

#include <cstdlib>

#include <iostream>
#include <limits>

#include <boost/config.hpp>
#include <boost/exception/info.hpp>
#include <boost/integer.hpp>
#include <boost/integer_traits.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/static_assert.hpp>
#include <boost/throw_exception.hpp>
#include <boost/type_traits/is_floating_point.hpp>
#include <boost/type_traits/is_signed.hpp>
#include <boost/type_traits/is_unsigned.hpp>
#include <boost/type_traits/make_unsigned.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/exception/define_exception.hpp>
#include <sake/boost_ext/mpl/if.hpp>
#include <sake/boost_ext/mpl/uint.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/expr_traits/best_conversion.hpp>
#include <sake/core/introspection/is_callable_function.hpp>
#include <sake/core/introspection/is_callable_member_function.hpp>
#include <sake/core/math/iec559_traits.hpp>
#include <sake/core/math/intlog2_fwd.hpp>
#include <sake/core/math/zero.hpp>
#include <sake/core/utility/assert.hpp>
#include <sake/core/utility/debug.hpp>
#include <sake/core/utility/declval.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>
#include <sake/core/utility/workaround.hpp>

#ifdef _MSC_VER
#include <intrin.h>
#endif // #ifdef _MSC_VER

namespace sake
{

SAKE_BOOST_EXT_DEFINE_EXCEPTION( sake, intlog2_error )
namespace intlog2_errinfo
{
namespace tag
{
template< class T > struct arg;
} // namespace tag
template< class T >
struct arg
{ typedef boost::error_info< tag::arg<T>, T > type; };
} // namespace intlog2_errinfo

namespace intlog2_private
{

template< class T >
struct dispatch_index;

template<
    class T,
    class Result = typename result_of::intlog2<T>::type,
    unsigned int = dispatch_index<T>::value
>
struct dispatch;

} // namespace intlog2_private

namespace result_of
{

/*******************************************************************************
 * struct result_of::intlog2<T>
 ******************************************************************************/

template< class T >
struct intlog2
    : extension::intlog2<
          typename boost_ext::remove_qualifiers<T>::type
      >
{ };

/*******************************************************************************
 * struct result_of::extension::intlog2< T, Enable = void >
 ******************************************************************************/

namespace extension
{

template< class T, class Enable /*= void*/ >
struct intlog2
    : default_impl::intlog2<T>
{ };

} // namespace extension

/*******************************************************************************
 * struct result_of::default_impl::intlog2<T>
 ******************************************************************************/

namespace default_impl
{

typedef boost::mpl::vector2< int, unsigned int > intlog2_result_types;

template< class T >
struct intlog2
    : intlog2_private::dispatch<
          typename boost_ext::remove_qualifiers<T>::type,
          void
      >
{ };

} // namespace default_impl

} // namespace result_of

/*******************************************************************************
 * intlog2(T const & x) -> result_of::intlog2<T>::type
 * intlog2_nothrow(T const & x) -> result_of::intlog2<T>::type
 * struct functional::intlog2
 * struct functional::intlog2_nothrow
 ******************************************************************************/

namespace functional
{

struct intlog2
{
    SAKE_RESULT_FROM_METAFUNCTION( result_of::intlog2, 1 )

    template< class T >
    typename result_of::intlog2<T>::type
    operator()(T const & x) const
    { return intlog2_private::dispatch<T>::apply(x); }
};

struct intlog2_nothrow
{
    SAKE_RESULT_FROM_METAFUNCTION( result_of::intlog2, 1 )

    template< class T >
    typename result_of::intlog2<T>::type
    operator()(T const & x) const
    { return intlog2_private::dispatch<T>::apply_nothrow(x); }
};

} // namespace functional

#ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
namespace intlog2_adl_barrier
{ functional::intlog2 const intlog2 = { };
  functional::intlog2_nothrow const intlog2_nothrow = { }; }
using namespace intlog2_adl_barrier;
#else // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
functional::intlog2 const intlog2 = { };
functional::intlog2_nothrow const intlog2_nothrow = { };
#endif // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS

} // namespace sake

namespace sake_intlog2_private
{

#define SAKE_INTROSPECTION_TRAIT_NAME    is_callable
#define SAKE_INTROSPECTION_FUNCTION_NAME intlog2_nothrow
#define SAKE_INTROSPECTION_FUNCTION_ARITY_LIMITS ( 1, 1 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_FUNCTION()

template< class T, class Result >
struct adl
{
    static Result apply(T const & x)
    { return intlog2_nothrow(x); }
};

template< class T >
struct adl< T, void >
{
    SAKE_EXPR_BEST_CONVERSION_TYPEDEF(
        typename intlog2_nothrow(::sake::declcref<T>()),
        ::sake::result_of::default_impl::intlog2_result_types,
        type
    );
};

} // namespace sake_intlog2_private

namespace sake
{

namespace intlog2_private
{

#define SAKE_INTROSPECTION_TRAIT_NAME           is_callable_mem_fun
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME intlog2_nothrow
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY_LIMITS ( 0, 0 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_MEMBER_FUNCTION()

#if defined( _MSC_VER )

typedef unsigned long ulong;

inline unsigned int
uint_impl(ulong const x)
{
    ulong result;
    _BitScanReverse(&result, x);
    return static_cast< unsigned int >(result);
}

int const ulong_bits = std::numeric_limits< ulong >::digits;

#if defined( _M_IA64 ) || defined( _M_X64 ) || defined( WIN64 )

inline unsigned int
uint64_impl(unsigned __int64 const x)
{
    ulong result;
    _BitScanReverse64(&result, x);
    return static_cast< unsigned int >(result);
}

template< class T >
inline typename boost::enable_if_c<
    (std::numeric_limits<T>::digits > ulong_bits),
    unsigned int
>::type
uint_impl(T const x)
{
    BOOST_STATIC_ASSERT((std::numeric_limits<T>::digits <= std::numeric_limits< unsigned __int64 >::digits));
    return uint64_impl(static_cast< unsigned __int64 >(x));
}

#else // #if defined( _M_IA64 ) || defined( _M_X64 ) || defined( WIN64 )

ulong const ulong_mask =
    (static_cast< ulong >(1) << (ulong_bits - 1))
  | ((static_cast< ulong >(1) << (ulong_bits - 1)) - 1);

template< class T, int = 1 + (std::numeric_limits<T>::digits - 1) / ulong_bits >
struct uint_dispatch;

template< class T >
struct uint_dispatch<T,2>
{
    static unsigned int
    apply(T const x)
    {
        if(ulong const y = static_cast< ulong >((x >> ulong_bits) & ulong_mask))
            return uint_impl(y) + ulong_bits;
        ulong const y = static_cast< ulong >(x & ulong_mask);
        return uint_impl(y);
    }
};

template< class T >
inline typename boost::enable_if_c<
    (std::numeric_limits<T>::digits > ulong_bits),
    unsigned int
>::type
uint_impl(const T& x)
{ return uint_dispatch<T>::apply(x); }

#endif // #if defined( _M_IA64 ) || defined( _M_X64 ) || defined( WIN64 )

#elif defined( __GNUC__ ) // #if defined( ... )

inline unsigned int
uint_impl(unsigned int const x)
{ return static_cast< unsigned int >((std::numeric_limits< unsigned int >::digits - 1) - __builtin_clz(x)); }
inline unsigned int
uint_impl(unsigned long const x)
{ return static_cast< unsigned int >((std::numeric_limits< unsigned long >::digits - 1) - __builtin_clzl(x)); }
#ifndef BOOST_NO_LONG_LONG
inline unsigned int
uint_impl(unsigned long long const x)
{ return static_cast< unsigned int >((std::numeric_limits< unsigned long long >::digits - 1) - __builtin_clzll(x)); }
#endif // #ifndef BOOST_NO_LONG_LONG

#else // #if defined( ... )

template< int Bits >
struct uint_dispatch
{
    BOOST_STATIC_ASSERT((Bits > 1));
    template< class T >
    static unsigned int
    apply(T const x)
    {
        SAKE_ASSERT_RELATION_ALL( (( 1, <=, x )) (( x/2, <=, (static_cast<T>(1) << (Bits - 1)) - 1 )) );
        return x < (static_cast<T>(1) << Bits/2) ?
               uint_dispatch< Bits/2 >::apply(x) :
               Bits/2 + uint_dispatch< (Bits+1)/2 >::apply(x >> (Bits/2));
    }
};

template<>
struct uint_dispatch<4>
{
    template< class T >
    static unsigned int
    apply(T const x)
    {
        SAKE_ASSERT_RELATION_ALL( (( 1, <=, x )) (( x, <=, 15 )) );
        static unsigned int const _[] = { 0, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3 };
        return _[x];
    }
};

template<>
struct uint_dispatch<3>
{
    template< class T >
    static unsigned int
    apply(T const x)
    {
        SAKE_ASSERT_RELATION_ALL( (( 1, <=, x )) (( x, <=, 7 )) );
        static unsigned int const _[] = { 0, 0, 1, 1, 2, 2, 2, 2 };
        return _[x];
    }
};

template<>
struct uint_dispatch<2>
{
    template< class T >
    static unsigned int
    apply(T const x)
    {
        SAKE_ASSERT_RELATION_ALL( (( 1, <=, x )) (( x, <=, 3 )) );
        static unsigned int const _[] = { 0, 0, 1, 1 };
        return _[x];
    }
};

template<>
struct uint_dispatch<1>
{
    template< class T >
    static unsigned int
    apply(T const x)
    {
        SAKE_ASSERT_RELATION( x, ==, 1 );
        return 0;
    }
};

template< class T >
inline unsigned int
uint_impl(T const x)
{
    static const int digits = std::numeric_limits<T>::digits;
    BOOST_STATIC_ASSERT((digits - 1 <= boost::integer_traits< unsigned int >::const_max));
    return uint_dispatch< digits >::apply(x);
}

#endif // #if defined( ... )

template< class T >
inline int
float_impl(T const x)
{
    typedef typename boost::uint_t< sake::iec559_traits<T>::bits >::fast uint;
    union { T x; uint i; } u = { x };
    int const biased_exponent = static_cast< int >(u.i >> iec559_traits<T>::significand_bits);
    return biased_exponent != 0 ?
           biased_exponent - sake::iec559_traits<T>::exponent_bias :
           static_cast< int >(intlog2_private::uint_impl(u.i))
               - (sake::iec559_traits<T>::exponent_bias + iec559_traits<T>::significand_bits - 1);
}

#ifdef SAKE_DEBUG

template< class T >
inline int
float_check()
{
    if(intlog2_private::float_impl(static_cast<T>(1.0)) !=  0
    || intlog2_private::float_impl(static_cast<T>(2.0)) != +1
    || intlog2_private::float_impl(static_cast<T>(0.5)) != -1) {
        std::cerr << "ERROR: sake::intlog2( " << typeid( T ).name() << " ) is "
                     "not implemented correctly on this platform.\n"
                     "       A correct implementation should be provided in "
                     "<sake/core/math/intlog2.hpp>."
                  << std::endl;
        std::abort();
    }
    return 0;
}

#endif // #ifdef SAKE_DEBUG

using boost_ext::mpl::uint;

template< class T >
struct dispatch_index
{
    static unsigned int const value = boost_ext::mpl::
             if_< boost::is_unsigned<T>, uint<5> >::type::template
        else_if < boost::is_signed<T>, uint<4> >::type::template
        else_if < boost::is_floating_point<T>, uint<3> >::type::template
        else_if < is_callable_mem_fun< T const & >, uint<2> >::type::template
        else_if < ::sake_intlog2_private::is_callable< void ( T const & ) >, uint<1> >::type::template
        else_   < uint<0> >::type::value;
};

template< class T, class Result >
struct dispatch< T, Result, 5 >
{
    typedef unsigned int type;
    static type apply(T const x)
    {
        if(x == 0)
            boost::throw_exception(intlog2_error()
                << typename intlog2_errinfo::arg<T>::type(x));
        return apply_nothrow(x);
    }
    static type apply_nothrow(T const x)
    {
        SAKE_ASSERT_RELATION( x, !=, 0 );
        return intlog2_private::uint_impl(x);
    }
};

template< class T, class Result >
struct dispatch< T, Result, 4 >
{
    typedef unsigned int type;
    static type apply(T const x)
    {
        if(x <= 0)
            boost::throw_exception(intlog2_error()
                << typename intlog2_errinfo::arg<T>::type(x));
        return apply_nothrow(x);
    }
    static type apply_nothrow(T const x)
    {
        typedef typename boost::make_unsigned<T>::type uint_type;
        SAKE_ASSERT_RELATION( x, >, 0 );
        return dispatch< uint_type, Result >::apply_nothrow(x);
    }
};

template< class T, class Result >
struct dispatch< T, Result, 3 >
{
    BOOST_STATIC_ASSERT((std::numeric_limits<T>::is_iec559));
    BOOST_STATIC_ASSERT((std::numeric_limits<T>::radix == 2));
    typedef int type;
    static type apply(T const x)
    {
        if(x <= 0 || x == std::numeric_limits<T>::infinity() || x != x)
            boost::throw_exception(intlog2_error()
                << typename intlog2_errinfo::arg<T>::type(x));
        return apply_nothrow(x);
    }
    static type apply_nothrow(T const x)
    {
#ifdef SAKE_DEBUG
        static int const _float_check = intlog2_private::float_check<T>();
        static_cast< void >(_float_check);
#endif // #ifdef SAKE_DEBUG
        SAKE_ASSERT_RELATION_ALL(
            (( 0, <, x ))
            (( x, <, std::numeric_limits<T>::infinity() ))
            (( x, ==, x ))
        );
        return intlog2_private::float_impl(x);
    }
};

template< class T >
struct dispatch< T, void, 2 >
{
    SAKE_EXPR_BEST_CONVERSION_TYPEDEF(
        typename sake::declcref<T>().intlog2_nothrow(),
        result_of::default_impl::intlog2_result_types,
        type
    );
};

template< class T, class Result >
struct dispatch< T, Result, 2 >
{
    static Result apply(T const & x)
    {
        if(x <= sake::zero)
            boost::throw_exception(intlog2_error()
                << typename intlog2_errinfo::arg<T>::type(x));
        return x.intlog2_nothrow();
    }
    static Result apply_nothrow(T const & x)
    {
        SAKE_ASSERT_RELATION( x, >, sake::zero );
        return x.intlog2_nothrow();
    }
};

template< class T, class Result >
struct dispatch< T, Result, 1 >
    : ::sake_intlog2_private::adl< T, Result >
{ };

template< class T, class Result >
struct dispatch< T, Result, 0 >
{ typedef void type; };

} // namespace intlog2_private

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_INTLOG2_HPP

/*******************************************************************************
 * sake/core/data_structures/pair.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct pair< T0, T1 >
 * make_pair(T0&& first, T1&& second) -> result_of::make_pair< T0, T1 >::type
 *
 * swap(pair< T0, T1 >& x, pair< T0, T1 >& y) -> void
 * hash_value(pair< T0, T1 > const & x) -> std::size_t
 *
 * operator==(pair< T0, T1 > const & x, pair< U0, U1 > const & y) -> ...
 * operator!=(pair< T0, T1 > const & x, pair< U0, U1 > const & y) -> ...
 * operator<(pair< T0, T1 > const & x, pair< U0, U1 > const & y) -> ...
 * compare(pair< T0, T1 > const & x, pair< U0, U1 > const & y) -> ...
 *
 * A sake::pair is nearly functionally identical to a std::pair, except that it
 * handles reference types and is emplacer- and move-aware.
 ******************************************************************************/

#ifndef SAKE_CORE_DATA_STRUCTURES_PAIR_HPP
#define SAKE_CORE_DATA_STRUCTURES_PAIR_HPP

#include <cstddef>

#include <boost/functional/hash.hpp>
#include <boost/fusion/adapted/std_pair.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/preprocessor/seq/seq.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/fusion/adapted/pair.hpp>
#include <sake/boost_ext/fusion/sequence/intrinsic/at.hpp>
#include <sake/boost_ext/type_traits/common_type.hpp>
#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/add_reference_add_const.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/data_structures/pair_fwd.hpp>
#include <sake/core/functional/operators/logical_and.hpp>
#include <sake/core/functional/operators/logical_or.hpp>
#include <sake/core/functional/operators/equal.hpp>
#include <sake/core/functional/operators/less.hpp>
#include <sake/core/functional/operators/not_equal.hpp>
#include <sake/core/math/compare.hpp>
#include <sake/core/math/zero.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/movable.hpp>
#include <sake/core/ref/wrapped_parameter_to_reference.hpp>
#include <sake/core/utility/emplacer.hpp>
#include <sake/core/utility/implicitly_defined/mem_fun.hpp>
#include <sake/core/utility/private/is_compatible_sequence.hpp>
#include <sake/core/utility/swap.hpp>

namespace sake
{

/*******************************************************************************
 * struct pair< T0, T1 >
 ******************************************************************************/\

namespace pair_adl
{

template< class T0, class T1 >
struct pair
{
    typedef T0 first_type;
    typedef T1 second_type;
    typedef boost::mpl::vector2< T0, T1 > values_type;

    first_type first;
    second_type second;

    SAKE_OPTIMAL_MOVABLE_COPYABLE_IF_MOVABLE( typename pair, ( T0 ) ( T1 ) )

    SAKE_IMPLICITLY_DEFINED_MEM_FUN(
        typename pair,
        ( default_ctor ) ( move_ctor ) ( copy_assign ) ( move_assign ),
        BOOST_PP_SEQ_NIL, ( first ) ( second )
    )

    pair(sake::emplacer< void ( ) >, sake::emplacer< void ( ) >)
    { }

private:
    typedef typename boost::remove_cv< T0 >::type T0_;
    typedef typename boost::remove_cv< T1 >::type T1_;
public:

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U0 >
    pair(U0&& x0, sake::emplacer< void ( ) >)
        : first (sake::emplacer_construct< T0_ >(sake::forward< U0 >(x0)))
    { }

    template< class U1 >
    pair(sake::emplacer< void ( ) >, U1&& x1)
        : second(sake::emplacer_construct< T1_ >(sake::forward< U1 >(x1)))
    { }

    template< class U0, class U1 >
    pair(U0&& x0, U1&& x1)
        : first (sake::emplacer_construct< T0_ >(sake::forward< U0 >(x0))),
          second(sake::emplacer_construct< T1_ >(sake::forward< U1 >(x1)))
    { }

    template< class Sequence >
    pair(Sequence&& s,
        typename boost::enable_if_c< utility_private::is_compatible_sequence<
            pair, Sequence >::value >::type* = 0)
        : first (boost_ext::fusion::at_c<0>(sake::forward< Sequence >(s))),
          second(boost_ext::fusion::at_c<1>(sake::forward< Sequence >(s)))
    { }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U0 >
    pair(U0& x0, sake::emplacer< void ( ) >)
        : first (sake::emplacer_construct< T0_ >(x0))
    { }

    template< class U0 >
    pair(U0 const & x0, sake::emplacer< void ( ) >)
        : first (sake::emplacer_construct< T0_ >(x0))
    { }

    template< class U1 >
    pair(sake::emplacer< void ( ) >, U1& x1)
        : second(sake::emplacer_construct< T1_ >(x1))
    { }

    template< class U1 >
    pair(sake::emplacer< void ( ) >, U1 const & x1)
        : second(sake::emplacer_construct< T1_ >(x1))
    { }

    template< class U0, class U1 >
    pair(U0& x0, U1& x1)
        : first (sake::emplacer_construct< T0_ >(x0)),
          second(sake::emplacer_construct< T1_ >(x1))
    { }

    template< class U0, class U1 >
    pair(U0& x0, U1 const & x1)
        : first (sake::emplacer_construct< T0_ >(x0)),
          second(sake::emplacer_construct< T1_ >(x1))
    { }

    template< class U0, class U1 >
    pair(U0 const & x0, U1& x1)
        : first (sake::emplacer_construct< T0_ >(x0)),
          second(sake::emplacer_construct< T1_ >(x1))
    { }

    template< class U0, class U1 >
    pair(U0 const & x0, U1 const & x1)
        : first (sake::emplacer_construct< T0_ >(x0)),
          second(sake::emplacer_construct< T1_ >(x1))
    { }

    template< class Sequence >
    pair(Sequence& s,
        typename boost::enable_if_c< utility_private::is_compatible_sequence<
            pair, Sequence& >::value >::type* = 0)
        : first (boost_ext::fusion::at_c<0>(s)),
          second(boost_ext::fusion::at_c<1>(s))
    { }

    template< class Sequence >
    pair(Sequence const & s,
        typename boost::enable_if_c< utility_private::is_compatible_sequence<
            pair, Sequence const & >::value >::type* = 0)
        : first (boost_ext::fusion::at_c<0>(s)),
          second(boost_ext::fusion::at_c<1>(s))
    { }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U0, class U1 >
    pair&
    assign(SAKE_FWD_REF( U0 ) x0, SAKE_FWD_REF( U1 ) x1)
    {
        sake::emplacer_assign(first,  sake::forward< U0 >(x0));
        sake::emplacer_assign(second, sake::forward< U1 >(x1));
        return *this;
    }

    template< class Sequence >
    typename boost::enable_if_c<
        utility_private::is_compatible_sequence<
            pair, SAKE_FWD_PARAM( Sequence ) >::value,
        pair&
    >::type
    operator=(SAKE_FWD_REF( Sequence ) s)
    {
        first  = boost_ext::fusion::at_c<0>(sake::forward< Sequence >(s));
        second = boost_ext::fusion::at_c<1>(sake::forward< Sequence >(s));
        return *this;
    }

    void swap(pair& other)
    {
        sake::swap(first,  other.first);
        sake::swap(second, other.second);
    }

    std::size_t
    hash_value() const
    {
        std::size_t x = 0;
        boost::hash_combine(x, first);
        boost::hash_combine(x, second);
        return x;
    }
};

} // namespace tuple_adl

/*******************************************************************************
 * make_pair(T0&& x0, T1&& x1) -> result_of::make_pair< T0, T1 >::type
 ******************************************************************************/

namespace result_of
{

template< class T0, class T1 >
struct make_pair
{
    typedef sake::pair<
        typename sake::wrapped_parameter_to_reference<
            typename boost_ext::remove_qualifiers< T0 >::type
        >::type,
        typename sake::wrapped_parameter_to_reference<
            typename boost_ext::remove_qualifiers< T1 >::type
        >::type
    > type;
};

} // namespace result_of

template< class T0, class T1 >
inline typename result_of::make_pair< T0, T1 >::type
make_pair(SAKE_FWD_REF( T0 ) x0, SAKE_FWD_REF( T1 ) x1)
{
    typedef typename result_of::make_pair< T0, T1 >::type result_type;
    return result_type(sake::forward< T0 >(x0), sake::forward< T1 >(x1));
}

namespace tuple_adl
{

/*******************************************************************************
 * swap(pair< T0, T1 >& x, pair< T0, T1 >& y) -> void
 * hash_value(pair< T0, T1 > const & x) -> std::size_t
 ******************************************************************************/

template< class T0, class T1 >
inline void
swap(pair< T0, T1 >& x, pair< T0, T1 >& y)
{ x.swap(y); }

template< class T0, class T1 >
inline std::size_t
hash_value(pair< T0, T1 > const & x)
{ return x.hash_value(); }

/*******************************************************************************
 * operator==(pair< T0, T1 > const & x, pair< U0, U1 > const & y) -> ...
 * operator!=(pair< T0, T1 > const & x, pair< U0, U1 > const & y) -> ...
 * operator<(pair< T0, T1 > const & x, pair< U0, U1 > const & y) -> ...
 * compare(pair< T0, T1 > const & x, pair< U0, U1 > const & y) -> ...
 ******************************************************************************/

template< class T0, class T1, class U0, class U1 >
inline typename operators::result_of::and_<
    typename operators::result_of::equal<
        typename boost_ext::add_reference_add_const< T0 >::type,
        typename boost_ext::add_reference_add_const< U0 >::type
    >::type,
    typename operators::result_of::equal<
        typename boost_ext::add_reference_add_const< T1 >::type,
        typename boost_ext::add_reference_add_const< U1 >::type
    >::type
>::type
operator==(sake::pair< T0, T1 > const & x, sake::pair< U0, U1 > const & y)
{ return x.first == y.first && x.second == y.second; }

template< class T0, class T1, class U0, class U1 >
inline typename operators::result_of::or_<
    typename operators::result_of::not_equal<
        typename boost_ext::add_reference_add_const< T0 >::type,
        typename boost_ext::add_reference_add_const< U0 >::type
    >::type,
    typename operators::result_of::not_equal<
        typename boost_ext::add_reference_add_const< T1 >::type,
        typename boost_ext::add_reference_add_const< U1 >::type
    >::type
>::type
operator!=(sake::pair< T0, T1 > const & x, sake::pair< U0, U1 > const & y)
{ return x.first != y.first || x.second != y.second; }

template< class T0, class T1, class U0, class U1 >
inline typename boost_ext::common_type<
    typename operators::result_of::less<
        typename result_of::compare<
            typename boost_ext::add_reference_add_const< T0 >::type,
            typename boost_ext::add_reference_add_const< U0 >::type
        >::type,
        sake::zero_t
    >::type,
    typename operators::result_of::less<
        typename boost_ext::add_reference_add_const< T1 >::type,
        typename boost_ext::add_reference_add_const< U1 >::type
    >::type
>::type
operator<(sake::pair< T0, T1 > const & x, sake::pair< U0, U1 > const & y)
{
    typename result_of::compare<
        typename boost_ext::add_reference_add_const< T0 >::type,
        typename boost_ext::add_reference_add_const< U0 >::type
    >::type const s0 = sake::compare(x.first, y.first);
    return s0 == sake::zero ? x.second < y.second : s0 < sake::zero;
}

template< class T0, class T1, class U0, class U1 >
inline typename boost_ext::common_type<
    typename result_of::compare<
        typename boost_ext::add_reference_add_const< T0 >::type,
        typename boost_ext::add_reference_add_const< U0 >::type
    >::type,
    typename result_of::compare<
        typename boost_ext::add_reference_add_const< T1 >::type,
        typename boost_ext::add_reference_add_const< U1 >::type
    >::type
>::type
compare(sake::pair< T0, T1 > const & x, sake::pair< U0, U1 > const & y)
{
    typename result_of::compare<
        typename boost_ext::add_reference_add_const< T0 >::type,
        typename boost_ext::add_reference_add_const< U0 >::type
    >::type const s0 = sake::compare(x.first, y.first);
    return s0 == sake::zero ? sake::compare(x.second, y.second) : s0;
}

} // namespace tuple_adl

} // namespace sake

#endif // #ifndef SAKE_CORE_DATA_STRUCTURES_PAIR_HPP

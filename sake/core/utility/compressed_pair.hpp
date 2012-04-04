/*******************************************************************************
 * sake/core/utility/compressed_pair.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct compressed_pair< T0, T1 >
 *
 * swap(compressed_pair< T0, T1 >& x, compressed_pair< T0, T1 >& y) -> void
 * hash_value(compressed_pair< T0, T1 > const & x) -> std::size_t
 *
 * operator==(compressed_pair< T0, T1 > const & x, compressed_pair< U0, U1 > const & y) -> ...
 * operator!=(compressed_pair< T0, T1 > const & x, compressed_pair< U0, U1 > const & y) -> ...
 * operator<(compressed_pair< T0, T1 > const & x, compressed_pair< U0, U1 > const & y) -> ...
 * compare(compressed_pair< T0, T1 > const & x, compressed_pair< U0, U1 > const & y) -> ...
 *
 * A sake::compressed_pair is nearly functionally identical to a
 * boost::compressed_pair, except that it is emplace- and move-aware.
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_COMPRESSED_PAIR_HPP
#define SAKE_CORE_UTILITY_COMPRESSED_PAIR_HPP

#include <cstddef>

#include <boost/config.hpp>
#include <boost/functional/hash.hpp>
#include <boost/fusion/adapted/std_pair.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/preprocessor/seq/seq.hpp>
#include <boost/type_traits/is_empty.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/fusion/adapted/compressed_pair.hpp>
#include <sake/boost_ext/fusion/sequence/intrinsic/at.hpp>
#include <sake/boost_ext/type_traits/common_type.hpp>
#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/add_reference_add_const.hpp>

#include <sake/core/functional/operators/logical_and.hpp>
#include <sake/core/functional/operators/logical_or.hpp>
#include <sake/core/functional/operators/equal.hpp>
#include <sake/core/functional/operators/less.hpp>
#include <sake/core/functional/operators/not_equal.hpp>
#include <sake/core/math/compare.hpp>
#include <sake/core/math/zero.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/movable.hpp>
#include <sake/core/utility/compressed_pair_fwd.hpp>
#include <sake/core/utility/define_natural/mem_fun.hpp>
#include <sake/core/utility/emplacer.hpp>
#include <sake/core/utility/private/is_compatible_sequence.hpp>
#include <sake/core/utility/swap.hpp>

namespace sake
{

/*******************************************************************************
 * struct compressed_pair< T0, T1 >
 ******************************************************************************/

namespace compressed_pair_adl
{

namespace private_
{

template<
    class T0, class T1,
    bool = boost::is_empty< T0 >::value,
    bool = boost::is_empty< T1 >::value
>
struct storage;

} // namespace private_

template< class T0, class T1 >
struct compressed_pair
{
    typedef T0 first_type;
    typedef T1 second_type;
    typedef boost::mpl::vector2< T0, T1 > values_type;

    SAKE_BASIC_MOVABLE_COPYABLE( compressed_pair )

    SAKE_DEFINE_NATURAL_MEM_FUN(
        compressed_pair,
        ( default_ctor ) ( move_ctor ) ( move_assign ),
        BOOST_PP_SEQ_NIL, ( m_storage )
    )

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U0, class U1 >
    compressed_pair(U0&& x0, U1&& x1)
        : m_storage(
              sake::emplacer_wrap(sake::forward< U0 >(x0)),
              sake::emplacer_wrap(sake::forward< U1 >(x1))
          )
    { }

    template< class Sequence >
    compressed_pair(Sequence&& s,
        typename boost::enable_if_c< utility_private::is_compatible_sequence<
            compressed_pair, Sequence >::value >::type* = 0)
        : m_storage(
              sake::make_emplacer(boost_ext::fusion::at_c<0>(sake::forward< Sequence >(s))),
              sake::make_emplacer(boost_ext::fusion::at_c<1>(sake::forward< Sequence >(s)))
          )
    { }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U0, class U1 >
    compressed_pair(U0& x0, U1& x1)
        : m_storage(sake::emplacer_wrap(x0), sake::emplacer_wrap(x1))
    { }

    template< class U0, class U1 >
    compressed_pair(U0& x0, U1 const & x1)
        : m_storage(sake::emplacer_wrap(x0), sake::emplacer_wrap(x1))
    { }

    template< class U0, class U1 >
    compressed_pair(U0 const & x0, U1& x1)
        : m_storage(sake::emplacer_wrap(x0), sake::emplacer_wrap(x1))
    { }

    template< class U0, class U1 >
    compressed_pair(U0 const & x0, U1 const & x1)
        : m_storage(sake::emplacer_wrap(x0), sake::emplacer_wrap(x1))
    { }

    template< class Sequence >
    compressed_pair(Sequence& s,
        typename boost::enable_if_c< utility_private::is_compatible_sequence<
            compressed_pair, Sequence& >::value >::type* = 0)
        : m_storage(
              sake::make_emplacer(boost_ext::fusion::at_c<0>(s)),
              sake::make_emplacer(boost_ext::fusion::at_c<1>(s))
          )
    { }

    template< class Sequence >
    compressed_pair(Sequence const & s,
        typename boost::enable_if_c< utility_private::is_compatible_sequence<
            compressed_pair, Sequence const & >::value >::type* = 0)
        : m_storage(
              sake::make_emplacer(boost_ext::fusion::at_c<0>(s)),
              sake::make_emplacer(boost_ext::fusion::at_c<1>(s))
          )
    { }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U0, class U1 >
    compressed_pair&
    assign(SAKE_FWD_REF( U0 ) x0, SAKE_FWD_REF( U1 ) x1)
    {
        sake::emplacer_assign(first(),  sake::forward< U0 >(x0));
        sake::emplacer_assign(second(), sake::forward< U1 >(x1));
        return *this;
    }

    template< class Sequence >
    typename boost::enable_if_c<
        utility_private::is_compatible_sequence<
            compressed_pair, SAKE_FWD_PARAM( Sequence ) >::value,
        compressed_pair&
    >::type
    operator=(SAKE_FWD_REF( Sequence ) s)
    {
        first()  = boost_ext::fusion::at_c<0>(sake::forward< Sequence >(s));
        second() = boost_ext::fusion::at_c<1>(sake::forward< Sequence >(s));
        return *this;
    }

    void swap(compressed_pair& other)
    {
        sake::swap(first(),  other.first());
        sake::swap(second(), other.second());
    }

    std::size_t
    hash_value() const
    {
        std::size_t result = 0;
        boost::hash_combine(result, first());
        boost::hash_combine(result, second());
        return x;
    }

    typename boost_ext::add_reference< T0 >::type
    first()
    { return m_storage.first(); }
    typename boost_ext::add_reference_add_const< T0 >::type
    first() const
    { return m_storage.first(); }

    typename boost_ext::add_reference< T1 >::type
    second()
    { return m_storage.second(); }
    typename boost_ext::add_reference_add_const< T1 >::type
    second() const
    { return m_storage.second(); }

private:
    private_::storage< T0, T1 > m_storage;
};

/*******************************************************************************
 * swap(compressed_pair< T0, T1 >& x, compressed_pair< T0, T1 >& y) -> void
 * hash_value(compressed_pair< T0, T1 > const & x) -> std::size_t
 ******************************************************************************/

template< class T0, class T1 >
inline void
swap(compressed_pair< T0, T1 >& x, compressed_pair< T0, T1 >& y)
{ x.swap(y); }

template< class T0, class T1 >
inline std::size_t
hash_value(compressed_pair< T0, T1 > const & x)
{ return x.hash_value(); }

/*******************************************************************************
 * operator==(compressed_pair< T0, T1 > const & x, compressed_pair< U0, U1 > const & y) -> ...
 * operator!=(compressed_pair< T0, T1 > const & x, compressed_pair< U0, U1 > const & y) -> ...
 * operator<(compressed_pair< T0, T1 > const & x, compressed_pair< U0, U1 > const & y) -> ...
 * compare(compressed_pair< T0, T1 > const & x, compressed_pair< U0, U1 > const & y) -> ...
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
operator==(compressed_pair< T0, T1 > const & x, compressed_pair< U0, U1 > const & y)
{ return x.first() == y.first() && x.second() == y.second(); }

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
operator!=(compressed_pair< T0, T1 > const & x, compressed_pair< U0, U1 > const & y)
{ return x.first() != y.first() || x.second() != y.second(); }

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
operator<(compressed_pair< T0, T1 > const & x, compressed_pair< U0, U1 > const & y)
{
    typename result_of::compare<
        typename boost_ext::add_reference_add_const< T0 >::type,
        typename boost_ext::add_reference_add_const< U0 >::type
    >::type const s0 = sake::compare(x.first(), y.first());
    return s0 == sake::zero ? x.second() < y.second() : s0 < sake::zero;
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
compare(compressed_pair< T0, T1 > const & x, compressed_pair< U0, U1 > const & y)
{
    typename result_of::compare<
        typename boost_ext::add_reference_add_const< T0 >::type,
        typename boost_ext::add_reference_add_const< U0 >::type
    >::type const s0 = sake::compare(x.first(), y.first());
    return s0 == sake::zero ? sake::compare(x.second(), y.second()) : s0;
}

namespace private_
{

template< class T0, class T1 >
struct storage< T0, T1, false, false >
{
    SAKE_BASIC_MOVABLE_COPYABLE( storage )

    SAKE_DEFINE_NATURAL_MEM_FUN(
        typename storage,
        ( default_ctor ) ( move_ctor ) ( copy_assign_if_any_umc ) ( move_assign ),
        BOOST_PP_SEQ_NIL, (( T0, m_x0 )) (( T1, m_x1 ))
    )

    storage(sake::emplacer< void ( ) >, sake::emplacer< void ( ) >)
    { }

    template< class Emplacer0 >
    storage(Emplacer0 e0, sake::emplacer< void ( ) >)
        : m_x0(sake::emplacer_construct< typename boost::remove_cv< T0 >::type >(e0))
    { }

    template< class Emplacer1 >
    storage(sake::emplacer< void ( ) >, Emplacer1 e1)
        : m_x1(sake::emplacer_construct< typename boost::remove_cv< T1 >::type >(e1))
    { }

    template< class Emplacer0, class Emplacer1 >
    storage(Emplacer0 e0, Emplacer1 e1)
        : m_x0(sake::emplacer_construct< typename boost::remove_cv< T0 >::type >(e0)),
          m_x1(sake::emplacer_construct< typename boost::remove_cv< T1 >::type >(e1))
    { }

    typename boost_ext::add_reference< T0 >::type
    first()
    { return m_x0; }
    typename boost_ext::add_reference_add_const< T0 >::type
    first() const
    { return m_x0; }

    typename boost_ext::add_reference< T1 >::type
    second()
    { return m_x1; }
    typename boost_ext::add_reference_add_const< T1 >::type
    second() const
    { return m_x1; }

private:
    T0 m_x0;
    T1 m_x1;
};

template< class T0, class T1, bool _ >
struct storage< T0, T1, true, _ >
    : private T0
{
    SAKE_BASIC_MOVABLE_COPYABLE( storage )

    SAKE_DEFINE_NATURAL_MEM_FUN(
        typename storage,
        ( default_ctor ) ( move_ctor ) ( copy_assign_if_any_umc ) ( move_assign ),
        ( T0 ), (( T1, m_x1 ))
    )

    storage(sake::emplacer< void ( ) >, sake::emplacer< void ( ) >)
    { }

    template< class Emplacer0 >
    storage(Emplacer0 e0, sake::emplacer< void ( ) >)
        : T0(sake::emplacer_construct< typename boost::remove_cv< T0 >::type >(e0))
    { }

    template< class Emplacer1 >
    storage(sake::emplacer< void ( ) >, Emplacer1 e1)
        : m_x1(sake::emplacer_construct< typename boost::remove_cv< T1 >::type >(e1))
    { }

    template< class Emplacer0, class Emplacer1 >
    storage(Emplacer0 const e0, Emplacer1 const e1)
        : T0(sake::emplacer_construct< typename boost::remove_cv< T0 >::type >(e0)),
          m_x1(sake::emplacer_construct< typename boost::remove_cv< T1 >::type >(e1))
    { }

    T0&
    first()
    { return *this; }
    T0 const &
    first() const
    { return *this; }

    typename boost_ext::add_reference< T1 >::type
    second()
    { return m_x1; }
    typename boost_ext::add_reference_add_const< T1 >::type
    second() const
    { return m_x1; }

private:
    T1 m_x1;
};

template< class T0, class T1 >
struct storage< T0, T1, false, true >
    : private T1
{
    SAKE_BASIC_MOVABLE_COPYABLE( storage )

    SAKE_DEFINE_NATURAL_MEM_FUN(
        typename storage,
        ( default_ctor ) ( move_ctor ) ( copy_assign_if_any_umc ) ( move_assign ),
        ( T1 ), (( T0, m_x0 ))
    )

    storage(sake::emplacer< void ( ) >, sake::emplacer< void ( ) >)
    { }

    template< class Emplacer0 >
    storage(Emplacer0 e0, sake::emplacer< void ( ) >)
        : m_x0(sake::emplacer_construct< typename boost::remove_cv< T0 >::type >(e0))
    { }

    template< class Emplacer1 >
    storage(sake::emplacer< void ( ) >, Emplacer1 e1)
        : T1(sake::emplacer_construct< typename boost::remove_cv< T1 >::type >(e1))
    { }

    template< class Emplacer0, class Emplacer1 >
    storage(Emplacer0 const e0, Emplacer1 const e1)
        : T1(sake::emplacer_construct< typename boost::remove_cv< T1 >::type >(e1)),
          m_x0(sake::emplacer_construct< typename boost::remove_cv< T0 >::type >(e0))
    { }

    typename boost_ext::add_reference< T0 >::type
    first()
    { return m_x0; }
    typename boost_ext::add_reference_add_const< T0 >::type
    first() const
    { return m_x0; }

    T1&
    second()
    { return *this; }
    T1 const &
    second() const
    { return *this; }

private:
    T0 m_x0;
};

} // namespace private_

} // namespace compressed_pair_adl

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_COMPRESSED_PAIR_HPP

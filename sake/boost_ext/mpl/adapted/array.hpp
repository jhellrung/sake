/*******************************************************************************
 * sake/boost_ext/mpl/adapted/array.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * This provides the template specializations necessary for a boost::array to be
 * considered a Boost.MPL sequence.
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_MPL_ADAPTED_ARRAY_HPP
#define SAKE_BOOST_EXT_MPL_ADAPTED_ARRAY_HPP

#include <cstddef>

#include <boost/mpl/advance_fwd.hpp>
#include <boost/mpl/at_fwd.hpp>
#include <boost/mpl/back_fwd.hpp>
#include <boost/mpl/begin_end_fwd.hpp>
#include <boost/mpl/begin_end.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/clear_fwd.hpp>
#include <boost/mpl/distance_fwd.hpp>
#include <boost/mpl/empty_fwd.hpp>
#include <boost/mpl/erase_fwd.hpp>
#include <boost/mpl/front_fwd.hpp>
#include <boost/mpl/insert_fwd.hpp>
#include <boost/mpl/insert_range_fwd.hpp>
#include <boost/mpl/iterator_tags.hpp>
#include <boost/mpl/next_prior.hpp>
#include <boost/mpl/pop_back_fwd.hpp>
#include <boost/mpl/pop_front_fwd.hpp>
#include <boost/mpl/push_back_fwd.hpp>
#include <boost/mpl/push_front_fwd.hpp>
#include <boost/mpl/sequence_tag_fwd.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/size_fwd.hpp>
#include <boost/mpl/size_t.hpp>
#include <boost/static_assert.hpp>

namespace boost
{

struct array_mpl_tag;

// Forward declaration.
template< class T, std::size_t N >
struct array;

struct array_mpl_iterator_tag;

template< class A, std::size_t I >
struct array_mpl_iterator
{
    BOOST_STATIC_ASSERT((0 <= I));
    BOOST_STATIC_ASSERT((I <= A::static_size));
    typedef array_mpl_iterator_tag tag; // for tag
    typedef typename A::value_type type; // for deref
    typedef array_mpl_iterator< A, I+1 > next; // for next
    typedef array_mpl_iterator< A, I-1 > prior; // for prior
    typedef boost::mpl::random_access_iterator_tag category; // Random Access Iterator requirement
};

namespace mpl
{

template< class T, std::size_t N >
struct sequence_tag< boost::array<T,N> >
{ typedef boost::array_mpl_tag type; };

/*******************************************************************************
 * array_mpl_iterator Random Access Iterator metafunction definitions
 * See
 *     http://www.boost.org/libs/mpl/doc/refmanual/forward-iterator.html
 *     http://www.boost.org/libs/mpl/doc/refmanual/bidirectional-iterator.html
 *     http://www.boost.org/libs/mpl/doc/refmanual/random-access-iterator.html
 ******************************************************************************/

template<>
struct advance_impl< boost::array_mpl_iterator_tag >
{
    template< class, class > struct apply;
    template< class A, std::size_t I, class K >
    struct apply< boost::array_mpl_iterator<A,I>, K >
    { typedef boost::array_mpl_iterator< A, I + K::value > type; };
};

template<>
struct distance_impl< boost::array_mpl_iterator_tag >
{
    template< class, class > struct apply;
    template< class A, std::size_t I, std::size_t J >
    struct apply<
        boost::array_mpl_iterator<A,I>,
        boost::array_mpl_iterator<A,J>
    >
    {
        BOOST_STATIC_ASSERT((I <= J));
        typedef boost::mpl::size_t< J-I > type;
    };
};

/*******************************************************************************
 * Forward Sequence requirements.
 * See
 *     http://www.boost.org/libs/mpl/doc/refmanual/forward-sequence.html
 ******************************************************************************/

template<>
struct begin_impl< boost::array_mpl_tag >
{
    template< class A >
    struct apply
    { typedef boost::array_mpl_iterator<A,0> type; };
};

template<>
struct end_impl< boost::array_mpl_tag >
{
    template< class A >
    struct apply
    { typedef boost::array_mpl_iterator< A, A::static_size > type; };
};

template<>
struct size_impl< boost::array_mpl_tag >
{
    template< class A >
    struct apply
        : boost::mpl::size_t< A::static_size >
    { };
};

template<>
struct empty_impl< boost::array_mpl_tag >
{
    template< class A >
    struct apply
        : boost::mpl::bool_< (A::static_size == 0) >
    { };
};

template<>
struct front_impl< boost::array_mpl_tag >
{
    template< class A >
    struct apply
    { typedef typename A::value_type type; };
};

/*******************************************************************************
 * Bidirectional Sequence requirements.
 * See
 *     http://www.boost.org/libs/mpl/doc/refmanual/bidirectional-sequence.html
 ******************************************************************************/

template<>
struct back_impl< boost::array_mpl_tag >
{
    template< class A >
    struct apply
    { typedef typename A::value_type type; };
};

/*******************************************************************************
 * RandomAccess Sequence requirements.
 * See
 *     http://www.boost.org/libs/mpl/doc/refmanual/random-access-sequence.html
 ******************************************************************************/

template<>
struct at_impl< boost::array_mpl_tag >
{
    template< class A, class I >
    struct apply
    { typedef typename A::value_type type; };
};

/*******************************************************************************
 * (Incomplete) Extensible Sequence requirements.
 * See
 *     http://www.boost.org/libs/mpl/doc/refmanual/extensible-sequence.html
 * Insertions only make sense when the type(s) being inserted is/are the same as
 * the value_type of the array.
 ******************************************************************************/

template<>
struct insert_impl< boost::array_mpl_tag >
{
    template< class A, class I, class T >
    struct apply
    { typedef boost::array< typename A::value_type, A::static_size + 1 > type; };
};

template<>
struct insert_range_impl< boost::array_mpl_tag >
{
    template< class A, class I, class R >
    struct apply
    {
        typedef boost::array<
            typename A::value_type,
            A::static_size + boost::mpl::size<R>::type::value
        > type;
    };
};

template<>
struct erase_impl< boost::array_mpl_tag >
{
    template< class, class, class > struct apply;
    template< class A, std::size_t I >
    struct apply< A, boost::array_mpl_iterator<A,I>, boost::mpl::na >
    { typedef boost::array< typename A::value_type, A::static_size - 1 > type; };
    template< class A, std::size_t I, std::size_t J >
    struct apply< A, boost::array_mpl_iterator<A,I>, boost::array_mpl_iterator<A,J> >
    {
        BOOST_STATIC_ASSERT((I <= J));
        typedef boost::array< typename A::value_type, A::static_size - (J-I) > type;
    };
};

template<>
struct clear_impl< boost::array_mpl_tag >
{
    template< class A >
    struct apply
    { typedef boost::array< typename A::value_type, 0 > type; };
};

/*******************************************************************************
 * (Incomplete) Front Extensible Sequence requirements.
 * See
 *     http://www.boost.org/libs/mpl/doc/refmanual/front-extensible-sequence.html
 * push_front only makes sense when type being prepended is the same as the
 * value_type of the array.
 ******************************************************************************/

template<>
struct push_front_impl< boost::array_mpl_tag >
{
    template< class A, class T >
    struct apply
    { typedef boost::array< typename A::value_type, A::static_size + 1 > type; };
};

template<>
struct pop_front_impl< boost::array_mpl_tag >
{
    template< class A >
    struct apply
    { typedef boost::array< typename A::value_type, A::static_size - 1 > type; };
};

/*******************************************************************************
 * (Incomplete) Back Extensible Sequence requirements.
 * See
 *     http://www.boost.org/libs/mpl/doc/refmanual/back-extensible-sequence.html
 * push_back only makes sense when type being appended is the same as the
 * value_type of the array.
 ******************************************************************************/

template<>
struct push_back_impl< boost::array_mpl_tag >
{
    template< class A, class T >
    struct apply
    { typedef boost::array< typename A::value_type, A::static_size + 1 > type; };
};

template<>
struct pop_back_impl< boost::array_mpl_tag >
{
    template< class A >
    struct apply
    { typedef boost::array< typename A::value_type, A::static_size - 1 > type; };
};

} // namespace mpl

} // namespace boost

#endif // #ifndef SAKE_BOOST_EXT_MPL_ADAPTED_ARRAY_HPP

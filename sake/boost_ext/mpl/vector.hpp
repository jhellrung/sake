/*******************************************************************************
 * sake/boost_ext/mpl/vector.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::mpl::vector< T... >
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_MPL_VECTOR_HPP
#define SAKE_BOOST_EXT_MPL_VECTOR_HPP

#include <boost/config.hpp>

#ifndef BOOST_NO_VARIADIC_TEMPLATES

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
#include <boost/mpl/size_fwd.hpp>
#include <boost/mpl/size_t.hpp>
#include <boost/static_assert.hpp>

namespace sake
{

namespace boost_ext
{

namespace mpl
{

struct vector_tag;

template< class... T >
struct vector
{
    typedef vector type;
    typedef vector_tag tag; // for sequence_tag
    static std::size_t const size = sizeof...( T );
};

struct vector_iterator_tag;

struct vector_iterator_base
{
    typedef vector_iterator_tag tag; // for tag
    typedef boost::mpl::random_access_iterator_tag category; // Random Access Iterator requirement
};

template< class V, class W >
struct vector_iterator;

template<>
struct vector_iterator< vector<>, vector<> >
    : vector_iterator_base
{ };

template< class T0, class... T >
struct vector_iterator< vector< T0, T... >, vector<> >
    : vector_iterator_base
{
    typedef T0 type; // for deref
    typedef vector_iterator< vector< T... >, vector< T0 > > next; // for next
    static std::size_t const index = 0;
};

template< class U0, class... U >
struct vector_iterator< vector<>, vector< U0, U... > >
    : vector_iterator_base
{
    typedef vector_iterator< vector< U0 >, vector< U... > > prior; // for prior
    static std::size_t const index = 1 + sizeof...( U );
};

template< class T0, class... T, class U0, class... U >
struct vector_iterator< vector< T0, T... >, vector< U0, U... > >
    : vector_iterator_base
{
    typedef T0 type; // for deref
    typedef vector_iterator< vector< T... >, vector< T0, U0, U... > > next; // for next
    typedef vector_iterator< vector< U0, T0, T... >, vector< U... > > prior; // for prior
    static std::size_t const index = 1 + sizeof...( U );
};

namespace vector_private
{

template< class I, std::size_t K >
struct advance_c;
template< class V, class W, std::size_t K >
struct advance_c< vector_iterator<V,W>, K >
    : advance_c< typename vector_iterator<V,W>::next, K-1 >
{ };
template< class V, class W >
struct advance_c< vector_iterator<V,W>, 0 >
{ typedef vector_iterator<V,W> type; };

template< class V, std::size_t I >
struct at_c;
template< class T0, class... T, std::size_t I >
struct at_c< vector< T0, T... >, I >
    : at_c< vector< T... >, I-1 >
{ };
template< class T0, class... T >
struct at_c< vector< T0, T... >, 0 >
{ typedef T0 type; };

template< class V, class W = vector<> >
struct reverse;
template< class T0, class... T, class... U >
struct reverse< vector< T0, T... >, vector< U... > >
    : reverse< vector< T... >, vector< T0, U... > >
{ };
template< class U... >
struct reverse< vector<>, vector< U... > >
{ typedef vector< U... > type; };

template< class V, class W = vector<> >
struct pop_back;
template< class T0, class... T, class... U >
struct pop_back< vector< T0, T... >, vector< U... > >
    : pop_back< vector< T... >, vector< U..., T0 > >
{ };
template< class T0, class... U >
struct pop_back< vector< T0 >, vector< U... > >
{ typedef vector< U... > type; };

} // namespace vector_private

} // namespace mpl

} // namespace boost_ext

} // namespace sake

namespace boost
{

namespace mpl
{

/*******************************************************************************
 * sake::boost_ext::mpl::vector_iterator Random Access Iterator metafunction
 * definitions
 * See
 *     http://www.boost.org/libs/mpl/doc/refmanual/forward-iterator.html
 *     http://www.boost.org/libs/mpl/doc/refmanual/bidirectional-iterator.html
 *     http://www.boost.org/libs/mpl/doc/refmanual/random-access-iterator.html
 ******************************************************************************/

template<>
struct advance_impl< sake::boost_ext::mpl::vector_iterator_tag >
{
    template< class I, class K >
    struct apply
        : sake::boost_ext::mpl::vector_private::advance_c< I, K::value >
    { };
};

template<>
struct distance_impl< sake::boost_ext::mpl::vector_iterator_tag >
{
    template< class I, class J >
    struct apply
    {
        BOOST_STATIC_ASSERT((I::index <= J::index));
        typedef boost::mpl::size_t< J::index - I::index > type;
    };
};

/*******************************************************************************
 * Forward Sequence requirements.
 * See
 *     http://www.boost.org/libs/mpl/doc/refmanual/forward-sequence.html
 ******************************************************************************/

template<>
struct begin_impl< sake::boost_ext::mpl::vector_tag >  
{
    template< class V >
    struct apply
    {
        typedef sake::boost_ext::mpl::vector_iterator<
            typename V::type, sake::boost_ext::mpl::vector<> > type;
    };
};

template<>
struct end_impl< sake::boost_ext::mpl::vector_tag >
{
    template< class V >
    struct apply
        : advance_impl< sake::boost_ext::mpl::vector_iterator_tag >::template apply<
              typename begin_impl< sake::boost_ext::mpl::vector_tag >::template apply<V>::type,
              boost::mpl::size_t< V::size >
          > >
    { };
};

template<>
struct size_impl< sake::boost_ext::mpl::vector_tag >
{
    template< class V >
    struct apply
        : boost::mpl::size_t< V::size >
    { };
};

template<>
struct empty_impl< sake::boost_ext::mpl::vector_tag >
{
    template< class V >
    struct apply
        : boost::mpl::bool_< (V::size == 0) >
    { };
};

template<>
struct front_impl< sake::boost_ext::mpl::vector_tag >
{
    template< class V >
    struct apply
        : apply< typename V::type >
    { };
    template< class T0, class... T >
    struct apply< sake::boost_ext::mpl::vector< T0, T... > >
    { typedef T0 type; };
};

/*******************************************************************************
 * Bidirectional Sequence requirements.
 * See
 *     http://www.boost.org/libs/mpl/doc/refmanual/bidirectional-sequence.html
 ******************************************************************************/

template<>
struct back_impl< sake::boost_ext::mpl::vector_tag >
{
    template< class V >
    struct apply
        : apply< typename V::type >
    { };
    template< class T0 >
    struct apply< sake::boost_ext::mpl::vector< T0 > >
    { typedef T0 type; };
    template< class T0, class... T >
    struct apply< sake::boost_ext::mpl::vector< T0, T... > >
        : apply< sake::boost_ext::mpl::vector< T... > >
    { };
};

/*******************************************************************************
 * Random Access Sequence requirements.
 * See
 *     http://www.boost.org/libs/mpl/doc/refmanual/random-access-sequence.html
 ******************************************************************************/

template<>
struct at_impl< sake::boost_ext::mpl::vector_tag >
{
    template< class V, class I >
    struct apply
        : sake::boost_ext::mpl::vector_private::at_c< typename V::type, I::value >
    { };
};

/*******************************************************************************
 * Extensible Sequence requirements.
 * See
 *     http://www.boost.org/libs/mpl/doc/refmanual/extensible-sequence.html
 ******************************************************************************/

template<>
struct insert_impl< sake::boost_ext::mpl::vector_tag >
{
    template< class, class, class > struct apply;
    template< class _, class... T, class W, class X >
    struct apply<
        _,
        sake::boost_ext::mpl::vector_iterator<
            sake::boost_ext::mpl::vector< T... >, W >,
        X
    >
        : sake::boost_ext::mpl::vector_private::reverse<
              W, sake::boost_ext::mpl::vector< X, T... > >
    { };
};

template<>
struct insert_range_impl< sake::boost_ext::mpl::vector_tag >
{
    template< class, class, class > struct apply;
private:
    template< class Ri, class Re, class V >
    struct helper;
    template< class Ri, class Re, class... T >
    struct helper< Ri, Re, sake::boost_ext::mpl::vector< T... > >
        : helper<
              typename boost::mpl::next< Ri >::type, Re,
              sake::boost_ext::mpl::vector<
                  typename boost::mpl::deref< Ri >::type, T... >
          >
    { };
    template< class Re, class... T >
    struct helper< Re, Re, sake::boost_ext::mpl::vector< T... > >
    { typedef sake::boost_ext::vector< T... > type; };
public:
    template< class _, class V, class W, class R >
    struct apply< _, sake::boost_ext::mpl::vector_iterator<V,W>, R >
        : sake::boost_ext::mpl::vector_private::reverse<
              typename helper<
                  typename boost::mpl::begin<R>::type,
                  typename boost::mpl::end<R>::type,
                  W
              >::type,
              V
          >
    { };
};

template<>
struct erase_impl< sake::boost_ext::mpl::vector_tag >
{
    template< class, class, class > struct apply;
    template< class _, class T0, class... T, class W >
    struct apply<
        _,
        sake::boost_ext::mpl::vector_iterator<
            sake::boost_ext::mpl::vector< T0, T... >, W >,
        boost::mpl::na
    >
        : sake::boost_ext::mpl::vector_private::reverse<
              W, sake::boost_ext::mpl::vector< T... > >
    { };
    template< class _, class Va, class Wa, class Vb, class Wb >
    struct apply<
        _,
        sake::boost_ext::mpl::vector_iterator< Va, Wa >,
        sake::boost_ext::mpl::vector_iterator< Vb, Wb >
    >
        : sake::boost_ext::mpl::vector_private::reverse< Wa, Vb >
    { };
};

template<>
struct clear_impl< sake::boost_ext::mpl::vector_tag >
{
    template< class >
    struct apply
    { typedef sake::boost_ext::mpl::vector<> type; };
};

/*******************************************************************************
 * Front Extensible Sequence requirements.
 * See
 *     http://www.boost.org/libs/mpl/doc/refmanual/front-extensible-sequence.html
 ******************************************************************************/

template<>
struct push_front_impl< sake::boost_ext::mpl::vector_tag >
{
    template< class V, class X >
    struct apply
        : apply< typename V::type, X >
    { };
    template< class... T, class X >
    struct apply< sake::boost_ext::mpl::vector< T... >, X >
    { typedef sake::boost_ext::mpl::vector< X, T... > type; };
};

template<>
struct pop_front_impl< sake::boost_ext::mpl::vector_tag >
{
    template< class V >
    struct apply
        : apply< typename V::type >
    { };
    template< class T0, class... T >
    struct apply< sake::boost_ext::mpl::vector< T0, T... > >
    { typedef sake::boost_ext::mpl::vector< T... > type; };
};

/*******************************************************************************
 * Back Extensible Sequence requirements.
 * See
 *     http://www.boost.org/libs/mpl/doc/refmanual/back-extensible-sequence.html
 ******************************************************************************/

template<>
struct push_back_impl< sake::boost_ext::mpl::vector_tag >
{
    template< class V, class X >
    struct apply
        : apply< typename V::type, X >
    { };
    template< class... T, class X >
    struct apply< sake::boost_ext::mpl::vector< T... >, X >
    { typedef sake::boost_ext::mpl::vector< T..., X > type; };
};

template<>
struct pop_back_impl< sake::boost_ext::mpl::vector_tag >
{
    template< class V >
    struct apply
        : sake::boost_ext::mpl::vector_private::pop_back< typename V::type >
    { };
};

} // namespace mpl

} // namespace boost

#endif // #ifndef BOOST_NO_VARIADIC_TEMPLATES

#endif // #ifndef SAKE_BOOST_EXT_MPL_VECTOR_HPP

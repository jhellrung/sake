/*******************************************************************************
 * sake/boost_ext/fusion/adapted/tuple.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_FUSION_ADAPTED_TUPLE_HPP
#define SAKE_BOOST_EXT_FUSION_ADAPTED_TUPLE_HPP

#include <cstddef>

#include <boost/config.hpp>
#include <boost/fusion/iterator/iterator_facade.hpp>
#include <boost/fusion/sequence/intrinsic/at.hpp>
#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/sequence/intrinsic/end.hpp>
#include <boost/fusion/sequence/intrinsic/size.hpp>
#include <boost/fusion/sequence/intrinsic/value_at.hpp>
#include <boost/fusion/support/category_of.hpp>
#include <boost/fusion/support/is_sequence.hpp>
#include <boost/fusion/support/is_view.hpp>
#include <boost/fusion/support/tag_of_fwd.hpp>
#include <boost/mpl/at.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/integral_constant.hpp>

#include <sake/core/data_structures/tuple/fwd.hpp>

namespace sake
{

namespace boost_ext
{

namespace fusion
{

struct tuple_tag;

template< class T, int I >
struct tuple_iterator
    : boost::fusion::iterator_facade<
          tuple_iterator<T,I>,
          boost::fusion::random_access_traversal_tag
      >
{
    BOOST_STATIC_ASSERT((I <= T::static_size));
    static std::size_t const i = I;

    explicit tuple_iterator(T& x) : m_x(x) { }

    template< class This >
    struct value_of
    {
        static std::size_t const i = This::i;
        typedef typename boost::mpl::at_c< typename T::values_type, i >::type type;
    };
    template< class > friend struct value_of;

    template< class This >
    struct deref
    {
        static std::size_t const i = This::i;
        BOOST_STATIC_ASSERT((i < T::static_size));
        typedef typename T::result_of::template at_c<T,i>::type type;
        static type call(This this_)
        { return this_.m_x.template at_c<i>(); }
    };
    template< class > friend struct deref;

    template< class This >
    struct next
    {
        static std::size_t const i = This::i;
        BOOST_STATIC_ASSERT((i < T::static_size));
        typedef tuple_iterator< T, i+1 > type;
        static type call(This this_)
        { return type(this_.m_x); }
    };
    template< class > friend struct next;

    template< class This >
    struct prior
    {
        static std::size_t const i = This::i;
        BOOST_STATIC_ASSERT((i > 0));
        typedef tuple_iterator< T, i-1 > type;
        static type call(This this_)
        { return type(this_.m_x); }
    };
    template< class > friend struct prior;

    template< class This0, class This1 >
    struct distance
    {
        static std::size_t const i0 = This0::i;
        static std::size_t const i1 = This1::i;
        typedef boost::integral_constant< int, i1 - i0 > type;
        static type call(This0, This1)
        { return type(); }
    };
    template< class, class > friend struct distance;

    template< class This, class N >
    struct advance
    {
        static std::size_t const i = This::i;
        static int const n = N::value;
        BOOST_STATIC_ASSERT((0 <= i + n));
        BOOST_STATIC_ASSERT((i + n <= T::static_size));
        typedef tuple_iterator< T, i+n > type;
        static type call(This this_)
        { return type(this_.m_x); }
    };
    template< class, class > friend struct advance;

private:
    T& m_x;
};

} // namespace fusion

} // namespace boost_ext

} // namespace sake

namespace boost
{

namespace fusion
{

namespace extension
{

template<>
struct is_sequence_impl< sake::boost_ext::fusion::tuple_tag >
{
    template< class >
    struct apply
        : boost::true_type
    { };
};

template<>
struct is_view_impl< sake::boost_ext::fusion::tuple_tag >
{
    template< class >
    struct apply
        : boost::false_type
    { };
};

template<>
struct category_of_impl< sake::boost_ext::fusion::tuple_tag >
{
    template<class>
    struct apply
    { typedef boost::fusion::random_access_traversal_tag type; };
};

/*******************************************************************************
 * Sequence requirements.
 * See
 *     http://www.boost.org/libs/fusion/doc/html/fusion/sequence/concepts/forward_sequence.html
 *     http://www.boost.org/libs/fusion/doc/html/fusion/sequence/concepts/bidirectional_sequence.html
 *     http://www.boost.org/libs/fusion/doc/html/fusion/sequence/concepts/random_access_sequence.html
 ******************************************************************************/

template<>
struct begin_impl< sake::boost_ext::fusion::tuple_tag >
{
    template< class This >
    struct apply
    {
        typedef sake::boost_ext::fusion::tuple_iterator< This, 0 > type;
        static type call(This& this_)
        { return type(this_); }
    };
};

template<>
struct end_impl< sake::boost_ext::fusion::tuple_tag >
{
    template< class This >
    struct apply
    {
        typedef sake::boost_ext::fusion::tuple_iterator< This, This::static_size > type;
        static type call(This& this_)
        { return type(this_); }
    };
};

template<>
struct size_impl< sake::boost_ext::fusion::tuple_tag >
{
    template< class This >
    struct apply
        : boost::integral_constant< std::size_t, This::static_size >
    { };
};

template<>
struct at_impl< sake::boost_ext::fusion::tuple_tag >
{
    template< class This, class I >
    struct apply
    {
        static std::size_t const i = I::value;
        BOOST_STATIC_ASSERT((0 <= i));
        BOOST_STATIC_ASSERT((i < This::static_size));
        typedef typename This::result_of::template at_c< This, i >::type type;
        static type call(This& this_)
        { return this_.template at_c<i>(); }
    };
};

template<>
struct value_at_impl< sake::boost_ext::fusion::tuple_tag >
{
    template< class This, class I >
    struct apply
        : boost::mpl::at< typename This::values_type, I >
    { };
};

} // namespace extension

namespace traits
{

#ifndef BOOST_NO_VARIADIC_TEMPLATES
template< class... T >
struct tag_of< sake::tuple< T... > >
#else // #ifndef BOOST_NO_VARIADIC_TEMPLATES
template< BOOST_PP_ENUM_PARAMS( SAKE_TUPLE_MAX_SIZE, class T ) >
struct tag_of< sake::tuple< BOOST_PP_ENUM_PARAMS( SAKE_TUPLE_MAX_SIZE, T ) > >
#endif // #ifndef BOOST_NO_VARIADIC_TEMPLATES
{ typedef sake::boost_ext::fusion::tuple_tag type; };

} // namespace traits

} // namespace fusion

} // namespace boost

#endif // #ifndef SAKE_BOOST_EXT_FUSION_ADAPTED_TUPLE_HPP

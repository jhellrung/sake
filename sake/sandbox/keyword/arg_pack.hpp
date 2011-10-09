/*******************************************************************************
 * sake/sandbox/keyword/arg_pack.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef BOOST_PP_IS_ITERATING

#ifndef SAKE_SANDBOX_KEYWORD_ARG_PACK_HPP
#define SAKE_SANDBOX_KEYWORD_ARG_PACK_HPP

#include <boost/config.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_shifted_params.hpp>

#include <sake/boost_ext/mpl/vector.hpp>

#include <sake/sandbox/keyword/fwd.hpp>

namespace sake
{

namespace keyword
{

template< class V >
struct arg_pack_from_mpl_vector;

template<>
struct arg_pack<>
{
    struct result_of
    {
        template< class K >
        struct at
        { typename K::value_type type; };
    };

    template< class K >
    typename result_of::template at<K>::type
    operator[](K const k) const
    { return at(k, typename K::tag()); }

protected:
    template< class K, class Tag >
    typename K::value_type
    at(K const k, Tag) const
    { return k.value(); }
};

#ifndef BOOST_PP_NO_VARIADIC_TEMPLATES

template< class... A >
struct arg_pack_from_mpl_vector< boost_ext::mpl::vector< A... > >
{ typedef arg_pack< A... > type; };

template< class A0, class... A >
struct arg_pack< A0, A... >
    : arg_pack< A... >
{
    explicit arg_pack(A0 const a0, A const... a)
        : arg_pack< A... >(a...),
          m_a0(a0)
    { }

    struct result_of
    {
        template< class K, class Tag = typename K::tag >
        struct at
            : arg_pack< A... >::result_of::template at<K>
        { };
        template< class K >
        struct at< K, typename A0::tag >
        { typedef typename A0::value_type type; };
    };

    template< class K >
    typename result_of::template at<K>::type
    operator[](K const k) const
    { return at(k, typename K::tag()); }

protected:
    template< class K >
    typename A0::value_type
    at(K const k, typename A0::tag) const
    { return m_a0.value(); }

    using arg_pack< A... >::at;

private:
    A0 const m_a0;
};

#else // #ifndef BOOST_PP_NO_VARIADIC_TEMPLATES

#define BOOST_PP_ITERATION_LIMITS ( 1, SAKE_KEYWORD_MAX_ARITY )
#define BOOST_PP_FILENAME_1       <sake/sandbox/keyword/arg_pack.hpp>
#include BOOST_PP_ITERATE()

#endif // #ifndef BOOST_PP_NO_VARIADIC_TEMPLATES

} // namespace keyword

} // namespace sake

#endif // #ifndef SAKE_SANDBOX_KEYWORD_ARG_PACK_HPP

#else // #ifndef BOOST_PP_IS_ITERATING

#define N BOOST_PP_ITERATION()

#define class_A0N     BOOST_PP_ENUM_PARAMS( N, class A )
#define A0N           BOOST_PP_ENUM_PARAMS( N, A )
#define A1N           BOOST_PP_ENUM_SHIFTED_PARAMS( N, A )
#define A0N_const_a0N BOOST_PP_ENUM_BINARY_PARAMS( N, A, const a )
#define a1N           BOOST_PP_ENUM_SHIFTED_PARAMS( N, A, const a )

template< class_A0N >
struct arg_pack_from_mpl_vector< boost::mpl::vector< A0N > >
{ typedef arg_pack< A0N > type; };

template< class_A0N >
struct arg_pack_from_mpl_vector< BOOST_PP_CAT( boost::mpl::vector, N ) < A0N > >
{ typedef arg_pack< A0N > type; };

template< class_A0N >
#if N == SAKE_KEYWORD_MAX_ARITY
struct arg_pack
#else // #if N == SAKE_KEYWORD_MAX_ARITY
struct arg_pack< BOOST_PP_ENUM_PARAMS( N, A ) >
#endif // #if N == SAKE_KEYWORD_MAX_ARITY
    : arg_pack< A1N >
{
#if N == 1
    explicit
#endif // #if N == 1
    arg_pack(A0N_const_a0N)
        : arg_pack< A1N >(a1N),
          m_a0(a0)
    { }

    struct result_of
    {
        template< class K, class Tag = typename K::tag >
        struct at
            : arg_pack< A1N >::result_of::template at<K>
        { };
        template< class K >
        struct at< K, typename A0::tag >
        { typedef typename A0::value_type type; };
    };

    template< class K >
    typename result_of::template at<K>::type
    operator[](K const k) const
    { return at(k, typename K::tag()); }

protected:
    template< class K >
    typename A0::value_type
    at(K const k, typename A0::tag) const
    { return m_a0.value(); }

    using arg_pack< A1N >::at;

private:
    A0 const m_a0;
};

#undef class_A0N
#undef A0N
#undef A1N
#undef A0N_const_a0N
#undef a1N

#undef N

#endif // #ifndef BOOST_PP_IS_ITERATING

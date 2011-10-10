/*******************************************************************************
 * sake/boost_ext/mpl/and.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::mpl::and[N]< T0, ..., T[N-1] >
 * struct boost_ext::mpl::and_< T... >
 ******************************************************************************/

#ifndef BOOST_PP_IS_ITERATING

#ifndef SAKE_BOOST_EXT_MPL_AND_HPP
#define SAKE_BOOST_EXT_MPL_AND_HPP

#include <boost/config.hpp>
#include <boost/preprocessor/arithmetic/dec.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_shifted_params.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/integral_constant.hpp>

#ifndef SAKE_BOOST_EXT_MPL_LIMIT_NUMBERED_METAFUNCTION_ARITY
#define SAKE_BOOST_EXT_MPL_LIMIT_NUMBERED_METAFUNCTION_ARITY 8
#endif // #ifndef SAKE_BOOST_EXT_MPL_LIMIT_NUMBERED_METAFUNCTION_ARITY

namespace sake
{

namespace boost_ext
{

namespace mpl
{

template< class = void >
struct and0;

template<>
struct and0< void >
    : boost::true_type
{ };

namespace and_private
{

template< bool, class T >
struct helper;

template< class T >
struct helper< true, T >
    : T
{ static bool const value = T::value; };

template< class T >
struct helper< false, T >
    : boost::false_type
{ };

} // namespace and_private

#define BOOST_PP_ITERATION_LIMITS ( 1, SAKE_BOOST_EXT_MPL_LIMIT_NUMBERED_METAFUNCTION_ARITY )
#define BOOST_PP_FILENAME_1       <sake/boost_ext/mpl/and.hpp>
#include BOOST_PP_ITERATE()

#ifndef BOOST_NO_VARIADIC_TEMPLATES

template< class... T >
struct and_;

template<>
struct and_<>
    : boost::true_type
{ };

template< class T0, class... T >
struct and_
    : and_private::helper< T0::type::value, and_< T... > >
{ };

#endif // #ifndef BOOST_NO_VARIADIC_TEMPLATES

namespace and_private
{

namespace
{

BOOST_STATIC_ASSERT(( boost_ext::mpl::and0<>::value));
BOOST_STATIC_ASSERT(( boost_ext::mpl::and1< boost::true_type >::value));
BOOST_STATIC_ASSERT((!boost_ext::mpl::and1< boost::false_type >::value));
BOOST_STATIC_ASSERT(( boost_ext::mpl::and2< boost::true_type, boost::true_type >::value));
BOOST_STATIC_ASSERT((!boost_ext::mpl::and2< boost::true_type, boost::false_type >::value));
BOOST_STATIC_ASSERT((!boost_ext::mpl::and2< boost::false_type, boost::true_type >::value));
BOOST_STATIC_ASSERT((!boost_ext::mpl::and2< boost::false_type, boost::false_type >::value));

} // namespace

} // namespace and_private

} // namespace mpl

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_MPL_AND_HPP

#else // #ifndef BOOST_PP_IS_ITERATING

#define N BOOST_PP_ITERATION()

template< BOOST_PP_ENUM_PARAMS( N, class T ) >
struct BOOST_PP_CAT( and, N )
    : and_private::helper<
          T0::type::value,
          BOOST_PP_CAT( and, BOOST_PP_DEC( N ) )<
              BOOST_PP_ENUM_SHIFTED_PARAMS( N, T )
          >
      >
{ };

#undef N

#endif // #ifndef BOOST_PP_IS_ITERATING

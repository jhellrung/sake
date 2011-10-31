/*******************************************************************************
 * sake/boost_ext/mpl/as_vector.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::mpl::as_vector< Sequence >
 ******************************************************************************/

#ifndef BOOST_PP_IS_ITERATING

#ifndef SAKE_BOOST_EXT_MPL_AS_VECTOR_HPP
#define SAKE_BOOST_EXT_MPL_AS_VECTOR_HPP

#include <boost/config.hpp>
#include <boost/mpl/back_inserter.hpp>
#include <boost/mpl/copy.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/vector/vector0.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/mpl/vector/vector20.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/tuple/rem.hpp>
#include <boost/preprocessor/tuple/reverse.hpp>

#include <sake/boost_ext/mpl/vector.hpp>

#ifndef SAKE_BOOST_EXT_MPL_AS_VECTOR_MAX_SIZE
#define SAKE_BOOST_EXT_MPL_AS_VECTOR_MAX_SIZE 16
#endif // #ifndef SAKE_BOOST_EXT_MPL_AS_VECTOR_MAX_SIZE

namespace sake
{

namespace boost_ext
{

namespace mpl
{

template< class Sequence >
struct as_vector
#if   !defined( BOOST_NO_VARIADIC_TEMPLATES )
    : boost::mpl::copy<
          Sequence,
          boost::mpl::back_inserter< boost_ext::mpl::vector<> >
      >
#elif !defined( BOOST_MPL_CFG_TYPEOF_BASED_SEQUENCES )
    : boost::mpl::copy<
          Sequence,
          boost::mpl::back_inserter< boost::mpl::vector0<> >
      >
#else // #if ...
    : boost_ext::mpl::as_vector<
          typename boost::mpl::copy<
              Sequence,
              boost::mpl::back_inserter< boost::mpl::vector0<> >
          >::type
      >
#endif // #if ...
{ };

#define BOOST_PP_ITERATION_LIMITS ( 0, SAKE_BOOST_EXT_MPL_AS_VECTOR_MAX_SIZE )
#define BOOST_PP_FILENAME_1       <sake/boost_ext/mpl/as_vector.hpp>
#include BOOST_PP_ITERATE()

} // namespace mpl

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_MPL_AS_VECTOR_HPP

#else // #ifndef BOOST_PP_IS_ITERATING

#define N BOOST_PP_ITERATION()

#define class_T0N BOOST_PP_ENUM_PARAMS( N, class T )
#define T0N       BOOST_PP_ENUM_PARAMS( N, T )

#define vectorN boost::mpl::BOOST_PP_CAT( vector, N )

template< class_T0N >
struct as_vector< boost::mpl::vector< T0N > >
#ifndef BOOST_NO_VARIADIC_TEMPLATES
{ typedef boost_ext::mpl::vector< T0N > type; };
#else // BOOST_NO_VARIADIC_TEMPLATES
{ typedef vectorN< T0N > type; };
#endif // BOOST_NO_VARIADIC_TEMPLATES

template< class_T0N >
struct as_vector< vectorN< T0N > >
#ifndef BOOST_NO_VARIADIC_TEMPLATES
{ typedef boost_ext::mpl::vector< T0N > type; };
#else // BOOST_NO_VARIADIC_TEMPLATES
{ typedef vectorN< T0N > type; };
#endif // BOOST_NO_VARIADIC_TEMPLATES

#ifndef BOOST_NO_VARIADIC_TEMPLATES
template< class_T0N >
struct as_vector< boost_ext::mpl::vector< T0N > >
{ typedef boost_ext::mpl::vector< T0N > type; };
#endif // BOOST_NO_VARIADIC_TEMPLATES

#if N > 0 && defined( BOOST_MPL_CFG_TYPEOF_BASED_SEQUENCES )

#define TN0 BOOST_PP_TUPLE_REM_CTOR( N, BOOST_PP_TUPLE_REVERSE( N, ( T0N ) ) )

template< class_T0N >
struct as_vector<
    BOOST_PP_ENUM_PARAMS( N, boost::mpl::v_item< T ),
    boost::mpl::vector<>,
    BOOST_PP_ENUM_PARAMS( N, 0 > BOOST_PP_INTERCEPT )
>
#ifndef BOOST_NO_VARIADIC_TEMPLATES
{ typedef boost_ext::mpl::vector< TN0 > type; };
#else // BOOST_NO_VARIADIC_TEMPLATES
{ typedef vectorN< TN0 > type; };
#endif // BOOST_NO_VARIADIC_TEMPLATES

template< class_T0N >
struct as_vector<
    BOOST_PP_ENUM_PARAMS( N, boost::mpl::v_item< T ),
    boost::mpl::vector0<>,
    BOOST_PP_ENUM_PARAMS( N, 0 > BOOST_PP_INTERCEPT )
>
#ifndef BOOST_NO_VARIADIC_TEMPLATES
{ typedef boost_ext::mpl::vector< TN0 > type; };
#else // BOOST_NO_VARIADIC_TEMPLATES
{ typedef vectorN< TN0 > type; };
#endif // BOOST_NO_VARIADIC_TEMPLATES

#undef TN0

template< class_T0N >
struct as_vector<
    BOOST_PP_ENUM_PARAMS( N, boost::mpl::v_item< T ),
    boost::mpl::vector<>,
    BOOST_PP_ENUM_PARAMS( N, 1 > BOOST_PP_INTERCEPT )
>
#ifndef BOOST_NO_VARIADIC_TEMPLATES
{ typedef boost_ext::mpl::vector< T0N > type; };
#else // BOOST_NO_VARIADIC_TEMPLATES
{ typedef vectorN< T0N > type; };
#endif // BOOST_NO_VARIADIC_TEMPLATES

template< class_T0N >
struct as_vector<
    BOOST_PP_ENUM_PARAMS( N, boost::mpl::v_item< T ),
    boost::mpl::vector0<>,
    BOOST_PP_ENUM_PARAMS( N, 1 > BOOST_PP_INTERCEPT )
>
#ifndef BOOST_NO_VARIADIC_TEMPLATES
{ typedef boost_ext::mpl::vector< T0N > type; };
#else // BOOST_NO_VARIADIC_TEMPLATES
{ typedef vectorN< T0N > type; };
#endif // BOOST_NO_VARIADIC_TEMPLATES

#endif // #if N > 0 && defined( BOOST_MPL_CFG_TYPEOF_BASED_SEQUENCES )

#undef vectorN

#undef class_T0N
#undef T0N

#undef N

#endif // #ifndef BOOST_PP_IS_ITERATING

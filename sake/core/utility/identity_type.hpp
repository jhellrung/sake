/*******************************************************************************
 * sake/core/utility/identity_type.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_IDENTITY_TYPE_WRAP( _T_ )
 * #define SAKE_IDENTITY_TYPE( T )
 *
 * SAKE_IDENTITY_TYPE_WRAP( _T_ ) expands to a type identical to T (where T is
 * _T_ after stripping off the outer level of parentheses, which is required)
 * but which may be passed as a parameter to a preprocessor macro.  This is
 * useful if T has unparenthesized commas in it, e.g., X<Y,Z>.  If T is a
 * dependent type, you'll need to prefix SAKE_IDENTITY_TYPE_WRAP( _T_ ) with a
 * "typename" keyword.
 *
 * SAKE_IDENTITY_TYPE is a slight generalization of SAKE_IDENTITY_TYPE_WRAP.  If
 * its argument T is not a Boost.Preprocessor 1-tuple, it simply expands to T.
 * Otherwise, it expands to SAKE_IDENTITY_TYPE_WRAP T.  One may prefix the
 * 1-tuple argument with the "typename" keyword to indicate that the argument
 * corresponds to a dependent type.
 *
 * Example:
 *
 * template< class T, class U > struct X;
 *
 * SAKE_IDENTITY_TYPE( int )
 *     -> int
 * SAKE_IDENTITY_TYPE((( int )))
 *     -> SAKE_IDENTITY_TYPE_WRAP(( int ))
 * SAKE_IDENTITY_TYPE((( X<T,U> )))
 *     -> SAKE_IDENTITY_TYPE_WRAP(( X<T,U> ))
 * SAKE_IDENTITY_TYPE( typename (( X<T,U> )) )
 *     -> typename SAKE_IDENTITY_TYPE_WRAP(( X<T,U> ))
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_IDENTITY_TYPE_HPP
#define SAKE_CORE_UTILITY_IDENTITY_TYPE_HPP

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/control/expr_iif.hpp>
#include <boost/preprocessor/detail/is_unary.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>

#include <sake/boost_ext/preprocessor/keyword/typename.hpp>

#define SAKE_IDENTITY_TYPE_WRAP( _T_ ) \
    ::sake::identity_type_private::impl< void _T_ >::type

#define SAKE_IDENTITY_TYPE( T ) \
    BOOST_PP_CAT( SAKE_IDENTITY_TYPE_a, BOOST_PP_IS_UNARY( T ) ) ( T )
#define SAKE_IDENTITY_TYPE_a0( T ) \
    SAKE_IDENTITY_TYPE_b( T, SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX_TYPENAME( T ) )
#define SAKE_IDENTITY_TYPE_b( T, U ) \
    BOOST_PP_CAT( SAKE_IDENTITY_TYPE_b, BOOST_PP_IS_UNARY( U ) ) ( T, U )
#define SAKE_IDENTITY_TYPE_b0( T, U ) \
    T
#define SAKE_IDENTITY_TYPE_b1( T, U ) \
    BOOST_PP_EXPR_IIF( SAKE_BOOST_EXT_PP_KEYWORD_HAS_PREFIX_TYPENAME( T ), typename ) \
    SAKE_IDENTITY_TYPE_WRAP U
#define SAKE_IDENTITY_TYPE_a1( T ) \
    SAKE_IDENTITY_TYPE_WRAP T

namespace sake
{

namespace identity_type_private
{

template< class >
struct impl;

template<>
struct impl< void ( ) >
{ typedef void type; };

template< class T >
struct impl< void ( T ) >
{ typedef T type; };

BOOST_STATIC_ASSERT((boost::is_same< void, SAKE_IDENTITY_TYPE_WRAP(( void )) >::value));
BOOST_STATIC_ASSERT((boost::is_same<  int, SAKE_IDENTITY_TYPE_WRAP((  int )) >::value));

BOOST_STATIC_ASSERT((boost::is_same< void, SAKE_IDENTITY_TYPE( void ) >::value));
BOOST_STATIC_ASSERT((boost::is_same<  int, SAKE_IDENTITY_TYPE(  int ) >::value));
BOOST_STATIC_ASSERT((boost::is_same< void, SAKE_IDENTITY_TYPE((( void ))) >::value));
BOOST_STATIC_ASSERT((boost::is_same<  int, SAKE_IDENTITY_TYPE(((  int ))) >::value));

} // namespace identity_type_private

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_IDENTITY_TYPE_HPP

/*******************************************************************************
 * sake/core/utility/identity_type.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_IDENTITY_TYPE( _T_ )
 *
 * Expands to a type identical to T but which may be passed as a parameter to a
 * preprocessor macro, where T is _T_ after stripping off the outer level of
 * parentheses (_T_ should be wrapped in parentheses).  If T is a dependent
 * type, you'll need to prefix SAKE_IDENTITY_TYPE( _T_ ) with a "typename"
 * keyword.
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_IDENTITY_TYPE_HPP
#define SAKE_CORE_UTILITY_IDENTITY_TYPE_HPP

#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>

#define SAKE_IDENTITY_TYPE( _T_ ) \
    sake::identity_type_private::impl< void ( _T_ ) >::type

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

BOOST_MPL_ASSERT((boost::is_same< void, SAKE_IDENTITY_TYPE( void ) >));
BOOST_MPL_ASSERT((boost::is_same< int, SAKE_IDENTITY_TYPE( int ) >));

} // namespace identity_type_private

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_IDENTITY_TYPE_HPP

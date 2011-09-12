/*******************************************************************************
 * core/introspection/private/builtin_has_operator_logical_not.hpp
 *
 * Copyright 2010, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_INTROSPECTION_PRIVATE_BUILTIN_HAS_OPERATOR_LOGICAL_NOT_HPP
#define SAKE_CORE_INTROSPECTION_PRIVATE_BUILTIN_HAS_OPERATOR_LOGICAL_NOT_HPP

#include <boost/mpl/and.hpp>
#include <boost/mpl/apply.hpp>

#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <boost/mpl/placeholders.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <sake/core/expr_traits/apply.hpp>
#include <sake/core/utility/declval.hpp>
#include <sake/core/utility/identity_type.hpp>

namespace sake
{

namespace introspection_private
{

template< class T, class Result, class ResultMetafunction >
struct builtin_has_operator_logical_not
    : boost::mpl::and_<
          boost_ext::is_convertible< T, bool >,
          boost_ext::is_convertible< bool, Result >,
          boost::mpl::apply1< ResultMetafunction, bool >
      >
{ };

#define test( T ) \
    BOOST_STATIC_ASSERT( SAKE_EXPR_APPLY( \
        SAKE_IDENTITY_TYPE(( boost::is_same< boost::mpl::_1, bool > )), \
        !sake::declval<T>() \
    ) );
test( bool )
test( int )
test( void* )
test( int* )
test( void (*)( ) )
#undef test

} // namespace introspection_private

} // namespace sake

#endif // #ifndef SAKE_CORE_INTROSPECTION_PRIVATE_BUILTIN_HAS_OPERATOR_LOGICAL_NOT_HPP

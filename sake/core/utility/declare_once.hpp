/*******************************************************************************
 * sake/core/utility/declare_once.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_DECLARE_ONCE_GLOBAL_DECLARATION( name )
 * #define SAKE_DECLARE_ONCE( type, name )
 *
 * This macro pair is really only useful when writing complicated macros,
 * specifically when one macro has to reference the variables in another macro,
 * but neither macro has enough common information to construct a unique name
 * with each invocation.  Thus, they must share (and, possibly, reuse) a common
 * identifier across all invocations.
 * This is a slight modification of an "awful hack" (in the words of Steven
 * Watanabe) used in Boost.ScopeExit.  See
 *     http://permalink.gmane.org/gmane.comp.lib.boost.devel/214464
 *
 * The use is relatively simple.  First, invoke the macro
 * SAKE_DECLARE_ONCE_GLOBAL_DECLARATION at global scope with the desired
 * (unique) identifier.  Now statement-level scope may have an arbitrary number
 * of SAKE_DECLARE_ONCE invocations, and only the first will actually declare
 * the desired identifier; the rest will effectively be no-ops.
 *
 * Example:
 *
 * SAKE_DECLARE_ONCE_GLOBAL_DECLARATION( xxx );
 *
 * void f()
 * {
 *     // declares an object xxx of unspecified type with value member of type
 *     // int.
 *     SAKE_DECLARE_ONCE( int, xxx );
 *     // use xxx.value to access the underlying int
 *     SAKE_DECLARE_ONCE( int, xxx ); // no-op
 *     SAKE_DECLARE_ONCE( int, xxx ); // no-op
 * }
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_DECLARE_ONCE_HPP
#define SAKE_CORE_UTILITY_DECLARE_ONCE_HPP

#include <cstddef>

#include <sake/core/utility/true_false_tag.hpp>

#define SAKE_DECLARE_ONCE_GLOBAL_DECLARATION( name ) \
    extern ::sake::declare_once_private::undeclared name

#define SAKE_DECLARE_ONCE( type, name ) \
    ::sake::declare_once_private::declared< \
        type, \
        ::sake::declare_once_private::switch_on_declared< \
            sizeof( ::sake::declare_once_private::is_declared( name ) ) \
        >::template_or_value < 0 >::value \
    > name

// The second time SAKE_DECLARE_ONCE( type, name ) is expanded in the same
// scope, the expression will be parsed differently, effectively yielding a
// no-op.
// ::sake::declare_once_private::declared<
//     type,
//     ::sake::declare_once_private::switch_on_declared<
//         sizeof( ::sake::declare_once_private::is_declared( name ) )
//     >::template_or_value < 0
// >::value > name ;

namespace sake
{

namespace declare_once_private
{

struct undeclared { };

template< class T, bool Switch = true >
struct declared
{ T value; };

template< class T >
struct declared< T, false >
{ static int const value = 0; };

sake::false_tag
is_declared(undeclared);
template< class T >
sake::true_tag
is_declared(declared<T> const &);

template< std::size_t N >
struct switch_on_declared;

template<>
struct switch_on_declared< SAKE_SIZEOF_TRUE_TAG >
{ static int const template_or_value = 0; };

template<>
struct switch_on_declared< SAKE_SIZEOF_FALSE_TAG >
{
    template< int N >
    struct template_or_value
    { static bool const value = true; };
};

template< class T >
void operator>(int, const declared<T>&)
{ }

} // namespace declare_once_private

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_DECLARE_ONCE_HPP

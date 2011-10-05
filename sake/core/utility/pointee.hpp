/*******************************************************************************
 * sake/core/utility/pointee.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct pointee<P>
 *
 * This is a metafunction giving the type the given (possibly smart) pointer
 * or iterator refers to.  This includes a const qualifier if the pointer or
 * iterator has only const access to the pointee object.
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_POINTEE_HPP
#define SAKE_CORE_UTILITY_POINTEE_HPP

#include <boost/iterator/iterator_traits.hpp>

#include <sake/boost_ext/type_traits/add_const_if.hpp>

#include <sake/core/expr_traits/is_convertible.hpp>
#include <sake/core/introspection/has_type_element_type.hpp>
#include <sake/core/introspection/has_type_value_type.hpp>
#include <sake/core/introspection/is_incrementable.hpp>
#include <sake/core/utility/declval.hpp>
#include <sake/core/utility/extension.hpp>

namespace sake
{

template< class P >
struct pointee;

namespace no_ext
{

namespace pointee_private
{

template<
    class P,
    bool = sake::is_incrementable<P>::value,      // iterators
    bool = sake::has_type_element_type<P>::value, // Boost-style smart pointers
    bool = sake::has_type_value_type<P>::value    // best guess for everything else
>
struct dispatch;

template< class P, bool HasTypeElementType, bool HasTypeValueType >
struct dispatch< P, true, HasTypeElementType, HasTypeValueType >
{
private:
    typedef typename boost::iterator_value<P>::type value_type;
    static bool const is_const = !SAKE_EXPR_IS_CONVERTIBLE( *sake::declref<P>(), value_type& );
public:
    typedef typename boost_ext::add_const_if_c< is_const, value_type >::type type;
};

template< class P, bool HasTypeValueType >
struct dispatch< P, false, true, HasTypeValueType >
{ typedef typename P::element_type type; };

template< class P >
struct dispatch< P, false, false, true >
{ typedef typename P::value_type type; };

template< class P >
struct dispatch< P, false, false, false >
// last ditch effort
{ typedef typename P::type type; };

} // namespace pointee_private

template< class P >
struct pointee
    : pointee_private::dispatch<P>
{ };

template< class T >
struct pointee< T* >
{ typedef T type; };

} // namespace no_ext

SAKE_EXTENSION_UNARY_CLASS( pointee )

template< class P >
struct pointee
    : ext::pointee<P>
{ };

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_POINTEE_HPP

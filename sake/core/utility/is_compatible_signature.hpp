/*******************************************************************************
 * sake/core/utility/is_compatible_signature.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct is_compatible_signature< R ( T0, ... ), S ( U0, ... ) >
 *
 * This metafunction evaluates to true iff if the signature R ( T0, ... ) is
 * compatible with the signature S ( U0, ... ), meaning that R is convertible to
 * S, the number of Ti is equal to the number of Ui, and each Ui is convertible
 * to the corresponding Ti.
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_IS_COMPATIBLE_SIGNATURE_HPP
#define SAKE_CORE_UTILITY_IS_COMPATIBLE_SIGNATURE_HPP

#include <boost/function_types/function_arity.hpp>
#include <boost/function_types/is_function.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/function_types/result_type.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/mpl/unpack_args.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/mpl/zip_view.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/integral_constant.hpp>

#include <sake/boost_ext/mpl/all.hpp>
#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>

namespace sake
{

template< class From, class To >
struct is_compatible_signature;

namespace is_compatible_signature_private
{

template<
    class From, class To,
    bool = (boost::function_types::function_arity< From >::value
         == boost::function_types::function_arity< To   >::value)
>
struct dispatch;

template< class From, class To >
struct dispatch< From, To, false >
    : boost::false_type
{ };

template< class From, class To >
struct dispatch< From, To, true >
{
    BOOST_STATIC_ASSERT((boost::function_types::is_function< From >::value));
    BOOST_STATIC_ASSERT((boost::function_types::is_function< To   >::value));
private:
    typedef typename boost::function_types::result_type< From >::type from_result_type;
    typedef typename boost::function_types::result_type< To   >::type   to_result_type;
    typedef typename boost::function_types::parameter_types< From >::type from_parameter_types;
    typedef typename boost::function_types::parameter_types< To   >::type   to_parameter_types;
public:
    static bool const value = boost_ext::mpl::and2<
        boost_ext::is_convertible< from_result_type, to_result_type >,
        boost_ext::mpl::all<
            boost::mpl::zip_view< boost::mpl::vector2< to_parameter_types, from_parameter_types > >,
            boost::mpl::unpack_args< boost::mpl::quote2< boost_ext::is_convertible > >
        >
    >::value;
    typedef dispatch type;
};

} // namespace is_compatible_signature_private

template< class From, class To >
struct is_compatible_signature
    : is_compatible_signature_private::dispatch< From, To >
{ };

namespace is_compatible_signature_private
{

namespace
{

#define test( from, to ) \
    BOOST_STATIC_ASSERT((is_compatible_signature< from, to >::value));
test( int ( ), int ( ) )
test( int ( ), void ( ) )
test( int ( ), long ( ) )
test( int* ( ), void* ( ) )
test( int ( int ), int ( int ) )
test( int ( int ), long ( short ) )
test( int* ( void* ), void* ( int* ) )
#undef test
#define test( from, to ) \
    BOOST_STATIC_ASSERT(!(is_compatible_signature< from, to >::value));
test( void ( ), int ( ) )
test( void* ( ), int* ( ) )
test( void ( ), void ( int ) )
test( void ( int ), void ( ) )
test( void* ( int ), int* ( int ) )
test( void ( int* ), void ( void* ) )
#undef test

} // namespace

} // namespace is_compatible_signature_private

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_IS_COMPATIBLE_SIGNATURE_HPP

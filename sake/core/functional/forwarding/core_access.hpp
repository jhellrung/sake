/*******************************************************************************
 * sake/core/functional/forwarding/core_access.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_FUNCTIONAL_FORWARDING_CORE_ACCESS_HPP
#define SAKE_CORE_FUNCTIONAL_FORWARDING_CORE_ACCESS_HPP

#include <boost/config.hpp>
#include <boost/function_types/result_type.hpp>
#include <boost/preprocessor/tuple/rem.hpp>

#include <sake/core/functional/forwarding/fwd.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/utility/overload.hpp>

namespace sake
{

namespace forwarding
{

class core_access
{
    template< class, class > friend class nullary_base;
    template< class, class, int > friend struct nullary_base_private::impl;
    template< class, class > friend class base;

    template< class Signature > // Signature = {const} Derived ( T0, ... )
    struct enable
    {
        typedef typename boost::function_types::
            result_type< Signature >::type::template
            derived_enable< Signature >::type type;
    };

    template< class Signature > // Signature = {const} Derived ( T0, ... )
    struct result
    {
        typedef typename boost::function_types::
            result_type< Signature >::type::template
            derived_result< Signature >::type type;
    };

#ifndef BOOST_NO_VARIADIC_TEMPLATES

    template< class Derived, class... T >
    typename result< Derived ( SAKE_FWD2_PARAM( T )... ) >::type
    static apply(Derived& this_, SAKE_FWD2_REF( T )... x)
    { return this_.derived_apply(sake::forward<T>(x)...); }

#else // #ifndef BOOST_NO_VARIADIC_TEMPLATES

    template< class Derived >
    static typename result< Derived ( ) >::type
    apply(Derived& this_)
    { return this_.derived_apply(); }

#define SAKE_OVERLOAD_DECLARE_TEMPLATE_PARAMS( r, n, classT_tuple ) \
    class Derived, BOOST_PP_TUPLE_REM_CTOR( n, classT_tuple )
#define SAKE_OVERLOAD_RESULT( r, n, T_tuple ) \
    result< Derived T_tuple >
#define SAKE_OVERLOAD_FUNCTION_NAME \
    static apply
#define SAKE_OVERLOAD_DECLARE_PARAMS( r, n, T_tuple, Tx_tuple ) \
    Derived& this_, BOOST_PP_TUPLE_REM_CTOR( n, Tx_tuple )
#define SAKE_OVERLOAD_BODY( r, n, T_tuple, x_tuple, forward_x_tuple ) \
    return this_.derived_apply forward_x_tuple ;
#define SAKE_OVERLOAD_FWD2_MAX_ARITY SAKE_FORWARDING_BASE_MAX_ARITY
#include SAKE_OVERLOAD_GENERATE()

#endif // #ifndef BOOST_NO_VARIADIC_TEMPLATES

};

} // namespace forwarding

} // namespace sake

#endif // #ifndef SAKE_CORE_FUNCTIONAL_FORWARDING_CORE_ACCESS_HPP

/*******************************************************************************
 * sake/core/functional/forwarding/base.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * class forwarding::base< Derived, Params = keyword::default_params >
 ******************************************************************************/

#ifndef SAKE_CORE_FUNCTIONAL_FORWARDING_BASE_HPP
#define SAKE_CORE_FUNCTIONAL_FORWARDING_BASE_HPP

#include <boost/config.hpp>
#include <boost/preprocessor/tuple/rem.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/at.hpp>
#include <sake/boost_ext/mpl/lazy_at.hpp>
#include <sake/boost_ext/type_traits/is_base_of_sans_qualifiers.hpp>

#include <sake/core/emplacer/emplacer.hpp>
#include <sake/core/functional/forwarding/fwd.hpp>
#include <sake/core/functional/forwarding/core_access.hpp>
#include <sake/core/functional/forwarding/keyword.hpp>
#include <sake/core/functional/forwarding/nullary_base.hpp>
#include <sake/core/memberwise/default_constructor.hpp>
#include <sake/core/memberwise/mem_fun.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/movable.hpp>
#include <sake/core/utility/overload.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake
{

namespace forwarding
{

template< class Derived, class Params /*= keyword::default_params*/ >
class base
    : public nullary_base< Derived, Params >
{
    typedef nullary_base< Derived, Params > nullary_base_;
protected:
    template< class Signature >
    struct enable;
public:
    SAKE_BASIC_MOVABLE_COPYABLE_MEMBERWISE(
        typename base,
        (( nullary_base_ ))
    )

    SAKE_MEMBERWISE_MEM_FUN(
        typename base,
        ( swap ) ( hash_value ),
        (( nullary_base_ ))
    )

    template< class Signature >
    struct result;

    using nullary_base_::operator();

#if !defined( BOOST_NO_RVALUE_REFERENCES ) \
 && !defined( BOOST_NO_VARIADIC_TEMPLATES )

    template< class... T >
    typename boost::lazy_enable_if_c<
        enable< Derived ( T... ) >::value,
        result< Derived ( T... ) >
    >::type
    operator()(T&&... x)
    {
        return sake::forward::core_access::apply(
            derived(), sake::forward<T>(x)...);
    }

#else // #if !defined( ... ) && !defined( ... )

#define SAKE_OVERLOAD_ENABLE( r, n, T_tuple ) \
    enable< Derived T_tuple >
#define SAKE_OVERLOAD_RESULT( r, n, T_tuple ) \
    result< Derived T_tuple >
#define SAKE_OVERLOAD_FUNCTION_NAME \
    operator()
#define SAKE_OVERLOAD_BODY( r, n, T_tuple, x_tuple, forward_x_tuple ) \
    return sake::forwarding::core_access::apply( \
        derived(), BOOST_PP_TUPLE_REM_CTOR( n, forward_x_tuple ) );
#define SAKE_OVERLOAD_PERFECT_MAX_ARITY SAKE_FORWARDING_BASE_PERFECT_MAX_ARITY
#define SAKE_OVERLOAD_FWD_MAX_ARITY     SAKE_FORWARDING_BASE_FWD_MAX_ARITY
#define SAKE_OVERLOAD_FWD2_MAX_ARITY    SAKE_FORWARDING_BASE_FWD2_MAX_ARITY
#include SAKE_OVERLOAD_GENERATE()

#endif // #if !defined( ... ) && !defined( ... )

#if !defined( BOOST_NO_RVALUE_REFERENCES ) \
 && !defined( BOOST_NO_VARIADIC_TEMPLATES )

    template< class... T >
    typename boost::lazy_enable_if_c<
        enable< Derived const ( T... ) >::value,
        result< Derived const ( T... ) >
    >::type
    operator()(T&&... x) const
    {
        return sake::forwarding::core_access::apply(
            derived(), sake::forward<T>(x)...);
    }

#else // #if !defined( ... ) && !defined( ... )

#define SAKE_OVERLOAD_ENABLE( r, n, T_tuple ) \
    enable< Derived const T_tuple >
#define SAKE_OVERLOAD_RESULT( r, n, T_tuple ) \
    result< Derived const T_tuple >
#define SAKE_OVERLOAD_FUNCTION_NAME \
    operator()
#define SAKE_OVERLOAD_DECLARATION_SUFFIX \
    const
#define SAKE_OVERLOAD_BODY( r, n, T_tuple, x_tuple, forward_x_tuple ) \
    return sake::forwarding::core_access::apply( \
        derived(), BOOST_PP_TUPLE_REM_CTOR( n, forward_x_tuple ) );
#define SAKE_OVERLOAD_PERFECT_MAX_ARITY SAKE_FORWARDING_BASE_PERFECT_MAX_ARITY
#define SAKE_OVERLOAD_FWD_MAX_ARITY     SAKE_FORWARDING_BASE_FWD_MAX_ARITY
#define SAKE_OVERLOAD_FWD2_MAX_ARITY    SAKE_FORWARDING_BASE_FWD2_MAX_ARITY
#include SAKE_OVERLOAD_GENERATE()

#endif // #if !defined( ... ) && !defined( ... )

protected:
    SAKE_USING_TYPEDEF( typename nullary_base_, chained_base_type );
    using nullary_base_::derived;

    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename base,
        (( nullary_base_ ))
    )

    template< class T >
    explicit base(SAKE_FWD2_REF( T ) x,
        typename boost::disable_if_c< boost_ext::is_base_of_sans_qualifiers<
            base, T >::value >::type* = 0);

    template< class V >
    explicit base(sake::emplacer< V ( ) >);

    friend class sake::forwarding::core_access;

    template< class Signature >
    struct derived_enable;
    template< class Signature >
    struct derived_result;
};

/*******************************************************************************
 * forwarding::base member definitions
 ******************************************************************************/

template< class Derived, class Params >
template< class T >
inline
base< Derived, Params >::
base(SAKE_FWD2_REF( T ) x,
    typename boost::disable_if_c< boost_ext::is_base_of_sans_qualifiers<
        base, T >::value >::type*)
    : nullary_base_(sake::forward<T>(x))
{ }

template< class Derived, class Params >
template< class V >
inline
base< Derived, Params >::
base(sake::emplacer< V ( ) >)
{ }

template< class Derived, class Params >
template< class Signature >
struct base< Derived, Params >::
enable
{
    typedef typename sake::forwarding::core_access::
        enable< Signature >::type type;
    static bool const value = type::value;
};

template< class Derived, class Params >
template< class Signature >
struct base< Derived, Params >::
derived_enable
    : boost::true_type
{ };

template< class Derived, class Params >
template< class Signature >
struct base< Derived, Params >::
result
    : boost_ext::mpl::lazy_at<
          Params,
          keyword::tag::result,
          sake::forwarding::core_access::result< Signature >
      >
{ };

template< class Derived, class Params >
template< class Signature >
struct base< Derived, Params >::
derived_result
    : boost_ext::mpl::at<
          Params,
          sake::forwarding::keyword::tag::result,
          void
      >
{ };

} // namespace forwarding

} // namespace sake

#endif // #ifndef SAKE_CORE_FUNCTIONAL_FORWARDING_BASE_HPP

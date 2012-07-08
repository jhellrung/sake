/*******************************************************************************
 * sake/core/data_structures/optional/private/forwarding_base.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_DATA_STRUCTURES_OPTIONAL_PRIVATE_FORWARDING_BASE_HPP
#define SAKE_CORE_DATA_STRUCTURES_OPTIONAL_PRIVATE_FORWARDING_BASE_HPP

#include <boost/config.hpp>
#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_void.hpp>

#include <sake/boost_ext/mpl/at.hpp>

#include <sake/core/data_structures/optional/fwd.hpp>
#include <sake/core/functional/default_construct.hpp>
#include <sake/core/functional/forwarding/base.hpp>
#include <sake/core/functional/forwarding/core_access.hpp>
#include <sake/core/functional/forwarding/deduced_enable.hpp>
#include <sake/core/functional/forwarding/deduced_params.hpp>
#include <sake/core/functional/forwarding/deduced_result.hpp>
#include <sake/core/functional/forwarding/keyword.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/utility/overload.hpp>
#include <sake/core/utility/template_keyword.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake
{

namespace optional_adl
{

namespace private_
{

template< class T >
class forwarding_base_params
{
    typedef typename sake::forwarding::deduced_params<T>::type deduced_params_;
    typedef typename boost_ext::mpl::at<
        deduced_params_,
        sake::forwarding::keyword::tag::result,
        void
    >::type result_type;
public:
    typedef typename sake::insert_keyword_value_if_c<
        !boost::is_void< result_type >::value,
        deduced_params_,
        sake::forwarding::keyword::result,
        sake::optional< result_type >
    >::type type;
};

template< class T >
struct forwarding_base
    : sake::forwarding::base<
          sake::optional<T>,
          typename private_::forwarding_base_params<T>::type
      >
{
private:
    typedef sake::forwarding::base<
        sake::optional<T>,
        typename private_::forwarding_base_params<T>::type
    > forwarding_base_;
protected:
    using forwarding_base_::derived;

    friend class sake::forwarding::core_access;

    template< class Signature >
    struct derived_enable
        : sake::forwarding::deduced_enable< Signature, T >
    { };

    template< class Signature >
    class derived_result
    {
        typedef typename sake::forwarding::
            deduced_result< Signature, T >::type deduced_result_;
    public:
        typedef typename boost::mpl::if_c<
            boost::is_void< deduced_result_ >::value,
            void, sake::optional< deduced_result_ >
        >::type type;
    };

    SAKE_USING_TYPEDEF(
        typename forwarding_base_, protected_nullary_result_type );

    protected_nullary_result_type derived_apply()
    {
        return derived().initialized() ?
               derived().get()() :
               sake::default_construct< protected_nullary_result_type >();
    }
    protected_nullary_result_type derived_apply() const
    {
        return derived().initialized() ?
               derived().get()() :
               sake::default_construct< protected_nullary_result_type >();
    }

#ifndef BOOST_NO_VARIADIC_TEMPLATES

    template< class... U >
    typename derived_result< sake::optional<T> ( SAKE_FWD2_PARAM( U )... ) >::type
    derived_apply(SAKE_FWD2_REF( U )... x)
    {
        typedef typename derived_result<
            sake::optional<T> ( SAKE_FWD2_PARAM( U )... )
        >::type result_type;
        return derived().initialized() ?
               derived().get()(sake::forward<U>(x)...) :
               sake::default_construct< result_type >();
    }

#else // #ifndef BOOST_NO_VARIADIC_TEMPLATES

#define SAKE_OVERLOAD_RESULT( r, n, U_tuple ) \
    derived_result< sake::optional<T> U_tuple >
#define SAKE_OVERLOAD_FUNCTION_NAME \
    derived_apply
#define SAKE_OVERLOAD_BODY( r, n, U_tuple, x_tuple, forward_x_tuple ) \
    return derived().initialized() ? \
           derived().get() forward_x_tuple : \
           sake::default_construct< typename SAKE_OVERLOAD_RESULT( r, n, U_tuple )::type >();
#define SAKE_OVERLOAD_FWD2_MAX_ARITY SAKE_FORWARDING_BASE_MAX_ARITY
#include SAKE_OVERLOAD_GENERATE()

#endif // #ifndef BOOST_NO_VARIADIC_TEMPLATES

#ifndef BOOST_NO_VARIADIC_TEMPLATES

    template< class... U >
    typename derived_result< sake::optional<T> const ( SAKE_FWD2_PARAM( U )... ) >::type
    derived_apply(SAKE_FWD2_REF( U )... x) const
    {
        typedef typename derived_result<
            sake::optional<T> const ( SAKE_FWD2_PARAM( U )... )
        >::type result_type;
        return derived().initialized() ?
               derived().get()(sake::forward<U>(x)...) :
               sake::default_construct< result_type >();
    }

#else // #ifndef BOOST_NO_VARIADIC_TEMPLATES

#define SAKE_OVERLOAD_RESULT( r, n, U_tuple ) \
    derived_result< sake::optional<T> const U_tuple >
#define SAKE_OVERLOAD_FUNCTION_NAME \
    derived_apply
#define SAKE_OVERLOAD_DECLARATION_SUFFIX \
    const
#define SAKE_OVERLOAD_BODY( r, n, U_tuple, x_tuple, forward_x_tuple ) \
    return derived().initialized() ? \
           derived().get() forward_x_tuple : \
           sake::default_construct< typename SAKE_OVERLOAD_RESULT( r, n, U_tuple )::type >();
#define SAKE_OVERLOAD_FWD2_MAX_ARITY SAKE_FORWARDING_BASE_MAX_ARITY
#include SAKE_OVERLOAD_GENERATE()

#endif // #ifndef BOOST_NO_VARIADIC_TEMPLATES

};

} // namespace private_

} // namespace optional_adl

} // namespace sake

#endif // #ifndef SAKE_CORE_DATA_STRUCTURES_OPTIONAL_PRIVATE_FORWARDING_BASE_HPP

/*******************************************************************************
 * sake/core/data_structures/optional/private/forwarding_base.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_DATA_STRUCTURES_OPTIONAL_PRIVATE_FORWARDING_BASE_HPP
#define SAKE_CORE_DATA_STRUCTURES_OPTIONAL_PRIVATE_FORWARDING_BASE_HPP

#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_void.hpp>

#include <sake/boost_ext/mpl/at.hpp>

#include <sake/core/data_structures/optional/fwd.hpp>
#include <sake/core/functional/construct.hpp>
#include <sake/core/functional/forwarding/base.hpp>
#include <sake/core/functional/forwarding/core_access.hpp>
#include <sake/core/functional/forwarding/deduced_enable.hpp>
#include <sake/core/functional/forwarding/deduced_params.hpp>
#include <sake/core/functional/forwarding/deduced_result.hpp>
#include <sake/core/functional/forwarding/keyword.hpp>
#include <sake/core/utility/overload.hpp>
#include <sake/core/utility/template_keyword.hpp>

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
    typedef typename sake::insert_keyword_value_if_not_c<
        boost::is_void< result_type >::value,
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
        typename sake::forwarding::deduced_params<T>::type
    > forwarding_base_;
protected:
    using forwarding_base_::derived;

    friend class sake::forwarding::core_access;

    template< class Signature >
    struct enable_impl
        : sake::forwarding::deduced_enable< Signature, T >
    { };

    template< class Signature >
    class result_impl
    {
        typedef typename sake::forwarding::deduced_result< Signature, T >::type deduced_result_;
    public:
        typedef typename boost::mpl::if_c<
            boost::is_void< deduced_result_ >::value,
            void,
            sake::optional< deduced_result_ >
        >::type type;
    };

private:
    typedef typename forwarding_base_::protected_nullary_result_type
        private_nullary_result_type;
public:

    private_nullary_result_type apply_impl()
    {
        return derived().initialized() ?
               derived().get()() :
               sake::construct< private_nullary_result_type >();
    }
    private_nullary_result_type apply_impl() const
    {
        return derived().initialized() ?
               derived().get()() :
               sake::construct< private_nullary_result_type >();
    }

    // template< class T0, ... >
    // typename result_impl< optional<T> ( T0, ... ) >::type
    // apply_impl(T0&& x0, ... );
#define SAKE_OVERLOAD_RESULT( r, n, T_tuple ) \
    result_impl< optional<T> T_tuple >
#define SAKE_OVERLOAD_FUNCTION_NAME \
    apply_impl
#define SAKE_OVERLOAD_BODY( r, n, T_tuple, x_tuple, forward_x_tuple ) \
    return derived().initialized() ? \
           derived().get() forward_x_tuple : \
           sake::construct< typename SAKE_OVERLOAD_RESULT( r, n, T_tuple ) ::type >();
#define SAKE_OVERLOAD_FWD2_MAX_ARITY SAKE_FORWARDING_BASE_MAX_ARITY
#include SAKE_OVERLOAD_GENERATE()

    // template< class T0, ... >
    // typename result_impl< optional<T> const ( T0, ... ) >::type
    // apply_impl(T0&& x0, ... ) const;
#define SAKE_OVERLOAD_RESULT( r, n, T_tuple ) \
    result_impl< optional<T> const T_tuple >
#define SAKE_OVERLOAD_FUNCTION_NAME \
    apply_impl
#define SAKE_OVERLOAD_DECLARATION_SUFFIX \
    const
#define SAKE_OVERLOAD_BODY( r, n, T_tuple, x_tuple, forward_x_tuple ) \
    return derived().initialized() ? \
           derived().get() forward_x_tuple : \
           sake::construct< typename SAKE_OVERLOAD_RESULT( r, n, T_tuple ) ::type >();
#define SAKE_OVERLOAD_FWD2_MAX_ARITY SAKE_FORWARDING_BASE_MAX_ARITY
#include SAKE_OVERLOAD_GENERATE()

};

} // namespace private_

} // namespace optional_adl

} // namespace sake

#endif // #ifndef SAKE_CORE_DATA_STRUCTURES_OPTIONAL_PRIVATE_FORWARDING_BASE_HPP

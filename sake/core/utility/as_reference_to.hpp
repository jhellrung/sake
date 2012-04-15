/*******************************************************************************
 * sake/core/utility/as_reference_to.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * as_reference_to< To >(From& from)
 *     -> result_of::as_reference_to< From, To >::type
 * struct functional::as_reference_to< To = void >
 *
 * This converts the given from parameter into a reference to a (possibly
 * cv-qualified) To.
 *
 * This allows (for a given (possibly cv-qualified) type T) for T*, T&, a
 * reference wrapper of T, an iterator with value_type T, a smart pointer with
 * pointee type T, an optional<T>, and compositions of the aforementioned to all
 * be treated as a handle to a T object in a generic way.
 *
 * Any UDT from which a reference to a (possibly cv-qualified) T may be
 * extracted may overload the 2-argument form of as_reference_to, and it will be
 * found via ADL.  The result_of::as_reference_to metafunction is evaluated
 * based on the overload found via ADL, so it should not be specialized.
 *
 * If result_of::as_reference_to< From, To >::type is void, then From& cannot be
 * converted into any reference to a To via as_reference_to.
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_AS_REFERENCE_TO_HPP
#define SAKE_CORE_UTILITY_AS_REFERENCE_TO_HPP

#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/is_volatile.hpp>

#include <sake/boost_ext/mpl/if.hpp>
#include <sake/boost_ext/type_traits/add_cv_if.hpp>
#include <sake/boost_ext/type_traits/is_reference.hpp>
#include <sake/boost_ext/type_traits/is_lvalue_reference.hpp>
#include <sake/boost_ext/type_traits/remove_reference.hpp>

#include <sake/core/expr_traits/apply.hpp>
#include <sake/core/expr_traits/is_convertible.hpp>
#include <sake/core/introspection/has_operator_star.hpp>
#include <sake/core/introspection/is_callable_function.hpp>
#include <sake/core/utility/declval.hpp>
#include <sake/core/utility/int_tag.hpp>
#include <sake/core/utility/is_convertible_wnrbt.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>
#include <sake/core/utility/type_tag.hpp>

namespace sake
{

namespace as_reference_to_private
{

template< class From, class To >
struct dispatch_index;

template<
    class From, class To,
    int = dispatch_index< From, To >::value
>
struct dispatch;

} // namespace as_reference_to_private

namespace result_of
{

template< class From, class To >
struct as_reference_to
    : as_reference_to_private::dispatch<
          From,
          typename sake::remove_type_tag< To >::type
      >
{ };

template< class From, class To >
struct as_reference_to< From&, To >
    : as_reference_to< From, To >
{ };

} // namespace result_of

/*******************************************************************************
 * as_reference_to< To >(From& from)
 *     -> result_of::as_reference_to< From, To >::type
 * struct functional::as_reference_to< To = void >
 ******************************************************************************/

namespace functional
{

template< class To = void >
struct as_reference_to
{
    template< class > struct result;
    template< class This, class From >
    struct result< This ( From ) >
        : sake::result_of::as_reference_to< From, To >
    { };

    template< class From >
    typename sake::result_of::as_reference_to< From, To >::type
    operator()(From& from) const
    { return as_reference_to_private::dispatch< From, To >::apply(from); }

    template< class From >
    typename sake::result_of::as_reference_to< From const, To >::type
    operator()(From const & from) const
    { return as_reference_to_private::dispatch< From const, To >::apply(from); }
};

template<>
struct as_reference_to< void >
{
    SAKE_RESULT_FROM_METAFUNCTION( sake::result_of::as_reference_to, 2 )

    template< class From, class To >
    typename sake::result_of::as_reference_to< From, To >::type
    operator()(From& from, sake::type_tag< To >) const
    { return sake::functional::as_reference_to< To >()(from); }

    template< class From, class To >
    typename sake::result_of::as_reference_to< From const, To >::type
    operator()(From const & from, sake::type_tag< To >) const
    { return sake::functional::as_reference_to< To >()(from); }
};

} // namespace functional

namespace as_reference_to_adl_barrier
{

template< class To, class From >
inline typename sake::result_of::as_reference_to< From, To >::type
as_reference_to(From& from)
{ return sake::functional::as_reference_to< To >()(from); }

template< class To, class From >
inline typename sake::result_of::as_reference_to< From const, To >::type
as_reference_to(From const & from)
{ return sake::functional::as_reference_to< To >()(from); }

} // namespace as_reference_to_adl_barrier

using namespace as_reference_to_adl_barrier;

} // namespace sake

namespace sake_as_reference_to_private
{

#define SAKE_INTROSPECTION_TRAIT_NAME    is_callable
#define SAKE_INTROSPECTION_FUNCTION_NAME as_reference_to
#define SAKE_INTROSPECTION_FUNCTION_ARITY_LIMITS ( 2, 2 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_FUNCTION()

template< class From, class To >
struct adl
{
    BOOST_STATIC_ASSERT((!::sake::boost_ext::is_reference< To >::value));
    static bool const is_lvalue_reference = SAKE_EXPR_APPLY(
        ::boost::mpl::quote1< ::sake::boost_ext::is_lvalue_reference >,
        as_reference_to(::sake::declcref< From >(), ::sake::type_tag< To >())
    );
    BOOST_STATIC_ASSERT((is_lvalue_reference));
    static bool const const_ = SAKE_EXPR_APPLY(
        ::boost::is_const< ::sake::boost_ext::remove_reference< ::boost::mpl::_1 > >,
        as_reference_to(::sake::declcref< From >(), ::sake::type_tag< To >())
    );
    static bool const volatile_ = SAKE_EXPR_APPLY(
        ::boost::is_volatile< ::sake::boost_ext::remove_reference< ::boost::mpl::_1 > >,
        as_reference_to(::sake::declcref< From >(), ::sake::type_tag< To >())
    );
    typedef typename ::sake::boost_ext::add_cv_if_c< const_, volatile_, To >::type & type;
    static type apply(From& from)
    { return as_reference_to(from, ::sake::type_tag< To >()); }
};

} // namespace sake_as_reference_to_private

namespace sake
{

namespace as_reference_to_private
{

template< class From, class To >
struct dispatch_index
{
    static int const value = boost_ext::mpl::
         if_<
        ::sake_as_reference_to_private::is_callable< void ( From&, sake::type_tag< To > ) >,
        sake::int_tag<3>
    >::type::template
    else_if <
        sake::is_convertible_wnrbt< From&, To const & >,
        sake::int_tag<2>
    >::type::template
    else_if <
        sake::is_convertible_wnrbt< From&, To const volatile & >,
        sake::int_tag<2>
    >::type::template
    else_if <
        sake::has_operator_star< From& >,
        sake::int_tag<1>
    >::type::template
    else_   <
        sake::int_tag<0>
    >::type::value;
};

template< class From, class To >
struct dispatch< From, To, 3 >
    : ::sake_as_reference_to_private::adl< From, To >
{ };

template< class From, class To >
struct dispatch< From, To, 2 >
{
    static bool const const_ = !SAKE_EXPR_IS_CONVERTIBLE(
        sake::declref< From >(), To volatile & );
    static bool const volatile_ = !SAKE_EXPR_IS_CONVERTIBLE(
        sake::declref< From >(), To const & );
    typedef typename boost_ext::add_cv_if_c< const_, volatile_, To >::type & type;
    static type apply(From& from)
    { return from; }
};

template< class From, class To >
struct dispatch< From, To, 1 >
{
    static bool const const_ = SAKE_EXPR_APPLY(
        ::boost::is_const< ::sake::boost_ext::remove_reference< ::boost::mpl::_1 > >,
        sake::functional::as_reference_to< To >()(*sake::declref< From >())
    );
    static bool const volatile_ = SAKE_EXPR_APPLY(
        ::boost::is_volatile< ::sake::boost_ext::remove_reference< ::boost::mpl::_1 > >,
        sake::functional::as_reference_to< To >()(*sake::declref< From >())
    );
    typedef typename ::sake::boost_ext::add_cv_if_c< const_, volatile_, To >::type & type;
    static type apply(From& from)
    { return sake::functional::as_reference_to< To >()(*from); }
};

template< class From, class To >
struct dispatch< From, To, 0 >
{ typedef void type; };

} // namespace as_reference_to_private

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_AS_REFERENCE_TO_HPP

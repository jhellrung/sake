/*******************************************************************************
 * sake/core/utility/as_reference_to.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * as_reference_to_< To >(From& from)
 *     -> result_of::as_reference_to< From, To >::type
 * as_reference_to(From& from, type_tag< To >)
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
 * converted into any reference to a To.
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_AS_REFERENCE_TO_HPP
#define SAKE_CORE_UTILITY_AS_REFERENCE_TO_HPP

#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/is_volatile.hpp>

#include <sake/boost_ext/type_traits/add_cv_if.hpp>
#include <sake/boost_ext/type_traits/is_convertible_wnrbt.hpp>
#include <sake/boost_ext/type_traits/is_reference.hpp>
#include <sake/boost_ext/type_traits/remove_reference.hpp>

#include <sake/core/expr_traits/apply.hpp>
#include <sake/core/expr_traits/is_convertible.hpp>
#include <sake/core/expr_traits/is_void.hpp>
#include <sake/core/introspection/has_operator_star.hpp>
#include <sake/core/utility/declval.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>
#include <sake/core/utility/type_tag.hpp>

namespace sake
{

namespace result_of
{

template< class From, class To >
struct as_reference_to;

} // namespace result_of

namespace as_reference_to_no_adl
{

namespace private_
{

template<
    class From, class To,
    bool = boost_ext::is_convertible_wnrbt< From&, To const & >::value ||
           boost_ext::is_convertible_wnrbt< From&, To const volatile & >::value,
    bool = sake::has_operator_star< From& >::value
>
struct dispatch;

} // namespace private_

template< class From, class To >
inline typename private_::dispatch< From, To >::result_type
as_reference_to(From& from, sake::type_tag< To >)
{ return private_::dispatch< From, To >::apply(from); }

} // namespace as_reference_to_no_adl

} // namespace sake

namespace sake_as_reference_to_private
{

using namespace ::sake::as_reference_to_no_adl;

template< class From, class To >
struct has_void_result
{
    // purposely unqualified call to as_reference_to to allow for ADL
    static const bool value = SAKE_EXPR_IS_VOID( as_reference_to(
        ::sake::declval_ref< From >(),
        ::sake::type_tag< To >()
    ) );
    typedef has_void_result type;
};

template< class From, class To >
struct star_has_void_result
{
    // purposely unqualified call to as_reference_to to allow for ADL
    static const bool value = SAKE_EXPR_IS_VOID( as_reference_to(
        *::sake::declval< From >(),
        ::sake::type_tag< To >()
    ) );
    typedef star_has_void_result type;
};

#define define_non_void_result_helper( expression ) \
    static const bool reference = SAKE_EXPR_APPLY( \
        ::boost::mpl::quote1< ::sake::boost_ext::is_reference >, \
        expression \
    ); \
    BOOST_STATIC_ASSERT((reference)); \
    static const bool const_ = SAKE_EXPR_APPLY( \
        ::boost::is_const< ::sake::boost_ext::remove_reference< ::boost::mpl::_1 > >, \
        expression \
    ); \
    static const bool volatile_ = SAKE_EXPR_APPLY( \
        ::boost::is_volatile< ::sake::boost_ext::remove_reference< ::boost::mpl::_1 > >, \
        expression \
    ); \
    typedef typename ::sake::boost_ext::add_cv_if_c< const_, volatile_, To >::type & type;

template< class From, class To >
struct non_void_result_helper
{
    define_non_void_result_helper(
        as_reference_to(
            ::sake::declval_ref< From >(),
            ::sake::type_tag< To >()
        )
    )
};

template< class From, class To >
struct star_non_void_result_helper
{
    define_non_void_result_helper(
        as_reference_to(
            *::sake::declval< From >(),
            ::sake::type_tag< To >()
        )
    )
};

#undef define_non_void_result_helper

template< class To, class From >
inline typename ::sake::result_of::as_reference_to< From, To >::type
impl(From& from)
{
    BOOST_STATIC_ASSERT((!::sake::boost_ext::is_reference< To >::value));
    return as_reference_to(from, ::sake::type_tag< To >());
}

} // namespace sake_as_reference_to_private

namespace sake
{

namespace result_of
{

template< class From, class To >
struct as_reference_to
    : boost::mpl::eval_if<
          ::sake_as_reference_to_private::has_void_result<
              From,
              typename sake::remove_type_tag< To >::type
          >,
          boost::mpl::identity< void >,
          ::sake_as_reference_to_private::non_void_result_helper<
              From,
              typename sake::remove_type_tag< To >::type
          >
      >
{ };

template< class From, class To >
struct as_reference_to< From&, To >
    : as_reference_to< From, To >
{ };

} // namespace result_of

namespace functional
{

template< class To = void >
struct as_reference_to
{
    template< class > struct result;
    template< class This, class From >
    struct result< This ( From ) >
        : result_of::as_reference_to< From, To >
    { };

    template< class From >
    typename result_of::as_reference_to< From, To >::type
    operator()(From& from) const
    { return ::sake_as_reference_to_private::impl< To >(from); }

    template< class From >
    typename result_of::as_reference_to< From const, To >::type
    operator()(From const & from) const
    { return ::sake_as_reference_to_private::impl< To >(from); }
};

template<>
struct as_reference_to< void >
{
    SAKE_RESULT_FROM_METAFUNCTION( 2, result_of::as_reference_to )

    template< class From, class To >
    typename result_of::as_reference_to< From, To >::type
    operator()(From& from, sake::type_tag< To >) const
    { return ::sake_as_reference_to_private::impl< To >(from); }

    template< class From, class To >
    typename result_of::as_reference_to< From const, To >::type
    operator()(From const & from, sake::type_tag< To >) const
    { return ::sake_as_reference_to_private::impl< To >(from); }
};

} // namespace functional

functional::as_reference_to<> const as_reference_to = { };

template< class To, class From >
inline typename result_of::as_reference_to< From, To >::type
as_reference_to_(From& from)
{ return ::sake_as_reference_to_private::impl< To >(from); }

template< class To, class From >
inline typename result_of::as_reference_to< From const, To >::type
as_reference_to_(From const & from)
{ return ::sake_as_reference_to_private::impl< To >(from); }

namespace as_reference_to_no_adl
{

namespace private_
{

template< class From, class To >
struct dispatch< From, To, false, false >
{
    typedef void result_type;
    static void apply(From&)
    { }
};

template< class From, class To, bool HasOperatorStar >
struct dispatch< From, To, true, HasOperatorStar >
{
    static const bool const_ = !SAKE_EXPR_IS_CONVERTIBLE(
        sake::declval_ref< From >(),
        To volatile &
    );
    static const bool volatile_ = !SAKE_EXPR_IS_CONVERTIBLE(
        sake::declval_ref< From >(),
        To const &
    );
    typedef typename boost_ext::add_cv_if_c< const_, volatile_, To >::type & result_type;
    static result_type apply(From& from)
    { return from; }
};

template< class From, class To >
struct dispatch< From, To, false, true >
{
    typedef typename boost::mpl::eval_if<
        sake_as_reference_to_private::star_has_void_result< From&, To >,
        boost::mpl::identity< void >,
        sake_as_reference_to_private::star_non_void_result_helper< From&, To >
    >::type result_type;
    static result_type apply(From& from)
    { return sake::as_reference_to_< To >(*from); }
};

} // namespace private_

} // namespace as_reference_to_no_adl

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_AS_REFERENCE_TO_HPP

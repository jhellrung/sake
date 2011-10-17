/*******************************************************************************
 * sake/core/functional/forwarding/nullary_base.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * class forwarding::nullary_base< Derived, Params = keyword::default_params >
 ******************************************************************************/

#ifndef BOOST_PP_IS_SELFISH

#ifndef SAKE_CORE_FUNCTIONAL_FORWARDING_NULLARY_BASE_HPP
#define SAKE_CORE_FUNCTIONAL_FORWARDING_NULLARY_BASE_HPP

#include <boost/mpl/at.hpp>
#include <boost/mpl/has_key.hpp>
#include <boost/mpl/not.hpp>
#include <boost/preprocessor/iteration/self.hpp>
#include <boost/preprocessor/seq/seq.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/at.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/functional/forwarding/core_access.hpp>
#include <sake/core/functional/forwarding/keyword.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/movable.hpp>
#include <sake/core/utility/emplacer_fwd.hpp>
#include <sake/core/utility/implicitly_defined_mem_fun.hpp>
#include <sake/core/utility/using_typename.hpp>
#include <sake/core/utility/void.hpp>

namespace sake
{

namespace forwarding
{

namespace nullary_base_private
{

template<
    class Derived, class Params,
    int = 1 * boost::mpl::has_key< Params, keyword::tag::result >::value
        + 2 * boost::mpl::has_key< Params, keyword::tag::nullary_callable >::value
        + 4 * boost::mpl::has_key< Params, keyword::tag::nullary_const_callable >::value
>
struct impl;

} // namespace nullary_base_private

template< class Derived, class Params /*= keyword::default_params*/ >
class nullary_base
    : public nullary_base_private::impl< Derived, Params >
{
    typedef nullary_base_private::impl< Derived, Params > impl_;
    SAKE_BASIC_MOVABLE_COPYABLE( nullary_base )
protected:
    SAKE_USING_TYPENAME( typename impl_, chained_base_type );
    SAKE_IMPLICITLY_DEFINED_MEM_FUN(
        nullary_base,
        ( default_ctor ) ( move_ctor ) ( move_assign ),
        ( impl_ ), BOOST_PP_SEQ_NIL
    )
    template< class T >
    explicit nullary_base(SAKE_FWD2_REF( T ) x)
        : impl_(sake::forward<T>(x))
    { }
};

namespace nullary_base_private
{

template< class > struct dummy;

#define n 0
#define specialized_declarations() \
    template< class T > void operator()(dummy<T>);
#define nullary_result_type void
#define BOOST_PP_INDIRECT_SELF <sake/core/functional/forwarding/nullary_base.hpp>
#include BOOST_PP_INCLUDE_SELF()

#define n 1
#define specialized_declarations() \
    typedef typename boost::mpl::at< Params, keyword::tag::result >::type result_type; \
    template< class T > void operator()(dummy<T>);
#define nullary_result_type result_type
#define BOOST_PP_INDIRECT_SELF <sake/core/functional/forwarding/nullary_base.hpp>
#include BOOST_PP_INCLUDE_SELF()

#define n 3
#define specialized_declarations() \
    typedef typename boost::mpl::at< Params, keyword::tag::result >::type result_type; \
    result_type operator()() \
    { return core_access::apply(derived()); }
#define nullary_result_type result_type
#define BOOST_PP_INDIRECT_SELF <sake/core/functional/forwarding/nullary_base.hpp>
#include BOOST_PP_INCLUDE_SELF()

#define n 5
#define specialized_declarations() \
    typedef typename boost::mpl::at< Params, keyword::tag::result >::type result_type; \
    result_type operator()() const \
    { return core_access::apply(derived()); }
#define nullary_result_type result_type
#define BOOST_PP_INDIRECT_SELF <sake/core/functional/forwarding/nullary_base.hpp>
#include BOOST_PP_INCLUDE_SELF()

#define n 7
#define specialized_declarations() \
    typedef typename boost::mpl::at< Params, keyword::tag::result >::type result_type; \
    result_type operator()() \
    { return core_access::apply(derived()); } \
    result_type operator()() const \
    { return core_access::apply(derived()); }
#define nullary_result_type result_type
#define BOOST_PP_INDIRECT_SELF <sake/core/functional/forwarding/nullary_base.hpp>
#include BOOST_PP_INCLUDE_SELF()

} // namespace nullary_base_private

} // namespace forwarding

} // namespace sake

#endif // #ifndef SAKE_CORE_FUNCTIONAL_FORWARDING_NULLARY_BASE_HPP

#else // #ifndef BOOST_PP_IS_SELFISH

template< class Derived, class Params >
struct impl< Derived, Params, n >
    : boost_ext::mpl::at< Params, keyword::tag::chained_base, sake::void_ >::type
{
    SAKE_BASIC_MOVABLE_COPYABLE( impl )
    specialized_declarations()
protected:
    friend class core_access;
    typedef Derived derived_type;
    derived_type& derived()
    { return *static_cast< derived_type* >(this); }
    derived_type const & derived() const
    { return *static_cast< derived_type const * >(this); }

    typedef typename boost_ext::mpl::at<
        Params, keyword::tag::chained_base, sake::void_
    >::type chained_base_type;

    SAKE_IMPLICITLY_DEFINED_MEM_FUN(
        typename impl,
        ( default_ctor ) ( move_ctor ) ( copy_assign_if_any_umc ) ( move_assign ),
        ( chained_base_type ), BOOST_PP_SEQ_NIL
    )

    template< class T >
    explicit impl(SAKE_FWD2_REF( T ) x,
        typename boost::disable_if_c< boost::is_base_of<
            impl,
            typename boost_ext::remove_qualifiers<T>::type
        >::value >::type* = 0)
        : chained_base_type(sake::emplacer_construct< chained_base_type >(sake::forward<T>(x)))
    { }

    typedef nullary_result_type protected_nullary_result_type;
    protected_nullary_result_type apply_impl() const;
};

#undef n
#undef specialized_declarations
#undef nullary_result_type

#endif // #ifndef BOOST_PP_IS_SELFISH

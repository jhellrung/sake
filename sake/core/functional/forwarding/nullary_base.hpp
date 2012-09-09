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
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/at.hpp>
#include <sake/boost_ext/type_traits/is_base_of_sans_qualifiers.hpp>

#include <sake/core/emplacer/constructible.hpp>
#include <sake/core/emplacer/emplacer.hpp>
#include <sake/core/functional/forwarding/core_access.hpp>
#include <sake/core/functional/forwarding/keyword.hpp>
#include <sake/core/memberwise/default_constructor.hpp>
#include <sake/core/memberwise/mem_fun.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/movable.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake
{

namespace forwarding
{

namespace nullary_base_private
{

template< class Params >
struct impl_index;

template<
    class Derived, class Params,
    int = impl_index< Params >::value
>
struct impl;

} // namespace nullary_base_private

template< class Derived, class Params /*= keyword::default_params*/ >
class nullary_base
    : public nullary_base_private::impl< Derived, Params >
{
    typedef nullary_base_private::impl< Derived, Params > impl_;
public:
    SAKE_BASIC_MOVABLE_COPYABLE_MEMBERWISE(
        typename nullary_base,
        (( impl_ ))
    )
    SAKE_MEMBERWISE_MEM_FUN(
        typename nullary_base,
        ( swap ) ( hash_value ),
        (( impl_ ))
    )
protected:
    SAKE_USING_TYPEDEF( typename impl_, chained_base_type );
    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename nullary_base,
        (( impl_ ))
    )
    template< class T >
    explicit nullary_base(SAKE_FWD2_REF( T ) x,
        typename boost::disable_if_c< boost_ext::is_base_of_sans_qualifiers<
            nullary_base, T >::value >::type* = 0)
        : impl_(sake::forward<T>(x))
    { }
    template< class V >
    explicit nullary_base(sake::emplacer< V ( ) >)
    { }
};

namespace nullary_base_private
{

template< class Params >
struct impl_index
{
    static int const value =
        1 * boost::mpl::has_key< Params, keyword::tag::result >::value
      + 2 * boost::mpl::has_key< Params, keyword::tag::nullary_callable >::value
      + 4 * boost::mpl::has_key< Params, keyword::tag::nullary_const_callable >::value;
};

struct dummy_base { };

template< class > struct dummy_param;

#define n 0
#define specialized_declarations() \
    public: template< class T > void operator()(nullary_base_private::dummy_param<T>); \
    protected: typedef void protected_nullary_result_type; \
    protected: typedef void protected_nullary_const_result_type;
#define BOOST_PP_INDIRECT_SELF <sake/core/functional/forwarding/nullary_base.hpp>
#include BOOST_PP_INCLUDE_SELF()

#define n 1
#define specialized_declarations() \
    public: typedef typename boost::mpl::at< \
            Params, sake::forwarding::keyword::tag::result \
            >::type result_type; \
    public: template< class T > void operator()(nullary_base_private::dummy_param<T>); \
    protected: typedef void protected_nullary_result_type; \
    protected: typedef void protected_nullary_const_result_type;
#define BOOST_PP_INDIRECT_SELF <sake/core/functional/forwarding/nullary_base.hpp>
#include BOOST_PP_INCLUDE_SELF()

#define n 2
#define specialized_declarations() \
    protected: typedef typename boost::mpl::at< \
               Params, sake::forwarding::keyword::tag::nullary_callable \
               >::type protected_nullary_result_type; \
    protected: typedef void protected_nullary_const_result_type; \
    public: protected_nullary_result_type operator()() \
            { return sake::forward::core_access::apply(derived()); }
#define BOOST_PP_INDIRECT_SELF <sake/core/functional/forwarding/nullary_base.hpp>
#include BOOST_PP_INCLUDE_SELF()

#define n 3
#define specialized_declarations() \
    public: typedef typename boost::mpl::at< \
            Params, sake::forwarding::keyword::tag::result \
            >::type result_type; \
    public: result_type operator()() \
            { return sake::forwarding::core_access::apply(derived()); } \
    protected: typedef result_type protected_nullary_result_type; \
    protected: typedef void protected_nullary_const_result_type;
#define BOOST_PP_INDIRECT_SELF <sake/core/functional/forwarding/nullary_base.hpp>
#include BOOST_PP_INCLUDE_SELF()

#define n 4
#define specialized_declarations() \
    protected: typedef typename boost::mpl::at< \
               Params, sake::forwarding::keyword::tag::nullary_const_callable \
               >::type protected_nullary_const_result_type; \
    protected: typedef void protected_nullary_result_type; \
    public: protected_nullary_const_result_type operator()() const \
            { return sake::forward::core_access::apply(derived()); }
#define BOOST_PP_INDIRECT_SELF <sake/core/functional/forwarding/nullary_base.hpp>
#include BOOST_PP_INCLUDE_SELF()

#define n 5
#define specialized_declarations() \
    public: typedef typename boost::mpl::at< \
            Params, sake::forwarding::keyword::tag::result \
            >::type result_type; \
    public: result_type operator()() const \
            { return sake::forwarding::core_access::apply(derived()); } \
    protected: typedef void protected_nullary_result_type; \
    protected: typedef result_type protected_nullary_const_result_type;
#define BOOST_PP_INDIRECT_SELF <sake/core/functional/forwarding/nullary_base.hpp>
#include BOOST_PP_INCLUDE_SELF()

#define n 6
#define specialized_declarations() \
    protected: typedef typename boost::mpl::at< \
               Params, sake::forwarding::keyword::tag::nullary_callable \
               >::type protected_nullary_result_type; \
    protected: typedef typename boost::mpl::at< \
               Params, sake::forwarding::keyword::tag::nullary_const_callable \
               >::type protected_nullary_const_result_type; \
    public: protected_nullary_result_type operator()() \
            { return sake::forward::core_access::apply(derived()); } \
    public: protected_nullary_const_result_type operator()() const \
            { return sake::forward::core_access::apply(derived()); }
#define BOOST_PP_INDIRECT_SELF <sake/core/functional/forwarding/nullary_base.hpp>
#include BOOST_PP_INCLUDE_SELF()

#define n 7
#define specialized_declarations() \
    public: typedef typename boost::mpl::at< \
            Params, sake::forwarding::keyword::tag::result \
            >::type result_type; \
    public: result_type operator()() \
            { return sake::forwarding::core_access::apply(derived()); } \
    public: result_type operator()() const \
            { return sake::forwarding::core_access::apply(derived()); } \
    protected: typedef result_type protected_nullary_result_type; \
    protected: typedef result_type protected_nullary_const_result_type;
#define BOOST_PP_INDIRECT_SELF <sake/core/functional/forwarding/nullary_base.hpp>
#include BOOST_PP_INCLUDE_SELF()

} // namespace nullary_base_private

} // namespace forwarding

} // namespace sake

#endif // #ifndef SAKE_CORE_FUNCTIONAL_FORWARDING_NULLARY_BASE_HPP

#else // #ifndef BOOST_PP_IS_SELFISH

template< class Derived, class Params >
struct impl< Derived, Params, n >
    : boost_ext::mpl::at<
          Params, sake::forwarding::keyword::tag::chained_base,
          nullary_base_private::dummy_base
      >::type
{
    specialized_declarations()
protected:
    typedef typename boost_ext::mpl::at<
        Params, sake::forwarding::keyword::tag::chained_base,
        nullary_base_private::dummy_base
    >::type chained_base_type;
public:
    SAKE_BASIC_MOVABLE_COPYABLE_MEMBERWISE(
        typename impl,
        (( chained_base_type ))
    )
    SAKE_MEMBERWISE_MEM_FUN(
        typename impl,
        ( swap ) ( hash_value ),
        (( chained_base_type ))
    )
protected:
    typedef Derived derived_type;
    Derived& derived()
    { return *static_cast< Derived* >(this); }
    Derived const & derived() const
    { return *static_cast< Derived const * >(this); }

    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename impl,
        (( chained_base_type ))
    )

    template< class T >
    explicit impl(SAKE_FWD2_REF( T ) x,
        typename boost::disable_if_c< boost_ext::is_base_of_sans_qualifiers<
            impl, T >::value >::type* = 0)
        : chained_base_type(sake::emplacer_constructible<
              chained_base_type >(sake::forward<T>(x)))
    { }

    friend class sake::forwarding::core_access;
};

#undef n
#undef specialized_declarations

#endif // #ifndef BOOST_PP_IS_SELFISH

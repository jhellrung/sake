/*******************************************************************************
 * sake/core/data_structures/tuple/private/value_constructor.ipp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifdef SAKE_TUPLE_INCLUDE_HEADERS

#include <boost/config.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/tuple/rem.hpp>
#include <boost/preprocessor/tuple/to_seq.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/mpl/vector.hpp>
#include <sake/boost_ext/type_traits/is_base_of_sans_qualifiers.hpp>

#include <sake/core/data_structures/tuple/fwd.hpp>
#include <sake/core/emplacer/constructible.hpp>
#include <sake/core/emplacer/emplacer.hpp>
#include <sake/core/functional/construct.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/move.hpp>
#include <sake/core/move/rv_sink.hpp>
#include <sake/core/utility/overload.hpp>
#include <sake/core/utility/value_constructor_enable.hpp>

#endif // #ifdef SAKE_TUPLE_INCLUDE_HEADERS

#ifdef SAKE_TUPLE_DEFINE_MEMBERS

#ifndef N
#error
#endif // #ifndef N

#if N == 1

private:
    template< class U0 >
    struct value_constructor_enable
        : boost_ext::mpl::and2<
              boost_ext::not_is_base_of_sans_qualifiers< tuple, U0 >,
              typename sake::value_constructor_enable< nocv0_type, U0 >
          >
    { };
    template< class U0 >
    struct value_constructor_enabler
        : boost::enable_if_c< value_constructor_enable< U0 >::value >
    { };
public:

    template< class V0 >
    explicit tuple(sake::emplacer< V0 ( ) >,
        typename value_constructor_enabler< sake::emplacer< V0 ( ) > >::type* = 0)
    { }

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U0 >
    explicit tuple(U0&& x0,
        typename value_constructor_enabler< U0 >::type* = 0)
        : _0(sake::emplacer_constructible< nocv0_type >(sake::forward< U0 >(x0)))
    { }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

private:
    typedef sake::rv_sink_traits1<
        nocv0_type,
        boost::mpl::quote1< value_constructor_enable >
    > value0_constructor_rv_sink_traits;
    typedef typename value0_constructor_rv_sink_traits::template
        default_< sake::functional::construct< nocv0_type > >
        value0_constructor_rv_sink_default_type;
public:
    // lvalues + explicit movable rvalues
    template< class U0 >
    explicit tuple(U0& x0,
        typename value0_constructor_rv_sink_traits::template
            ref_enabler< U0 >::type* = 0)
        : _0(sake::emplacer_constructible< nocv0_type >(x0))
    { }
    // implicit T0 rvalues
    explicit tuple(typename value0_constructor_rv_sink_traits::primary_type x0)
        : _0(sake::move(x0.value))
    { }
    // implicit movable rvalues
    explicit tuple(value0_constructor_rv_sink_default_type x0)
        : _0(x0())
    { }
    // const lvalues + non-movable rvalues
    template< class U0 >
    explicit tuple(U0 const & x0,
        typename value0_constructor_rv_sink_traits::template
            cref_enabler< U0 >::type* = 0)
        : _0(sake::emplacer_constructible< nocv0_type >(x0))
    { }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

#else // #if N == 1

private:
    template< class_U0N >
    struct value_constructor_enable
        : boost::mpl::equal<
              value_types,
#ifndef BOOST_NO_VARIADIC_TEMPLATES
              boost_ext::mpl::vector< U0N >,
#else // #ifndef BOOST_NO_VARIADIC_TEMPLATES
              boost::mpl::BOOST_PP_CAT( vector, N )< U0N >,
#endif // #ifndef BOOST_NO_VARIADIC_TEMPLATES
              boost::mpl::quote2< sake::value_constructor_enable >
          >
    { };
    template< class_U0N >
    struct value_constructor_enabler
        : boost::enable_if_c< value_constructor_enable< U0N >::value >
    { };
public:

#if N == 2

    template< class V0, class V1 >
    tuple(sake::emplacer< V0 ( ) >, sake::emplacer< V1 ( ) >,
        typename value_constructor_enabler<
            sake::emplacer< V0 ( ) >, sake::emplacer< V1 ( ) > >::type* = 0)
    { }

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U0, class V1 >
    tuple(U0&& x0, sake::emplacer< V1 ( ) >,
        typename value_constructor_enabler<
            U0, sake::emplacer< V1 ( ) > >::type* = 0)
        : _0(sake::emplacer_constructible< nocv0_type >(sake::forward< U0 >(x0)))
    { }

    template< class V0, class U1 >
    tuple(sake::emplacer< V0 ( ) >, U1&& x1,
        typename value_constructor_enabler<
            sake::emplacer< V0 ( ) >, U1 >::type* = 0)
        : _1(sake::emplacer_constructible< nocv1_type >(sake::forward< U1 >(x1)))
    { }

    template< class U0, class U1 >
    tuple(U0&& x0, U1&& x1,
        value_constructor_enabler< U0, U1 >::type* = 0)
        : _0(sake::emplacer_constructible< nocv0_type >(sake::forward< U0 >(x0))),
          _1(sake::emplacer_constructible< nocv1_type >(sake::forward< U1 >(x1)))
    { }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

private:
    typedef sake::rv_sink_traits1<
        nocv0_type,
        sake::value_constructor_enable< nocv0_type >
    > value0_constructor_rv_sink_traits;
    typedef typename value0_constructor_rv_sink_traits::template
        default_< sake::functional::construct< nocv0_type > >
        value0_constructor_rv_sink_default_type;
    typedef sake::rv_sink_traits1<
        nocv1_type,
        sake::value_constructor_enable< nocv1_type >
    > value1_constructor_rv_sink_traits;
    typedef typename value1_constructor_rv_sink_traits::template
        default_< sake::functional::construct< nocv1_type > >
        value1_constructor_rv_sink_default_type;
public:

    // lvalues + explicit movable rvalues
    template< class U0, class V1 >
    tuple(U0& x0, sake::emplacer< V1 ( ) >,
        typename boost::enable_if_c< boost_ext::mpl::and2<
            typename value0_constructor_rv_sink_traits::template
                ref_enable< U0 >,
            sake::value_constructor_enable<
                nocv1_type, sake::emplacer< V1 ( ) > >
        >::value >::type* = 0)
        : _0(sake::emplacer_constructible< nocv0_type >(x0))
    { }
    template< class V0, class U1 >
    tuple(sake::emplacer< V0 ( ) >, U1& x1,
        typename boost::enable_if_c< boost_ext::mpl::and2<
            sake::value_constructor_enable<
                nocv0_type, sake::emplacer< V0 ( ) > >,
            typename value1_constructor_rv_sink_traits::template
                ref_enable< U1 >
        >::value >::type* = 0)
        : _1(sake::emplacer_constructible< nocv1_type >(x1))
    { }

    // T0/T1 rvalues
    template< class V1 >
    tuple(
        typename value0_constructor_rv_sink_traits::primary_type x0,
        sake::emplacer< V1 ( ) >,
        typename sake::value_constructor_enabler<
            nocv1_type, sake::emplacer< V1 ( ) > >::type* = 0)
        : _0(sake::move(x0.value))
    { }
    template< class V0 >
    tuple(
        sake::emplacer< V0 ( ) >,
        typename value1_constructor_rv_sink_traits::primary_type x1,
        typename sake::value_constructor_enabler<
            nocv0_type, sake::emplacer< V0 ( ) > >::type* = 0)
        : _1(sake::move(x1.value))
    { }

    // movable implicit rvalues
    template< class V1 >
    tuple(
        value0_constructor_rv_sink_default_type x0,
        sake::emplacer< V1 ( ) >,
        typename sake::value_constructor_enabler<
            nocv1_type, sake::emplacer< V1 ( ) > >::type* = 0)
        : _0(x0())
    { }
    template< class V0 >
    tuple(
        sake::emplacer< V0 ( ) >,
        value1_constructor_rv_sink_default_type x1,
        typename sake::value_constructor_enabler<
            nocv0_type, sake::emplacer< V0 ( ) > >::type* = 0)
        : _1(x1())
    { }

    // const lvalues + non-movable rvalues
    template< class U0, class V1 >
    tuple(U0 const & x0, sake::emplacer< V1 ( ) >,
        typename boost::enable_if_c< boost_ext::mpl::and2<
            typename value0_constructor_rv_sink_traits::template
                cref_enable< U0 >,
            sake::value_constructor_enable<
                nocv1_type, sake::emplacer< V1 ( ) > >
        >::value >::type* = 0)
        : _0(sake::emplacer_constructible< nocv0_type >(x0))
    { }
    template< class V0, class U1 >
    tuple(sake::emplacer< V0 ( ) >, U1 const & x1,
        typename boost::enable_if_c< boost_ext::mpl::and2<
            sake::value_constructor_enable<
                nocv0_type, sake::emplacer< V0 ( ) > >,
            typename value1_constructor_rv_sink_traits::template
                cref_enable< U1 >
        >::value >::type* = 0)
        : _1(sake::emplacer_constructible< nocv1_type >(x1))
    { }

    // Okay, let's do all permutations of the above...
    template< class U0, class U1 >
    tuple(
        U0& x0,
        U1& x1,
        typename boost::enable_if_c< boost_ext::mpl::and2<
            typename value0_constructor_rv_sink_traits::template
                ref_enable< U0 >,
            typename value1_constructor_rv_sink_traits::template
                ref_enable< U1 >
        >::value >::type* = 0)
        : _0(sake::emplacer_constructible< nocv0_type >(x0)),
          _1(sake::emplacer_constructible< nocv1_type >(x1))
    { }
    template< class U0 >
    tuple(
        U0& x0,
        typename value1_constructor_rv_sink_traits::primary_type x1,
        typename value0_constructor_rv_sink_traits::template
            ref_enabler< U0 >::type* = 0)
        : _0(sake::emplacer_constructible< nocv0_type >(x0)),
          _1(sake::move(x1.value))
    { }
    template< class U0 >
    tuple(
        U0& x0,
        value1_constructor_rv_sink_default_type x1,
        typename value0_constructor_rv_sink_traits::template
            ref_enabler< U0 >::type* = 0)
        : _0(sake::emplacer_constructible< nocv0_type >(x0)),
          _1(x1())
    { }
    template< class U0, class U1 >
    tuple(
        U0& x0,
        U1 const & x1,
        typename boost::enable_if_c< boost_ext::mpl::and2<
            typename value0_constructor_rv_sink_traits::template
                ref_enable< U0 >,
            typename value1_constructor_rv_sink_traits::template
                cref_enable< U1 >
        >::value >::type* = 0)
        : _0(sake::emplacer_constructible< nocv0_type >(x0)),
          _1(sake::emplacer_constructible< nocv1_type >(x1))
    { }

    template< class U1 >
    tuple(
        typename value0_constructor_rv_sink_traits::primary_type x0,
        U1& x1,
        typename value1_constructor_rv_sink_traits::template
            ref_enabler< U1 >::type* = 0)
        : _0(sake::move(x0.value)),
          _1(sake::emplacer_constructible< nocv1_type >(x1))
    { }
    tuple(
        typename value0_constructor_rv_sink_traits::primary_type x0,
        typename value1_constructor_rv_sink_traits::primary_type x1)
        : _0(sake::move(x0.value)),
          _1(sake::move(x1.value))
    { }
    tuple(
        typename value0_constructor_rv_sink_traits::primary_type x0,
        value1_constructor_rv_sink_default_type x1)
        : _0(sake::move(x0.value)),
          _1(x1())
    { }
    template< class U1 >
    tuple(
        typename value0_constructor_rv_sink_traits::primary_type x0,
        U1 const & x1,
        typename value1_constructor_rv_sink_traits::template
            cref_enabler< U1 >::type* = 0)
        : _0(sake::move(x0.value)),
          _1(sake::emplacer_constructible< nocv1_type >(x1))
    { }

    template< class U1 >
    tuple(
        value0_constructor_rv_sink_default_type x0,
        U1& x1,
        typename value1_constructor_rv_sink_traits::template
            ref_enabler< U1 >::type* = 0)
        : _0(x0()),
          _1(sake::emplacer_constructible< nocv1_type >(x1))
    { }
    tuple(
        value0_constructor_rv_sink_default_type x0,
        typename value1_constructor_rv_sink_traits::primary_type x1)
        : _0(x0()),
          _1(sake::move(x1.value))
    { }
    tuple(
        value0_constructor_rv_sink_default_type x0,
        value1_constructor_rv_sink_default_type x1)
        : _0(x0()),
          _1(x1())
    { }
    template< class U1 >
    tuple(
        value0_constructor_rv_sink_default_type x0,
        U1 const & x1,
        typename value1_constructor_rv_sink_traits::template
            cref_enabler< U1 >::type* = 0)
        : _0(x0()),
          _1(sake::emplacer_constructible< nocv1_type >(x1))
    { }

    template< class U0, class U1 >
    tuple(
        U0 const & x0,
        U1& x1,
        typename boost::enable_if_c< boost_ext::mpl::and2<
            typename value0_constructor_rv_sink_traits::template
                cref_enable< U0 >,
            typename value1_constructor_rv_sink_traits::template
                ref_enable< U1 >
        >::value >::type* = 0)
        : _0(sake::emplacer_constructible< nocv0_type >(x0)),
          _1(sake::emplacer_constructible< nocv1_type >(x1))
    { }
    template< class U0 >
    tuple(
        U0 const & x0,
        typename value1_constructor_rv_sink_traits::primary_type x1,
        typename value0_constructor_rv_sink_traits::template
            cref_enabler< U0 >::type* = 0)
        : _0(sake::emplacer_constructible< nocv0_type >(x0)),
          _1(sake::move(x1.value))
    { }
    template< class U0 >
    tuple(
        U0 const & x0,
        value1_constructor_rv_sink_default_type x1,
        typename value0_constructor_rv_sink_traits::template
            cref_enabler< U0 >::type* = 0)
        : _0(sake::emplacer_constructible< nocv0_type >(x0)),
          _1(x1())
    { }
    template< class U0, class U1 >
    tuple(
        U0 const & x0,
        U1 const & x1,
        typename boost::enable_if_c< boost_ext::mpl::and2<
            typename value0_constructor_rv_sink_traits::template
                cref_enable< U0 >,
            typename value1_constructor_rv_sink_traits::template
            cref_enable< U1 >
        >::value >::type* = 0)
        : _0(sake::emplacer_constructible< nocv0_type >(x0)),
          _1(sake::emplacer_constructible< nocv1_type >(x1))
    { }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

#else // #if N == 2

    // template< class U0, ... >
    // tuple(U0&& x0, ... );
#define SAKE_OVERLOAD_ENABLE( r, n, T_tuple ) \
    value_constructor_enable< BOOST_PP_TUPLE_REM_CTOR( n, T_tuple ) >
#define SAKE_OVERLOAD_CONSTRUCTOR_NAME \
    tuple
#define SAKE_OVERLOAD_INITIALIZATION_LIST( r, n, T_tuple, x_tuple, forward_x_tuple ) \
    BOOST_PP_SEQ_FOR_EACH_I_R( r, \
        comma_i_emplacer_constructible_nocvi_type_elem, \
        ~, \
        BOOST_PP_TUPLE_TO_SEQ( n, forward_x_tuple ) \
    )
#define SAKE_OVERLOAD_BODY( r, n, T_tuple, x_tuple, forward_x_tuple )
#define SAKE_OVERLOAD_MIN_ARITY         N
#if N <= SAKE_TUPLE_PERFECT_MAX_ARITY
#define SAKE_OVERLOAD_PERFECT_MAX_ARITY N
#else // #if N <= SAKE_TUPLE_PERFECT_MAX_ARITY
#define SAKE_OVERLOAD_FWD_MAX_ARITY     N
#define SAKE_OVERLOAD_FWD2_MAX_ARITY    N
#endif // #if N <= SAKE_TUPLE_PERFECT_MAX_ARITY
#include SAKE_OVERLOAD_GENERATE()

#endif // #if N == 2

#endif // #if N == 1

#endif // #ifdef SAKE_TUPLE_DEFINE_MEMBERS

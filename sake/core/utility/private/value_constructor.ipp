/*******************************************************************************
 * sake/core/utility/private/value_constructor.ipp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_VALUE_CONSTRUCTOR_CLASS_NAME
#error Must define SAKE_VALUE_CONSTRUCTOR_CLASS_NAME.
#endif // #ifndef SAKE_VALUE_CONSTRUCTOR_CLASS_NAME

#ifndef SAKE_VALUE_CONSTRUCTOR_FORWARD
#error Must define SAKE_VALUE_CONSTRUCTOR_FORWARD.
#endif // #ifndef SAKE_VALUE_CONSTRUCTOR_FORWARD

#ifndef SAKE_VALUE_CONSTRUCTOR_ARITY
#error Must define SAKE_VALUE_CONSTRUCTOR_ARITY.
#endif // #ifndef SAKE_VALUE_CONSTRUCTOR_ARITY

#ifndef SAKE_VALUE_CONSTRUCTOR_T
#define SAKE_VALUE_CONSTRUCTOR_T T
#endif // #ifndef SAKE_VALUE_CONSTRUCTOR_T
#ifndef SAKE_VALUE_CONSTRUCTOR_X
#define SAKE_VALUE_CONSTRUCTOR_X x
#endif // #ifndef SAKE_VALUE_CONSTRUCTOR_X

#if   SAKE_VALUE_CONSTRUCTOR_ARITY == 1

#ifndef SAKE_VALUE_CONSTRUCTOR_TYPE0
#error Must define SAKE_VALUE_CONSTRUCTOR_TYPE0 when\
 SAKE_VALUE_CONSTRUCTOR_ARITY == 1
#endif // #ifndef SAKE_VALUE_CONSTRUCTOR_TYPE0

#define SAKE_VALUE_CONSTRUCTOR_T0 BOOST_PP_CAT( SAKE_VALUE_CONSTRUCTOR_T, 0 )
#define SAKE_VALUE_CONSTRUCTOR_x0 BOOST_PP_CAT( SAKE_VALUE_CONSTRUCTOR_X, 0 )

    template< class SAKE_VALUE_CONSTRUCTOR_T0 >
    struct _sake_value_constructor_1_enable
        : ::sake::boost_ext::mpl::and2<
              ::sake::boost_ext::not_is_base_of_sans_qualifiers<
                  SAKE_VALUE_CONSTRUCTOR_CLASS_NAME,
                  SAKE_VALUE_CONSTRUCTOR_T0
              >,
              typename ::sake::value_constructor_enable<
                  SAKE_VALUE_CONSTRUCTOR_TYPE0,
                  SAKE_VALUE_CONSTRUCTOR_T0
              >
          >
    { };
    template< class SAKE_VALUE_CONSTRUCTOR_T0 >
    struct _sake_value_constructor_1_enabler
        : ::boost::enable_if_c< _sake_value_constructor_1_enable<
              SAKE_VALUE_CONSTRUCTOR_T0 >::value >
    { };

    template< class SAKE_VALUE_CONSTRUCTOR_T0 >
    explicit SAKE_VALUE_CONSTRUCTOR_CLASS_NAME(
        ::sake::emplacer< SAKE_VALUE_CONSTRUCTOR_T0 ( ) >,
        typename _sake_value_constructor_1_enabler<
            ::sake::emplacer< SAKE_VALUE_CONSTRUCTOR_T0 ( ) > >::type* = 0)
    { }

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class SAKE_VALUE_CONSTRUCTOR_T0 >
    explicit SAKE_VALUE_CONSTRUCTOR_CLASS_NAME(
        SAKE_VALUE_CONSTRUCTOR_T0 && SAKE_VALUE_CONSTRUCTOR_X0,
        typename _sake_value_constructor_1_enabler<
            SAKE_VALUE_CONSTRUCTOR_T0 >::type* = 0)
        : SAKE_VALUE_CONSTRUCTOR_FORWARD(
              ::sake::forward< SAKE_VALUE_CONSTRUCTOR_T0 >(
                  SAKE_VALUE_CONSTRUCTOR_X0))
    { }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    typedef ::sake::rv_sink_traits1<
        SAKE_VALUE_CONSTRUCTOR_TYPE0,
        ::boost::mpl::quote1< _sake_value_constructor_1_enable >
    > _sake_value_constructor_1_rv_sink_traits;
    typedef typename _sake_value_constructor_1_rv_sink_traits::template
        default_< ::sake::functional::construct<
            SAKE_VALUE_CONSTRUCTOR_TYPE0 > >
        _sake_value_constructor_1_rv_sink_default_type;

    // lvalues + explicit movable rvalues
    template< class SAKE_VALUE_CONSTRUCTOR_T0 >
    explicit SAKE_VALUE_CONSTRUCTOR_CLASS_NAME(
        SAKE_VALUE_CONSTRUCTOR_T0 & SAKE_VALUE_CONSTRUCTOR_X0,
        typename _sake_value_constructor_1_rv_sink_traits::template
            ref_enabler< SAKE_VALUE_CONSTRUCTOR_T0 >::type* = 0)
        : SAKE_VALUE_CONSTRUCTOR_FORWARD(
              SAKE_VALUE_CONSTRUCTOR_X0)
    { }
    // implicit T0 rvalues
    explicit SAKE_VALUE_CONSTRUCTOR_CLASS_NAME(
        typename _sake_value_constructor_1_rv_sink_traits::primary_type
            SAKE_VALUE_CONSTRUCTOR_X0 )
        : SAKE_VALUE_CONSTRUCTOR_FORWARD(
              sake::move(SAKE_VALUE_CONSTRUCTOR_X0.value))
    { }
    // implicit movable rvalues
    explicit SAKE_VALUE_CONSTRUCTOR_CLASS_NAME(
        _sake_value_constructor_1_rv_sink_default_type
            SAKE_VALUE_CONSTRUCTOR_X0 )
        : SAKE_VALUE_CONSTRUCTOR_FORWARD(
              SAKE_RV_CAST( SAKE_VALUE_CONSTRUCTOR_X0() ))
    { }
    // const lvalues + non-movable rvalues
    template< class SAKE_VALUE_CONSTRUCTOR_T0 >
    explicit SAKE_VALUE_CONSTRUCTOR_CLASS_NAME(
        SAKE_VALUE_CONSTRUCTOR_T0 const & SAKE_VALUE_CONSTRUCTOR_X0,
        typename _sake_value_constructor_1_rv_sink_traits::template
            cref_enabler< SAKE_VALUE_CONSTRUCTOR_T0 >::type* = 0)
        : SAKE_VALUE_CONSTRUCTOR_FORWARD(
              SAKE_VALUE_CONSTRUCTOR_X0)
    { }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

#undef SAKE_VALUE_CONSTRUCTOR_T0
#undef SAKE_VALUE_CONSTRUCTOR_X0

#elif SAKE_VALUE_CONSTRUCTOR_ARITY == 2

#ifndef SAKE_VALUE_CONSTRUCTOR_TYPE0
#error Must define SAKE_VALUE_CONSTRUCTOR_TYPE0 when\
 SAKE_VALUE_CONSTRUCTOR_ARITY == 2
#endif // #ifndef SAKE_VALUE_CONSTRUCTOR_TYPE0
#ifndef SAKE_VALUE_CONSTRUCTOR_TYPE1
#error Must define SAKE_VALUE_CONSTRUCTOR_TYPE1 when\
 SAKE_VALUE_CONSTRUCTOR_ARITY == 2
#endif // #ifndef SAKE_VALUE_CONSTRUCTOR_TYPE1

#define SAKE_VALUE_CONSTRUCTOR_T0 BOOST_PP_CAT( SAKE_VALUE_CONSTRUCTOR_T, 0 )
#define SAKE_VALUE_CONSTRUCTOR_T1 BOOST_PP_CAT( SAKE_VALUE_CONSTRUCTOR_T, 1 )
#define SAKE_VALUE_CONSTRUCTOR_x0 BOOST_PP_CAT( SAKE_VALUE_CONSTRUCTOR_X, 0 )
#define SAKE_VALUE_CONSTRUCTOR_x1 BOOST_PP_CAT( SAKE_VALUE_CONSTRUCTOR_X, 1 )

    template<
        class SAKE_VALUE_CONSTRUCTOR_T0,
        class SAKE_VALUE_CONSTRUCTOR_T1
    >
    struct _sake_value_constructor_2_enable
        : ::sake::boost_ext::mpl::and2<
              typename ::sake::value_constructor_enable<
                  SAKE_VALUE_CONSTRUCTOR_TYPE0,
                  SAKE_VALUE_CONSTRUCTOR_T0
              >,
              typename ::sake::value_constructor_enable<
                  SAKE_VALUE_CONSTRUCTOR_TYPE1,
                  SAKE_VALUE_CONSTRUCTOR_T1
              >
          >
    { };
    template<
        class SAKE_VALUE_CONSTRUCTOR_T0,
        class SAKE_VALUE_CONSTRUCTOR_T1
    >
    struct _sake_value_constructor_2_enabler
        : ::boost::enable_if_c< _sake_value_constructor_2_enable<
              SAKE_VALUE_CONSTRUCTOR_T0,
              SAKE_VALUE_CONSTRUCTOR_T1
          >::value >
    { };

    template<
        class SAKE_VALUE_CONSTRUCTOR_T0,
        class SAKE_VALUE_CONSTRUCTOR_T1
    >
    SAKE_VALUE_CONSTRUCTOR_CLASS_NAME(
        ::sake::emplacer< SAKE_VALUE_CONSTRUCTOR_T0 ( ) >,
        ::sake::emplacer< SAKE_VALUE_CONSTRUCTOR_T0 ( ) >,
        typename _sake_value_constructor_2_enabler<
            ::sake::emplacer< SAKE_VALUE_CONSTRUCTOR_T0 ( ) >,
            ::sake::emplacer< SAKE_VALUE_CONSTRUCTOR_T1 ( ) >
        >::type* = 0)
    { }

#ifndef BOOST_NO_RVALUE_REFERENCES

    template<
        class SAKE_VALUE_CONSTRUCTOR_T0,
        class SAKE_VALUE_CONSTRUCTOR_T1
    >
    SAKE_VALUE_CONSTRUCTOR_CLASS_NAME(
        SAKE_VALUE_CONSTRUCTOR_T0 && SAKE_VALUE_CONSTRUCTOR_X0,
        SAKE_VALUE_CONSTRUCTOR_T1 && SAKE_VALUE_CONSTRUCTOR_X1,
        typename _sake_value_constructor_2_enabler<
            SAKE_VALUE_CONSTRUCTOR_T0,
            SAKE_VALUE_CONSTRUCTOR_T1
        >::type* = 0)
        : SAKE_VALUE_CONSTRUCTOR_FORWARD(
              sake::forward< SAKE_VALUE_CONSTRUCTOR_T0 >(
                  SAKE_VALUE_CONSTRUCTOR_X0),
              sake::forward< SAKE_VALUE_CONSTRUCTOR_T1 >(
                  SAKE_VALUE_CONSTRUCTOR_X1)
          )
    { }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    typedef ::sake::rv_sink_traits1<
        SAKE_VALUE_CONSTRUCTOR_TYPE0,
        ::sake::value_constructor_enable< SAKE_VALUE_CONSTRUCTOR_TYPE0 >
    > _sake_value_constructor_2_0_rv_sink_traits;
    typedef typename _sake_value_constructor_2_0_rv_sink_traits::template
        default_< ::sake::functional::construct<
            SAKE_VALUE_CONSTRUCTOR_TYPE0 > >
        _sake_value_constructor_2_0_rv_sink_default_type;
    typedef ::sake::rv_sink_traits1<
        SAKE_VALUE_CONSTRUCTOR_TYPE1,
        ::sake::value_constructor_enable< SAKE_VALUE_CONSTRUCTOR_TYPE1 >
    > _sake_value_constructor_2_1_rv_sink_traits;
    typedef typename _sake_value_constructor_2_1_rv_sink_traits::template
        default_< ::sake::functional::construct<
            SAKE_VALUE_CONSTRUCTOR_TYPE1 > >
        _sake_value_constructor_2_1_rv_sink_default_type;

    template<
        class SAKE_VALUE_CONSTRUCTOR_T0,
        class SAKE_VALUE_CONSTRUCTOR_T1
    >
    SAKE_VALUE_CONSTRUCTOR_CLASS_NAME(
        SAKE_VALUE_CONSTRUCTOR_T0 & SAKE_VALUE_CONSTRUCTOR_X0,
        SAKE_VALUE_CONSTRUCTOR_T1 & SAKE_VALUE_CONSTRUCTOR_X1,
        typename boost::enable_if_c< boost_ext::mpl::and2<
            typename _sake_value_constructor_2_0_rv_sink_traits::template
                ref_enable< SAKE_VALUE_CONSTRUCTOR_T0 >,
            typename _sake_value_constructor_2_1_rv_sink_traits::template
                ref_enable< SAKE_VALUE_CONSTRUCTOR_T1 >
        >::value >::type* = 0)
        : SAKE_VALUE_CONSTRUCTOR_FORWARD(
              SAKE_VALUE_CONSTRUCTOR_X0,
              SAKE_VALUE_CONSTRUCTOR_X1
          )
    { }
    template< class SAKE_VALUE_CONSTRUCTOR_T0 >
    SAKE_VALUE_CONSTRUCTOR_CLASS_NAME(
        SAKE_VALUE_CONSTRUCTOR_T0 & SAKE_VALUE_CONSTRUCTOR_X0,
        typename _sake_value_constructor_2_1_rv_sink_traits::primary_type
            SAKE_VALUE_CONSTRUCTOR_X1,
        typename _sake_value_constructor_2_0_rv_sink_traits::template
            ref_enabler< SAKE_VALUE_CONSTRUCTOR_T0 >::type* = 0)
        : SAKE_VALUE_CONSTRUCTOR_FORWARD(
              SAKE_VALUE_CONSTRUCTOR_X0,
              sake::move(SAKE_VALUE_CONSTRUCTOR_X1.value)
          )
    { }
    template< class SAKE_VALUE_CONSTRUCTOR_T0 >
    SAKE_VALUE_CONSTRUCTOR_CLASS_NAME(
        SAKE_VALUE_CONSTRUCTOR_T0 & SAKE_VALUE_CONSTRUCTOR_X0,
        _sake_value_constructor_2_1_rv_sink_default_type
            SAKE_VALUE_CONSTRUCTOR_X1,
        typename _sake_value_constructor_2_0_rv_sink_traits::template
            ref_enabler< SAKE_VALUE_CONSTRUCTOR_T0 >::type* = 0)
        : SAKE_VALUE_CONSTRUCTOR_FORWARD(
              SAKE_VALUE_CONSTRUCTOR_X0,
              SAKE_RV_CAST( SAKE_VALUE_CONSTRUCTOR_X1() )
          )
    { }
    template<
        class SAKE_VALUE_CONSTRUCTOR_T0,
        class SAKE_VALUE_CONSTRUCTOR_T1 
    >
    SAKE_VALUE_CONSTRUCTOR_CLASS_NAME(
        SAKE_VALUE_CONSTRUCTOR_T0 & SAKE_VALUE_CONSTRUCTOR_X0,
        SAKE_VALUE_CONSTRUCTOR_T1 const & SAKE_VALUE_CONSTRUCTOR_X1,
        typename boost::enable_if_c< boost_ext::mpl::and2<
            typename _sake_value_constructor_2_0_rv_sink_traits::template
                ref_enable< SAKE_VALUE_CONSTRUCTOR_T0 >,
            typename _sake_value_constructor_2_1_rv_sink_traits::template
                cref_enable< SAKE_VALUE_CONSTRUCTOR_T1 >
        >::value >::type* = 0)
        : SAKE_VALUE_CONSTRUCTOR_FORWARD(
              SAKE_VALUE_CONSTRUCTOR_X0,
              SAKE_VALUE_CONSTRUCTOR_X1
          )
    { }

    template< class SAKE_VALUE_CONSTRUCTOR_T1 >
    SAKE_VALUE_CONSTRUCTOR_CLASS_NAME(
        typename _sake_value_constructor_2_0_rv_sink_traits::primary_type
            SAKE_VALUE_CONSTRUCTOR_X0,
        SAKE_VALUE_CONSTRUCTOR_T1 & SAKE_VALUE_CONSTRUCTOR_X1,
        typename _sake_value_constructor_2_1_rv_sink_traits::template
            ref_enabler< SAKE_VALUE_CONSTRUCTOR_T1 >::type* = 0)
        : SAKE_VALUE_CONSTRUCTOR_FORWARD(
              sake::move(SAKE_VALUE_CONSTRUCTOR_X0.value),
              SAKE_VALUE_CONSTRUCTOR_X1
          )
    { }
    SAKE_VALUE_CONSTRUCTOR_CLASS_NAME(
        typename _sake_value_constructor_2_0_rv_sink_traits::primary_type
            SAKE_VALUE_CONSTRUCTOR_X0,
        typename _sake_value_constructor_2_1_rv_sink_traits::primary_type
            SAKE_VALUE_CONSTRUCTOR_X1)
        : SAKE_VALUE_CONSTRUCTOR_FORWARD(
              sake::move(SAKE_VALUE_CONSTRUCTOR_X0.value),
              sake::move(SAKE_VALUE_CONSTRUCTOR_X1.value)
          )
    { }
    SAKE_VALUE_CONSTRUCTOR_CLASS_NAME(
        typename _sake_value_constructor_2_0_rv_sink_traits::primary_type
            SAKE_VALUE_CONSTRUCTOR_X0,
        _sake_value_constructor_2_1_rv_sink_default_type
            SAKE_VALUE_CONSTRUCTOR_X1)
        : SAKE_VALUE_CONSTRUCTOR_FORWARD(
              sake::move(SAKE_VALUE_CONSTRUCTOR_X0.value),
              SAKE_RV_CAST( SAKE_VALUE_CONSTRUCTOR_X1() )
          )
    { }
    template< class SAKE_VALUE_CONSTRUCTOR_T1 >
    SAKE_VALUE_CONSTRUCTOR_CLASS_NAME(
        typename _sake_value_constructor_2_0_rv_sink_traits::primary_type
            SAKE_VALUE_CONSTRUCTOR_X0,
        SAKE_VALUE_CONSTRUCTOR_T1 const & SAKE_VALUE_CONSTRUCTOR_X1,
        typename _sake_value_constructor_2_1_rv_sink_traits::template
            cref_enabler< SAKE_VALUE_CONSTRUCTOR_T1 >::type* = 0)
        : SAKE_VALUE_CONSTRUCTOR_FORWARD(
              sake::move(SAKE_VALUE_CONSTRUCTOR_X0.value),
              SAKE_VALUE_CONSTRUCTOR_X1
          )
    { }

    template< class SAKE_VALUE_CONSTRUCTOR_T1 >
    SAKE_VALUE_CONSTRUCTOR_CLASS_NAME(
        _sake_value_constructor_2_0_rv_sink_default_type
            SAKE_VALUE_CONSTRUCTOR_X0,
        SAKE_VALUE_CONSTRUCTOR_T1 & SAKE_VALUE_CONSTRUCTOR_X1,
        typename _sake_value_constructor_2_1_rv_sink_traits::template
            ref_enabler< SAKE_VALUE_CONSTRUCTOR_T1 >::type* = 0)
        : SAKE_VALUE_CONSTRUCTOR_FORWARD(
              SAKE_RV_CAST( SAKE_VALUE_CONSTRUCTOR_X0() ),
              SAKE_VALUE_CONSTRUCTOR_X1
          )
    { }
    SAKE_VALUE_CONSTRUCTOR_CLASS_NAME(
        _sake_value_constructor_2_0_rv_sink_default_type
            SAKE_VALUE_CONSTRUCTOR_X0,
        typename _sake_value_constructor_2_1_rv_sink_traits::primary_type
            SAKE_VALUE_CONSTRUCTOR_X1)
        : SAKE_VALUE_CONSTRUCTOR_FORWARD(
              SAKE_RV_CAST( SAKE_VALUE_CONSTRUCTOR_X0() ),
              sake::move(SAKE_VALUE_CONSTRUCTOR_X1.value)
          )
    { }
    SAKE_VALUE_CONSTRUCTOR_CLASS_NAME(
        _sake_value_constructor_2_0_rv_sink_default_type
            SAKE_VALUE_CONSTRUCTOR_X0,
        _sake_value_constructor_2_1_rv_sink_default_type
            SAKE_VALUE_CONSTRUCTOR_X1)
        : SAKE_VALUE_CONSTRUCTOR_FORWARD(
              SAKE_RV_CAST( SAKE_VALUE_CONSTRUCTOR_X0() ),
              SAKE_RV_CAST( SAKE_VALUE_CONSTRUCTOR_X1() )
          )
    { }
    template< class SAKE_VALUE_CONSTRUCTOR_T1 >
    SAKE_VALUE_CONSTRUCTOR_CLASS_NAME(
        _sake_value_constructor_2_0_rv_sink_default_type
            SAKE_VALUE_CONSTRUCTOR_X0,
        SAKE_VALUE_CONSTRUCTOR_T1 const & SAKE_VALUE_CONSTRUCTOR_X1,
        typename _sake_value_constructor_2_1_rv_sink_traits::template
            cref_enabler< SAKE_VALUE_CONSTRUCTOR_T1 >::type* = 0)
        : SAKE_VALUE_CONSTRUCTOR_FORWARD(
              SAKE_RV_CAST( SAKE_VALUE_CONSTRUCTOR_X0() ),
              SAKE_VALUE_CONSTRUCTOR_X1
          )
    { }

    template< class SAKE_VALUE_CONSTRUCTOR_T0, class SAKE_VALUE_CONSTRUCTOR_T1 >
    SAKE_VALUE_CONSTRUCTOR_CLASS_NAME(
        SAKE_VALUE_CONSTRUCTOR_T0 const & SAKE_VALUE_CONSTRUCTOR_X0,
        SAKE_VALUE_CONSTRUCTOR_T1 & SAKE_VALUE_CONSTRUCTOR_X1,
        typename boost::enable_if_c< boost_ext::mpl::and2<
            typename _sake_value_constructor_2_0_rv_sink_traits::template
                cref_enable< SAKE_VALUE_CONSTRUCTOR_T0 >,
            typename _sake_value_constructor_2_1_rv_sink_traits::template
                ref_enable< SAKE_VALUE_CONSTRUCTOR_T1 >
        >::value >::type* = 0)
        : SAKE_VALUE_CONSTRUCTOR_FORWARD(
              SAKE_VALUE_CONSTRUCTOR_X0,
              SAKE_VALUE_CONSTRUCTOR_X1
          )
    { }
    template< class SAKE_VALUE_CONSTRUCTOR_T0 >
    SAKE_VALUE_CONSTRUCTOR_CLASS_NAME(
        SAKE_VALUE_CONSTRUCTOR_T0 const & SAKE_VALUE_CONSTRUCTOR_X0,
        typename _sake_value_constructor_2_1_rv_sink_traits::primary_type
            SAKE_VALUE_CONSTRUCTOR_X1,
        typename _sake_value_constructor_2_0_rv_sink_traits::template
            cref_enabler< SAKE_VALUE_CONSTRUCTOR_T0 >::type* = 0)
        : SAKE_VALUE_CONSTRUCTOR_FORWARD(
              SAKE_VALUE_CONSTRUCTOR_X0,
              sake::move(SAKE_VALUE_CONSTRUCTOR_X1.value)
          )
    { }
    template< class SAKE_VALUE_CONSTRUCTOR_T0 >
    SAKE_VALUE_CONSTRUCTOR_CLASS_NAME(
        SAKE_VALUE_CONSTRUCTOR_T0 const & SAKE_VALUE_CONSTRUCTOR_X0,
        _sake_value_constructor_2_1_rv_sink_default_type
            SAKE_VALUE_CONSTRUCTOR_X1,
        typename _sake_value_constructor_2_0_rv_sink_traits::template
            cref_enabler< SAKE_VALUE_CONSTRUCTOR_T0 >::type* = 0)
        : SAKE_VALUE_CONSTRUCTOR_FORWARD(
              SAKE_VALUE_CONSTRUCTOR_X0,
              SAKE_RV_CAST( SAKE_VALUE_CONSTRUCTOR_X1() )
          )
    { }
    template< class SAKE_VALUE_CONSTRUCTOR_T0, class SAKE_VALUE_CONSTRUCTOR_T1 >
    SAKE_VALUE_CONSTRUCTOR_CLASS_NAME(
        SAKE_VALUE_CONSTRUCTOR_T0 const & SAKE_VALUE_CONSTRUCTOR_X0,
        SAKE_VALUE_CONSTRUCTOR_T1 const & SAKE_VALUE_CONSTRUCTOR_X1,
        typename boost::enable_if_c< boost_ext::mpl::and2<
            typename _sake_value_constructor_2_0_rv_sink_traits::template
                cref_enable< SAKE_VALUE_CONSTRUCTOR_T0 >,
            typename _sake_value_constructor_2_1_rv_sink_traits::template
                cref_enable< SAKE_VALUE_CONSTRUCTOR_T1 >
        >::value >::type* = 0)
        : SAKE_VALUE_CONSTRUCTOR_FORWARD(
              SAKE_VALUE_CONSTRUCTOR_X0,
              SAKE_VALUE_CONSTRUCTOR_X1
          )
    { }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

#undef SAKE_VALUE_CONSTRUCTOR_T0
#undef SAKE_VALUE_CONSTRUCTOR_T1
#undef SAKE_VALUE_CONSTRUCTOR_X0
#undef SAKE_VALUE_CONSTRUCTOR_X1

#else // #if SAKE_VALUE_CONSTRUCTOR_ARITY == 1/2

#define SAKE_OVERLOAD_T SAKE_VALUE_CONSTRUCTOR_T
#define SAKE_OVERLOAD_X SAKE_VALUE_CONSTRUCTOR_X
#define SAKE_OVERLOAD_CONSTRUCTOR_NAME \
    SAKE_VALUE_CONSTRUCTOR_CLASS_NAME
#define SAKE_OVERLOAD_INITIALIZATION_LIST( r, n, U_tuple, x_tuple, forward_x_tuple ) \
    SAKE_VALUE_CONSTRUCTOR_FORWARD forward_x_tuple
#define SAKE_OVERLOAD_BODY( r, n, T_tuple, x_tuple, forward_x_tuple )
#define SAKE_OVERLOAD_MIN_ARITY         SAKE_VALUE_CONSTRUCTOR_ARITY
#if SAKE_VALUE_CONSTRUCTOR_ARITY <= SAKE_VALUE_CONSTRUCTOR_PERFECT_MAX_ARITY
#define SAKE_OVERLOAD_PERFECT_MAX_ARITY SAKE_VALUE_CONSTRUCTOR_ARITY
#else // #if SAKE_VALUE_CONSTRUCTOR_ARITY <= SAKE_VALUE_CONSTRUCTOR_PERFECT_MAX_ARITY
#define SAKE_OVERLOAD_FWD_MAX_ARITY     SAKE_VALUE_CONSTRUCTOR_ARITY
#define SAKE_OVERLOAD_FWD2_MAX_ARITY    SAKE_VALUE_CONSTRUCTOR_ARITY
#endif // #if SAKE_VALUE_CONSTRUCTOR_ARITY <= SAKE_VALUE_CONSTRUCTOR_PERFECT_MAX_ARITY
#include SAKE_OVERLOAD_GENERATE()

#endif // #if SAKE_VALUE_CONSTRUCTOR_ARITY == 1/2

#undef SAKE_VALUE_CONSTRUCTOR_T
#undef SAKE_VALUE_CONSTRUCTOR_X

#undef SAKE_VALUE_CONSTRUCTOR_TYPE0
#undef SAKE_VALUE_CONSTRUCTOR_TYPE1

#undef SAKE_VALUE_CONSTRUCTOR_ARITY
#undef SAKE_VALUE_CONSTRUCTOR_PERFECT_MAX_ARITY

#undef SAKE_VALUE_CONSTRUCTOR_CLASS_NAME
#undef SAKE_VALUE_CONSTRUCTOR_FORWARD

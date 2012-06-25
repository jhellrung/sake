/*******************************************************************************
 * sake/core/utility/private/value_constructor/value_constructor.ipp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_VALUE_CONSTRUCTOR_CLASS_NAME
#error Must define SAKE_VALUE_CONSTRUCTOR_CLASS_NAME.
#endif // #ifndef SAKE_VALUE_CONSTRUCTOR_CLASS_NAME

#ifndef SAKE_VALUE_CONSTRUCTOR_TYPES
#error Must define SAKE_VALUE_CONSTRUCTOR_TYPES.
#endif // #ifndef SAKE_VALUE_CONSTRUCTOR_TYPES

#ifndef SAKE_VALUE_CONSTRUCTOR_FORWARD
#error Must define SAKE_VALUE_CONSTRUCTOR_FORWARD.
#endif // #ifndef SAKE_VALUE_CONSTRUCTOR_FORWARD

#if   SAKE_VALUE_CONSTRUCTOR_arity == 1

    template< class _SakeValueConstructorT0 >
    struct _sake_value_constructor_1_enable
        : ::sake::boost_ext::mpl::and2<
              ::sake::boost_ext::not_is_base_of_sans_qualifiers<
                  SAKE_VALUE_CONSTRUCTOR_CLASS_NAME, _SakeValueConstructorT0 >,
              typename ::sake::value_constructor_enable<
                  SAKE_VALUE_CONSTRUCTOR_type0, _SakeValueConstructorT0 >
          >
    { };
    template< class _SakeValueConstructorT0 >
    struct _sake_value_constructor_1_enabler
        : ::boost::enable_if_c< _sake_value_constructor_1_enable<
              _SakeValueConstructorT0 >::value >
    { };

    template< class _SakeValueConstructorT0 >
    explicit SAKE_VALUE_CONSTRUCTOR_CLASS_NAME(
        ::sake::emplacer< _SakeValueConstructorT0 ( ) >,
        typename _sake_value_constructor_1_enabler<
            ::sake::emplacer< _SakeValueConstructorT0 ( ) > >::type* = 0)
    { }

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class _SakeValueConstructorT0 >
    explicit SAKE_VALUE_CONSTRUCTOR_CLASS_NAME(
        _SakeValueConstructorT0 && _sake_value_constructor_x0,
        typename _sake_value_constructor_1_enabler<
            _SakeValueConstructorT0 >::type* = 0)
        : SAKE_VALUE_CONSTRUCTOR_FORWARD(
              ::sake::forward< _SakeValueConstructorT0 >(
                  _sake_value_constructor_x0))
    { }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    typedef ::sake::rv_sink_traits1<
        SAKE_VALUE_CONSTRUCTOR_type0,
        ::boost::mpl::quote1< _sake_value_constructor_1_enable >
    > _sake_value_constructor_1_0_rv_sink_traits;
    typedef typename _sake_value_constructor_1_0_rv_sink_traits::template
        default_< ::sake::functional::construct<
            SAKE_VALUE_CONSTRUCTOR_type0 > >
        _sake_value_constructor_1_0_rv_sink_default_type;

    // lvalues + explicit movable rvalues
    template< class _SakeValueConstructorT0 >
    explicit SAKE_VALUE_CONSTRUCTOR_CLASS_NAME(
        _SakeValueConstructorT0 & _sake_value_constructor_x0,
        typename _sake_value_constructor_1_0_rv_sink_traits::template
            ref_enabler< _SakeValueConstructorT0 >::type* = 0)
        : SAKE_VALUE_CONSTRUCTOR_FORWARD(
              _sake_value_constructor_x0)
    { }
    // implicit T0 rvalues
    explicit SAKE_VALUE_CONSTRUCTOR_CLASS_NAME(
        typename _sake_value_constructor_1_0_rv_sink_traits::primary_type
            _sake_value_constructor_x0 )
        : SAKE_VALUE_CONSTRUCTOR_FORWARD(
              sake::move(_sake_value_constructor_x0.value))
    { }
    // implicit movable rvalues
    explicit SAKE_VALUE_CONSTRUCTOR_CLASS_NAME(
        _sake_value_constructor_1_0_rv_sink_default_type
            _sake_value_constructor_x0 )
        : SAKE_VALUE_CONSTRUCTOR_FORWARD(
              SAKE_RV_CAST( _sake_value_constructor_x0() ))
    { }
    // const lvalues + non-movable rvalues
    template< class _SakeValueConstructorT0 >
    explicit SAKE_VALUE_CONSTRUCTOR_CLASS_NAME(
        _SakeValueConstructorT0 const & _sake_value_constructor_x0,
        typename _sake_value_constructor_1_0_rv_sink_traits::template
            cref_enabler< _SakeValueConstructorT0 >::type* = 0)
        : SAKE_VALUE_CONSTRUCTOR_FORWARD(
              _sake_value_constructor_x0)
    { }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

#else // #if SAKE_VALUE_CONSTRUCTOR_arity == 1

    template< SAKE_VALUE_CONSTRUCTOR_class_T0N >
    struct SAKE_VALUE_CONSTRUCTOR_sake_value_constructor_N_enable
        : ::sake::boost_ext::mpl::
              BOOST_PP_CAT( and, SAKE_VALUE_CONSTRUCTOR_arity )< BOOST_PP_ENUM(
                  SAKE_VALUE_CONSTRUCTOR_arity,
                  SAKE_VALUE_CONSTRUCTOR_value_constructor_enable_typen_Tn,
                  ~
              ) >
    { };

    template< SAKE_VALUE_CONSTRUCTOR_class_T0N >
    struct SAKE_VALUE_CONSTRUCTOR_sake_value_constructor_N_enabler
        : ::boost::enable_if_c<
              SAKE_VALUE_CONSTRUCTOR_sake_value_constructor_N_enable<
                  SAKE_VALUE_CONSTRUCTOR_T0N >::value >
    { };

    template< SAKE_VALUE_CONSTRUCTOR_class_T0N >
    SAKE_VALUE_CONSTRUCTOR_CLASS_NAME(
        SAKE_VALUE_CONSTRUCTOR_emplacer_T0N,
        typename SAKE_VALUE_CONSTRUCTOR_sake_value_constructor_N_enabler<
            SAKE_VALUE_CONSTRUCTOR_emplacer_T0N >::type* = 0)
    { }

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< SAKE_VALUE_CONSTRUCTOR_class_T0N >
    SAKE_VALUE_CONSTRUCTOR_CLASS_NAME(
        BOOST_PP_ENUM_BINARY_PARAMS(
            SAKE_VALUE_CONSTRUCTOR_arity,
            _SakeValueConstructorT, && _sake_value_constructor_x
        ),
        typename SAKE_VALUE_CONSTRUCTOR_sake_value_constructor_N_enabler<
            SAKE_VALUE_CONSTRUCTOR_T0N >::type* = 0)
        : SAKE_VALUE_CONSTRUCTOR_FORWARD( BOOST_PP_ENUM(
              SAKE_VALUE_CONSTRUCTOR_arity,
              SAKE_VALUE_CONSTRUCTOR_forward_Tn_xn,
              ~
          ) )
    { }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

#if SAKE_VALUE_CONSTRUCTOR_arity == 2

    typedef ::sake::rv_sink_traits1<
        SAKE_VALUE_CONSTRUCTOR_type0,
        ::sake::value_constructor_enable< SAKE_VALUE_CONSTRUCTOR_type0 >
    > _sake_value_constructor_2_0_rv_sink_traits;
    typedef typename _sake_value_constructor_2_0_rv_sink_traits::template
        default_< ::sake::functional::construct<
            SAKE_VALUE_CONSTRUCTOR_type0 > >
        _sake_value_constructor_2_0_rv_sink_default_type;
    typedef ::sake::rv_sink_traits1<
        SAKE_VALUE_CONSTRUCTOR_type1,
        ::sake::value_constructor_enable< SAKE_VALUE_CONSTRUCTOR_type1 >
    > _sake_value_constructor_2_1_rv_sink_traits;
    typedef typename _sake_value_constructor_2_1_rv_sink_traits::template
        default_< ::sake::functional::construct<
            SAKE_VALUE_CONSTRUCTOR_type1 > >
        _sake_value_constructor_2_1_rv_sink_default_type;

    template<
        class _SakeValueConstructorT0,
        class _SakeValueConstructorT1
    >
    SAKE_VALUE_CONSTRUCTOR_CLASS_NAME(
        _SakeValueConstructorT0 & _sake_value_constructor_x0,
        _SakeValueConstructorT1 & _sake_value_constructor_x1,
        typename boost::enable_if_c< boost_ext::mpl::and2<
            typename _sake_value_constructor_2_0_rv_sink_traits::template
                ref_enable< _SakeValueConstructorT0 >,
            typename _sake_value_constructor_2_1_rv_sink_traits::template
                ref_enable< _SakeValueConstructorT1 >
        >::value >::type* = 0)
        : SAKE_VALUE_CONSTRUCTOR_FORWARD(
              _sake_value_constructor_x0,
              _sake_value_constructor_x1
          )
    { }
    template< class _SakeValueConstructorT0 >
    SAKE_VALUE_CONSTRUCTOR_CLASS_NAME(
        _SakeValueConstructorT0 & _sake_value_constructor_x0,
        typename _sake_value_constructor_2_1_rv_sink_traits::primary_type
            _sake_value_constructor_x1,
        typename _sake_value_constructor_2_0_rv_sink_traits::template
            ref_enabler< _SakeValueConstructorT0 >::type* = 0)
        : SAKE_VALUE_CONSTRUCTOR_FORWARD(
              _sake_value_constructor_x0,
              sake::move(_sake_value_constructor_x1.value)
          )
    { }
    template< class _SakeValueConstructorT0 >
    SAKE_VALUE_CONSTRUCTOR_CLASS_NAME(
        _SakeValueConstructorT0 & _sake_value_constructor_x0,
        _sake_value_constructor_2_1_rv_sink_default_type
            _sake_value_constructor_x1,
        typename _sake_value_constructor_2_0_rv_sink_traits::template
            ref_enabler< _SakeValueConstructorT0 >::type* = 0)
        : SAKE_VALUE_CONSTRUCTOR_FORWARD(
              _sake_value_constructor_x0,
              SAKE_RV_CAST( _sake_value_constructor_x1() )
          )
    { }
    template<
        class _SakeValueConstructorT0,
        class _SakeValueConstructorT1 
    >
    SAKE_VALUE_CONSTRUCTOR_CLASS_NAME(
        _SakeValueConstructorT0 & _sake_value_constructor_x0,
        _SakeValueConstructorT1 const & _sake_value_constructor_x1,
        typename boost::enable_if_c< boost_ext::mpl::and2<
            typename _sake_value_constructor_2_0_rv_sink_traits::template
                ref_enable< _SakeValueConstructorT0 >,
            typename _sake_value_constructor_2_1_rv_sink_traits::template
                cref_enable< _SakeValueConstructorT1 >
        >::value >::type* = 0)
        : SAKE_VALUE_CONSTRUCTOR_FORWARD(
              _sake_value_constructor_x0,
              _sake_value_constructor_x1
          )
    { }

    template< class _SakeValueConstructorT1 >
    SAKE_VALUE_CONSTRUCTOR_CLASS_NAME(
        typename _sake_value_constructor_2_0_rv_sink_traits::primary_type
            _sake_value_constructor_x0,
        _SakeValueConstructorT1 & _sake_value_constructor_x1,
        typename _sake_value_constructor_2_1_rv_sink_traits::template
            ref_enabler< _SakeValueConstructorT1 >::type* = 0)
        : SAKE_VALUE_CONSTRUCTOR_FORWARD(
              sake::move(_sake_value_constructor_x0.value),
              _sake_value_constructor_x1
          )
    { }
    SAKE_VALUE_CONSTRUCTOR_CLASS_NAME(
        typename _sake_value_constructor_2_0_rv_sink_traits::primary_type
            _sake_value_constructor_x0,
        typename _sake_value_constructor_2_1_rv_sink_traits::primary_type
            _sake_value_constructor_x1)
        : SAKE_VALUE_CONSTRUCTOR_FORWARD(
              sake::move(_sake_value_constructor_x0.value),
              sake::move(_sake_value_constructor_x1.value)
          )
    { }
    SAKE_VALUE_CONSTRUCTOR_CLASS_NAME(
        typename _sake_value_constructor_2_0_rv_sink_traits::primary_type
            _sake_value_constructor_x0,
        _sake_value_constructor_2_1_rv_sink_default_type
            _sake_value_constructor_x1)
        : SAKE_VALUE_CONSTRUCTOR_FORWARD(
              sake::move(_sake_value_constructor_x0.value),
              SAKE_RV_CAST( _sake_value_constructor_x1() )
          )
    { }
    template< class _SakeValueConstructorT1 >
    SAKE_VALUE_CONSTRUCTOR_CLASS_NAME(
        typename _sake_value_constructor_2_0_rv_sink_traits::primary_type
            _sake_value_constructor_x0,
        _SakeValueConstructorT1 const & _sake_value_constructor_x1,
        typename _sake_value_constructor_2_1_rv_sink_traits::template
            cref_enabler< _SakeValueConstructorT1 >::type* = 0)
        : SAKE_VALUE_CONSTRUCTOR_FORWARD(
              sake::move(_sake_value_constructor_x0.value),
              _sake_value_constructor_x1
          )
    { }

    template< class _SakeValueConstructorT1 >
    SAKE_VALUE_CONSTRUCTOR_CLASS_NAME(
        _sake_value_constructor_2_0_rv_sink_default_type
            _sake_value_constructor_x0,
        _SakeValueConstructorT1 & _sake_value_constructor_x1,
        typename _sake_value_constructor_2_1_rv_sink_traits::template
            ref_enabler< _SakeValueConstructorT1 >::type* = 0)
        : SAKE_VALUE_CONSTRUCTOR_FORWARD(
              SAKE_RV_CAST( _sake_value_constructor_x0() ),
              _sake_value_constructor_x1
          )
    { }
    SAKE_VALUE_CONSTRUCTOR_CLASS_NAME(
        _sake_value_constructor_2_0_rv_sink_default_type
            _sake_value_constructor_x0,
        typename _sake_value_constructor_2_1_rv_sink_traits::primary_type
            _sake_value_constructor_x1)
        : SAKE_VALUE_CONSTRUCTOR_FORWARD(
              SAKE_RV_CAST( _sake_value_constructor_x0() ),
              sake::move(_sake_value_constructor_x1.value)
          )
    { }
    SAKE_VALUE_CONSTRUCTOR_CLASS_NAME(
        _sake_value_constructor_2_0_rv_sink_default_type
            _sake_value_constructor_x0,
        _sake_value_constructor_2_1_rv_sink_default_type
            _sake_value_constructor_x1)
        : SAKE_VALUE_CONSTRUCTOR_FORWARD(
              SAKE_RV_CAST( _sake_value_constructor_x0() ),
              SAKE_RV_CAST( _sake_value_constructor_x1() )
          )
    { }
    template< class _SakeValueConstructorT1 >
    SAKE_VALUE_CONSTRUCTOR_CLASS_NAME(
        _sake_value_constructor_2_0_rv_sink_default_type
            _sake_value_constructor_x0,
        _SakeValueConstructorT1 const & _sake_value_constructor_x1,
        typename _sake_value_constructor_2_1_rv_sink_traits::template
            cref_enabler< _SakeValueConstructorT1 >::type* = 0)
        : SAKE_VALUE_CONSTRUCTOR_FORWARD(
              SAKE_RV_CAST( _sake_value_constructor_x0() ),
              _sake_value_constructor_x1
          )
    { }

    template< class _SakeValueConstructorT0, class _SakeValueConstructorT1 >
    SAKE_VALUE_CONSTRUCTOR_CLASS_NAME(
        _SakeValueConstructorT0 const & _sake_value_constructor_x0,
        _SakeValueConstructorT1 & _sake_value_constructor_x1,
        typename boost::enable_if_c< boost_ext::mpl::and2<
            typename _sake_value_constructor_2_0_rv_sink_traits::template
                cref_enable< _SakeValueConstructorT0 >,
            typename _sake_value_constructor_2_1_rv_sink_traits::template
                ref_enable< _SakeValueConstructorT1 >
        >::value >::type* = 0)
        : SAKE_VALUE_CONSTRUCTOR_FORWARD(
              _sake_value_constructor_x0,
              _sake_value_constructor_x1
          )
    { }
    template< class _SakeValueConstructorT0 >
    SAKE_VALUE_CONSTRUCTOR_CLASS_NAME(
        _SakeValueConstructorT0 const & _sake_value_constructor_x0,
        typename _sake_value_constructor_2_1_rv_sink_traits::primary_type
            _sake_value_constructor_x1,
        typename _sake_value_constructor_2_0_rv_sink_traits::template
            cref_enabler< _SakeValueConstructorT0 >::type* = 0)
        : SAKE_VALUE_CONSTRUCTOR_FORWARD(
              _sake_value_constructor_x0,
              sake::move(_sake_value_constructor_x1.value)
          )
    { }
    template< class _SakeValueConstructorT0 >
    SAKE_VALUE_CONSTRUCTOR_CLASS_NAME(
        _SakeValueConstructorT0 const & _sake_value_constructor_x0,
        _sake_value_constructor_2_1_rv_sink_default_type
            _sake_value_constructor_x1,
        typename _sake_value_constructor_2_0_rv_sink_traits::template
            cref_enabler< _SakeValueConstructorT0 >::type* = 0)
        : SAKE_VALUE_CONSTRUCTOR_FORWARD(
              _sake_value_constructor_x0,
              SAKE_RV_CAST( _sake_value_constructor_x1() )
          )
    { }
    template< class _SakeValueConstructorT0, class _SakeValueConstructorT1 >
    SAKE_VALUE_CONSTRUCTOR_CLASS_NAME(
        _SakeValueConstructorT0 const & _sake_value_constructor_x0,
        _SakeValueConstructorT1 const & _sake_value_constructor_x1,
        typename boost::enable_if_c< boost_ext::mpl::and2<
            typename _sake_value_constructor_2_0_rv_sink_traits::template
                cref_enable< _SakeValueConstructorT0 >,
            typename _sake_value_constructor_2_1_rv_sink_traits::template
                cref_enable< _SakeValueConstructorT1 >
        >::value >::type* = 0)
        : SAKE_VALUE_CONSTRUCTOR_FORWARD(
              _sake_value_constructor_x0,
              _sake_value_constructor_x1
          )
    { }

#else // #if SAKE_VALUE_CONSTRUCTOR_arity == 2

#define SAKE_OVERLOAD_CONSTRUCTOR_NAME \
    SAKE_VALUE_CONSTRUCTOR_CLASS_NAME
#define SAKE_OVERLOAD_ENABLE( r, n, T_tuple ) \
    SAKE_VALUE_CONSTRUCTOR_sake_value_constructor_N_enable< \
        BOOST_PP_TUPLE_REM_CTOR( n, T_tuple ) >
#define SAKE_OVERLOAD_INITIALIZATION_LIST( r, n, T_tuple, x_tuple, forward_x_tuple ) \
    SAKE_VALUE_CONSTRUCTOR_FORWARD forward_x_tuple
#define SAKE_OVERLOAD_BODY( r, n, T_tuple, x_tuple, forward_x_tuple )
#define SAKE_OVERLOAD_MIN_ARITY         SAKE_VALUE_CONSTRUCTOR_arity
#ifdef SAKE_VALUE_CONSTRUCTOR_PERFECT_FORWARDING
#define SAKE_OVERLOAD_PERFECT_MAX_ARITY SAKE_VALUE_CONSTRUCTOR_arity
#else // #ifdef SAKE_VALUE_CONSTRUCTOR_PERFECT_FORWARDING
#define SAKE_OVERLOAD_FWD_MAX_ARITY     SAKE_VALUE_CONSTRUCTOR_arity
#define SAKE_OVERLOAD_FWD2_MAX_ARITY    SAKE_VALUE_CONSTRUCTOR_arity
#endif // #ifdef SAKE_VALUE_CONSTRUCTOR_PERFECT_FORWARDING
#include SAKE_OVERLOAD_GENERATE()

#endif // #if SAKE_VALUE_CONSTRUCTOR_arity == 2

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

#endif // #if SAKE_VALUE_CONSTRUCTOR_arity == 1

#undef SAKE_VALUE_CONSTRUCTOR_CLASS_NAME
#undef SAKE_VALUE_CONSTRUCTOR_TYPES
#undef SAKE_VALUE_CONSTRUCTOR_FORWARD
#undef SAKE_VALUE_CONSTRUCTOR_PERFECT_FORWARDING

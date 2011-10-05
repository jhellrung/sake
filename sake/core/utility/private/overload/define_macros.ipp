/*******************************************************************************
 * sake/core/utility/private/overload/define_macros.ipp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/



#define SAKE_OVERLOAD_Tk( k ) BOOST_PP_CAT( SAKE_OVERLOAD_T, k )
#define SAKE_OVERLOAD_xk( k ) BOOST_PP_CAT( SAKE_OVERLOAD_X, k )

#define SAKE_OVERLOAD_forward_Tn_xn( z, n, data ) \
    ::sake::forward< SAKE_OVERLOAD_Tk( n ) >( SAKE_OVERLOAD_xk( n ) )

#define SAKE_OVERLOAD_class_T0N BOOST_PP_ENUM_PARAMS( SAKE_OVERLOAD_N, class SAKE_OVERLOAD_T )
#define SAKE_OVERLOAD_T0N       BOOST_PP_ENUM_PARAMS( SAKE_OVERLOAD_N, SAKE_OVERLOAD_T )

#ifndef BOOST_NO_RVALUE_REFERENCES

#define SAKE_OVERLOAD_xn( z, n, data ) SAKE_OVERLOAD_xk( n )
#define SAKE_OVERLOAD_T0N_x0N         BOOST_PP_ENUM_BINARY_PARAMS( SAKE_OVERLOAD_N, SAKE_OVERLOAD_T, && SAKE_OVERLOAD_X )
#define SAKE_OVERLOAD_x0N             BOOST_PP_ENUM( SAKE_OVERLOAD_N, SAKE_OVERLOAD_xn, ~ )
#define SAKE_OVERLOAD_forward_T0N_x0N BOOST_PP_ENUM( SAKE_OVERLOAD_N, SAKE_OVERLOAD_forward_Tn_xn, ~ )

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

#define SAKE_OVERLOAD_comma_remove_rvref( r, data, i, elem ) \
    BOOST_PP_COMMA_IF( i ) typename ::sake::boost_ext::remove_rvalue_reference< elem >::type
#define SAKE_OVERLOAD_comma_elem_xi( r, data, i, elem ) \
    BOOST_PP_COMMA_IF( i ) elem SAKE_OVERLOAD_xk( i )
#define SAKE_OVERLOAD_AS_LVALUE_xn( z, n, data ) \
    SAKE_BOOST_EXT_AS_LVALUE( SAKE_OVERLOAD_xk( n ) )

#define SAKE_OVERLOAD_perfect_template_param_seq( z, n, data ) \
    ( ( SAKE_OVERLOAD_Tk( n ) & ) ( SAKE_OVERLOAD_Tk( n ) const & ) )
#define SAKE_OVERLOAD_fwd_template_param( z, n, data )  ( SAKE_OVERLOAD_Tk( n ) const & )
#define SAKE_OVERLOAD_fwd2_template_param( z, n, data ) ( SAKE_OVERLOAD_Tk( n )       & )

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES



#define SAKE_OVERLOAD_param_spec( s, data, elem ) \
    ( perfect_tag, elem, default )

#define SAKE_OVERLOAD_replace_default( x, y ) \
    BOOST_PP_IIF( SAKE_BOOST_EXT_PP_KEYWORD_IS_DEFAULT( x ), y, x )



#define SAKE_OVERLOAD_tagged_param_seq_seq \
    BOOST_PP_SEQ_FOR_EACH_I( \
        SAKE_OVERLOAD_tagged_param_seq_of_param_spec, \
        ~, \
        SAKE_OVERLOAD_PARAM_SPEC_SEQ \
    )
#define SAKE_OVERLOAD_tagged_param_seq_of_param_spec( r, data, i, elem ) \
    ( BOOST_PP_CAT( \
        SAKE_OVERLOAD_tagged_param_seq_of_param_spec_, \
        BOOST_PP_TUPLE_ELEM( 3, 0, elem ) \
    ) ( \
        BOOST_PP_TUPLE_ELEM( 3, 1, elem ), \
        SAKE_OVERLOAD_replace_default( BOOST_PP_TUPLE_ELEM( 3, 2, elem ), SAKE_OVERLOAD_xk( i ) ), \
        SAKE_OVERLOAD_Tk( i ) \
    ) )

#define SAKE_OVERLOAD_tagged_param_seq_of_param_spec_ref_tag( T, x, default_T ) \
    (( ref_tag, SAKE_OVERLOAD_replace_default( T, default_T ), x, ~ ))
#define SAKE_OVERLOAD_tagged_param_seq_of_param_spec_cref_tag( T, x, default_T ) \
    (( cref_tag, SAKE_OVERLOAD_replace_default( T, default_T ), x, BOOST_PP_SEQ_NIL ))
#define SAKE_OVERLOAD_tagged_param_seq_of_param_spec_val_tag( T, x, default_T ) \
    (( val_tag, SAKE_OVERLOAD_replace_default( T, default_T ), x, ~ ))
#define SAKE_OVERLOAD_tagged_param_seq_of_param_spec_cval_tag( T, x, default_T ) \
    (( cval_tag, SAKE_OVERLOAD_replace_default( T, default_T ), x, ~ ))
#define SAKE_OVERLOAD_tagged_param_seq_of_param_spec_ptr_tag( T, x, default_T ) \
    (( ptr_tag, SAKE_OVERLOAD_replace_default( T, default_T ), x, ~ ))
#define SAKE_OVERLOAD_tagged_param_seq_of_param_spec_cptr_tag( T, x, default_T ) \
    (( cptr_tag, SAKE_OVERLOAD_replace_default( T, default_T ), x, ~ ))

#define SAKE_OVERLOAD_tagged_param_seq_of_param_spec_lit_tag( Ts, x, default_T ) \
    BOOST_PP_SEQ_TRANSFORM( SAKE_OVERLOAD_lit_tagged_param_of_param, x, Ts )
#define SAKE_OVERLOAD_lit_tagged_param_of_param( s, data, elem ) \
    ( lit_tag, elem, data, ~ )

#ifndef BOOST_NO_RVALUE_REFERENCES
#define SAKE_OVERLOAD_tagged_param_seq_of_param_spec_perfect_tag( Ts, x, default_T ) (( rv_tag, default_T, x, ~ ))
#define SAKE_OVERLOAD_tagged_param_seq_of_param_spec_fwd_tag( Ts, x, default_T )     (( rv_tag, default_T, x, ~ ))
#define SAKE_OVERLOAD_tagged_param_seq_of_param_spec_fwd2_tag( Ts, x, default_T )    (( rv_tag, default_T, x, ~ ))
#else // #ifndef BOOST_NO_RVALUE_REFERENCES
#define SAKE_OVERLOAD_tagged_param_seq_of_param_spec_perfect_tag( Ts, x, default_T ) \
    (( cref_tag, default_T, x, Ts )) \
    (( ref_tag, default_T, x, ~ )) \
    SAKE_OVERLOAD_tagged_param_seq_of_rv_params( Ts, x )
#define SAKE_OVERLOAD_tagged_param_seq_of_param_spec_fwd_tag( Ts, x, default_T ) \
    (( cref_tag, default_T, x, Ts )) \
    SAKE_OVERLOAD_tagged_param_seq_of_rv_params( Ts, x )
#define SAKE_OVERLOAD_tagged_param_seq_of_param_spec_fwd2_tag( Ts, x, default_T ) \
    (( ref_tag, default_T, x, ~ ))
#define SAKE_OVERLOAD_tagged_param_seq_of_rv_params( Ts, x ) \
    BOOST_PP_CAT( \
        SAKE_OVERLOAD_tagged_param_seq_of_rv_params_, \
        SAKE_BOOST_EXT_PP_SEQ_IS_NIL( Ts ) \
    ) ( Ts, x )
#define SAKE_OVERLOAD_tagged_param_seq_of_rv_params_1( Ts, x )
#define SAKE_OVERLOAD_tagged_param_seq_of_rv_params_0( Ts, x ) \
    BOOST_PP_SEQ_TRANSFORM( SAKE_OVERLOAD_lit_tagged_param_of_rv_param, x, Ts )
#define SAKE_OVERLOAD_lit_tagged_param_of_rv_param( s, data, elem ) \
    ( lit_tag, typename ::sake::overload_private::rv_param< elem >::type, data, ~ )
#endif // #ifndef BOOST_NO_RVALUE_REFERENCES



#ifndef BOOST_NO_RVALUE_REFERENCES
#define SAKE_OVERLOAD_T_of_tagged_param_ref_tag( T )  T &
#define SAKE_OVERLOAD_T_of_tagged_param_cref_tag( T ) T const &
#else // #ifndef BOOST_NO_RVALUE_REFERENCES
#define SAKE_OVERLOAD_T_of_tagged_param_ref_tag( T )  typename ::sake::boost_ext::remove_rvalue_reference< T & >::type
#define SAKE_OVERLOAD_T_of_tagged_param_cref_tag( T ) typename ::sake::boost_ext::remove_rvalue_reference< T const & >::type
#endif // #ifndef BOOST_NO_RVALUE_REFERENCES
#define SAKE_OVERLOAD_T_of_tagged_param_val_tag( T )  T
#define SAKE_OVERLOAD_T_of_tagged_param_cval_tag( T ) T
#define SAKE_OVERLOAD_T_of_tagged_param_ptr_tag( T )  T *
#define SAKE_OVERLOAD_T_of_tagged_param_cptr_tag( T ) T const *
#define SAKE_OVERLOAD_T_of_tagged_param_rv_tag( T )   T
#define SAKE_OVERLOAD_T_of_tagged_param_lit_tag( T )  typename ::sake::overload_private::remove_crv< T >::type
#define SAKE_OVERLOAD_T_of_tagged_param( tag, T, x, params ) \
    BOOST_PP_CAT( SAKE_OVERLOAD_T_of_tagged_param_, tag ) ( T )

#define SAKE_OVERLOAD_Tx_of_tagged_param_ref_tag( T, x )  T & x
#define SAKE_OVERLOAD_Tx_of_tagged_param_cref_tag( T, x ) T const & x
#define SAKE_OVERLOAD_Tx_of_tagged_param_val_tag( T, x )  T x
#define SAKE_OVERLOAD_Tx_of_tagged_param_cval_tag( T, x ) T const x
#define SAKE_OVERLOAD_Tx_of_tagged_param_ptr_tag( T, x )  T * x
#define SAKE_OVERLOAD_Tx_of_tagged_param_cptr_tag( T, x ) T const * x
#define SAKE_OVERLOAD_Tx_of_tagged_param_rv_tag( T, x )   T && x
#define SAKE_OVERLOAD_Tx_of_tagged_param_lit_tag( T, x )  T x
#define SAKE_OVERLOAD_Tx_of_tagged_param( tag, T, x, params ) \
    BOOST_PP_CAT( SAKE_OVERLOAD_Tx_of_tagged_param_, tag ) ( T, x )

#define SAKE_OVERLOAD_x_of_tagged_param_ref_tag( x )  SAKE_BOOST_EXT_AS_LVALUE( x )
#define SAKE_OVERLOAD_x_of_tagged_param_cref_tag( x ) SAKE_BOOST_EXT_AS_LVALUE( x )
#define SAKE_OVERLOAD_x_of_tagged_param_val_tag( x )  x
#define SAKE_OVERLOAD_x_of_tagged_param_cval_tag( x ) x
#define SAKE_OVERLOAD_x_of_tagged_param_ptr_tag( x )  x
#define SAKE_OVERLOAD_x_of_tagged_param_cptr_tag( x ) x
#define SAKE_OVERLOAD_x_of_tagged_param_rv_tag( x )   x
#define SAKE_OVERLOAD_x_of_tagged_param_lit_tag( x )  SAKE_BOOST_EXT_AS_LVALUE( x )
#define SAKE_OVERLOAD_x_of_tagged_param( tag, T, x, params ) \
    BOOST_PP_CAT( SAKE_OVERLOAD_x_of_tagged_param_, tag ) ( x )

#ifndef BOOST_NO_RVALUE_REFERENCES
#define SAKE_OVERLOAD_forward_x_of_tagged_param_ref_tag( T, x )  x
#define SAKE_OVERLOAD_forward_x_of_tagged_param_cref_tag( T, x ) x
#else // #ifndef BOOST_NO_RVALUE_REFERENCES
#define SAKE_OVERLOAD_forward_x_of_tagged_param_ref_tag( T, x )  ::sake::forward< T >( x )
#define SAKE_OVERLOAD_forward_x_of_tagged_param_cref_tag( T, x ) ::sake::forward< T >( x )
#endif // #ifndef BOOST_NO_RVALUE_REFERENCES
#define SAKE_OVERLOAD_forward_x_of_tagged_param_val_tag( T, x )  ::sake::move( x )
#define SAKE_OVERLOAD_forward_x_of_tagged_param_cval_tag( T, x ) x
#define SAKE_OVERLOAD_forward_x_of_tagged_param_ptr_tag( T, x )  x
#define SAKE_OVERLOAD_forward_x_of_tagged_param_cptr_tag( T, x ) x
#define SAKE_OVERLOAD_forward_x_of_tagged_param_rv_tag( T, x )   ::sake::forward< T >( x )
#define SAKE_OVERLOAD_forward_x_of_tagged_param_lit_tag( T, x ) \
    static_cast< typename ::sake::overload_private::add_forward_reference< T >::type >( x )
#define SAKE_OVERLOAD_forward_x_of_tagged_param( tag, T, x, params ) \
    BOOST_PP_CAT( SAKE_OVERLOAD_forward_x_of_tagged_param_, tag ) ( T, x )

#define SAKE_OVERLOAD_is_template_tagged_param_ref_tag  1
#define SAKE_OVERLOAD_is_template_tagged_param_cref_tag 1
#define SAKE_OVERLOAD_is_template_tagged_param_val_tag  1
#define SAKE_OVERLOAD_is_template_tagged_param_cval_tag 1
#define SAKE_OVERLOAD_is_template_tagged_param_ptr_tag  1
#define SAKE_OVERLOAD_is_template_tagged_param_cptr_tag 1
#define SAKE_OVERLOAD_is_template_tagged_param_rv_tag   1
#define SAKE_OVERLOAD_is_template_tagged_param_lit_tag  0
#define SAKE_OVERLOAD_is_template_tagged_param( tag, T, x, params ) \
    BOOST_PP_CAT( SAKE_OVERLOAD_is_template_tagged_param_, tag )



#define SAKE_OVERLOAD_comma_T_of_tagged_param( r, data, i, elem ) \
    BOOST_PP_COMMA_IF( i ) SAKE_OVERLOAD_T_of_tagged_param elem
#define SAKE_OVERLOAD_comma_Tx_of_tagged_param( r, data, i, elem ) \
    BOOST_PP_COMMA_IF( i ) SAKE_OVERLOAD_Tx_of_tagged_param elem
#define SAKE_OVERLOAD_comma_x_of_tagged_param( r, data, i, elem ) \
    BOOST_PP_COMMA_IF( i ) SAKE_OVERLOAD_x_of_tagged_param elem
#define SAKE_OVERLOAD_comma_forward_x_of_tagged_param( r, data, i, elem ) \
    BOOST_PP_COMMA_IF( i ) SAKE_OVERLOAD_forward_x_of_tagged_param elem



#define SAKE_OVERLOAD_has_template_tagged_param( tagged_param_seq ) \
    BOOST_PP_SEQ_FOLD_LEFT( SAKE_OVERLOAD_bitor_is_template_tagged_param, 0, tagged_param_seq )
#define SAKE_OVERLOAD_bitor_is_template_tagged_param( s, state, elem ) \
    BOOST_PP_BITOR( state, SAKE_OVERLOAD_is_template_tagged_param elem )

#define SAKE_OVERLOAD_count_template_tagged_params( tagged_param_seq ) \
    BOOST_PP_SEQ_FOLD_LEFT( SAKE_OVERLOAD_if_is_template_tagged_param_inc, 0, tagged_param_seq )
#define SAKE_OVERLOAD_if_is_template_tagged_param_inc( s, state, elem ) \
    BOOST_PP_IIF( SAKE_OVERLOAD_is_template_tagged_param elem, BOOST_PP_INC( state ), state )

#define SAKE_OVERLOAD_declare_template_params_R( r, tagged_param_seq ) \
    BOOST_PP_CAT( \
        SAKE_OVERLOAD_declare_template_params_R_, \
        SAKE_OVERLOAD_has_template_tagged_param( tagged_param_seq ) \
    ) ( r, tagged_param_seq )
#define SAKE_OVERLOAD_declare_template_params_R_0( r, tagged_param_seq )
#define SAKE_OVERLOAD_declare_template_params_R_1( r, tagged_param_seq ) \
    template< SAKE_OVERLOAD_DECLARE_TEMPLATE_PARAMS( \
        SAKE_OVERLOAD_count_template_tagged_params( tagged_param_seq ), \
        BOOST_PP_SEQ_TO_TUPLE( BOOST_PP_SEQ_FOR_EACH_R( r, \
            SAKE_OVERLOAD_if_is_template_tagged_param_class_T, \
            ~, \
            tagged_param_seq \
        ) ) \
    ) >
#define SAKE_OVERLOAD_if_is_template_tagged_param_class_T( r, data, elem ) \
    BOOST_PP_EXPR_IIF( \
        SAKE_OVERLOAD_is_template_tagged_param elem, \
        ( class BOOST_PP_TUPLE_ELEM( 4, 1, elem ) ) \
    )



#ifdef BOOST_NO_RVALUE_REFERENCES

#define SAKE_OVERLOAD_is_constrained_template_tagged_param_ref_tag( params )  0
#define SAKE_OVERLOAD_is_constrained_template_tagged_param_cref_tag( params ) \
    BOOST_PP_COMPL( SAKE_BOOST_EXT_PP_SEQ_IS_NIL( params ) )
#define SAKE_OVERLOAD_is_constrained_template_tagged_param_val_tag( params )  0
#define SAKE_OVERLOAD_is_constrained_template_tagged_param_cval_tag( params ) 0
#define SAKE_OVERLOAD_is_constrained_template_tagged_param_ptr_tag( params )  0
#define SAKE_OVERLOAD_is_constrained_template_tagged_param_cptr_tag( params ) 0
#define SAKE_OVERLOAD_is_constrained_template_tagged_param_rv_tag( params )   0
#define SAKE_OVERLOAD_is_constrained_template_tagged_param_lit_tag( params )  0
#define SAKE_OVERLOAD_is_constrained_template_tagged_param( tag, T, x, params ) \
    BOOST_PP_CAT( SAKE_OVERLOAD_is_constrained_template_tagged_param_, tag ) ( params )

#define SAKE_OVERLOAD_has_constrained_template_tagged_param( tagged_param_seq ) \
    BOOST_PP_SEQ_FOLD_LEFT( SAKE_OVERLOAD_bitor_is_constrained_template_tagged_param, 0, tagged_param_seq )
#define SAKE_OVERLOAD_bitor_is_constrained_template_tagged_param( s, state, elem ) \
    BOOST_PP_BITOR( state, SAKE_OVERLOAD_is_constrained_template_tagged_param elem )

#define SAKE_OVERLOAD_count_constrained_template_tagged_params( tagged_param_seq ) \
    BOOST_PP_SEQ_FOLD_LEFT( SAKE_OVERLOAD_if_is_constrained_template_tagged_param_inc, 0, tagged_param_seq )
#define SAKE_OVERLOAD_if_is_constrained_template_tagged_param_inc( s, state, elem ) \
    BOOST_PP_IIF( SAKE_OVERLOAD_is_constrained_template_tagged_param elem, BOOST_PP_INC( state ), state )

#define SAKE_OVERLOAD_enable_cond_R( r, tagged_param_seq ) \
    (( ::boost::mpl::not_< \
        BOOST_PP_CAT( \
            ::sake::boost_ext::mpl::or, \
            SAKE_OVERLOAD_count_constrained_template_tagged_params( tagged_param_seq ) \
        ) < )) \
            ( SAKE_BOOST_EXT_PP_SEQ_FOR_EACH_ ## r ( \
                SAKE_OVERLOAD_if_is_constrained_template_tagged_param_disable_cond_R, \
                ~, \
                tagged_param_seq \
            ) ) \
        (( > \
    > ))
#define SAKE_OVERLOAD_if_is_constrained_template_tagged_param_disable_cond_R( r, data, elem ) \
    BOOST_PP_CAT( \
        SAKE_OVERLOAD_tagged_param_disable_cond_R_, \
        SAKE_OVERLOAD_is_constrained_template_tagged_param elem \
    ) ( r, BOOST_PP_TUPLE_ELEM( 4, 1, elem ), BOOST_PP_TUPLE_ELEM( 4, 3, elem ) )
#define SAKE_OVERLOAD_tagged_param_disable_cond_R_0( r, T, params )
#define SAKE_OVERLOAD_tagged_param_disable_cond_R_1( r, T, params ) \
    SAKE_BOOST_EXT_PP_SEQ_FOR_EACH_ ## r ( \
        SAKE_OVERLOAD_is_same_sans_qualifier_T_param, \
        T, \
        params \
    )
#define SAKE_OVERLOAD_is_same_sans_qualifier_T_param( r, data, elem ) \
    ( ::sake::boost_ext::is_same_sans_qualifiers< data ) ( elem > )

#endif // #ifdef BOOST_NO_RVALUE_REFERENCES

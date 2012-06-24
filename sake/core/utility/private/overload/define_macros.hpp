/*******************************************************************************
 * sake/core/utility/private/overload/define_macros.ipp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_PRIVATE_OVERLOAD_DEFINE_MACROS_HPP
#define SAKE_CORE_UTILITY_PRIVATE_OVERLOAD_DEFINE_MACROS_HPP

#define SAKE_OVERLOAD_Tn_( n ) BOOST_PP_CAT( SAKE_OVERLOAD_T, n )
#define SAKE_OVERLOAD_xn_( n ) BOOST_PP_CAT( SAKE_OVERLOAD_X, n )

#define SAKE_OVERLOAD_forward_Tn_xn( z, n, data ) \
    ::sake::forward< SAKE_OVERLOAD_Tn_( n ) >( SAKE_OVERLOAD_xn_( n ) )

#define SAKE_OVERLOAD_class_T0N BOOST_PP_ENUM_PARAMS( SAKE_OVERLOAD_N, class SAKE_OVERLOAD_T )
#define SAKE_OVERLOAD_T0N       BOOST_PP_ENUM_PARAMS( SAKE_OVERLOAD_N, SAKE_OVERLOAD_T )

#ifndef BOOST_NO_RVALUE_REFERENCES

#define SAKE_OVERLOAD_xn( z, n, data ) SAKE_OVERLOAD_xn_( n )
#define SAKE_OVERLOAD_T0N_x0N         BOOST_PP_ENUM_BINARY_PARAMS( SAKE_OVERLOAD_N, SAKE_OVERLOAD_T, && SAKE_OVERLOAD_X )
#define SAKE_OVERLOAD_x0N             BOOST_PP_ENUM( SAKE_OVERLOAD_N, SAKE_OVERLOAD_xn, ~ )
#define SAKE_OVERLOAD_forward_T0N_x0N BOOST_PP_ENUM( SAKE_OVERLOAD_N, SAKE_OVERLOAD_forward_Tn_xn, ~ )

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

#define SAKE_OVERLOAD_comma_remove_rvref_elem( r, data, i, elem ) \
    BOOST_PP_COMMA_IF( i ) typename ::sake::boost_ext::remove_rvalue_reference< elem >::type
#define SAKE_OVERLOAD_comma_elem_xi( r, data, i, elem ) \
    BOOST_PP_COMMA_IF( i ) elem SAKE_OVERLOAD_xn_( i )
#define SAKE_OVERLOAD_AS_LVALUE_xn( z, n, data ) \
    SAKE_BOOST_EXT_AS_LVALUE( SAKE_OVERLOAD_xn_( n ) )

#define SAKE_OVERLOAD_perfect_template_param_seq( z, n, data ) \
    ( ( SAKE_OVERLOAD_Tn_( n ) & ) ( SAKE_OVERLOAD_Tn_( n ) const & ) )
#define SAKE_OVERLOAD_fwd_template_param( z, n, data ) \
    ( SAKE_OVERLOAD_Tn_( n ) const & )
#define SAKE_OVERLOAD_fwd2_template_param( z, n, data ) \
    ( SAKE_OVERLOAD_Tn_( n ) & )

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

#endif // #ifndef SAKE_CORE_UTILITY_PRIVATE_OVERLOAD_DEFINE_MACROS_HPP

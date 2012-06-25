/*******************************************************************************
 * sake/core/utility/overload.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_OVERLOAD_GENERATE()
 *
 * This provides machinery to generate a family of overloaded functions, with
 * support for capturing rvalues and perfect forwarding of lvalues in C++03.
 * One defines certain named parameter macros to define the overload set, then
 * #include's SAKE_OVERLOAD_GENERATE().
 * 
 * The macros to define to specify the desired overload set are as follows:
 *
 * SAKE_OVERLOAD_DEFINE_RESULT
 *     [ optional ]
 *     If defined, will declare and define the result struct used by
 *     Boost.ResultOf.  Should only be defined with
 *     SAKE_OVERLOAD_MEMBER_FUNCTION_NAME or
 *     SAKE_OVERLOAD_CONST_MEMBER_FUNCTION_NAME.
 * SAKE_OVERLOAD_DECLARE_RESULT_TEMPLATE_PARAMS( r, n, classT_tuple )
 *     [ defaults to "class This, BOOST_PP_TUPLE_REM_CTOR( n, classT_tuple )" ]
 *     Expands to a comma-delimited template parameter declaration of the result
 *     struct.  Should only be defined with SAKE_OVERLOAD_DEFINE_RESULT.
 * SAKE_OVERLOAD_RESULT_SPECIALIZATION( r, n, T_tuple )
 *     [ defaults to "result< This T_tuple >" ]
 *     Expands to a specialization of the result struct.  Should only be defined
 *     with SAKE_OVERLOAD_DEFINE_RESULT.
 *
 * SAKE_OVERLOAD_RESULT( r, n, T_tuple )
 *     [ optional ]
 *     Expands to an invokation of a Boost.MPL metafunction giving the result
 *     type of the overloaded function.  Should not be defined with
 *     SAKE_OVERLOAD_CONSTRUCTOR_NAME.
 * SAKE_OVERLOAD_RESULT_TYPE
 *     [ defaults to "void" ]
 *     Expands to the result type of the overloaded function.  Should not be
 *     defined with SAKE_OVERLOAD_CONSTRUCTOR_NAME.
 *
 * SAKE_OVERLOAD_DECLARE_TEMPLATE_PARAMS( r, n, classT_tuple )
 *     [ defaults to "BOOST_PP_TUPLE_REM_CTOR( n, classT_tuple )" ]
 *     Expands to a comma-delimited template parameter declaration of an
 *     overloaded function.  Should not be defined with
 *     SAKE_OVERLOAD_PARAM_SPEC_SEQ.
 * SAKE_OVERLOAD_ENABLE( r, n, T_tuple )
 *     [ optional ]
 *     Expands to an invokation of a Boost.MPL boolean metafunction indicating
 *     whether to enable this overloaded function instantiation.
 * SAKE_OVERLOAD_FUNCTION_NAME
 *     [ optional ]
 *     Expands to the name of the overloaded function.
 * SAKE_OVERLOAD_CONSTRUCTOR_NAME
 *     [ optional ]
 *     Expands to the name of the overloaded constructor.
 * SAKE_OVERLOAD_DECLARE_PARAMS( r, n, T_tuple, Tx_tuple )
 *     [ defaults to "BOOST_PP_TUPLE_REM_CTOR( n, Tx_tuple )" ]
 *     Expands to a comma-delimited list of parameter declarations for the
 *     overloaded function.
 * SAKE_OVERLOAD_DECLARATION_SUFFIX
 *     [ optional ]
 *     Expands to any suffix qualifiers (e.g., "const", "volatile") or
 *     attributes.
 * SAKE_OVERLOAD_INITIALIZATION_LIST( r, n, T_tuple, x_tuple, forward_x_tuple )
 *     [ optional ]
 *     Expands to a comma-delimited initialization list (excluding the ":").
 *     Should only be defined with SAKE_OVERLOAD_CONSTRUCTOR_NAME.
 * SAKE_OVERLOAD_BODY( r, n, T_tuple, x_tuple, forward_x_tuple )
 *     Expands to the body of the overloaded function.
 *
 * SAKE_OVERLOAD_MIN_ARITY
 *     [ defaults to "1" ]
 *     Expands to an integer giving the minimum arity of the overloaded
 *     functions.
 * SAKE_OVERLOAD_PERFECT_MAX_ARITY
 *     [ optional ]
 *     Expands to an integer giving the maximum arity of the overloaded
 *     functions with "perfect" forwarding.  When BOOST_NO_RVALUE_REFERENCES is
 *     defined, an overload set with "perfect" forwarding have all combinations
 *     of templated reference-to-const and reference-to-non-const parameters,
 *     resulting in 2^(N+1)-2 generated overloads for a maximum arity N.
 * SAKE_OVERLOAD_FWD_MAX_ARITY
 *     [ optional ]
 *     Expands to an integer giving the maximum arity of the
 *     "SAKE_FWD_REF( T[n] )" overloads.  When BOOST_NO_RVALUE_REFERENCES is
 *     defined, this is the same as "T[n] const &".  Thus, it can capture
 *     rvalues, but it does not preserve const-ness of lvalues.
 * SAKE_OVERLOAD_FWD2_MAX_ARITY
 *     [ optional ]
 *     Expands to an integer giving the maximum arity of the
 *     "SAKE_FWD2_REF( T[n] )" overloads.  When BOOST_NO_RVALUE_REFERENCES is
 *     defined, this is the same as "T[n] &".  Thus, it preserves const-ness of
 *     lvalues, but it cannot capture rvalues.
 *
 * Note: At most one of SAKE_OVERLOAD_RESULT and SAKE_OVERLOAD_RESULT_TYPE
 *       should be defined.
 * Note: Exactly one of the SAKE_OVERLOAD_XXX_NAME macros must be defined.
 * Note: For the SAKE_OVERLOAD_XXX_NAME macros, the overload name should include
 *       any prefix qualifiers (e.g., "static", "explicit") or attributes.
 * Note: The SAKE_OVERLOAD_XXX_ARITY macros don't, strictly speaking, describe
 *       the arity limits of the overload set, but rather the size limits on the
 *       tuples passed to the above macros.  This will typically be identical to
 *       the arity limits on the overload set, but defining
 *       SAKE_OVERLOAD_DECLARE_PARAMS could make the the arity larger by
 *       inserting additional function parameters.
 * Note: After #include'ing SAKE_OVERLOAD_GENERATE(), all the above macros are
 *       #undef'ed.
 *
 * The macro arguments for the above are as follows (T and x are implementation-
 * defined prefixes):
 *
 * r
 *     The next available state of the BOOST_PP_FOR construct. (Normally ignored
 *     but occasionally necessary to maximize preprocessing efficiency.)
 * n
 *     The size of each of the remaining Boost.PP tuple argument(s).  n will be
 *     between SAKE_OVERLOAD_MIN_ARITY and the overall maximum among the
 *     *_MAX_ARITY macros, inclusive.
 * classT_tuple
 *     A tuple of class declarations:
 *         ( class T0, ..., class T[n-1] )
 * T_tuple
 *     A tuple of the parameter types, with rvalue references removed.  That is,
 *     each Ti is either an lvalue reference or an object.  These are the
 *     appropriate types to use for Boost.ResultOf.
 *         ( T0, ..., T[n-1] )                                     [ C++11 ]
 *         ( remove_rvalue_reference< T0 {const} & >::type, ... )  [ C++03 ]
 * Tx_tuple
 *     A tuple of the function parameter declarations, as they should appear in
 *     the function prototype.  These *do* include rvalue references.
 *         ( T0&& x0, ..., T[n-1]&& x[n-1] )                  [ C++11 ]
 *         ( T0 {const} & x0, ..., T[n-1] {const} & x[n-1] )  [ C++03 ]
 * x_tuple
 *     A tuple of the function parameters (as lvalues).
 *         ( x0, x1, ..., x[n-1] )                                  [ C++11 ]
 *         ( SAKE_AS_LVALUE( x0 ), ..., SAKE_AS_LVALUE( x[n-1] ) )  [ C++03 ]
 * forward_x_tuple
 *     A tuple of the forwarded function parameters.
 *         ( forward< T0 >(x0), ..., forward< T[n-1] >(x[n-1]) )
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_OVERLOAD_HPP
#define SAKE_CORE_UTILITY_OVERLOAD_HPP

#include <boost/config.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/seq/for_each_product.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/tuple/rem.hpp>
#include <boost/static_assert.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/core/utility/private/overload/define_macros.hpp>

#define SAKE_OVERLOAD_GENERATE() \
    <sake/core/utility/private/overload/overload.ipp>

#endif // #ifndef SAKE_CORE_UTILITY_OVERLOAD_HPP

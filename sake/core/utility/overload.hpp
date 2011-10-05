/*******************************************************************************
 * core/utility/overload.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_OVERLOAD_GENERATE()
 *
 * This provides machinery to generate a family of overloaded functions, with
 * support for capturing rvalues and perfect forwarding of lvalues in C++03.
 * One defines certain predetermined macros to define the overload set, then
 * #include's SAKE_OVERLOAD_GENERATE().
 * 
 * The macros to define to specify the desired overload set are as follows:
 *
 * SAKE_OVERLOAD_T
 *     [ defaults to "T" ]
 *     Expands to the template parameter prefix.
 * SAKE_OVERLOAD_X
 *     [ defaults to "x" ]
 *     Expands to the function argument prefix.
 *
 * SAKE_OVERLOAD_DEFINE_RESULT
 *     [ optional ]
 *     If defined, will declare and define the result struct used by
 *     Boost.ResultOf.  Should only be defined with
 *     SAKE_OVERLOAD_MEMBER_FUNCTION_NAME or
 *     SAKE_OVERLOAD_CONST_MEMBER_FUNCTION_NAME.
 * SAKE_OVERLOAD_DECLARE_RESULT_TEMPLATE_PARAMS( n, classT_tuple )
 *     [ defaults to "class This, BOOST_PP_TUPLE_REM_CTOR( n, classT_tuple )" ]
 *     Expands to a comma-delimited template parameter declaration of the result
 *     struct.  Should only be defined with SAKE_OVERLOAD_DEFINE_RESULT.
 * SAKE_OVERLOAD_RESULT_SPECIALIZATION( n, T_tuple )
 *     [ defaults to "result< This T_tuple >" ]
 *     Expands to a specialization of the result struct.  Should only be defined
 *     with SAKE_OVERLOAD_DEFINE_RESULT.
 *
 * SAKE_OVERLOAD_RV_PARAM_TYPES_SEQ
 *     [ optional ]
 *     Expands to a Boost.Preprocessor seq (per positional parameter) of
 *     Boost.Preprocessor seqs specifying the set of types for which to enable
 *     "rvalue capturing".  See also SAKE_OVERLOAD_PARAM_SPEC_SEQ below.
 * SAKE_OVERLOAD_PARAM_SPEC_SEQ
 *     [ optional ]
 *     [ defaults to (see below) ]
 *     Expands to a Boost.Preprocessor seq (per positional parameter) of
 *     parameter specifications.  A parameter specification is a
 *     Boost.Preprocessor 3-tuple taking the form
 *         ( [tag], [type(s)], [x] )
 *     The tag must be one of the tags in the following table, which also lists
 *     the corresponding semantics.
 *         [tag]       | semantics of [type(s)]                   | argument declaration
 *         ------------+------------------------------------------+----------------------
 *         ref_tag     | template parameter name T                | T       & x
 *         cref_tag    | template parameter name T                | T const & x
 *         val_tag     | template parameter name T                | T x
 *         cval_tag    | template parameter name T                | T const x
 *         ptr_tag     | template parameter name T                | T       * x
 *         cptr_tag    | template parameter name T                | T const * x
 *         perfect_tag | Boost.PP seq of rvalue-captureable types | T&& x (C++11); T & x, T const & x, ... (C++03)
 *         fwd_tag     | Boost.PP seq of rvalue-captureable types | T&& x (C++11); T const & x, ... (C++03)
 *         fwd2_tag    | Boost.PP seq of rvalue-captureable types | T&& x (C++11); T & x, ... (C++03)
 *         lit_tag     | Boost.PP seq of literal type names       | T x
 *     For {c}ref_tag, {c}val_tag, and {c}ptr_tag, one may use "default" in
 *     place of an actual template parameter name for [types(s)], and it will be
 *     replaced with an appropriate default name.  "default" may likewise be
 *     used for [x].
 *     The purpose of this macro is to allow one to capture rvalues of specific
 *     known types at specific positions in the function argument list via
 *     perfect_tag, fwd_tag, or fwd2_tag.  These 3 tags have no difference when
 *     BOOST_NO_RVALUE_REFERENCES is not defined (i.e., in C++11).  However, if
 *     BOOST_NO_RVALUE_REFERENCES is defined (i.e., in C++03), perfect_tag is
 *     necessary to capture other rvalues and simultaneously retain the
 *     const-ness of lvalues, at the expense of additional overloads.  fwd_tag
 *     and fwd2_tag correspond to SAKE_FWD_REF and SAKE_FWD2_REF, respectively,
 *     and will generate fewer overloads (at the cost of rejecting some argument
 *     bindings or failing to retain the const-ness of lvalues).  For these
 *     tags, [types(s)] may be BOOST_PP_SEQ_NIL.
 *     If SAKE_OVERLOAD_RV_PARAM_TYPES_SEQ is defined, this defaults to
 *         (( perfect_tag, ( U00 ) ( U01 ) ... ( U0[n0-1] ), default ))
 *         (( perfect_tag, ( U10 ) ( U11 ) ... ( U1[n1-1] ), default ))
 *         ...
 *     where
 *         Uij = BOOST_PP_SEQ_ELEM(
 *                   BOOST_PP_SEQ_ELEM(
 *                       SAKE_OVERLOAD_RV_PARAM_TYPES_SEQ,
 *                       i
 *                   ),
 *                   j
 *               )
 *     and
 *         ni = BOOST_PP_SEQ_SIZE(
 *                  BOOST_PP_SEQ_ELEM(
 *                      SAKE_OVERLOAD_RV_PARAM_TYPES_SEQ,
 *                      i
 *                  )
 *              )
 *     If BOOST_NO_RVALUE_REFERENCES is not defined, the above will generate
 *     overloaded functions whose first parameter is one of
 *         { T0& x0, T0 const & x0, U00&& x0, U01&& x0, ..., U0[n0-1]&& x0 };
 *     whose second parameter is one of
 *         { T1& x0, T1 const & x0, U10&& x1, U11&& x1, ..., U1[n1-1]&& x1 };
 *     and so on.  T0 and T1 will be templated types, while the Uij's are not
 *     (wrt the function declaration).  Here, we use Uij&& as short-hand for
 *     boost::rv< Uij >&.  Further, it is guaranteed (through SFINAE) that
 *     rvalues of type Uij passed to positional parameter i will be bound to an
 *     emulated rvalue reference (if Uij is movable), rather than Ti const &.
 *
 * SAKE_OVERLOAD_RESULT( n, T_tuple )
 *     [ optional ]
 *     Expands to an invokation of a Boost.MPL metafunction giving the result
 *     type of the overloaded function.  Should not be defined with
 *     SAKE_OVERLOAD_CTOR_NAME.
 * SAKE_OVERLOAD_RESULT_TYPE
 *     [ defaults to "void" ]
 *     Expands to the result type of the overloaded function.  Should not be
 *     defined with SAKE_OVERLOAD_CTOR_NAME.
 *
 * SAKE_OVERLOAD_DECLARE_TEMPLATE_PARAMS( n, classT_tuple )
 *     [ defaults to "BOOST_PP_TUPLE_REM_CTOR( n, classT_tuple )" ]
 *     Expands to a comma-delimited template parameter declaration of an
 *     overloaded function.  Should not be defined with
 *     SAKE_OVERLOAD_PARAM_SPEC_SEQ.
 * SAKE_OVERLOAD_IS_CALLABLE( n, T_tuple )
 *     [ optional ]
 *     Expands to an invokation of a Boost.MPL boolean metafunction indicating
 *     whether to enable this overloaded function instantiation.
 * SAKE_OVERLOAD_FUNCTION_NAME
 *     [ optional ]
 *     Expands to the name of the overloaded (free) function.
 * SAKE_OVERLOAD_MEMBER_FUNCTION_NAME
 *     [ optional ]
 *     Expands to the name of the overloaded (non-const) member function.
 * SAKE_OVERLOAD_CONST_MEMBER_FUNCTION_NAME
 *     [ optional ]
 *     Expands to the name of the overloaded const member function.
 * SAKE_OVERLOAD_CTOR_NAME
 *     [ optional ]
 *     Expands to the name of the overloaded constructor.
 * SAKE_OVERLOAD_DECLARE_PARAMS( n, T_tuple, Tx_tuple )
 *     [ defaults to "BOOST_PP_TUPLE_REM_CTOR( n, Tx_tuple )" ]
 *     Expands to a comma-delimited list of parameter declarations for the
 *     overloaded function.
 * SAKE_OVERLOAD_CTOR_INIT_LIST( n, T_tuple, x_tuple, forward_x_tuple )
 *     [ optional ]
 *     Expands to a comma-delimited initializer list (excluding the ":").
 *     Should only be defined with SAKE_OVERLOAD_CTOR_NAME.
 * SAKE_OVERLOAD_BODY( n, T_tuple, x_tuple, forward_x_tuple )
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
 * Note: For the SAKE_OVERLOAD_XXX_NAME macros, the overload name includes any
 *       prefix qualifier keywords, e.g., "static" or "explicit".
 * Note: If SAKE_OVERLOAD_PARAM_SPEC_SEQ is defined, then all the ARITY macros
 *       are ignored.  Otherwise, at least one of the
 *       SAKE_OVERLOAD_XXX_MAX_ARITY macros must be defined (those left
 *       undefined are set to 0).  When BOOST_NO_RVALUE_REFERENCES is not
 *       defined, the maximum arity of the generated overload set is the overall
 *       maximum among the SAKE_OVERLOAD_XXX_MAX_ARITY macros.
 * Note: The SAKE_OVERLOAD_XXX_ARITY macros don't, strictly speaking, describe
 *       the arity limits of the overload set, but rather the size limits on the
 *       tuples passed to the above macros.  This will typically be identical to
 *       the arity limits on the overload set, but defining
 *       SAKE_OVERLOAD_DECLARE_PARAMS could make the the arity larger.
 * Note: After #include'ing SAKE_OVERLOAD_GENERATE(), all the above macros are
 *       #undef'ed.
 *
 * The macro arguments for the above are as follows (T and x would be replaced
 * with SAKE_OVERLOAD_T and SAKE_OVERLOAD_X, respectively, if defined):
 *
 * n
 *     The size of each of the remaining Boost.Preprocessor tuple argument(s).
 *     n will be between SAKE_OVERLOAD_MIN_ARITY and the overall maximum among
 *     the SAKE_OVERLOAD_XXX_MAX_ARITY macros, inclusive.
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
#include <boost/mpl/not.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/control/expr_iif.hpp>
#include <boost/preprocessor/control/iif.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/logical/bitor.hpp>
#include <boost/preprocessor/logical/compl.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/seq/fold_left.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/seq/for_each_product.hpp>
#include <boost/preprocessor/seq/seq.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/seq/to_tuple.hpp>
#include <boost/preprocessor/seq/transform.hpp>
#include <boost/preprocessor/tuple/rem.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/static_assert.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/preprocessor/keyword/default.hpp>
#include <sake/boost_ext/preprocessor/seq/for_each.hpp>
#include <sake/boost_ext/preprocessor/seq/is_nil.hpp>
#include <sake/boost_ext/preprocessor/seq/juxt_enum.hpp>
#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/add_rvalue_reference.hpp>
#include <sake/boost_ext/type_traits/is_same_sans_qualifiers.hpp>
#include <sake/boost_ext/type_traits/remove_rvalue_reference.hpp>

#include <sake/core/move/as_lvalue.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/move.hpp>
#include <sake/core/utility/call_traits.hpp>
#include <sake/core/utility/private/overload/define_macros.ipp>

#define SAKE_OVERLOAD_GENERATE() <sake/core/utility/private/overload/overload.ipp>

namespace sake
{

namespace overload_private
{

template< class T >
struct remove_crv
    : boost_ext::remove_rvalue_reference<T>
{ };

template< class T >
struct remove_crv< const T >
    : boost_ext::remove_rvalue_reference<T>
{ };

template< class T >
struct add_forward_reference
    : boost_ext::add_reference<
          typename boost_ext::add_rvalue_reference<T>::type
      >
{ };

template< class T >
struct rv_param
{
    typedef typename call_traits<
        typename boost_ext::add_rvalue_reference<
            typename remove_crv<T>::type
        >::type
    >::param_type type;
};

} // namespace overload_private

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_OVERLOAD_HPP

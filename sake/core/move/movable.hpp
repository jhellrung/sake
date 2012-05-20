/*******************************************************************************
 * sake/core/move/movable.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * SAKE_BASIC_MOVABLE_COPYABLE( T )
 * SAKE_MOVABLE_NONCOPYABLE( T )
 * SAKE_MOVABLE_COPYABLE_DEFINE_COPY_ASSIGN_FROM_MOVE( T )
 * SAKE_MOVABLE_COPYABLE_DEFINE_COPY_ASSIGN_FROM_SWAP( T )
 * SAKE_OPTIMAL_MOVABLE_COPYABLE( T )
 * SAKE_FRIENDLY_MOVABLE_COPYABLE( T )
 *
 * SAKE_BASIC_MOVABLE_COPYABLE_IF_C( typenameT, cond )
 * SAKE_OPTIMAL_MOVABLE_COPYABLE_IF_C( typenameT, cond )
 * SAKE_FRIENDLY_MOVABLE_COPYABLE_IF_C( typenameT, cond )
 *
 * SAKE_BASIC_MOVABLE_COPYABLE_MEMBERWISE[_R]( [r,] typenameT, member_seq )
 * SAKE_OPTIMAL_MOVABLE_COPYABLE_MEMBERWISE[_R]( [r,] typenameT, member_seq )
 * SAKE_FRIENDLY_MOVABLE_COPYABLE_MEMBERWISE[_R]( [r,] typenameT, member_seq )
 *
 * Note: For the IF_C and MEMBERWISE macros:
 * - typenameT should be the name of the enclosing class prefixed with the
 *   "typename" keyword if the class is a dependent type.
 * Note: For the MEMBERWISE macros:
 * - member_seq should be a Boost.PP sequence of base class specifiers followed
 *   by member variable specifiers. A base class specifier is a Boost.PP
 *   1-sequence simply consisting of the base class; a member variable specifier
 *   is a Boost.PP 2-sequence of the form ( type )( name ).
 *
 * These macros provide the move emulation machinery necessary for a class to be
 * movable in the absence of true rvalue references.  These macros can take the
 * place of BOOST_MOVABLE_BUT_NOT_COPYABLE and BOOST_COPYABLE_AND_MOVABLE.
 *
 * Each macro defines certain types and member functions to make it easy to
 * implement efficient moving for the enclosing class, regardless of whether
 * BOOST_NO_RVALUE_REFERENCES is defined.
 *
 * The MEMBERWISE macros define a memberwise copy constructor and move
 * constructor; and, if applicable, a memberwise copy assignment operator and
 * move assignment operator.  This dramatically reduces boilerplate in the
 * common case. The remaining macros require the enclosing class to define the
 * aforementioned special member functions, unless otherwise indicated.
 *
 * For all the macros except the MEMBERWISE macros, the enclosing class must
 * define a move constructor and, if applicable, a move assignment operator,
 * both taking parameters of type this_rvalue_param_type:
 *     T(this_rvalue_param_type other);
 *     T& operator=(this_rvalue_param_type other);
 *
 * BASIC_MOVABLE_COPYABLE allows a class to be move-constructible and explicitly
 * move-assignable, but it does not provide the machinery to move assign from
 * rvalue expressions.  This is ideal for base classes which typically wouldn't
 * need their assignment operator to capture (implicit) rvalues anyway. The
 * other MOVABLE_COPYABLE macros provide the machinery to move assign from
 * rvalue expressions.
 *
 * The DEFINE_COPY_ASSIGN_FROM macros define copy assignment automatically in
 * terms of either the well-known copy-and-swap idiom or the copy-and-move
 * idiom, so these require less additional work to implement than the
 * non-MEMBERWISE OPTIMAL and FRIENDLY macros, at the cost of a possibly
 * suboptimal copy asignment operator.
 *
 * The OPTIMAL_MOVABLE_COPYABLE[_IF_C] macros require the enclosing class to
 * define a copy assignment operator taking a parameter of type
 * this_copy_assign_param_type:
 *     T& operator=(this_copy_assign_param_type other);
 *
 * The FRIENDLY_MOVABLE_COPYABLE[_IF_C] macros require the enclosing class to
 * define a copy_assign_impl member function with signature
 *     T& copy_assign_impl(T const &)
 * The copy assignment operator will automatically be implemented either in
 * terms of the move assignment operator or copy_assign_impl.
 *
 * If BOOST_NO_RVALUE_REFERENCES is not defined, then the OPTIMAL and FRIENDLY
 * versions are functionally identical.  However, if BOOST_NO_RVALUE_REFERENCES
 * is defined, rvalue references must be emulated, and OPTIMAL and FRIENDLY
 * differ mainly in how they declare and implement the copy assignment operator.
 * OPTIMAL declares operator=(T&) to force T-rvalues to bind to the move
 * assignment operator, which enables move emulation to to be as efficient as
 * using true rvalue references (hence, optimal).  However, a class enclosing an
 * OPTIMAL movable type (either deriving from or containing as a member object)
 * will implicitly define a copy assignment operator taking a
 * reference-to-non-const, which is almost always undesirable (the enclosing
 * class won't be ConstCopyAssignable without explicitly defining the copy
 * assignment operator).  This could pose problems when mixing with
 * non-move-aware classes.  FRIENDLY declares operator=(T) and depends on copy
 * elision to efficiently capture rvalues.  This avoids the problem above with
 * OPTIMAL relating to enclosing classes, but now causes copy-assigning from
 * lvalues to be suboptimal (in general), since member objects would be
 * copied-and-moved, rather than simply copied.
 *
 * Since both techniques have drawbacks, both mechanisms are provided.
 *
 * The IF_C macros are identical to their non-IF_C counterparts unless
 * BOOST_NO_RVALUE_REFERENCES, in which case they only enable the move emulation
 * framework if the given boolean static constant evaluates to true. One still
 * defines the same collection of required member functions; these just might go
 * completely unused.
 *
 * In summary:
 * SAKE_BASIC_MOVABLE_COPYABLE
 * - defines no special member functions
 * - requires copy and move constructors; and copy and move assignment operators
 * SAKE_MOVABLE_NONCOPYABLE
 * - defines no special member functions
 * - requires move constructor; and, if applicable, move assignment operator
 * SAKE_MOVABLE_COPYABLE_DEFINE_COPY_ASSIGN_FROM
 * - defines copy assignment
 * - requires copy and move constructors; and move assignment operator
 * SAKE_OPTIMAL_MOVABLE_COPYABLE
 * - defines no special member functions
 * - requires copy and move constructors; and copy and move assignment operators
 * SAKE_FRIENDLY_MOVABLE_COPYABLE
 * - defines copy assignment operator
 * - requires copy and move constructors; and move assignment operator and
 *   copy_assign_impl member function
 * SAKE_{BASIC|OPTIMAL|FRIENDLY}_MOVABLE_COPYABLE_MEMBERWISE
 * - defines copy and move constructors; and copy and move assignment operators
 * - requires no additional special member functions
 ******************************************************************************/

#ifndef SAKE_CORE_MOVE_MOVABLE_HPP
#define SAKE_CORE_MOVE_MOVABLE_HPP

#include <boost/config.hpp>
#include <boost/preprocessor/facilities/empty.hpp>
#include <boost/preprocessor/repetition/deduce_r.hpp>
#include <boost/type_traits/integral_constant.hpp>

#include <sake/boost_ext/preprocessor/keyword/typename.hpp>

#include <sake/core/move/move.hpp>
#include <sake/core/type_traits/has_copy_constructor.hpp>
#include <sake/core/type_traits/has_move_constructor.hpp>
#include <sake/core/type_traits/has_nothrow_copy_assign.hpp>
#include <sake/core/type_traits/has_nothrow_copy_constructor.hpp>
#include <sake/core/type_traits/has_nothrow_move_assign.hpp>
#include <sake/core/type_traits/has_nothrow_move_constructor.hpp>
#include <sake/core/utility/memberwise/copy_assign.hpp>
#include <sake/core/utility/memberwise/copy_constructor.hpp>
#include <sake/core/utility/memberwise/move_assign.hpp>
#include <sake/core/utility/memberwise/move_constructor.hpp>
#include <sake/core/utility/memberwise/type_trait_tag.hpp>
#include <sake/core/utility/noncopyable.hpp>

#define SAKE_BASIC_MOVABLE_COPYABLE_IF_C( typenameT, cond ) \
    SAKE_BASIC_MOVABLE_COPYABLE_IF_C_impl( \
        SAKE_BOOST_EXT_PP_KEYWORD_GET_PREFIX_TYPENAME( typenameT ) BOOST_PP_EMPTY, \
        SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX_TYPENAME( typenameT ), \
        cond \
    )
#define SAKE_OPTIMAL_MOVABLE_COPYABLE_IF_C( typenameT, cond ) \
    SAKE_OPTIMAL_MOVABLE_COPYABLE_IF_C_impl( \
        SAKE_BOOST_EXT_PP_KEYWORD_GET_PREFIX_TYPENAME( typenameT ) BOOST_PP_EMPTY, \
        SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX_TYPENAME( typenameT ), \
        cond \
    )
#define SAKE_FRIENDLY_MOVABLE_COPYABLE_IF_C( typenameT, cond ) \
    SAKE_FRIENDLY_MOVABLE_COPYABLE_IF_C_impl( \
        SAKE_BOOST_EXT_PP_KEYWORD_GET_PREFIX_TYPENAME( typenameT ) BOOST_PP_EMPTY, \
        SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX_TYPENAME( typenameT ), \
        cond \
    )

#define SAKE_BASIC_MOVABLE_COPYABLE_MEMBERWISE( typenameT, member_seq ) \
    SAKE_BASIC_MOVABLE_COPYABLE_MEMBERWISE_R( BOOST_PP_DEDUCE_R(), typenameT, member_seq )
#define SAKE_OPTIMAL_MOVABLE_COPYABLE_MEMBERWISE( typenameT, member_seq ) \
    SAKE_OPTIMAL_MOVABLE_COPYABLE_MEMBERWISE_R( BOOST_PP_DEDUCE_R(), typenameT, member_seq )
#define SAKE_FRIENDLY_MOVABLE_COPYABLE_MEMBERWISE( typenameT, member_seq ) \
    SAKE_FRIENDLY_MOVABLE_COPYABLE_MEMBERWISE_R( BOOST_PP_DEDUCE_R(), typenameT, member_seq )

#define SAKE_BASIC_MOVABLE_COPYABLE_MEMBERWISE_R( r, typenameT, member_seq ) \
    SAKE_BASIC_MOVABLE_COPYABLE_MEMBERWISE_impl( r, \
        SAKE_BOOST_EXT_PP_KEYWORD_GET_PREFIX_TYPENAME( typenameT ) BOOST_PP_EMPTY, \
        SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX_TYPENAME( typenameT ), \
        member_seq \
    )
#define SAKE_OPTIMAL_MOVABLE_COPYABLE_MEMBERWISE_R( r, typenameT, member_seq ) \
    SAKE_OPTIMAL_MOVABLE_COPYABLE_MEMBERWISE_impl( r, \
        SAKE_BOOST_EXT_PP_KEYWORD_GET_PREFIX_TYPENAME( typenameT ) BOOST_PP_EMPTY, \
        SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX_TYPENAME( typenameT ), \
        member_seq \
    )
#define SAKE_FRIENDLY_MOVABLE_COPYABLE_MEMBERWISE_R( r, typenameT, member_seq ) \
    SAKE_FRIENDLY_MOVABLE_COPYABLE_MEMBERWISE_impl( r, \
        SAKE_BOOST_EXT_PP_KEYWORD_GET_PREFIX_TYPENAME( typenameT ) BOOST_PP_EMPTY, \
        SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX_TYPENAME( typenameT ), \
        member_seq \
    )



#define SAKE_MOVABLE_MEMBERWISE_typedef_has_xxx_tags( r, member_seq ) \
    SAKE_MEMBERWISE_TYPEDEF_TYPE_TRAIT_TAG_R( r, member_seq, has_copy_constructor ) \
    SAKE_MEMBERWISE_TYPEDEF_TYPE_TRAIT_TAG_R( r, member_seq, has_nothrow_copy_constructor ) \
    SAKE_MEMBERWISE_TYPEDEF_TYPE_TRAIT_TAG_R( r, member_seq, has_move_constructor ) \
    SAKE_MEMBERWISE_TYPEDEF_TYPE_TRAIT_TAG_R( r, member_seq, has_nothrow_move_constructor ) \
    SAKE_MEMBERWISE_TYPEDEF_TYPE_TRAIT_TAG_R( r, member_seq, has_nothrow_copy_assign ) \
    SAKE_MEMBERWISE_TYPEDEF_TYPE_TRAIT_TAG_R( r, member_seq, has_nothrow_move_assign )



#ifndef BOOST_NO_RVALUE_REFERENCES



#define SAKE_BASIC_MOVABLE_COPYABLE( T ) \
    typedef T&& this_rvalue_param_type;

#define SAKE_MOVABLE_NONCOPYABLE( T ) \
    typedef T&& this_rvalue_param_type; \
    SAKE_NONCOPYABLE( T )

#define SAKE_MOVABLE_COPYABLE_DEFINE_COPY_ASSIGN_FROM_MOVE( T ) \
    SAKE_BASIC_MOVABLE_COPYABLE( T ) \
    T& operator=(T const & other) \
    { T temp(other); return operator=(::sake::move(temp)); }

#define SAKE_MOVABLE_COPYABLE_DEFINE_COPY_ASSIGN_FROM_SWAP( T ) \
    SAKE_BASIC_MOVABLE_COPYABLE( T ) \
    T& operator=(T const & other) \
    { T temp(other); this->swap(temp); return *this; }

#define SAKE_OPTIMAL_MOVABLE_COPYABLE( T ) \
    typedef T const & this_copy_assign_param_type; \
    SAKE_BASIC_MOVABLE_COPYABLE( T )

#define SAKE_FRIENDLY_MOVABLE_COPYABLE( T ) \
    SAKE_BASIC_MOVABLE_COPYABLE( T ) \
    T& operator=(T const & other) \
    { return this->copy_assign_impl(other); }

#define SAKE_BASIC_MOVABLE_COPYABLE_IF_C_impl( typename, T, cond ) \
    SAKE_BASIC_MOVABLE_COPYABLE( T )
#define SAKE_OPTIMAL_MOVABLE_COPYABLE_IF_C_impl( typename, T, cond ) \
    SAKE_OPTIMAL_MOVABLE_COPYABLE( T )
#define SAKE_FRIENDLY_MOVABLE_COPYABLE_IF_C_impl( typename, T, cond ) \
    SAKE_FRIENDLY_MOVABLE_COPYABLE( T )

#ifndef BOOST_NO_DEFAULTED_FUNCTIONS
#define SAKE_BASIC_MOVABLE_COPYABLE_MEMBERWISE_impl( r, typename, T, member_seq ) \
    SAKE_MOVABLE_MEMBERWISE_typedef_has_xxx_tags( r, member_seq ) \
    SAKE_BASIC_MOVABLE_COPYABLE( T ) \
    T(T const &) = default; \
    T(T&&) = default; \
    T& operator=(T const &) = default; \
    T& operator=(T&&) = default;
#else // #ifndef BOOST_NO_DEFAULTED_FUNCTIONS
#define SAKE_BASIC_MOVABLE_COPYABLE_MEMBERWISE_impl( r, typename, T, member_seq ) \
    SAKE_MOVABLE_MEMBERWISE_typedef_has_xxx_tags( r, member_seq ) \
    SAKE_BASIC_MOVABLE_COPYABLE( T )
#endif // #ifndef BOOST_NO_DEFAULTED_FUNCTIONS

#define SAKE_OPTIMAL_MOVABLE_COPYABLE_MEMBERWISE_impl( r, typename, T, member_seq ) \
    typedef T const & this_copy_assign_param_type; \
    SAKE_BASIC_MOVABLE_COPYABLE_MEMBERWISE_impl( r, typename, T, member_seq )

#define SAKE_FRIENDLY_MOVABLE_COPYABLE_MEMBERWISE_impl( r, typename, T, member_seq ) \
    SAKE_BASIC_MOVABLE_COPYABLE_MEMBERWISE_impl( r, typename, T, member_seq )



#else // #ifndef BOOST_NO_RVALUE_REFERENCES



#include <boost/mpl/if.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/facilities/expand.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/seq/seq.hpp>
#include <boost/preprocessor/seq/size.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/preprocessor/seq/is_nil.hpp>
#include <sake/boost_ext/preprocessor/seq/size_01x.hpp>

#include <sake/core/move/has_move_emulation.hpp>
#include <sake/core/move/rv.hpp>
#include <sake/core/utility/using_typedef.hpp>
#include <sake/core/utility/memberwise/private/copy_assign_enable.hpp>

#define SAKE_BASIC_MOVABLE_COPYABLE( T ) \
    typedef ::boost::rv<T>& this_rvalue_param_type; \
    operator ::boost::rv<T> & () \
    { return *static_cast< ::boost::rv<T> * >(this); } \
    operator ::boost::rv<T> const & () const \
    { return *static_cast< ::boost::rv<T> const * >(this); } \

#define SAKE_MOVABLE_NONCOPYABLE( T ) \
    typedef ::boost::rv<T>& this_rvalue_param_type; \
    operator ::boost::rv<T>& () \
    { return *static_cast< ::boost::rv<T> * >(this); } \
    SAKE_NONCOPYABLE( T )

#define SAKE_MOVABLE_COPYABLE_DEFINE_COPY_ASSIGN_FROM_MOVE( T ) \
    SAKE_BASIC_MOVABLE_COPYABLE( T ) \
    T& operator=(T other) \
    { return operator=(::sake::move(other)); }

#define SAKE_MOVABLE_COPYABLE_DEFINE_COPY_ASSIGN_FROM_SWAP( T ) \
    SAKE_BASIC_MOVABLE_COPYABLE( T ) \
    T& operator=(T other) \
    { this->swap(other); return *this; }

#define SAKE_OPTIMAL_MOVABLE_COPYABLE( T ) \
    typedef ::boost::rv<T> const & this_copy_assign_param_type; \
    SAKE_BASIC_MOVABLE_COPYABLE( T ) \
    T& operator=(T& other) \
    { return operator=(const_cast< T const & >(other)); }

#define SAKE_FRIENDLY_MOVABLE_COPYABLE( T ) \
    SAKE_MOVABLE_COPYABLE_DEFINE_COPY_ASSIGN_FROM_MOVE( T )

#define SAKE_BASIC_MOVABLE_COPYABLE_IF_C_impl( typename, T, cond ) \
    static bool const _sake_movable_emulation_enable = cond; \
    typedef ::sake::movable_private::basic_traits< \
        T, _sake_movable_emulation_enable \
    > _sake_movable_traits; \
    SAKE_USING_TYPEDEF( typename() _sake_movable_traits, this_rvalue_param_type ); \
    typedef typename() _sake_movable_traits::rv_conv_type \
        _sake_movable_rv_conv_type; \
    operator _sake_movable_rv_conv_type & () \
    { return *static_cast< _sake_movable_rv_conv_type * >(this); } \
    operator _sake_movable_rv_conv_type const & () const \
    { return *static_cast< _sake_movable_rv_conv_type const * >(this); }

#define SAKE_OPTIMAL_MOVABLE_COPYABLE_IF_C_impl( typename, T, cond ) \
    static bool const _sake_movable_emulation_enable = cond; \
    typedef ::sake::movable_private::optimal_traits< \
        T, _sake_movable_emulation_enable, true \
    > _sake_movable_traits; \
    SAKE_USING_TYPEDEF( typename() _sake_movable_traits, this_rvalue_param_type ); \
    SAKE_USING_TYPEDEF( typename() _sake_movable_traits, this_copy_assign_param_type ); \
    typedef typename() _sake_movable_traits::rv_conv_type \
        _sake_movable_rv_conv_type; \
    typedef typename() _sake_movable_traits::enable_move_assign_param_type \
        _sake_movable_emulation_enable_move_assign_param_type; \
    operator _sake_movable_rv_conv_type & () \
    { return *static_cast< _sake_movable_rv_conv_type * >(this); } \
    operator _sake_movable_rv_conv_type const & () const \
    { return *static_cast< _sake_movable_rv_conv_type const * >(this); } \
    T& operator=(_sake_movable_emulation_enable_move_assign_param_type other) \
    { return operator=(const_cast< T const & >(other)); }

#define SAKE_FRIENDLY_MOVABLE_COPYABLE_IF_C_impl( typename, T, cond ) \
    static bool const _sake_movable_emulation_enable = cond; \
    typedef ::sake::movable_private::friendly_traits< \
        T, _sake_movable_emulation_enable, true \
    > _sake_movable_traits; \
    SAKE_USING_TYPEDEF( typename() _sake_movable_traits, this_rvalue_param_type ); \
    typedef typename() _sake_movable_traits::rv_conv_type \
        _sake_movable_rv_conv_type; \
    typedef typename() _sake_movable_traits::copy_assign_by_value_param_type \
        _sake_movable_copy_assign_by_value_param_type; \
    typedef typename() _sake_movable_traits::copy_assign_by_cref_param_type \
        _sake_movable_copy_assign_by_cref_param_type; \
    operator _sake_movable_rv_conv_type & () \
    { return *static_cast< _sake_movable_rv_conv_type * >(this); } \
    operator _sake_movable_rv_conv_type const & () const \
    { return *static_cast< _sake_movable_rv_conv_type const * >(this); } \
    T& operator=(_sake_movable_copy_assign_by_value_param_type other) \
    { return operator=(::sake::move(other)); } \
    T& operator=(_sake_movable_copy_assign_by_cref_param_type other) \
    { return this->copy_assign_impl(other); }

#define SAKE_BASIC_MOVABLE_COPYABLE_MEMBERWISE_impl( r, typename, T, member_seq ) \
    BOOST_PP_CAT( \
        SAKE_BASIC_MOVABLE_COPYABLE_MEMBERWISE_impl, \
        SAKE_BOOST_EXT_PP_SEQ_IS_NIL( member_seq ) \
    ) ( r, typename, T, member_seq )

#define SAKE_BASIC_MOVABLE_COPYABLE_MEMBERWISE_impl0( r, typename, T, member_seq ) \
    typedef ::sake::movable_private::basic_traits< \
        T, \
        SAKE_MOVABLE_MEMBERWISE_emulation_enable( r, member_seq )::value \
    > _sake_movable_traits; \
    SAKE_USING_TYPEDEF( typename() _sake_movable_traits, this_rvalue_param_type ); \
    typedef typename() _sake_movable_traits::rv_conv_type \
        _sake_movable_rv_conv_type; \
    operator _sake_movable_rv_conv_type & () \
    { return *static_cast< _sake_movable_rv_conv_type * >(this); } \
    operator _sake_movable_rv_conv_type const & () const \
    { return *static_cast< _sake_movable_rv_conv_type const * >(this); } \
    SAKE_MEMBERWISE_TYPEDEF_TYPE_TRAIT_TAG_R( r, member_seq, has_copy_constructor ) \
    SAKE_MEMBERWISE_TYPEDEF_TYPE_TRAIT_TAG_R( r, member_seq, has_nothrow_copy_constructor ) \
    SAKE_MEMBERWISE_MOVE_CONSTRUCTOR_R( r, typename() T, member_seq ) \
    SAKE_MEMBERWISE_COPY_ASSIGN_IF_ANY_HAS_UME_R( r, typename() T, member_seq ) \
    SAKE_MEMBERWISE_MOVE_ASSIGN_R( r, typename() T, member_seq )

#define SAKE_BASIC_MOVABLE_COPYABLE_MEMBERWISE_impl1( r, typename, T, member_seq ) \
    typedef ::sake::movable_private::disabler_param<0> this_rvalue_param_type; \
    SAKE_MOVABLE_MEMBERWISE_typedef_has_xxx_tags( r, member_seq )

#define SAKE_OPTIMAL_MOVABLE_COPYABLE_MEMBERWISE_impl( r, typename, T, member_seq ) \
    BOOST_PP_CAT( \
        SAKE_OPTIMAL_MOVABLE_COPYABLE_MEMBERWISE_impl, \
        SAKE_BOOST_EXT_PP_SEQ_IS_NIL( member_seq ) \
    ) ( r, typename, T, member_seq )

#define SAKE_OPTIMAL_MOVABLE_COPYABLE_MEMBERWISE_impl0( r, typename, T, member_seq ) \
    typedef ::sake::movable_private::optimal_traits< \
        T, \
        SAKE_MOVABLE_MEMBERWISE_emulation_enable( r, member_seq )::value, \
        SAKE_MEMBERWISE_copy_assign_enable( r, member_seq )::value \
    > _sake_movable_traits; \
    SAKE_USING_TYPEDEF( typename() _sake_movable_traits, this_rvalue_param_type ); \
    SAKE_USING_TYPEDEF( typename() _sake_movable_traits, this_copy_assign_param_type ); \
    typedef typename() _sake_movable_traits::rv_conv_type \
        _sake_movable_rv_conv_type; \
    typedef typename() _sake_movable_traits::enable_move_assign_param_type \
        _sake_movable_emulation_enable_move_assign_param_type; \
    operator _sake_movable_rv_conv_type & () \
    { return *static_cast< _sake_movable_rv_conv_type * >(this); } \
    operator _sake_movable_rv_conv_type const & () const \
    { return *static_cast< _sake_movable_rv_conv_type const * >(this); } \
    SAKE_MEMBERWISE_TYPEDEF_TYPE_TRAIT_TAG_R( r, member_seq, has_copy_constructor ) \
    SAKE_MEMBERWISE_TYPEDEF_TYPE_TRAIT_TAG_R( r, member_seq, has_nothrow_copy_constructor ) \
    SAKE_MEMBERWISE_MOVE_CONSTRUCTOR_R( r, typename() T, member_seq ) \
    SAKE_MEMBERWISE_COPY_ASSIGN_R( r, typename() T, member_seq ) \
    T& operator=(_sake_movable_emulation_enable_move_assign_param_type other) \
        BOOST_NOEXCEPT_IF((has_nothrow_copy_assign_tag::value)) \
    { return operator=(static_cast< T const & >(other)); } \
    SAKE_MEMBERWISE_MOVE_ASSIGN_R( r, typename() T, member_seq )

#define SAKE_OPTIMAL_MOVABLE_COPYABLE_MEMBERWISE_impl1( r, typename, T, member_seq ) \
    typedef ::sake::movable_private::disabler_param<0> this_rvalue_param_type; \
    SAKE_MOVABLE_MEMBERWISE_typedef_has_xxx_tags( r, member_seq )

#define SAKE_FRIENDLY_MOVABLE_COPYABLE_MEMBERWISE_impl( r, typename, T, member_seq ) \
    BOOST_PP_CAT( \
        SAKE_FRIENDLY_MOVABLE_COPYABLE_MEMBERWISE_impl, \
        SAKE_BOOST_EXT_PP_SEQ_IS_NIL( member_seq ) \
    ) ( r, typename, T, member_seq )

#define SAKE_FRIENDLY_MOVABLE_COPYABLE_MEMBERWISE_impl0( r, typename, T, member_seq ) \
    static bool const _sake_movable_emulation_enable = \
        SAKE_MOVABLE_MEMBERWISE_emulation_enable( r, member_seq )::value; \
    typedef ::sake::movable_private::friendly_traits< \
        T, \
        _sake_movable_emulation_enable, \
        SAKE_MEMBERWISE_copy_assign_enable( r, member_seq )::value \
    > _sake_movable_traits; \
    SAKE_USING_TYPEDEF( typename() _sake_movable_traits, this_rvalue_param_type ); \
    typedef typename() _sake_movable_traits::rv_conv_type \
        _sake_movable_rv_conv_type; \
    typedef typename() _sake_movable_traits::copy_assign_by_value_param_type \
        _sake_movable_copy_assign_param_type; \
    operator _sake_movable_rv_conv_type & () \
    { return *static_cast< _sake_movable_rv_conv_type * >(this); } \
    operator _sake_movable_rv_conv_type const & () const \
    { return *static_cast< _sake_movable_rv_conv_type const * >(this); } \
    SAKE_MEMBERWISE_TYPEDEF_TYPE_TRAIT_TAG_R( r, member_seq, has_copy_constructor ) \
    SAKE_MEMBERWISE_TYPEDEF_TYPE_TRAIT_TAG_R( r, member_seq, has_nothrow_copy_constructor ) \
    SAKE_MEMBERWISE_MOVE_CONSTRUCTOR_R( r, typename() T, member_seq ) \
    SAKE_MEMBERWISE_MOVE_ASSIGN_R( r, typename() T, member_seq ) \
    typedef typename() ::boost::mpl::if_c< \
        _sake_movable_emulation_enable, \
        ::sake::boost_ext::mpl::and2< \
            has_nothrow_copy_constructor_tag, \
            has_nothrow_move_assign_tag \
        >, \
        SAKE_MEMBERWISE_TYPE_TRAIT_TAG_R( r, member_seq, has_nothrow_copy_assign ) \
    >::type has_nothrow_copy_assign_tag; \
    template< class, class > friend class ::sake::has_type_has_nothrow_copy_assign_tag; \
    T& operator=(_sake_movable_copy_assign_param_type other) \
        BOOST_NOEXCEPT_IF((has_nothrow_copy_assign_tag::value)) \
    { return operator=(::sake::move(other)); }

#define SAKE_FRIENDLY_MOVABLE_COPYABLE_MEMBERWISE_impl1( r, typename, T, member_seq ) \
    typedef ::sake::movable_private::disabler_param<0> this_rvalue_param_type; \
    SAKE_MOVABLE_MEMBERWISE_typedef_has_xxx_tags( r, member_seq )

#define SAKE_MOVABLE_MEMBERWISE_emulation_enable( r, member_seq ) \
    BOOST_PP_CAT( \
        SAKE_MOVABLE_MEMBERWISE_emulation_enable_, \
        SAKE_BOOST_EXT_PP_SEQ_SIZE_01X( member_seq, x ) \
    ) ( r, member_seq )
#define SAKE_MOVABLE_MEMBERWISE_emulation_enable_0( r, member_seq ) \
    ::boost::false_type
#define SAKE_MOVABLE_MEMBERWISE_emulation_enable_1( r, member_seq ) \
    ::sake::has_move_emulation< BOOST_PP_SEQ_HEAD( BOOST_PP_SEQ_HEAD( member_seq ) ) >
#define SAKE_MOVABLE_MEMBERWISE_emulation_enable_x( r, member_seq ) \
    ::sake::boost_ext::mpl::BOOST_PP_CAT( or, BOOST_PP_SEQ_SIZE( member_seq ) ) < \
        BOOST_PP_SEQ_FOR_EACH_I_R( r, \
            SAKE_MOVABLE_comma_has_move_emulation, ~, member_seq ) \
    >
#define SAKE_MOVABLE_comma_has_move_emulation( r, data, i, elem ) \
    BOOST_PP_COMMA_IF( i ) \
    ::sake::has_move_emulation< BOOST_PP_SEQ_HEAD( elem ) >

namespace sake
{

namespace movable_private
{

template< class T, int >
class disabler_param : public T
{
    disabler_param();
    disabler_param(disabler_param&);
    ~disabler_param();
    void operator=(disabler_param&);
};

template< class T >
class disabler_conv : public T
{
    disabler_conv();
    disabler_conv(disabler_conv&);
    ~disabler_conv();
    void operator=(disabler_conv&);
};

template< class T, bool Enable >
struct basic_traits;
template< class T, bool Enable, bool CopyAssignEnable >
struct optimal_traits;
template< class T, bool Enable, bool CopyAssignEnable >
struct friendly_traits;

template< class T >
struct basic_traits< T, true >
{
    typedef boost::rv<T>  rv_conv_type;
    typedef boost::rv<T>& this_rvalue_param_type;
};

template< class T >
struct basic_traits< T, false >
{
    typedef disabler_conv<T>     rv_conv_type;
    typedef disabler_param<T,0>& this_rvalue_param_type;
};

template< class T >
struct optimal_traits< T, true, true >
{
    typedef boost::rv<T>         rv_conv_type;
    typedef boost::rv<T>&        this_rvalue_param_type;
    typedef boost::rv<T> const & this_copy_assign_param_type;
    typedef T&                   enable_move_assign_param_type;
};

template< class T >
struct optimal_traits< T, true, false >
{
    typedef boost::rv<T>         rv_conv_type;
    typedef boost::rv<T>&        this_rvalue_param_type;
    typedef disabler_param<T,0>& this_copy_assign_param_type;
    typedef disabler_param<T,1>& enable_move_assign_param_type;
};

template< class T, bool CopyAssignEnable >
struct optimal_traits< T, false, CopyAssignEnable >
{
    typedef disabler_conv<T>     rv_conv_type;
    typedef disabler_param<T,0>& this_rvalue_param_type;
    typedef disabler_param<T,1>& this_copy_assign_param_type;
    typedef disabler_param<T,2>& enable_move_assign_param_type;
};

template< class T >
struct friendly_traits< T, true, true >
{
    typedef boost::rv<T>         rv_conv_type;
    typedef boost::rv<T>&        this_rvalue_param_type;
    typedef T                    copy_assign_by_value_param_type;
    typedef disabler_param<T,0>& copy_assign_by_cref_param_type;
};

template< class T >
struct friendly_traits< T, true, false >
{
    typedef boost::rv<T>         rv_conv_type;
    typedef boost::rv<T>&        this_rvalue_param_type;
    typedef disabler_param<T,0>& copy_assign_by_value_param_type;
    typedef disabler_param<T,1>& copy_assign_by_cref_param_type;
};

template< class T, bool CopyAssignEnable >
struct friendly_traits< T, false, CopyAssignEnable >
{
    typedef disabler_conv<T>     rv_conv_type;
    typedef disabler_param<T,0>& this_rvalue_param_type;
    typedef disabler_param<T,1>& copy_assign_by_value_param_type;
    typedef T const &            copy_assign_by_cref_param_type;
};

} // namespace movable_private

} // namespace sake



#endif // #ifndef BOOST_NO_RVALUE_REFERENCES



#endif // #ifndef SAKE_CORE_MOVE_MOVABLE_HPP

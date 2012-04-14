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
 * SAKE_OPTIMAL_MOVABLE_COPYABLE_MEMBERWISE[_R]( [r,] typenameT, base_seq, member_type_name_seq )
 * SAKE_FRIENDLY_MOVABLE_COPYABLE_MEMBERWISE[_R]( [r,] typenameT, base_seq, member_type_name_seq )
 *
 * Note: For the MEMBERWISE macros, typenameT should be the name of the
 * enclosing class prefixed with the "typename" keyword if the class is a
 * dependent type.
 *
 * These macros provide the move emulation machinery necessary for a class to be
 * movable in the absence of true rvalue references.  These macros can take the
 * place of BOOST_MOVABLE_BUT_NOT_COPYABLE and BOOST_COPYABLE_AND_MOVABLE.
 *
 * Each macro defines certain types and member functions to make it easy to
 * implement efficient moving for the enclosing class, regardless of whether
 * BOOST_NO_RVALUE_REFERENCES is defined.
 *
 * All the macros above *except* the *_MEMBERWISE macros require the enclosing
 * class to define a move constructor and, if applicable, a move assignment
 * operator, both taking parameters of type this_rvalue_param_type:
 *
 *     T(this_rvalue_param_type other) : ... { ... }
 *     T& operator=(this_rvalue_param_type other) { ... }
 *
 * SAKE_BASIC_MOVABLE_COPYABLE allows a class to be move-constructible and
 * explicitly move-assignable, but it does not provide the machinery to move
 * assign from rvalue expressions.  This is ideal for base classes which
 * typically wouldn't need their assignment operator to capture rvalues anyway.
 * The remaining macros provide the machinery to move assign from rvalue
 * expressions.
 *
 * The SAKE_MOVABLE_COPYABLE_DEFINE_COPY_ASSIGN_FROM macros define copy
 * assignment automatically in terms of either the well-known copy-and-swap
 * idiom or the copy-and-move idiom, so these require the least amount of
 * additional work to use.  The remaining macros require some additional
 * definitions to define copy assignment.
 *
 * The OPTIMAL_MOVABLE_COPYABLE macro requires the enclosing class to define a
 * copy assignment operator taking a parameter of type
 * this_copy_assign_param_type:
 *     T& operator=(this_copy_assign_param_type other)
 *     { ... }
 *
 * The FRIENDLY_MOVABLE_COPYABLE macro requires the enclosing class to define a
 * copy_assign_impl member function with signature
 *     T& copy_assign_impl(T const &)
 * The copy assignment operator will automatically be implemented either in
 * terms of the move assignment operator or copy_assign_impl.
 *
 * The MEMBERWISE macros define a memberwise copy constructor, move constructor,
 * copy assignment operator, and move assignment operator, thus lessening the
 * amount of boilerplate in the common case.
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
 ******************************************************************************/

#ifndef SAKE_CORE_MOVE_MOVABLE_HPP
#define SAKE_CORE_MOVE_MOVABLE_HPP

#include <boost/config.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/facilities/expand.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/repetition/deduce_r.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/seq/size.hpp>

#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/preprocessor/keyword/typename.hpp>
#include <sake/boost_ext/preprocessor/seq/is_nil.hpp>
#include <sake/boost_ext/preprocessor/tuple/rem.hpp>

#include <sake/core/move/is_movable.hpp>
#include <sake/core/move/move.hpp>
#include <sake/core/move/rv.hpp>
#include <sake/core/utility/memberwise/copy_assign.hpp>
#include <sake/core/utility/memberwise/copy_ctor.hpp>
#include <sake/core/utility/memberwise/move_assign.hpp>
#include <sake/core/utility/memberwise/move_ctor.hpp>
#include <sake/core/utility/non_copyable.hpp>
#include <sake/core/utility/using_typedef.hpp>

#define SAKE_OPTIMAL_MOVABLE_COPYABLE_MEMBERWISE( \
    typenameT, base_seq, member_type_name_seq ) \
    SAKE_OPTIMAL_MOVABLE_COPYABLE_MEMBERWISE_R( BOOST_PP_DEDUCE_R(), \
        typenameT, base_seq, member_type_name_seq )
#define SAKE_FRIENDLY_MOVABLE_COPYABLE_MEMBERWISE( \
    typenameT, base_seq, member_type_name_seq ) \
    SAKE_FRIENDLY_MOVABLE_COPYABLE_MEMBERWISE_R( BOOST_PP_DEDUCE_R(), \
        typenameT, base_seq, member_type_name_seq )

#define SAKE_OPTIMAL_MOVABLE_COPYABLE_MEMBERWISE_R( r, \
    typenameT, base_seq, member_type_name_seq ) \
    SAKE_OPTIMAL_MOVABLE_COPYABLE_MEMBERWISE_impl( r, \
        SAKE_BOOST_EXT_PP_KEYWORD_GET_PREFIX_TYPENAME( typenameT ) BOOST_PP_EMPTY, \
        SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX_TYPENAME( typenameT ), \
        base_seq, \
        SAKE_MEMBERWISE_seq_transform_tuple_elem( r, 0, member_type_name_seq ), \
        SAKE_MEMBERWISE_seq_transform_tuple_elem( r, 1, member_type_name_seq ) \
    )
#define SAKE_FRIENDLY_MOVABLE_COPYABLE_MEMBERWISE_R( r, \
    typenameT, base_seq, member_type_name_seq ) \
    SAKE_FRIENDLY_MOVABLE_COPYABLE_MEMBERWISE_impl( r, \
        SAKE_BOOST_EXT_PP_KEYWORD_GET_PREFIX_TYPENAME( typenameT ) BOOST_PP_EMPTY, \
        SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX_TYPENAME( typenameT ), \
        base_seq, \
        SAKE_MEMBERWISE_seq_transform_tuple_elem( r, 0, member_type_name_seq ), \
        SAKE_MEMBERWISE_seq_transform_tuple_elem( r, 1, member_type_name_seq ) \
    )

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
    { return copy_assign_impl(other); }

#define SAKE_OPTIMAL_MOVABLE_COPYABLE_MEMBERWISE_impl( r, \
    typename, T, base_seq, member_type_seq, member_name_seq ) \
    SAKE_OPTIMAL_MOVABLE_COPYABLE( T ) \
    SAKE_MEMBERWISE_COPY_CTOR_R( r, T, base_seq, member_name_seq ) \
    SAKE_MEMBERWISE_MOVE_CTOR_R( r, T, base_seq, member_name_seq ) \
    SAKE_MEMBERWISE_COPY_ASSIGN_R( r, T, base_seq, member_name_seq ) \
    SAKE_MEMBERWISE_MOVE_ASSIGN_R( r, T, base_seq, member_name_seq )

#define SAKE_FRIENDLY_MOVABLE_COPYABLE_MEMBERWISE_impl( r, \
    typename, T, base_seq, member_type_seq, member_name_seq ) \
    SAKE_OPTIMAL_MOVABLE_COPYABLE_MEMBERWISE_impl( r, \
        typename, T, base_seq, member_type_seq, member_name_seq )

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

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
    SAKE_FRIENDLY_MOVABLE_COPYABLE( T )

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
    SAKE_BASIC_MOVABLE_COPYABLE( T ) \
    T& operator=(T other) \
    { return operator=(::sake::move(other)); }

#define SAKE_OPTIMAL_MOVABLE_COPYABLE_MEMBERWISE_impl( r, \
    typename, T, base_seq, member_type_seq, member_name_seq ) \
    static bool const _sake_movable_memberwise_enable = \
        SAKE_MOVABLE_memberwise_enable( r, \
            SAKE_BOOST_EXT_PP_SEQ_JOIN( ( base_seq ) ( member_type_seq ) ) ); \
    typedef ::sake::movable_private::optimal_memberwise_traits< \
        T, _sake_movable_memberwise_enable \
    > _sake_movable_memberwise_traits; \
    typedef typename() _sake_movable_memberwise_traits::rv_conv_type _sake_rv_conv_type; \
    typedef typename() _sake_movable_memberwise_traits::enable_move_assign_param_type _sake_enable_move_assign_param_type; \
    SAKE_USING_TYPEDEF( typename() _sake_movable_memberwise_traits, this_rvalue_param_type ); \
    SAKE_USING_TYPEDEF( typename() _sake_movable_memberwise_traits, this_copy_assign_param_type ); \
    operator _sake_rv_conv_type & () \
    { return *static_cast< _sake_rv_conv_type * >(this); } \
    operator _sake_rv_conv_type const & () const \
    { return *static_cast< _sake_rv_conv_type const * >(this); } \
    SAKE_MEMBERWISE_MOVE_CTOR_R( r, T, base_seq, member_name_seq ) \
    T& operator=(_sake_enable_move_assign_param_type other) \
    { return operator=(const_cast< T const & >(other)); } \
    SAKE_MEMBERWISE_COPY_ASSIGN_R( r, T, base_seq, member_name_seq ) \
    SAKE_MEMBERWISE_MOVE_ASSIGN_R( r, T, base_seq, member_name_seq )

#define SAKE_FRIENDLY_MOVABLE_COPYABLE_MEMBERWISE_impl( r, \
    typename, T, base_seq, member_type_seq, member_name_seq ) \
    static bool const _sake_movable_memberwise_enable = \
        SAKE_MOVABLE_memberwise_enable( r, \
            SAKE_BOOST_EXT_PP_SEQ_JOIN( ( base_seq ) ( member_type_seq ) ) ); \
    typedef ::sake::movable_private::friendly_memberwise_traits< \
        T, _sake_movable_memberwise_enable \
    > _sake_movable_memberwise_traits; \
    typedef typename() _sake_movable_memberwise_traits::rv_conv_type _sake_rv_conv_type; \
    typedef typename() _sake_movable_memberwise_traits::copy_assign_param_type _sake_copy_assign_param_type; \
    SAKE_USING_TYPEDEF( typename() _sake_movable_memberwise_traits, this_rvalue_param_type ); \
    SAKE_USING_TYPEDEF( typename() _sake_movable_memberwise_traits, this_copy_assign_param_type ); \
    operator _sake_rv_conv_type & () \
    { return *static_cast< _sake_rv_conv_type * >(this); } \
    operator _sake_rv_conv_type const & () const \
    { return *static_cast< _sake_rv_conv_type const * >(this); } \
    SAKE_MEMBERWISE_MOVE_CTOR_R( r, T, base_seq, member_name_seq ) \
    T& operator=(_sake_copy_assign_param_type other) \
    { return operator=(::sake::move(other)); } \
    SAKE_MEMBERWISE_MOVE_ASSIGN_R( r, T, base_seq, member_name_seq )

#define SAKE_MOVABLE_memberwise_enable( r, type_seq ) \
    BOOST_PP_CAT( \
        SAKE_MOVABLE_memberwise_enable_, \
        SAKE_BOOST_EXT_PP_SEQ_IS_NIL( type_seq ) \
    ) ( r, type_seq )
#define SAKE_MOVABLE_memberwise_enable_0( r, type_seq ) \
    BOOST_PP_CAT( \
        SAKE_MOVABLE_memberwise_enable_0, \
        BOOST_PP_EQUAL( 1, BOOST_PP_SEQ_SIZE( type_seq ) ) \
    ) ( r, type_seq )
#define SAKE_MOVABLE_memberwise_enable_00( r, type_seq ) \
    ::sake::boost_ext::mpl::BOOST_PP_CAT( or, BOOST_PP_SEQ_SIZE( type_seq ) ) < \
        BOOST_PP_SEQ_FOR_EACH_I_R( r, SAKE_MOVABLE_comma_is_movable, ~, type_seq ) \
    >::value
#define SAKE_MOVABLE_comma_is_movable( r, data, i, elem ) \
    BOOST_PP_COMMA_IF( i ) ::sake::is_movable< elem >
#define SAKE_MOVABLE_memberwise_enable_01( r, type_seq ) \
    ::sake::is_movable< SAKE_BOOST_EXT_PP_TUPLE_REM1 type_seq >::value
#define SAKE_MOVABLE_memberwise_enable_1( r, type_seq ) \
    false

namespace sake
{

namespace movable_private
{

template< class T, int > struct disabler;

template< class T, bool Enable >
struct optimal_memberwise_traits;
template< class T, bool Enable >
struct friendly_memberwise_traits;

template< class T >
struct optimal_memberwise_traits< T, true >
{
    typedef boost::rv<T>         rv_conv_type;
    typedef boost::rv<T>&        this_rvalue_param_type;
    typedef boost::rv<T> const & this_copy_assign_param_type;
    typedef T&                   enable_move_assign_param_type;
};

template< class T >
struct optimal_memberwise_traits< T, false >
{
    typedef disabler<T,0> rv_conv_type;
    typedef disabler<T,1> this_rvalue_param_type;
    typedef disabler<T,2> this_copy_assign_param_type;
    typedef disabler<T,3> enable_move_assign_param_type;
};

template< class T >
struct friendly_memberwise_traits< T, true >
{
    typedef boost::rv<T>  rv_conv_type;
    typedef boost::rv<T>& this_rvalue_param_type;
    typedef T             copy_assign_param_type;
};

template< class T >
struct friendly_memberwise_traits< T, false >
{
    typedef disabler<T,0> rv_conv_type;
    typedef disabler<T,1> this_rvalue_param_type;
    typedef disabler<T,2> copy_assign_param_type;
};

} // namespace movable_private

} // namespace sake

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

#endif // #ifndef SAKE_CORE_MOVE_MOVABLE_HPP

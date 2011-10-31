/*******************************************************************************
 * sake/core/move/movable.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * SAKE_BASIC_MOVABLE_COPYABLE( T )
 * SAKE_MOVABLE_NONCOPYABLE( T )
 * SAKE_MOVABLE_COPYABLE_DEFINE_COPY_ASSIGN_FROM_MOVE( T )
 * SAKE_MOVABLE_COPYABLE_DEFINE_COPY_ASSIGN_FROM_SWAP( T )
 * SAKE_OPTIMAL_MOVABLE_COPYABLE( T )
 * SAKE_FRIENDLY_MOVABLE_COPYABLE( T )
 * SAKE_OPTIMAL_MOVABLE_COPYABLE_IF_C( T, c )
 * SAKE_FRIENDLY_MOVABLE_COPYABLE_IF_C( T, c )
 * SAKE_OPTIMAL_MOVABLE_COPYABLE_IF_MOVABLE[_R]( [r,] T, U_seq )
 * SAKE_FRIENDLY_MOVABLE_COPYABLE_IF_MOVABLE[_R]( [r,] T, U_seq )
 *
 * Note: For *_IF_* only, if T is a dependent type, it should be prefixed with
 * the "typename" keyword.
 *
 * These macros provide the move emulation machinery necessary for a class to be
 * movable in the absence of true rvalue references.  These macros can take the
 * place of BOOST_MOVABLE_BUT_NOT_COPYABLE and BOOST_COPYABLE_AND_MOVABLE.
 *
 * Each macro defines certain types and member functions to make it easy to
 * implement efficient moving for the enclosing class, regardless of whether
 * BOOST_NO_RVALUE_REFERENCES is defined.
 *
 * All macros require the enclosing class to define a move constructor and, if
 * applicable, a move assignment operator, both taking parameters of type
 * this_rvalue_param_type:
 *
 *     T(this_rvalue_param_type other) : ... { ... }
 *     T& operator=(this_rvalue_param_type other) { ... }
 *
 * When using the *_IF_* macros, if the given condition is false,
 * this_rvalue_param_type is a typedef for some dummy type, effectively
 * disabling the move constructor and move assignment operators defined in the
 * enclosing class and thus always using copy construction and copy assignment.
 * For these macros only, if the enclosing class T is a dependent type, then one
 * must prefix the enclosing class with the "typename" keyword.
 *
 * SAKE_BASIC_MOVABLE_COPYABLE allows a class to be move-constructible and
 * explicitly move-assignable, but it does not provide the machinery to move
 * assign from rvalues.  This is ideal for base classes which typically wouldn't
 * need their assignment operator to capture rvalues anyway.  The remaining
 * macros provide the machinery to move assign from rvalues.
 *
 * The SAKE_MOVABLE_COPYABLE_DEFINE_COPY_ASSIGN_FROM_* macros define copy
 * assignment automatically in terms of either the well-known copy-and-swap
 * idiom or the copy-and-move idiom, so these require the least amount of
 * additional work to use.  The remaining macros require some additional
 * definitions to define copy assignment.
 *
 * The OPTIMAL macros require the enclosing class to define a copy assignment
 * operator.  This should take a parameter of type this_copy_assign_param_type:
 *     T& operator=(this_copy_assign_param_type other)
 *     { ... }
 *
 * The FRIENDLY macros require the enclosing class to define a member function
 * copy_assign_impl with the signature
 *     T& copy_assign_impl(T const &)
 * The copy assignment operator will automatically be implemented either in
 * terms of the move assignment operator or copy_assign_impl.
 *
 * The *_IF_C macros enable move emulation if the given condition is true.  If
 * false, the move constructor and move assignment operator will be disabled.
 * The *_IF_MOVABLE macros enable move emulation if at least one of the types in
 * the Boost.Preprocessor seq U_seq is movable.
 *
 * If BOOST_NO_RVALUE_REFERENCES is not defined, then the OPTIMAL and FRIENDLY
 * versions are functionally identical.  However, if BOOST_NO_RVALUE_REFERENCES
 * is defined, rvalue references must be emulated, and OPTIMAL and FRIENDLY
 * differ mainly in how they declare and implement the copy assignment operator.
 * OPTIMAL declares operator=(T&) to force T-rvalues to bind to the move
 * assignment operator, which enables move emulation to to be as efficient as
 * using true rvalue references (hence, optimal).  However, a class enclosing an
 * OPTIMAL (either deriving from or containing as a member object) movable type
 * will implicitly-define a copy assignment operator taking a
 * reference-to-non-const, which is almost always undesirable (the enclosing
 * class won't be ConstCopyAssignable without explicitly defining the copy
 * assignment operator).  This could pose problems when mixing with
 * non-move-aware classes.  FRIENDLY declares operator=(T) and depends on copy
 * elision to efficiently capture rvalues.  This avoids the problem above with
 * OPTIMAL relating to enclosing classes, but now causes copy-assigning from
 * lvalues to be suboptimal (in general), since member objects would be
 * copied-and-moved or copied-and-copied, rather than just copied once.
 *
 * Since both techniques have drawbacks, both mechanisms are provided.
 ******************************************************************************/

#ifndef SAKE_CORE_MOVE_MOVABLE_HPP
#define SAKE_CORE_MOVE_MOVABLE_HPP

#include <boost/config.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/control/expr_iif.hpp>
#include <boost/preprocessor/facilities/expand.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/repetition/deduce_r.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/seq/size.hpp>

#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/preprocessor/keyword/typename.hpp>

#include <sake/core/move/rv_sink.hpp>
#include <sake/core/move/is_movable.hpp>
#include <sake/core/move/move.hpp>
#include <sake/core/move/rv.hpp>
#include <sake/core/utility/non_copyable.hpp>
#include <sake/core/utility/using_typename.hpp>

#define SAKE_OPTIMAL_MOVABLE_COPYABLE_IF_MOVABLE( T, U_seq ) \
    SAKE_OPTIMAL_MOVABLE_COPYABLE_IF_MOVABLE_R( BOOST_PP_DEDUCE_R(), T, U_seq )
#define SAKE_FRIENDLY_MOVABLE_COPYABLE_IF_MOVABLE( T, U_seq ) \
    SAKE_FRIENDLY_MOVABLE_COPYABLE_IF_MOVABLE_R( BOOST_PP_DEDUCE_R(), T, U_seq )

namespace sake
{

namespace movable_private
{

template< class T, int > struct disabler;

template< class T >
struct non_movable_traits
{
    typedef disabler<T,0> rv_conv_type;
    typedef disabler<T,1> this_rvalue_param_type;
    typedef T const &     this_copy_assign_param_type;
    typedef disabler<T,3> enable_optimal_move_assign_param_type;
    typedef disabler<T,4> enable_friendly_move_assign_param_type;
    typedef T const &     disable_friendly_move_assign_param_type;
};

} // namespace movable_private

} // namespace sake

#ifndef BOOST_NO_RVALUE_REFERENCES

#define SAKE_BASIC_MOVABLE_COPYABLE( T ) \
    private: \
    typedef T&& this_rvalue_param_type; \
    public:

#define SAKE_MOVABLE_NONCOPYABLE( T ) \
    private: \
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
    private: \
    typedef T const & this_copy_assign_param_type; \
    SAKE_BASIC_MOVABLE_COPYABLE( T )

#define SAKE_FRIENDLY_MOVABLE_COPYABLE( T ) \
    SAKE_BASIC_MOVABLE_COPYABLE( T ) \
    T& operator=(T const & other) \
    { return copy_assign_impl(other); }

#define SAKE_OPTIMAL_MOVABLE_COPYABLE_IF_C( T, c ) \
    SAKE_OPTIMAL_MOVABLE_COPYABLE_IF_C_impl( \
        BOOST_PP_EXPR_IIF( \
            SAKE_BOOST_EXT_PP_KEYWORD_HAS_PREFIX_TYPENAME( T ), \
            typename \
        ) BOOST_PP_EMPTY, \
        SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX_TYPENAME( T ), \
        c \
    )
#define SAKE_OPTIMAL_MOVABLE_COPYABLE_IF_C_impl( typename_, T, c ) \
    private: \
    typedef ::sake::movable_private::traits< T, c > _sake_movable_traits; \
    SAKE_USING_TYPENAME( typename_() _sake_movable_traits, this_rvalue_param_type ); \
    typedef T const & this_copy_assign_param_type; \
    public:

#define SAKE_FRIENDLY_MOVABLE_COPYABLE_IF_C( T, c ) \
    SAKE_FRIENDLY_MOVABLE_COPYABLE_IF_C_impl( \
        BOOST_PP_EXPR_IIF( \
            SAKE_BOOST_EXT_PP_KEYWORD_HAS_PREFIX_TYPENAME( T ), \
            typename \
        ) BOOST_PP_EMPTY, \
        SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX_TYPENAME( T ), \
        c \
    )
#define SAKE_FRIENDLY_MOVABLE_COPYABLE_IF_C_impl( typename_, T, c ) \
    private: \
    typedef ::sake::movable_private::traits< T, c > _sake_movable_traits; \
    SAKE_USING_TYPENAME( typename_() _sake_movable_traits, this_rvalue_param_type ); \
    public: \
    T& operator=(T const & other) \
    { return copy_assign_impl(other); }

#define SAKE_OPTIMAL_MOVABLE_COPYABLE_IF_MOVABLE_R( r, T, U_seq ) \
    SAKE_OPTIMAL_MOVABLE_COPYABLE( T )

#define SAKE_FRIENDLY_MOVABLE_COPYABLE_IF_MOVABLE_R( r, T, U_seq ) \
    SAKE_FRIENDLY_MOVABLE_COPYABLE( T )

namespace sake
{

namespace movable_private
{

template< class T, bool c >
struct traits;

template< class T >
struct traits< T, false >
    : non_movable_traits<T>
{ };

template< class T >
struct traits< T, true >
{
    typedef T&& this_rvalue_param_type;
};

} // namespace movable_private

} // namespace sake

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

#define SAKE_BASIC_MOVABLE_COPYABLE( T ) \
    private: \
    typedef ::boost::rv<T>& this_rvalue_param_type; \
    public: \
    operator ::boost::rv<T> & () \
    { return *static_cast< ::boost::rv<T>* >(this); } \
    operator ::boost::rv<T> const & () const \
    { return *static_cast< ::boost::rv<T> const * >(this); } \
    template< class V, class R, class P > \
    operator ::sake::rv_sink< V,R,P, ::sake::rv_sink_private::apply1_pred<P,T>::value >() \
    { return ::sake::rv_sink< V,R,P, ::sake::rv_sink_private::apply1_pred<P,T>::value >(this); }

#define SAKE_MOVABLE_NONCOPYABLE( T ) \
    private: \
    typedef ::boost::rv<T>& this_rvalue_param_type; \
    public: \
    operator ::boost::rv<T>& () \
    { return *static_cast< ::boost::rv<T>* >(this); } \
    SAKE_NONCOPYABLE( T )

#define SAKE_MOVABLE_COPYABLE_DEFINE_COPY_ASSIGN_FROM_MOVE( T ) \
    SAKE_FRIENDLY_MOVABLE_COPYABLE( T )

#define SAKE_MOVABLE_COPYABLE_DEFINE_COPY_ASSIGN_FROM_SWAP( T ) \
    SAKE_BASIC_MOVABLE_COPYABLE( T ) \
    T& operator=(T other) \
    { this->swap(other); return *this; }

#define SAKE_OPTIMAL_MOVABLE_COPYABLE( T ) \
    private: \
    typedef ::boost::rv<T> const & this_copy_assign_param_type; \
    SAKE_BASIC_MOVABLE_COPYABLE( T ) \
    T& operator=(T& other) \
    { return operator=(const_cast< T const & >(other)); }

#define SAKE_FRIENDLY_MOVABLE_COPYABLE( T ) \
    SAKE_BASIC_MOVABLE_COPYABLE( T ) \
    T& operator=(T other) \
    { return operator=(::sake::move(other)); }

#define SAKE_MOVABLE_COPYABLE_IF_C_common( typename_, T, c ) \
    typedef typename_() _sake_movable_traits::rv_conv_type _sake_rv_conv_type; \
    SAKE_USING_TYPENAME( typename_() _sake_movable_traits, this_rvalue_param_type ); \
    public: \
    operator _sake_rv_conv_type& () \
    { return *static_cast< _sake_rv_conv_type* >(this); } \
    operator _sake_rv_conv_type const & () const \
    { return *static_cast< _sake_rv_conv_type const * >(this); } \
    template< class V, class R, class P > \
    operator ::sake::rv_sink< V,R,P, ::sake::rv_sink_private::apply1_pred_if_c<c,P,T>::value >() \
    { return ::sake::rv_sink< V,R,P, ::sake::rv_sink_private::apply1_pred_if_c<c,P,T>::value >(this); }

#define SAKE_OPTIMAL_MOVABLE_COPYABLE_IF_C( T, c ) \
    SAKE_OPTIMAL_MOVABLE_COPYABLE_IF_C_impl( \
        BOOST_PP_EXPR_IIF( \
            SAKE_BOOST_EXT_PP_KEYWORD_HAS_PREFIX_TYPENAME( T ), \
            typename \
        ) BOOST_PP_EMPTY, \
        SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX_TYPENAME( T ), \
        c \
    )
#define SAKE_OPTIMAL_MOVABLE_COPYABLE_IF_C_impl( typename_, T, c ) \
    private: \
    typedef ::sake::movable_private::optimal_traits< T, c > _sake_movable_traits; \
    SAKE_USING_TYPENAME( typename_() _sake_movable_traits, this_copy_assign_param_type ); \
    typedef typename_() _sake_movable_traits::enable_optimal_move_assign_param_type \
        _sake_enable_move_assign_param_type; \
    SAKE_MOVABLE_COPYABLE_IF_C_common( typename_, T, c ) \
    T& operator=(_sake_enable_move_assign_param_type other) \
    { return operator=(const_cast< T const & >(other)); }

#define SAKE_FRIENDLY_MOVABLE_COPYABLE_IF_C( T, c ) \
    SAKE_FRIENDLY_MOVABLE_COPYABLE_IF_C_impl( \
        BOOST_PP_EXPR_IIF( \
            SAKE_BOOST_EXT_PP_KEYWORD_HAS_PREFIX_TYPENAME( T ), \
            typename \
        ) BOOST_PP_EMPTY, \
        SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX_TYPENAME( T ), \
        c \
    )
#define SAKE_FRIENDLY_MOVABLE_COPYABLE_IF_C_impl( typename_, T, c ) \
    private: \
    typedef ::sake::boost_ext::movable_private::friendly_traits< T, c > _sake_movable_traits; \
    typedef typename_() _sake_movable_traits::enable_friendly_move_assign_param_type \
        _sake_enable_move_assign_param_type; \
    typedef typename_() _sake_movable_traits::disable_friendly_move_assign_param_type \
        _sake_disable_move_assign_param_type; \
    SAKE_MOVABLE_COPYABLE_IF_C_common( typename_, T, c ) \
    T& operator=(_sake_enable_move_assign_param_type other) \
    { return operator=(::sake::boost_ext::move(other)); } \
    T& operator=(_sake_disable_move_assign_param_type other) \
    { return copy_assign_impl(other); }

#define SAKE_OPTIMAL_MOVABLE_COPYABLE_IF_MOVABLE_R( r, T, U_seq ) \
    SAKE_OPTIMAL_MOVABLE_COPYABLE_IF_C( T, SAKE_MOVABLE_any_movable( r, U_seq ) )

#define SAKE_FRIENDLY_MOVABLE_COPYABLE_IF_MOVABLE_R( r, T, U_seq ) \
    SAKE_FRIENDLY_MOVABLE_COPYABLE_IF_C( T, SAKE_MOVABLE_any_movable( r, U_seq ) )

#define SAKE_MOVABLE_any_movable( r, U_seq ) \
    BOOST_PP_CAT( ::sake::boost_ext::mpl::or, BOOST_PP_SEQ_SIZE( U_seq ) ) < \
        BOOST_PP_SEQ_FOR_EACH_I_R( r, SAKE_MOVABLE_comma_is_movable, ~, U_seq ) \
    >::value
#define SAKE_MOVABLE_comma_is_movable( r, data, i, elem ) \
    BOOST_PP_COMMA_IF( i ) ::sake::is_movable< elem >

namespace sake
{

namespace movable_private
{

template< class T, bool c >
struct optimal_traits;

template< class T >
struct optimal_traits< T, false >
    : non_movable_traits<T>
{ };

template< class T >
struct optimal_traits< T, true >
{
    typedef boost::rv<T>         rv_conv_type;
    typedef boost::rv<T>&        this_rvalue_param_type;
    typedef boost::rv<T> const & this_copy_assign_param_type;
    typedef T&                   enable_optimal_move_assign_param_type;
};

template< class T, bool c >
struct friendly_traits;

template< class T >
struct friendly_traits< T, false >
    : non_movable_traits<T>
{ };

template< class T >
struct friendly_traits< T, true >
{
    typedef boost::rv<T>  rv_conv_type;
    typedef boost::rv<T>& this_rvalue_param_type;
    typedef T             enable_friendly_move_assign_param_type;
    typedef disabler<T,0> disable_friendly_move_assign_param_type;
};

} // namespace movable_private

} // namespace sake

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

#endif // #ifndef SAKE_CORE_MOVE_MOVABLE_HPP

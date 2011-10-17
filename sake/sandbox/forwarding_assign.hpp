/*******************************************************************************
 * sake/core/utility/forwarding_assign.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_FORWARDING_ASSIGN[_R]( [r,] T, V, body )
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_FORWARDING_ASSIGN_HPP
#define SAKE_CORE_UTILITY_FORWARDING_ASSIGN_HPP

#include <boost/config.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/detail/is_unary.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/repetition/deduce_r.hpp>
#include <boost/preprocessor/seq/enum.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/all.hpp>
#include <sake/boost_ext/mpl/any.hpp>
#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/preprocessor/keyword/cv.hpp>
#include <sake/boost_ext/type_traits/add_rvalue_reference.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>
#include <sake/boost_ext/type_traits/is_same_sans_qualifiers.hpp>
#include <sake/boost_ext/type_traits/remove_rvalue_reference.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/move/any_rv.hpp>
#include <sake/core/move/as_lvalue.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/movable.hpp>
#include <sake/core/utility/call_traits.hpp>
#include <sake/core/utility/void.hpp>

namespace sake
{

#define SAKE_FORWARDING_ASSIGN( class_name, value_types, body ) \
    SAKE_FORWARDING_ASSIGN_R( BOOST_PP_DEDUCE_R(), class_name, value_types, body )

#ifndef BOOST_NO_RVALUE_REFERENCES

#define SAKE_FORWARDING_ASSIGN_R( r, class_name, value_types, body ) \
    template< class _SakeForwardingAssignT > \
    class_name & \
    operator=(_SakeForwardingAssignT&& _sake_forwarding_assign_x) \
    SAKE_BOOST_EXT_PP_KEYWORD_GET_PREFIX_CV( class_name ) \
    { body( _SakeForwardingAssignT, \
            _sake_forwarding_assign_x, \
            ::sake::forward< _SakeForwardingAssignT >(_sake_forwarding_assign_x) ) }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

#define SAKE_FORWARDING_ASSIGN_R( r, class_name, value_types, body ) \
    BOOST_PP_CAT( SAKE_FORWARDING_ASSIGN_impl, BOOST_PP_IS_UNARY( value_types ) ) \
    ( r, class_name, value_types, body )

#define SAKE_FORWARDING_ASSIGN_impl0( r, class_name, value_type, body ) \
    SAKE_FORWARDING_ASSIGN_operator_assign_lvalue( class_name, body ) \
    SAKE_FORWARDING_ASSIGN_operator_assign_rvalue( class_name, _, value_type, body ) \
    typedef ::sake::any_rv< \
        ::sake::forwarding_assign_private::binder< class_name >, \
        class_name &, \
        ::sake::forwarding_assign_private::not_is_same_sans_qualifiers_as< \
            SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX_CLASS( value_type ) > \
    > _sake_forwarding_assign_any_rv_type; \
    SAKE_FORWARDING_ASSIGN_operator_assign_any_rv( class_name ) \
    SAKE_FORWARDING_ASSIGN_operator_assign_const_lvalue( \
        class_name, \
        ( ::sake::boost_ext::is_same_sans_qualifiers< \
            value_type )( _SakeForwardingAssignT > ), \
        body \
    )

#define SAKE_FORWARDING_ASSIGN_impl1( r, class_name, value_types, body ) \
    SAKE_FORWARDING_ASSIGN_operator_assign_lvalue( class_name, body ) \
    BOOST_PP_SEQ_FOR_EACH_I_R( r, \
        SAKE_FORWARDING_ASSIGN_operator_assign_rvalue_helper, ( class_name, body ), value_types ) \
    typedef ::boost::mpl::BOOST_PP_CAT( vector, BOOST_PP_SEQ_SIZE( value_types ) )< \
        BOOST_PP_SEQ_FOR_EACH_I_R( r, SAKE_FORWARDING_ASSIGN_comma_value, ~, value_types ) \
    > _sake_forwarding_assign_value_types; \
    typedef ::sake::any_rv< \
        ::sake::forwarding_assign_private::binder< class_name >, \
        class_name &, \
        ::sake::forwarding_assign_private::not_any_is_same_sans_qualifiers< \
            _sake_forwarding_assign_value_types > \
    > _sake_forwarding_assign_any_rv_type; \
    SAKE_FORWARDING_ASSIGN_operator_assign_const_lvalue( \
        class_name, \
        ( ::sake::forwarding_assign_private::any_is_same_sans_qualifiers< \
            _sake_forwarding_assign_value_types )( \
            _SakeForwardingAssignT > ), \
        body \
    )
#define SAKE_FORWARDING_ASSIGN_operator_assign_rvalue_helper( r, data, i, elem ) \
    SAKE_FORWARDING_ASSIGN_operator_assign_rvalue( \
        BOOST_PP_TUPLE_ELEM( 2, 0, data ), i, elem, BOOST_PP_TUPLE_ELEM( 2, 1, data ) )
#define SAKE_FORWARDING_ASSIGN_comma_value( r, data, i, elem ) \
    BOOST_PP_COMMA_IF( i ) SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX_CLASS( elem )

#define SAKE_FORWARDING_ASSIGN_operator_assign_lvalue( class_name, body ) \
    template< class _SakeForwardingAssignT > \
    typename ::boost::disable_if_c< \
        ::boost::is_base_of< \
            SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX_CV( class_name ), \
            typename ::boost::remove_const< _SakeForwardingAssignT >::type \
        >::value, \
        class_name & \
    >::type \
    operator=(_SakeForwardingAssignT& _sake_forwarding_assign_x) \
    SAKE_BOOST_EXT_PP_KEYWORD_GET_PREFIX_CV( class_name ) \
    { body( typename ::sake::boost_ext::remove_rvalue_reference< _SakeForwardingAssignT& >::type, \
            SAKE_AS_LVALUE( _sake_forwarding_assign_x ), \
            _sake_forwarding_assign_x ) } \

#define SAKE_FORWARDING_ASSIGN_operator_assign_rvalue( class_name, i, value_type, body ) \
    typedef SAKE_BOOST_EXT_PP_KEYWORD_GET_PREFIX_DEPENDENT( value_type ) \
        ::sake::forwarding_assign_private::rparam_type< \
            SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX_CLASS( value_type ) \
        >::type BOOST_PP_CAT( _sake_forwarding_assign_rparam_type, i ); \
    class_name & \
    operator=(BOOST_PP_CAT( _sake_forwarding_assign_rparam_type, i ) x) \
    SAKE_BOOST_EXT_PP_KEYWORD_GET_PREFIX_CV( class_name ) \
    { body( SAKE_BOOST_EXT_PP_KEYWORD_GET_PREFIX_DEPENDENT( value_type ) \
                ::sake::boost_ext::remove_rvalue_reference< \
                    BOOST_PP_CAT( _sake_forwarding_assign_rparam_type, i ) >::type, \
            SAKE_AS_LVALUE( x ), \
            x ) } \

#define SAKE_FORWARDING_ASSIGN_operator_assign_any_rv( class_name ) \
    class_name & \
    operator=(_sake_forwarding_assign_any_rv_type x) \
    SAKE_BOOST_EXT_PP_KEYWORD_GET_PREFIX_CV( class_name ) \
    { return x(::sake::forwarding_assign_private::binder< class_name >(*this)); } \

#define SAKE_FORWARDING_ASSIGN_operator_assign_const_lvalue( class_name, disable_cond, body ) \
    template< class _SakeForwardingAssignT > \
    typename ::boost::disable_if_c< \
        ::sake::boost_ext::mpl::or3< \
            ::boost::is_base_of< \
                SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX_CV( class_name ), \
                _SakeForwardingAssignT \
            >, \
            BOOST_PP_SEQ_ENUM( disable_cond ), \
            ::sake::boost_ext::is_convertible< \
                _SakeForwardingAssignT&, \
                _sake_forwarding_assign_any_rv_type \
            > \
        >::value, \
        class_name & \
    >::type \
    operator=(SAKE_FWD_REF( _SakeForwardingAssignT ) _sake_forwarding_assign_x) \
    SAKE_BOOST_EXT_PP_KEYWORD_GET_PREFIX_CV( class_name ) \
    { body( SAKE_FWD_PARAM( _SakeForwardingAssignT ), \
            SAKE_AS_LVALUE( _sake_forwarding_assign_x ), \
            ::sake::forward< _SakeForwardingAssignT >(_sake_forwarding_assign_x) ) }

namespace forwarding_assign_private
{

template< class T >
struct rparam_type
{
    typedef typename sake::call_traits<
        typename boost_ext::add_rvalue_reference<T>::type
    >::param_type type;
};

template< class T >
struct binder
{
    explicit binder(T& this_) : m_this(this_) { }
    typedef T& result_type;
    template< class U >
    T& operator()(SAKE_FWD2_REF( U ) x) const
    { return m_this = sake::forward<U>(x); }
private:
    T& m_this;
};

template< class T >
struct is_same_sans_qualifiers_as
{
    template< class U >
    struct apply
        : boost_ext::is_same_sans_qualifiers<T,U>
    { };
};

template< class T >
struct not_is_same_sans_qualifiers_as
{
    template< class U >
    struct apply
    {
        static bool const value = !boost_ext::is_same_sans_qualifiers<T,U>::value;
        typedef apply type;
    };
};

template< class Sequence, class T >
struct any_is_same_sans_qualifiers
    : boost_ext::mpl::any<
          Sequence,
          is_same_sans_qualifiers_as<T>
      >
{ };

template< class Sequence >
struct not_any_is_same_sans_qualifiers
{
    template< class T >
    struct apply
        : boost_ext::mpl::all<
              Sequence,
              not_is_same_sans_qualifiers_as<T>
          >
    { };
};

} // namespace forwarding_assign_private

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_FORWARDING_ASSIGN_HPP

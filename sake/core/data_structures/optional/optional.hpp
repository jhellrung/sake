/*******************************************************************************
 * core/data_structures/optional.hpp
 *
 * Copyright 2010, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct optional<T>
 * extension::has_default_constructor< optional<T> >
 * operator==(optional< T0 > const & x0, optional< T1 > const & x1) -> bool
 * operator!=(optional< T0 > const & x0, optional< T1 > const & x1) -> bool
 * operator< (optional< T0 > const & x0, optional< T1 > const & x1) -> bool
 * operator> (optional< T0 > const & x0, optional< T1 > const & x1) -> bool
 * operator<=(optional< T0 > const & x0, optional< T1 > const & x1) -> bool
 * operator>=(optional< T0 > const & x0, optional< T1 > const & x1) -> bool
 *
 * See the Boost.Optional documentation
 *     http://www.boost.org/libs/optional/doc/html/index.html
 ******************************************************************************/

#ifndef SAKE_CORE_DATA_STRUCTURES_OPTIONAL_HPP
#define SAKE_CORE_DATA_STRUCTURES_OPTIONAL_HPP

#include <boost/config.hpp>
// #include <boost/mpl/and.hpp>
// #include <boost/mpl/not.hpp>
// #include <boost/mpl/or.hpp>
// #include <boost/preprocessor/facilities/apply.hpp>
#include <boost/static_assert.hpp>
// #include <boost/type_traits/alignment_of.hpp>
#include <boost/type_traits/integral_constant.hpp>
// #include <boost/type_traits/is_base_of.hpp>
// #include <boost/type_traits/is_same.hpp>
// #include <boost/type_traits/type_with_alignment.hpp>
#include <boost/utility/enable_if.hpp>

// #include <sake/boost_ext/mpl/and.hpp>
// #include <sake/boost_ext/mpl/or.hpp>
// #include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/add_reference_add_const.hpp>
#include <sake/boost_ext/type_traits/add_rvalue_reference.hpp>
#include <sake/boost_ext/type_traits/common_type.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>
// #include <sake/boost_ext/type_traits/is_convertible_wnrbt.hpp>
#include <sake/boost_ext/type_traits/is_reference.hpp>
// #include <sake/boost_ext/type_traits/remove_qualifiers.hpp>
#include <sake/boost_ext/type_traits/remove_reference_add_const.hpp>

#include <sake/core/data_structures/optional_fwd.hpp>
#include <sake/core/data_structures/private/optional.hpp>
// #include <sake/core/functional/forwarding/forwarding_base.hpp>
// #include <sake/core/functional/forwarding/forwarding_base_deduced_callable.hpp>
// #include <sake/core/functional/forwarding/forwarding_base_deduced_params.hpp>
// #include <sake/core/functional/forwarding/forwarding_base_deduced_result.hpp>
// #include <sake/core/functional/forwarding/forwarding_core_access.hpp>
// #include <sake/core/introspection/is_assignable.hpp>
#include <sake/core/move/movable.hpp>
// #include <sake/core/move/move.hpp>
// #include <sake/core/move/rv.hpp>
// #include <sake/core/move/rv_sink.hpp>
// #include <sake/core/utility/address_of.hpp>
// #include <sake/core/utility/assert.hpp>
// #include <sake/core/utility/call_traits.hpp>
// #include <sake/core/utility/emplacer_fwd.hpp>
// #include <sake/core/utility/explicit_bool.hpp>
// #include <sake/core/utility/forwarding_assign.hpp>
// #include <sake/core/utility/forwarding_unary_ctor.hpp>
#include <sake/core/utility/has_default_constructor_fwd.hpp>
// #include <sake/core/utility/nullptr.hpp>
// #include <sake/core/utility/overload.hpp>

namespace sake
{

/*******************************************************************************
 * struct optional<T>
 ******************************************************************************/

namespace optional_adl
{

template< class T >
struct optional
    : private_::forwarding_base<T>
{
    BOOST_STATIC_ASSERT((!boost_ext::is_reference<T>::value));

    typedef T value_type;

    SAKE_OPTIMAL_MOVABLE_COPYABLE_IF_MOVABLE( typename optional, ( T ) )

    optional();
    optional(optional const & other);
    optional(this_rvalue_param_type other);

    ~optional();

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U >
    explicit optional(U&& x);
    template< class U >
    optional(U&& x, bool const active);
    template< class U >
    optional(optional<U> const & other,
        typename boost::enable_if_c< boost_ext::is_convertible<
            typename boost_ext::add_reference_add_const<U>::type, T
        >::value >::type* = 0);
    template< class U >
    optional(optional<U>&& other,
        typename boost::enable_if_c< boost_ext::is_convertible<
            typename boost_ext::add_rvalue_reference<U>::type, T
        >::value >::type* = 0);

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

private:
    typedef typename rv_sink_traits::rv_param<T>::type value_rv_param_type;
    struct value_ctor_rv_sink
    {
        template< class U >
        struct apply
        {
            static bool const value =
                !boost::is_base_of< optional, U >::value
             && !boost_ext::is_same_sans_cv<U,T>::value
             && !sake::is_optional<U>::value;
            typedef apply type;
        };
        typedef sake::rv_sink<
            rv_sink_visitors::ctor_impl< optional >,
            void,
            value_ctor_rv_sink
        > type;
    };
    friend struct rv_sink_visitors::ctor_impl< optional >;
    template< class U >
    void ctor_impl(U& x);
public:
    // lvalues
    template< class U >
    explicit optional(U& x,
        typename boost::disable_if_c< boost::is_base_of<
            optional, typename boost::remove_const<U>::type
        >::value >::type* = 0);
    template< class U >
    optional(U& x, bool const active);
    // T rvalues
    explicit optional(value_rv_param_type x);
    optional(value_rv_param_type x, bool const active);
    // movable rvalues
    explicit optional(typename value_ctor_rv_sink::type x);
    optional(typename value_ctor_rv_sink::type x, bool const active);
    // const lvalues + non-movable rvalues
    template< class U >
    explicit optional(U const & x,
        typename boost::disable_if_c< boost_ext::mpl::or3<
            boost::is_base_of< optional, U >,
            boost_ext::is_convertible< U&, value_rv_param_type >,
            boost_ext::is_convertible< U&, typename value_ctor_rv_sink::type >
        >::value >::type* = 0);
    template< class U >
    optional(U const & x, bool const active,
        typename boost::disable_if_c< boost_ext::mpl::or2<
            boost_ext::is_convertible< U&, value_rv_param_type >,
            boost_ext::is_convertible< U&, typename value_ctor_rv_sink::type >
        >::value >::type* = 0);

    // optional lvalue
    template< class U >
    optional(optional<U>& other,
        
    template< class U >
    optional(U& other,
        typename boost::enable_if_c< boost_ext::mpl::and2<
            boost::mpl::not_< boost::is_base_of<
                optional,
                typename boost::remove_const<U>::type
            > >,
            sake::is_optional<
                typename boost::remove_const<U>::type,
                boost_ext::is_convertible<
                    boost_ext::add_reference<
                        boost_ext::propagate_const<
                            U, boost::mpl::_1
                        >
                    >,
                    T
                >
            >
        >::value >::type* = 0);
    // optional (explicit) rvalue
    template< class U >
    optional(SAKE_RV_REF( optional<U> ) other)

private:
    typedef sake::rv_sink< optional_construct_binder, void, 
public:
    template< class U >
    optional(optional<U> const & other,
        typename boost::disable_if_c< boost_ext::mpl::or3<
            boost::is_same<U,T>,
            boost_ext::is_convertible< optional<U>&, optional_rv_sink_type >,
            boost::mpl::not_< boost_ext::is_convertible<
                typename boost_ext::add_reference_add_const<U>::type, T
            > >
        >::value >::type* = 0);
    template< class U >
    optional(SAKE_RV_REF( optional<U> ) other,
        typename boost::enable_if_c< boost_ext::is_convertible<
            typename boost_ext::add_rvalue_reference<U>::type, T
        >::value >::type* = 0);
    optional(optional_rv_sink_type other);

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U >
    optional(optional<U> const & other,
        typename boost::enable_if_c< boost_ext::is_convertible<
            typename boost_ext::add_reference_add_const<U>::type, T
        >::value >::type* = 0);
    template< class U >
    optional(SAKE_RV_REF( optional<U> ) other,
        typename boost::enable_if< boost_ext::is_convertible<
            typename boost_ext::add_rvalue_reference<U>::type, T
        >::value >::type* = 0);

    optional& operator=(this_copy_assign_param_type other);
    optional& operator=(this_rvalue_param_type other);

    optional& operator=(null_optional_t);

    // template< class T0 >
    // optional&
    // operator=(T0&& x0);
#define SAKE_FORWARDING_ASSIGN_CLASS optional
#define SAKE_FORWARDING_ASSIGN_VALUE_TYPE T
#include SAKE_FORWARDING_ASSIGN_GENERATE()

    template< class Emplacer >
    typename enable_if_is_maybe_typed_emplacer< Emplacer, optional& >::type
    operator=(const Emplacer emplacer);

    template< class U >
    typename boost::enable_if<
        boost::mpl::and_<
            boost::mpl::not_< boost::is_same<T,U> >,
            boost_ext::is_convertible<
                typename boost_ext::add_reference_add_const<U>::type,
                T
            >
        >,
        optional&
    >::type operator=(const optional<U>& other);
    template< class U >
    typename boost::enable_if<
        boost_ext::is_convertible< typename boost_ext::add_rvalue_reference<U>::type, T >,
        optional&
    >::type operator=(SAKE_BOOST_EXT_RV_REF( optional<U> ) other);

    T& get();
    const T& get() const;
    T* get_ptr();
    const T* get_ptr() const;

    T& operator*();
    const T& operator*() const;
    T* operator->();
    const T* operator->() const;

    void reset();
    void reset(null_optional_t);

    void reset(optional& other);
    void reset(this_copy_assign_param_type other);
    void reset(this_rvalue_param_type other);

#define SAKE_OVERLOAD_RV_PARAM_TYPES_SEQ (( T ))
#define SAKE_OVERLOAD_IS_CALLABLE( n, T_tuple ) \
    boost::mpl::and_< \
        boost::mpl::not_< boost::is_same< \
            typename boost_ext::remove_qualifiers< BOOST_PP_APPLY( T_tuple ) >::type, \
            optional \
        > >, \
        boost_ext::is_convertible< BOOST_PP_APPLY( T_tuple ), T > \
    >
#define SAKE_OVERLOAD_MEM_FN_NAME reset
#define SAKE_OVERLOAD_BODY( n, T_tuple, x_tuple, forward_x_tuple ) \
    reset( make_emplacer forward_x_tuple );
#include SAKE_OVERLOAD_GENERATE()

    template< class Emplacer >
    typename enable_if_is_maybe_typed_emplacer< Emplacer >::type
    reset(const Emplacer emplacer);

    template< class U >
    typename boost::enable_if<
        boost::mpl::and_<
            boost::mpl::not_< boost::is_same<T,U> >,
            boost_ext::is_convertible<
                typename boost_ext::add_reference_add_const<U>::type,
                T
            >
        >
    >::type
    reset(const optional<U>& other);
    template< class U >
    typename boost::enable_if< boost_ext::is_convertible<
        typename boost_ext::add_rvalue_reference<U>::type, T
    > >::type
    reset(SAKE_BOOST_EXT_RV_REF( optional<U> ) other);

    bool operator!() const;
    SAKE_EXPLICIT_BOOL()

private:
    union aligned_storage_t
    {
        unsigned char _[sizeof( T ) + 1];
        typename boost::type_with_alignment<
            boost::alignment_of<T>::value
        >::type _aligned;
    };
    BOOST_MPL_ASSERT_RELATION( sizeof( T ) + 1, <=, sizeof( aligned_storage_t ) );

    aligned_storage_t m_storage;
    unsigned char& m_active();
    bool m_active() const;

    void destruct();

    friend class forwarding_core_access;

    template<class> friend struct optional;
    template<class,class,bool> friend struct optional_private::copy_assign_dispatch;
    template<class,class,bool> friend struct optional_private::move_assign_dispatch;
};

/*******************************************************************************
 * struct optional< T& >
 ******************************************************************************/

template< class T >
struct optional< T& >
    : optional_private::base< T& >
{
    typedef T& value_type;

    optional();
    //optional(const optional& other);

    //~optional();

    optional(null_optional_t);

    // template< class T0 >
    // optional(T0&& x0);
#define SAKE_FORWARDING_UNARY_CTOR_CLASS optional
#define SAKE_FORWARDING_UNARY_CTOR_VALUE_TYPE T&
#define SAKE_FORWARDING_UNARY_CTOR_INIT_LIST( T0, x0, forward_x0 ) \
    mp(sake::address_of(emplacer_construct< T& >( x0 )))
#include SAKE_FORWARDING_UNARY_CTOR_GENERATE()

    // template< class T0 >
    // optional(T0&& x0, const bool active);
#define SAKE_OVERLOAD_RV_PARAM_TYPES_SEQ (( T ))
#define SAKE_OVERLOAD_CTOR_NAME optional
#define SAKE_OVERLOAD_DECLARE_PARAMS( n, T_tuple, Tx_tuple ) \
    BOOST_PP_APPLY( Tx_tuple ), const bool active
#define SAKE_OVERLOAD_CTOR_INIT_LIST( n, T_tuple, x_tuple, forward_x_tuple ) \
    mp(active ? sake::address_of(emplacer_construct< T& >(BOOST_PP_APPLY( x_tuple ))) : nullptr)
#include SAKE_OVERLOAD_GENERATE()

    template<class> friend struct optional;
    template< class U >
    explicit optional(optional<U>& other,
        typename boost::enable_if< boost_ext::is_convertible_wnrbt<
            typename boost_ext::add_reference<U>::type,
            T&
        > >::type* = 0);
    template< class U >
    explicit optional(const optional<U>& other,
        typename boost::enable_if< boost_ext::is_convertible_wnrbt<
            typename boost_ext::add_reference_add_const<U>::type,
            T&
        > >::type* = 0);

    //optional& operator=(const optional other);

    optional& operator=(null_optional_t);

    // template< class T0 >
    // optional&
    // operator=(T0&& x0);
#define SAKE_FORWARDING_ASSIGN_CLASS optional
#define SAKE_FORWARDING_ASSIGN_VALUE_TYPE T&
#define SAKE_FORWARDING_ASSIGN_BODY( T0, x0, forward_x0 ) \
    mp = sake::address_of(emplacer_construct< T& >( x0 )); \
    return *this;
#include SAKE_FORWARDING_ASSIGN_GENERATE()

    template< class U >
    typename boost::enable_if<
        boost_ext::is_convertible_wnrbt<
            typename boost_ext::add_reference<U>::type,
            T&
        >,
        optional&
    >::type operator=(optional<U>& other);
    template< class U >
    typename boost::enable_if<
        boost_ext::is_convertible_wnrbt<
            typename boost_ext::add_reference_add_const<U>::type,
            T&
        >,
        optional&
    >::type operator=(const optional<U>& other);

    T& get() const;
    T* get_ptr() const;

    T& operator*() const;
    T* operator->() const;

    void reset();
    void reset(null_optional_t);

    void reset(const optional other);

#define SAKE_OVERLOAD_IS_CALLABLE( n, T_tuple ) \
    boost::mpl::or_< \
        boost_ext::is_convertible_wnrbt< BOOST_PP_APPLY( T_tuple ), T& >, \
        is_maybe_typed_emplacer< \
            typename boost_ext::remove_qualifiers< BOOST_PP_APPLY( T_tuple ) >::type \
        > \
    >
#define SAKE_OVERLOAD_MEM_FN_NAME reset
#define SAKE_OVERLOAD_BODY( n, T_tuple, x_tuple, forward_x_tuple ) \
    reset x_tuple ;
#define SAKE_OVERLOAD_PERFECT_MAX_ARITY 1
#include SAKE_OVERLOAD_GENERATE()

    template< class U >
    typename boost::enable_if< boost_ext::is_convertible_wnrbt<
        typename boost_ext::add_reference<U>::type,
        T&
    > >::type reset(optional<U>& other);
    template< class U >
    typename boost::enable_if< boost_ext::is_convertible_wnrbt<
        typename boost_ext::add_reference_add_const<U>::type,
        T&
    > >::type reset(const optional<U>& other);

    bool operator!() const;
    SAKE_EXPLICIT_BOOL()

private:
    T* mp;

    void destruct();

    friend class forwarding_core_access;

    template<class> friend struct optional;
};

} // namespace optional_adl

/*******************************************************************************
 * extension::has_default_constructor< sake::optional<T> >
 ******************************************************************************/

namespace extension
{

template< class T >
struct has_default_constructor< sake::optional<T>, void >
    : boost::true_type
{ };

} // namespace extension

namespace optional_adl
{

/*******************************************************************************
 * optional comparison operators
 ******************************************************************************/

template< class T0, class T1 >
inline typename boost_ext::common_type<
    bool,
    typename sake::operators::result_of::and_<
        bool,
        typename sake::operators::result_of::equal<
            typename boost_ext::add_reference_add_const< T0 >::type,
            typename boost_ext::add_reference_add_const< T1 >::type
        >::type
    >::type
>::type
operator==(sake::optional< T0 > const & x0, sake::optional< T1 > const & x1)
{
    typename boost_ext::remove_reference_add_const< T0 >::type * p0 = x0.get_ptr();
    typename boost_ext::remove_reference_add_const< T1 >::type * p1 = x1.get_ptr();
    return p0 == 0 ? p1 == 0 : p1 != 0 && *p1 == *p2;
}

#define define_comparison_operator( op, expr ) \
template< class T0, class T1 > \
inline bool operator op (sake::optional< T0 > const & x0, sake::optional< T1 > const & x1) \
{ \
    typename boost_ext::add_const_remove_reference< T0 >::type * p0 = x0.get_ptr(); \
    typename boost_ext::add_const_remove_reference< T1 >::type * p1 = x1.get_ptr(); \
    return expr ; \
}

define_comparison_operator( ==, p0 == nullptr ? p1 == nullptr : p1 != nullptr && *p0 == *p1 )
define_comparison_operator( !=, p0 == nullptr ? p1 != nullptr : p1 == nullptr || *p0 != *p1 )
define_comparison_operator( < , p1 != nullptr && (p0 == nullptr || *p0 < *p1 ) )
define_comparison_operator( > , p0 != nullptr && (p1 == nullptr || *p0 > *p1 ) )
define_comparison_operator( <=, p0 == nullptr || p1 != nullptr && *p0 <= *p1 )
define_comparison_operator( >=, p1 == nullptr || p0 != nullptr && *p0 >= *p1 )

#undef define_comparison_operator

/*******************************************************************************
 * struct optional_private::base
 ******************************************************************************/

namespace optional_private
{

template< class T >
struct base
    : forwarding_base< optional<T>, typename forwarding_base_deduced_params<T>::type >
{
protected:
    typedef forwarding_base< optional<T>, typename forwarding_base_deduced_params<T>::type > forwarding_base_;
    using forwarding_base_::derived;

    friend class forwarding_core_access;

    template< class Signature > struct callable_impl : forwarding_base_deduced_callable< Signature, T > { };
    template< class Signature > struct result_impl : forwarding_base_deduced_result< Signature, T > { };

    typedef typename forwarding_base_::protected_nullary_result_type private_nullary_result_type;

    private_nullary_result_type call_impl()       { return (*derived())(); }
    private_nullary_result_type call_impl() const { return (*derived())(); }

    // template< class T0, ... >
    // typename result_impl< optional<T> ( T0, ... ) >::type
    // call_impl(T0&& x0, ... );
#define SAKE_OVERLOAD_RESULT( n, T_tuple ) result_impl< optional<T> T_tuple >
#define SAKE_OVERLOAD_MEM_FN_NAME          call_impl
#define SAKE_OVERLOAD_BODY( n, T_tuple, x_tuple, forward_x_tuple ) \
    return (*derived()) forward_x_tuple ;
#define SAKE_OVERLOAD_FWD2_MAX_ARITY SAKE_FORWARDING_BASE_MAX_ARITY
#include SAKE_OVERLOAD_GENERATE()

    // template< class T0, ... >
    // typename result_impl< const optional<T> ( T0, ... ) >::type
    // call_impl(T0&& x0, ... ) const;
#define SAKE_OVERLOAD_RESULT( n, T_tuple ) result_impl< const optional<T> T_tuple >
#define SAKE_OVERLOAD_CONST_MEM_FN_NAME    call_impl
#define SAKE_OVERLOAD_BODY( n, T_tuple, x_tuple, forward_x_tuple ) \
    return (*derived()) forward_x_tuple ;
#define SAKE_OVERLOAD_FWD2_MAX_ARITY SAKE_FORWARDING_BASE_MAX_ARITY
#include SAKE_OVERLOAD_GENERATE()

};

} // namespace optional_private

/*******************************************************************************
 * optional member function implementations
 ******************************************************************************/

template< class T >
inline
optional<T>::
optional()
{ m_active() = false; }

template< class T >
inline
optional<T>::
optional(optional const & other)
{
    if(m_active() = other.m_active())
        new(m_storage._) T(*other);
}

template< class T >
inline
optional<T>::
optional(this_rvalue_param_type other)
{
    if(m_active() = other.m_active()) {
        new(m_storage._) T(sake::move(*other));
        other.destruct();
    }
}

template< class T >
inline
optional<T>::
~optional()
{ reset(); }

#ifndef BOOST_NO_RVALUE_REFERENCES

template< class T >
template< class U >
inline
optional<T>::
optional(U&& x)
{
    sake::emplacer_construct<T>(sake::forward<U>(x), m_storage._);
    m_active() = true;
}

template< class T >
template< class U >
inline
optional<T>::
optional(U&& x, bool const active)
{
    if(m_active() = active)
        sake::emplacer_construct<T>(sake::forward<U>(x), m_storage._);
}

template< class T >
template< class U >
inline
optional<T>::
optional(optional<U> const & other,
    typename boost::enable_if_c< boost_ext::is_convertible<
        typename boost_ext::add_reference_add_const<U>::type, T
    >::value >::type*)
{
    if(m_active() = static_cast< bool >(other))
        new(m_storage._) T(*other);
}

template< class T >
template< class U >
inline
optional<T>::
optional(optional<U>&& other,
    typename boost::enable_if_c< boost_ext::is_convertible<
        typename boost_ext::add_rvalue_reference<U>::type, T
    >::value >::type*)
{
    if(m_active() = static_cast< bool >(other)) {
        new(m_storage._) T(sake::move(*other));
        other.destruct();
    }
}

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

template< class T >
template< class U >
inline void
optional<T>::
init(U& x)
{ new(m_storage._) T(x); }

template< class T >
template< class U >
inline
optional<T>::
optional(U& x,
    typename boost::disable_if_c<
        boost::is_base_of< optional, U >::value
    >::type*)
{
    sake::emplacer_construct<T>(x, m_storage._);
    m_active() = true;
}

template< class T >
template< class U >
inline
optional<T>::
optional(U& x, bool const active)
{
    if(m_active() = active)
        sake::emplacer_construct<T>(x, m_storage._);
}

template< class T >
inline
optional<T>::
optional(rparam_type x)
{
    new(m_storage._) T(x);
    m_active() = true;
}

template< class T >
inline
optional<T>::
optional(rparam_type x, bool const active)
{
    if(m_active() = active)
        new(m_storage._) T(x);
}

template< class T >
inline
optional<T>::
optional(rv_sink_type x)
{
    x(construct_binder(m_storage._));
    m_active() = true;
}

template< class T >
inline
optional<T>::
optional(rv_sink_type x, bool const active)
{
    if(m_active() = active)
        x(construct_binder(m_storage._));
}

template< class T >
template< class U >
inline
optional<T>::
optional(U const & x,
    typename boost::disable_if_c<
        boost_ext::mpl::or2<
            boost::is_base_of< optional, U >,
            boost::is_same<U,T>,
            boost_ext::is_convertible< U&, rv_sink_type >
        >::value
    >::type*)
{
    sake::emplacer_construct<T>(x, m_storage._);
    m_active() = true;
}

template< class T >
template< class U >
inline
optional<T>::
optional(U const & x, bool const active,
    typename boost::disable_if_c<
        boost_ext::mpl::or2<
            boost::is_same<U,T>,
            boost_ext::is_convertible< U&, rv_sink_type >
        >::value
    >::type*)
{
    if(m_active() = active)
        sake::emplacer_construct<T>(x, m_storage._);
}

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

template< class T >
template< class U >
inline
optional<T>::
optional(optional<U> const & other,
    typename boost::enable_if< boost::mpl::and_<
        boost::mpl::not_< boost::is_same<U,T> >,
        boost_ext::is_convertible<
            typename boost_ext::add_reference_add_const<U>::type,
            T
        >
    > >::type*)
{
    if(m_active() = static_cast< bool >(other))
        new(m_storage._) T(*other);
}

template< class T >
template< class U >
inline
optional<T>::
optional(SAKE_BOOST_EXT_RV_REF( optional<U> ) other,
    typename boost::enable_if< boost_ext::is_convertible<
        typename boost_ext::add_rvalue_reference<U>::type, T
    > >::type*)
{
    if(m_active() = static_cast< bool >(other)) {
        new(m_storage._) T(boost_ext::move(*other));
        other.destruct();
    }
}

namespace optional_private
{

template<
    class T, class U,
    bool = sake::is_assignable< T, T& ( const U& ) >::value
>
struct copy_assign_dispatch;

template< class T, class U >
struct copy_assign_dispatch< T, U, true >
{
    static void apply(optional<T>& this_, const optional<U>& other)
    {
        if(this_.m_active()) {
            if(other)
                *this_ = *other;
            else
                this_.destruct();
        }
        else if(other) {
            new(this_.m_storage._) T(*other);
            this_.m_active() = true;
        }
    }
};

template< class T, class U >
struct copy_assign_dispatch< T, U, false >
{
    static void apply(optional<T>& this_, const optional<U>& other)
    { this_.reset(other); }
};

} // namespace optional_private

template< class T >
inline optional<T>&
optional<T>::
operator=(this_copy_assign_param_type other)
{
    optional_private::copy_assign_dispatch<T,T>::apply(*this, other);
    return *this;
}

namespace optional_private
{

template<
    class T, class U,
    bool = sake::is_assignable< T, T& ( SAKE_BOOST_EXT_RV_REF( U ) ) >::value
>
struct move_assign_dispatch;

template< class T, class U >
struct move_assign_dispatch< T, U, true >
{
    static void apply(optional<T>& this_, SAKE_BOOST_EXT_RV_REF( optional<U> ) other)
    {
        if(this_.m_active()) {
            if(other) {
                *this_ = boost_ext::move(*other);
                other.destruct();
            }
            else
                this_.destruct();
        }
        else if(other) {
            new(this_.m_storage._) T(boost_ext::move(*other));
            this_.m_active() = true;
            other.destruct();
        }
    }
};

template< class T, class U >
struct move_assign_dispatch< T, U, false >
{
    static void apply(optional<T>& this_, SAKE_BOOST_EXT_RV_REF( optional<U> ) other)
    { this_.reset(boost_ext::move(other)); }
};

} // namespace optional_private

template< class T >
inline optional<T>&
optional<T>::
operator=(this_rvalue_param_type other)
{
    optional_private::move_assign_dispatch<T,T>::apply(*this, boost_ext::move(other));
    return *this;
}

template< class T >
inline optional<T>&
optional<T>::
operator=(null_optional_t)
{
    reset();
    return *this;
}

template< class T >
template< class Emplacer >
typename enable_if_is_maybe_typed_emplacer<
    Emplacer,
    optional<T>&
>::type
optional<T>::
operator=(const Emplacer emplacer)
{
    if(m_active())
        emplacer_assign(get(), emplacer);
    else {
        emplacer_construct<T>(emplacer, m_storage._);
        m_active() = true;
    }
    return *this;
}

template< class T >
template< class U >
inline typename boost::enable_if<
    boost::mpl::and_<
        boost::mpl::not_< boost::is_same<T,U> >,
        boost_ext::is_convertible<
            typename boost_ext::add_reference_add_const<U>::type,
            T
        >
    >,
    optional<T>&
>::type
optional<T>::
operator=(const optional<U>& other)
{
    optional_private::copy_assign_dispatch<T,U>::apply(other);
    return *this;
}

template< class T >
template< class U >
inline typename boost::enable_if<
    boost_ext::is_convertible< typename boost_ext::add_rvalue_reference<U>::type, T >,
    optional<T>&
>::type
optional<T>::
operator=(SAKE_BOOST_EXT_RV_REF( optional<U> ) other)
{
    optional_private::move_assign_dispatch<T,U>::apply(other);
    return *this;
}

template< class T >
inline T&
optional<T>::
get()
{
    SAKE_ASSERT((m_active()));
    return *reinterpret_cast< T* >(m_storage._);
}

template< class T >
inline const T&
optional<T>::
get() const
{
    SAKE_ASSERT((m_active()));
    return *reinterpret_cast< const T* >(m_storage._);
}

template< class T >
inline T*
optional<T>::
get_ptr()
{ return m_active() ? reinterpret_cast< T* >(m_storage._) : nullptr; }

template< class T >
inline const T*
optional<T>::
get_ptr() const
{ return m_active() ? reinterpret_cast< const T* >(m_storage._) : nullptr; }

template< class T >
inline T&
optional<T>::
operator*()
{ return get(); }

template< class T >
inline const T&
optional<T>::
operator*() const
{ return get(); }

template< class T >
inline T*
optional<T>::
operator->()
{
    SAKE_ASSERT((m_active()));
    return reinterpret_cast< T* >(m_storage._);
}

template< class T >
inline const T*
optional<T>::
operator->() const
{
    SAKE_ASSERT((m_active()));
    return reinterpret_cast< const T* >(m_storage._);
}

template< class T >
inline void
optional<T>::
reset()
{
    if(m_active())
        destruct();
}

template< class T >
inline void
optional<T>::
reset(null_optional_t)
{ reset(); }

template< class T >
inline void
optional<T>::
reset(optional& other)
{ reset(static_cast< const optional& >(other)); }

template< class T >
inline void
optional<T>::
reset(this_copy_assign_param_type other)
{
    if(other)
        reset(*other);
    else
        reset();
}

template< class T >
inline void
optional<T>::
reset(this_rvalue_param_type other)
{
    if(other) {
        reset(boost_ext::move(*other));
        other.destruct();
    }
    else
        reset();
}

template< class T >
template< class Emplacer >
inline typename enable_if_is_maybe_typed_emplacer< Emplacer >::type
optional<T>::
reset(const Emplacer emplacer)
{
    reset();
    emplacer_construct<T>(emplacer, m_storage._);
    m_active() = true;
}

template< class T >
template< class U >
inline typename boost::enable_if<
    boost::mpl::and_<
        boost::mpl::not_< boost::is_same<T,U> >,
        boost_ext::is_convertible<
            typename boost_ext::add_reference_add_const<U>::type,
            T
        >
    >
>::type
optional<T>::
reset(const optional<U>& other)
{
    if(other)
        reset(*other);
    else
        reset();
}

template< class T >
template< class U >
inline typename boost::enable_if< boost_ext::is_convertible<
    typename boost_ext::add_rvalue_reference<U>::type, T
> >::type
optional<T>::
reset(SAKE_BOOST_EXT_RV_REF( optional<U> ) other)
{
    if(other) {
        reset(boost_ext::move(*other));
        other.destruct();
    }
    else
        reset();
}

template< class T >
inline bool
optional<T>::
operator!() const
{ return !m_active(); }

template< class T >
inline unsigned char&
optional<T>::
m_active()
{ return m_storage._[sizeof( T )]; }

template< class T >
inline bool
optional<T>::
m_active() const
{ return m_storage._[sizeof( T )] != 0; }

template< class T >
inline void
optional<T>::
destruct()
{
    SAKE_ASSERT((m_active()));
    reinterpret_cast< T* >(m_storage._)->~T();
    m_active() = false;
}

/*******************************************************************************
 * optional< T& > member function implementations
 ******************************************************************************/

template< class T >
inline
optional< T& >::
optional()
    : mp(nullptr)
{ }

template< class T >
inline
optional< T& >::
optional(null_optional_t)
    : mp(nullptr)
{ }

template< class T >
template< class U >
inline
optional< T& >::
optional(optional<U>& other,
    typename boost::enable_if< boost_ext::is_convertible_wnrbt<
        typename boost_ext::add_reference<U>::type,
        T&
    > >::type*)
    : mp(nullptr)
{
    if(other)
        mp = sake::address_of(static_cast< T& >(*other));
}

template< class T >
template< class U >
inline
optional< T& >::
optional(const optional<U>& other,
    typename boost::enable_if< boost_ext::is_convertible_wnrbt<
        typename boost_ext::add_reference_add_const<U>::type,
        T&
    > >::type*)
    : mp(nullptr)
{
    if(other)
        mp = sake::address_of(static_cast< T& >(*other));
}

template< class T >
inline optional< T& >&
optional< T& >::
operator=(null_optional_t)
{
    reset();
    return *this;
}

template< class T >
template< class U >
inline typename boost::enable_if<
    boost_ext::is_convertible_wnrbt<
        typename boost_ext::add_reference<U>::type,
        T&
    >,
    optional< T& >&
>::type
optional< T& >::
operator=(optional<U>& other)
{
    reset(other);
    return *this;
}

template< class T >
template< class U >
inline typename boost::enable_if<
    boost_ext::is_convertible_wnrbt<
        typename boost_ext::add_reference_add_const<U>::type,
        T&
    >,
    optional< T& >&
>::type
optional< T& >::
operator=(const optional<U>& other)
{
    reset(other);
    return *this;
}

template< class T >
inline T&
optional< T& >::
get() const
{
    SAKE_ASSERT_RELATION( mp, !=, nullptr );
    return *mp;
}

template< class T >
inline T*
optional< T& >::
get_ptr() const
{ return mp; }

template< class T >
inline T&
optional< T& >::
operator*() const
{ return get(); }

template< class T >
inline T*
optional< T& >::
operator->() const
{
    SAKE_ASSERT_RELATION( mp, !=, nullptr );
    return mp;
}

template< class T >
inline void
optional< T& >::
reset()
{ mp = nullptr; }

template< class T >
inline void
optional< T& >::
reset(null_optional_t)
{ mp = nullptr; }

template< class T >
inline void
optional< T& >::
reset(const optional other)
{ mp = other.mp; }

template< class T >
template< class U >
inline typename boost::enable_if< boost_ext::is_convertible_wnrbt<
    typename boost_ext::add_reference<U>::type,
    T&
> >::type
optional< T& >::
reset(optional<U>& other)
{
    if(other)
        reset(static_cast< T& >(*other));
    else
        reset();
}

template< class T >
template< class U >
inline typename boost::enable_if< boost_ext::is_convertible_wnrbt<
    typename boost_ext::add_reference_add_const<U>::type,
    T&
> >::type
optional< T& >::
reset(const optional<U>& other)
{
    if(other)
        reset(static_cast< T& >(*other));
    else
        reset();
}

template< class T >
inline bool
optional< T& >::
operator!() const
{ return mp == nullptr; }

template< class T >
inline void
optional< T& >::
destruct()
{ mp = nullptr; }

} // namespace sake

#endif // #ifndef SAKE_CORE_DATA_STRUCTURES_OPTIONAL_HPP

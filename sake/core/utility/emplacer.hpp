/*******************************************************************************
 * sake/core/utility/emplacer.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * class emplacer< void ( U0, ... ) >
 * class emplacer< T ( U0, ... ) >
 *
 * emplacer_construct<T>(emplacer< Signature > e, void* p) -> void
 * emplacer_construct<T>(emplacer< Signature > e) -> T [qualified]
 * emplacer_assign(T& x, emplacer< Signature > e) -> T&
 *
 * make_emplacer(U0&& y0, ...) -> emplacer< void ( U0, ... ) >
 * make_typed_emplacer<T>(U0&& y0, ...) -> emplacer< T ( U0, ... ) >
 *
 * emplacer_wrap(U&& y) -> result_of::emplacer_wrap<U>::type
 * typed_emplacer_wrap<T>(U&& y) -> result_of::typed_emplacer_wrap<T,U>::type
 *
 * emplacer objects provide an abstraction for constructor arguments, allowing
 * one a generic facility to pass constructor arguments and do in-place
 * construction.  This is vaguely similar to Boost.InPlaceFactory and
 * Boost.TypedInPlaceFactory.
 *
 * An emplacer object holds bound constructor arguments.  A bound constructor
 * argument is held by lvalue reference if the corresponding Ui is an lvalue
 * reference; by value if the corresponding Ui is a non-reference and
 * by-value-optimal; by reference-to-const if the corresponding Ui is a
 * non-reference, not by-value-optimal, and non-movable; and by (emulated)
 * rvalue reference otherwise.  One consequence is that emplacers are very cheap
 * to copy (no more expensive than passing each of the bound arguments as
 * separate function arguments) and hence should always be passed by value.
 *
 * [typed_]emplacer_wrap is identical to the one-argument form of
 * make_[typed_]emplacer except when the argument is already an emplacer object,
 * in which case it amounts to an identity operation.
 ******************************************************************************/

#ifndef BOOST_PP_IS_ITERATING

#ifndef SAKE_CORE_UTILITY_EMPLACER_HPP
#define SAKE_CORE_UTILITY_EMPLACER_HPP

#include <boost/config.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/preprocessor/tuple/rem.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/is_volatile.hpp>
#include <boost/type_traits/is_object.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_void.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/utility/result_of.hpp>

#include <sake/boost_ext/type_traits/is_same_sans_qualifiers.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/move/forward.hpp>
#include <sake/core/utility/emplacer_access.hpp>
#include <sake/core/utility/emplacer_fwd.hpp>
#include <sake/core/utility/non_copy_assignable.hpp>
#include <sake/core/utility/overload.hpp>
#include <sake/core/utility/private/emplacer/base.hpp>
#include <sake/core/utility/private/emplacer/traits.hpp>
#include <sake/core/utility/type_tag.hpp>

namespace sake
{

/*******************************************************************************
 * class emplacer< void ( ) >
 ******************************************************************************/

template<>
class emplacer< void ( ) >
{
public:
    typedef void value_type;

    template< class > struct result;
    template< class This, class P, class T >
    struct result< This ( P, T ) >
    { typedef void type; };
    template< class This, class T >
    struct result< This ( T ) >
        : sake::remove_type_tag<
              typename boost_ext::remove_qualifiers<T>::type
          >
    { };

    template< class T >
    static void
    construct(void* const p)
    { emplacer_access::construct<T>(p, emplacer()); }

    template< class T >
    static typename result< emplacer const ( T ) >::type
    construct()
    { return emplacer_access::construct<T>(emplacer()); }

    template< class T >
    static T&
    assign(T& x)
    { return emplacer_access::assign(x, emplacer()); }

    struct result_of
    {
        template< class T >
        struct as_typed
        { typedef emplacer< T ( ) > type; };
    };
    template< class T >
    static typename result_of::as_typed<T>::type
    as_typed()
    {
        BOOST_STATIC_ASSERT((!boost::is_void<T>::value));
        return typename result_of::as_typed<T>::type();
    }

    template< class T >
    void
    operator()(void* const p, sake::type_tag<T>) const
    { construct<T>(p); }

    template< class T >
    typename result< emplacer const ( T ) >::type
    operator()(sake::type_tag<T>) const
    { return construct<T>(); }
};

/*******************************************************************************
 * class emplacer< T ( ) >
 ******************************************************************************/

template< class T >
class emplacer< T ( ) >
{
    BOOST_STATIC_ASSERT((boost::is_object<T>::value));
    BOOST_STATIC_ASSERT(!(boost::is_const<T>::value));
    BOOST_STATIC_ASSERT(!(boost::is_volatile<T>::value));
public:
    typedef T value_type;

    typedef T result_type;

    static void
    construct(void* const p)
    { emplacer_access::construct<T>(p, emplacer()); }

    static result_type
    construct()
    { return emplacer_access::construct<T>(emplacer()); }

    template< class V >
    V&
    assign(V& x)
    { return emplacer_access::assign(x, emplacer()); }

    void
    operator()(void* const p) const
    { construct(p); }

    result_type
    operator()() const
    { return construct(); }
};

/*******************************************************************************
 * class emplacer< void ( U0 ) >
 ******************************************************************************/

template< class U0 >
class emplacer< void ( U0 ) >
    : public emplacer_private::base< void ( U0 ) >
{
    typedef emplacer_private::base< void ( U0 ) > base_;
public:
    SAKE_NON_COPY_ASSIGNABLE( emplacer )

    typedef void value_type;

    template< class > struct result;
    template< class This, class P, class T >
    struct result< This ( P, T ) >
    { typedef void type; };
    template< class This, class T >
    struct result< This ( T ) >
    {
        typedef typename sake::remove_type_tag<
            typename boost_ext::remove_qualifiers<T>::type
        >::type T_;
        typedef typename boost::mpl::if_c<
            boost_ext::is_same_sans_qualifiers< T_, U0 >::value,
            typename emplacer_private::traits< U0 >::type,
            T_
        >::type type;
    };

    explicit emplacer(typename emplacer_private::traits< U0 >::type y)
        : base_(emplacer_private::traits< U0 >::cast(y))
    { }

    template< class T >
    void
    construct(void* const p) const
    { emplacer_access::construct<T>(p, *this); }

    template< class T >
    typename result< emplacer const ( T ) >::type
    construct() const
    { return emplacer_access::construct<T>(*this); }

    template< class T >
    T&
    assign(T& x) const
    { return emplacer_access::assign(x, *this); }

    template< class T >
    void
    operator()(void* const p, sake::type_tag<T>) const
    { construct<T>(p); }

    template< class T >
    typename result< emplacer const ( T ) >::type
    operator()(sake::type_tag<T>) const
    { return construct<T>(); }
};

/*******************************************************************************
 * class emplacer< T ( U0 ) >
 ******************************************************************************/

template< class T, class U0 >
class emplacer< T ( U0 ) >
    : public emplacer_private::base< void ( U0 ) >
{
    BOOST_STATIC_ASSERT(!(boost::is_const<T>::value));
    BOOST_STATIC_ASSERT(!(boost::is_volatile<T>::value));
    typedef emplacer_private::base< void ( U0 ) > base_;
public:
    SAKE_NON_COPY_ASSIGNABLE( emplacer )

    typedef T value_type;

    typedef typename boost::mpl::if_c<
        boost_ext::is_same_sans_qualifiers< T, U0 >::value,
        typename emplacer_private::traits< U0 >::type,
        T
    >::type result_type;

    explicit emplacer(typename emplacer_private::traits< U0 >::type y)
        : base_(emplacer_private::traits< U0 >::cast(y))
    { }

    explicit emplacer(base_ e)
        : base_(e)
    { }

    void
    construct(void* const p) const
    { emplacer_access::construct<T>(p, *this); }

    result_type
    construct() const
    { return emplacer_access::construct<T>(*this); }

    template< class V >
    V&
    assign(V& x) const
    { return emplacer_access::assign(x, *this); }

    void
    operator()(void* const p) const
    { construct(p); }

    result_type
    operator()() const
    { return construct(); }
};

#ifndef BOOST_NO_VARIADIC_TEMPLATES

/*******************************************************************************
 * class emplacer< void ( U0, U1, U... ) >
 ******************************************************************************/

template< class U0, class U1, class... U >
class emplacer< void ( U0, U1, U... ) >
    : public emplacer_private::base< void ( U0, U1, U... ) >
{
    typedef emplacer_private::base< void ( U0, U1, U... ) > base_;
public:
    SAKE_NON_COPY_ASSIGNABLE( emplacer )

    typedef void value_type;

    template< class > struct result;
    template< class This, class P, class T >
    struct result< This ( P, T ) >
    { typedef void type; };
    template< class This, class T >
    struct result< This ( T ) >
        : sake::remove_type_tag<
              typename boost_ext::remove_qualifiers<T>::type
          >
    { };

    emplacer(
        typename emplacer_private::traits< U0 >::type y0,
        typename emplacer_private::traits< U1 >::type y1,
        typename emplacer_private::traits<U>::type... y)
        : base_(
              emplacer_private::traits< U0 >::cast(y0),
              emplacer_private::traits< U1 >::cast(y1),
              emplacer_private::traits<U>::cast(y)...
          )
    { }

    template< class T >
    void
    construct(void* const p) const
    { emplacer_access::construct<T>(p, *this); }

    template< class T >
    typename result< emplacer const ( T ) >::type
    construct() const
    { return emplacer_access::construct<T>(*this); }

    template< class T >
    T&
    assign(T& x) const
    { return emplacer_access::assign(x, *this); }

    struct result_of
    {
        template< class T >
        struct as_typed
        { typedef emplacer< T ( U0, U1, U... ) > type; };
    };
    template< class T >
    typename result_of::as_typed<T>::type
    as_typed() const
    {
        BOOST_STATIC_ASSERT((!boost::is_void<T>::value));
        return typename result_of::as_typed<T>::type(*this);
    }

    template< class T >
    void
    operator()(void* const p, sake::type_tag<T>) const
    { construct<T>(p); }

    template< class T >
    typename result< emplacer const ( T ) >::type
    operator()(sake::type_tag<T>) const
    { return construct<T>(); }
};

/*******************************************************************************
 * class emplacer< T ( U0, U1, U... ) >
 ******************************************************************************/

template< class T, class U0, class U1, class... U >
class emplacer< T ( U0, U1, U... ) >
    : public emplacer_private::base< void ( U0, U1, U... ) >
{
    BOOST_STATIC_ASSERT((boost_ext::is_object<T>::value));
    BOOST_STATIC_ASSERT(!(boost::is_const<T>::value));
    BOOST_STATIC_ASSERT(!(boost::is_volatile<T>::value));
    typedef emplacer_private::base< void ( U0, U1, U... ) > base_;
public:
    SAKE_NON_COPY_ASSIGNABLE( emplacer )

    typedef T value_type;

    typedef T result_type;

    emplacer(
        typename emplacer_private::traits< U0 >::type y0,
        typename emplacer_private::traits< U1 >::type y1,
        typename emplacer_private::traits<U>::type... y)
        : base_(
              emplacer_private::traits< U0 >::cast(y0),
              emplacer_private::traits< U1 >::cast(y1),
              emplacer_private::traits<U>::cast(y)...
          )
    { }

    explicit emplacer(base_ e)
        : base_(e)
    { }

    void
    construct(void* const p) const
    { emplacer_access::construct<T>(p, *this); }

    result_type
    construct() const
    { return emplacer_access::construct<T>(*this); }

    template< class V >
    V&
    assign(V& x) const
    { return emplacer_access::assign(x, *this); }

    void
    operator()(void* const p) const
    { construct(p); }

    result_type
    operator()() const
    { return construct(); }
};

#else // #ifndef BOOST_NO_VARIADIC_TEMPLATES

#define traits_Un_type_yn( z, n, data ) \
    typename emplacer_private::traits< BOOST_PP_CAT( U, n ) >::type BOOST_PP_CAT( y, n )
#define traits_Un_cast_yn( z, n, data ) \
    emplacer_private::traits< BOOST_PP_CAT( U, n ) >::cast( BOOST_PP_CAT( y, n ) )

#define BOOST_PP_ITERATION_LIMITS ( 2, SAKE_EMPLACER_MAX_ARITY )
#define BOOST_PP_FILENAME_1       <sake/core/utility/emplacer.hpp>
#include BOOST_PP_ITERATE()

#undef traits_Un_type_yn
#undef traits_Un_cast_yn

#endif // #ifndef BOOST_NO_VARIADIC_TEMPLATES

/*******************************************************************************
 * emplacer_construct<T>(emplacer< Signature > e, void* p) -> void
 * emplacer_construct<T>(emplacer< Signature > e) -> T [qualified]
 * emplacer_assign(T& x, emplacer< Signature > e) -> T&
 ******************************************************************************/

template< class T, class Signature >
inline typename boost::enable_if_c<
    boost::is_void< typename sake::emplacer< Signature >::value_type >::value
>::type
emplacer_construct(sake::emplacer< Signature > e, void* const p)
{ e.template construct<T>(p); }

template< class T, class Signature >
inline typename boost::enable_if_c<
    boost::is_same< T, typename sake::emplacer< Signature >::value_type >::value
>::type
emplacer_construct(sake::emplacer< Signature > e, void* const p)
{ e.construct(p); }

template< class T, class Signature >
inline typename boost::enable_if_c<
    boost::is_void< typename sake::emplacer< Signature >::value_type >::value,
    typename boost::result_of< sake::emplacer< Signature > const ( T ) >::type
>::type
emplacer_construct(sake::emplacer< Signature > e)
{ return e.template construct<T>(); }

template< class T, class Signature >
inline typename boost::enable_if_c<
    boost::is_same< T, typename sake::emplacer< Signature >::value_type >::value,
    typename sake::emplacer< Signature >::result_type
>::type
emplacer_construct(sake::emplacer< Signature > e)
{ return e.construct(); }

template< class T, class Signature >
inline T&
emplacer_assign(T& x, sake::emplacer< Signature > e)
{ return e.assign(x); }

/*******************************************************************************
 * make_emplacer(U0&& y0, ...) -> emplacer< void ( U0, ... ) >
 * make_typed_emplacer<T>(U0&& y0, ...) -> emplacer< T ( U0, ... ) >
 ******************************************************************************/

#if !defined( BOOST_NO_RVALUE_REFERENCES ) \
 && !defined( BOOST_NO_VARIADIC_TEMPLATES )

template< class... U >
inline sake::emplacer< void ( U... ) >
make_emplacer(U&&... y)
{ return sake::emplacer< void ( U... ) >(sake::forward<U>(y)...); }

template< class T, class... U >
inline sake::emplacer< T ( U... ) >
make_typed_emplacer(U&&... y)
{ return sake::emplacer< T ( U... ) >(sake::forward<U>(y)...); }

#else // #if !defined( ... ) && !defined( ... )

inline sake::emplacer< void ( ) >
make_emplacer()
{ return sake::emplacer< void ( ) >(); }

#define SAKE_OVERLOAD_RESULT( n, U_tuple ) \
    boost::mpl::identity< sake::emplacer< void U_tuple > >
#define SAKE_OVERLOAD_FUNCTION_NAME \
    make_emplacer
#define SAKE_OVERLOAD_BODY( n, U_tuple, y_tuple, forward_y_tuple ) \
    return sake::emplacer< void U_tuple > forward_y_tuple ;
#define SAKE_OVERLOAD_PERFECT_MAX_ARITY SAKE_EMPLACER_PERFECT_MAX_ARITY
#define SAKE_OVERLOAD_FWD_MAX_ARITY     SAKE_EMPLACER_FWD_MAX_ARITY
#define SAKE_OVERLOAD_FWD2_MAX_ARITY    SAKE_EMPLACER_FWD2_MAX_ARITY
#include SAKE_OVERLOAD_GENERATE()

template< class T >
inline sake::emplacer< T ( ) >
make_typed_emplacer()
{ return sake::emplacer< T ( ) >(); }

#define SAKE_OVERLOAD_DECLARE_TEMPLATE_PARAMS( n, classU_tuple ) \
    class T, BOOST_PP_TUPLE_REM_CTOR( n, classU_tuple )
#define SAKE_OVERLOAD_RESULT( n, U_tuple ) \
    boost::mpl::identity< sake::emplacer< T U_tuple > >
#define SAKE_OVERLOAD_FUNCTION_NAME \
    make_typed_emplacer
#define SAKE_OVERLOAD_BODY( n, U_tuple, y_tuple, forward_y_tuple ) \
    return sake::emplacer< T U_tuple > forward_y_tuple ;
#define SAKE_OVERLOAD_PERFECT_MAX_ARITY SAKE_EMPLACER_PERFECT_MAX_ARITY
#define SAKE_OVERLOAD_FWD_MAX_ARITY     SAKE_EMPLACER_FWD_MAX_ARITY
#define SAKE_OVERLOAD_FWD2_MAX_ARITY    SAKE_EMPLACER_FWD2_MAX_ARITY
#include SAKE_OVERLOAD_GENERATE()

#endif // #if !defined( ... ) && !defined( ... )

/*******************************************************************************
 * emplacer_wrap(U&& y) -> result_of::emplacer_wrap<U>::type
 * typed_emplacer_wrap<T>(U&& y) -> result_of::typed_emplacer_wrap<T,U>::type
 ******************************************************************************/

namespace emplacer_private
{

template<
    class U,
    class U_ = typename boost_ext::remove_qualifiers<U>::type,
    bool = sake::is_emplacer<U_>::value
>
struct wrap_dispatch;

template<
    class T, class U,
    class U_ = typename boost_ext::remove_qualifiers<U>::type,
    bool = sake::is_emplacer<
               U_, boost::mpl::quote1< boost::is_void >
           >::value,
    bool = sake::is_emplacer<
               U_, boost::mpl::not_< boost::is_void< boost::mpl::_1 > >
           >::value
>
struct typed_wrap_dispatch;

} // namespace emplacer_private

namespace result_of
{

template< class U >
struct emplacer_wrap
    : emplacer_private::wrap_dispatch<U>
{ };

template< class T, class U >
struct typed_emplacer_wrap
    : emplacer_private::typed_wrap_dispatch<T,U>
{ };

} // namespace result_of

template< class U >
inline typename result_of::emplacer_wrap< SAKE_FWD2_PARAM( U ) >::type
emplacer_wrap(SAKE_FWD2_REF( U ) x)
{ return emplacer_private::wrap_dispatch< SAKE_FWD2_PARAM( U ) >::
        apply(sake::forward<U>(x)); }

template< class T, class U >
inline typename result_of::typed_emplacer_wrap< T, SAKE_FWD2_PARAM( U ) >::type
typed_emplacer_wrap(SAKE_FWD2_REF( U ) x)
{ return emplacer_private::typed_wrap_dispatch< T, SAKE_FWD2_PARAM( U ) >::
        apply(sake::forward<U>(x)); }

/*******************************************************************************
 * namespace emplacer_private
 ******************************************************************************/

namespace emplacer_private
{

template< class U, class U_ >
struct wrap_dispatch< U, U_, false >
{
    typedef sake::emplacer< void ( U ) > type;
    static type apply(typename traits<U>::type x)
    { return type(traits<U>::cast(x)); }
};

template< class U, class U_ >
struct wrap_dispatch< U, U_, true >
{
    typedef U_ type;
    static type apply(type e)
    { return e; }
};

template< class T, class U, class U_ >
struct typed_wrap_dispatch< T, U, U_, false, false >
{
    typedef sake::emplacer< T ( U ) > type;
    static type apply(typename traits<U>::type x)
    { return type(traits<U>::cast(x)); }
};

template< class T, class U, class U_ >
struct typed_wrap_dispatch< T, U, U_, true, false >
{
    typedef typename U_::result_of::template as_typed<T>::type type;
    static type apply(U_ e)
    { return e.template as_typed<T>(); }
};

template< class T, class U, class U_ >
struct typed_wrap_dispatch< T, U, U_, false, true >
{
    typedef U_ type;
    static type apply(type e)
    { return e; }
};

} // namespace emplacer_private

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_EMPLACER_HPP

#else // #ifndef BOOST_PP_IS_ITERATING

#define N BOOST_PP_ITERATION()

#define class_U0N       BOOST_PP_ENUM_PARAMS( N, class U )
#define comma_class_U0N BOOST_PP_ENUM_TRAILING_PARAMS( N, class U )
#define U0N             BOOST_PP_ENUM_PARAMS( N, U )

#define traits_U0N_type_y0N BOOST_PP_ENUM( N, traits_Un_type_yn, ~ )
#define traits_U0N_cast_y0N BOOST_PP_ENUM( N, traits_Un_cast_yn, ~ )

/*******************************************************************************
 * class emplacer< void ( U0, ... ) >
 ******************************************************************************/

template< class_U0N >
class emplacer< void ( U0N ) >
    : public emplacer_private::base< void ( U0N ) >
{
    typedef emplacer_private::base< void ( U0N ) > base_;
public:
    SAKE_NON_COPY_ASSIGNABLE( emplacer )

    typedef void value_type;

    emplacer(traits_U0N_type_y0N)
        : base_(traits_U0N_cast_y0N)
    { }

    template< class T >
    void
    construct(void* const p)
    { emplacer_access::construct<T>(p, *this); }

    template< class T >
    T
    construct()
    { return emplacer_access::construct<T>(*this); }

    template< class T >
    T&
    assign(T& x)
    { return emplacer_access::assign(x, *this); }

    struct result_of
    {
        template< class T >
        struct as_typed
        { typedef emplacer< T ( U0N ) > type; };
    };
    template< class T >
    typename result_of::template as_typed<T>::type
    as_typed()
    {
        BOOST_STATIC_ASSERT((!boost::is_void<T>::value));
        return typename result_of::template as_typed<T>::type(*this);
    }

    template< class > struct result;
    template< class This, class P, class T >
    struct result< This ( P, T ) >
    { typedef void type; };
    template< class This, class T >
    struct result< This ( T ) >
        : sake::remove_type_tag<
              typename boost_ext::remove_qualifiers<T>::type
          >
    { };

    template< class T >
    void
    operator()(void* const p, sake::type_tag<T>) const
    { construct<T>(p); }

    template< class T >
    T
    operator()(sake::type_tag<T>) const
    { return construct<T>(); }
};

/*******************************************************************************
 * class emplacer< T ( U0, ... ) >
 ******************************************************************************/

template< class T comma_class_U0N >
class emplacer< T ( U0N ) >
    : public emplacer_private::base< void ( U0N ) >
{
    BOOST_STATIC_ASSERT((boost::is_object<T>::value));
    BOOST_STATIC_ASSERT(!(boost::is_const<T>::value));
    BOOST_STATIC_ASSERT(!(boost::is_volatile<T>::value));
    typedef emplacer_private::base< void ( U0N ) > base_;
public:
    SAKE_NON_COPY_ASSIGNABLE( emplacer )

    typedef T value_type;
    typedef T result_type;

    emplacer(traits_U0N_type_y0N)
        : base_(traits_U0N_cast_y0N)
    { }

    explicit emplacer(base_ e)
        : base_(e)
    { }

    void
    construct(void* const p) const
    { emplacer_access::construct<T>(p, *this); }

    result_type
    construct() const
    { return emplacer_access::construct<T>(*this); }

    template< class V >
    V&
    assign(V& x) const
    { return emplacer_access::assign(x, *this); }

    result_type
    operator()() const
    { return construct(); }

    void
    operator()(void* const p) const
    { construct(p); }
};

#undef traits_U0N_type_y0N
#undef traits_U0N_cast_y0N

#undef class_U0N
#undef comma_class_U0N
#undef U0N

#undef N

#endif // #ifndef BOOST_PP_IS_ITERATING

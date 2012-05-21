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
 * emplacer_construct<T>(emplacer< Signature > e) -> T
 * emplacer_constructible<T>(emplacer< Signature > e) -> [unspecified]
 *
 * emplacer objects provide an abstraction for constructor arguments, allowing
 * one a generic facility to pass constructor arguments and do in-place
 * construction.  This is similar to Boost.InPlaceFactory and
 * Boost.TypedInPlaceFactory.
 *
 * An emplacer object holds bound constructor arguments.  A bound constructor
 * argument is held by lvalue reference if the corresponding Ui is an lvalue
 * reference; by value if the corresponding Ui is a non-reference and
 * by-value-optimal; by reference-to-const if the corresponding Ui is a
 * non-reference, not by-value-optimal, and non-movable; and by (emulated)
 * rvalue reference otherwise.  One consequence is that emplacers are very cheap
 * to copy (no more expensive than passing each of the bound arguments as
 * separate function arguments) and hence should be passed by value when
 * convenient.
 ******************************************************************************/

#ifndef BOOST_PP_IS_ITERATING

#ifndef SAKE_CORE_EMPLACER_HPP
#define SAKE_CORE_EMPLACER_HPP

#include <boost/config.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_object.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_void.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/utility/result_of.hpp>

#include <sake/boost_ext/mpl/or.hpp>

#include <sake/core/emplacer/access.hpp>
#include <sake/core/emplacer/fwd.hpp>
#include <sake/core/emplacer/private/base.hpp>
#include <sake/core/emplacer/private/cast.hpp>
#include <sake/core/utility/noncopy_assignable.hpp>
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

    template< class T >
    static void construct(void* const p)
    { emplacer_access::construct<T>(emplacer(), p); }

    template< class T >
    static T construct()
    { return emplacer_access::construct<T>(emplacer()); }

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

    template< class > struct result;
    template< class This, class P, class T >
    struct result< This ( P, T ) >
    { typedef void type; };
    template< class This, class T >
    struct result< This ( T ) >
        : sake::remove_qualified_type_tag<T>
    { };

    template< class T >
    void operator()(void* const p, sake::type_tag<T>) const
    { construct<T>(p); }
    template< class T >
    T operator()(sake::type_tag<T>) const
    { return construct<T>(); }
};

/*******************************************************************************
 * class emplacer< T ( ) >
 ******************************************************************************/

template< class T >
class emplacer< T ( ) >
{
    BOOST_STATIC_ASSERT((boost::is_object<T>::value));
    BOOST_STATIC_ASSERT((!boost_ext::is_cv_or<T>::value));
public:
    typedef T value_type;

    static void construct(void* const p)
    { emplacer_access::construct<T>(emplacer(), p); }

    static T construct()
    { return emplacer_access::construct<T>(emplacer()); }

    typedef T result_type;
    void operator()(void* const p) const { construct(p); }
    T operator()() const { return construct(); }
};

#ifndef BOOST_NO_VARIADIC_TEMPLATES

/*******************************************************************************
 * class emplacer< void ( U0, U... ) >
 ******************************************************************************/

template< class U0, class... U >
class emplacer< void ( U0, U... ) >
    : public emplacer_private::base< void ( U0, U... ) >
{
    typedef emplacer_private::base< void ( U0, U... ) > base_;
public:
    SAKE_NONCOPY_ASSIGNABLE( emplacer )

    typedef void value_type;

    emplacer(
        typename emplacer_private::cast< U0 >::type y0,
        typename emplacer_private::cast<U>::type... y)
        : base_(emplacer_private::cast< U0 >::apply(y0),
                emplacer_private::cast<U>::apply(y)...)
    { }

    template< class T >
    void construct(void* const p) const
    { emplacer_access::construct<T>(*this, p); }

    template< class T >
    T construct() const
    { return emplacer_access::construct<T>(*this); }

    struct result_of
    {
        template< class T >
        struct as_typed
        { typedef emplacer< T ( U0, U... ) > type; };
    };
    template< class T >
    typename result_of::as_typed<T>::type
    as_typed() const
    {
        BOOST_STATIC_ASSERT((!boost::is_void<T>::value));
        return typename result_of::as_typed<T>::type(*this);
    }

    template< class > struct result;
    template< class This, class P, class T >
    struct result< This ( P, T ) >
    { typedef void type; };
    template< class This, class T >
    struct result< This ( T ) >
        : sake::remove_qualified_type_tag<T>
    { };

    template< class T >
    void operator()(void* const p, sake::type_tag<T>) const
    { construct<T>(p); }
    template< class T >
    T operator()(sake::type_tag<T>) const
    { return construct<T>(); }
};

/*******************************************************************************
 * class emplacer< T ( U0, U... ) >
 ******************************************************************************/

template< class T, class U0, class... U >
class emplacer< T ( U0, U... ) >
    : public emplacer_private::base< void ( U0, U... ) >
{
    BOOST_STATIC_ASSERT(((sizeof...( U ) == 0) || boost::is_object<T>::value));
    BOOST_STATIC_ASSERT((!boost_ext::is_cv_or<T>::value));
    typedef emplacer_private::base< void ( U0, U... ) > base_;
public:
    SAKE_NONCOPY_ASSIGNABLE( emplacer )

    typedef T value_type;

    emplacer(
        typename emplacer_private::cast< U0 >::type y0,
        typename emplacer_private::cast<U>::type... y)
        : base_(emplacer_private::cast< U0 >::apply(y0),
                emplacer_private::cast<U>::apply(y)...)
    { }

    explicit emplacer(base_ e)
        : base_(e)
    { }

    void construct(void* const p) const
    { emplacer_access::construct<T>(*this, p); }

    T construct() const
    { return emplacer_access::construct<T>(*this); }

    typedef T result_type;
    void operator()(void* const p) const { construct(p); }
    T operator()() const { return construct(); }
};

#else // #ifndef BOOST_NO_VARIADIC_TEMPLATES

#define cast_Un_type_yn( z, n, data ) \
    typename emplacer_private::cast< BOOST_PP_CAT( U, n ) >::type BOOST_PP_CAT( y, n )
#define cast_Un_apply_yn( z, n, data ) \
    emplacer_private::cast< BOOST_PP_CAT( U, n ) >::apply( BOOST_PP_CAT( y, n ) )

#define BOOST_PP_ITERATION_LIMITS ( 1, SAKE_EMPLACER_MAX_ARITY )
#define BOOST_PP_FILENAME_1       <sake/core/emplacer/emplacer.hpp>
#include BOOST_PP_ITERATE()

#undef cast_Un_type_yn
#undef cast_Un_apply_yn

#endif // #ifndef BOOST_NO_VARIADIC_TEMPLATES

/*******************************************************************************
 * emplacer_construct<T>(emplacer< Signature > e, void* p) -> void
 * emplacer_construct<T>(emplacer< Signature > e) -> T
 ******************************************************************************/

template< class T, class Signature >
inline typename boost::enable_if_c< boost::is_void<
    typename sake::emplacer< Signature >::value_type >::value >::type
emplacer_construct(sake::emplacer< Signature > e, void* const p)
{ e.template construct<T>(p); }

template< class T, class Signature >
inline typename boost::enable_if_c< boost::is_same<
    T, typename sake::emplacer< Signature >::value_type >::value >::type
emplacer_construct(sake::emplacer< Signature > e, void* const p)
{ e.construct(p); }

template< class T, class Signature >
inline typename boost::enable_if_c< boost::is_void<
    typename sake::emplacer< Signature >::value_type >::value, T >::type
emplacer_construct(sake::emplacer< Signature > e)
{ return e.template construct<T>(); }

template< class T, class Signature >
inline typename boost::enable_if_c< boost::is_same<
    T, typename sake::emplacer< Signature >::value_type >::value, T >::type
emplacer_construct(sake::emplacer< Signature > e)
{ return e.construct(); }

/*******************************************************************************
 * emplacer_constructible<T>(emplacer< Signature > e) -> [unspecified]
 ******************************************************************************/

template< class T, class Signature >
inline T
emplacer_constructible(sake::emplacer< Signature > e)
{ return sake::emplacer_construct<T>(e); }

template< class T, class V, class U0 >
inline typename boost::lazy_enable_if_c<
    boost_ext::mpl::or2< boost::is_void<V>, boost::is_same<T,V> >::value,
    sake::emplacer_private::cast< U0 >
>::type
emplacer_constructible(sake::emplacer< V ( U0 ) > e)
{ return e.template at_c<0>(); }

} // namespace sake

#endif // #ifndef SAKE_CORE_EMPLACER_HPP

#else // #ifndef BOOST_PP_IS_ITERATING

#define N BOOST_PP_ITERATION()

#define class_U0N       BOOST_PP_ENUM_PARAMS( N, class U )
#define comma_class_U0N BOOST_PP_ENUM_TRAILING_PARAMS( N, class U )
#define U0N             BOOST_PP_ENUM_PARAMS( N, U )

#define cast_U0N_type_y0N  BOOST_PP_ENUM( N, cast_Un_type_yn, ~ )
#define cast_U0N_apply_y0N BOOST_PP_ENUM( N, cast_Un_apply_yn, ~ )

/*******************************************************************************
 * class emplacer< void ( U0, ... ) >
 ******************************************************************************/

template< class_U0N >
class emplacer< void ( U0N ) >
    : public emplacer_private::base< void ( U0N ) >
{
    typedef emplacer_private::base< void ( U0N ) > base_;
public:
    SAKE_NONCOPY_ASSIGNABLE( emplacer )

    typedef void value_type;

    emplacer(cast_U0N_type_y0N)
        : base_(cast_U0N_apply_y0N)
    { }

    template< class T >
    void construct(void* const p)
    { emplacer_access::construct<T>(*this, p); }

    template< class T >
    T construct()
    { return emplacer_access::construct<T>(*this); }

    struct result_of
    {
        template< class T >
        struct as_typed
        { typedef emplacer< T ( U0N ) > type; };
    };
    template< class T >
    typename result_of::template as_typed<T>::type
    as_typed() const
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
        : sake::remove_qualified_type_tag<T>
    { };

    template< class T >
    void operator()(void* const p, sake::type_tag<T>) const
    { construct<T>(p); }
    template< class T >
    T operator()(sake::type_tag<T>) const
    { return construct<T>(); }
};

/*******************************************************************************
 * class emplacer< T ( U0, ... ) >
 ******************************************************************************/

template< class T comma_class_U0N >
class emplacer< T ( U0N ) >
    : public emplacer_private::base< void ( U0N ) >
{
    BOOST_STATIC_ASSERT(((N == 1) || boost::is_object<T>::value));
    BOOST_STATIC_ASSERT((!boost_ext::is_cv_or<T>::value));
    typedef emplacer_private::base< void ( U0N ) > base_;
public:
    SAKE_NONCOPY_ASSIGNABLE( emplacer )

    typedef T value_type;

    emplacer(cast_U0N_type_y0N)
        : base_(cast_U0N_apply_y0N)
    { }

    explicit emplacer(base_ e)
        : base_(e)
    { }

    void construct(void* const p) const
    { emplacer_access::construct<T>(*this, p); }

    T construct() const
    { return emplacer_access::construct<T>(*this); }

    typedef T result_type;
    void operator()(void* const p) const { construct(p); }
    T operator()() const { return construct(); }
};

#undef cast_U0N_type_y0N
#undef cast_U0N_apply_y0N

#undef class_U0N
#undef comma_class_U0N
#undef U0N

#undef N

#endif // #ifndef BOOST_PP_IS_ITERATING

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
 * make_emplacer(U0&& y0, ...) -> emplacer< void ( U0, ... ) >
 * make_typed_emplacer<T>(U0&& y0, ...) -> emplacer< T ( U0, ... ) >
 *
 * emplacer_wrap(U&& y) -> result_of::emplacer_wrap<U>::type
 * typed_emplacer_wrap<T>(U&& y) -> result_of::typed_emplacer_wrap<T,U>::type
 *
 * emplacer objects provide an abstraction for constructor arguments, allowing
 * one to do generic in-place construction of objects.  This is very similar to
 * Boost.InPlaceFactory and Boost.TypedInPlaceFactory, with the major advantage
 * that this provides support for rvalue references and lvalue
 * references-to-non-const.
 *
 * An emplacer object holds bound constructor parameters.  A bound constructor
 * parameter is held by lvalue reference if the corresponding Ui is an lvalue
 * reference; by value if the corresponding Ui is a non-reference and
 * by-value-optimal; by reference-to-const if the corresponding Ui is a
 * non-reference, not by-value-optimal, and non-movable; and by (emulated)
 * rvalue reference otherwise.  One consequence is that emplacers are very cheap
 * to copy (no more expensive than passing each of the bound parameters as
 * separate function arguments) and should always be passed by value.
 *
 * [typed_]emplacer_wrap is identical to the one-argument form of
 * make_[typed_]emplacer except when the argument is already an emplacer object
 * in which case it amounts to an identity operation.
 ******************************************************************************/

#ifndef BOOST_PP_IS_ITERATING

#ifndef SAKE_CORE_UTILITY_EMPLACER_HPP
#define SAKE_CORE_UTILITY_EMPLACER_HPP

#include <boost/config.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_shifted.hpp>
#include <boost/preprocessor/repetition/enum_shifted_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/preprocessor/tuple/rem.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/is_object.hpp>
#include <boost/type_traits/is_void.hpp>

#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/add_reference_add_const.hpp>
#include <sake/boost_ext/type_traits/add_rvalue_reference.hpp>
#include <sake/boost_ext/type_traits/is_reference.hpp>
#include <sake/boost_ext/type_traits/is_rvalue_reference.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/move/forward.hpp>
#include <sake/core/utility/emplacer_access.hpp>
#include <sake/core/utility/emplacer_fwd.hpp>
#include <sake/core/utility/is_by_value_optimal.hpp>
#include <sake/core/utility/non_copy_assignable.hpp>
#include <sake/core/utility/overload.hpp>
#include <sake/core/utility/type_tag.hpp>

namespace sake
{

namespace emplacer_private
{

template< class U, bool = sake::is_by_value_optimal<U>::value >
struct traits;

template< class Signature >
struct base;

template<>
struct base< void ( ) >
{
protected:
    template< unsigned int >
    struct at_c_impl
    { };
};

template<
    class U,
    bool = sake::is_emplacer<
               typename boost_ext::remove_qualifiers<U>::type
           >::value
>
struct wrap_dispatch;

template<
    class T, class U,
    bool = sake::is_emplacer<
               typename boost_ext::remove_qualifiers<U>::type,
               boost::mpl::quote1< boost::is_void >
           >::value,
    bool = sake::is_emplacer<
               typename boost_ext::remove_qualifiers<U>::type,
               boost::mpl::not_< boost::is_void< boost::mpl::_1 > >
           >::value
>
struct typed_wrap_dispatch;

} // namespace emplacer_private

#ifndef BOOST_NO_VARIADIC_TEMPLATES

/*******************************************************************************
 * class emplacer< void ( U... ) >
 ******************************************************************************/

template< class... U >
class emplacer< void ( U... ) >
    : public emplacer_private::base< void ( U... ) >
{
    typedef emplacer_private::base< void ( U... ) > base_;
public:
    SAKE_NON_COPY_ASSIGNABLE( emplacer )

    typedef void value_type;

    explicit emplacer(typename emplacer_private::traits<U>::type... y)
        : base_(emplacer_private::traits<U>::cast(y)...)
    { }

    template< class T >
    void construct(void* const p) const
    { emplacer_access::construct<T>(p, *this); }

    template< class T >
    T construct() const
    { return emplacer_access::construct<T>(*this); }

    template< class T >
    void assign(T& x) const
    { emplacer_access::assign(x, *this); }

    struct result_of
    {
        template< class T >
        struct as_typed
        { typedef emplacer< T ( U... ) > type; };
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
        : sake::remove_type_tag<
              typename boost_ext::remove_qualifiers<T>::type
          >
    { };

    template< class T >
    void operator()(void* const p, sake::type_tag<T>) const
    { construct<T>(p); }

    template< class T >
    T operator()(sake::type_tag<T>) const
    { return construct<T>(); }
};

/*******************************************************************************
 * class emplacer< T ( U... ) >
 ******************************************************************************/

template< class T, class... U >
class emplacer< T ( U... ) >
    : public emplacer_private::base< void ( U... ) >
{
    typedef emplacer_private::base< void ( U... ) > base_;
public:

    SAKE_NON_COPY_ASSIGNABLE( emplacer )

    typedef T value_type;

    explicit emplacer(typename emplacer_private::traits<U>::type... y)
        : base_(emplacer_private::traits<U>::cast(y)...)
    { }

    explicit emplacer(base_ e)
        : base_(e)
    { }

    void construct(void* const p)
    { emplacer_access::construct<T>(p, *this); }

    T construct()
    { return emplacer_access::construct<T>(*this); }

    void assign(typename boost_ext::add_reference<T>::type x)
    { emplacer_access::assign(x, *this); }

    typedef T result_type;
    T operator()() const
    { return construct(); }
    void operator()(void* const p) const
    { construct(p); }
};

#else // #ifndef BOOST_NO_VARIADIC_TEMPLATES

#define traits_Un_type_yn( z, n, data ) \
    typename emplacer_private::traits< BOOST_PP_CAT( U, n ) >::type BOOST_PP_CAT( y, n )
#define traits_Un_cast_yn( z, n, data ) \
    emplacer_private::traits< BOOST_PP_CAT( U, n ) >::cast( BOOST_PP_CAT( y, n ) )

#define BOOST_PP_ITERATION_LIMITS ( 0, SAKE_EMPLACER_MAX_ARITY )
#define BOOST_PP_FILENAME_1       <sake/core/utility/emplacer.hpp>
#include BOOST_PP_ITERATE()

#undef traits_Un_type_yn
#undef traits_Un_cast_yn

#endif // #ifndef BOOST_NO_VARIADIC_TEMPLATES

/*******************************************************************************
 * make_emplacer(U0&& y0, ...) -> emplacer< void ( U0, ... ) >
 * make_typed_emplacer<T>(U0&& y0, ...) -> emplacer< T ( U0, ... ) >
 ******************************************************************************/

#if !defined( BOOST_NO_RVALUE_REFERENCES ) \
 && !defined( BOOST_NO_VARIADIC_TEMPLATES )

template< class... U >
inline emplacer< void ( U... ) >
make_emplacer(U&&... y)
{ return emplacer< void ( U... ) >(sake::forward<U>(y)...); }

template< class T, class... U >
inline emplacer< T ( U... ) >
make_typed_emplacer(U&&... y)
{ return emplacer< T ( U... ) >(sake::forward<U>(y)...); }

#else // #if !defined( ... ) && !defined( ... )

inline emplacer< void ( ) >
make_emplacer()
{ return emplacer< void ( ) >(); }

#define SAKE_OVERLOAD_RESULT( n, U_tuple ) \
    boost::mpl::identity< emplacer< void U_tuple > >
#define SAKE_OVERLOAD_FUNCTION_NAME \
    make_emplacer
#define SAKE_OVERLOAD_BODY( n, U_tuple, y_tuple, forward_y_tuple ) \
    return emplacer< void U_tuple > forward_y_tuple ;
#define SAKE_OVERLOAD_PERFECT_MAX_ARITY SAKE_EMPLACER_PERFECT_MAX_ARITY
#define SAKE_OVERLOAD_FWD_MAX_ARITY     SAKE_EMPLACER_FWD_MAX_ARITY
#define SAKE_OVERLOAD_FWD2_MAX_ARITY    SAKE_EMPLACER_FWD2_MAX_ARITY
#include SAKE_OVERLOAD_GENERATE()

template< class T >
inline emplacer< T ( ) >
make_typed_emplacer()
{ return emplacer< T ( ) >(); }

#define SAKE_OVERLOAD_DECLARE_TEMPLATE_PARAMS( n, classU_tuple ) \
    class T, BOOST_PP_TUPLE_REM_CTOR( n, classU_tuple )
#define SAKE_OVERLOAD_RESULT( n, U_tuple ) \
    boost::mpl::identity< emplacer< T U_tuple > >
#define SAKE_OVERLOAD_FUNCTION_NAME \
    make_typed_emplacer
#define SAKE_OVERLOAD_BODY( n, U_tuple, y_tuple, forward_y_tuple ) \
    return emplacer< T U_tuple > forward_y_tuple ;
#define SAKE_OVERLOAD_PERFECT_MAX_ARITY SAKE_EMPLACER_PERFECT_MAX_ARITY
#define SAKE_OVERLOAD_FWD_MAX_ARITY     SAKE_EMPLACER_FWD_MAX_ARITY
#define SAKE_OVERLOAD_FWD2_MAX_ARITY    SAKE_EMPLACER_FWD2_MAX_ARITY
#include SAKE_OVERLOAD_GENERATE()

#endif // #if !defined( ... ) && !defined( ... )

/*******************************************************************************
 * emplacer_wrap(U&& y) -> result_of::emplacer_wrap<U>::type
 * typed_emplacer_wrap<T>(U&& y) -> result_of::typed_emplacer_wrap<T,U>::type
 ******************************************************************************/

namespace result_of
{

template< class U >
struct emplacer_wrap
{ typedef typename emplacer_private::wrap_dispatch<U>::result_type type; };

template< class T, class U >
struct typed_emplacer_wrap
{ typedef typename emplacer_private::typed_wrap_dispatch<T,U>::result_type type; };

} // namespace result_of

template< class U >
inline typename result_of::emplacer_wrap< SAKE_FWD2_PARAM( U ) >::type
emplacer_wrap(SAKE_FWD2_REF( U ) x)
{
    return emplacer_private::wrap_dispatch< SAKE_FWD2_PARAM( U ) >::
        apply(sake::forward<U>(x));
}

template< class T, class U >
inline typename result_of::typed_emplacer_wrap< T, SAKE_FWD2_PARAM( U ) >::type
typed_emplacer_wrap(SAKE_FWD2_REF( U ) x)
{
    return emplacer_private::typed_wrap_dispatch< T, SAKE_FWD2_PARAM( U ) >::
        apply(sake::forward<U>(x));
}

/*******************************************************************************
 * namespace emplacer_private
 ******************************************************************************/

namespace emplacer_private
{

template< class U >
struct traits< U, false >
{
    BOOST_STATIC_ASSERT((!boost_ext::is_reference<U>::value));
    typedef typename boost_ext::add_reference_add_const<
        typename boost_ext::add_rvalue_reference<U>::type
    >::type type;
    template< class V >
    static type cast(V& x)
    { return static_cast< type >(x); }
};

template< class U >
struct traits< U, true >
{
    BOOST_STATIC_ASSERT((!boost_ext::is_rvalue_reference<U>::value));
    typedef typename boost::remove_cv<U>::type type;
    static type cast(type x)
    { return x; }
};

#ifndef BOOST_NO_VARIADIC_TEMPLATES

template< class U0, class... U >
struct base< void ( U0, U... ) >
    : base< void ( U... ) >
{
    base(traits< U0 >::type y0, traits<U>::type... y)
        : base< void ( U... ) >(traits<U>::cast(y)...),
          m_y0(traits< U0 >::cast(y0))
    { }

protected:
    template< unsigned int K, class = void >
    struct at_c_impl
        : base< void ( U... ) >::template at_c_impl< K-1 >
    { };
    template< class _ >
    struct at_c_impl< 0, _ >
    {
        typedef typename traits< U0 >::type type;
        static type apply(base const & this_)
        { return traits< U0 >::cast(this_.m_y0); }
    };
    template< unsigned int, class > friend struct at_c_impl;
public:
    template< unsigned int K >
    typename at_c_impl<K>::type
    at_c() const
    { return at_c_impl<K>::apply(*this); }

private:
    typename traits< U0 >::type m_y0;
};

#endif // #ifndef BOOST_NO_VARIADIC_TEMPLATES

template< class U >
struct wrap_dispatch< U, false >
{
    typedef emplacer< void ( U ) > result_type;
    static result_type
    apply(typename traits<U>::type x)
    { return result_type(traits<U>::cast(x)); }
};

template< class U >
struct wrap_dispatch< U, true >
{
    typedef typename boost_ext::remove_qualifiers<U>::type result_type;
    static result_type
    apply(result_type x)
    { return x; }
};

template< class T, class U >
struct typed_wrap_dispatch< T, U, false, false >
{
    typedef emplacer< T ( U ) > result_type;
    static result_type
    apply(typename traits<U>::type x)
    { return result_type(traits<U>::cast(x)); }
};

template< class T, class U >
struct typed_wrap_dispatch< T, U, true, false >
{
    typedef typename boost_ext::remove_qualifiers<U>::type::
        result_of::template as_typed<T>::type result_type;
    static result_type
    apply(typename boost_ext::remove_qualifiers<U>::type const x)
    { return x.template as_typed<T>(); }
};

template< class T, class U >
struct typed_wrap_dispatch< T, U, false, true >
{
    typedef typename boost_ext::remove_qualifiers<U>::type result_type;
    static result_type
    apply(result_type x)
    { return x; }
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

#if N != 0

#define U1N BOOST_PP_ENUM_SHIFTED_PARAMS( N, U )

#define traits_U1N_cast_y1N BOOST_PP_ENUM_SHIFTED( N, traits_Un_cast_yn, ~ )

namespace emplacer_private
{

template< class_U0N >
struct base< void ( U0N ) >
    : base< void ( U1N ) >
{
    base(traits_U0N_type_y0N)
        : base< void ( U1N ) >(traits_U1N_cast_y1N),
          m_y0(traits< U0 >::cast(y0))
    { }

protected:
    template< unsigned int K, class = void >
    struct at_c_impl
#if N == 1
        : base< void ( ) >::at_c_impl< K-1 >
#else // #if N == 1
        : base< void ( U1N ) >::template at_c_impl< K-1 >
#endif // #if N == 1
    { };
    template< class _ >
    struct at_c_impl< 0, _ >
    {
        typedef typename traits< U0 >::type type;
        static type apply(base const & this_)
        { return traits< U0 >::cast(this_.m_y0); }
    };
    template< unsigned int, class > friend struct at_c_impl;
public:
    template< unsigned int K >
    typename at_c_impl<K>::type
    at_c() const
    { return at_c_impl<K>::apply(*this); }

private:
    typename traits< U0 >::type m_y0;
};

} // namespace emplacer_private

#undef traits_U1N_cast_y1N

#undef U1N

#endif // #if N != 0

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

#if N != 0
#if N == 1
    explicit
#endif // #if N == 1
    emplacer(traits_U0N_type_y0N)
        : base_(traits_U0N_cast_y0N)
    { }
#endif // #if N != 0

    template< class T >
    void construct(void* const p)
    { emplacer_access::construct<T>(p, *this); }

    template< class T >
    T construct()
    { return emplacer_access::construct<T>(*this); }

    template< class T >
    void assign(T& x)
    { emplacer_access::assign(x, *this); }

    struct result_of
    {
        template< class T >
        struct as_typed
        { typedef emplacer< T ( U0N ) > type; };
    };
    template< class T >
    typename result_of::as_typed<T>::type
    as_typed()
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
        : sake::remove_type_tag<
              typename boost_ext::remove_qualifiers<T>::type
          >
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
    typedef emplacer_private::base< void ( U0N ) > base_;
public:

    SAKE_NON_COPY_ASSIGNABLE( emplacer )

    typedef T value_type;

#if N != 0
#if N == 1
    explicit
#endif // #if N == 1
    emplacer(traits_U0N_type_y0N)
        : base_(traits_U0N_cast_y0N)
    { }
#endif // #if N != 0

    void construct(void* const p)
    { emplacer_access::construct<T>(p, *this); }

    T construct()
    { return emplacer_access::construct<T>(*this); }

    void assign(typename boost_ext::add_reference<T>::type x)
    { emplacer_access::assign(x, *this); }

    typedef T result_type;
    T operator()() const
    { return construct(); }
    void operator()(void* const p) const
    { construct(p); }
};

#undef traits_U0N_type_y0N
#undef traits_U0N_cast_y0N

#undef class_U0N
#undef comma_class_U0N
#undef U0N

#undef N

#endif // #ifndef BOOST_PP_IS_ITERATING

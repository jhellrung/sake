/*******************************************************************************
 * sake/core/emplacer/access.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * class emplacer_access
 *
 * This is provided to easily grant constructor access to the emplacer class
 * template.
 ******************************************************************************/

#ifndef BOOST_PP_IS_ITERATING

#ifndef SAKE_CORE_EMPLACER_ACCESS_HPP
#define SAKE_CORE_EMPLACER_ACCESS_HPP

#include <boost/config.hpp>
#include <boost/mpl/not.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_object.hpp>
#include <boost/type_traits/is_pod.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_void.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/mpl/range_c.hpp>
#include <sake/boost_ext/mpl/vector_c.hpp>
#include <sake/boost_ext/type_traits/is_cv_or.hpp>
#include <sake/boost_ext/type_traits/is_reference.hpp>

#include <sake/core/emplacer/fwd.hpp>
#include <sake/core/emplacer/private/cast.hpp>
#include <sake/core/utility/is_convertible_wnrbt.hpp>

namespace sake
{

class emplacer_access
{
    template< class Signature >
    friend class sake::emplacer;

    template< class T, class V >
    struct is_void_or_same
        : boost_ext::mpl::or2< boost::is_void<V>, boost::is_same<T,V> >
    { };

    /***************************************************************************
     * construct<T>(sake::emplacer< V ( ) > e) -> T
     **************************************************************************/

    template< class T, class V >
    static typename boost::enable_if_c<
        boost::is_pod<T>::value,
        T
    >::type
    construct(sake::emplacer< V ( ) >)
    {
        BOOST_STATIC_ASSERT((!boost_ext::is_cv_or<T>::value));
        BOOST_STATIC_ASSERT((is_void_or_same<T,V>::value));
        T result;
        return result;
    }

    template< class T, class V >
    static typename boost::disable_if_c<
        boost::is_pod<T>::value,
        T
    >::type
    construct(sake::emplacer< V ( ) >)
    {
        BOOST_STATIC_ASSERT((boost::is_object<T>::value));
        BOOST_STATIC_ASSERT((!boost_ext::is_cv_or<T>::value));
        BOOST_STATIC_ASSERT((is_void_or_same<T,V>::value));
        return T();
    }

    /***************************************************************************
     * construct<T>(sake::emplacer< V ( U0 ) > e) -> T
     **************************************************************************/

    template< class T, class V, class U0 >
    static T
    construct(sake::emplacer< V ( U0 ) > e)
    {
        BOOST_STATIC_ASSERT((!boost_ext::is_cv_or<T>::value));
        BOOST_STATIC_ASSERT((is_void_or_same<T,V>::value));
        BOOST_STATIC_ASSERT((boost_ext::mpl::or2<
            boost::mpl::not_< boost_ext::is_reference<T> >,
            sake::is_convertible_wnrbt< U0, T >
        >::value));
        return static_cast<T>(e.template at_c<0>());
    }

    /***************************************************************************
     * construct<T>(sake::emplacer< V ( ... ) > e, void* const p) -> void
     * construct<T>(sake::emplacer< V ( U0, U1, ... ) > e) -> T
     **************************************************************************/

#ifndef BOOST_NO_VARIADIC_TEMPLATES

    template< class T, class V, class... U >
    static void
    construct(sake::emplacer< V ( U... ) > e, void* const p)
    {
        typedef boost_ext::mpl::range_c< int, 0, sizeof...( U ) > range_c_;
        construct_impl(e, p, range_c_());
    }
    template< class T, class V, class... U, int... N >
    static void
    construct_impl(sake::emplacer< V ( U... ) > e, void* const p,
        boost_ext::mpl::vector_c< int, N... >)
    {
        BOOST_STATIC_ASSERT((boost::is_object<T>::value));
        BOOST_STATIC_ASSERT((!boost_ext::is_cv_or<T>::value));
        BOOST_STATIC_ASSERT((is_void_or_same<T,V>::value));
        new(p) T(e.template at_c<N>()...);
    }

    template< class T, class V, class U0, class U1, class... U >
    static T
    construct(sake::emplacer< V ( U0, U1, U... ) > e)
    {
        typedef boost_ext::mpl::range_c< int, 0, 2 + sizeof...( U ) > range_c_;
        return construct_impl(e, range_c_());
    }
    template< class T, class V, class... U, int... N >
    static T
    construct_impl(sake::emplacer< V ( U... ) > e,
        boost_ext::mpl::vector_c< int, N... >)
    {
        BOOST_STATIC_ASSERT((boost::is_object<T>::value));
        BOOST_STATIC_ASSERT((!boost_ext::is_cv_or<T>::value));
        BOOST_STATIC_ASSERT((is_void_or_same<T,V>::value));
        return T(e.template at_c<N>()...);
    }

#else // #ifndef BOOST_NO_VARIADIC_TEMPLATES

#define e_at_c_n( z, n, data ) e.template at_c<n>()

#define BOOST_PP_ITERATION_LIMITS ( 0, SAKE_EMPLACER_MAX_ARITY )
#define BOOST_PP_FILENAME_1       <sake/core/emplacer/access.hpp>
#include BOOST_PP_ITERATE()

#undef e_at_c_n

#endif // #ifndef BOOST_NO_VARIADIC_TEMPLATES

};

} // namespace sake

#endif // #ifndef SAKE_CORE_EMPLACER_ACCESS_HPP

#else // #ifndef BOOST_PP_IS_ITERATING

#define N BOOST_PP_ITERATION()

#define class_U0N       BOOST_PP_ENUM_PARAMS( N, class U )
#define comma_class_U0N BOOST_PP_ENUM_TRAILING_PARAMS( N, class U )
#define U0N             BOOST_PP_ENUM_PARAMS( N, U )
#define e_at_c_0N       BOOST_PP_ENUM( N, e_at_c_n, ~ )

    template< class T, class V comma_class_U0N >
    static void
    construct(sake::emplacer< V ( U0N ) > e, void* const p)
    {
        BOOST_STATIC_ASSERT((boost::is_object<T>::value));
        BOOST_STATIC_ASSERT((!boost_ext::is_cv_or<T>::value));
        BOOST_STATIC_ASSERT((is_void_or_same<T,V>::value));
        new(p) T(e_at_c_0N);
    }

#if N >= 2

    template< class T, class V, class_U0N >
    static T
    construct(sake::emplacer< V ( U0N ) > e)
    {
        BOOST_STATIC_ASSERT((boost::is_object<T>::value));
        BOOST_STATIC_ASSERT((!boost_ext::is_cv_or<T>::value));
        BOOST_STATIC_ASSERT((is_void_or_same<T,V>::value));
        return T(e_at_c_0N);
    }

#endif // #if N >= 2

#undef class_U0N
#undef comma_class_U0N
#undef U0N
#undef e_at_c_0N

#undef N

#endif // #ifndef BOOST_PP_IS_ITERATING

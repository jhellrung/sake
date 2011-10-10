/*******************************************************************************
 * sake/core/utility/emplacer_access.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * class emplacer_access
 *
 * This is provided to easily grant constructor access to the emplacer class
 * template.
 ******************************************************************************/

#ifndef BOOST_PP_IS_ITERATING

#ifndef SAKE_CORE_UTILITY_EMPLACER_ACCESS_HPP
#define SAKE_CORE_UTILITY_EMPLACER_ACCESS_HPP

#include <boost/config.hpp>
#include <boost/mpl/integral_c.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_object.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_void.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/range_c.hpp>
#include <sake/boost_ext/mpl/vector.hpp>
#include <sake/boost_ext/type_traits/is_convertible_wnrbt.hpp>
#include <sake/boost_ext/type_traits/is_same_sans_qualifiers.hpp>
#include <sake/boost_ext/type_traits/is_reference.hpp>

#include <sake/core/move/is_movable.hpp>
#include <sake/core/move/rv.hpp>
#include <sake/core/utility/emplacer_fwd.hpp>

namespace sake
{

class emplacer_access
{
    template< class Signature >
    friend class sake::emplacer;

    /***************************************************************************
     * construct<T>(sake::emplacer< V ( ) > e) -> T
     **************************************************************************/

    template< class T, class V >
    static T
    construct(sake::emplacer< V ( ) >)
    {
        BOOST_STATIC_ASSERT((boost::is_object<T>::value));
        BOOST_STATIC_ASSERT((
            boost::is_void<V>::value
         || boost::is_same<T,V>::value
        ));
        return T();
    }

    /***************************************************************************
     * construct<T>(sake::emplacer< V ( U0 ) > e) -> T
     **************************************************************************/

    template< class T, class V, class U0 >
    static typename boost::enable_if_c<
        boost_ext::is_same_sans_qualifiers< T, U0 >::value,
        T
    >::type
    construct(sake::emplacer< V ( U0 ) > e)
    {
        BOOST_STATIC_ASSERT((
            boost::is_void<V>::value
         || boost::is_same<T,V>::value
        ));
        BOOST_STATIC_ASSERT((
           !boost_ext::is_reference<T>::value
         || boost_ext::is_reference< U0 >::value
        ));
        return e.template at_c<0>();
    }

    template< class T, class V, class U0 >
    static typename boost::disable_if_c<
        boost_ext::is_same_sans_qualifiers< T, U0 >::value,
        T
    >::type
    construct(sake::emplacer< V ( U0 ) > e)
    {
        BOOST_STATIC_ASSERT((
            boost::is_void<V>::value
         || boost::is_same<T,V>::value
        ));
        BOOST_STATIC_ASSERT((
           !boost_ext::is_reference<T>::value
         || boost_ext::is_convertible_wnrbt< U0, T >::value
        ));
        return static_cast<T>(e.template at_c<0>());
    }

    /***************************************************************************
     * construct<T>(void* const p, sake::emplacer< V ( ... ) > e) -> void
     * construct<T>(sake::emplacer< V ( U0, U1, ... ) > e) -> T
     **************************************************************************/

#ifndef BOOST_NO_VARIADIC_TEMPLATES

    template< class T, class V, class... U >
    static void
    construct(void* const p, sake::emplacer< V ( U... ) > e)
    {
        typedef typename boost_ext::mpl::range_c<
            unsigned int, 0, sizeof...( U )
        >::type range_c_type;
        construct(p, e, range_c_type());
    }
    template< class T, class V, class... U, class... N >
    static void
    construct(void* const p, sake::emplacer< V ( U... ) > e,
        boost_ext::mpl::vector< boost::mpl::integral_c< unsigned int, N >... >)
    {
        BOOST_STATIC_ASSERT((boost::is_object<T>::value));
        BOOST_STATIC_ASSERT((
            boost::is_void<V>::value
         || boost::is_same<T,V>::value
        ));
        new(p) T(e.template at_c<N>()...);
    }

    template< class T, class V, class... U >
    static typename boost::enable_if_c<
        (sizeof...( U ) >= 2),
        T
    >::type
    construct(sake::emplacer< V ( U... ) > e)
    {
        typedef typename boost_ext::mpl::range_c<
            unsigned int, 0, sizeof...( U )
        >::type range_c_type;
        return construct(e, range_c_type());
    }
    template< class T, class V, class... U, class... N >
    static T
    construct(sake::emplacer< V ( U... ) > e,
        boost_ext::mpl::vector< boost::mpl::integral_c< unsigned int, N >... >)
    {
        BOOST_STATIC_ASSERT((boost::is_object<T>::value));
        BOOST_STATIC_ASSERT((
            boost::is_void<V>::value
         || boost::is_same<T,V>::value
        ));
        return T(e.template at_c<N>()...);
    }

#else // #ifndef BOOST_NO_VARIADIC_TEMPLATES

#define e_at_c_n( z, n, data ) e.template at_c<n>()

#define BOOST_PP_ITERATION_LIMITS ( 0, SAKE_EMPLACER_MAX_ARITY )
#define BOOST_PP_FILENAME_1       <sake/core/utility/emplacer_access.hpp>
#include BOOST_PP_ITERATE()

#undef e_at_c_n

#endif // #ifndef BOOST_NO_VARIADIC_TEMPLATES

    /***************************************************************************
     * assign(T& x, sake::emplacer< V ( ) > e) -> void
     **************************************************************************/

    template< class T, class V >
    static void
    assign(T& x, sake::emplacer< V ( ) >)
    {
        BOOST_STATIC_ASSERT((
            boost::is_void<V>::value
         || boost::is_same<T,V>::value
        ));
        return x = T();
    }

    /***************************************************************************
     * assign(T& x, sake::emplacer< V ( U0 ) > e) -> void
     **************************************************************************/

    template< class T, class V, class U0 >
    static typename boost::enable_if_c<
        boost_ext::is_same_sans_qualifiers< T, U0 >::value
    >::type
    assign(T& x, sake::emplacer< V ( U0 ) > e)
    {
        BOOST_STATIC_ASSERT((
            boost::is_void<V>::value
         || boost::is_same<T,V>::value
        ));
        x = e.template at_c<0>();
    }

    /***************************************************************************
     * assign(T& x, sake::emplacer< V ( U0, U1, ... ) > e) -> void
     **************************************************************************/

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class T, class Signature >
    static void
    assign(T& x, sake::emplacer< Signature > e)
    { x = construct<T>(e); }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class T, class Signature >
    static typename boost::enable_if_c<
        sake::is_movable<T>::value
    >::type
    assign(T& x, sake::emplacer< Signature > e)
    { x = static_cast< SAKE_RV_REF( T ) >(construct<T>(e)); }

    template< class T, class Signature >
    static typename boost::disable_if_c<
        sake::is_movable<T>::value
    >::type
    assign(T& x, sake::emplacer< Signature > e)
    { x = construct<T>(e); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

};

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_EMPLACER_ACCESS_HPP

#else // #ifndef BOOST_PP_IS_ITERATING

#define N BOOST_PP_ITERATION()

#define class_U0N       BOOST_PP_ENUM_PARAMS( N, class U )
#define comma_class_U0N BOOST_PP_ENUM_TRAILING_PARAMS( N, class U )
#define U0N             BOOST_PP_ENUM_PARAMS( N, U )
#define e_at_c_0N       BOOST_PP_ENUM( N, e_at_c_n, ~ )

    template< class T, class V comma_class_U0N >
    static void
    construct(void* const p, sake::emplacer< V ( U0N ) > e)
    {
        BOOST_STATIC_ASSERT((boost::is_object<T>::value));
        BOOST_STATIC_ASSERT((
            boost::is_void<V>::value
         || boost::is_same<T,V>::value
        ));
        new(p) T(e_at_c_0N);
    }

#if N >= 2

    template< class T, class V, class_U0N >
    static T
    construct(sake::emplacer< V ( U0N ) > e)
    {
        BOOST_STATIC_ASSERT((boost::is_object<T>::value));
        BOOST_STATIC_ASSERT((
            boost::is_void<V>::value
         || boost::is_same<T,V>::value
        ));
        return T(e_at_c_0N);
    }

#endif // #if N >= 2

#undef class_U0N
#undef comma_class_U0N
#undef U0N
#undef e_at_c_0N

#undef N

#endif // #ifndef BOOST_PP_IS_ITERATING

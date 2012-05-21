/*******************************************************************************
 * sake/core/emplacer/private/base.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef BOOST_PP_IS_ITERATING

#ifndef SAKE_CORE_EMPLACER_PRIVATE_BASE_HPP
#define SAKE_CORE_EMPLACER_PRIVATE_BASE_HPP

#include <boost/config.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_shifted.hpp>
#include <boost/preprocessor/repetition/enum_shifted_params.hpp>

#include <sake/core/emplacer/fwd.hpp>
#include <sake/core/emplacer/private/cast.hpp>

namespace sake
{

namespace emplacer_private
{

template< class Signature >
struct base;

template<>
struct base< void ( ) >
{
protected:
    template< unsigned int, class = void >
    struct at_c_impl
    { };
};

#ifndef BOOST_NO_VARIADIC_TEMPLATES

template< class U0, class... U >
struct base< void ( U0, U... ) >
    : base< void ( U... ) >
{
    base(emplacer_private::cast< U0 >::type y0, emplacer_private::cast<U>::type... y)
        : base< void ( U... ) >(emplacer_private::cast<U>::apply(y)...),
          m_y0(emplacer_private::cast< U0 >::apply(y0))
    { }

protected:
    template< unsigned int K, class = void >
    struct at_c_impl
        : base< void ( U... ) >::template at_c_impl< K-1 >
    { };
    template< class _ >
    struct at_c_impl< 0, _ >
    {
        typedef typename emplacer_private::cast< U0 >::type type;
        static type apply(base const & this_)
        { return emplacer_private::cast< U0 >::apply(this_.m_y0); }
    };
    template< unsigned int, class > friend struct at_c_impl;
public:
    template< unsigned int K >
    typename at_c_impl<K>::type
    at_c() const
    { return at_c_impl<K>::apply(*this); }

private:
    typename emplacer_private::cast< U0 >::type m_y0;
};

#else // #ifndef BOOST_NO_VARIADIC_TEMPLATES

#define cast_Un_type_yn( z, n, data ) \
    typename emplacer_private::cast< BOOST_PP_CAT( U, n ) >::type BOOST_PP_CAT( y, n )
#define cast_Un_apply_yn( z, n, data ) \
    emplacer_private::cast< BOOST_PP_CAT( U, n ) >::apply( BOOST_PP_CAT( y, n ) )

#define BOOST_PP_ITERATION_LIMITS ( 1, SAKE_EMPLACER_MAX_ARITY )
#define BOOST_PP_FILENAME_1       <sake/core/emplacer/private/base.hpp>
#include BOOST_PP_ITERATE()

#undef cast_Un_type_yn
#undef cast_Un_apply_yn

#endif // #ifndef BOOST_NO_VARIADIC_TEMPLATES

} // namespace emplacer_private

} // namespace sake

#endif // #ifndef SAKE_CORE_EMPLACER_PRIVATE_BASE_HPP

#else // #ifndef BOOST_PP_IS_ITERATING

#define N BOOST_PP_ITERATION()

#define class_U0N BOOST_PP_ENUM_PARAMS( N, class U )
#define U0N       BOOST_PP_ENUM_PARAMS( N, U )
#define U1N       BOOST_PP_ENUM_SHIFTED_PARAMS( N, U )

#define cast_U0N_type_y0N  BOOST_PP_ENUM( N, cast_Un_type_yn, ~ )
#define cast_U1N_apply_y1N BOOST_PP_ENUM_SHIFTED( N, cast_Un_apply_yn, ~ )

template< class_U0N >
struct base< void ( U0N ) >
    : base< void ( U1N ) >
{
    base(cast_U0N_type_y0N)
        : base< void ( U1N ) >(cast_U1N_apply_y1N),
          m_y0(emplacer_private::cast< U0 >::apply(y0))
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
        typedef typename emplacer_private::cast< U0 >::type type;
        static type apply(base const & this_)
        { return emplacer_private::cast< U0 >::apply(this_.m_y0); }
    };
    template< unsigned int, class > friend struct at_c_impl;
public:
    template< unsigned int K >
    typename at_c_impl<K>::type
    at_c() const
    { return at_c_impl<K>::apply(*this); }

private:
    typename emplacer_private::cast< U0 >::type m_y0;
};

#undef cast_U0N_type_y0N
#undef cast_U1N_apply_y1N

#undef class_U0N
#undef U0N
#undef U1N

#undef N

#endif // #ifndef BOOST_PP_IS_ITERATING

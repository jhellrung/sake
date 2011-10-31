/*******************************************************************************
 * sake/boost_ext/fusion/sequence/unpack.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * SAKE_BOOST_EXT_FUSION_UNPACK[_R]( [r,] vars, expr )
 *
 * SAKE_BOOST_EXT_FUSION_UNPACK( vars, expr ) "unpacks" the tuple returned by
 * expr into the variables given by the Boost.PP Seq vars.  This is similar to
 * boost::fusion::tie, except you can declare each variable inline.  Also, note
 * that expr will be evaluated precisely once, and if it evaluates to an rvalue,
 * a copy of that rvalue will be held for the duration of the scope.  Thus, you
 * may use reference types in vars, even if expr evaluates to an rvalue.
 *
 * Example:
 *     SAKE_BOOST_EXT_FUSION_UNPACK(
 *         ( int& x ) ( std::string s ),
 *         boost::fusion::make_vector( 42, "hello" )
 *     )
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_FUSION_SEQUENCE_UNPACK_HPP
#define SAKE_BOOST_EXT_FUSION_SEQUENCE_UNPACK_HPP

#include <boost/preprocessor/repetition/deduce_r.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>

#include <sake/boost_ext/fusion/sequence/intrinsic/at_c.hpp>

#include <sake/core/expr_traits/type_tag_of.hpp>
#include <sake/core/utility/auto_any.hpp>
#include <sake/core/utility/type_tag.hpp>
#include <sake/core/utility/unique_name.hpp>

#define SAKE_BOOST_EXT_FUSION_UNPACK( vars, expr ) \
    SAKE_BOOST_EXT_FUSION_UNPACK_R( BOOST_PP_DEDUCE_R(), vars, expr )

#define SAKE_BOOST_EXT_FUSION_UNPACK_R( r, vars, expr ) \
    SAKE_BOOST_EXT_FUSION_UNPACK_impl( \
        r, vars, expr, \
        SAKE_UNIQUE_NAME( _sake_boost_ext_fusion_unpack_ ) )

#define SAKE_BOOST_EXT_FUSION_UNPACK_impl( r, vars, expr, name ) \
    ::sake::auto_any_t name = ::sake::auto_capture( SAKE_EXPR_TYPE_TAG_OF( expr ), expr ); \
    BOOST_PP_SEQ_FOR_EACH_I_R( r, \
        SAKE_BOOST_EXT_FUSION_UNPACK_init_var, \
        ( SAKE_EXPR_TYPE_TAG_OF( expr ), name ), \
        vars )

#define SAKE_BOOST_EXT_FUSION_UNPACK_init_var( r, data, i, elem ) \
    elem = ::sake::boost_ext::fusion::unpack_private::at_c< i > data ;

namespace sake
{

namespace boost_ext
{

namespace fusion
{

namespace unpack_private
{

template< int N, class T >
inline typename boost_ext::fusion::result_of::at_c<T,N>::type
at_c(sake::type_tag<T>, sake::auto_any_t s)
{
    typedef typename boost_ext::fusion::result_of::at_c<T,N>::type result_type;
    return static_cast< result_type >(boost_ext::fusion::at_c<N>(sake::auto_any_cast<T>(s)));
}

} // namespace unpack_private

} // namespace fusion

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_FUSION_SEQUENCE_UNPACK_HPP

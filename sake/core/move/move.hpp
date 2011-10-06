/*******************************************************************************
 * sake/core/move/move.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * move(T&& x) -> result_of::move<T>::type
 * struct functional::move
 * #define SAKE_MOVE( Expr )
 *
 * SAKE_MOVE( Expr ) is intended to be a replacement for boost::move and/or
 * sake::move that also works for rvalues in the absence of rvalue references.
 * Specifically, if BOOST_NO_RVALUE_REFERENCES, it casts movable rvalues of type
 * T and movable lvalues of type T& to boost::rv<T>&.  This is necessary to
 * coerce generic forwarding functions to capture rvalues as emulated rvalue
 * references.
 *
 * Note: If Expr accesses class members through this-> or by explicit
 * qualification, this might not work (MSVC9, at least, complains).  The
 * workaround is to bring class members into the current class scope with using
 * declarations, or assign "this" to a temporary variable and access members
 * through that temporary variable.
 ******************************************************************************/

#ifndef SAKE_CORE_MOVE_MOVE_HPP
#define SAKE_CORE_MOVE_MOVE_HPP

#include <boost/config.hpp>
#include <boost/mpl/assert.hpp>

#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/add_reference_add_const.hpp>
#include <sake/boost_ext/type_traits/add_rvalue_reference.hpp>
#include <sake/boost_ext/type_traits/is_rvalue_reference.hpp>
#include <sake/boost_ext/type_traits/remove_reference.hpp>

#include <sake/core/expr_traits/type_tag_of.hpp>
#include <sake/core/utility/bisfo.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>
#include <sake/core/utility/type_tag.hpp>

#ifndef BOOST_NO_RVALUE_REFERENCES

#define SAKE_MOVE( Expr ) ::sake::move( Expr )

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

#define SAKE_MOVE( Expr ) \
    ::sake::move_private::impl( SAKE_EXPR_TYPE_TAG_OF( Expr ), Expr )

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

namespace sake
{

namespace result_of
{

#ifndef BOOST_NO_RVALUE_REFERENCES

template< class T >
struct move
{ typedef T&& type; };

template< class T >
struct move< T& >
{ typedef T&& type; };

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

template< class T >
struct move
    : boost_ext::add_reference_add_const<
          typename boost_ext::add_rvalue_reference<T>::type
      >
{ };

template< class T >
struct move< T& >
    : boost_ext::add_reference<
          typename boost_ext::add_rvalue_reference<T>::type
      >
{ };

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

template< class T >
struct move< T const & >
{
    BOOST_MPL_ASSERT_NOT((boost_ext::is_rvalue_reference< T& >));
    typedef T const & type;
};

} // namespace result_of

namespace functional
{

struct move
{
    SAKE_RESULT_FROM_METAFUNCTION( result_of::move, 1 )

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class T >
    typename result_of::move<T>::type
    operator()(T&& x) const
    { return static_cast< typename result_of::move<T>::type >(x); }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class T >
    typename result_of::move< T& >::type
    operator()(T& x) const
    { return static_cast< typename result_of::move< T& >::type >(x); }

    template< class T >
    typename result_of::move< T const & >::type
    operator()(T const & x) const
    { return static_cast< typename result_of::move< T const & >::type >(x); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

};

} // namespace functional

SAKE_BISFO( move )

#ifdef BOOST_NO_RVALUE_REFERENCES

namespace move_private
{

template<
    class T,
    bool = sake::is_movable<
        typename boost_ext::remove_reference<T>::type
    >::value
>
struct traits;

template< class T >
struct traits< T, false >
{
    typedef typename boost_ext::add_reference_add_const<T>::type result_type;
    typedef result_type param_type;
};

template< class T >
struct traits< T, true >
{
    typedef typename boost_ext::add_rvalue_reference<
        typename boost_ext::remove_reference<T>::type
    >::type result_type;
    typedef typename boost_ext::add_rvalue_reference<T>::type param_type;
};

template< class T >
inline typename traits<T>::result_type
impl(type_tag<T>, typename traits<T>::param_type x)
{ return static_cast< typename traits<T>::result_type >(x); }

} // namespace move_private

#endif // #ifdef BOOST_NO_RVALUE_REFERENCES

} // namespace sake

#endif // #ifndef SAKE_CORE_MOVE_MOVE_HPP

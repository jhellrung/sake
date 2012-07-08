/*******************************************************************************
 * sake/core/move/move.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * move(T&& x) -> result_of::move<T>::type
 * struct functional::move
 * #define SAKE_MOVE( expression )
 *
 * SAKE_MOVE( expression ) is intended to be a replacement for boost::move
 * and/or sake::move that also works for rvalues in the absence of rvalue
 * references.  Specifically, if BOOST_NO_RVALUE_REFERENCES, it casts movable
 * rvalues of type T and movable lvalues of type T& to boost::rv<T>&.  This is
 * necessary to coerce generic forwarding functions to capture rvalues as
 * emulated rvalue references.  Non-movable rvalues are cast to T const &.
 *
 * Note: If expression accesses class members through this-> or by explicit
 * qualification, this might not work (MSVC9, at least, complains).  The
 * workaround is to bring class members into the current class scope with using
 * declarations, or assign "this" to a temporary variable and access members
 * through that temporary variable.
 ******************************************************************************/

#ifndef SAKE_CORE_MOVE_MOVE_HPP
#define SAKE_CORE_MOVE_MOVE_HPP

#include <boost/config.hpp>

#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/add_reference_add_const.hpp>
#include <sake/boost_ext/type_traits/add_rvalue_reference.hpp>
#include <sake/boost_ext/type_traits/remove_rvalue_reference.hpp>

#include <sake/core/config.hpp>
#include <sake/core/expr_traits/type_tag_of.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>
#include <sake/core/utility/type_tag.hpp>

#ifndef BOOST_NO_RVALUE_REFERENCES

#define SAKE_MOVE( expression ) ::sake::move( expression )

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

#define SAKE_MOVE( expression ) \
    ::sake::move_private::impl( SAKE_EXPR_TYPE_TAG_OF( expression ), expression )

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

template< class T >
struct move< T const & >
{ typedef T const & type; };

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

namespace move_private
{

template< class T >
struct impl
    : boost_ext::add_reference_add_const<
          typename boost_ext::add_rvalue_reference<T>::type >
{ };

template< class T >
struct impl< T& >
    : boost_ext::add_reference<
          typename boost_ext::add_rvalue_reference<T>::type >
{ };

template< class T >
struct impl< T const & >
{ typedef T const & type; };

} // namespace move_private

template< class T >
struct move
    : move_private::impl<
          typename boost_ext::remove_rvalue_reference<T>::type >
{ };

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

} // namespace result_of

namespace functional
{

struct move
{
    SAKE_RESULT_FROM_METAFUNCTION( sake::result_of::move, 1 )

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class T >
    typename sake::result_of::move<T>::type
    operator()(T&& x) const
    { return static_cast< typename sake::result_of::move<T>::type >(x); }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class T >
    typename sake::result_of::move< T& >::type
    operator()(T& x) const
    { return x; }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

};

} // namespace functional

#ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
namespace move_adl_barrier
{ sake::functional::move const move = { }; }
using namespace move_adl_barrier;
#else // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
sake::functional::move const move = { };
#endif // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS

#ifdef BOOST_NO_RVALUE_REFERENCES

namespace move_private
{

template< class T >
inline typename sake::result_of::move<T>::type
impl(sake::type_tag<T>, typename sake::result_of::move<T>::type x)
{ return x; }

} // namespace move_private

#endif // #ifdef BOOST_NO_RVALUE_REFERENCES

} // namespace sake

#endif // #ifndef SAKE_CORE_MOVE_MOVE_HPP

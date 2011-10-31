/*******************************************************************************
 * sake/boost_ext/fusion/sequence/intrisic/back.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * boost_ext::fusion::back(Sequence&& s)
 *     -> boost_ext::fusion::result_of::back< Sequence >::type
 *
 * boost_ext::fusion::back is identical to boost::fusion::back, except it
 * propagates rvalue reference qualifiers from Sequence&& onto the result in
 * some circumstances.  See boost_ext::fusion::at.
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_FUSION_SEQUENCE_INTRINSIC_BACK_HPP
#define SAKE_BOOST_EXT_FUSION_SEQUENCE_INTRINSIC_BACK_HPP

#include <boost/config.hpp>
#include <boost/fusion/sequence/intrinsic/back.hpp>
#include <boost/fusion/sequence/intrinsic/size.hpp>
#include <boost/fusion/sequence/intrinsic/value_at.hpp>
#include <boost/fusion/support/is_view.hpp>

#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/add_rvalue_reference.hpp>
#include <sake/boost_ext/type_traits/is_lvalue_reference_to_nonconst.hpp>
#include <sake/boost_ext/type_traits/is_reference.hpp>
#include <sake/boost_ext/type_traits/remove_reference.hpp>
#include <sake/boost_ext/type_traits/remove_rvalue_reference.hpp>

#include <sake/core/move/as_lvalue.hpp>

namespace sake
{

namespace boost_ext
{

namespace fusion
{

namespace result_of
{

namespace back_private
{

template<
    class Sequence,
    class T = typename boost::fusion::result_of::back< Sequence >::type,
    bool = boost_ext::is_lvalue_reference_to_nonconst<T>::value
       && !boost::fusion::traits::is_view< Sequence >::value
       && !boost_ext::is_reference<
               typename boost::fusion::result_of::value_at_c<
                   Sequence,
                   boost::fusion::result_of::size< Sequence >::value - 1
               >::type
           >::value
>
struct dispatch;

template< class Sequence, class T >
struct dispatch< Sequence, T, false >
{ typedef T type; };

template< class Sequence, class T >
struct dispatch< Sequence, T, true >
    : boost_ext::add_reference<
          typename boost_ext::add_rvalue_reference<
              typename boost_ext::remove_reference<T>::type
          >::type
      >
{ };

} // namespace back_private

template< class Sequence >
struct back
    : back_private::dispatch< Sequence >
{ };

template< class Sequence >
struct back< Sequence& >
    : boost::fusion::result_of::back< Sequence >
{ };

} // namespace result_of

#ifndef BOOST_NO_RVALUE_REFERENCES

template< class Sequence >
inline typename result_of::back< Sequence >::type
back(Sequence&& s)
{
    typedef typename result_of::back< Sequence >::type result_type;
    return static_cast< result_type >(boost::fusion::back(s));
}

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

template< class Sequence >
inline typename result_of::back<
    typename boost_ext::remove_rvalue_reference< Sequence& >::type
>::type
back(Sequence& s)
{
    typedef typename result_of::back<
        typename boost_ext::remove_rvalue_reference< Sequence& >::type
    >::type result_type;
    return static_cast< result_type >(boost::fusion::back(SAKE_AS_LVALUE( s )));
}

template< class Sequence >
inline typename result_of::back< Sequence const & >::type
back(Sequence const & s)
{ return boost::fusion::back(s); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

} // namespace fusion

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_FUSION_SEQUENCE_INTRINSIC_BACK_HPP

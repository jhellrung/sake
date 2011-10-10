/*******************************************************************************
 * sake/core/keyword/tag_args.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_KEYWORD_TAG_ARGS_HPP
#define SAKE_CORE_KEYWORD_TAG_ARGS_HPP

#include <boost/config.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/back_inserter.hpp>
#include <boost/mpl/begin_end.hpp>
#include <boost/mpl/copy_if.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/erase.hpp>
#include <boost/mpl/find_if.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/push_front.hpp>
#include <boost/mpl/vector/vector0.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/type_traits/integral_constant.hpp>

#include <sake/boost_ext/mpl/vector.hpp>
#include <sake/boost_ext/type_traits/add_reference_add_const.hpp>
#include <sake/boost_ext/type_traits/add_rvalue_reference.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/keyword/param_spec.hpp>
#include <sake/core/keyword/tagged_value.hpp>

namespace sake
{

namespace keyword
{

template< class A, class P, class D = void >
struct tag_args;

namespace tag_args_private
{

#ifndef BOOST_NO_VARIADIC_TEMPLATES
typedef boost_ext::mpl::vector<> empty_mpl_vector_type;
#else // #ifndef BOOST_NO_VARIADIC_TEMPLATES
typedef boost::mpl::vector0<> empty_mpl_vector_type;
#endif // #ifndef BOOST_NO_VARIADIC_TEMPLATES

struct is_deduced
{
    template< class >
    struct apply
        : boost::false_type
    { };
    template< class Tag, class Pred >
    struct apply< keyword::optional< keyword::deduced< Tag >, Pred > >
        : boost::true_type
    { };
    template< class Tag, class Pred >
    struct apply< keyword::required< keyword::deduced< Tag >, Pred > >
        : boost::true_type
    { };
};

template< class Ax >
struct apply_pred
{
    template< class >
    struct apply;
    template< class Tag, class Pred >
    struct apply< keyword::optional< keyword::deduced< Tag >, Pred > >
        : boost::mpl::apply1< Pred, Ax >
    { };
    template< class Tag, class Pred >
    struct apply< keyword::required< keyword::deduced< Tag >, Pred > >
        : boost::mpl::apply1< Pred, Ax >
    { };
};

template< class S, class T >
struct push_front_or_void
    : boost::mpl::push_front<S,T>
{ };

template< class T >
struct push_front_or_void< void, T >
{ typedef void type; };

template< class Ai, class Ae, class Pi, class Pe, class D >
struct iterate_positional;
template<
    class Ai, class Ae, class Pi, class Pe, class D,
    class Px = typename boost::mpl::deref< Pi >::type
>
struct iterate_positional_0;
template<
    class Ai, class Ae, class Pi, class Pe, class D, class Px,
    class Ax = typename boost::mpl::deref< Ai >::type,
    class Ax_ = typename boost_ext::remove_qualifiers< Ax >::type
>
struct iterate_positional_1;
template< class Ai, class Ae, class Pi, class Pe, class D, class Px, class Ax >
struct iterate_positional_2;
template< class Ai, class Ae, class Pi, class Pe, class D, class Tag, class Ax >
struct iterate_positional_3;

template< class Ai, class Ae, class D >
struct iterate_deduced;
template<
    class Ai, class Ae, class D,
    class Ax = typename boost::mpl::deref< Ai >::type,
    class Ax_ = typename boost_ext::remove_qualifiers< Ax >::type
>
struct iterate_deduced_0;
template<
    class Ai, class Ae, class D, class Ax,
    class Di = typename boost::mpl::find_if< D, apply_pred< Ax > >::type,
    class De = typename boost::mpl::end<D>::type
>
struct iterate_deduced_1;
template<
    class Ai, class Ae, class D, class Ax, class Di,
    class Dx = typename boost::mpl::deref< Di >::type
>
struct iterate_deduced_2;
template< class Ai, class Ae, class D, class Ax, class Di, class Tag >
struct iterate_deduced_3;

template< class Ae, class Pe, class D >
struct iterate_positional<Ae,Ae,Pe,Pe,D>
{ typedef empty_mpl_vector_type type; };

template< class Ae, class Pi, class Pe, class D >
struct iterate_positional<Ae,Ae,Pi,Pe,D>
{ typedef empty_mpl_vector_type type; };

template< class Ai, class Ae, class Pe, class D >
struct iterate_positional<Ai,Ae,Pe,Pe,D>
    : iterate_deduced<Ai,Ae,D>
{ };

template< class Ai, class Ae, class Pi, class Pe, class D >
struct iterate_positional
    : iterate_positional_0<Ai,Ae,Pi,Pe,D>
{ };

template< class Ai, class Ae, class Pi, class Pe, class D, class Tag, class Pred >
struct iterate_positional_0< Ai,Ae,Pi,Pe,D, keyword::optional< keyword::deduced< Tag >, Pred > >
    : iterate_deduced<Ai,Ae,D>
{ };

template< class Ai, class Ae, class Pi, class Pe, class D, class Tag, class Pred >
struct iterate_positional_0< Ai,Ae,Pi,Pe,D, keyword::required< keyword::deduced< Tag >, Pred > >
    : iterate_deduced<Ai,Ae,D>
{ };

template< class Ai, class Ae, class Pi, class Pe, class D, class Px >
struct iterate_positional_0
    : iterate_positional_1< Ai,Ae,Pi,Pe,D, Px >
{ };

template< class Ai, class Ae, class Pi, class Pe, class D, class Px, class Ax, class Tag, class T >
struct iterate_positional_1< Ai,Ae,Pi,Pe,D, Px, Ax, keyword::tagged_value< Tag, T > >
    : push_front_or_void<
          typename iterate_deduced< typename boost::mpl::next< Ai >::type, Ae, D >::type,
          keyword::tagged_value< Tag, T >
      >
{ };

template< class Ai, class Ae, class Pi, class Pe, class D, class Px, class Ax, class Ax_ >
struct iterate_positional_1
    : iterate_positional_2< Ai,Ae,Pi,Pe,D, Px, Ax >
{ };

template< class Ai, class Ae, class Pi, class Pe, class D, class Tag, class Ax >
struct iterate_positional_2
    : iterate_positional_3< Ai,Ae,Pi,Pe,D, Tag, Ax >
{ };

template< class Ai, class Ae, class Pi, class Pe, class D, class Tag, class Pred, class Ax >
struct iterate_positional_2< Ai,Ae,Pi,Pe,D, keyword::optional< Tag, Pred >, Ax >
    : iterate_positional_3< Ai,Ae,Pi,Pe,D, Tag, Ax >
{ };

template< class Ai, class Ae, class Pi, class Pe, class D, class Tag, class Pred, class Ax >
struct iterate_positional_2< Ai,Ae,Pi,Pe,D, keyword::required< Tag, Pred >, Ax >
    : iterate_positional_3< Ai,Ae,Pi,Pe,D, Tag, Ax >
{ };

template< class Ai, class Ae, class Pi, class Pe, class D, class Tag, class Ax >
struct iterate_positional_3
    : push_front_or_void<
          typename iterate_positional<
              typename boost::mpl::next< Ai >::type, Ae,
              typename boost::mpl::next< Pi >::type, Pe,
              D
          >::type,
          keyword::tagged_value<
              Tag,
              typename boost_ext::add_reference_add_const<
                  typename boost_ext::add_rvalue_reference< Ax >::type
              >::type
          >
      >
{ };

template< class Ae, class D >
struct iterate_deduced<Ae,Ae,D>
{ typedef empty_mpl_vector_type type; };

template< class Ai, class Ae, class D >
struct iterate_deduced
    : iterate_deduced_0<Ai,Ae,D>
{ };

template< class Ai, class Ae, class D, class Ax, class Tag, class T >
struct iterate_deduced_0< Ai,Ae,D, Ax, keyword::tagged_value< Tag, T > >
    : push_front_or_void<
          typename iterate_deduced< typename boost::mpl::next< Ai>::type, Ae, D >::type,
          keyword::tagged_value< Tag, T >
      >
{ };

template< class Ai, class Ae, class D, class Ax, class Ax_ >
struct iterate_deduced_0
    : iterate_deduced_1< Ai,Ae,D, Ax >
{ };

template< class Ai, class Ae, class D, class Ax, class De >
struct iterate_deduced_1< Ai,Ae,D, Ax, De, De >
{ typedef void type; };

template< class Ai, class Ae, class D, class Ax, class Di, class De >
struct iterate_deduced_1
    : iterate_deduced_2< Ai,Ae,D, Ax, Di >
{ };

template< class Ai, class Ae, class D, class Ax, class Di, class Tag, class Pred >
struct iterate_deduced_2< Ai,Ae,D, Ax, Di, keyword::optional< keyword::deduced< Tag >, Pred > >
    : iterate_deduced_3< Ai,Ae,D, Ax, Di, Tag >
{ };

template< class Ai, class Ae, class D, class Ax, class Di, class Tag, class Pred >
struct iterate_deduced_2< Ai,Ae,D, Ax, Di, keyword::required< keyword::deduced< Tag >, Pred > >
    : iterate_deduced_3< Ai,Ae,D, Ax, Di, Tag >
{ };

template< class Ai, class Ae, class D, class Ax, class Di, class Tag >
struct iterate_deduced_3
    : push_front_or_void<
          typename iterate_deduced<
              typename boost::mpl::next< Ai >::type,
              Ae,
              typename boost::mpl::erase< D, Di >::type
          >::type,
          keyword::tagged_value<
              Tag,
              typename boost_ext::add_reference_add_const<
                  typename boost_ext::add_rvalue_reference< Ax >::type
              >::type
          >
      >
{ };

} // namespace tag_args_private

template< class A, class P, class D >
struct tag_args
    : tag_args_private::iterate_positional<
          typename boost::mpl::begin<A>::type,
          typename boost::mpl::end<A>::type,
          typename boost::mpl::begin<P>::type,
          typename boost::mpl::end<P>::type,
          D
      >
{ };

template< class A, class P >
struct tag_args< A, P, void >
{
    typedef typename boost::mpl::copy_if<
        P,
        tag_args_private::is_deduced,
        boost::mpl::back_inserter< tag_args_private::empty_mpl_vector_type >
    >::type d;
    typedef typename tag_args<A,P,d>::type type;
};

} // namespace keyword

} // namespace sake

#endif // #ifndef SAKE_CORE_KEYWORD_TAG_ARGS_HPP

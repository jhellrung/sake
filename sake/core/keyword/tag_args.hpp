/*******************************************************************************
 * sake/core/keyword/tag_args.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct keyword::tag_args< Args, ParamSpecs >
 *
 * "Tags" the the Boost.MPL sequence of arguments types given by Args according
 * parameter specification given by ParamSpecs.  ParamSpecs should be a
 * Boost.MPL sequence of parameter specifictions (see param_spec.hpp).
 *
 * tag_args attempts to generate a Boost.MPL sequence TaggedValues of tagged
 * values (see tagged_value.hpp) corresponding to Args, such that
 * - if Args[i] is already a tagged value, then TaggedValues[i] == Args[i];
 * - otherwise, TaggedValues[i]::value_type == Args[i]&&.
 *
 * The algorithm used to build TaggedValues goes as follows.
 *
 * let Deduceds := [ P in ParamSpecs : P is deduced ]
 * let TaggedValues := [ ]
 * let i := 0
 * loop:
 *     if i == size( Args ): return
 *     if i == size( ParamSpecs ): break
 *     let P := ParamSpecs[i]
 *     if P is deduced: break
 *     let A := Args[i]
 *     if A is a tagged value:
 *         TaggedValues := [ TaggedValues, A ]
 *         break
 *     TaggedValues := [ TaggedValues, tagged_value< tag(P), A&& > ]
 *     i := i + 1
 * loop:
 *     if i == size( Args ): return TaggedValues
 *     let A := Args[i]
 *     if A is a tagged value:
 *         TaggedValues := [ TaggedValues, A ]
 *     else
 *         let D := the first deduced parameter specification in Deduceds such
 *                  that A satisfies D's predicate; if no such D exists, set
 *                  TaggedValues to void and return
 *         TaggedValues := [ TaggedValues, tagged_value< tag(D), A&& > ]
 *         Deduced := Deduced with D removed
 *     i := i + 1
 *
 * As can be seen, if the above algorithm fails, TaggedValues will be void.
 *
 * The general idea is to tag arguments in two stages. The first stage tags
 * arguments purely positionally, and does so until a tagged value is reached in
 * Args or a deduced parameter specification is reached in ParamSpecs. At this
 * point, the argument switches over to the second stage, and only tags untagged
 * values in Args with tags from *deduced* parameter specifications.
 *
 * Note that this is precisely the algorithm that Boost.Parameter in its
 * "parameters" interface.
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
#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/push_front.hpp>
#include <boost/mpl/vector/vector0.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/integral_constant.hpp>

#include <sake/boost_ext/mpl/as_vector.hpp>
#include <sake/boost_ext/mpl/vector.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/keyword/param_spec.hpp>
#include <sake/core/keyword/tagged_value.hpp>
#include <sake/core/utility/call_traits.hpp>

namespace sake
{

namespace keyword
{

namespace tag_args_private
{
#ifndef BOOST_NO_VARIADIC_TEMPLATES
typedef boost_ext::mpl::vector<> empty_mpl_vector_type;
#else // #ifndef BOOST_NO_VARIADIC_TEMPLATES
typedef boost::mpl::vector0<> empty_mpl_vector_type;
#endif // #ifndef BOOST_NO_VARIADIC_TEMPLATES
struct is_deduced;
template< class Ai, class Ae, class Pi, class Pe, class D >
struct iterate_positional;
} // namespace tag_args_private

template< class Args, class ParamSpecs >
struct tag_args
{
    BOOST_STATIC_ASSERT((boost::mpl::is_sequence< Args >::value));
    BOOST_STATIC_ASSERT((boost::mpl::is_sequence< ParamSpecs >::value));
    typedef typename boost::mpl::copy_if<
        ParamSpecs,
        tag_args_private::is_deduced,
        boost::mpl::back_inserter< tag_args_private::empty_mpl_vector_type >
    >::type deduced_type;
    typedef typename boost_ext::mpl::as_vector<
        typename tag_args_private::iterate_positional<
            typename boost::mpl::begin< Args >::type,
            typename boost::mpl::end< Args >::type,
            typename boost::mpl::begin< ParamSpecs >::type,
            typename boost::mpl::end< ParamSpecs >::type,
            deduced_type
        >::type
    >::type type;
};

namespace tag_args_private
{

template< class T >
struct tag
{ typedef T type; };
template< class Tag, class Pred >
struct tag< sake::keyword::optional< Tag, Pred > >
{ typedef Tag type; };
template< class Tag, class Pred >
struct tag< sake::keyword::required< Tag, Pred > >
{ typedef Tag type; };
template< class Tag, class Pred >
struct tag< sake::keyword::optional< sake::keyword::deduced< Tag >, Pred > >
{ typedef Tag type; };
template< class Tag, class Pred >
struct tag< sake::keyword::required< sake::keyword::deduced< Tag >, Pred > >
{ typedef Tag type; };

struct is_deduced
{
    template< class >
    struct apply
        : boost::false_type
    { };
    template< class Tag, class Pred >
    struct apply< sake::keyword::optional<
        sake::keyword::deduced< Tag >, Pred > >
        : boost::true_type
    { };
    template< class Tag, class Pred >
    struct apply< sake::keyword::required<
        sake::keyword::deduced< Tag >, Pred > >
        : boost::true_type
    { };
};

template< class Ax >
struct apply_pred
{
    template< class >
    struct apply;
    template< class Tag, class Pred >
    struct apply< sake::keyword::optional<
        sake::keyword::deduced< Tag >, Pred > >
        : boost::mpl::apply1< Pred, Ax >
    { };
    template< class Tag, class Pred >
    struct apply< sake::keyword::required<
        sake::keyword::deduced< Tag >, Pred > >
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

template<
    class Ai, class Ae, class Pi, class Pe, class D,
    class Px = typename boost::mpl::deref< Pi >::type
>
struct iterator_positional_dispatch_deduced;
template<
    class Ai, class Ae, class Pi, class Pe, class D, class Px,
    class Ax = typename boost::mpl::deref< Ai >::type,
    class Ax_ = typename boost_ext::remove_qualifiers< Ax >::type
>
struct iterator_positional_dispatch_tagged_value;

template< class Ai, class Ae, class D >
struct iterate_deduced;
template<
    class Ai, class Ae, class D,
    class Ax = typename boost::mpl::deref< Ai >::type,
    class Ax_ = typename boost_ext::remove_qualifiers< Ax >::type
>
struct iterate_deduced_dispatch_tagged_value;
template<
    class Ai, class Ae, class D, class Ax,
    class Di = typename boost::mpl::find_if< D, apply_pred< Ax > >::type,
    class De = typename boost::mpl::end<D>::type
>
struct iterate_deduced_dispatch_find_if;

template< class Ae, class Pe, class D >
struct iterate_positional<Ae,Ae,Pe,Pe,D>
{ typedef tag_args_private::empty_mpl_vector_type type; };

template< class Ae, class Pi, class Pe, class D >
struct iterate_positional<Ae,Ae,Pi,Pe,D>
{ typedef tag_args_private::empty_mpl_vector_type type; };

template< class Ai, class Ae, class Pe, class D >
struct iterate_positional<Ai,Ae,Pe,Pe,D>
    : tag_args_private::iterate_deduced<Ai,Ae,D>
{ };

template< class Ai, class Ae, class Pi, class Pe, class D >
struct iterate_positional
    : tag_args_private::iterator_positional_dispatch_deduced<Ai,Ae,Pi,Pe,D>
{ };

template<
    class Ai, class Ae, class Pi, class Pe, class D,
    class Tag, class Pred
>
struct iterator_positional_dispatch_deduced<
    Ai,Ae,Pi,Pe,D,
    sake::keyword::optional< sake::keyword::deduced< Tag >, Pred >
>
    : tag_args_private::iterate_deduced<Ai,Ae,D>
{ };

template<
    class Ai, class Ae, class Pi, class Pe, class D,
    class Tag, class Pred
>
struct iterator_positional_dispatch_deduced<
    Ai,Ae,Pi,Pe,D,
    sake::keyword::required< sake::keyword::deduced< Tag >, Pred >
>
    : tag_args_private::iterate_deduced<Ai,Ae,D>
{ };

template<
    class Ai, class Ae, class Pi, class Pe, class D,
    class Px
>
struct iterator_positional_dispatch_deduced
    : tag_args_private::iterator_positional_dispatch_tagged_value<
          Ai,Ae,Pi,Pe,D,
          Px
      >
{ };

template<
    class Ai, class Ae, class Pi, class Pe, class D,
    class Px, class Ax, class Tag, class T
>
struct iterator_positional_dispatch_tagged_value<
    Ai,Ae,Pi,Pe,D,
    Px, Ax, sake::keyword::tagged_value< Tag, T >
>
    : tag_args_private::push_front_or_void<
          typename tag_args_private::iterate_deduced<
              typename boost::mpl::next< Ai >::type, Ae, D >::type,
          sake::keyword::tagged_value< Tag, T >
      >
{ };

template<
    class Ai, class Ae, class Pi, class Pe, class D,
    class Px, class Ax, class Ax_
>
struct iterator_positional_dispatch_tagged_value
    : tag_args_private::push_front_or_void<
          typename tag_args_private::iterate_positional<
              typename boost::mpl::next< Ai >::type, Ae,
              typename boost::mpl::next< Pi >::type, Pe,
              D
          >::type,
          sake::keyword::tagged_value<
              typename tag_args_private::tag< Px >::type,
              typename sake::call_traits< Ax >::fwd_cast_type
          >
      >
{ };

template< class Ae, class D >
struct iterate_deduced<Ae,Ae,D>
{ typedef tag_args_private::empty_mpl_vector_type type; };

template< class Ai, class Ae, class D >
struct iterate_deduced
    : tag_args_private::iterate_deduced_dispatch_tagged_value<Ai,Ae,D>
{ };

template<
    class Ai, class Ae, class D,
    class Ax, class Tag, class T
>
struct iterate_deduced_dispatch_tagged_value<
    Ai,Ae,D,
    Ax, sake::keyword::tagged_value< Tag, T >
>
    : tag_args_private::push_front_or_void<
          typename tag_args_private::iterate_deduced<
              typename boost::mpl::next< Ai>::type, Ae, D >::type,
          sake::keyword::tagged_value< Tag, T >
      >
{ };

template<
    class Ai, class Ae, class D,
    class Ax, class Ax_
>
struct iterate_deduced_dispatch_tagged_value
    : tag_args_private::iterate_deduced_dispatch_find_if< Ai,Ae,D, Ax >
{ };

template<
    class Ai, class Ae, class D,
    class Ax, class De
>
struct iterate_deduced_dispatch_find_if<
    Ai,Ae,D,
    Ax, De, De
>
{ typedef void type; };

template< class Ai, class Ae, class D, class Ax, class Di, class De >
struct iterate_deduced_dispatch_find_if
    : tag_args_private::push_front_or_void<
          typename tag_args_private::iterate_deduced<
              typename boost::mpl::next< Ai >::type, Ae,
              typename boost::mpl::erase< D, Di >::type
          >::type,
          sake::keyword::tagged_value<
              typename tag_args_private::tag<
                  typename boost::mpl::deref< Di >::type >::type,
              typename sake::call_traits< Ax >::fwd_cast_type
          >
      >
{ };

} // namespace tag_args_private

} // namespace keyword

} // namespace sake

#endif // #ifndef SAKE_CORE_KEYWORD_TAG_ARGS_HPP

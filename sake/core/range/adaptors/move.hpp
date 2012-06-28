/*******************************************************************************
 * sake/core/range/adaptors/move.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * class range::adaptors::move<R>
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_ADAPTORS_MOVE_HPP
#define SAKE_CORE_RANGE_ADAPTORS_MOVE_HPP

#include <boost/concept/assert.hpp>
#include <boost/mpl/map/map10.hpp>

#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/remove_reference.hpp>
#include <sake/boost_ext/type_traits/remove_reference_add_const.hpp>

#include <sake/core/iterator/adaptors/fwd.hpp>
#include <sake/core/iterator/adaptors/move.hpp>
#include <sake/core/memberwise/mem_fun.hpp>
#include <sake/core/move/movable.hpp>
#include <sake/core/move/move.hpp>
#include <sake/core/move/rv.hpp>
#include <sake/core/range/adaptor.hpp>
#include <sake/core/range/adaptors/fwd.hpp>
#include <sake/core/range/adaptors/multidim/move.hpp>
#include <sake/core/range/concepts/Range.hpp>
#include <sake/core/range/core_access.hpp>
#include <sake/core/range/keyword.hpp>
#include <sake/core/range/traits.hpp>
#include <sake/core/range/traits_fwd.hpp>
#include <sake/core/utility/using_typedef.hpp>
#include <sake/core/utility/value_constructor.hpp>

namespace sake
{

namespace range
{

namespace adaptors
{

namespace move_private
{

template< class R >
struct traits;
template< class Reference >
struct at_dispatch;

} // namespace move_private

template< class R >
class move
    : public move_private::traits<R>::adaptor_
{
    typedef move_private::traits<R> traits_;
    SAKE_USING_TYPEDEF( typename traits_, adaptor_ );
public:
    SAKE_OPTIMAL_MOVABLE_COPYABLE_MEMBERWISE(
        typename move,
        (( adaptor_ ))
    )
    SAKE_MEMBERWISE_MEM_FUN(
        typename move,
        ( default_constructor ) ( swap ),
        (( adaptor_ ))
    )

    // template< class S >
    // explicit move(S&& r)
    //     : adaptor_(sake::forward<S>(r))
    // { }
#define SAKE_VALUE_CONSTRUCTOR_CLASS_NAME move
#define SAKE_VALUE_CONSTRUCTOR_TYPES      ( R )
#define SAKE_VALUE_CONSTRUCTOR_FORWARD    adaptor_
#include SAKE_VALUE_CONSTRUCTOR_GENERATE()

    using adaptor_::base;
    typename boost_ext::add_reference<R>::type
    base()
    { return adaptor_::protected_base(); }

private:
    friend class sake::range::core_access;

    template< class This, class Introversal >
    struct derived_iterator_with_of
    {
        typedef sake::iterator::adaptors::move<
            typename adaptor_::template
                base_iterator_with_of< This, Introversal >::type > type;
    };

    template< class This, class T, class Introversal >
    static typename adaptor_::template
        iterator_with_of< This, Introversal >::type
    derived_iter_at(This& this_, T const & x, Introversal)
    {
        return typename adaptor_::template
            iterator_with_of< This, Introversal >::type(
                adaptor_::base_iter_at(this_, x, Introversal()));
    }

    template< class This, class T >
    static typename adaptor_::template reference_of< This >::type
    derived_at(This& this_, T const x)
    {
        return move_private::at_dispatch<
            typename adaptor_::template reference_of< This >::type
        >::apply(this_.protected_base(), x);
    }
};

namespace move_private
{

template< class R >
struct traits
{
    typedef typename boost_ext::remove_reference<R>::type base_;
    BOOST_CONCEPT_ASSERT((sake::concepts::Range< base_ >));
    typedef typename sake::range_iterator< base_ >::type base_iterator;
    typedef typename sake::range_iterator<
        typename boost_ext::remove_reference_add_const<R>::type
    >::type base_const_iterator;
    typedef sake::iterator::adaptors::move< base_iterator > iterator;
    typedef sake::iterator::adaptors::move< base_const_iterator > const_iterator;
    typedef sake::range::adaptor<
        sake::range::adaptors::move<R>, R,
        boost::mpl::map2<
            sake::range::keyword::iterator< iterator >,
            sake::range::keyword::const_iterator< const_iterator >
        >
    > adaptor_;
};

template< class Reference >
struct at_dispatch
{
    template< class B, class T >
    static Reference
    apply(B& b, T const x)
    { return sake::range_traits<B>::at(b,x); }
};

template< class V >
struct at_dispatch< SAKE_RV_REF( V ) >
{
    template< class B, class T >
    static SAKE_RV_REF( V )
    apply(B& b, T const x)
    { return sake::move(sake::range_traits<B>::at(b,x)); }
};

} // namespace move_private

} // namespace adaptors

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_ADAPTORS_MOVE_HPP

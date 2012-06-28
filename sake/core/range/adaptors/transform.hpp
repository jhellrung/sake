/*******************************************************************************
 * sake/core/range/adaptors/transform.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * class range::adaptors::transform< R, F, Params = boost::mpl::map0<> >
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_ADAPTORS_TRANSFORM_HPP
#define SAKE_CORE_RANGE_ADAPTORS_TRANSFORM_HPP

#include <boost/concept/assert.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/map/map0.hpp>
#include <boost/mpl/map/map10.hpp>
#include <boost/static_assert.hpp>

#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/add_reference_add_const.hpp>
#include <sake/boost_ext/type_traits/remove_reference.hpp>
#include <sake/boost_ext/type_traits/remove_reference_add_const.hpp>

#include <sake/core/concepts/Function.hpp>
#include <sake/core/iterator/adaptors/fwd.hpp>
#include <sake/core/iterator/adaptors/transform.hpp>
#include <sake/core/memberwise/mem_fun.hpp>
#include <sake/core/move/movable.hpp>
#include <sake/core/range/adaptor.hpp>
#include <sake/core/range/adaptors/fwd.hpp>
#include <sake/core/range/adaptors/multidim/transform.hpp>
#include <sake/core/range/concepts/Range.hpp>
#include <sake/core/range/core_access.hpp>
#include <sake/core/range/keyword.hpp>
#include <sake/core/range/traits.hpp>
#include <sake/core/range/traits_fwd.hpp>
#include <sake/core/utility/is_by_value_optimal.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake
{

namespace range
{

namespace adaptors
{

namespace transform_private
{

template< class R, class F, class Params >
struct traits;

} // namespace transform_private

template< class R, class F, class Params /*= boost::mpl::map0<>*/ >
class transform
    : public transform_private::traits< R, F, Params >::adaptor_
{
    typedef transform_private::traits< R, F, Params > traits_;
    SAKE_USING_TYPEDEF( typename traits_, adaptor_ );
public:
    SAKE_OPTIMAL_MOVABLE_COPYABLE_MEMBERWISE(
        typename transform,
        (( adaptor_ ))
    )
    SAKE_MEMBERWISE_MEM_FUN(
        typename transform,
        ( default_constructor ) ( swap ),
        (( adaptor_ ))
    )

    // template< class S >
    // explicit transform(S&& r)
    //     : adaptor_(sake::forward<S>(r))
    // { }
#define SAKE_VALUE_CONSTRUCTOR_CLASS_NAME transform
#define SAKE_VALUE_CONSTRUCTOR_TYPES      ( R )
#define SAKE_VALUE_CONSTRUCTOR_FORWARD    adaptor_
#include SAKE_VALUE_CONSTRUCTOR_GENERATE()

    // template< class S, class G >
    // transform(S&& r, G&& f)
    //     : adaptor_(sake::forward<S>(r), sake::forward<G>(f))
    // { }
#define SAKE_VALUE_CONSTRUCTOR_CLASS_NAME transform
#define SAKE_VALUE_CONSTRUCTOR_TYPES      ( R ) ( F )
#define SAKE_VALUE_CONSTRUCTOR_FORWARD    adaptor_
#include SAKE_VALUE_CONSTRUCTOR_GENERATE()

    typedef F function_type;

    typename boost_ext::add_reference<F>::type
    function()
    { return adaptor_::member(); }
    typename boost_ext::add_reference_add_const<F>::type
    function() const
    { return adaptor_::member(); }

private:
    friend class sake::range::core_access;

    template< class This, class Introversal >
    struct derived_iterator_with_of
    {
        typedef sake::iterator::adaptors::transform<
            typename adaptor_::template
                base_iterator_with_of< This, Introversal >::type,
            typename traits_::iterator_function_type,
            Params
        > type;
    };

    template< class This, class T, class Introversal >
    static typename adaptor_::template
        iterator_with_of< This, Introversal >::type
    derived_iter_at(This& this_, T const & x, Introversal)
    {
        return typename adaptor_::template
            iterator_with_of< This, Introversal >::type(
                adaptor_::base_iter_at(this_, x, Introversal()),
                this_.function()
            );
    }

    template< class This, class T >
    static typename adaptor_::template reference_of< This >::type
    derived_at(This& this_, T const x)
    { return this_.function()(adaptor_::base_at(this_, x)); }
};

namespace transform_private
{

template< class R, class F, class Params >
struct traits
{
    typedef typename boost_ext::remove_reference<R>::type base_;
    BOOST_CONCEPT_ASSERT((sake::concepts::Range< base_ >));
    typedef sake::range_traits< base_ > base_traits;
    typedef sake::range_traits<
        typename boost_ext::remove_reference_add_const<R>::type
    > base_const_traits;
    typedef typename base_traits::reference base_reference;
    typedef typename base_const_traits::reference base_const_reference;
    BOOST_CONCEPT_ASSERT((
        sake::concepts::Function< F, void ( base_reference ) >));
    BOOST_CONCEPT_ASSERT((
        sake::concepts::Function< F, void ( base_const_reference ) >));
    BOOST_STATIC_ASSERT((boost::mpl::is_sequence< Params >::value));

    typedef typename base_traits::iterator base_iterator;
    typedef typename base_const_traits::iterator base_const_iterator;
    typedef typename boost::mpl::if_c<
        sake::is_by_value_optimal<F>::value,
        F, typename boost_ext::add_reference_add_const<F>::type
    >::type iterator_function_type;
    typedef sake::iterator::adaptors::transform<
        base_iterator, iterator_function_type, Params > iterator;
    typedef sake::iterator::adaptors::transform<
        base_const_iterator, iterator_function_type, Params > const_iterator;
    typedef sake::range::adaptor<
        sake::range::adaptors::transform< R, F, Params >, R,
        boost::mpl::map3<
            sake::range::keyword::iterator< iterator >,
            sake::range::keyword::const_iterator< const_iterator >,
            sake::range::keyword::member<F>
        >
    > adaptor_;
};

} // namespace transform_private

} // namespace adaptors

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_ADAPTORS_TRANSFORM_HPP

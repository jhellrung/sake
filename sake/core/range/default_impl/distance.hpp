/*******************************************************************************
 * sake/core/range/default_impl/distance.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_DEFAULT_IMPL_DISTANCE_HPP
#define SAKE_CORE_RANGE_DEFAULT_IMPL_DISTANCE_HPP

#include <boost/mpl/placeholders.hpp>

#include <sake/boost_ext/mpl/if.hpp>
#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/introspection/has_operator_minus.hpp>
#include <sake/core/introspection/is_callable_function.hpp>
#include <sake/core/introspection/is_callable_member_function.hpp>
#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/iterator/categories.hpp>
#include <sake/core/iterator/distance.hpp>
#include <sake/core/iterator/distance_fwd.hpp>
#include <sake/core/iterator/facade_fwd.hpp>
#include <sake/core/range/adapt/fwd.hpp>
#include <sake/core/range/adapt/transform.hpp>
#include <sake/core/range/algorithm/accumulate.hpp>
#include <sake/core/range/algorithm/fwd.hpp>
#include <sake/core/range/distance.hpp>
#include <sake/core/range/distance_fwd.hpp>
#include <sake/core/range/multidim_traits.hpp>
#include <sake/core/range/static_size.hpp>
#include <sake/core/range/traits.hpp>
#include <sake/core/range/traits_fwd.hpp>
#include <sake/core/utility/int_tag.hpp>
#include <sake/core/utility/is_template_base_of.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake_range_distance_private
{

#define SAKE_INTROSPECTION_TRAIT_NAME    is_callable
#define SAKE_INTROSPECTION_FUNCTION_NAME range_distance
#define SAKE_INTROSPECTION_FUNCTION_ARITY_LIMITS ( 1, 1 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_FUNCTION()

template< class R >
inline typename ::sake::range_difference< R const >::type
adl(R const & r)
{ return range_distance(r); }

} // namespace sake_range_distance_private

namespace sake
{

namespace range
{

namespace default_impl
{

namespace distance_private
{

#define SAKE_INTROSPECTION_TRAIT_NAME           is_callable_mem_fun
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME size
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY_LIMITS ( 0, 0 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_MEMBER_FUNCTION()

template< class R >
inline typename sake::range_difference< R const >::type
dispatch(R const & /*r*/, sake::int_tag<5>)
{ return sake::range_static_size<R>::value; }

template< class R >
inline typename sake::range_difference< R const >::type
dispatch(R const & r, sake::int_tag<4>)
{ return r.distance(); }

template< class R >
inline typename sake::range_difference< R const >::type
dispatch(R const & r, sake::int_tag<3>)
{ return ::sake_range_distance_private::adl(r); }

template< class R >
inline typename sake::range_difference< R const >::type
dispatch(R const & r, sake::int_tag<2>)
{
    typedef sake::range_traits< R const > traits_;
    return traits_::end(r) - traits_::begin(r);
}

template< class R >
inline typename sake::range_difference< R const >::type
dispatch(R const & r, sake::int_tag<1>)
{
    return sake::range::algorithm::functional::accumulate()(
        sake::range_multidim_traits< R const >::outer(r)
      | sake::range::adapt::functional::transform<
            sake::range::functional::distance >(),
        static_cast< typename sake::range_difference< R const >::type >(0)
    );
}

template< class R >
inline typename sake::range_difference< R const >::type
dispatch(R const & r, sake::int_tag<0>)
{
    typedef sake::range_traits< R const > traits_;
    return sake::iterator::functional::distance()(
        traits_::begin(r, sake::end_detect_introversal_tag()),
        sake::_end
    );
}

} // namespace distance_private

template< class R >
inline typename sake::range_difference< R const >::type
distance(R const & r)
{
    typedef sake::range_traits< R const > traits_;
    SAKE_USING_TYPEDEF( typename traits_, difference_type );
    typedef typename boost_ext::mpl::
    if_<
        sake::range_has_static_size<R>,
        sake::int_tag<5>
    >::type::template
    else_if<
        distance_private::is_callable_mem_fun<
            R const &, difference_type ( ) >,
        sake::int_tag<4>
    >::type::template
    else_if<
        ::sake_range_distance_private::is_callable<
            difference_type ( R const & ) >,
        sake::int_tag<3>
    >::type::template
    else_if<
        boost_ext::mpl::or2<
            boost_ext::is_convertible<
                typename traits_::traversal,
                boost::random_access_traversal_tag
            >,
            sake::is_template_base_of2<
                sake::iterator::facade, typename traits_::iterator,
                sake::has_operator_minus<
                    boost::mpl::_1, boost::mpl::_1, difference_type >
            >
        >,
        sake::int_tag<2>
    >::type::template
    else_if<
        sake::range_multidim_enable< R const >,
        sake::int_tag<1>
    >::type::template
    else_<
        sake::int_tag<0>
    >::type int_tag_;
    return distance_private::dispatch(r, int_tag_());
}

} // namespace default_impl

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_DEFAULT_IMPL_DISTANCE_HPP

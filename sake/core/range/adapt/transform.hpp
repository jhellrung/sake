/*******************************************************************************
 * sake/core/range/adapt/transform.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * range::adapt::transform(F f, Params = ...)
 *     -> range::adapt::result_of::transform< F, Params >::type
 * struct range::adapt::functional::transform< F = void, Params = ... >
 *
 * r | range::adapt::transform(f)
 *     -> range::adapt::result_of::transform<F>::apply<R>::type
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_ADAPT_TRANSFORM_HPP
#define SAKE_CORE_RANGE_ADAPT_TRANSFORM_HPP

#include <boost/config.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/map/map0.hpp>
#include <boost/static_assert.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/add_reference_add_const.hpp>
#include <sake/boost_ext/type_traits/is_base_of_sans_qualifiers.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>
#include <sake/boost_ext/type_traits/remove_rvalue_reference.hpp>

#include <sake/core/config.hpp>
#include <sake/core/memberwise/copy_tags.hpp>
#include <sake/core/memberwise/default_constructor.hpp>
#include <sake/core/memberwise/destructor_tags.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/move.hpp>
#include <sake/core/range/adapt/fwd.hpp>
#include <sake/core/range/adaptors/construct/fwd.hpp>
#include <sake/core/range/adaptors/construct/transform.hpp>
#include <sake/core/range/adaptors/traits/enable.hpp>
#include <sake/core/utility/type_tag.hpp>
#include <sake/core/utility/base_member.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>
#include <sake/core/utility/value_constructor_enable.hpp>

namespace sake
{

namespace range
{

namespace adapt
{

namespace result_of
{

template< class F, class Params = boost::mpl::map0<> >
class transform
{
    typedef typename boost_ext::remove_qualifiers<F>::type function_type;
    typedef typename sake::remove_type_tag<
        typename boost_ext::remove_qualifiers< Params >::type
    >::type params_type;
    BOOST_STATIC_ASSERT((boost::mpl::is_sequence< params_type >::value));
public:
    typedef sake::range::adapt::functional::transform<
        function_type, params_type > type;

    template< class R >
    struct apply
    { typedef typename sake::range::adaptors::construct::
          result_of::transform< R, function_type, params_type >::type type; };
};

} // namespace result_of

namespace transform_private
{

template< class R, class F, class Params >
inline typename sake::range::adaptors::construct::result_of::transform<
    SAKE_FWD2_PARAM( R ), F, Params >::type
impl(SAKE_FWD2_REF( R ) r, F const & f, sake::type_tag< Params >)
{
    return typename sake::range::adaptors::construct::result_of::transform<
        SAKE_FWD2_PARAM( R ), F, Params >::type(sake::forward<R>(r), f);
}

} // namespace transform_private

namespace functional
{

template<>
struct transform< void, boost::mpl::map0<> >
{
    SAKE_RESULT_FROM_METAFUNCTION(
        sake::range::adapt::result_of::transform, (1,2) )

    template< class F >
    typename result< transform ( F ) >::type
    operator()(F const & f) const
    { return typename result< transform ( F ) >::type(f); }

    template< class F, class Params >
    typename result< transform ( F, Params ) >::type
    operator()(F const & f, Params) const
    { return typename result< transform ( F, Params ) >::type(f); }
};

template< class F /*= void*/, class Params /*= boost::mpl::map0<>*/ >
struct transform
    : sake::base_member<F>
{
private:
    typedef sake::base_member<F> base_member_;
public:

    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename transform,
        (( base_member_ ))
    )
    SAKE_MEMBERWISE_COPY_TAGS( (( base_member_ )) )
    SAKE_MEMBERWISE_DESTRUCTOR_TAGS( (( base_member_ )) )

private:
    template< class F_ >
    struct constructor_enable
        : boost_ext::mpl::and2<
              boost_ext::not_is_base_of_sans_qualifiers< transform, F_ >,
              sake::value_constructor_enable< F, F_ >
          >
    { };
    template< class F_ >
    struct constructor_enabler
        : boost::enable_if_c< constructor_enable<
              typename boost_ext::remove_rvalue_reference< F_ >::type >::value >
    { };
public:

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class F_ >
    explicit transform(F_&& f,
        typename constructor_enabler< F_ >::type* = 0)
        : base_member_(sake::forward< F_ >(f))
    { }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class F_ >
    explicit transform(F_& f,
        typename constructor_enabler< F_& >::type* = 0)
        : base_member_(f)
    { }

    template< class F_ >
    explicit transform(F_ const & f,
        typename constructor_enabler< F_ const & >::type* = 0)
        : base_member_(f)
    { }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

    typedef F function_type;

    typename boost_ext::add_reference<F>::type
    function()
    { return base_member_::member(); }
    typename boost_ext::add_reference_add_const<F>::type
    function() const
    { return base_member_::member(); }

    template< class > struct result;
    template< class This, class R >
    struct result< This ( R ) >
    { typedef typename sake::range::adapt::
        result_of::transform< F, Params >::template apply<R>::type type; };

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class R >
    typename result< transform ( R ) >::type
    operator()(R&& r) const
    { return apply_impl(sake::forward<R>(r)); }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class R >
    typename sake::range::adaptors::traits::lazy_val_enabler<
        R, result< transform ( R ) > >::type
    operator()(R r) const
    { return apply_impl(sake::move(r)); }

    template< class R >
    typename sake::range::adaptors::traits::lazy_ref_enabler<
        R, result< transform ( R& ) > >::type
    operator()(R& r) const
    { return apply_impl(r); }

    template< class R >
    typename sake::range::adaptors::traits::lazy_ref_enabler<
        R, result< transform ( R const & ) > >::type
    operator()(R const & r) const
    { return apply_impl(r); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

private:
    template< class R >
    typename result< transform ( SAKE_FWD2_PARAM( R ) ) >::type
    apply_impl(SAKE_FWD2_REF( R ) r) const
    {
        return transform_private::impl(
            sake::forward<R>(r), function(), sake::type_tag< Params >());
    }
};

#ifndef BOOST_NO_RVALUE_REFERENCES

template< class R, class F, class P >
inline typename
    sake::range::adaptors::construct::result_of::transform<R,F,P>::type
operator|(R&& r, sake::range::adapt::functional::transform<F,P> const & _)
{
    return transform_private::impl(
        sake::forward<R>(r), _.function(), sake::type_tag<P>());
}

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

template< class R, class F, class P >
inline typename sake::range::adaptors::traits::lazy_val_enabler< R,
    sake::range::adaptors::construct::result_of::transform<R,F,P>
>::type
operator|(R r, sake::range::adapt::functional::transform<F,P> const & _)
{
    return transform_private::impl(
        sake::move(r), _.function(), sake::type_tag<P>());
}

template< class R, class F, class P >
inline typename sake::range::adaptors::traits::lazy_ref_enabler< R,
    sake::range::adaptors::construct::result_of::transform< R&, F, P >
>::type
operator|(R& r, sake::range::adapt::functional::transform<F,P> const & _)
{
    return transform_private::impl(
        r, _.function(), sake::type_tag<P>());
}

template< class R, class F, class P >
inline typename sake::range::adaptors::traits::lazy_ref_enabler< R,
    sake::range::adaptors::construct::result_of::transform< R const &, F, P >
>::type
operator|(R const & r, sake::range::adapt::functional::transform<F,P> const & _)
{
    return transform_private::impl(
        r, _.function(), sake::type_tag<P>());
}

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

} // namespace functional

#ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
namespace transform_adl_barrier
{ sake::range::adapt::functional::transform<> const transform = { }; }
using namespace transform_adl_barrier;
#else // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
sake::range::adapt::functional::transform<> const transform = { };
#endif // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS

} // namespace adapt

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_ADAPT_TRANSFORM_HPP

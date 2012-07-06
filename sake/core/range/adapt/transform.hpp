/*******************************************************************************
 * sake/core/range/adapt/transform.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * range::adapt::transform(R&& r, F f)
 *     -> range::adapt::result_of::transform<R,F>::type
 * struct range::adapt::functional::transform
 *
 * r | range::adapt::transform(f)
 *     -> range::adapt::result_of::transform<R,F>::type
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_ADAPT_MOVE_HPP
#define SAKE_CORE_RANGE_ADAPT_MOVE_HPP

#include <boost/concept/assert.hpp>
#include <boost/config.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/map/map0.hpp>
#include <boost/static_assert.hpp>

#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/add_reference_add_const.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>
#include <sake/boost_ext/type_traits/remove_reference.hpp>

#include <sake/core/concepts/Function.hpp>
#include <sake/core/memberwise/default_constructor.hpp>
#include <sake/core/memberwise/type_trait_tag.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/move.hpp>
#include <sake/core/range/adapt/fwd.hpp>
#include <sake/core/range/adapt/traits/base.hpp>
#include <sake/core/range/adapt/traits/enable.hpp>
#include <sake/core/range/adaptors/transform.hpp>
#include <sake/core/range/concepts/fwd.hpp>
#include <sake/core/range/concepts/Range.hpp>
#include <sake/core/range/traits.hpp>
#include <sake/core/range/traits_fwd.hpp>
#include <sake/core/utility/base_member.hpp>
#include <sake/core/utility/call_traits.hpp>

namespace sake
{

namespace range
{

namespace adapt
{

/*******************************************************************************
 * struct range::adapt::result_of::transform< R, F = ..., Params = ... >
 ******************************************************************************/

namespace result_of
{

template< class R, class F = void, class Params = boost::mpl::map0<> >
struct transform
{
private:
    typedef typename boost_ext::remove_reference<R>::type range_;
    BOOST_CONCEPT_ASSERT((sake::concepts::Range< range_ >));
    BOOST_CONCEPT_ASSERT((sake::concepts::Function<
        typename boost_ext::remove_qualifiers<F>::type,
        void ( typename sake::range_reference< range_ >::type )
    >));
    BOOST_STATIC_ASSERT((boost::mpl::is_sequence< Params >::value));
public:
    typedef sake::range::adaptors::transform<
        typename sake::range::adapt::traits::base<R>::type,
        typename boost_ext::remove_qualifiers<F>::type,
        Params
    > type;
};

template< class F, class Params >
struct transform< F, void, Params >
{
    BOOST_STATIC_ASSERT((boost::mpl::is_sequence< Params >::value));
    typedef sake::range::adapt::functional::transform<
        typename boost_ext::remove_qualifiers<F>::type,
        Params
    > type;
};

} // namespace result_of

/*******************************************************************************
 * namespace range::adapt::transform_private
 ******************************************************************************/

namespace transform_private
{

template< class R, class F >
inline typename sake::range::adapt::result_of::transform<
    SAKE_FWD2_PARAM( R ), F >::type
impl(SAKE_FWD2_REF( R ) r, F const & f)
{
    return typename sake::range::adapt::result_of::transform<
        SAKE_FWD2_PARAM( R ), F >::type(sake::forward<R>(r), f);
}

} // namespace transform_private

namespace functional
{

/*******************************************************************************
 * struct range::adapt::functional::transform< F, Params = ... >
 ******************************************************************************/

template< class F /*= void */, class Params /*= boost::mpl::map0<>*/ >
struct transform
    : sake::base_member<F>
{
    BOOST_STATIC_ASSERT((!boost::is_void<F>::value));
private:
    typedef sake::base_member<F> base_member_;
public:

    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename transform,
        (( base_member_ ))
    )
    SAKE_MEMBERWISE_TYPEDEF_TYPE_TRAIT_TAG(
        (( base_member_ )),
        ( has_copy_constructor )
        ( has_nothrow_copy_constructor )
        ( has_nothrow_copy_assign )
    )

    explicit transform(typename sake::call_traits<F>::param_type f)
        : base_member_(f)
    { }

    typename boost_ext::add_reference<F>::type
    function()
    { return base_member_::member(); }
    typename boost_ext::add_reference_add_const<F>::type
    function() const
    { return base_member_::member(); }

    template< class > struct result;
    template< class This, class R >
    struct result< This ( R ) >
        : sake::range::adapt::result_of::transform< R, F, Params >
    { };

private:
    template< class R >
    typename result< transform ( SAKE_FWD2_PARAM( R ) ) >::type
    apply_impl(SAKE_FWD2_REF( R ) r) const
    {
        return typename result< transform ( SAKE_FWD2_PARAM( R ) ) >::type(
            sake::forward<R>(r), function());
    }
public:

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class R >
    typename result< transform ( R ) >::type
    operator()(R&& r) const
    { return apply_impl(sake::forward<R>(r)); }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class R >
    typename sake::range::adapt::traits::lazy_val_enabler<
        R, result< transform ( R ) > >::type
    operator()(R r) const
    { return apply_impl(sake::move(r)); }

    template< class R >
    typename sake::range::adapt::traits::lazy_ref_enabler<
        R, result< transform ( R& ) > >::type
    operator()(R& r) const
    { return apply_impl(r); }

    template< class R >
    typename sake::range::adapt::traits::lazy_ref_enabler<
        R, result< transform ( R const & ) > >::type
    operator()(R const & r) const
    { return apply_impl(r); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

};

/*******************************************************************************
 * struct range::adapt::functional::transform< void, Params = ... >
 ******************************************************************************/

template<>
struct transform< void, boost::mpl::map0<> >
{
    SAKE_RESULT_FROM_METAFUNCTION(
        sake::range::adapt::result_of::transform, (1,2) )

    template< class F >
    typename result< transform ( F ) >::type
    operator()(F const & f) const
    { return typename result< transform ( F ) >::type(f); }

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class R, class F >
    typename result< transform ( R, F ) >::type
    operator()(R&& r, F const & f) const
    { return transform_private::impl(sake::forward<R>(r), f); }
    
#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class R, class F >
    typename sake::range::adapt::traits::lazy_val_enabler<
        R, result< transform ( R, F ) > >::type
    operator()(R r, F const & f) const
    { return transform_private::impl(sake::move(r), f); }

    template< class R, class F >
    typename sake::range::adapt::traits::lazy_ref_enabler<
        R, result< transform ( R&, F ) > >::type
    operator()(R& r, F const & f) const
    { return transform_private::impl(r,f); }

    template< class R, class F >
    typename sake::range::adapt::traits::lazy_ref_enabler<
        R, result< transform ( R const &, F ) > >::type
    operator()(R const & r, F const & f) const
    { return transform_private::impl(r,f); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

};

/*******************************************************************************
 * r | range::adapt::transform(f)
 *     -> range::adapt::result_of::transform<R,F>::type
 ******************************************************************************/

#ifndef BOOST_NO_RVALUE_REFERENCES

template< class R, class F >
inline typename sake::range::adapt::result_of::transform<R,F>::type
operator|(R&& r, sake::range::adapt::functional::transform<F> const & f)
{ return transform_private::impl(sake::forward<R>(r), f.function()); }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

template< class R, class F >
inline typename sake::range::adapt::traits::lazy_val_enabler<
    R, sake::range::adapt::result_of::transform<R,F> >::type
operator|(R r, sake::range::adapt::functional::transform<F> const & f)
{ return transform_private::impl(sake::move(r), f.function()); }

template< class R, class F >
inline typename sake::range::adapt::traits::lazy_ref_enabler<
    R, sake::range::adapt::result_of::transform< R&, F > >::type
operator|(R& r, sake::range::adapt::functional::transform<F> const & f)
{ return transform_private::impl(r, f.function()); }

template< class R, class F >
inline typename sake::range::adapt::traits::lazy_ref_enabler<
    R, sake::range::adapt::result_of::transform< R const &, F > >::type
operator|(R const & r, sake::range::adapt::functional::transform<F> const & f)
{ return transform_private::impl(r, f.function()); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

} // namespace functional

static sake::range::adapt::functional::transform<> const transform = { };

} // namespace adapt

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_ADAPT_MOVE_HPP

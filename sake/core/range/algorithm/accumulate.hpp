/*******************************************************************************
 * sake/core/range/algorithm/accumulate.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_ALGORITHM_ACCUMULATE_HPP
#define SAKE_CORE_RANGE_ALGORITHM_ACCUMULATE_HPP

#include <boost/concept/assert.hpp>
#include <boost/config.hpp>

#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>
#include <sake/boost_ext/type_traits/remove_reference.hpp>

#include <sake/core/concepts/Function.hpp>
#include <sake/core/functional/operators/plus.hpp>
#include <sake/core/math/zero.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/move.hpp>
#include <sake/core/move/rv.hpp>
#include <sake/core/move/rv_sink.hpp>
#include <sake/core/range/algorithm/fwd.hpp>
#include <sake/core/range/algorithm/private/accumulate.hpp>
#include <sake/core/range/concepts/fwd.hpp>
#include <sake/core/range/concepts/Range.hpp>
#include <sake/core/range/forward_traits.hpp>
#include <sake/core/range/forward_traits_fwd.hpp>
#include <sake/core/utility/call_traits.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>

namespace sake
{

namespace range
{

namespace algorithm
{

/*******************************************************************************
 * struct range::algorithm::result_of::accumulate< R, T = ..., F = ... >
 ******************************************************************************/

namespace result_of
{

template<
    class R,
    class T = typename sake::range_forward_traits<R>::value_type,
    class F = sake::operators::functional::plus
>
struct accumulate
{
    typedef typename boost_ext::remove_qualifiers<T>::type type;
private:
    BOOST_CONCEPT_ASSERT((sake::concepts::Range<
        typename boost_ext::remove_reference<R>::type >));
    BOOST_CONCEPT_ASSERT((sake::concepts::Function<
        typename boost_ext::remove_qualifiers<F>::type,
        type (
            typename sake::result_of::move< type& >::type,
            typename sake::range_forward_reference<R>::type
        )
    >));
};

} // namespace result_of

/*******************************************************************************
 * struct range::algorithm::functional::accumulate
 ******************************************************************************/

namespace functional
{

struct accumulate
{
    SAKE_RESULT_FROM_METAFUNCTION(
        sake::range::algorithm::result_of::accumulate, (1,3) )

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class R >
    typename result< accumulate ( R ) >::type
    operator()(R&& r) const
    { return apply_impl(sake::forward<R>(r)); }

    template< class R, class T >
    typename result< accumulate ( R, T ) >::type
    operator()(R&& r, T&& x) const
    { return apply_impl(sake::forward<R>(r), sake::forward<T>(x)); }

    template< class R, class T, class F >
    typename result< accumulate ( R, T, F ) >::type
    operator()(R&& r, T&& x, F const & f) const
    { return apply_impl(sake::forward<R>(r), sake::forward<T>(x), f); }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class R >
    typename result< accumulate ( R& ) >::type
    operator()(R& r) const
    { return apply_impl(r); }

    template< class R >
    typename result< accumulate ( R const & ) >::type
    operator()(R const & r) const
    { return apply_impl(r); }

private:
    typedef sake::rv_sink_traits<> rv_sink_traits_;
    template< class T, class F = sake::operators::functional::plus >
    class rv_sink_visitor
    {
        T& x;
        typename sake::call_traits<F>::param_type f;
        rv_sink_visitor(T& x_, typename sake::call_traits<F>::param_type f_)
            : x(x_), f(f_)
        { }
        friend struct accumulate;
    public:
        typedef T result_type;
        template< class R >
        T operator()(SAKE_RV_REF( R ) r) const
        { return apply_impl(r, sake::move(x), f); }
    };
    template< class, class > friend class rv_sink_visitor;
    template< class T, class F = sake::operators::functional::plus >
    struct rv_sink_default
    { typedef rv_sink_traits_::default_< rv_sink_visitor<T,F> > type; };
public:

    // lvalues + movable explicit rvalues
    template< class R, class T >
    typename result< accumulate ( R&, T ) >::type
    operator()(R& r, T x) const
    { return apply_impl(r, sake::move(x)); }
    template< class R, class T, class F >
    typename result< accumulate ( R&, T, F ) >::type
    operator()(R& r, T x, F const & f) const
    { return apply_impl(r, sake::move(x), f); }

    template< class T >
    T operator()(typename rv_sink_default<T>::type r, T x) const
    { return r(rv_sink_visitor<T>(x, sake::operators::plus)); }
    template< class T, class F >
    T operator()(typename rv_sink_default<T,F>::type r, T x, F const & f) const
    { return r(rv_sink_visitor<T,F>(x,f)); }

    // const lvalues + non-movable rvalues
    template< class R, class T >
    typename rv_sink_traits_::template cref_lazy_enabler<
        R, result< accumulate ( R const &, T ) > >::type
    operator()(R const & r, T x) const
    { return apply_impl(r, sake::move(x)); }
    template< class R, class T, class F >
    typename rv_sink_traits_::template cref_lazy_enabler<
        R, result< accumulate ( R const &, T, F ) > >::type
    operator()(R const & r, T x, F const & f) const
    { return apply_impl(r, sake::move(x), f); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

private:
    template< class R >
    static typename result< accumulate (
        SAKE_FWD2_PARAM( R ) ) >::type
    apply_impl(SAKE_FWD2_REF( R ) r)
    {
        typedef typename result< accumulate (
            SAKE_FWD2_PARAM( R ) ) >::type result_type;
        return accumulate_private::impl(
            sake::forward<R>(r),
            sake::zero.as< result_type >(),
            sake::operators::plus
        );
    }

    template< class R, class T >
    static typename result< accumulate (
        SAKE_FWD2_PARAM( R ), SAKE_FWD2_PARAM( T ) ) >::type
    apply_impl(SAKE_FWD2_REF( R ) r, SAKE_FWD2_REF( T ) x)
    {
        return accumulate_private::impl(
            sake::forward<R>(r),
            sake::forward<T>(x),
            sake::operators::plus
        );
    }

    template< class R, class T, class F >
    static typename result< accumulate (
        SAKE_FWD2_PARAM( R ), SAKE_FWD2_PARAM( T ), F ) >::type
    apply_impl(SAKE_FWD2_REF( R ) r, SAKE_FWD2_REF( T ) x, F const & f)
    {
        return accumulate_private::impl(
            sake::forward<R>(r),
            sake::forward<T>(x),
            f
        );
    }
};

} // namespace functional

static sake::range::algorithm::functional::accumulate const accumulate = { };

} // namespace algorithm

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_ALGORITHM_ACCUMULATE_HPP

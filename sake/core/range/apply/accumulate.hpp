/*******************************************************************************
 * sake/core/range/apply/accumulate.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * range::apply::accumulate(T x)
 *     -> range::apply::result_of::accumulate<T>::type
 * range::apply::accumulate(T x, F f)
 *     -> range::apply::result_of::accumulate<T,F>::type
 * struct range::apply::functional::accumulate< T = void, F = ... >
 *
 * r | range::apply::accumulate
 *     -> range::apply::result_of::accumulate<>::apply<R>::type
 * r | range::apply::accumulate(x)
 *     -> range::apply::result_of::accumulate<T>::apply<R>::type
 * r | range::apply::accumulate(x,f)
 *     -> range::apply::result_of::accumulate<T,F>::apply<R>::type
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_APPLY_ACCUMULATE_HPP
#define SAKE_CORE_RANGE_APPLY_ACCUMULATE_HPP

#include <boost/config.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/add_reference_add_const.hpp>
#include <sake/boost_ext/type_traits/is_base_of_sans_qualifiers.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>
#include <sake/boost_ext/type_traits/remove_rvalue_reference.hpp>

#include <sake/core/config.hpp>
#include <sake/core/emplacer/construct.hpp>
#include <sake/core/functional/operators/plus.hpp>
#include <sake/core/memberwise/copy_tags.hpp>
#include <sake/core/memberwise/default_constructor.hpp>
#include <sake/core/memberwise/destructor_tags.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/rv_sink.hpp>
#include <sake/core/range/algorithm/accumulate.hpp>
#include <sake/core/range/algorithm/fwd.hpp>
#include <sake/core/range/apply/fwd.hpp>
#include <sake/core/utility/compressed_pair/compressed_pair.hpp>
#include <sake/core/utility/overload.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>
#include <sake/core/utility/value_constructor_enable.hpp>

namespace sake
{

namespace range
{

namespace apply
{

/*******************************************************************************
 * struct range::algorithm::result_of::accumulate< T, F = ... >
 ******************************************************************************/

namespace result_of
{

template<
    class T,
    class F = sake::operators::functional::plus
>
class accumulate
{
    typedef typename boost_ext::remove_qualifiers<T>::type value_type;
    typedef typename boost_ext::remove_qualifiers<F>::type function_type;
public:
    typedef sake::range::apply::functional::accumulate<
        value_type, function_type > type;

    template< class R >
    struct apply
    { typedef typename sake::range::algorithm::
        result_of::accumulate< R, value_type, function_type >::type type; };
};

} // namespace result_of

namespace functional
{

template< class _ >
struct accumulate< void, _ >
{
    SAKE_RESULT_FROM_METAFUNCTION(
        sake::range::apply::result_of::accumulate, (1,2) )

    template< class T >
    typename result< accumulate ( T ) >::type
    operator()(T const & x) const
    { return typename result< accumulate ( T ) >::type(x) ; }

    template< class T, class F >
    typename result< accumulate ( T, F ) >::type
    operator()(T const & x, F const & f) const
    { return typename result< accumulate ( T, F ) >::type(x,f); }
};

template< class T /*= void*/, class F /*= sake::operators::functional::plus*/ >
struct accumulate
{
private:
    typedef sake::compressed_pair<T,F> m_pair_type;
public:

    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename accumulate,
        (( m_pair_type )( m_pair ))
    )
    SAKE_MEMBERWISE_COPY_TAGS( (( m_pair_type )( m_pair )) )
    SAKE_MEMBERWISE_DESTRUCTOR_TAGS( (( m_pair_type )( m_pair )) )

private:
    template< class T_, class F_ = void >
    struct constructor_enable
        : boost_ext::mpl::and2<
              sake::value_constructor_enable< T, T_ >,
              sake::value_constructor_enable< F, F_ >
          >
    { };
    template< class T_ >
    struct constructor_enable< T_, void >
        : boost_ext::mpl::and2<
              boost_ext::not_is_base_of_sans_qualifiers< accumulate, T_ >,
              sake::value_constructor_enable< T, T_ >
          >
    { };
    template< class T_ >
    struct constructor_enabler
        : boost::enable_if_c< constructor_enable<
              typename boost_ext::remove_rvalue_reference< T_ >::type >::value >
    { };
public:

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class T_ >
    explicit accumulate(T_&& x,
        typename constructor_enabler< T_ >::type* = 0)
        : m_pair(sake::forward< T_ >(x), sake::constructor::emplacer())
    { }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class T_ >
    explicit accumulate(T_& x,
        typename constructor_enabler< T_& >::type* = 0)
        : m_pair(x, sake::construct::emplacer())
    { }

    template< class T_ >
    explicit accumulate(T_ const & x,
        typename constructor_enabler< T_ const & >::type* = 0)
        : m_pair(x, sake::construct::emplacer())
    { }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

    // template< class T_, class F_ >
    // accumulate(T_&& x, F_&& f)
    //     : m_pair(sake::forward< T_ >(x), sake::forward< F_ >(f))
    // { }
#define SAKE_OVERLOAD_CONSTRUCTOR_NAME \
    accumulate
#define SAKE_OVERLOAD_ENABLE( r, n, T_tuple ) \
    constructor_enable< SAKE_BOOST_EXT_PP_TUPLE_REM2 T_tuple >
#define SAKE_OVERLOAD_INITIALIZATION_LIST( r, \
    n, T_tuple, x_tuple, forward_x_tuple ) \
    m_pair forward_x_tuple
#define SAKE_OVERLOAD_BODY( r, n, T_tuple, x_tuple, forward_x_tuple )
#define SAKE_OVERLOAD_MIN_ARITY         2
#define SAKE_OVERLOAD_PERFECT_MAX_ARITY 2
#include SAKE_OVERLOAD_GENERATE()

    typedef T value_type;

    typename boost_ext::add_reference<T>::type
    value()
    { return m_pair.first(); }
    typename boost_ext::add_reference_add_const<T>::type
    value() const
    { return m_pair.first(); }

    typedef F function_type;

    typename boost_ext::add_reference<F>::type
    function()
    { return m_pair.second(); }
    typename boost_ext::add_reference_add_const<F>::type
    function() const
    { return m_pair.second(); }

    typedef typename boost_ext::remove_qualifiers<T>::type result_type;

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class R >
    result_type operator()(R&& r) const
    { return apply_impl(sake::forward<R>(r)); }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

private:
    typedef sake::rv_sink_traits<> rv_sink_traits_;
    typedef rv_sink_traits_::default_< accumulate > rv_sink_default_type;
public:

    // lvalues + movable explicit rvalues
    template< class R >
    result_type
    operator()(R& r) const
    { return apply_impl(r); }

    // movable implicit rvalues
    result_type
    operator()(rv_sink_default_type r) const
    { return r(*this); }

    // const lvalues + non-movable rvalues
    template< class R >
    typename rv_sink_traits_::cref_enabler< R, result_type >::type
    operator()(R const & r) const
    { return apply_impl(r); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

private:
    m_pair_type m_pair;

    template< class R >
    result_type apply_impl(SAKE_FWD2_REF( R ) r) const
    {
        return sake::range::algorithm::functional::accumulate()(
            sake::forward<R>(r), value(), function());
    }
};

#ifndef BOOST_NO_RVALUE_REFERENCES

template< class R >
inline typename sake::range::algorithm::result_of::accumulate<R>::type
operator|(R&& r, sake::range::apply::functional::accumulate<>)
{
    return sake::range::algorithm::functional::accumulate()(
        sake::forward<R>(r));
}

template< class R, class T, class F >
inline typename sake::range::apply::functional::accumulate<T,F>::result_type
operator|(R&& r, sake::range::apply::functional::accumulate<T,F> const & _)
{
    return sake::range::algorithm::functional::accumulate()(
        sake::forward<R>(r), _.value(), _.function());
}

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

template< class R >
inline typename sake::range::algorithm::result_of::accumulate< R& >::type
operator|(R& r, sake::range::apply::functional::accumulate<>)
{ return sake::range::algorithm::functional::accumulate()(r); }

template< class R >
inline typename sake::range::algorithm::result_of::accumulate< R const & >::type
operator|(R const & r, sake::range::apply::functional::accumulate<>)
{ return sake::range::algorithm::functional::accumulate()(r); }

template< class R, class T, class F >
inline typename sake::range::apply::functional::accumulate<T,F>::result_type
operator|(
    R& r, sake::range::apply::functional::accumulate<T,F> const & _)
{
    return sake::range::algorithm::functional::accumulate()(
        sake::forward<R>(r), _.value(), _.function());
}

template< class T, class F >
inline typename sake::range::apply::functional::accumulate<T,F>::result_type
operator|(
    sake::rv_sink_traits<>::default_<
        sake::range::apply::functional::accumulate<T,F> > r,
    sake::range::apply::functional::accumulate<T,F> const & _)
{ r(_); }

template< class R, class T, class F >
inline typename sake::rv_sink_traits<>::cref_enabler<
    R, typename sake::range::apply::functional::accumulate<T,F>::result_type
>::type
operator|(
    R const & r, sake::range::apply::functional::accumulate<T,F> const & _)
{
    return sake::range::algorithm::functional::accumulate()(
        sake::forward<R>(r), _.value(), _.function());
}

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

} // namespace functional

#ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
namespace accumulate_adl_barrier
{ sake::range::apply::functional::accumulate<> const accumulate = { }; }
using namespace accumulate_adl_barrier
#else // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
sake::range::apply::functional::accumulate<> const accumulate = { };
#endif // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS

} // namespace apply

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_APPLY_ACCUMULATE_HPP

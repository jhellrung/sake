/*******************************************************************************
 * sake/core/range/apply/for_each.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * range::apply::for_each(F f)
 *     -> range::apply::result_of::for_each<F>::type
 * struct range::apply::functional::for_each< F = void >
 *
 * r | range::apply::for_each(f) -> void
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_APPLY_FOR_EACH_HPP
#define SAKE_CORE_RANGE_APPLY_FOR_EACH_HPP

#include <boost/config.hpp>
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
#include <sake/core/move/rv_sink.hpp>
#include <sake/core/range/algorithm/for_each.hpp>
#include <sake/core/range/algorithm/fwd.hpp>
#include <sake/core/range/apply/fwd.hpp>
#include <sake/core/utility/base_member.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>
#include <sake/core/utility/value_constructor_enable.hpp>

namespace sake
{

namespace range
{

namespace apply
{

/*******************************************************************************
 * struct range::apply::result_of::for_each<F>
 ******************************************************************************/

namespace result_of
{

template< class F >
class for_each
{
    typedef typename boost_ext::remove_qualifiers<F>::type function_type;
public:
    typedef sake::range::apply::functional::for_each< function_type > type;

    template< class R >
    struct apply
    { typedef typename sake::range::algorithm::
        result_of::for_each< R, function_type >::type type; };
};

} // namespace result_of

namespace functional
{

template<>
struct for_each< void >
{
    SAKE_RESULT_FROM_METAFUNCTION( sake::range::apply::result_of::for_each, 1 )

    template< class F >
    typename result< for_each ( F ) >::type
    operator()(F const & f) const
    { return typename result< for_each ( F ) >::type(f); }
};

template< class F /* = void*/ >
struct for_each
    : sake::base_member<F>
{
private:
    typedef sake::base_member<F> base_member_;
public:

    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename for_each,
        (( base_member_ ))
    )
    SAKE_MEMBERWISE_COPY_TAGS( (( base_member_ )) )
    SAKE_MEMBERWISE_DESTRUCTOR_TAGS( (( base_member_ )) )

private:
    template< class F_ >
    struct constructor_enable
        : boost_ext::mpl::and2<
              boost_ext::not_is_base_of_sans_qualifiers< for_each, F_ >,
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
    explicit for_each(F_&& f,
        typename constructor_enabler< F_ >::type* = 0)
        : base_member_(sake::forward< F_ >(f))
    { }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class F_ >
    explicit for_each(F_& f,
        typename constructor_enabler< F_& >::type* = 0)
        : base_member_(f)
    { }

    template< class F_ >
    explicit for_each(F_ const & f,
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

    typedef void result_type;

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class R >
    void operator()(R&& r) const
    { apply_impl(sake::forward<R>(r)); }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

private:
    typedef sake::rv_sink_traits<> rv_sink_traits_;
    typedef rv_sink_traits_::default_< for_each > rv_sink_default_type;
public:

    // lvalues + movable explicit rvalues
    template< class R >
    void
    operator()(R& r) const
    { apply_impl(r); }

    // movable implicit rvalues
    void
    operator()(rv_sink_default_type r) const
    { r(*this); }

    // const lvalues + non-movable rvalues
    template< class R >
    typename rv_sink_traits_::cref_enabler<R>::type
    operator()(R const & r) const
    { apply_impl(r); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

private:
    template< class R >
    void apply_impl(SAKE_FWD2_REF( R ) r) const
    {
        sake::range::algorithm::functional::for_each()(
            sake::forward<R>(r), function());
    }
};

#ifndef BOOST_NO_RVALUE_REFERENCES

template< class R, class F >
inline void
operator|(R&& r, sake::range::apply::functional::for_each<F> const & _)
{
    sake::range::algorithm::functional::for_each()(
        sake::forward<R>(r), _.function());
}

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

template< class R, class F >
inline void
operator|(R& r, sake::range::apply::functional::for_each<F> const & _)
{ sake::range::algorithm::functional::for_each()(r, _.function()); }

template< class F >
inline void
operator|(
    sake::rv_sink_traits<>::default_<
        sake::range::apply::functional::for_each<F> > r,
    sake::range::apply::functional::for_each<F> const & _)
{ r(_); }

template< class R, class F >
inline typename sake::rv_sink_traits<>::cref_enabler<R>::type
operator|(R const & r, sake::range::apply::functional::for_each<F> const & _)
{ sake::range::algorithm::functional::for_each()(r, _.function()); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

} // namespace functional

#ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
namespace for_each_adl_barrier
{ sake::range::apply::functional::for_each<> const for_each = { }; }
using namespace for_each_adl_barrier;
#else // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
sake::range::apply::functional::for_each<> const for_each = { };
#endif // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS

} // namespace apply

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_APPLY_FOR_EACH_HPP

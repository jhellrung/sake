/*******************************************************************************
 * sake/core/range/algorithm/for_each.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * range::algorithm::for_each(R&& r, F f) -> void
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_ALGORITHM_FOR_EACH_HPP
#define SAKE_CORE_RANGE_ALGORITHM_FOR_EACH_HPP

#include <boost/config.hpp>
#include <boost/concept/assert.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_void.hpp>

#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/add_reference_add_const.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>
#include <sake/boost_ext/type_traits/remove_reference.hpp>

#include <sake/core/concepts/Function.hpp>
#include <sake/core/memberwise/default_constructor.hpp>
#include <sake/core/memberwise/type_trait_tag.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/rv_sink.hpp>
#include <sake/core/range/algorithm/fwd.hpp>
#include <sake/core/range/algorithm/private/for_each.hpp>
#include <sake/core/range/concepts/fwd.hpp>
#include <sake/core/range/concepts/Range.hpp>
#include <sake/core/utility/base_member.hpp>
#include <sake/core/utility/call_traits.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>

namespace sake
{

namespace range
{

namespace algorithm
{

/*******************************************************************************
 * struct range::algorithm::result_of::for_each< R, F = void >
 ******************************************************************************/

namespace result_of
{

template< class R, class F = void >
struct for_each
{
private:
    BOOST_CONCEPT_ASSERT((sake::concepts::Range<
        typename boost_ext::remove_reference<R>::type >));
    BOOST_CONCEPT_ASSERT((sake::concepts::Function<
        typename boost_ext::remove_qualifiers<F>::type,
        void ( typename sake::range_forward_reference<R>::type )
    >));
public:
    typedef void type;
};

template< class F >
struct for_each< F, void >
{
    typedef sake::range::algorithm::functional::for_each<
        typename boost_ext::remove_qualifiers<F>::type > type;
};

} // namespace result_of

namespace functional
{

/*******************************************************************************
 * struct range::algorithm::functional::for_each<F>
 ******************************************************************************/

template< class F /* = void*/ >
struct for_each
    : sake::base_member<F>
{
    BOOST_STATIC_ASSERT((!boost::is_void<F>::value));
private:
    typedef sake::base_member<F> base_member_;
public:

    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename for_each,
        (( base_member_ ))
    )
    SAKE_MEMBERWISE_TYPEDEF_TYPE_TRAIT_TAG(
        (( base_member_ )),
        ( has_copy_constructor )
        ( has_nothrow_copy_constructor )
        ( has_nothrow_copy_assign )
    )

    explicit for_each(typename sake::call_traits<F>::param_type f)
        : base_member_(f)
    { }

    typename boost_ext::add_reference<F>::type
    function()
    { return base_member_::member(); }
    typename boost_ext::add_reference_add_const<F>::type
    function() const
    { return base_member_::member(); }

    typedef void result_type;

private:
    template< class R >
    void apply_impl(SAKE_FWD2_REF( R ) r) const
    { for_each_private::impl(sake::forward<R>(r), function()); }
public:

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
    void operator()(R& r) const
    { apply_impl(r); }

    // movable implicit rvalues
    void operator()(rv_sink_default_type r) const
    { r(*this); }

    // const lvalues + non-movable rvalues
    template< class R >
    typename rv_sink_traits_::template cref_enabler<R>::type
    operator()(R const & r) const
    { apply_impl(r); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

};

/*******************************************************************************
 * struct range::algorithm::functional::for_each< void >
 ******************************************************************************/

template<>
struct for_each< void >
{
    SAKE_RESULT_FROM_METAFUNCTION(
        sake::range::algorithm::result_of::for_each, (1,2) )

    template< class F >
    typename result< for_each ( F ) >::type
    operator()(F const & f) const
    { return typename result< for_each ( F ) >::type(f); }

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class R, class F >
    void operator()(R&& r, F const & f) const
    { for_each_private::impl(sake::forward<R>(r), f); }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

private:
    typedef sake::rv_sink_traits<> rv_sink_traits_;
    template< class F >
    struct rv_sink_default
    { typedef rv_sink_traits_::default_<
        typename result< for_each ( F ) >::type > type; };
public:

    // lvalues + movable explicit rvalues
    template< class R, class F >
    void operator()(R& r, F const & f) const
    { for_each_private::impl(r,f); }

    // movable implicit rvalues
    template< class F >
    void operator()(typename rv_sink_default<F>::type r, F const & f) const
    { r(operator()(f)); }

    // const lvalues + non-movable rvalues
    template< class R, class F >
    typename rv_sink_traits_::cref_enabler<R>::type
    operator()(R const & r, F const & f) const
    { for_each_private::impl(r,f); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

};

} // namespace functional

static sake::range::algorithm::functional::for_each<> const for_each = { };

} // namespace algorithm

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_ALGORITHM_FOR_EACH_HPP

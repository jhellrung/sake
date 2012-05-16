/*******************************************************************************
 * sake/core/move/rv_sink.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct rv_sink_traits<
 *     Sequence = boost::mpl::vector0<>,
 *     Pred = boost::mpl::always< boost::true_type >
 * >
 * struct rv_sink_traits0<
 *     Pred = boost::mpl::always< boost::true_type >
 * >
 * struct rv_sink_traits1<
 *     T,
 *     Pred = boost::mpl::always< boost::true_type >
 * >
 * struct primary_rv_sink<T>
 *
 * struct rv_sink_visitors::operator_assign<T>
 * rv_sink_visitors::make_operator_assign(T& x)
 *     -> rv_sink_visitors::operator_assign<T>
 ******************************************************************************/

#ifndef SAKE_CORE_MOVE_RV_SINK_HPP
#define SAKE_CORE_MOVE_RV_SINK_HPP

#include <boost/config.hpp>

#ifdef BOOST_NO_RVALUE_REFERENCES

#include <boost/mpl/always.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/back_inserter.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/filter_view.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/vector/vector0.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/mpl/any.hpp>
#include <sake/boost_ext/mpl/curry_quote.hpp>
#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/mpl/result_type.hpp>
#include <sake/boost_ext/type_traits/is_cv_or.hpp>
#include <sake/boost_ext/type_traits/remove_rvalue_reference.hpp>

#include <sake/core/move/has_move_emulation.hpp>
#include <sake/core/move/move.hpp>
#include <sake/core/move/rv.hpp>
#include <sake/core/introspection/has_type_result_type.hpp>
#include <sake/core/utility/address_of.hpp>
#include <sake/core/utility/default_tag.hpp>
#include <sake/core/utility/noncopyable.hpp>

namespace sake
{

template< class T >
struct primary_rv_sink;

namespace rv_sink_private
{

template< class Visitor, class Result >
struct default_base;

} // namespace rv_sink_private

/*******************************************************************************
 * struct rv_sink_traits<
 *     Sequence = boost::mpl::vector0<>,
 *     Pred = boost::mpl::always< boost::true_type >
 * >
 ******************************************************************************/

template<
    class Sequence = boost::mpl::vector0<>,
    class Pred = boost::mpl::always< boost::true_type >
>
struct rv_sink_traits
{
    BOOST_STATIC_ASSERT((boost::mpl::is_sequence< Sequence >::value));
    BOOST_STATIC_ASSERT((!boost_ext::mpl::any<
        Sequence,
        boost::mpl::quote1< boost_ext::is_cv_or >
    >::value));

    typedef Sequence value_types;

    typedef typename boost::mpl::transform<
        Sequence,
        boost::mpl::quote1< sake::primary_rv_sink >,
        boost::mpl::back_inserter< boost::mpl::vector0<> >
    >::type primary_types;

private:
    template< class U > struct default_constructor_enabler;
public:
    template<
        class Visitor,
        class Result = sake::default_tag
    >
    class default_;
    template< class, class > friend class default_;

    template< class U >
    struct ref_enable;
    template< class U, class Result = void >
    struct ref_enabler;

    template< class U >
    struct cref_enable;
    template< class U, class Result = void >
    struct cref_enabler;
};

/*******************************************************************************
 * rv_sink_traits member definitions
 ******************************************************************************/

template< class Sequence, class Pred >
template< class U >
struct rv_sink_traits< Sequence, Pred >::
default_constructor_enabler
    : boost::enable_if_c< boost_ext::mpl::and3<
          sake::has_move_emulation<U>,
          boost::mpl::not_< boost_ext::mpl::any<
              Sequence,
              typename boost_ext::mpl::curry_quote2< boost::is_same >::apply<U>::type
          > >,
          boost::mpl::apply1< Pred, U >
      >::value >
{ };

template< class Sequence, class Pred >
template< class Visitor, class Result >
class rv_sink_traits< Sequence, Pred >::
default_
    : public rv_sink_private::default_base< Visitor, Result >
{
    typedef rv_sink_private::default_base< Visitor, Result > default_base_;
public:
    // implicit by design
    template< class U >
    default_(U const & x,
        typename default_constructor_enabler<U>::type* = 0)
        : default_base_(const_cast< U& >(x))
    { }
};

template< class Sequence, class Pred >
template< class U >
struct rv_sink_traits< Sequence, Pred >::
ref_enable
    : boost::mpl::apply1<
          Pred, typename boost_ext::remove_rvalue_reference< U& >::type
      >::type
{ };

template< class Sequence, class Pred >
template< class U, class Result >
struct rv_sink_traits< Sequence, Pred >::
ref_enabler
    : boost::enable_if_c< ref_enable<U>::value, Result >
{ };

template< class Sequence, class Pred >
template< class U >
struct rv_sink_traits< Sequence, Pred >::
cref_enable
    : boost_ext::mpl::and2<
          boost::mpl::not_< boost_ext::mpl::or3<
              boost_ext::is_rvalue_reference< U const & >,
              sake::has_move_emulation<U>,
              boost_ext::mpl::any<
                  boost::mpl::filter_view<
                      Sequence, boost::mpl::quote1< sake::has_move_emulation > >,
                  typename boost_ext::mpl::curry_quote2<
                      boost::is_same >::apply<U>::type
              >
          > >,
          boost::mpl::apply1< Pred, U const & >
      >
{ };

template< class Sequence, class Pred >
template< class U, class Result >
struct rv_sink_traits< Sequence, Pred >::
cref_enabler
    : boost::enable_if_c< cref_enable<U>::value, Result >
{ };

/*******************************************************************************
 * struct rv_sink_traits0<
 *     Pred = boost::mpl::always< boost::true_type >
 * >
 ******************************************************************************/

template<
    class Pred = boost::mpl::always< boost::true_type >
>
struct rv_sink_traits0
    : sake::rv_sink_traits< boost::mpl::vector0<>, Pred >
{ };

/*******************************************************************************
 * struct rv_sink_traits1<
 *     T,
 *     Pred = boost::mpl::always< boost::true_type >
 * >
 ******************************************************************************/

template<
    class T,
    class Pred = boost::mpl::always< boost::true_type >
>
struct rv_sink_traits1
    : sake::rv_sink_traits< boost::mpl::vector1<T>, Pred >
{
    typedef T value_type;

    typedef sake::primary_rv_sink<T> primary_type;
};

/*******************************************************************************
 * struct rv_sink_visitors::operator_assign<T>
 * rv_sink_visitors::make_operator_assign(T& x)
 *     -> rv_sink_visitors::operator_assign<T>
 ******************************************************************************/

namespace rv_sink_visitors
{

template< class T >
struct operator_assign
{
    explicit operator_assign(T& this_) : m_this(this_) { }
    typedef T& result_type;
    template< class U >
    result_type operator()(SAKE_RV_REF( U ) x) const
    { return m_this = x; }
private:
    T& m_this;
};

template< class T >
inline sake::rv_sink_visitors::operator_assign<T>
make_operator_assign(T& x)
{ return sake::rv_sink_visitors::operator_assign<T>(x); }

} // namespace rv_sink_visitors

/*******************************************************************************
 * struct primary_rv_sink<T>
 ******************************************************************************/

template< class T >
struct primary_rv_sink
{
    BOOST_STATIC_ASSERT((!boost_ext::is_cv_or<T>::value));

    T& value;

    typename sake::result_of::move< T& >::type
    move() const
    { return sake::move(value); }

    SAKE_NONCOPYABLE( primary_rv_sink )

    // implicit by design
    template< class U >
    primary_rv_sink(U const & value_,
        typename boost::enable_if_c< boost::is_same<T,U>::value >::type* = 0)
        : value(const_cast< T& >(value_))
    { }
};

template< class T >
struct primary_rv_sink< T& >
{ };

namespace rv_sink_private
{

template< class Visitor, class Result >
struct default_base
{
    typedef typename sake::lazy_replace_default_tag<
        Result,
        boost::mpl::eval_if<
            sake::has_type_result_type< Visitor >,
            boost_ext::mpl::result_type< Visitor >,
            boost::mpl::identity< void >
        >
    >::type result_type;

    SAKE_NONCOPYABLE( default_base )

    result_type operator()() const
    { return m_apply(Visitor(), mp); }

    result_type operator()(Visitor v) const
    { return m_apply(v, mp); }

protected:
    template< class U >
    explicit default_base(U& x)
        : m_apply(apply<U>),
          mp(static_cast< void* >(sake::address_of(x)))
    { }

private:
    template< class U >
    static result_type apply(Visitor v, void* p)
    { return v(sake::move(*static_cast< U* >(p))); }

    result_type (&m_apply)(Visitor, void*);
    void* const mp;
};

} // namespace rv_sink_private

} // namespace sake

#endif // #ifdef BOOST_NO_RVALUE_REFERENCES

#endif // #ifndef SAKE_CORE_MOVE_RV_SINK_HPP

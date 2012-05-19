/*******************************************************************************
 * sake/boost_ext/fusion/support/default_rv_sink.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * class boost_ext::fusion::default_rv_sink<
 *     Results,
 *     Sequence = boost::mpl::vector0<>,
 *     Pred = boost::mpl::always< boost::true_type >,
 * >
 * class boost_ext::fusion::default_rv_sink0<
 *     Results,
 *     Pred = boost::mpl::always< boost::true_type >,
 * >
 * class boost_ext::fusion::default_rv_sink1<
 *     Results,
 *     T,
 *     Pred = boost::mpl::always< boost::true_type >,
 * >
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_FUSION_SUPPORT_DEFAULT_RV_SINK_HPP
#define SAKE_BOOST_EXT_FUSION_SUPPORT_DEFAULT_RV_SINK_HPP

#include <boost/config.hpp>

#ifdef BOOST_NO_RVALUE_REFERENCES

#include <cstddef>

#include <boost/fusion/support/is_sequence.hpp>
#include <boost/mpl/always.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/vector/vector0.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/fusion/convert/forward_as_mpl_sequence.hpp>
#include <sake/boost_ext/fusion/sequence/intrinsic/at.hpp>
#include <sake/boost_ext/fusion/support/is_sequence_with_category.hpp>
#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/mpl/any.hpp>
#include <sake/boost_ext/mpl/curry_quote.hpp>
#include <sake/boost_ext/type_traits/is_cv_or.hpp>

#include <sake/core/move/has_move_emulation.hpp>
#include <sake/core/move/move.hpp>
#include <sake/core/utility/address_of.hpp>
#include <sake/core/utility/cast/implicit.hpp>
#include <sake/core/utility/is_convertible_wnrbt.hpp>
#include <sake/core/utility/noncopy_assignable.hpp>
#include <sake/core/utility/noncopyable.hpp>
#include <sake/core/utility/workaround.hpp>

namespace sake
{

namespace boost_ext
{

namespace fusion
{

namespace default_rv_sink_private
{

template< class Results, class U >
struct equal_is_convertible_wnrbt_helper;

template<
    class Results,
    std::size_t N = boost::mpl::size< Results >::value
>
class at_dispatch_base;

} // namespace default_rv_sink_private

/*******************************************************************************
 * struct boost_ext::fusion::default_rv_sink<
 *     Results,
 *     Sequence = boost::mpl::vector0<>,
 *     Pred = boost::mpl::always< boost::true_type >,
 * >
 ******************************************************************************/

template<
    class Results,
    class Sequence = boost::mpl::vector0<>,
    class Pred = boost::mpl::always< boost::true_type >
>
class default_rv_sink
    : public default_rv_sink_private::at_dispatch_base< Results >
{
    BOOST_STATIC_ASSERT((boost::mpl::is_sequence< Results >::value));
    BOOST_STATIC_ASSERT((boost::mpl::is_sequence< Sequence >::value));
    BOOST_STATIC_ASSERT((!boost_ext::mpl::any<
        Sequence,
        boost::mpl::quote1< boost_ext::is_cv_or >
    >::value));
    typedef default_rv_sink_private::at_dispatch_base< Results > at_dispatch_base_;
public:
#if SAKE_WORKAROUND_GNUC_VERSION_LESS_EQUAL( (4,6,3) )
    // GCC 4.6.3 requires default_base to be copy constructible in order for
    // function arguments to bind to default_<...>.
    SAKE_NONCOPY_ASSIGNABLE( default_rv_sink )
#else // #if SAKE_WORKAROUND_GNUC_VERSION_LESS_EQUAL( (4,6,3) )
    SAKE_NONCOPYABLE( default_rv_sink )
#endif // #if SAKE_WORKAROUND_GNUC_VERSION_LESS_EQUAL( (4,6,3) )

protected:
    template< class U >
    struct constructor_enabler
        : boost::enable_if_c< boost_ext::mpl::and5<
              sake::has_move_emulation<U>,
              boost_ext::fusion::traits::is_random_access_sequence<U>,
              default_rv_sink_private::equal_is_convertible_wnrbt_helper< Results, U >,
              boost::mpl::not_< boost_ext::mpl::any<
                  Sequence,
                  typename boost_ext::mpl::curry_quote2< boost::is_same >::apply<U>::type
              > >,
              boost::mpl::apply1< Pred, U >
          >::value >
    { };
public:
    // implicit by design
    template< class U >
    default_rv_sink(U const & x,
        typename constructor_enabler<U>::type* = 0)
        : at_dispatch_base_(const_cast< U& >(x))
    { }

    template< class N >
    typename boost::mpl::at_c< Results, N::value >::type
    operator[](N) const
    {
        typedef boost::integral_constant< std::size_t, N::value > N_;
        return at_dispatch_base_::at_impl(N_());
    }
};

/*******************************************************************************
 * class boost_ext::fusion::default_rv_sink0<
 *     Results,
 *     Pred = boost::mpl::always< boost::true_type >,
 * >
 ******************************************************************************/

template<
    class Results,
    class Pred = boost::mpl::always< boost::true_type >
>
class default_rv_sink0
    : public default_rv_sink< Results, boost::mpl::vector0<>, Pred >
{
    typedef default_rv_sink< Results, boost::mpl::vector0<>, Pred > default_rv_sink_;
public:
    // implicit by design
    template< class U >
    default_rv_sink0(U const & x,
        typename default_rv_sink_::template constructor_enabler<U>::type* = 0)
        : default_rv_sink_(x)
    { }
};

/*******************************************************************************
 * class boost_ext::fusion::default_rv_sink1<
 *     Results,
 *     T,
 *     Pred = boost::mpl::always< boost::true_type >,
 * >
 ******************************************************************************/

template<
    class Results,
    class T,
    class Pred = boost::mpl::always< boost::true_type >
>
class default_rv_sink1
    : public default_rv_sink< Results, boost::mpl::vector1<T>, Pred >
{
    typedef default_rv_sink< Results, boost::mpl::vector1<T>, Pred > default_rv_sink_;
public:
    // implicit by design
    template< class U >
    default_rv_sink1(U const & x,
        typename default_rv_sink_::template constructor_enabler<U>::type* = 0)
        : default_rv_sink_(x)
    { }
};

namespace default_rv_sink_private
{

template< class Results, class U >
struct equal_is_convertible_wnrbt_helper
    : boost::mpl::equal<
          typename boost_ext::fusion::result_of::forward_as_mpl_sequence<U>::type,
          Results,
          boost::mpl::quote2< sake::is_convertible_wnrbt >
      >
{ };

template< class Result, std::size_t N, class U >
Result at_impl(void* p)
{
    return sake::implicit_cast< Result >(
        boost_ext::fusion::at_c<N>(
            sake::move(*static_cast< U* >(p))));
}

#if SAKE_WORKAROUND_MSC_VERSION_LESS_EQUAL( 1500 )

template< class Result >
inline typename boost::enable_if_c<
    sake::has_move_emulation< Result >::value, Result >::type
at_helper(Result (&at_impl_)( void* ), void* const p)
{ return static_cast< SAKE_RV_REF( Result ) >(at_impl_(p)); }

template< class Result >
inline typename boost::disable_if_c<
    sake::has_move_emulation< Result >::value, Result >::type
at_helper(Result (&at_impl_)( void* ), void* const p)
{ return at_impl_(p); }

#endif // #if SAKE_WORKAROUND_MSC_VERSION_LESS_EQUAL( 1500 )

template< class Results, std::size_t N >
class at_dispatch_base
    : public at_dispatch_base< Results, N-1 >
{
    typedef at_dispatch_base< Results, N-1 > at_dispatch_base_;
    typedef typename boost::mpl::at_c< Results, N-1 >::type result_type;
protected:

    template< class U >
    explicit at_dispatch_base(U& x)
        : at_dispatch_base_(x),
          m_at_impl(default_rv_sink_private::at_impl< result_type, N-1, U >)
    { }

    using at_dispatch_base_::m_p;
    using at_dispatch_base_::at_impl;

    result_type at_impl(boost::integral_constant< std::size_t, N-1 >) const
#if SAKE_WORKAROUND_MSC_VERSION_LESS_EQUAL( 1500 )
    // MSVC9 won't elide copying of the result, so we explicitly move.
    { return default_rv_sink_private::at_helper(m_at_impl, m_p); }
#else // #if SAKE_WORKAROUND_MSC_VERSION_LESS_EQUAL( 1500 )
    { return m_at_impl(m_p); }
#endif // #if SAKE_WORKAROUND_MSC_VERSION_LESS_EQUAL( 1500 )

private:
    result_type (&m_at_impl)( void* );
};

template< class Results >
class at_dispatch_base< Results, 0 >
{
protected:
    template< class U >
    explicit at_dispatch_base(U& x)
        : m_p(static_cast< void* >(sake::address_of(x)))
    { }

    void* const m_p;

    void at_impl(...) const;
};

} // namespace default_rv_sink_private

} // namespace fusion

} // namespace boost_ext

} // namespace sake

#endif // #ifdef BOOST_NO_RVALUE_REFERENCES

#endif // #ifndef SAKE_BOOST_EXT_FUSION_SUPPORT_DEFAULT_RV_SINK_HPP

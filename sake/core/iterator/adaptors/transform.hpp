/*******************************************************************************
 * sake/core/iterator/adaptors/transform.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * class iterator::adaptors::transform< I, F, Params = boost::mpl::map0<> >
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_ADAPTORS_TRANSFORM_HPP
#define SAKE_CORE_ITERATOR_ADAPTORS_TRANSFORM_HPP

#include <boost/concept/assert.hpp>
#include <boost/config.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/map/map0.hpp>
#include <boost/mpl/map/map10.hpp>
#include <boost/mpl/not.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/mpl/at.hpp>
#include <sake/boost_ext/mpl/lazy_at.hpp>
#include <sake/boost_ext/preprocessor/tuple/rem.hpp>
#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/add_reference_add_const.hpp>
#include <sake/boost_ext/type_traits/is_base_of_sans_qualifiers.hpp>
#include <sake/boost_ext/type_traits/remove_reference.hpp>
#include <sake/boost_ext/type_traits/remove_rvalue_reference.hpp>
#include <sake/boost_ext/utility/result_of.hpp>

#include <sake/core/concepts/Function.hpp>
#include <sake/core/iterator/adaptor.hpp>
#include <sake/core/iterator/adaptors/fwd.hpp>
#include <sake/core/iterator/adaptors/multidim/transform.hpp>
#include <sake/core/iterator/categories.hpp>
#include <sake/core/iterator/concepts/fwd.hpp>
#include <sake/core/iterator/concepts/Iterator.hpp>
#include <sake/core/iterator/core_access.hpp>
#include <sake/core/iterator/is_convertible.hpp>
#include <sake/core/iterator/keyword.hpp>
#include <sake/core/iterator/multidim_traits_fwd.hpp>
#include <sake/core/iterator/traits.hpp>
#include <sake/core/iterator/traits_fwd.hpp>
#include <sake/core/memberwise/copy_tags.hpp>
#include <sake/core/memberwise/destructor_tags.hpp>
#include <sake/core/memberwise/mem_fun.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/utility/is_convertible_wnrbt.hpp>
#include <sake/core/utility/overload.hpp>
#include <sake/core/utility/using_typedef.hpp>
#include <sake/core/utility/value_constructor_enable.hpp>

namespace sake
{

namespace iterator
{

namespace adaptors
{

namespace transform_private
{

template< class I, class F, class Params >
struct traits;

} // namespace transform_private

/*******************************************************************************
 * class iterator::adaptors::transform<I>
 ******************************************************************************/

template< class I, class F, class Params /*= boost::mpl::map0<>*/ >
class transform
    : public transform_private::traits< I, F, Params >::adaptor_
{
    typedef transform_private::traits< I, F, Params > traits_;
    SAKE_USING_TYPEDEF( typename traits_, adaptor_ );
public:
    SAKE_USING_TYPEDEF( typename adaptor_, reference );

    SAKE_MEMBERWISE_MEM_FUN(
        typename transform,
        ( default_constructor )( swap ),
        (( adaptor_ ))
    )
    SAKE_MEMBERWISE_COPY_TAGS( (( adaptor_ )) )
    SAKE_MEMBERWISE_DESTRUCTOR_TAGS( (( adaptor_ )) )

private:
    template< class T0 >
    struct explicit_constructor_enable
        : boost_ext::mpl::and2<
              boost::mpl::not_<
                  boost_ext::is_base_of_sans_qualifiers< transform, T0 > >,
              sake::value_constructor_enable< I, T0 >
          >
    { };
    template< class T0 >
    struct explicit_constructor_enabler
        : boost::enable_if_c< explicit_constructor_enable<
              typename boost_ext::remove_rvalue_reference< T0 >::type
          >::value >
    { };
public:

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class T0 >
    explicit transform(T0&& x0,
        typename explicit_constructor_enabler< T0 >::type* = 0)
        : adaptor_(sake::forward< T0 >(x0))
    { }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class T0 >
    explicit transform(T0& x0,
        typename explicit_constructor_enabler< T0& >::type* = 0)
        : adaptor_(x0)
    { }

    template< class T0 >
    explicit transform(T0 const & x0,
        typename explicit_constructor_enabler< T0 const & >::type* = 0)
        : adaptor_(x0)
    { }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

private:
    template< class T0, class T1 >
    struct constructor_enable
        : boost_ext::mpl::and2<
              sake::value_constructor_enable< I, T0 >,
              sake::value_constructor_enable< F, T1 >
          >
    { };
public:
#define SAKE_OVERLOAD_CONSTRUCTOR_NAME \
    transform
#define SAKE_OVERLOAD_ENABLE( r, n, T_tuple ) \
    constructor_enable< SAKE_BOOST_EXT_PP_TUPLE_REM2 T_tuple >
#define SAKE_OVERLOAD_INITIALIZATION_LIST( \
    r, n, T_tuple, x_tuple, forward_x_tuple ) \
    adaptor_ forward_x_tuple
#define SAKE_OVERLOAD_BODY( r, n, T_tuple, x_tuple, forward_x_tuple )
#define SAKE_OVERLOAD_MIN_ARITY         2
#define SAKE_OVERLOAD_PERFECT_MAX_ARITY 2
#include SAKE_OVERLOAD_GENERATE()

private:
    template< class J, class G >
    struct converting_constructor_enable
        : boost_ext::mpl::and2<
              sake::iterator::is_convertible<J,I>,
              sake::is_convertible_wnrbt<
                  typename boost_ext::add_reference<G>::type, F >
          >
    { };
    template< class J, class G >
    struct converting_constructor_enabler
        : boost::enable_if_c< converting_constructor_enable<J,G>::value >
    { };
public:
    template< class J, class G, class P >
    transform(sake::iterator::adaptors::transform<J,G,P>& other,
        typename converting_constructor_enabler<J,G>::type* = 0)
        : adaptor_(other.base(), other.function())
    { }
    template< class J, class G, class P >
    transform(sake::iterator::adaptors::transform<J,G,P> const & other,
        typename converting_constructor_enabler<
            J, typename boost::add_const<G>::type >::type* = 0)
        : adaptor_(other.base(), other.function())
    { }

    using adaptor_::base;

    typedef F function_type;

    typename boost_ext::add_reference<F>::type
    function()
    { return adaptor_::member(); }
    typename boost_ext::add_reference_add_const<F>::type
    function() const
    { return adaptor_::member(); }

private:
    friend class sake::iterator::core_access;
    template< class, class >
    friend struct sake::extension::iterator_multidim_traits;

    reference derived_dereference() const
    { return function()(*base()); }

    template< class Introterminal >
    struct derived_relax
    {
        typedef sake::iterator::adaptors::transform<
            typename sake::iterator_relax< I, Introterminal >::type,
            F, Params
        > type;
    };

    template< class T, class Introterminal >
    typename adaptor_::template relax< Introterminal >::type
    derived_at(T const & x, Introterminal) const
    {
        return typename adaptor_::template relax< Introterminal >::type(
            adaptor_::base_at(x, Introterminal()), function());
    }
};

/*******************************************************************************
 * namespace iterator::adaptors::transform_private
 ******************************************************************************/

namespace transform_private
{

template< class I, class F, class Params >
struct traits
{
    BOOST_CONCEPT_ASSERT((sake::concepts::Iterator<I>));
    typedef typename sake::iterator_reference<I>::type base_reference;
    BOOST_CONCEPT_ASSERT((
        sake::concepts::Function< F, void ( base_reference ) >));
    BOOST_STATIC_ASSERT((boost::mpl::is_sequence< Params >::value));
    typedef typename boost_ext::mpl::lazy_at<
        Params, sake::iterator::keyword::tag::reference,
        boost_ext::result_of<
            typename boost_ext::remove_reference<F>::type ( base_reference ) >
    >::type reference;
    typedef typename boost_ext::mpl::lazy_at<
        Params, sake::iterator::keyword::tag::value,
        boost_ext::remove_reference< reference >
    >::type facade_value_type;
    typedef typename sake::iterator_traversal<I>::type base_traversal;
    typedef typename boost_ext::mpl::at<
        Params, sake::iterator::keyword::tag::traversal,
        base_traversal
    >::type traversal;
    typedef sake::iterator::adaptor<
        sake::iterator::adaptors::transform< I, F, Params >, I,
        boost::mpl::map4<
            sake::iterator::keyword::value< facade_value_type >,
            sake::iterator::keyword::reference< reference >,
            sake::iterator::keyword::traversal< traversal >,
            sake::iterator::keyword::member<F>
        >
    > adaptor_;
};

} // namespace transform_private

} // namespace adaptors

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_ADAPTORS_TRANSFORM_HPP

/*******************************************************************************
 * sake/core/iterator/adaptors/move.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * class iterator::adaptors::move<I>
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_MOVE_ADAPTORS_ITERATOR_HPP
#define SAKE_CORE_ITERATOR_MOVE_ADAPTORS_ITERATOR_HPP

#include <boost/concept/assert.hpp>
#include <boost/config.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/map/map10.hpp>
#include <boost/mpl/not.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/type_traits/is_base_of_sans_qualifiers.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>
#include <sake/boost_ext/type_traits/is_lvalue_reference_to_nonconst.hpp>
#include <sake/boost_ext/type_traits/remove_reference.hpp>
#include <sake/boost_ext/type_traits/remove_rvalue_reference.hpp>

#include <sake/core/functional/construct.hpp>
#include <sake/core/iterator/adaptor.hpp>
#include <sake/core/iterator/adaptors/fwd.hpp>
#include <sake/core/iterator/adaptors/transform.hpp>
#include <sake/core/iterator/categories.hpp>
#include <sake/core/iterator/concepts/Iterator.hpp>
#include <sake/core/iterator/core_access.hpp>
#include <sake/core/iterator/is_convertible.hpp>
#include <sake/core/iterator/keyword.hpp>
#include <sake/core/iterator/multidim_traits_fwd.hpp>
#include <sake/core/iterator/traits.hpp>
#include <sake/core/memberwise/mem_fun.hpp>
#include <sake/core/memberwise/type_trait_tag.hpp>
#include <sake/core/move/has_move_emulation.hpp>
#include <sake/core/move/move.hpp>
#include <sake/core/range/adaptors/fwd.hpp>
#include <sake/core/range/adaptors/move.hpp>
#include <sake/core/utility/using_typedef.hpp>
#include <sake/core/utility/value_constructor_enable.hpp>

namespace sake
{

namespace iterator
{

namespace adaptors
{

namespace move_private
{

template< class I >
struct traits_bool;
template< class I, bool = traits_bool<I>::value >
struct traits;

} // namespace move_private

/*******************************************************************************
 * class iterator::adaptors::move<I>
 ******************************************************************************/

template< class I >
class move
    : public move_private::traits<I>::adaptor_
{
    typedef move_private::traits<I> traits_;
    SAKE_USING_TYPEDEF( typename traits_, adaptor_ );
public:
    SAKE_USING_TYPEDEF( typename adaptor_, reference );

    SAKE_MEMBERWISE_MEM_FUN(
        typename move,
        ( default_constructor )( swap ),
        (( adaptor_ ))
    )
    SAKE_MEMBERWISE_TYPEDEF_TYPE_TRAIT_TAG(
        (( adaptor_ )),
        ( has_copy_constructor )
        ( has_nothrow_copy_constructor )
        ( has_nothrow_copy_assign )
    )

private:
    template< class T0 >
    struct explicit_constructor_enable
        : boost_ext::mpl::and2<
              boost::mpl::not_<
                  boost_ext::is_base_of_sans_qualifiers< move, T0 > >,
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
    explicit move(T0&& x0,
        typename explicit_constructor_enabler< T0 >::type* = 0)
        : adaptor_(sake::forward< T0 >(x0))
    { }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class T0 >
    explicit move(T0& x0,
        typename explicit_constructor_enabler< T0& >::type* = 0)
        : adaptor_(x0)
    { }

    template< class T0 >
    explicit move(T0 const & x0,
        typename explicit_constructor_enabler< T0 const & >::type* = 0)
        : adaptor_(x0)
    { }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class J >
    move(sake::iterator::adaptors::move<J> const & other,
        typename boost::enable_if_c<
            sake::iterator::is_convertible<J,I>::value >::type* = 0)
        : adaptor_(other.base())
    { }

    using adaptor_::base;

private:
    friend class sake::iterator::core_access;
    template< class, class >
    friend struct sake::extension::iterator_multidim_traits;

    reference derived_dereference() const
    { return traits_::dereference(base()); }

    template< class Introversal >
    struct derived_relax
    {
        typedef sake::iterator::adaptors::move<
            typename sake::iterator_relax< I, Introversal >::type > type;
    };

    template< class T, class Introversal >
    typename derived_relax< Introversal >::type
    derived_at(T const & x, Introversal) const
    {
        return typename derived_relax< Introversal >::type(
            adaptor_::base_at(x, Introversal()));
    }
};

} // namespace adaptors

} // namespace iterator

/*******************************************************************************
 * struct extension::iterator_multidim_traits< iterator::adaptors::move<I> >
 ******************************************************************************/

namespace extension
{

template< class I >
struct iterator_multidim_traits<
    sake::iterator::adaptors::move<I>,
    typename boost::enable_if_c<
        sake::iterator_multidim_enable<I>::value >::type
>
{
    typedef boost::true_type enable_tag;

    typedef sake::iterator::adaptors::move<I> this_type;
    typedef sake::iterator_multidim_traits<I> base_traits;

    typedef typename base_traits::outer_iterator base_outer_iterator;
    typedef typename sake::iterator_reference<
        base_outer_iterator
    >::type base_inner_range;
    typedef sake::range::adaptors::move< base_inner_range > inner_range;

    typedef sake::iterator::adaptors::transform<
        base_outer_iterator,
        sake::functional::construct< inner_range >
    > outer_iterator;

    template< class Introversal >
    struct inner_iterator_with
        : inner_range::template iterator_with< Introversal >
    { };

    static outer_iterator
    outer(this_type const & i)
    { return outer_iterator(base_traits::outer(i.base())); }

    template< class Introversal >
    static typename inner_iterator_with< Introversal >::type
    inner(this_type const & i, Introversal)
    {
        return typename inner_iterator_with< Introversal >::type(
            base_traits::inner(i.base(), Introversal()));
    }

    template< class Inner >
    static void
    at_ip(this_type& i, outer_iterator const & j, Inner const & k)
    {
        base_traits::at_ip(i.protected_base(), j.base(), k.base());
        return i;
    }

    template< class Inner, class Introversal >
    static typename this_type::template relax< Introversal >::type
    at(
        this_type const & i,
        outer_iterator const & j, Inner const & k,
        Introversal)
    {
        return typename this_type::template relax< Introversal >::type(
            base_traits::at(i.base(), j.base(), k.base(), Introversal()));
    }
};

} // namespace extension

/*******************************************************************************
 * namespace iterator::adaptors::move_private
 ******************************************************************************/

namespace iterator
{

namespace adaptors
{

namespace move_private
{

template< class I >
struct traits_bool
{
    BOOST_CONCEPT_ASSERT((sake::concepts::Iterator<I>));
    typedef typename sake::iterator_reference<I>::type reference;
#ifndef BOOST_NO_RVALUE_REFERENCES
    static bool const value =
        boost_ext::is_lvalue_reference_to_nonconst< reference >::value;
#else // #ifndef BOOST_NO_RVALUE_REFERENCES
    static bool const value = boost_ext::mpl::and2<
        boost_ext::is_lvalue_reference_to_nonconst< reference >,
        sake::has_move_emulation<
            typename boost_ext::remove_reference< reference >::type >
    >::value;
#endif // #ifndef BOOST_NO_RVALUE_REFERENCES
};

template< class I >
struct traits< I, false >
{
    typedef typename sake::iterator_reference<I>::type reference;
    typedef sake::iterator::adaptor<
        sake::iterator::adaptors::move<I>, I
    > adaptor_;
    static reference dereference(I const & i)
    { return *i; }
};

template< class I >
struct traits< I, true >
{
    typedef typename sake::iterator_reference<I>::type base_reference;
    typedef typename sake::result_of::move< base_reference >::type reference;
    typedef typename sake::iterator_traversal<I>::type base_traversal;
    typedef typename boost::mpl::if_c<
        boost_ext::is_convertible<
            base_traversal, boost::forward_traversal_tag >::value,
        boost::single_pass_traversal_tag,
        base_traversal
    >::type traversal;
    typedef sake::iterator::adaptor<
        sake::iterator::adaptors::move<I>, I,
        boost::mpl::map2<
            sake::iterator::keyword::reference< reference >,
            sake::iterator::keyword::traversal< traversal >
        >
    > adaptor_;
    static reference dereference(I const & i)
    { return sake::move(*i); }
};

namespace
{

BOOST_CONCEPT_ASSERT((
    sake::concepts::Iterator< sake::iterator::adaptors::move< int* > >));
BOOST_CONCEPT_ASSERT((
    sake::concepts::Iterator< sake::iterator::adaptors::move< int const * > >));

} // namespace

} // namespace move_private

} // namespace adaptors

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_MOVE_ADAPTORS_ITERATOR_HPP

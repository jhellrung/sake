/*******************************************************************************
 * sake/core/iterator/move_iterator.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct move_iterator<I>
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_MOVE_ITERATOR_HPP
#define SAKE_CORE_ITERATOR_MOVE_ITERATOR_HPP

#include <boost/concept/assert.hpp>
#include <boost/config.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/map/map10.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>
#include <sake/boost_ext/type_traits/is_lvalue_reference_to_nonconst.hpp>
#include <sake/boost_ext/type_traits/remove_reference.hpp>

#include <sake/core/iterator/adaptor.hpp>
#include <sake/core/iterator/begin.hpp>
#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/iterator/categories.hpp>
#include <sake/core/iterator/concepts/Iterator.hpp>
#include <sake/core/iterator/core_access.hpp>
#include <sake/core/iterator/end.hpp>
#include <sake/core/iterator/keyword.hpp>
#include <sake/core/iterator/private/adaptor/at_helper.hpp>
#include <sake/core/iterator/traits.hpp>
#include <sake/core/move/has_move_emulation.hpp>
#include <sake/core/move/move.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake
{

namespace move_iterator_private
{

template< class I >
struct traits_bool;
template< class I, bool = traits_bool<I>::value >
struct traits;

} // namespace move_iterator_private

template< class I >
class move_iterator
    : public move_iterator_private::traits<I>::adaptor_
{
    typedef move_iterator_private::traits<I> traits_;
    SAKE_USING_TYPEDEF( typename traits_, adaptor_ );
public:
    SAKE_USING_TYPEDEF( typename adaptor_, reference );

    explicit move_iterator(I const & i)
        : adaptor_(i)
    { }

    using adaptor_::base;

    operator I const & () const
    { return base(); }

private:
    friend class sake::iterator::core_access;

    reference derived_dereference() const
    { return traits_::dereference(base()); }

    template< class Introversal >
    struct derived_relax
    {
        typedef sake::move_iterator<
            typename sake::iterator_relax< I, Introversal >::type > type;
    };

    template< class T, class Introversal >
    typename derived_relax< Introversal >::type
    derived_at(T const & x, Introversal) const
    {
        typedef typename derived_relax< Introversal >::type result_type;
        return result_type(sake::iterator::at(
            base(), sake::iterator::adaptor_private::at_helper<I>(x),
            Introversal()
        ));
    }

    template< class Introversal >
    typename derived_relax< Introversal >::type
    derived_at_begin(Introversal) const
    { return derived_at(sake::_begin, Introversal()); }

    template< class Introversal >
    typename derived_relax< Introversal >::type
    derived_at_end(Introversal) const
    { return derived_at(sake::_end, Introversal()); }
};

namespace move_iterator_private
{

template< class I >
struct traits_bool
{
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
    typedef sake::adaptor<
        sake::move_iterator<I>, I
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
        sake::move_iterator<I>, I,
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
    sake::concepts::Iterator< sake::move_iterator< int* > >));
BOOST_CONCEPT_ASSERT((
    sake::concepts::Iterator< sake::move_iterator< int const * > >));

} // namespace

} // namespace move_iterator_private

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_MOVE_ITERATOR_HPP

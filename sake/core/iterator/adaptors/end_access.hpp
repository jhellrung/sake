/*******************************************************************************
 * sake/core/iterator/adaptors/end_access.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * class iterator::adaptors::end_access<I>
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_ADAPTORS_END_ACCESS_HPP
#define SAKE_CORE_ITERATOR_ADAPTORS_END_ACCESS_HPP

#include <boost/concept/assert.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/static_assert.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/type_traits/add_const_if_c.hpp>
#include <sake/boost_ext/type_traits/common_type.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/data_structures/optional/operators.hpp>
#include <sake/core/data_structures/optional/optional.hpp>
#include <sake/core/introspection/has_operator_less.hpp>
#include <sake/core/iterator/adaptors/fwd.hpp>
#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/iterator/categories.hpp>
#include <sake/core/iterator/concepts/Iterator.hpp>
#include <sake/core/iterator/core_access.hpp>
#include <sake/core/iterator/facade.hpp>
#include <sake/core/iterator/facade_fwd.hpp>
#include <sake/core/iterator/is_const_iterator.hpp>
#include <sake/core/iterator/is_convertible.hpp>
#include <sake/core/iterator/keyword.hpp>
#include <sake/core/iterator/traits.hpp>
#include <sake/core/iterator/traits_fwd.hpp>
#include <sake/core/memberwise/mem_fun.hpp>
#include <sake/core/memberwise/type_trait_tag.hpp>
#include <sake/core/utility/is_template_base_of.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake
{

namespace iterator
{

namespace adaptors
{

namespace end_access_private
{

template< class I >
struct traits;

} // namespace end_access_private

/*******************************************************************************
 * class iterator::adaptors::end_access<I>
 ******************************************************************************/

template< class I >
class end_access
    : public end_access_private::traits<I>::facade_
{
    typedef end_access_private::traits<I> traits_;
    SAKE_USING_TYPEDEF( typename traits_, facade_ );
public:
    SAKE_USING_TYPEDEF( typename facade_, reference );

    typedef I base_type;

    SAKE_MEMBERWISE_MEM_FUN(
        typename end_access,
        ( default_constructor )( swap ),
        (( sake::optional<I> )( m_base ))
    )
    SAKE_MEMBERWISE_TYPEDEF_TYPE_TRAIT_TAG(
        (( sake::optional<I> )( m_base )),
        ( has_copy_constructor )
        ( has_nothrow_copy_constructor )
        ( has_nothrow_copy_assign )
    )

    explicit end_access(I const & i)
        : m_base(i, i != sake::_end)
    { }

    explicit end_access(sake::end_tag)
    { }

    template< class J >
    end_access(end_access<J> const & other,
        typename boost::enable_if_c<
            sake::iterator::is_convertible<J,I>::value >::type* = 0)
        : m_base(other.base())
    { }

    sake::optional<I> const &
    base() const
    { return m_base; }

private:
    sake::optional<I> m_base;

    friend class sake::iterator::core_access;

    reference derived_dereference() const
    { return *m_base.get(); }
    void derived_increment()
    {
        if(++m_base.get() == sake::_end)
            m_base.destruct();
    }

    template< class Other >
    bool derived_equal(Other const & other) const
    { return m_base == other.m_base; }
    bool derived_equal(sake::begin_tag) const
    { return m_base && m_base.get() == sake::_begin; }
    bool derived_equal(sake::end_tag) const
    { return !m_base; }

    template< class Other >
    bool derived_less(Other const & other) const
    { return m_base < other.m_base; }
    template< class Other >
    bool derived_less_equal(Other const & other) const
    { return m_base <= other.m_base; }

    template< class Introterminal >
    class derived_relax
    {
        typedef typename sake::introterminal_join<
            typename sake::introterminal_meet<
                Introterminal,
                sake::begin_detect_introterminal_tag
            >::type,
            sake::end_detect_introterminal_tag
        >::type base_introterminal;
    public:
        typedef sake::iterator::adaptors::end_access<
            typename sake::iterator_relax< I, base_introterminal >::type > type;
    };

    void derived_at_ip(sake::end_tag)
    { m_base.reset(); }

    template< class Introterminal >
    typename facade_::template relax< Introterminal >::type
    derived_at(sake::end_tag, Introterminal) const
    {
        return typename facade_::template
            relax< Introterminal >::type(sake::_end);
    }
};

/*******************************************************************************
 * namespace iterator::adaptors::end_access_private
 ******************************************************************************/

namespace end_access_private
{

template< class I >
struct traits
{
    BOOST_CONCEPT_ASSERT((sake::concepts::Iterator<I>));

    typedef sake::iterator_traits<I> base_traits;
    SAKE_USING_TYPEDEF( typename base_traits, value_type );
    typedef typename boost_ext::add_const_if_c<
        sake::is_const_iterator<I>::value,
        base_value_type
    >::type facade_value_type;

    SAKE_USING_TYPEDEF( typename base_traits, reference );

    SAKE_USING_TYPEDEF( typename base_traits, difference_type );

    typedef typename base_traits::traversal base_traversal;
    typedef typename boost_ext::common_type<
        base_traversal, boost::forward_traversal_tag
    >::type iterator_traversal;

    typedef typename base_traits::introterminal base_introterminal;
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        base_introterminal, sake::end_detect_introterminal_tag >::value));
    BOOST_STATIC_ASSERT((!boost_ext::is_convertible<
        base_introterminal, sake::end_access_introterminal_tag >::value));
    typedef typename sake::introterminal_join<
        typename sake::introterminal_meet<
            base_introterminal, sake::begin_detect_introterminal_tag >::type,
        sake::end_access_introterminal_tag
    >::type iterator_introterminal;

    typedef typename sake::is_template_base_of2<
        sake::iterator::facade, I,
        sake::has_operator_less< boost::mpl::_1 >
    >::type compare_enable;

    typedef sake::iterator::facade<
        sake::iterator::keyword::value< facade_value_type >,
        sake::iterator::keyword::reference< reference >,
        sake::iterator::keyword::difference< difference_type >,
        sake::iterator::keyword::traversal< iterator_traversal >,
        sake::iterator::keyword::introterminal< iterator_introterminal >,
        sake::iterator::keyword::compare_enable< compare_enable >
    > facade_;
};

} // namespace end_access_private

} // namespace adaptors

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_ADAPTORS_END_ACCESS_HPP

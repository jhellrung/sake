/*******************************************************************************
 * sake/core/iterator/adaptors/private/introversal/impl_base.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_ADAPTORS_PRIVATE_INTROVERSAL_IMPL_BASE_HPP
#define SAKE_CORE_ITERATOR_ADAPTORS_PRIVATE_INTROVERSAL_IMPL_BASE_HPP

#include <boost/mpl/has_key.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/core/iterator/adaptors/private/introversal/traits.hpp>
#include <sake/core/iterator/at.hpp>
#include <sake/core/iterator/at_ip.hpp>
#include <sake/core/iterator/begin.hpp>
#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/iterator/core_access.hpp>
#include <sake/core/iterator/end.hpp>
#include <sake/core/iterator/private/adaptor/as_assignable.hpp>
#include <sake/core/iterator/private/adaptor/as_convertible.hpp>
#include <sake/core/iterator/private/adaptor/as_convertible_relax.hpp>
#include <sake/core/memberwise/default_constructor.hpp>
#include <sake/core/memberwise/swap.hpp>
#include <sake/core/memberwise/type_trait_tag.hpp>
#include <sake/core/range/traits.hpp>
#include <sake/core/range/traits_fwd.hpp>
#include <sake/core/utility/int_tag.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake
{

namespace iterator
{

namespace adaptors
{

namespace introversal_private
{

template< class Tags >
struct impl_base_index
{
    static bool const explicit_begin =
        boost::mpl::has_key< Tags, sake::begin_tag >::value;
    static bool const explicit_end =
        boost::mpl::has_key< Tags, sake::end_tag >::value;
    static int const value = explicit_begin + 2 * explicit_end;
};

template<
    class I, class Tags, class IntroversalMask,
    int = impl_base_index< Tags >::value
>
class impl_base;

/*******************************************************************************
 * class iterator::adaptors::introversal_private::impl_base< ..., 0 >
 ******************************************************************************/

template< class I, class Tags, class IntroversalMask >
class impl_base< I, Tags, IntroversalMask, 0 >
    : public introversal_private::traits< I, Tags, IntroversalMask >::adaptor_
{
    typedef introversal_private::traits< I, Tags, IntroversalMask > traits_;
    SAKE_USING_TYPEDEF( typename traits_, adaptor_ );
    SAKE_USING_TYPEDEF( typename traits_, base_introversal );
public:
    using adaptor_::base;
    SAKE_MEMBERWISE_SWAP( typename impl_base, (( adaptor_ )) )
protected:
    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename impl_base,
        (( adaptor_ ))
    )
    SAKE_MEMBERWISE_TYPEDEF_TYPE_TRAIT_TAG(
        (( adaptor_ )),
        ( has_copy_constructor )
        ( has_nothrow_copy_constructor )
        ( has_nothrow_copy_assign )
    )

    template< class R >
    impl_base(R& r, sake::begin_tag)
        : adaptor_(sake::range_traits<R>::begin(r, base_introversal()))
    { }
    template< class R >
    impl_base(R& r, sake::end_tag)
        : adaptor_(sake::range_traits<R>::end(r, base_introversal()))
    { }
    template< class R, class J >
    impl_base(R& r, J const & j)
        : adaptor_(sake::range_traits<R>::iter_at(r, j, base_introversal()))
    { }

    template< class J >
    explicit impl_base(J const & j,
        typename boost::disable_if_c< boost::is_base_of<
            impl_base, J >::value >::type* = 0)
        : adaptor_(sake::iterator::adaptor_private::as_convertible<I>(j))
    { }

    template< class J >
    impl_base(J const & j, sake::begin_tag, sake::end_tag)
        : adaptor_(j)
    { }

    template< class J >
    typename boost::disable_if_c<
        boost::is_base_of< impl_base, J >::value,
        impl_base&
    >::type
    operator=(J const & j)
    {
        adaptor_::protected_base() =
            sake::iterator::adaptor_private::as_assignable<I>(j);
        return *this;
    }

    friend class sake::iterator::core_access;

    template< class J, class Introversal >
    typename adaptor_::template relax< Introversal >::type
    derived_at(J const & j, Introversal) const
    {
        static unsigned int const result_base_value =
            Introversal::value & IntroversalMask::value;
        return sake::iterator::at(
            base(),
            sake::iterator::adaptor_private::as_convertible_relax<I>(j),
            sake::introversal_tag_c< result_base_value >()
        );
    }

    template< class Introversal >
    typename adaptor_::template relax< Introversal >::type
    derived_at(sake::begin_tag, Introversal) const
    { return derived_iter_at(sake::_begin, Introversal()); }

    template< class Introversal >
    typename adaptor_::template relax< Introversal >::type
    derived_at(sake::end_tag, Introversal) const
    { return derived_iter_at(sake::_end, Introversal()); }
};

/*******************************************************************************
 * class iterator::adaptors::introversal_private::impl_base< ..., 1 >
 ******************************************************************************/

template< class I, class Tags, class IntroversalMask >
class impl_base< I, Tags, IntroversalMask, 1 >
    : public introversal_private::traits< I, Tags, IntroversalMask >::adaptor_
{
    typedef introversal_private::traits< I, Tags, IntroversalMask > traits_;
    SAKE_USING_TYPEDEF( typename traits_, adaptor_ );
    SAKE_USING_TYPEDEF( typename traits_, base_introversal );
    SAKE_USING_TYPEDEF( typename traits_, null_base_type );
public:
    SAKE_USING_TYPEDEF( typename adaptor_, difference_type );
    using adaptor_::base;
protected:
    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename impl_base,
        (( adaptor_ ))
        (( null_base_type )( m_begin ))
    )

    template< class R >
    impl_base(R& r, sake::begin_tag)
        : adaptor_(sake::range_traits<R>::begin(r, base_introversal())),
          m_begin(adaptor_::base())
    { }
    template< class R >
    impl_base(R& r, sake::end_tag)
        : adaptor_(sake::range_traits<R>::begin(r, base_introversal())),
          m_begin(sake::range_traits<R>::begin(r))
    { }
    template< class R, class J >
    impl_base(R& r, J const & j)
        : adaptor_(sake::range_traits<R>::iter_at(r, j, base_introversal())),
          m_begin(sake::range_traits<R>::begin(r))
    { }

    template< class J >
    explicit impl_base(J const & j,
        typename boost::disable_if_c< boost::is_base_of<
            impl_base, J >::value >::type* = 0)
        : adaptor_(sake::iterator::adaptor_private::as_convertible<I>(j)),
          m_begin(sake::iterator::adaptor_private::
              as_convertible< null_base_type >(sake::iterator::begin(j)))
    { }

    template< class J, class Begin >
    impl_base(J const & j, Begin const & begin_, sake::end_tag)
        : adaptor_(j),
          m_begin(begin_)
    { }

    template< class J >
    typename boost::disable_if_c<
        boost::is_base_of< impl_base, J >::value,
        impl_base&
    >::type
    operator=(J const & j)
    {
        adaptor_::protected_base() =
            sake::iterator::adaptor_private::as_assignable<I>(j);
        m_begin = sake::iterator::adaptor_private::
            as_assignable< null_base_type >(sake::iterator::begin(j));
        return *this;
    }

    friend class sake::iterator::core_access;

    using adaptor_::derived_equal;
    bool derived_equal(sake::begin_tag) const
    { return base() == m_begin; }

    using adaptor_::derived_difference;
    difference_type derived_difference(sake::begin_tag) const
    { return base() - m_begin; }

    using adaptor_::derived_at_ip;
    void derived_at_ip(sake::begin_tag)
    { sake::iterator::at_ip(adaptor_::protected_base(), m_begin); }

    template< class J, class Introversal >
    typename adaptor_::template relax< Introversal >::type
    derived_at(J const & j, Introversal) const
    {
        static bool const explicit_begin = 0 !=
           (sake::begin_access_introversal_tag::value
          & ~(base_introversal::value & IntroversalMask::value)
          & Introversal::value);
        typedef sake::int_tag< explicit_begin > int_tag_;
        return derived_at(j, Introversal(), int_tag_());
    }

    template< class J, class Introversal >
    typename adaptor_::template relax< Introversal >::type
    derived_at(J const & j, Introversal, sake::int_tag<0>) const
    {
        static unsigned int const result_base_value =
            Introversal::value & IntroversalMask::value;
        return sake::iterator::at(
            base(),
            sake::iterator::adaptor_private::as_convertible_relax<I>(j),
            sake::introversal_tag_c< result_base_value >()
        );
    }

    template< class J, class Introversal >
    typename adaptor_::template relax< Introversal >::type
    derived_at(J const & j, Introversal, sake::int_tag<1>) const
    {
        typedef typename adaptor_::template
            relax< Introversal >::type result_type;
        static unsigned int const result_base_value =
            sake::end_access_introversal_tag::value
          & Introversal::value & IntroversalMask::value;
        return result_type(
            sake::iterator::at(
                base(),
                sake::iterator::adaptor_private::as_convertible_relax<I>(j),
                sake::introversal_tag_c< result_base_value >()
            ),
            m_begin, sake::_end
        );
    }

    template< class Introversal >
    typename adaptor_::template relax< Introversal >::type
    derived_at(sake::begin_tag, Introversal) const
    { return derived_at(m_begin, Introversal()); }

private:
    null_base_type m_begin;
};

/*******************************************************************************
 * class iterator::adaptors::introversal_private::impl_base< ..., 2 >
 ******************************************************************************/

template< class I, class Tags, class IntroversalMask >
class impl_base< I, Tags, IntroversalMask, 2 >
    : public introversal_private::traits< I, Tags, IntroversalMask >::adaptor_
{
    typedef introversal_private::traits< I, Tags, IntroversalMask > traits_;
    SAKE_USING_TYPEDEF( typename traits_, adaptor_ );
    SAKE_USING_TYPEDEF( typename traits_, base_introversal );
    SAKE_USING_TYPEDEF( typename traits_, null_base_type );
public:
    SAKE_USING_TYPEDEF( typename adaptor_, difference_type );
    using adaptor_::base;
protected:
    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename impl_base,
        (( adaptor_ ))
        (( null_base_type )( m_end ))
    )

    template< class R >
    impl_base(R& r, sake::begin_tag)
        : adaptor_(sake::range_traits<R>::begin(r, base_introversal())),
          m_end(sake::range_traits<R>::end(r))
    { }
    template< class R >
    impl_base(R& r, sake::end_tag)
        : adaptor_(sake::range_traits<R>::end(r, base_introversal())),
          m_end(adaptor_::base())
    { }
    template< class R, class J >
    impl_base(R& r, J const & j)
        : adaptor_(sake::range_traits<R>::iter_at(r, j, base_introversal())),
          m_end(sake::range_traits<R>::end(r))
    { }

    template< class J >
    explicit impl_base(J const & j,
        typename boost::disable_if_c< boost::is_base_of<
            impl_base, J >::value >::type* = 0)
        : adaptor_(sake::iterator::adaptor_private::as_convertible<I>(j)),
          m_end(sake::iterator::adaptor_private::
              as_convertible< null_base_type >(sake::iterator::end(j)))
    { }

    template< class J, class End >
    impl_base(J const & j, sake::begin_tag, End const & end_)
        : adaptor_(j),
          m_end(end_)
    { }

    template< class J >
    typename boost::disable_if_c<
        boost::is_base_of< impl_base, J >::value,
        impl_base&
    >::type
    operator=(J const & j)
    {
        adaptor_::protected_base() =
            sake::iterator::adaptor_private::as_assignable<I>(j);
        m_end = sake::iterator::adaptor_private::
            as_assignable< null_base_type >(sake::iterator::end(j));
        return *this;
    }

    friend class sake::iterator::core_access;

    using adaptor_::derived_equal;
    bool derived_equal(sake::end_tag) const
    { return base() == m_end; }

    using adaptor_::derived_difference;
    difference_type derived_difference(sake::end_tag) const
    { return base() - m_end; }

    using adaptor_::derived_at_ip;
    void derived_at_ip(sake::end_tag)
    { sake::iterator::at_ip(adaptor_::protected_base(), m_end); }

    template< class J, class Introversal >
    typename adaptor_::template relax< Introversal >::type
    derived_at(J const & j, Introversal) const
    {
        static bool const explicit_end = 0 !=
           (sake::end_access_introversal_tag::value
          & ~(base_introversal::value & IntroversalMask::value)
          & Introversal::value);
        typedef sake::int_tag< explicit_end > int_tag_;
        return derived_at(j, Introversal(), int_tag_());
    }

    template< class J, class Introversal >
    typename adaptor_::template relax< Introversal >::type
    derived_at(J const & j, Introversal, sake::int_tag<0>) const
    {
        static unsigned int const result_base_value =
            Introversal::value & IntroversalMask::value;
        return sake::iterator::at(
            base(),
            sake::iterator::adaptor_private::as_convertible_relax<I>(j),
            sake::introversal_tag_c< result_base_value >()
        );
    }

    template< class J, class Introversal >
    typename adaptor_::template relax< Introversal >::type
    derived_at(J const & j, Introversal, sake::int_tag<1>) const
    {
        typedef typename adaptor_::template
            relax< Introversal >::type result_type;
        static unsigned int const result_base_value =
            sake::begin_access_introversal_tag::value
          & Introversal::value & IntroversalMask::value;
        return result_type(
            sake::iterator::at(
                base(),
                sake::iterator::adaptor_private::as_convertible_relax<I>(j),
                sake::introversal_tag_c< result_base_value >()
            ),
            sake::_begin, m_end
        );
    }

    template< class Introversal >
    typename adaptor_::template relax< Introversal >::type
    derived_at(sake::end_tag, Introversal) const
    { return derived_at(m_end, Introversal()); }

private:
    null_base_type m_end;
};

/*******************************************************************************
 * class iterator::adaptors::introversal_private::impl_base< ..., 3 >
 ******************************************************************************/

template< class I, class Tags, class IntroversalMask >
class impl_base< I, Tags, IntroversalMask, 3 >
    : public introversal_private::traits< I, Tags, IntroversalMask >::adaptor_
{
    typedef introversal_private::traits< I, Tags, IntroversalMask > traits_;
    SAKE_USING_TYPEDEF( typename traits_, adaptor_ );
    SAKE_USING_TYPEDEF( typename traits_, base_introversal );
    SAKE_USING_TYPEDEF( typename traits_, null_base_type );
public:
    SAKE_USING_TYPEDEF( typename adaptor_, difference_type );
    using adaptor_::base;
protected:
    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename impl_base,
        (( adaptor_ ))
        (( null_base_type )( m_begin ))
        (( null_base_type )( m_end ))
    )

    template< class R >
    impl_base(R& r, sake::begin_tag)
        : adaptor_(sake::range_traits<R>::begin(r, base_introversal())),
          m_begin(adaptor_::base()),
          m_end(sake::range_traits<R>::end(r))
    { }
    template< class R >
    impl_base(R& r, sake::end_tag)
        : adaptor_(sake::range_traits<R>::end(r, base_introversal())),
          m_begin(sake::range_traits<R>::begin(r)),
          m_end(adaptor_::base())
    { }
    template< class R, class J >
    impl_base(R& r, J const & j)
        : adaptor_(sake::range_traits<R>::iter_at(r, j, base_introversal())),
          m_begin(sake::range_traits<R>::begin(r)),
          m_end(sake::range_traits<R>::end(r))
    { }

    template< class J >
    explicit impl_base(J const & j,
        typename boost::disable_if_c< boost::is_base_of<
            impl_base, J >::value >::type* = 0)
        : adaptor_(sake::iterator::adaptor_private::as_convertible<I>(j)),
          m_begin(sake::iterator::adaptor_private::
              as_convertible< null_base_type >(sake::iterator::begin(j))),
          m_end(sake::iterator::adaptor_private::
              as_convertible< null_base_type >(sake::iterator::end(j)))
    { }

    template< class J, class Begin, class End >
    impl_base(J const & j, Begin const & begin_, End const & end_)
        : adaptor_(j),
          m_begin(begin_),
          m_end(end_)
    { }

    template< class J >
    typename boost::disable_if_c<
        boost::is_base_of< impl_base, J >::value,
        impl_base&
    >::type
    operator=(J const & j)
    {
        adaptor_::protected_base() =
            sake::iterator::adaptor_private::as_assignable<I>(j);
        m_begin = sake::iterator::adaptor_private::
            as_assignable< null_base_type >(sake::iterator::begin(j));
        m_end = sake::iterator::adaptor_private::
            as_assignable< null_base_type >(sake::iterator::end(j));
        return *this;
    }

    friend class sake::iterator::core_access;

    using adaptor_::derived_equal;
    bool derived_equal(sake::begin_tag) const
    { return base() == m_begin; }
    bool derived_equal(sake::end_tag) const
    { return base() == m_end; }

    using adaptor_::derived_difference;
    difference_type derived_difference(sake::begin_tag) const
    { return base() - m_begin; }
    difference_type derived_difference(sake::end_tag) const
    { return base() - m_end; }

    using adaptor_::derived_at_ip;
    void derived_at_ip(sake::begin_tag)
    { sake::iterator::at_ip(adaptor_::protected_base(), m_begin); }
    void derived_at_ip(sake::end_tag)
    { sake::iterator::at_ip(adaptor_::protected_base(), m_end); }

    template< class J, class Introversal >
    typename adaptor_::template relax< Introversal >::type
    derived_at(J const & j, Introversal) const
    {
        static bool const explicit_begin = 0 !=
           (sake::begin_access_introversal_tag::value
          & ~(base_introversal::value & IntroversalMask::value)
          & Introversal::value);
        static bool const explicit_end = 0 !=
           (sake::end_access_introversal_tag::value
          & ~(base_introversal::value & IntroversalMask::value)
          & Introversal::value);
        typedef sake::int_tag< explicit_begin + 2 * explicit_end > int_tag_;
        return derived_at(j, Introversal(), int_tag_());
    }

    template< class J, class Introversal >
    typename adaptor_::template relax< Introversal >::type
    derived_at(J const & j, Introversal, sake::int_tag<0>) const
    {
        static unsigned int const result_base_value =
            Introversal::value & IntroversalMask::value;
        return sake::iterator::at(
            base(),
            sake::iterator::adaptor_private::as_convertible_relax<I>(j),
            sake::introversal_tag_c< result_base_value >()
        );
    }

    template< class J, class Introversal >
    typename adaptor_::template relax< Introversal >::type
    derived_at(J const & j, Introversal, sake::int_tag<1>) const
    {
        typedef typename adaptor_::template
            relax< Introversal >::type result_type;
        static unsigned int const result_base_value =
            sake::end_access_introversal_tag::value
          & Introversal::value & IntroversalMask::value;
        return result_type(
            sake::iterator::at(
                base(),
                sake::iterator::adaptor_private::as_convertible_relax<I>(j),
                sake::introversal_tag_c< result_base_value >()
            ),
            m_begin, sake::_end
        );
    }

    template< class J, class Introversal >
    typename adaptor_::template relax< Introversal >::type
    derived_at(J const & j, Introversal, sake::int_tag<2>) const
    {
        typedef typename adaptor_::template
            relax< Introversal >::type result_type;
        static unsigned int const result_base_value =
            sake::begin_access_introversal_tag::value
          & Introversal::value & IntroversalMask::value;
        return result_type(
            sake::iterator::at(
                base(),
                sake::iterator::adaptor_private::as_convertible_relax<I>(j),
                sake::introversal_tag_c< result_base_value >()
            ),
            sake::_begin, m_end
        );
    }

    template< class J, class Introversal >
    typename adaptor_::template relax< Introversal >::type
    derived_at(J const & j, Introversal, sake::int_tag<3>) const
    {
        typedef typename adaptor_::template
            relax< Introversal >::type result_type;
        return result_type(
            sake::iterator::at(
                base(),
                sake::iterator::adaptor_private::as_convertible_relax<I>(j),
                sake::null_introversal_tag()
            ),
            m_begin, m_end
        );
    }

    template< class Introversal >
    typename adaptor_::template relax< Introversal >::type
    derived_at(sake::begin_tag, Introversal) const
    { return derived_at(m_begin, Introversal()); }

    template< class Introversal >
    typename adaptor_::template relax< Introversal >::type
    derived_at(sake::end_tag, Introversal) const
    { return derived_at(m_end, Introversal()); }

private:
    null_base_type m_begin;
    null_base_type m_end;
};

} // namespace introversal_private

} // namespace adaptors

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_ADAPTORS_PRIVATE_INTROVERSAL_IMPL_BASE_HPP

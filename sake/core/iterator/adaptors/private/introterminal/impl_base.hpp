/*******************************************************************************
 * sake/core/iterator/adaptors/private/introterminal/impl_base.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_ADAPTORS_PRIVATE_INTROTERMINAL_IMPL_BASE_HPP
#define SAKE_CORE_ITERATOR_ADAPTORS_PRIVATE_INTROTERMINAL_IMPL_BASE_HPP

#include <boost/mpl/has_key.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/core/iterator/adaptors/private/introterminal/traits.hpp>
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

namespace introterminal_private
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
    class I, class Tags, class IntroterminalMask,
    int = impl_base_index< Tags >::value
>
class impl_base;

/*******************************************************************************
 * class iterator::adaptors::introterminal_private::impl_base< ..., 0 >
 ******************************************************************************/

template< class I, class Tags, class IntroterminalMask >
class impl_base< I, Tags, IntroterminalMask, 0 >
    : public introterminal_private::traits<
          I, Tags, IntroterminalMask >::adaptor_
{
    typedef introterminal_private::traits<
        I, Tags, IntroterminalMask > traits_;
    SAKE_USING_TYPEDEF( typename traits_, adaptor_ );
    SAKE_USING_TYPEDEF( typename traits_, base_introterminal );
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
        : adaptor_(sake::range_traits<R>::begin(r, base_introterminal()))
    { }
    template< class R >
    impl_base(R& r, sake::end_tag)
        : adaptor_(sake::range_traits<R>::end(r, base_introterminal()))
    { }
    template< class R, class J >
    impl_base(R& r, J const & j)
        : adaptor_(sake::range_traits<R>::iter_at(r, j, base_introterminal()))
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

    template< class J, class Introterminal >
    typename adaptor_::template relax< Introterminal >::type
    derived_at(J const & j, Introterminal) const
    {
        static unsigned int const result_base_value =
            Introterminal::value & IntroterminalMask::value;
        return sake::iterator::at(
            base(),
            sake::iterator::adaptor_private::as_convertible_relax<I>(j),
            sake::introterminal_tag_c< result_base_value >()
        );
    }

    template< class Introterminal >
    typename adaptor_::template relax< Introterminal >::type
    derived_at(sake::begin_tag, Introterminal) const
    { return derived_iter_at(sake::_begin, Introterminal()); }

    template< class Introterminal >
    typename adaptor_::template relax< Introterminal >::type
    derived_at(sake::end_tag, Introterminal) const
    { return derived_iter_at(sake::_end, Introterminal()); }
};

/*******************************************************************************
 * class iterator::adaptors::introterminal_private::impl_base< ..., 1 >
 ******************************************************************************/

template< class I, class Tags, class IntroterminalMask >
class impl_base< I, Tags, IntroterminalMask, 1 >
    : public introterminal_private::traits<
          I, Tags, IntroterminalMask >::adaptor_
{
    typedef introterminal_private::traits<
        I, Tags, IntroterminalMask > traits_;
    SAKE_USING_TYPEDEF( typename traits_, adaptor_ );
    SAKE_USING_TYPEDEF( typename traits_, base_introterminal );
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
        : adaptor_(sake::range_traits<R>::begin(r, base_introterminal())),
          m_begin(adaptor_::base())
    { }
    template< class R >
    impl_base(R& r, sake::end_tag)
        : adaptor_(sake::range_traits<R>::begin(r, base_introterminal())),
          m_begin(sake::range_traits<R>::begin(r))
    { }
    template< class R, class J >
    impl_base(R& r, J const & j)
        : adaptor_(sake::range_traits<R>::iter_at(r, j, base_introterminal())),
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

    template< class J, class Introterminal >
    typename adaptor_::template relax< Introterminal >::type
    derived_at(J const & j, Introterminal) const
    {
        static bool const explicit_begin = 0 !=
           (sake::begin_access_introterminal_tag::value
          & ~(base_introterminal::value & IntroterminalMask::value)
          & Introterminal::value);
        typedef sake::int_tag< explicit_begin > int_tag_;
        return derived_at(j, Introterminal(), int_tag_());
    }

    template< class J, class Introterminal >
    typename adaptor_::template relax< Introterminal >::type
    derived_at(J const & j, Introterminal, sake::int_tag<0>) const
    {
        static unsigned int const result_base_value =
            Introterminal::value & IntroterminalMask::value;
        return sake::iterator::at(
            base(),
            sake::iterator::adaptor_private::as_convertible_relax<I>(j),
            sake::introterminal_tag_c< result_base_value >()
        );
    }

    template< class J, class Introterminal >
    typename adaptor_::template relax< Introterminal >::type
    derived_at(J const & j, Introterminal, sake::int_tag<1>) const
    {
        typedef typename adaptor_::template
            relax< Introterminal >::type result_type;
        static unsigned int const result_base_value =
            sake::end_access_introterminal_tag::value
          & Introterminal::value & IntroterminalMask::value;
        return result_type(
            sake::iterator::at(
                base(),
                sake::iterator::adaptor_private::as_convertible_relax<I>(j),
                sake::introterminal_tag_c< result_base_value >()
            ),
            m_begin, sake::_end
        );
    }

    template< class Introterminal >
    typename adaptor_::template relax< Introterminal >::type
    derived_at(sake::begin_tag, Introterminal) const
    { return derived_at(m_begin, Introterminal()); }

private:
    null_base_type m_begin;
};

/*******************************************************************************
 * class iterator::adaptors::introterminal_private::impl_base< ..., 2 >
 ******************************************************************************/

template< class I, class Tags, class IntroterminalMask >
class impl_base< I, Tags, IntroterminalMask, 2 >
    : public introterminal_private::traits<
          I, Tags, IntroterminalMask >::adaptor_
{
    typedef introterminal_private::traits<
        I, Tags, IntroterminalMask > traits_;
    SAKE_USING_TYPEDEF( typename traits_, adaptor_ );
    SAKE_USING_TYPEDEF( typename traits_, base_introterminal );
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
        : adaptor_(sake::range_traits<R>::begin(r, base_introterminal())),
          m_end(sake::range_traits<R>::end(r))
    { }
    template< class R >
    impl_base(R& r, sake::end_tag)
        : adaptor_(sake::range_traits<R>::end(r, base_introterminal())),
          m_end(adaptor_::base())
    { }
    template< class R, class J >
    impl_base(R& r, J const & j)
        : adaptor_(sake::range_traits<R>::iter_at(r, j, base_introterminal())),
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

    template< class J, class Introterminal >
    typename adaptor_::template relax< Introterminal >::type
    derived_at(J const & j, Introterminal) const
    {
        static bool const explicit_end = 0 !=
           (sake::end_access_introterminal_tag::value
          & ~(base_introterminal::value & IntroterminalMask::value)
          & Introterminal::value);
        typedef sake::int_tag< explicit_end > int_tag_;
        return derived_at(j, Introterminal(), int_tag_());
    }

    template< class J, class Introterminal >
    typename adaptor_::template relax< Introterminal >::type
    derived_at(J const & j, Introterminal, sake::int_tag<0>) const
    {
        static unsigned int const result_base_value =
            Introterminal::value & IntroterminalMask::value;
        return sake::iterator::at(
            base(),
            sake::iterator::adaptor_private::as_convertible_relax<I>(j),
            sake::introterminal_tag_c< result_base_value >()
        );
    }

    template< class J, class Introterminal >
    typename adaptor_::template relax< Introterminal >::type
    derived_at(J const & j, Introterminal, sake::int_tag<1>) const
    {
        typedef typename adaptor_::template
            relax< Introterminal >::type result_type;
        static unsigned int const result_base_value =
            sake::begin_access_introterminal_tag::value
          & Introterminal::value & IntroterminalMask::value;
        return result_type(
            sake::iterator::at(
                base(),
                sake::iterator::adaptor_private::as_convertible_relax<I>(j),
                sake::introterminal_tag_c< result_base_value >()
            ),
            sake::_begin, m_end
        );
    }

    template< class Introterminal >
    typename adaptor_::template relax< Introterminal >::type
    derived_at(sake::end_tag, Introterminal) const
    { return derived_at(m_end, Introterminal()); }

private:
    null_base_type m_end;
};

/*******************************************************************************
 * class iterator::adaptors::introterminal_private::impl_base< ..., 3 >
 ******************************************************************************/

template< class I, class Tags, class IntroterminalMask >
class impl_base< I, Tags, IntroterminalMask, 3 >
    : public introterminal_private::traits<
          I, Tags, IntroterminalMask >::adaptor_
{
    typedef introterminal_private::traits<
        I, Tags, IntroterminalMask > traits_;
    SAKE_USING_TYPEDEF( typename traits_, adaptor_ );
    SAKE_USING_TYPEDEF( typename traits_, base_introterminal );
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
        : adaptor_(sake::range_traits<R>::begin(r, base_introterminal())),
          m_begin(adaptor_::base()),
          m_end(sake::range_traits<R>::end(r))
    { }
    template< class R >
    impl_base(R& r, sake::end_tag)
        : adaptor_(sake::range_traits<R>::end(r, base_introterminal())),
          m_begin(sake::range_traits<R>::begin(r)),
          m_end(adaptor_::base())
    { }
    template< class R, class J >
    impl_base(R& r, J const & j)
        : adaptor_(sake::range_traits<R>::iter_at(r, j, base_introterminal())),
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

    template< class J, class Introterminal >
    typename adaptor_::template relax< Introterminal >::type
    derived_at(J const & j, Introterminal) const
    {
        static bool const explicit_begin = 0 !=
           (sake::begin_access_introterminal_tag::value
          & ~(base_introterminal::value & IntroterminalMask::value)
          & Introterminal::value);
        static bool const explicit_end = 0 !=
           (sake::end_access_introterminal_tag::value
          & ~(base_introterminal::value & IntroterminalMask::value)
          & Introterminal::value);
        typedef sake::int_tag< explicit_begin + 2 * explicit_end > int_tag_;
        return derived_at(j, Introterminal(), int_tag_());
    }

    template< class J, class Introterminal >
    typename adaptor_::template relax< Introterminal >::type
    derived_at(J const & j, Introterminal, sake::int_tag<0>) const
    {
        static unsigned int const result_base_value =
            Introterminal::value & IntroterminalMask::value;
        return sake::iterator::at(
            base(),
            sake::iterator::adaptor_private::as_convertible_relax<I>(j),
            sake::introterminal_tag_c< result_base_value >()
        );
    }

    template< class J, class Introterminal >
    typename adaptor_::template relax< Introterminal >::type
    derived_at(J const & j, Introterminal, sake::int_tag<1>) const
    {
        typedef typename adaptor_::template
            relax< Introterminal >::type result_type;
        static unsigned int const result_base_value =
            sake::end_access_introterminal_tag::value
          & Introterminal::value & IntroterminalMask::value;
        return result_type(
            sake::iterator::at(
                base(),
                sake::iterator::adaptor_private::as_convertible_relax<I>(j),
                sake::introterminal_tag_c< result_base_value >()
            ),
            m_begin, sake::_end
        );
    }

    template< class J, class Introterminal >
    typename adaptor_::template relax< Introterminal >::type
    derived_at(J const & j, Introterminal, sake::int_tag<2>) const
    {
        typedef typename adaptor_::template
            relax< Introterminal >::type result_type;
        static unsigned int const result_base_value =
            sake::begin_access_introterminal_tag::value
          & Introterminal::value & IntroterminalMask::value;
        return result_type(
            sake::iterator::at(
                base(),
                sake::iterator::adaptor_private::as_convertible_relax<I>(j),
                sake::introterminal_tag_c< result_base_value >()
            ),
            sake::_begin, m_end
        );
    }

    template< class J, class Introterminal >
    typename adaptor_::template relax< Introterminal >::type
    derived_at(J const & j, Introterminal, sake::int_tag<3>) const
    {
        typedef typename adaptor_::template
            relax< Introterminal >::type result_type;
        return result_type(
            sake::iterator::at(
                base(),
                sake::iterator::adaptor_private::as_convertible_relax<I>(j),
                sake::null_introterminal_tag()
            ),
            m_begin, m_end
        );
    }

    template< class Introterminal >
    typename adaptor_::template relax< Introterminal >::type
    derived_at(sake::begin_tag, Introterminal) const
    { return derived_at(m_begin, Introterminal()); }

    template< class Introterminal >
    typename adaptor_::template relax< Introterminal >::type
    derived_at(sake::end_tag, Introterminal) const
    { return derived_at(m_end, Introterminal()); }

private:
    null_base_type m_begin;
    null_base_type m_end;
};

} // namespace introterminal_private

} // namespace adaptors

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_ADAPTORS_PRIVATE_INTROTERMINAL_IMPL_BASE_HPP

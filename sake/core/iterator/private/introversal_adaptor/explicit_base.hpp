/*******************************************************************************
 * sake/core/iterator/private/introversal_adaptor/explicit_base.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_PRIVATE_INTROVERSAL_ADAPTOR_EXPLICIT_BASE_HPP
#define SAKE_CORE_ITERATOR_PRIVATE_INTROVERSAL_ADAPTOR_EXPLICIT_BASE_HPP

#include <boost/type_traits/is_base_of.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/core/iterator/at.hpp>
#include <sake/core/iterator/at_ip.hpp>
#include <sake/core/iterator/begin.hpp>
#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/iterator/core_access.hpp>
#include <sake/core/iterator/end.hpp>
#include <sake/core/iterator/private/adaptor/assign_helper.hpp>
#include <sake/core/iterator/private/adaptor/construct_helper.hpp>
#include <sake/core/iterator/private/adaptor/at_helper.hpp>
#include <sake/core/iterator/private/introversal_adaptor/traits.hpp>
#include <sake/core/memberwise/default_constructor.hpp>
#include <sake/core/range/traits.hpp>
#include <sake/core/range/traits_fwd.hpp>
#include <sake/core/utility/int_tag.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake
{

namespace iterator
{

namespace introversal_adaptor_private
{

template<
    class I,
    bool ExplicitBegin, bool ExplicitEnd,
    class IntroversalMask
>
class explicit_base;

template< class I, class IntroversalMask >
class explicit_base< I, false, false, IntroversalMask >
    : public introversal_adaptor_private::traits<
          I, false, false, IntroversalMask >::adaptor_
{
    typedef introversal_adaptor_private::traits<
        I, false, false, IntroversalMask > traits_;
    SAKE_USING_TYPEDEF( typename traits_, adaptor_ );
    SAKE_USING_TYPEDEF( typename traits_, base_introversal );
public:
    using adaptor_::base;
protected:
    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename explicit_base,
        (( adaptor_ ))
    )

    template< class R >
    explicit_base(R& r, sake::begin_tag)
        : adaptor_(sake::range_traits<R>::iter_at(
              r, sake::_begin, base_introversal()))
    { }
    template< class R >
    explicit_base(R& r, sake::end_tag)
        : adaptor_(sake::range_traits<R>::iter_at(
              r, sake::_end, base_introversal()))
    { }
    template< class R, class J >
    explicit_base(R& r, J const & j)
        : adaptor_(sake::range_traits<R>::iter_at(r, j, base_introversal()))
    { }

    template< class J >
    explicit explicit_base(J const & j,
        typename boost::disable_if_c< boost::is_base_of<
            explicit_base, J >::value >::type* = 0)
        : adaptor_(sake::iterator::adaptor_private::construct_helper<I>(j))
    { }

    template< class J >
    explicit_base(J const & j, sake::begin_tag, sake::end_tag)
        : adaptor_(j)
    { }

    template< class J >
    typename boost::disable_if_c<
        boost::is_base_of< explicit_base, J >::value,
        explicit_base&
    >::type
    operator=(J const & j)
    {
        protected_base() = sake::iterator::adaptor_private::assign_helper<I>(j);
        return *this;
    }

    using adaptor_::protected_base;

    friend class sake::iterator::core_access;

    template< class J, class Introversal >
    typename adaptor_::template relax< Introversal >::type
    derived_at(J const & j, Introversal) const
    {
        static unsigned int const result_base_value =
            Introversal::value & IntroversalMask::value;
        return sake::iterator::at(
            base(), sake::iterator::adaptor_private::at_helper<I>(j),
            sake::introversal_tag_c< result_base_value >()
        );
    }

    template< class Introversal >
    typename adaptor_::template relax< Introversal >::type
    derived_at_begin(Introversal) const
    { return derived_iter_at(sake::_begin, Introversal()); }

    template< class Introversal >
    typename adaptor_::template relax< Introversal >::type
    derived_at_end(Introversal) const
    { return derived_iter_at(sake::_end, Introversal()); }
};

template< class I, class IntroversalMask >
class explicit_base< I, true, false, IntroversalMask >
    : public introversal_adaptor_private::traits<
          I, true, false, IntroversalMask >::adaptor_
{
    typedef introversal_adaptor_private::traits<
        I, true, false, IntroversalMask > traits_;
    SAKE_USING_TYPEDEF( typename traits_, adaptor_ );
    SAKE_USING_TYPEDEF( typename traits_, base_introversal );
    SAKE_USING_TYPEDEF( typename traits_, null_base_type );
public:
    SAKE_USING_TYPEDEF( typename adaptor_, difference_type );
    using adaptor_::base;
protected:
    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename explicit_base,
        (( adaptor_ ))
        (( null_base_type )( m_begin ))
    )

    template< class R >
    explicit_base(R& r, sake::begin_tag)
        : adaptor_(sake::range_traits<R>::iter_at(
              r, sake::_begin, base_introversal())),
          m_begin(adaptor_::base())
    { }
    template< class R >
    explicit_base(R& r, sake::end_tag)
        : adaptor_(sake::range_traits<R>::iter_at(
              r, sake::_begin, base_introversal())),
          m_begin(sake::range_traits<R>::iter_at(r, sake::_begin))
    { }
    template< class R, class J >
    explicit_base(R& r, J const & j)
        : adaptor_(sake::range_traits<R>::iter_at(r, j, base_introversal())),
          m_begin(sake::range_traits<R>::iter_at(r, sake::_begin))
    { }

    template< class J >
    explicit explicit_base(J const & j,
        typename boost::disable_if_c< boost::is_base_of<
            explicit_base, J >::value >::type* = 0)
        : adaptor_(sake::iterator::adaptor_private::construct_helper<I>(j)),
          m_begin(sake::iterator::adaptor_private::
              construct_helper< null_base_type >(sake::iterator::begin(j)))
    { }

    template< class J, class Begin >
    explicit_base(J const & j, Begin const & begin_, sake::end_tag)
        : adaptor_(j),
          m_begin(begin_)
    { }

    template< class J >
    typename boost::disable_if_c<
        boost::is_base_of< explicit_base, J >::value,
        explicit_base&
    >::type
    operator=(J const & j)
    {
        protected_base() = sake::iterator::adaptor_private::assign_helper<I>(j);
        m_begin = sake::iterator::adaptor_private::
            assign_helper< null_base_type >(sake::iterator::begin(j));
        return *this;
    }

    using adaptor_::protected_base;

private:
    null_base_type m_begin;
protected:

    friend class sake::iterator::core_access;

    bool derived_equal_begin() const
    { return base() == m_begin; }

    difference_type derived_difference_begin() const
    { return base() - m_begin; }

    void derived_at_begin_ip()
    { sake::iterator::at_ip(protected_base(), m_begin); }

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
            base(), sake::iterator::adaptor_private::at_helper<I>(j),
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
                base(), sake::iterator::adaptor_private::at_helper<I>(j),
                sake::introversal_tag_c< result_base_value >()
            ),
            m_begin, sake::_end
        );
    }

    template< class Introversal >
    typename adaptor_::template relax< Introversal >::type
    derived_at_begin(Introversal) const
    { return derived_at(m_begin, Introversal()); }

    template< class Introversal >
    typename adaptor_::template relax< Introversal >::type
    derived_at_end(Introversal) const
    { return derived_at(sake::_end, Introversal()); }
};

template< class I, class IntroversalMask >
class explicit_base< I, false, true, IntroversalMask >
    : public introversal_adaptor_private::traits<
          I, false, true, IntroversalMask >::adaptor_
{
    typedef introversal_adaptor_private::traits<
        I, false, true, IntroversalMask > traits_;
    SAKE_USING_TYPEDEF( typename traits_, adaptor_ );
    SAKE_USING_TYPEDEF( typename traits_, base_introversal );
    SAKE_USING_TYPEDEF( typename traits_, null_base_type );
public:
    SAKE_USING_TYPEDEF( typename adaptor_, difference_type );
    using adaptor_::base;
protected:
    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename explicit_base,
        (( adaptor_ ))
        (( null_base_type )( m_end ))
    )

    template< class R >
    explicit_base(R& r, sake::begin_tag)
        : adaptor_(sake::range_traits<R>::iter_at(
              r, sake::_begin, base_introversal())),
          m_end(sake::range_traits<R>::iter_at(r, sake::_end))
    { }
    template< class R >
    explicit_base(R& r, sake::end_tag)
        : adaptor_(sake::range_traits<R>::iter_at(
              r, sake::_end, base_introversal())),
          m_end(adaptor_::base())
    { }
    template< class R, class J >
    explicit_base(R& r, J const & j)
        : adaptor_(sake::range_traits<R>::iter_at(r, j, base_introversal())),
          m_end(sake::range_traits<R>::iter_at(r, sake::_end))
    { }

    template< class J >
    explicit explicit_base(J const & j,
        typename boost::disable_if_c< boost::is_base_of<
            explicit_base, J >::value >::type* = 0)
        : adaptor_(sake::iterator::adaptor_private::construct_helper<I>(j)),
          m_end(sake::iterator::adaptor_private::
              construct_helper< null_base_type >(sake::iterator::end(j)))
    { }

    template< class J, class End >
    explicit_base(J const & j, sake::begin_tag, End const & end_)
        : adaptor_(j),
          m_end(end_)
    { }

    template< class J >
    typename boost::disable_if_c<
        boost::is_base_of< explicit_base, J >::value,
        explicit_base&
    >::type
    operator=(J const & j)
    {
        protected_base() = sake::iterator::adaptor_private::assign_helper<I>(j);
        m_end = sake::iterator::adaptor_private::
            assign_helper< null_base_type >(sake::iterator::end(j));
        return *this;
    }

    using adaptor_::protected_base;

private:
    null_base_type m_end;
protected:

    friend class sake::iterator::core_access;

    bool derived_equal_end() const
    { return base() == m_end; }

    difference_type derived_difference_end() const
    { return base() - m_end; }

    void derived_at_end_ip()
    { sake::iterator::at_ip(protected_base(), m_end); }

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
            base(), sake::iterator::adaptor_private::at_helper<I>(j),
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
                base(), sake::iterator::adaptor_private::at_helper<I>(j),
                sake::introversal_tag_c< result_base_value >()
            ),
            sake::_begin, m_end
        );
    }

    template< class Introversal >
    typename adaptor_::template relax< Introversal >::type
    derived_at_begin(Introversal) const
    { return derived_at(sake::_begin, Introversal()); }

    template< class Introversal >
    typename adaptor_::template relax< Introversal >::type
    derived_at_end(Introversal) const
    { return derived_at(m_end, Introversal()); }
};

template< class I, class IntroversalMask >
class explicit_base< I, true, true, IntroversalMask >
    : public introversal_adaptor_private::traits<
          I, true, true, IntroversalMask >::adaptor_
{
    typedef introversal_adaptor_private::traits<
        I, true, true, IntroversalMask > traits_;
    SAKE_USING_TYPEDEF( typename traits_, adaptor_ );
    SAKE_USING_TYPEDEF( typename traits_, base_introversal );
    SAKE_USING_TYPEDEF( typename traits_, null_base_type );
public:
    SAKE_USING_TYPEDEF( typename adaptor_, difference_type );
    using adaptor_::base;
protected:
    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename explicit_base,
        (( adaptor_ ))
        (( null_base_type )( m_begin ))
        (( null_base_type )( m_end ))
    )

    template< class R >
    explicit_base(R& r, sake::begin_tag)
        : adaptor_(sake::range_traits<R>::iter_at(
              r, sake::_begin, base_introversal())),
          m_begin(adaptor_::base()),
          m_end(sake::range_traits<R>::iter_at(r, sake::_end))
    { }
    template< class R >
    explicit_base(R& r, sake::end_tag)
        : adaptor_(sake::range_traits<R>::iter_at(
              r, sake::_end, base_introversal())),
          m_begin(sake::range_traits<R>::iter_at(r, sake::_begin)),
          m_end(adaptor_::base())
    { }
    template< class R, class J >
    explicit_base(R& r, J const & j)
        : adaptor_(sake::range_traits<R>::iter_at(r, j, base_introversal())),
          m_begin(sake::range_traits<R>::iter_at(r, sake::_begin)),
          m_end(sake::range_traits<R>::iter_at(r, sake::_end))
    { }

    template< class J >
    explicit explicit_base(J const & j,
        typename boost::disable_if_c< boost::is_base_of<
            explicit_base, J >::value >::type* = 0)
        : adaptor_(sake::iterator::adaptor_private::construct_helper<I>(j)),
          m_begin(sake::iterator::adaptor_private::
              construct_helper< null_base_type >(sake::iterator::begin(j))),
          m_end(sake::iterator::adaptor_private::
              construct_helper< null_base_type >(sake::iterator::end(j)))
    { }

    template< class J, class Begin, class End >
    explicit_base(J const & j, Begin const & begin_, End const & end_)
        : adaptor_(j),
          m_begin(begin_),
          m_end(end_)
    { }

    template< class J >
    typename boost::disable_if_c<
        boost::is_base_of< explicit_base, J >::value,
        explicit_base&
    >::type
    operator=(J const & j)
    {
        protected_base() = sake::iterator::adaptor_private::assign_helper<I>(j);
        m_begin = sake::iterator::adaptor_private::
            assign_helper< null_base_type >(sake::iterator::begin(j));
        m_end = sake::iterator::adaptor_private::
            assign_helper< null_base_type >(sake::iterator::end(j));
        return *this;
    }

    using adaptor_::protected_base;

private:
    null_base_type m_begin;
    null_base_type m_end;
protected:

    friend class sake::iterator::core_access;

    bool derived_equal_begin() const
    { return base() == m_begin; }
    bool derived_equal_end() const
    { return base() == m_end; }

    difference_type derived_difference_begin() const
    { return base() - m_begin; }
    difference_type derived_difference_end() const
    { return base() - m_end; }

    void derived_at_begin_ip()
    { sake::iterator::at_ip(protected_base(), m_begin); }
    void derived_at_end_ip()
    { sake::iterator::at_ip(protected_base(), m_end); }

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
            base(), sake::iterator::adaptor_private::at_helper<I>(j),
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
                base(), sake::iterator::adaptor_private::at_helper<I>(j),
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
                base(), sake::iterator::adaptor_private::at_helper<I>(j),
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
                base(), sake::iterator::adaptor_private::at_helper<I>(j),
                sake::null_introversal_tag()
            ),
            m_begin, m_end
        );
    }

    template< class Introversal >
    typename adaptor_::template relax< Introversal >::type
    derived_at_begin(Introversal) const
    { return derived_at(m_begin, Introversal()); }

    template< class Introversal >
    typename adaptor_::template relax< Introversal >::type
    derived_at_end(Introversal) const
    { return derived_at(m_end, Introversal()); }
};

} // namespace introversal_adaptor_private

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_PRIVATE_INTROVERSAL_ADAPTOR_EXPLICIT_BASE_HPP

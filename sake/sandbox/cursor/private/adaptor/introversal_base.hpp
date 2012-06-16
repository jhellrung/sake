/*******************************************************************************
 * sake/core/cursor/private/adaptor/introversal_base.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_CURSOR_PRIVATE_ADAPTOR_INTROVERSAL_BASE_HPP
#define SAKE_CORE_CURSOR_PRIVATE_ADAPTOR_INTROVERSAL_BASE_HPP

#include <boost/mpl/map/map10.hpp>

#include <sake/core/cursor/adaptor_fwd.hpp>
#include <sake/core/cursor/categories.hpp>
#include <sake/core/cursor/traits.hpp>
#include <sake/core/iterator/adaptor.hpp>
#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/iterator/core_access.hpp>
#include <sake/core/iterator/keyword.hpp>
#include <sake/core/memberwise/default_constructor.hpp>
#include <sake/core/range/traits.hpp>
#include <sake/core/range/traits_fwd.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake
{

namespace cursor_adaptor_private
{

template< class I, class Introversal, class IntroversalMask >
struct traits
{
    typedef sake::cursor_adaptor<
        I, Introversal, IntroversalMask
    > cursor_adaptor_;
    typedef typename sake::cursor_introversal<I>::type base_introversal;
    static unsigned int const masked_base_introversal_value =
        base_introversal::value & IntroversalMask::value;
    static bool const need_begin =
        sake::begin_access_introversal_tag::value
      & Introversal::value & ~masked_base_introversal_value::value;
    static bool const need_end =
        sake::end_access_introversal_tag::value
      & Introversal::value & ~masked_base_introversal_value::value;
    static int const introversal_base_index = need_begin + 2 * need_end;
    static unsigned int const introversal_value =
        masked_base_introversal_value
      | (need_begin ? sake::begin_access_introversal_tag::value)
      | (need_end ? sake::end_access_introversal_tag::value);
    typedef sake::cursor_introversal_tag_c< introversal_value > introversal;
    typedef sake::iterator_adaptor<
        cursor_adaptor_, I,
        boost::mpl::map1<
            sake::iterator_keyword::introversal< introversal >
        >
    > iterator_adaptor_;
};

template<
    class I, class Introversal, class IntroversalMask,
    int = cursor_adaptor_private::traits<
        I, Introversal, IntroversalMask >::introversal_base_index
>
class introversal_base;

template< class I, class Introversal, class IntroversalMask >
class introversal_base< I, Introversal, IntroversalMask, 0 >
    : public cursor_adaptor_private::traits<
          I, Introversal, IntroversalMask >::iterator_adaptor_
{
    typedef cursor_adaptor_private::traits<
        I, Introversal, IntroversalMask
    > traits_;
    SAKE_USING_TYPEDEF( typename traits_, iterator_adaptor_ );
    SAKE_USING_TYPEDEF( typename traits_, base_introversal );
protected:
    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename introversal_base,
        (( iterator_adaptor_ ))
    )

    template< class R >
    introversal_base(R& r, sake::begin_tag)
        : iterator_adaptor_(
              sake::range_traits<R>::template begin< base_introversal >(r))
    { }
    template< class R >
    introversal_base(R& r, sake::end_tag)
        : iterator_adaptor_(
              sake::range_traits<R>::template end< base_introversal >(r))
    { }
    template< class R >
    introversal_base(R& r, I const & i)
        : iterator_adaptor_(i)
    { }
};

template< class I, class Introversal, class IntroversalMask >
class introversal_base< I, Introversal, IntroversalMask, 1 >
    : public cursor_adaptor_private::traits<
          I, Introversal, IntroversalMask >::iterator_adaptor_
{
    typedef cursor_adaptor_private::traits<
        I, Introversal, IntroversalMask
    > traits_;
    SAKE_USING_TYPEDEF( typename traits_, iterator_adaptor_ );
public:
    SAKE_USING_TYPEDEF( typename iterator_adaptor_, difference_type );

    using iterator_adaptor_::base;

protected:
    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename introversal_base,
        (( iterator_adaptor_ )) (( I )( m_begin ))
    )

    template< class R >
    introversal_base(R& r, sake::begin_tag)
        : iterator_adaptor_(
              sake::range_traits<R>::template begin< base_introversal >(r)),
          m_begin(protected_base())
    { }
    template< class R >
    introversal_base(R& r, sake::end_tag)
        : iterator_adaptor_(
              sake::range_traits<R>::template end< base_introversal >(r)),
          m_begin(
              sake::range_traits<R>::template begin< base_introversal >(r))
    { }
    template< class R >
    introversal_base(R& r, I const & i)
        : iterator_adaptor_(i),
          m_begin(
              sake::range_traits<R>::template begin< base_introversal >(r))
    { }

    using iterator_adaptor_::protected_base;

private:
    I m_begin;

protected:
    friend class sake::iterator_core_access;

    bool derived_at_begin() const
    { return base() == m_begin; }

    cursor_adaptor_ derived_begin() const
    { return cursor_adaptor_(m_begin, m_begin); } // ???

    difference_type derived_difference_with_begin() const
    { return base() - m_begin; }
};

template< class I, class Introversal, class IntroversalMask >
class introversal_base< I, Introversal, IntroversalMask, 2 >
    : public cursor_adaptor_private::traits<
          I, Introversal, IntroversalMask >::iterator_adaptor_
{
    typedef cursor_adaptor_private::traits<
        I, Introversal, IntroversalMask
    > traits_;
    SAKE_USING_TYPEDEF( typename traits_, iterator_adaptor_ );
public:
    SAKE_USING_TYPEDEF( typename iterator_adaptor_, difference_type );

    using iterator_adaptor_::base;

protected:
    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename introversal_base,
        (( iterator_adaptor_ )) (( I )( m_end ))
    )

    template< class R >
    introversal_base(R& r, sake::begin_tag)
        : iterator_adaptor_(
              sake::range_traits<R>::template begin< base_introversal >(r))
    { }
    template< class R >
    introversal_base(R& r, sake::end_tag)
        : iterator_adaptor_(
              sake::range_traits<R>::template end< base_introversal >(r))
    { }
    template< class R >
    introversal_base(R& r, I const & i)
        : iterator_adaptor_(i)
    { }

    using iterator_adaptor_::protected_base;

private:
    I m_end;

protected:
    friend class sake::iterator_core_access;

    bool derived_at_end() const
    { return base() == m_end; }

    cursor_adaptor_ derived_end() const
    { return cursor_adaptor_(m_end, m_end); } // ???

    difference_type derived_difference_with_end() const
    { return base() - m_end; }
};

template< class I, class Introversal, class IntroversalMask >
class introversal_base< I, Introversal, IntroversalMask, 3 >
    : public cursor_adaptor_private::traits<
          I, Introversal, IntroversalMask >::iterator_adaptor_
{
    typedef cursor_adaptor_private::traits<
        I, Introversal, IntroversalMask
    > traits_;
    SAKE_USING_TYPEDEF( typename traits_, iterator_adaptor_ );
public:
    SAKE_USING_TYPEDEF( typename iterator_adaptor_, difference_type );

    using iterator_adaptor_::base;

protected:
    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename introversal_base,
        (( iterator_adaptor_ )) (( I )( m_begin )) (( I )( m_end ))
    )

    using iterator_adaptor_::protected_base;

private:
    I m_begin;
    I m_end;

protected:
    friend class sake::iterator_core_access;

    bool derived_at_begin() const
    { return base() == m_begin; }

    cursor_adaptor_ derived_begin() const
    { return cursor_adaptor_(m_begin, m_begin); } // ???

    bool derived_at_end() const
    { return base() == m_end; }

    cursor_adaptor_ derived_end() const
    { return cursor_adaptor_(m_end, m_end); } // ???

    difference_type derived_difference_with_begin() const
    { return base() - m_begin; }

    difference_type derived_difference_with_end() const
    { return base() - m_end; }
};

} // namespace cursor_adaptor_private

} // namespace sake

#endif // #ifndef SAKE_CORE_CURSOR_PRIVATE_ADAPTOR_INTROVERSAL_BASE_HPP

/*******************************************************************************
 * sake/core/iterator/introversal_adaptor.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_INTROVERSAL_ADAPTOR_HPP
#define SAKE_CORE_ITERATOR_INTROVERSAL_ADAPTOR_HPP

#include <boost/concept/assert.hpp>
#include <boost/config.hpp>

#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/iterator/concepts/Iterator.hpp>
#include <sake/core/iterator/core_access.hpp>
#include <sake/core/iterator/introversal_adaptor_fwd.hpp>
#include <sake/core/iterator/introversal_adaptor_relax.hpp>
#include <sake/core/iterator/private/introversal_adaptor/assign_enable.hpp>
#include <sake/core/iterator/private/introversal_adaptor/implicit_constructor_enable.hpp>
#include <sake/core/iterator/private/introversal_adaptor/explicit_base.hpp>
#include <sake/core/memberwise/default_constructor.hpp>
#include <sake/core/move/as_lvalue.hpp>

namespace sake
{

namespace iterator
{

template<
    class I,
    bool ExplicitBegin, bool ExplicitEnd,
    class IntroversalMask /*= sake::null_introversal_tag*/
>
class introversal_adaptor
    : public introversal_adaptor_private::explicit_base<
          I, ExplicitBegin, ExplicitEnd, IntroversalMask >
{
    BOOST_CONCEPT_ASSERT((sake::concepts::Iterator<I>));
    typedef introversal_adaptor_private::explicit_base<
        I, ExplicitBegin, ExplicitEnd, IntroversalMask > explicit_base_;
public:
    
    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename introversal_adaptor,
        (( explicit_base_ ))
    )

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class R >
    introversal_adaptor(R&& r, sake::begin_tag)
        : explicit_base_(r, sake::_begin)
    { }
    template< class R >
    introversal_adaptor(R&& r, sake::end_tag)
        : explicit_base_(r, sake::_end)
    { }
    template< class R, class J >
    introversal_adaptor(R&& r, J const & j)
        : explicit_base_(r,j)
    { }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class R >
    introversal_adaptor(R& r, sake::begin_tag)
        : explicit_base_(SAKE_AS_LVALUE(r), sake::_begin)
    { }
    template< class R >
    introversal_adaptor(R& r, sake::end_tag)
        : explicit_base_(SAKE_AS_LVALUE(r), sake::_end)
    { }
    template< class R, class J >
    introversal_adaptor(R& r, J const & j)
        : explicit_base_(SAKE_AS_LVALUE(r), j)
    { }

    template< class R >
    introversal_adaptor(R const & r, sake::begin_tag)
        : explicit_base_(r, sake::_begin)
    { }
    template< class R >
    introversal_adaptor(R const & r, sake::end_tag)
        : explicit_base_(r, sake::_end)
    { }
    template< class R, class J >
    introversal_adaptor(R const & r, J const & j)
        : explicit_base_(r,j)
    { }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

private:
    template< class J >
    struct implicit_constructor_enabler
        : introversal_adaptor_private::
              implicit_constructor_enabler< introversal_adaptor, J >
    { };
public:
    template< class J >
    introversal_adaptor(J const & j,
        typename implicit_constructor_enabler<J>::type* = 0)
        : explicit_base_(j)
    { }

    template< class J, class Begin, class End >
    introversal_adaptor(J const & j, Begin const & begin_, End const & end_)
        : explicit_base_(j, begin_, end_)
    { }

private:
    template< class J >
    struct assign_enabler
        : introversal_adaptor_private::
              assign_enabler< introversal_adaptor, J >
    { };
public:
    template< class J >
    typename assign_enabler<J>::type
    operator=(J const & j)
    { return explicit_base_::operator=(j).derived(); }

    operator I const & () const
    { return explicit_base_::base(); }

private:
    using explicit_base_::protected_base;

    friend class sake::iterator::core_access;

    template< class Introversal >
    struct derived_relax
    {
        typedef typename sake::iterator::introversal_adaptor_relax<
            I, Introversal, IntroversalMask >::type type;
    };
};

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_INTROVERSAL_ADAPTOR_HPP

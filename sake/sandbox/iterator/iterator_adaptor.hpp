/*******************************************************************************
 * sake/core/iterator/iterator_adaptor.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct iterator_adaptor<
 *     Derived,
 *     Base,
 *     Value       = sake::default_tag,
 *     Reference   = sake::default_tag,
 *     Difference  = sake::default_tag,
 *     Traversal   = sake::default_tag,
 *     Introversal = sake::default_tag,
 *     Member      = sake::void_,
 *     ChainedBase = sake::void_
 * >
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_ITERATOR_ADAPTOR_HPP
#define SAKE_CORE_ITERATOR_ITERATOR_ADAPTOR_HPP

// #include <boost/iterator/iterator_categories.hpp>
// #include <boost/iterator/iterator_traits.hpp>
// #include <boost/mpl/and.hpp>
// #include <boost/mpl/eval_if.hpp>
// #include <boost/mpl/identity.hpp>
// #include <boost/mpl/if.hpp>
// #include <boost/mpl/not.hpp>
// #include <boost/mpl/or.hpp>
// #include <boost/preprocessor/seq/seq.hpp>
// #include <boost/preprocessor/tuple/elem.hpp>
// #include <boost/type_traits/is_base_of.hpp>
// #include <boost/utility/enable_if.hpp>

// #include <sake/boost_ext/move/forward.hpp>
// #include <sake/boost_ext/type_traits/add_const_if.hpp>
// #include <sake/boost_ext/type_traits/add_reference.hpp>
// #include <sake/boost_ext/type_traits/add_reference_add_const.hpp>
// #include <sake/boost_ext/type_traits/is_convertible.hpp>
// #include <sake/boost_ext/type_traits/is_convertible_wndp2bp.hpp>
// #include <sake/boost_ext/type_traits/is_convertible_wnrbt.hpp>
// #include <sake/boost_ext/type_traits/remove_qualifiers.hpp>
// #include <sake/core/cursor/at_begin.hpp>
// #include <sake/core/cursor/at_end.hpp>
// #include <sake/core/cursor/begin_distance.hpp>
// #include <sake/core/cursor/begin_end_distance.hpp>
// #include <sake/core/cursor/end_distance.hpp>
// #include <sake/core/cursor/introversal.hpp>
// #include <sake/core/cursor/to_begin.hpp>
// #include <sake/core/cursor/to_end.hpp>
// #include <sake/core/iterator/is_const_iterator.hpp>
// #include <sake/core/iterator/iterator_core_access.hpp>
// #include <sake/core/iterator/iterator_facade.hpp>
#include <sake/core/iterator/private/adaptor_traits.hpp>
// #include <sake/core/math/compare.hpp>
// #include <sake/core/math/sign_t.hpp>
// #include <sake/core/utility/assignable.hpp>
// #include <sake/core/utility/autogen_default_ctor.hpp>
// #include <sake/core/utility/call_traits.hpp>
// #include <sake/core/utility/compressed_pair.hpp>
#include <sake/core/utility/default_tag.hpp>
// #include <sake/core/utility/default_constructible.hpp>
// #include <sake/core/utility/emplacer.hpp>
// #include <sake/core/utility/overload.hpp>
#include <sake/core/utility/using_typename.hpp>
#include <sake/core/utility/void.hpp>
// #include <sake/core/utility/workaround.hpp>

namespace sake
{

template<
    class Derived,
    class IteratorBase,
    class Value       = sake::default_tag,
    class Reference   = sake::default_tag,
    class Difference  = sake::default_tag,
    class Traversal   = sake::default_tag,
    class Introversal = sake::default_tag,
    class Member      = sake::void_,
    class ChainedBase = sake::void_
>
struct iterator_adaptor
    : iterator_adaptor_private::traits<
          Derived, IteratorBase,
          Value, Reference, Difference,
          Traversal, Introversal,
          ChainedBase
      >::iterator_facade_
{
private:
    typedef iterator_adaptor_private::traits<
        Derived, IteratorBase,
        Value, Reference, Difference,
        Traversal, Introversal,
        ChainedBase
    > traits_;
    SAKE_USING_TYPENAME( typename traits_, iterator_facade_ );
public:
    SAKE_USING_TYPENAME( typename iterator_facade_, value_type );
    SAKE_USING_TYPENAME( typename iterator_facade_, reference );
    SAKE_USING_TYPENAME( typename iterator_facade_, difference_type );

    typedef IteratorBase base_type;
    base_type const & base() const
    { return m_base_and_member.first(); }

    SAKE_ITERATOR_ENABLE_CURSOR( iterator_facade_ )

protected:
    typedef Member member_type;
    typedef ChainedBase chained_base_type;
private:
    typedef typename sake::call_traits< base_type >::param_type base_param_type;
    typedef typename sake::call_traits< Member >::reference       member_reference;
    typedef typename sake::call_traits< Member >::reference_const member_reference_const;
protected:

    SAKE_AUTOGEN_DEFAULT_CTOR( iterator_adaptor )

#if SAKE_WORKAROUND_MSVC_VERSION_LESS_EQUAL( 1500 )

    // MSVC9 has a tendency to ICE unless this constructor is explicitly separated.
    explicit iterator_adaptor(base_param_type it)
        : m_base_and_member(it, emplacer0<>())
    { }

    template< class BaseConstruct >
    explicit iterator_adaptor(const BaseConstruct& base_construct,
        typename boost::enable_if< boost::mpl::and_<
            boost::mpl::not_< boost::is_base_of< iterator_adaptor, BaseConstruct > >,
            boost::mpl::not_< boost_ext::is_convertible< const BaseConstruct&, base_type > >,
            is_maybe_typed_emplacer< BaseConstruct >
        > >::type* = 0)
        : m_base_and_member(base_construct, emplacer0<>())
    { }

#else // #if SAKE_WORKAROUND_MSVC_VERSION_LESS_EQUAL( 1500 )

    template< class BaseConstruct >
    explicit iterator_adaptor(const BaseConstruct& base_construct,
        typename boost::enable_if< boost::mpl::and_<
            boost::mpl::not_< boost::is_base_of< iterator_adaptor, BaseConstruct > >,
            boost::mpl::or_<
                boost_ext::is_convertible_wndp2bp< const BaseConstruct&, base_type >,
                is_maybe_typed_emplacer< BaseConstruct >
            >
        > >::type* = 0)
        : m_base_and_member(base_construct, emplacer0<>())
    { }

#endif // #if SAKE_WORKAROUND_MSVC_VERSION_LESS_EQUAL( 1500 )

    // template< class BaseConstruct, class MemberConstruct [, class ChainedBaseConstruct] >
    // iterator_adaptor(
    //     const BaseConstruct& base_construct,
    //     MemberConstruct&& member_construct
    //     [, const ChainedBaseConstruct& chained_base_construct]);
#define SAKE_OVERLOAD_PARAM_SPEC_SEQ \
    (( cref_tag, BaseConstruct, base_construct )) \
    (( perfect_tag, BOOST_PP_SEQ_NIL, member_construct ))
#define SAKE_OVERLOAD_CTOR_NAME iterator_adaptor
#define SAKE_OVERLOAD_CTOR_INIT_LIST( n, T_tuple, x_tuple, forward_x_tuple ) \
    m_base_and_member(base_construct, typed_emplacer_wrap< member_type >(member_construct))
#include SAKE_OVERLOAD_GENERATE()
#define SAKE_OVERLOAD_PARAM_SPEC_SEQ \
    (( cref_tag, BaseConstruct, base_construct )) \
    (( perfect_tag, BOOST_PP_SEQ_NIL, member_construct )) \
    (( cref_tag, ChainedBaseConstruct, chained_base_construct ))
#define SAKE_OVERLOAD_CTOR_NAME iterator_adaptor
#define SAKE_OVERLOAD_CTOR_INIT_LIST( n, T_tuple, x_tuple, forward_x_tuple ) \
    iterator_facade_(chained_base_construct), \
    m_base_and_member(base_construct, typed_emplacer_wrap< member_type >(member_construct))
#include SAKE_OVERLOAD_GENERATE()

    template<class,class,class,class,class,class,class,class,class> friend class iterator_adaptor;
    template< class D, class B, class V, class T, class I, class R, class F, class M, class C >
    iterator_adaptor(iterator_adaptor<D,B,V,T,I,R,F,M,C>& other,
        typename boost::enable_if< boost::mpl::and_<
            boost_ext::is_convertible_wndp2bp< typename iterator_adaptor<D,B,V,T,I,R,F,M,C>::base_type &, base_type >,
            boost_ext::is_convertible_wnrbt<
                typename iterator_adaptor<D,B,V,T,I,R,F,M,C>::member_reference,
                member_type
            >,
            boost_ext::is_convertible< typename iterator_adaptor<D,B,V,T,I,R,F,M,C>::chained_base_type &, chained_base_type >
        > >::type* = 0)
        : iterator_facade_(static_cast< typename iterator_adaptor<D,B,V,T,I,R,F,M,C>::iterator_facade_ & >(other)),
          m_base_and_member(other.m_base_and_member)
    { }
    template< class D, class B, class V, class T, class I, class R, class F, class M, class C >
    iterator_adaptor(const iterator_adaptor<D,B,V,T,I,R,F,M,C>& other,
        typename boost::enable_if< boost::mpl::and_<
            boost_ext::is_convertible_wndp2bp< typename iterator_adaptor<D,B,V,T,I,R,F,M,C>::base_type const &, base_type >,
            boost_ext::is_convertible_wnrbt<
                typename iterator_adaptor<D,B,V,T,I,R,F,M,C>::member_reference_const,
                member_type
            >,
            boost_ext::is_convertible< typename iterator_adaptor<D,B,V,T,I,R,F,M,C>::chained_base_type const &, chained_base_type >
        > >::type* = 0)
        : iterator_facade_(static_cast< typename iterator_adaptor<D,B,V,T,I,R,F,M,C>::iterator_facade_ const & >(other)),
          m_base_and_member(other.m_base_and_member)
    { }

    base_type       & base_reference()       { return m_base_and_member.first(); }
    base_type const & base_reference() const { return m_base_and_member.first(); }
    member_reference       member()       { return m_base_and_member.second(); }
    member_reference_const member() const { return m_base_and_member.second(); }

    typedef Derived derived_type;

    friend class iterator_core_access;

    // Readable/Writable
    reference dereference_impl() const { return *base(); }

    // Incrementable
    void increment_impl() { ++base_reference(); }

    // SinglePass
    template< class T >
    bool equal_to_impl(const T& other) const { return base() == other.base(); }

    // Bidirectional
    void decrement_impl() { --base_reference(); }

    // RandomAccess
private:
    typedef typename sake::call_traits< difference_type >::param_type difference_param_type;
protected:
    void advance_impl(difference_param_type n) { base_reference() += n; }
    template< class T >
    difference_type difference_impl(const T& other) const { return base() - other.base(); }
    template< class T >
    sake::sign_t compare_impl(const T& other) const { return adl::compare(base(), other.base()); }
    template< class T >
    bool less_impl(const T& other) const { return base() < other.base(); }
    template< class T >
    bool less_equal_impl(const T& other) const { return base() <= other.base(); }

    // BeginDetect
    bool at_begin_impl() const { return cursor::at_begin(base()); }
    // BeginAccess
    void to_begin_impl() { cursor::to_begin(base_reference()); }

    // EndDetect
    bool at_end_impl() const { return cursor::at_end(base()); }
    // EndAccess
    void to_end_impl() { cursor::to_end(base_reference()); }

    // Bidirectional + BeginDetect
    difference_type begin_distance_impl() const { return cursor::begin_distance(base()); }
    // Forward + EndDetect
    difference_type end_distance_impl() const { return cursor::end_distance(base()); }
    // Bidirectional + BeginDetect + EndDetect
    difference_type begin_end_distance_impl() const { return cursor::begin_end_distance(base()); }

private:
    typedef typename boost::mpl::eval_if<
        boost_ext::is_convertible<
            typename iterator_adaptor_traits_::traversal_type,
            boost::forward_traversal_tag
        >,
        result_of::as_default_constructible< Member >,
        result_of::as_assignable< Member >
    >::type member_storage_type;
    compressed_pair< base_type, member_storage_type > m_base_and_member;
};

} // namespace sake

#endif // SAKE_CORE_ITERATOR_ITERATOR_ADAPTOR_HPP

/*******************************************************************************
 * sake/core/ref/reference_wrapper.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * class reference_wrapper< T, Tags = ref_tag::default_tags >
 *
 * This is similar to Boost.ReferenceWrapper; see
 *     http://www.boost.org/doc/html/ref.html
 * However, a sake::reference_wrapper has a few advantages over a
 * boost::reference_wrapper:
 * - Tags to disable (default) or enable
 *   - assignment
 *   - default construction
 *   - implicit conversion from T&
 * - A forwarding operator().
 * - A nested result_type typedef iff T has a result_type member type.
 ******************************************************************************/

#ifndef SAKE_CORE_REF_REFERENCE_WRAPPER_HPP
#define SAKE_CORE_REF_REFERENCE_WRAPPER_HPP

#include <boost/config.hpp>
#include <boost/mpl/has_key.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/range/const_iterator.hpp>
#include <boost/range/mutable_iterator.hpp>
#include <boost/ref.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/fusion/adapted/reference_wrapper.hpp>
#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/functional/forwarding/base.hpp>
#include <sake/core/functional/forwarding/deduced_enable.hpp>
#include <sake/core/functional/forwarding/deduced_params.hpp>
#include <sake/core/functional/forwarding/deduced_result.hpp>
#include <sake/core/functional/forwarding/core_access.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/range/begin.hpp>
#include <sake/core/range/end.hpp>
#include <sake/core/range/traits.hpp>
#include <sake/core/ref/fwd.hpp>
#include <sake/core/ref/is_wrapped_parameter.hpp>
#include <sake/core/ref/tag.hpp>
#include <sake/core/utility/address_of.hpp>
#include <sake/core/utility/assert.hpp>
#include <sake/core/utility/noncopy_assignable.hpp>
#include <sake/core/utility/nullptr.hpp>
#include <sake/core/utility/overload.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake
{

namespace reference_wrapper_adl
{

namespace private_
{

struct dummy { };

template< class T, class Tags >
struct traits;

} // namespace private_

/*******************************************************************************
 * class reference_wrapper< T, Tags >
 ******************************************************************************/

template< class T, class Tags /*= ref_tag::default_tags*/ >
class reference_wrapper
    : public private_::traits< T, Tags >::forwarding_base_
{
    BOOST_STATIC_ASSERT((boost::mpl::is_sequence< Tags >::value));
    typedef private_::traits< T, Tags > traits_;
    SAKE_USING_TYPEDEF( typename traits_, forwarding_base_ );
public:
    typedef T type;
    typedef Tags tags;

    typedef boost::mpl::has_key< Tags, sake::ref_tag::default_constructible >
        has_nothrow_default_constructor_tag;
    // Fails unless has_nothrow_default_constructor_tag::value.
    reference_wrapper()
        BOOST_NOEXCEPT_IF((has_nothrow_default_constructor_tag::value));

    //reference_wrapper(reference_wrapper const &);
    //~reference_wrapper();

    typedef boost::true_type has_nothrow_copy_assign_tag;
    //reference_wrapper& operator=(reference_wrapper const &);

private:
    static bool const has_implicitly_convertible_tag =
        boost::mpl::has_key< Tags, sake::ref_tag::implicitly_convertible >::value;
    typedef typename boost::mpl::if_c<
        has_implicitly_convertible_tag,
        private_::dummy,
        T&
    >::type explicit_constructor_param_type;
    typedef typename boost::mpl::if_c<
        has_implicitly_convertible_tag,
        T&,
        private_::dummy
    >::type implicit_constructor_param_type;
public:
    explicit reference_wrapper(explicit_constructor_param_type x);
    reference_wrapper(implicit_constructor_param_type x);

    template< class T2, class Tags2 >
    reference_wrapper(reference_wrapper< T2, Tags2 > other,
        typename boost::enable_if_c< boost_ext::is_convertible< T2*, T* >::value >::type* = 0);
    template< class T2 >
    reference_wrapper(boost::reference_wrapper< T2 > other,
        typename boost::enable_if_c< boost_ext::is_convertible< T2*, T* >::value >::type* = 0);

private:
    static bool const has_assignable_tag = boost_ext::mpl::or2<
        boost::mpl::has_key< Tags, sake::ref_tag::default_constructible >,
        boost::mpl::has_key< Tags, sake::ref_tag::assignable >
    >::value;
    typedef typename boost::mpl::if_c<
        has_assignable_tag,
        T&,
        private_::dummy
    >::type assign_param_type;
public:
    reference_wrapper& operator=(assign_param_type x);

    operator T&() const;
    operator boost::reference_wrapper<T>() const;

    T& get() const;
    T* get_ptr() const;
    T& operator*() const;
    T* operator->() const;

private:
    T* m_p;

    friend class forwarding::core_access;

    template< class Signature > struct derived_enable;
    template< class Signature > struct derived_result;

    SAKE_USING_TYPEDEF(
        typename forwarding_base_, protected_nullary_result_type );

    protected_nullary_result_type derived_apply() const;

#ifndef BOOST_NO_VARIADIC_TEMPLATES

    template< class... T >
    typename derived_result< reference_wrapper ( SAKE_FWD2_PARAM( T )... ) >::type
    derived_apply(SAKE_FWD2_REF( T )... x) const
    { return (*m_p)(sake::forward<T>(x)...); }

#else // #ifndef BOOST_NO_VARIADIC_TEMPLATES

#define SAKE_OVERLOAD_RESULT( r, n, T_tuple ) \
    derived_result< reference_wrapper T_tuple >
#define SAKE_OVERLOAD_FUNCTION_NAME \
    derived_apply
#define SAKE_OVERLOAD_DECLARATION_SUFFIX \
    const
#define SAKE_OVERLOAD_BODY( r, n, T_tuple, x_tuple, forward_x_tuple ) \
    return (*m_p) forward_x_tuple ;
#define SAKE_OVERLOAD_FWD2_MAX_ARITY SAKE_FORWARDING_BASE_MAX_ARITY
#include SAKE_OVERLOAD_GENERATE()

#endif // #ifndef BOOST_NO_VARIADIC_TEMPLATES

};

/*******************************************************************************
 * Allow a reference_wrapper wrapping a range to be viewed as a range itself.
 ******************************************************************************/

template< class T, class Tags >
inline typename sake::range_iterator<T>::type
range_begin(sake::reference_wrapper< T, Tags > x)
{ return sake::range::begin(*x); }

template< class T, class Tags >
inline typename sake::range_iterator<T>::type
range_end(sake::reference_wrapper< T, Tags > x)
{ return sake::range::end(*x); }

template< class T, class Tags >
inline typename sake::range_iterator<T>::type
begin(sake::reference_wrapper< T, Tags > x)
{ return sake::range::begin(*x); }

template< class T, class Tags >
inline typename sake::range_iterator<T>::type
end(sake::reference_wrapper< T, Tags > x)
{ return sake::range::end(*x); }

} // namespace reference_wrapper_adl

/*******************************************************************************
 * struct extension::unwrap_reference< reference_wrapper< T, Tags >, void >
 * struct extension::is_wrapped_parameter< reference_wrapper< T, Tags >, void >
 ******************************************************************************/

namespace extension
{

template< class T, class Tags >
struct unwrap_reference< sake::reference_wrapper< T, Tags >, void >
{ typedef T type; };

template< class T, class Tags >
struct is_wrapped_parameter< sake::reference_wrapper< T, Tags >, void >
    : boost::mpl::has_key< Tags, sake::ref_tag::parameter >
{ };

} // namespace extension

} // namespace sake

/*******************************************************************************
 * struct boost::range_mutable_iterator< reference_wrapper< T, Tags > >
 * struct boost::range_const_iterator< reference_wrapper< T, Tags > >
 ******************************************************************************/

namespace boost
{

template< class T, class Tags >
struct range_mutable_iterator< sake::reference_wrapper< T, Tags > >
    : sake::range_iterator<T>
{ };

template< class T, class Tags >
struct range_const_iterator< sake::reference_wrapper< T, Tags > >
    : sake::range_iterator<T>
{ };

} // namespace boost

namespace sake
{

namespace reference_wrapper_adl
{

namespace private_
{

template< class T, class Tags >
struct traits
{
    typedef sake::forwarding::base<
        sake::reference_wrapper< T, Tags >,
        typename sake::forwarding::deduced_params<
            T&,
            typename boost::mpl::if_c<
                boost_ext::mpl::or2<
                    boost::mpl::has_key< Tags, sake::ref_tag::assignable >,
                    boost::mpl::has_key< Tags, sake::ref_tag::default_constructible >
                >::value,
                void,
                sake::noncopy_assignable_base
            >::type
        >::type
    > forwarding_base_;
};

} // namespace private_

/*******************************************************************************
 * reference_wrapper member definitions
 ******************************************************************************/

template< class T, class Tags >
reference_wrapper< T, Tags >::
reference_wrapper()
    BOOST_NOEXCEPT_IF((has_nothrow_default_constructor_tag::value))
    : m_p(nullptr)
{ BOOST_STATIC_ASSERT((has_nothrow_default_constructor_tag::value)); }

template< class T, class Tags >
reference_wrapper< T, Tags >::
reference_wrapper(explicit_constructor_param_type x)
    : m_p(sake::address_of(x))
{ }

template< class T, class Tags >
reference_wrapper< T, Tags >::
reference_wrapper(implicit_constructor_param_type x)
    : m_p(sake::address_of(x))
{ }

template< class T, class Tags >
template< class T2, class Tags2 >
reference_wrapper< T, Tags >::
reference_wrapper(reference_wrapper< T2, Tags2 > other,
    typename boost::enable_if_c< boost_ext::is_convertible< T2*, T* >::value >::type*)
    : m_p(other.get_ptr())
{ }

template< class T, class Tags >
template< class T2 >
reference_wrapper< T, Tags >::
reference_wrapper(boost::reference_wrapper< T2 > other,
    typename boost::enable_if_c< boost_ext::is_convertible< T2*, T* >::value >::type*)
    : m_p(other.get_pointer())
{ }

template< class T, class Tags >
inline reference_wrapper< T, Tags >&
reference_wrapper< T, Tags >::
operator=(assign_param_type x)
{
    m_p = sake::address_of(x);
    return *this;
}

template< class T, class Tags >
inline
reference_wrapper< T, Tags >::
operator T&() const
{
    SAKE_ASSERT_RELATION( m_p, !=, nullptr );
    return *m_p;
}

template< class T, class Tags >
inline
reference_wrapper< T, Tags >::
operator boost::reference_wrapper<T>() const
{
    SAKE_ASSERT_RELATION( m_p, !=, nullptr );
    return boost::ref(*m_p);
}

template< class T, class Tags >
inline T&
reference_wrapper< T, Tags >::
get() const
{
    SAKE_ASSERT_RELATION( m_p, !=, nullptr );
    return *m_p;
}

template< class T, class Tags >
inline T*
reference_wrapper< T, Tags >::
get_ptr() const
{ return m_p; }

template< class T, class Tags >
inline T&
reference_wrapper< T, Tags >::
operator*() const
{
    SAKE_ASSERT_RELATION( m_p, !=, nullptr );
    return *m_p;
}

template< class T, class Tags >
inline T*
reference_wrapper< T, Tags >::
operator->() const
{
    SAKE_ASSERT_RELATION( m_p, !=, nullptr );
    return m_p;
}

template< class T, class Tags >
template< class Signature >
struct reference_wrapper< T, Tags >::
derived_enable
    : forwarding::deduced_enable< Signature, T& >
{ };

template< class T, class Tags >
template< class Signature >
struct reference_wrapper< T, Tags >::
derived_result
    : forwarding::deduced_result< Signature, T& >
{ };

template< class T, class Tags >
inline typename reference_wrapper< T, Tags >::protected_nullary_result_type
reference_wrapper< T, Tags >::
derived_apply() const
{ return (*m_p)(); }

} // namespace reference_wrapper_adl

} // namespace sake

#endif // #ifndef SAKE_CORE_REF_REFERENCE_WRAPPER_HPP

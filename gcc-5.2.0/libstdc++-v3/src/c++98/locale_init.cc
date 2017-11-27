// Copyright (C) 1997-2015 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

#define _GLIBCXX_USE_CXX11_ABI 1
#include <clocale>
#include <cstring>
#include <cstdlib>     // For getenv, free.
#include <cctype>
#include <cwctype>     // For towupper, etc.
#include <locale>
#include <ext/concurrence.h>

#if _GLIBCXX_USE_DUAL_ABI
// This file is compiled with the new std::string ABI so std::numpunct<char>
// refers to std::__cxx11::numpunct<char>. These declarations let us refer
// to the other facets instantiated with the old ABI.
# define _GLIBCXX_LOC_ID(mangled) extern std::locale::id mangled
_GLIBCXX_LOC_ID(_ZNSt8numpunctIcE2idE);
_GLIBCXX_LOC_ID(_ZNSt8messagesIcE2idE);
# ifdef _GLIBCXX_USE_WCHAR_T
_GLIBCXX_LOC_ID(_ZNSt8numpunctIwE2idE);
_GLIBCXX_LOC_ID(_ZNSt8messagesIwE2idE);
# endif
#endif


namespace 
{
  const int num_facets = _GLIBCXX_NUM_FACETS + _GLIBCXX_NUM_UNICODE_FACETS
    + (_GLIBCXX_USE_DUAL_ABI ? _GLIBCXX_NUM_CXX11_FACETS : 0);

  __gnu_cxx::__mutex&
  get_locale_mutex()
  {
    static __gnu_cxx::__mutex locale_mutex;
    return locale_mutex;
  }

  using namespace std;

  typedef char fake_locale_Impl[sizeof(locale::_Impl)]
  __attribute__ ((aligned(__alignof__(locale::_Impl))));
  fake_locale_Impl c_locale_impl;

  typedef char fake_locale[sizeof(locale)]
  __attribute__ ((aligned(__alignof__(locale))));
  fake_locale c_locale;

  typedef char fake_name_vec[sizeof(char*)]
  __attribute__ ((aligned(__alignof__(char*))));
  fake_name_vec name_vec[6 + _GLIBCXX_NUM_CATEGORIES];

  typedef char fake_names[sizeof(char[2])]
  __attribute__ ((aligned(__alignof__(char[2]))));
  fake_names name_c[6 + _GLIBCXX_NUM_CATEGORIES];

  typedef char fake_facet_vec[sizeof(locale::facet*)]
  __attribute__ ((aligned(__alignof__(locale::facet*))));
  fake_facet_vec facet_vec[num_facets];

  typedef char fake_cache_vec[sizeof(locale::facet*)]
  __attribute__ ((aligned(__alignof__(locale::facet*))));
  fake_cache_vec cache_vec[num_facets];

  typedef char fake_ctype_c[sizeof(std::ctype<char>)]
  __attribute__ ((aligned(__alignof__(std::ctype<char>))));
  fake_ctype_c ctype_c;

  typedef char fake_numpunct_c[sizeof(numpunct<char>)]
  __attribute__ ((aligned(__alignof__(numpunct<char>))));
  fake_numpunct_c numpunct_c;

  typedef char fake_num_get_c[sizeof(num_get<char>)]
  __attribute__ ((aligned(__alignof__(num_get<char>))));
  fake_num_get_c num_get_c;

  typedef char fake_num_put_c[sizeof(num_put<char>)]
  __attribute__ ((aligned(__alignof__(num_put<char>))));
  fake_num_put_c num_put_c;

  typedef char fake_messages_c[sizeof(messages<char>)]
  __attribute__ ((aligned(__alignof__(messages<char>))));
  fake_messages_c messages_c;

#ifdef  _GLIBCXX_USE_WCHAR_T
  typedef char fake_wtype_w[sizeof(std::ctype<wchar_t>)]
  __attribute__ ((aligned(__alignof__(std::ctype<wchar_t>))));
  fake_wtype_w ctype_w;

  typedef char fake_numpunct_w[sizeof(numpunct<wchar_t>)]
  __attribute__ ((aligned(__alignof__(numpunct<wchar_t>))));
  fake_numpunct_w numpunct_w;

  typedef char fake_num_get_w[sizeof(num_get<wchar_t>)]
  __attribute__ ((aligned(__alignof__(num_get<wchar_t>))));
  fake_num_get_w num_get_w;

  typedef char fake_num_put_w[sizeof(num_put<wchar_t>)]
  __attribute__ ((aligned(__alignof__(num_put<wchar_t>))));
  fake_num_put_w num_put_w;

  typedef char fake_messages_w[sizeof(messages<wchar_t>)]
  __attribute__ ((aligned(__alignof__(messages<wchar_t>))));
  fake_messages_w messages_w;
#endif

  // Storage for "C" locale caches.
  typedef char fake_num_cache_c[sizeof(std::__numpunct_cache<char>)]
  __attribute__ ((aligned(__alignof__(std::__numpunct_cache<char>))));
  fake_num_cache_c numpunct_cache_c;

#ifdef _GLIBCXX_USE_WCHAR_T
  typedef char fake_num_cache_w[sizeof(std::__numpunct_cache<wchar_t>)]
  __attribute__ ((aligned(__alignof__(std::__numpunct_cache<wchar_t>))));
  fake_num_cache_w numpunct_cache_w;
#endif
} // anonymous namespace

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  locale::locale() throw() : _M_impl(0)
  { 
    _S_initialize();

    // Checked locking to optimize the common case where _S_global
    // still points to _S_classic (locale::_S_initialize_once()):
    // - If they are the same, just increment the reference count and
    //   we are done.  This effectively constructs a C locale object
    //   identical to the static c_locale.
    // - Otherwise, _S_global can and may be destroyed due to
    //   locale::global() call on another thread, in which case we
    //   fall back to lock protected access to both _S_global and
    //   its reference count.
    _M_impl = _S_global;
    if (_M_impl == _S_classic)
      _M_impl->_M_add_reference();
    else
      {
        __gnu_cxx::__scoped_lock sentry(get_locale_mutex());
        _S_global->_M_add_reference();
        _M_impl = _S_global;
      }
  }

  locale
  locale::global(const locale& __other)
  {
    _S_initialize();
    _Impl* __old;
    {
      __gnu_cxx::__scoped_lock sentry(get_locale_mutex());
      __old = _S_global;
      __other._M_impl->_M_add_reference();
      _S_global = __other._M_impl;
      const string __other_name = __other.name();
      if (__other_name != "*")
	setlocale(LC_ALL, __other_name.c_str());
    }

    // Reference count sanity check: one reference removed for the
    // subsition of __other locale, one added by return-by-value. Net
    // difference: zero. When the returned locale object's destrutor
    // is called, then the reference count is decremented and possibly
    // destroyed.
    return locale(__old);
  }

  const locale&
  locale::classic()
  { 
    _S_initialize();
    return *(new (&c_locale) locale(_S_classic));
  }

  void
  locale::_S_initialize_once() throw()
  {
    // 2 references.
    // One reference for _S_classic, one for _S_global
    _S_classic = new (&c_locale_impl) _Impl(2);
    _S_global = _S_classic; 	    
  }

  void  
  locale::_S_initialize()
  {
#ifdef __GTHREADS
    if (__gthread_active_p())
      __gthread_once(&_S_once, _S_initialize_once);
#endif
    if (!_S_classic)
      _S_initialize_once();
  }

  // Definitions for static const data members of locale::_Impl
  const locale::id* const
  locale::_Impl::_S_id_ctype[] =
  {
    &std::ctype<char>::id, 
#ifdef _GLIBCXX_USE_WCHAR_T
    &std::ctype<wchar_t>::id,
#endif
    0
  };

  const locale::id* const
  locale::_Impl::_S_id_numeric[] =
  {
    &num_get<char>::id,  
    &num_put<char>::id,  
    &numpunct<char>::id, 
#ifdef _GLIBCXX_USE_WCHAR_T
    &num_get<wchar_t>::id,
    &num_put<wchar_t>::id,
    &numpunct<wchar_t>::id,
#endif
    0
  };
  
  const locale::id* const
  locale::_Impl::_S_id_collate[] =
  {
    0
  };

  const locale::id* const
  locale::_Impl::_S_id_time[] =
  {
    0
  };
  
  const locale::id* const
  locale::_Impl::_S_id_monetary[] =
  {
    0
  };

  const locale::id* const
  locale::_Impl::_S_id_messages[] =
  {
    &std::messages<char>::id, 
#ifdef _GLIBCXX_USE_WCHAR_T
    &std::messages<wchar_t>::id,
#endif
    0
  };

  const locale::id* const* const
  locale::_Impl::_S_facet_categories[] =
  {
    // Order must match the decl order in class locale.
    locale::_Impl::_S_id_ctype,
    locale::_Impl::_S_id_numeric,
    locale::_Impl::_S_id_collate,
    locale::_Impl::_S_id_time,
    locale::_Impl::_S_id_monetary,
    locale::_Impl::_S_id_messages,
    0
  };

#if _GLIBCXX_USE_DUAL_ABI
  // Facets that are instantiated for both the COW and SSO std::string ABIs.
  // The COW ABI version must come first, followed by its SSO twin.
  const locale::id* const locale::_S_twinned_facets[] = {
    &::_ZNSt8numpunctIcE2idE,
    &numpunct<char>::id,
    &::_ZNSt8messagesIcE2idE,
    &std::messages<char>::id,
# ifdef _GLIBCXX_USE_WCHAR_T
    &::_ZNSt8numpunctIwE2idE,
    &numpunct<wchar_t>::id,
    &::_ZNSt8messagesIwE2idE,
    &std::messages<wchar_t>::id,
# endif
    0, 0
  };
#endif

  // Construct "C" _Impl.
  locale::_Impl::
  _Impl(size_t __refs) throw() 
  : _M_refcount(__refs), _M_facets(0), _M_facets_size(num_facets),
  _M_caches(0), _M_names(0)    
  {
    _M_facets = new (&facet_vec) const facet*[_M_facets_size]();
    _M_caches = new (&cache_vec) const facet*[_M_facets_size]();

    // Name the categories.
    _M_names = new (&name_vec) char*[_S_categories_size]();
    _M_names[0] = new (&name_c[0]) char[2];
    std::memcpy(_M_names[0], locale::facet::_S_get_c_name(), 2);

    // This is needed as presently the C++ version of "C" locales
    // != data in the underlying locale model for __timepunct,
    // numpunct, and moneypunct. Also, the "C" locales must be
    // constructed in a way such that they are pre-allocated.
    // NB: Set locale::facets(ref) count to one so that each individual
    // facet is not destroyed when the locale (and thus locale::_Impl) is
    // destroyed.
    _M_init_facet(new (&ctype_c) std::ctype<char>(0, false, 1));

    typedef __numpunct_cache<char> num_cache_c;
    num_cache_c* __npc = new (&numpunct_cache_c) num_cache_c(2);
    _M_init_facet(new (&numpunct_c) numpunct<char>(__npc, 1));

    _M_init_facet(new (&num_get_c) num_get<char>(1));
    _M_init_facet(new (&num_put_c) num_put<char>(1));

    _M_init_facet(new (&messages_c) std::messages<char>(1));	

#ifdef  _GLIBCXX_USE_WCHAR_T
    _M_init_facet(new (&ctype_w) std::ctype<wchar_t>(1));

    typedef __numpunct_cache<wchar_t> num_cache_w;
    num_cache_w* __npw = new (&numpunct_cache_w) num_cache_w(2);
    _M_init_facet(new (&numpunct_w) numpunct<wchar_t>(__npw, 1));

    _M_init_facet(new (&num_get_w) num_get<wchar_t>(1));
    _M_init_facet(new (&num_put_w) num_put<wchar_t>(1));

    _M_init_facet(new (&messages_w) std::messages<wchar_t>(1));
#endif

#if _GLIBCXX_USE_DUAL_ABI
    facet* extra[] = { __npc
# ifdef  _GLIBCXX_USE_WCHAR_T
        , __npw
# endif
    };

    _M_init_extra(extra);
#endif

    // This locale is safe to pre-cache, after all the facets have
    // been created and installed.
    _M_caches[numpunct<char>::id._M_id()] = __npc;
#ifdef  _GLIBCXX_USE_WCHAR_T
    _M_caches[numpunct<wchar_t>::id._M_id()] = __npw;
#endif
  }

_GLIBCXX_END_NAMESPACE_VERSION
} // namespace

// Copyright (C) 2014-2015 Free Software Foundation, Inc.
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

// Instantiate the facets using old std::string ABI.
#define _GLIBCXX_USE_CXX11_ABI 0
#include <locale>

#if ! _GLIBCXX_USE_DUAL_ABI
# error This file should not be compiled for this configuration.
#endif

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

namespace 
{
  typedef char fake_numpunct_c[sizeof(numpunct<char>)]
  __attribute__ ((aligned(__alignof__(numpunct<char>))));
  fake_numpunct_c numpunct_c;

  typedef char fake_messages_c[sizeof(messages<char>)]
  __attribute__ ((aligned(__alignof__(messages<char>))));
  fake_messages_c messages_c;

#ifdef  _GLIBCXX_USE_WCHAR_T
  typedef char fake_numpunct_w[sizeof(numpunct<wchar_t>)]
  __attribute__ ((aligned(__alignof__(numpunct<wchar_t>))));
  fake_numpunct_w numpunct_w;

  typedef char fake_messages_w[sizeof(messages<wchar_t>)]
  __attribute__ ((aligned(__alignof__(messages<wchar_t>))));
  fake_messages_w messages_w;
#endif
} // anonymous namespace

  void
  locale::_Impl::_M_init_extra(facet** caches)
  {
    auto __npc = static_cast<__numpunct_cache<char>*>(caches[0]);

    _M_init_facet_unchecked(new (&numpunct_c) numpunct<char>(__npc, 1));
    _M_init_facet_unchecked(new (&messages_c) std::messages<char>(1));
#ifdef _GLIBCXX_USE_WCHAR_T
    auto __npw = static_cast<__numpunct_cache<wchar_t>*>(caches[3]);

    _M_init_facet_unchecked(new (&numpunct_w) numpunct<wchar_t>(__npw, 1));
    _M_init_facet_unchecked(new (&messages_w) std::messages<wchar_t>(1));
#endif

    _M_caches[numpunct<char>::id._M_id()] = __npc;
#ifdef  _GLIBCXX_USE_WCHAR_T
    _M_caches[numpunct<wchar_t>::id._M_id()] = __npw;
#endif
  }

  void
  locale::_Impl::_M_init_extra(void* cloc, void* clocm,
                               const char* __s, const char* __smon)
  {
    auto& __cloc = *static_cast<__c_locale*>(cloc);

    _M_init_facet_unchecked(new numpunct<char>(__cloc));
    _M_init_facet_unchecked(new std::messages<char>(__cloc, __s));

#ifdef  _GLIBCXX_USE_WCHAR_T
    auto& __clocm = *static_cast<__c_locale*>(clocm);

    _M_init_facet_unchecked(new numpunct<wchar_t>(__cloc));
    _M_init_facet_unchecked(new std::messages<wchar_t>(__cloc, __s));
#endif	  
  }

// TODO should be in another file
  string
  locale::name() const
  {
    string __ret;
    if (!_M_impl->_M_names[0])
      __ret = '*';
    else if (_M_impl->_M_check_same_name())
      __ret = _M_impl->_M_names[0];
    else
      {
	__ret.reserve(128);
	__ret += _S_categories[0];
	__ret += '=';
	__ret += _M_impl->_M_names[0]; 
	for (size_t __i = 1; __i < _S_categories_size; ++__i)
	  {
	    __ret += ';';
	    __ret += _S_categories[__i];
	    __ret += '=';
	    __ret += _M_impl->_M_names[__i];
	  }
      }
    return __ret;
  }

_GLIBCXX_END_NAMESPACE_VERSION
}

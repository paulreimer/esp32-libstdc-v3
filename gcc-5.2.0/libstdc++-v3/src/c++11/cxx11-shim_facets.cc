// Locale support -*- C++ -*-

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

//
// ISO C++ 14882: 22.1  Locales
//

// This file defines classes that behave like the standard predefined locale
// facets except that they forward all virtual
// functions to another facet which uses a different std::string ABI,
// converting between string types as needed.
// When a user replaces one of the relevant facets the corresponding shim in
// this file is used so that the replacement facet can be used (via the shim)
// in code that uses the other std::string ABI from the replacing code.

#ifndef _GLIBCXX_USE_CXX11_ABI
# define _GLIBCXX_USE_CXX11_ABI 1
#endif
#include <locale>

#if ! _GLIBCXX_USE_DUAL_ABI
# error This file should not be compiled for this configuration.
#endif

namespace std _GLIBCXX_VISIBILITY(default)
{
  // Base class of facet shims, holds a reference to the underlying facet
  // that the shim forwards to.
  class locale::facet::__shim
  {
  public:
    const facet* _M_get() const { return _M_facet; }

    __shim(const __shim&) = delete;
    __shim& operator=(const __shim&) = delete;

  protected:
    explicit
    __shim(const facet* __f) : _M_facet(__f) { __f->_M_add_reference(); }

    ~__shim() { _M_facet->_M_remove_reference(); }

  private:
    const facet* _M_facet;
  };

namespace __facet_shims
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  namespace // unnamed
  {
    template<typename C>
      void __destroy_string(void* p)
      {
	static_cast<std::basic_string<C>*>(p)->~basic_string();
      }
  } // namespace

  // Manages a buffer of uninitialized memory that can store a std::string
  // or std::wstring, using either ABI, and convert to the other ABI.
  class __any_string
  {
    struct __attribute__((may_alias)) __str_rep
    {
      union {
	const void* _M_p;
	char* _M_pc;
#ifdef _GLIBCXX_USE_WCHAR_T
	wchar_t* _M_pwc;
#endif
      };
      size_t _M_len;
      char _M_unused[16];

      operator const char*() const { return _M_pc; }
#ifdef _GLIBCXX_USE_WCHAR_T
      operator const wchar_t*() const { return _M_pwc; }
#endif
    };
    union {
      __str_rep _M_str;
      char _M_bytes[sizeof(__str_rep)];
    };
    using __dtor_func = void(*)(void*);
    __dtor_func _M_dtor = nullptr;

#if _GLIBCXX_USE_CXX11_ABI
    // SSO strings overlay the entire __str_rep structure.
    static_assert(sizeof(std::string) == sizeof(__str_rep),
		  "std::string changed size!");
#else
    // COW strings overlay just the pointer, the length is stored manually.
    static_assert(sizeof(std::string) == sizeof(__str_rep::_M_p),
		  "std::string changed size!");
#endif
# ifdef _GLIBCXX_USE_WCHAR_T
    static_assert(sizeof(std::wstring) == sizeof(std::string),
		  "std::wstring and std::string are different sizes!");
# endif

  public:
    __any_string() = default;
    ~__any_string() { if (_M_dtor) _M_dtor(_M_bytes); }

    __any_string(const __any_string&) = delete;
    __any_string& operator=(const __any_string&) = delete;

    // Store a string (and its length if needed) in the buffer and
    // set _M_dtor to the function that runs the right destructor.
    template<typename C>
      __any_string&
      operator=(const basic_string<C>& s)
      {
	if (_M_dtor)
	  _M_dtor(_M_bytes);
	::new(_M_bytes) basic_string<C>(s);
#if ! _GLIBCXX_USE_CXX11_ABI
	_M_str._M_len = s.length();
#endif
	_M_dtor = __destroy_string<C>;
	return *this;
      }

    // Create a new string with a copy of the characters in the stored string.
    // The returned object will match the caller's string ABI, even when the
    // stored string doesn't.
    template<typename C>
      _GLIBCXX_DEFAULT_ABI_TAG
      operator basic_string<C>() const
      {
	if (!_M_dtor)
	  __throw_logic_error("uninitialized __any_string");
	return basic_string<C>(static_cast<const C*>(_M_str), _M_str._M_len);
      }
  };

  // This file is compiled twice, with and without this macro defined.
  // Define tag types to distinguish between the two cases and to allow
  // overloading on the tag.
  using current_abi = __bool_constant<_GLIBCXX_USE_CXX11_ABI>;
  using other_abi = __bool_constant<!_GLIBCXX_USE_CXX11_ABI>;

  using facet = locale::facet;

  // Declare the functions that shims defined in this file will call to
  // perform work in the context of the other ABI.
  // These will be defined when this file is recompiled for the other ABI
  // (at which point what is now "current_abi" will become "other_abi").

  template<typename C>
    void
    __numpunct_fill_cache(other_abi, const facet*, __numpunct_cache<C>*);

  template<typename C>
    messages_base::catalog
    __messages_open(other_abi, const facet*, const char*, size_t,
		    const locale&);

  template<typename C>
    void
    __messages_get(other_abi, const facet*, __any_string&,
		   messages_base::catalog, int, int, const C*, size_t);

  template<typename C>
    void
    __messages_close(other_abi, const facet*, messages_base::catalog);

  namespace // unnamed
  {
    template<typename _CharT>
      struct numpunct_shim : std::numpunct<_CharT>, facet::__shim
      {
	typedef typename numpunct<_CharT>::__cache_type __cache_type;

	// f must point to a type derived from numpunct<C>[abi:other]
	numpunct_shim(const facet* f, __cache_type* c = new __cache_type)
	: std::numpunct<_CharT>(c), __shim(f), _M_cache(c)
	{
	  __numpunct_fill_cache(other_abi{}, f, c);
	}

	~numpunct_shim()
	{
	  // Stop GNU locale's ~numpunct() from freeing the cached string.
	  _M_cache->_M_grouping_size = 0;
	}

	// No need to override any virtual functions, the base definitions
	// will return the cached data.

	__cache_type* _M_cache;
      };

    template<typename _CharT>
      struct messages_shim : std::messages<_CharT>, facet::__shim
      {
	typedef messages_base::catalog  catalog;
	typedef basic_string<_CharT>	string_type;

	// f must point to a type derived from messages<C>[abi:other]
	messages_shim(const facet* f) : __shim(f) { }

	virtual catalog
	do_open(const basic_string<char>& s, const locale& l) const
	{
	  return __messages_open<_CharT>(other_abi{}, _M_get(),
					 s.c_str(), s.size(), l);
	}

	virtual string_type
	do_get(catalog c, int set, int msgid, const string_type& dfault) const
	{
	  __any_string st;
	  __messages_get(other_abi{}, _M_get(), st, c, set, msgid,
			 dfault.c_str(), dfault.size());
	  return st;
	}

	virtual void
	do_close(catalog c) const
	{
	  __messages_close<_CharT>(other_abi{}, _M_get(), c);
	}
      };

    template class numpunct_shim<char>;
    template class messages_shim<char>;
#ifdef _GLIBCXX_USE_WCHAR_T
    template class numpunct_shim<wchar_t>;
    template class messages_shim<wchar_t>;
#endif

    template<typename C>
      inline size_t
      __copy(const C*& dest, const basic_string<C>& s)
      {
	auto len = s.length();
	C* p = new C[len+1];
	s.copy(p, len);
	p[len] = '\0';
	dest = p;
	return len;
      }

  } // namespace

  // Now define and instantiate the functions that will be called by the
  // shim facets defined when this file is recompiled for the other ABI.

  // Cache the values returned by the numpunct facet f.
  // Sets c->_M_allocated so that the __numpunct_cache destructor will
  // delete[] the strings allocated by this function.
  template<typename C>
    void
    __numpunct_fill_cache(current_abi, const facet* f, __numpunct_cache<C>* c)
    {
      auto* m = static_cast<const numpunct<C>*>(f);

      c->_M_decimal_point = m->decimal_point();
      c->_M_thousands_sep = m->thousands_sep();

      c->_M_grouping = nullptr;
      c->_M_truename = nullptr;
      c->_M_falsename = nullptr;
      // set _M_allocated so that if any allocation fails the previously
      // allocated strings will be deleted in ~__numpunct_cache()
      c->_M_allocated = true;

      c->_M_grouping_size = __copy(c->_M_grouping, m->grouping());
      c->_M_truename_size = __copy(c->_M_truename, m->truename());
      c->_M_falsename_size = __copy(c->_M_falsename, m->falsename());
    }

  template void
  __numpunct_fill_cache(current_abi, const facet*, __numpunct_cache<char>*);

#ifdef _GLIBCXX_USE_WCHAR_T
  template void
  __numpunct_fill_cache(current_abi, const facet*, __numpunct_cache<wchar_t>*);
#endif

  template<typename C>
    messages_base::catalog
    __messages_open(current_abi, const facet* f, const char* s, size_t n,
		    const locale& l)
    {
      auto* m = static_cast<const messages<C>*>(f);
      string str(s, n);
      return m->open(str, l);
    }

  template messages_base::catalog
  __messages_open<char>(current_abi, const facet*, const char*, size_t,
			const locale&);

#ifdef _GLIBCXX_USE_WCHAR_T
  template messages_base::catalog
  __messages_open<wchar_t>(current_abi, const facet*, const char*, size_t,
			   const locale&);
#endif

  template<typename C>
    void
    __messages_get(current_abi, const facet* f, __any_string& st,
		   messages_base::catalog c, int set, int msgid,
		   const C* s, size_t n)
    {
      auto* m = static_cast<const messages<C>*>(f);
      st = m->get(c, set, msgid, basic_string<C>(s, n));
    }

  template void
  __messages_get(current_abi, const facet*, __any_string&,
		 messages_base::catalog, int, int, const char*, size_t);

#ifdef _GLIBCXX_USE_WCHAR_T
  template void
  __messages_get(current_abi, const facet*, __any_string&,
		 messages_base::catalog, int, int, const wchar_t*, size_t);
#endif

  template<typename C>
    void
    __messages_close(current_abi, const facet* f, messages_base::catalog c)
    {
      static_cast<const messages<C>*>(f)->close(c);
    }

  template void
  __messages_close<char>(current_abi, const facet*, messages_base::catalog c);

#ifdef _GLIBCXX_USE_WCHAR_T
  template void
  __messages_close<wchar_t>(current_abi, const facet*,
			    messages_base::catalog c);
#endif

_GLIBCXX_END_NAMESPACE_VERSION
} // namespace __facet_shims

_GLIBCXX_BEGIN_NAMESPACE_VERSION
  // Create a new shim facet of type WHICH that forwards calls to F.
  // F is the replacement facet provided by the user, WHICH is the ID of
  // F's "other ABI twin" which we are replacing with a shim.
  const locale::facet*
#if _GLIBCXX_USE_CXX11_ABI
  locale::facet::_M_sso_shim(const locale::id* which) const
#else
  locale::facet::_M_cow_shim(const locale::id* which) const
#endif
  {
    using namespace __facet_shims;

#if __cpp_rtti
    // If this is already a shim just use its underlying facet.
    if (auto* p = dynamic_cast<const __shim*>(this))
      return p->_M_get();
#endif

    if (which == &numpunct<char>::id)
      return new numpunct_shim<char>{this};
    if (which == &std::messages<char>::id)
      return new messages_shim<char>{this};
#ifdef _GLIBCXX_USE_WCHAR_T
    if (which == &numpunct<wchar_t>::id)
      return new numpunct_shim<wchar_t>{this};
    if (which == &std::messages<wchar_t>::id)
      return new messages_shim<wchar_t>{this};
#endif
    __throw_logic_error("cannot create shim for unknown locale::facet");
  }

_GLIBCXX_END_NAMESPACE_VERSION
} // namespace std

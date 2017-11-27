// Locale support -*- C++ -*-

// Copyright (C) 2007-2015 Free Software Foundation, Inc.
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

/** @file bits/locale_facets_nonio.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{locale}
 */

//
// ISO C++ 14882: 22.1  Locales
//

#ifndef _LOCALE_FACETS_NONIO_H
#define _LOCALE_FACETS_NONIO_H 1

#pragma GCC system_header

#include <ctime>	// For struct tm

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  /**
   *  @brief  Messages facet base class providing catalog typedef.
   *  @ingroup locales
   */
  struct messages_base
  {
    typedef int catalog;
  };

_GLIBCXX_BEGIN_NAMESPACE_CXX11

  /**
   *  @brief  Primary class template messages.
   *  @ingroup locales
   *
   *  This facet encapsulates the code to retrieve messages from
   *  message catalogs.  The only thing defined by the standard for this facet
   *  is the interface.  All underlying functionality is
   *  implementation-defined.
   *
   *  This library currently implements 3 versions of the message facet.  The
   *  first version (gnu) is a wrapper around gettext, provided by libintl.
   *  The second version (ieee) is a wrapper around catgets.  The final
   *  version (default) does no actual translation.  These implementations are
   *  only provided for char and wchar_t instantiations.
   *
   *  The messages template uses protected virtual functions to
   *  provide the actual results.  The public accessors forward the
   *  call to the virtual functions.  These virtual functions are
   *  hooks for developers to implement the behavior they require from
   *  the messages facet.
  */
  template<typename _CharT>
    class messages : public locale::facet, public messages_base
    {
    public:
      // Types:
      //@{
      /// Public typedefs
      typedef _CharT			char_type;
      typedef basic_string<_CharT>	string_type;
      //@}

    protected:
      // Underlying "C" library locale information saved from
      // initialization, needed by messages_byname as well.
      __c_locale			_M_c_locale_messages;
      const char*			_M_name_messages;

    public:
      /// Numpunct facet id.
      static locale::id			id;

      /**
       *  @brief  Constructor performs initialization.
       *
       *  This is the constructor provided by the standard.
       *
       *  @param __refs  Passed to the base facet class.
      */
      explicit
      messages(size_t __refs = 0);

      // Non-standard.
      /**
       *  @brief  Internal constructor.  Not for general use.
       *
       *  This is a constructor for use by the library itself to set up new
       *  locales.
       *
       *  @param  __cloc  The C locale.
       *  @param  __s  The name of a locale.
       *  @param  __refs  Refcount to pass to the base class.
       */
      explicit
      messages(__c_locale __cloc, const char* __s, size_t __refs = 0);

      /*
       *  @brief  Open a message catalog.
       *
       *  This function opens and returns a handle to a message catalog by
       *  returning do_open(__s, __loc).
       *
       *  @param  __s  The catalog to open.
       *  @param  __loc  Locale to use for character set conversions.
       *  @return  Handle to the catalog or value < 0 if open fails.
      */
      catalog
      open(const basic_string<char>& __s, const locale& __loc) const
      { return this->do_open(__s, __loc); }

      // Non-standard and unorthodox, yet effective.
      /*
       *  @brief  Open a message catalog.
       *
       *  This non-standard function opens and returns a handle to a message
       *  catalog by returning do_open(s, loc).  The third argument provides a
       *  message catalog root directory for gnu gettext and is ignored
       *  otherwise.
       *
       *  @param  __s  The catalog to open.
       *  @param  __loc  Locale to use for character set conversions.
       *  @param  __dir  Message catalog root directory.
       *  @return  Handle to the catalog or value < 0 if open fails.
      */
      catalog
      open(const basic_string<char>&, const locale&, const char*) const;

      /*
       *  @brief  Look up a string in a message catalog.
       *
       *  This function retrieves and returns a message from a catalog by
       *  returning do_get(c, set, msgid, s).
       *
       *  For gnu, @a __set and @a msgid are ignored.  Returns gettext(s).
       *  For default, returns s. For ieee, returns catgets(c,set,msgid,s).
       *
       *  @param  __c  The catalog to access.
       *  @param  __set  Implementation-defined.
       *  @param  __msgid  Implementation-defined.
       *  @param  __s  Default return value if retrieval fails.
       *  @return  Retrieved message or @a __s if get fails.
      */
      string_type
      get(catalog __c, int __set, int __msgid, const string_type& __s) const
      { return this->do_get(__c, __set, __msgid, __s); }

      /*
       *  @brief  Close a message catalog.
       *
       *  Closes catalog @a c by calling do_close(c).
       *
       *  @param  __c  The catalog to close.
      */
      void
      close(catalog __c) const
      { return this->do_close(__c); }

    protected:
      /// Destructor.
      virtual
      ~messages();

      /*
       *  @brief  Open a message catalog.
       *
       *  This function opens and returns a handle to a message catalog in an
       *  implementation-defined manner.  This function is a hook for derived
       *  classes to change the value returned.
       *
       *  @param  __s  The catalog to open.
       *  @param  __loc  Locale to use for character set conversions.
       *  @return  Handle to the opened catalog, value < 0 if open failed.
      */
      virtual catalog
      do_open(const basic_string<char>&, const locale&) const;

      /*
       *  @brief  Look up a string in a message catalog.
       *
       *  This function retrieves and returns a message from a catalog in an
       *  implementation-defined manner.  This function is a hook for derived
       *  classes to change the value returned.
       *
       *  For gnu, @a __set and @a __msgid are ignored.  Returns gettext(s).
       *  For default, returns s. For ieee, returns catgets(c,set,msgid,s).
       *
       *  @param  __c  The catalog to access.
       *  @param  __set  Implementation-defined.
       *  @param  __msgid  Implementation-defined.
       *  @param  __s  Default return value if retrieval fails.
       *  @return  Retrieved message or @a __s if get fails.
      */
      virtual string_type
      do_get(catalog, int, int, const string_type& __dfault) const;

      /*
       *  @brief  Close a message catalog.
       *
       *  @param  __c  The catalog to close.
      */
      virtual void
      do_close(catalog) const;

      // Returns a locale and codeset-converted string, given a char* message.
      char*
      _M_convert_to_char(const string_type& __msg) const
      {
	// XXX
	return reinterpret_cast<char*>(const_cast<_CharT*>(__msg.c_str()));
      }

      // Returns a locale and codeset-converted string, given a char* message.
      string_type
      _M_convert_from_char(char*) const
      {
	// XXX
	return string_type();
      }
     };

  template<typename _CharT>
    locale::id messages<_CharT>::id;

  /// Specializations for required instantiations.
  template<>
    string
    messages<char>::do_get(catalog, int, int, const string&) const;

#ifdef _GLIBCXX_USE_WCHAR_T
  template<>
    wstring
    messages<wchar_t>::do_get(catalog, int, int, const wstring&) const;
#endif

   /// class messages_byname [22.2.7.2].
   template<typename _CharT>
    class messages_byname : public messages<_CharT>
    {
    public:
      typedef _CharT			char_type;
      typedef basic_string<_CharT>	string_type;

      explicit
      messages_byname(const char* __s, size_t __refs = 0);

#if __cplusplus >= 201103L
      explicit
      messages_byname(const string& __s, size_t __refs = 0)
      : messages_byname(__s.c_str(), __refs) { }
#endif

    protected:
      virtual
      ~messages_byname()
      { }
    };

_GLIBCXX_END_NAMESPACE_CXX11

_GLIBCXX_END_NAMESPACE_VERSION
} // namespace

// Include host and configuration specific messages functions.
#include <bits/messages_members.h>

#include <bits/locale_facets_nonio.tcc>

#endif

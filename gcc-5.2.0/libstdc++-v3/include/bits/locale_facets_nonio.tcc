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

/** @file bits/locale_facets_nonio.tcc
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{locale}
 */

#ifndef _LOCALE_FACETS_NONIO_TCC
#define _LOCALE_FACETS_NONIO_TCC 1

#pragma GCC system_header

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  // Inhibit implicit instantiations for required instantiations,
  // which are defined via explicit instantiations elsewhere.
#if _GLIBCXX_EXTERN_TEMPLATE
  extern template class messages<char>;
  extern template class messages_byname<char>;

  extern template
    bool
    has_facet<messages<char> >(const locale&);

#ifdef _GLIBCXX_USE_WCHAR_T
  extern template class messages<wchar_t>;
  extern template class messages_byname<wchar_t>;

  extern template
    const messages<wchar_t>&
    use_facet<messages<wchar_t> >(const locale&);

  extern template
    bool
    has_facet<messages<wchar_t> >(const locale&);
#endif
#endif

_GLIBCXX_END_NAMESPACE_VERSION
} // namespace std

#endif

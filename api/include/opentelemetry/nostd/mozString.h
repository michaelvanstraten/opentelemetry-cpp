/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef THIRD_PARTY_OPENTELEMETRY_CPP_MOZILLA_STRING_H_
#define THIRD_PARTY_OPENTELEMETRY_CPP_MOZILLA_STRING_H_

#include <initializer_list>
#include <iterator>
#include <limits>
#include <memory>
#include <ostream>
#include <regex>
#include <string_view>
#include <utility>

#include "mozilla/Span.h"
#include "nsTString.h"

namespace mozilla {

class NoOp {};

template <class charT, class traits = typename ::nsTString<charT>::char_traits,
          class Allocator = NoOp>
class basic_string {
 public:
  using impl_type = nsTString<charT>;

  using value_type = charT;
  using traits_type = traits;
  using allocator_type = Allocator;
  using reference = charT&;
  using const_reference = const charT&;
  using pointer = charT*;
  using const_pointer = const charT*;
  using iterator = typename impl_type::iterator;
  using const_iterator = typename impl_type::const_iterator;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  using difference_type = ptrdiff_t;
  using size_type = typename impl_type::size_type;

  using span_type = mozilla::Span<charT>;

  static constexpr size_type npos = size_type(-1);

  basic_string(const Allocator& a = Allocator()) : mImpl() {}

  basic_string(const basic_string& str, size_type pos = 0, size_type n = npos,
               const Allocator& a = Allocator())
      : mImpl() {
    // TODO: Choose a better alternative
    append(str, pos, (n == npos ? str.size() : n));
  }

  basic_string(const charT* s, size_type n, const Allocator& a = Allocator())
      : mImpl(s, n) {}

  basic_string(const charT* s, const Allocator& a = Allocator()) : mImpl(s) {}

  basic_string(size_type n, charT c, const Allocator& a = Allocator())
      : mImpl() {
    mImpl.SetLength(n);
    mImpl.Replace(0, n, c);
  }

  template <class InputIterator>
  basic_string(InputIterator begin, InputIterator end,
               const Allocator& a = Allocator())
      : mImpl() {
    // TODO: Choose a better alternative
    for (; begin != end; ++begin) {
      mImpl.Append(*begin);
    }
  }

  basic_string(std::initializer_list<charT> ilist,
               const Allocator& alloc = Allocator())
      : mImpl() {
    // TODO: Choose a better alternative
    for (const auto& elem : ilist) {
      mImpl.Append(elem);
    }
  }

  template <class StringViewLike>
  explicit basic_string(const StringViewLike& t,
                        const Allocator& alloc = Allocator())
      : mImpl() {
    // TODO: Choose a better alternative
    for (const auto& elem : t) {
      mImpl.Append(elem);
    }
  }

  template <class StringViewLike>
  basic_string(const StringViewLike& t, size_type pos, size_type n,
               const Allocator& alloc = Allocator())
      : mImpl() {
    const auto startRead = t.begin() + pos;
    const auto endRead = startRead + n;
    for (auto it = startRead; it != t.end() && it != endRead; ++it) {
      mImpl.Append(*it);
    }
  }

  ~basic_string() = default;

  basic_string& operator=(const basic_string& str) {
    mImpl = str.mImpl;
    return *this;
  }
  basic_string& operator=(const charT* s) {
    mImpl = s;
    return *this;
  }
  basic_string& operator=(charT c) {
    mImpl = c;
    return *this;
  }
  basic_string& operator=(const std::string& str) {
    mImpl = basic_string(str.c_str());
    return *this;
  }

  iterator begin() { return mImpl.BeginWriting(); }
  const_iterator begin() const { return mImpl.BeginReading(); }
  iterator end() { return mImpl.EndWriting(); }
  const_iterator end() const { return mImpl.EndReading(); }

  const_iterator cbegin() const { return mImpl.BeginReading(); }
  const_iterator cend() const { return mImpl.EndReading(); }

  reverse_iterator rbegin() { return std::make_reverse_iterator(end()); }
  const_reverse_iterator rbegin() const {
    return std::make_reverse_iterator(end());
  }
  reverse_iterator rend() { return std::make_reverse_iterator(begin()); }
  const_reverse_iterator rend() const {
    return std::make_reverse_iterator(begin());
  }

  size_type size() const { return mImpl.Length(); }
  size_type length() const { return mImpl.Length(); }
  size_type max_size() const {
    return ::mozilla::detail::nsTStringLengthStorage<char>::kMax;
  }

  void resize(size_type n) { mImpl.SetLength(n); }
  void resize(size_type n, charT c) {
    resize(n);
    mImpl.Replace(0, n, c);
  }

  size_type capacity() const { return mImpl.Capacity(); }
  void reserve(size_type res_arg = 0) { mImpl.SetCapacity(res_arg); }
  void clear() { mImpl.SetToEmptyBuffer(); }
  bool empty() const { return 0 == length(); }

  const_reference operator[](size_type pos) const {
    return *(mImpl.BeginReading() + pos);
  }
  reference operator[](size_type pos) { return *(mImpl.BeginWriting() + pos); }
  const_reference at(size_type n) const { return (*this)[pos]; }
  reference at(size_type n) { return (*this)[pos]; }

  basic_string& operator+=(const basic_string& str) {
    mImpl.Append(str.mImpl);
    return *this;
  }
  basic_string& operator+=(const charT* s) {
    mImpl.Append(s);
    return *this;
  }
  basic_string& operator+=(charT c) {
    mImpl.Append(c);
    return *this;
  }
  basic_string& append(const basic_string& str) {
    mImpl.Append(str.mImpl);
    return *this;
  }
  basic_string& append(const basic_string& str, size_type pos, size_type n) {
    mImpl.Append(str.mImpl.BeginReading() + pos, n);
    return *this;
  }
  basic_string& append(const charT* s, size_type n) {
    mImpl.Append(s, n);
    return *this;
  }
  basic_string& append(const charT* s) {
    mImpl.Append(s);
    return *this;
  }
  basic_string& append(size_type n, charT c) {
    const auto len = size();
    resize(len + n);
    insert(len, n, c);
    return *this;
  }

  template <class InputIterator>
  basic_string& append(InputIterator first, InputIterator last) {
    mImpl.Append(first, last);
    return *this;
  }

  void push_back(const charT c) { mImpl.Append(c); }

  basic_string& assign(const basic_string& str) {
    mImpl.Assign(std.mImpl);
    return *this;
  }
  basic_string& assign(const basic_string& str, size_type pos, size_type n) {
    mImpl.Assign(str.mImpl);
    return *this;
  }
  basic_string& assign(const charT* s, size_type n) {
    mImpl.Assign(span_type(s, n));
    return *this;
  }
  basic_string& assign(const charT* s) {
    mImpl.Assign(s);
    return *this;
  }
  basic_string& assign(size_type n, charT c) {
    resize(n);
    return replace(0, n, c);
  }

  template <class InputIterator>
  basic_string& assign(InputIterator first, InputIterator last) {
    mImpl.Assign(first, last);
    return *this;
  }

  basic_string& insert(size_type pos1, const basic_string& str) {
    mImpl.Insert(str.mImpl, pos1);
    return *this;
  }
  basic_string& insert(size_type pos1, const basic_string& str, size_type pos2,
                       size_type n) {
    assert(pos1 + n <= str.Length());
    mImpl.Replace(pos2, n, str.data() + pos1);
    return *this;
  }
  basic_string& insert(size_type pos, const charT* s, size_type n) {
    mImpl.Insert(span_type(s, n), pos);
    return *this;
  }
  basic_string& insert(size_type pos, const charT* s) {
    mImpl.Insert(s, pos);
    return *this;
  }
  basic_string& insert(size_type pos, size_type n, charT c) {
    mImpl.Replace(pos, n, c);
    return *this;
  }

  iterator insert(iterator p, charT c) {
    *p = c;
    return ++p;
  }

  void insert(iterator p, size_type n, charT c) {
    const auto pEnd = p + n;
    while (p != pEnd) {
      *p = c;
      ++p;
    }
  }

  template <class InputIterator>
  void insert(iterator p, InputIterator first, InputIterator last) {
    while (first != last) {
      *first = *p;
      ++first;
      ++p;
    } /* There are nicer ways to do this */
  }

  basic_string& erase(size_type pos = 0, size_type n = npos) {
    mImpl.Erase(pos, n);
  }
  iterator erase(iterator position) {
    ++position;
    return position;
  }  // TODO
  iterator erase(iterator first, iterator last) {
    ++last;
    return last;
  }  // TODO

  basic_string& replace(size_type pos1, size_type n1, const basic_string& str) {
    mImpl.Replace(pos1, n1, str.mImpl);
    return *this;
  }
  basic_string& replace(size_type pos1, size_type n1, const basic_string& str,
                        size_type pos2, size_type n2) {
    mImpl.Replace(pos1, n1, str.mImpl, pos2, n2);
    return *this;
  }
  basic_string& replace(size_type pos, size_type n1, const charT* s,
                        size_type n2) {
    mImpl.Replace(pos1, n1, s, 0, n2);
    return *this;
  }
  basic_string& replace(size_type pos, size_type n1, const charT* s) {
    mImpl.Replace(pos1, n1, s);
    return *this;
  }
  basic_string& replace(size_type pos, size_type n1, size_type n2, charT c) {
    mImpl.Replace(pos, n1, basic_string(n2, c));
    return *this;
  }

  basic_string& replace(iterator i1, iterator i2,
                        const basic_string& str){return *this}  // TODO
  basic_string& replace(iterator i1, iterator i2, const charT* s, size_type n) {
    return *this;
  }  // TODO
  basic_string& replace(iterator i1, iterator i2, const charT* s) {
    return *this;
  }  // TODO
  basic_string& replace(iterator i1, iterator i2, size_type n, charT c) {
    return *this;
  }  // TODO

  template <class InputIterator>
  basic_string& replace(iterator i1, iterator i2, InputIterator j1,
                        InputIterator j2) {
    return *this;
  }  // TODO

  size_type copy(charT* dest, size_type n, size_type pos = 0) const {
    return mImpl.View().copy(dest, n, pos);
  }

  void swap(basic_string<charT, traits, Allocator>& aOther) {
    std::swap(mImpl.View(), aOther.mImpl.View());
  }

  const charT* data() const {
    if constexpr (std::is_same_v<charT, char>) {
      return mImpl.BeginReading();
    }
  }

  const charT* c_str() const { return data(); }

  allocator_type get_allocator() const { return Allocator(); }

  size_type find(const basic_string& str, size_type pos = 0) const {
    return mImpl.View().find(str.mImpl.View(), pos);
  }
  size_type find(const charT* s, size_type pos, size_type n) const {
    mImpl.View().find(s, pos, n);
  }
  size_type find(const charT* s, size_type pos = 0) const {
    return mImpl.View().find(s, pos);
  }
  size_type find(charT c, size_type pos = 0) const {
    return mImpl.View().find(c, pos);
  }

  size_type rfind(const basic_string& str, size_type pos = npos) const {
    return mImpl.View().rfind(str.mImpl.View(), pos);
  }
  size_type rfind(const charT* s, size_type pos, size_type n) const {
    return mImpl.View().rfind(s, pos, n);
  }
  size_type rfind(const charT* s, size_type pos = npos) const {
    return mImpl.View().rfind(s, pos);
  }
  size_type rfind(charT c, size_type pos = npos) const {
    return mImpl.View().rfind(c, pos);
  }

  size_type find_first_of(const basic_string& str, size_type pos = 0) const {
    return mImpl.View().find_first_of(str.mImpl.View(), pos);
  }
  size_type find_first_of(const charT* s, size_type pos, size_type n) const {
    return mImpl.View().find_first_of(s, pos, n);
  }
  size_type find_first_of(const charT* s, size_type pos = 0) const {
    return mImpl.View().find_first_of(s, pos);
  }
  size_type find_first_of(charT c, size_type pos = 0) const {
    return mImpl.View().find_first_of(c, pos);
  }
  size_type find_last_of(const basic_string& str, size_type pos = npos) const {
    return mImpl.View().find_last_of(str.View(), pos);
  }
  size_type find_last_of(const charT* s, size_type pos, size_type n) const {
    mImpl.View().find_last_of(s, pos, n);
  }
  size_type find_last_of(const charT* s, size_type pos = npos) const {
    return mImpl.View().find_last_of(s, pos);
  }
  size_type find_last_of(charT c, size_type pos = npos) const {
    return mImpl.View().find_last_of(c, pos);
  }

  size_type find_first_not_of(const basic_string& str,
                              size_type pos = 0) const {
    return mImpl.View().find_first_not_of(str.mImpl.View(), pos);
  }
  size_type find_first_not_of(const charT* s, size_type pos,
                              size_type n) const {
    return mImpl.View().find_first_not_of(s, pos);
  }
  size_type find_first_not_of(const charT* s, size_type pos = 0) const {
    return mImpl.View().find_first_not_of(s, pos);
  }
  size_type find_first_not_of(charT c, size_type pos = 0) const {
    return mImpl.View().find_first_not_of(c, pos);
  }
  size_type find_last_not_of(const basic_string& str,
                             size_type pos = npos) const {
    return mImpl.View().find_last_not_of(str.mImpl.View(), pos);
  }
  size_type find_last_not_of(const charT* s, size_type pos, size_type n) const {
    return mImpl.View().find_last_not_of(s, pos);
  }
  size_type find_last_not_of(const charT* s, size_type pos = npos) const {
    return mImpl.View().find_last_not_of(s, pos);
  }
  size_type find_last_not_of(charT c, size_type pos = npos) const {
    return mImpl.View().find_last_not_of(c, pos);
  }

  basic_string substr(size_type pos = 0, size_type n = npos) const {
    return basic_string(mImpl.View().substr(pos, n));
  }

  int compare(const basic_string& str) const {
    return mImpl.View().compare(str.mImpl.View());
  }
  int compare(size_type pos1, size_type n1, const basic_string& str) const {
    return mImpl.View().compare(pos1, n1, str.mImpl.View());
  }
  int compare(size_type pos1, size_type n1, const basic_string& str,
              size_type pos2, size_type n2) const {
    return mImpl.View().compare(pos1, n1, str.mImpl.View(), pos2, n2);
  }
  int compare(const charT* s) const { return mImpl.View().compare(s); }
  int compare(size_type pos1, size_type n1, const charT* s,
              size_type n2 = npos) const {
    mImpl.View().compare(pos1, n1, s, n2);
  }

  operator std::basic_string_view<charT>() const noexcept {
    return mImpl.View();
  }

  operator std::basic_istringstream<charT>() const {
    return std::basic_istringstream<charT>(mImpl.BeginReading(),
                                           mImpl.Length());
  }

  operator const charT*() const { return mImpl.BeginReading(); }

  friend constexpr bool operator==(const basic_string& aLhs,
                                   const basic_string& aRhs) {
    return aLhs.mImpl.Equals(aRhs.mImpl);
  }

  friend constexpr basic_string operator+(const basic_string& aLhs,
                                          const basic_string& aRhs) {
    return basic_string(aLhs).append(aRhs);
  }

  friend constexpr basic_string operator+(const basic_string& aLhs,
                                          const charT* aRhs) {
    return basic_string(aLhs).append(aRhs);
  }

  friend constexpr basic_string operator+(const basic_string& aLhs,
                                          const std::string& aRhs) {
    return basic_string(aLhs).append(aRhs.c_str());
  }

  friend constexpr basic_string operator+(const basic_string& aLhs,
                                          charT aRhs) {
    return basic_string(aLhs).append(aRhs);
  }

  friend inline std::ostream& operator<<(std::ostream& aStream,
                                         const basic_string& aString) {
    aStream << aString.mImpl.View();

    return aStream;
  }

 private:
  impl_type mImpl;
};

}  // namespace mozilla

namespace std {

template <class charT>
struct hash<mozilla::basic_string<charT>> {
  std::size_t operator()(const mozilla::basic_string<charT>& k) const {
    return std::hash<std::string_view>{}(k);
  }
};

template <class charT>
inline std::ostream& operator+(std::ostream& aStream,
                               const mozilla::basic_string<charT>& aString) {
  return aStream << aString.View();
}

template <class CharT, class Traits>
std::basic_istream<CharT, Traits>& getline(
    std::basic_istream<CharT, Traits>& input, mozilla::basic_string<CharT>& str,
    CharT delim) {
  std::string result;
  getline(input, result, delim);
  str.append(result.data(), result.length());
  return input;
}

template <class CharT, class Traits>
bool regex_match(const mozilla::basic_string<CharT>& s,
                 const std::basic_regex<CharT, Traits>& e,
                 std::regex_constants::match_flag_type flags =
                     std::regex_constants::match_default) {
  return std::regex_match(s.c_str(), e, flags);
}

template <class CharT, class Traits = std::char_traits<CharT>,
          class Allocator = std::allocator<CharT>>
std::basic_string<CharT, Traits, Allocator> to_string(
    mozilla::basic_string<CharT>& str) {
  return std::basic_string<CharT, Traits, Allocator>(input);
}

}  // namespace std

#endif  // THIRD_PARTY_OPENTELEMETRY_CPP_MOZILLA_STRING_H_
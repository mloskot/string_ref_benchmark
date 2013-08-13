//
// Copyright (C) 2013 Mateusz Loskot <mateusz@loskot.net>
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy 
// at http://www.boost.org/LICENSE_1_0.txt)
//
#include <cassert>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>
#include <boost/utility/string_ref.hpp>
#include <atlstr.h>
#include "high_resolution_timer.hpp"

namespace {

std::size_t const max_iterations = 1000000;

template <typename T>
struct traits;

template <typename T>
struct traits<std::basic_string<T>>
{
    typedef std::basic_string<T> string_type;

    static std::size_t size(string_type const& s)
    {
        return s.size();
    }

    static string_type substr(string_type const& s, std::size_t off, std::size_t n)
    {
        return s.substr(off, n);
    }

    static string_type add(string_type const& s, string_type const& t)
    {
        return s + t;
    }
};

template <typename T>
struct traits<boost::basic_string_ref<T, std::char_traits<T>>>
{
    typedef boost::basic_string_ref<T, std::char_traits<T>> string_type;

    static std::size_t size(string_type const& s)
    {
        return s.size();
    }

    static string_type substr(string_type const& s, std::size_t off, std::size_t n)
    {
        return s.substr(off, n);
    }

    static std::basic_string<T> add(string_type const& s, string_type const& t)
    {
        return s.to_string() + t.to_string();
    }
};

#ifdef _MSC_VER
template <typename T>
struct traits<CStringT<T, ATL::StrTraitATL<T, ATL::ChTraitsCRT<T>>>>
{
    typedef CStringT<T, ATL::StrTraitATL<T, ATL::ChTraitsCRT<T>>> string_type;

    static std::size_t size(string_type const& s)
    {
        return static_cast<std::size_t>(s.GetLength());
    }

    static string_type substr(string_type const& s, std::size_t off, std::size_t n)
    {
        return s.Mid(off, n);
    }
    
    static string_type add(string_type const& s, string_type const& t)
    {
        return s + t;
    }
};
#endif

template<typename T>
struct test_substr
{
    template <typename U>
    void operator()(U const& s)
    {
        auto r = traits<T>::substr(std::get<0>(s), std::get<1>(s), std::get<2>(s));
        assert(traits<T>::size(r) == std::get<2>(s));
    }
};

template<typename T>
struct test_add
{
    template <typename U>
    void operator()(U const& s)
    {
        auto r = traits<T>::add(std::get<0>(s), std::get<0>(s));
        assert(traits<T>::size(r) == traits<T>::size(std::get<0>(s)) * 2);
    }
};

template <typename Data, typename Operation>
void benchmark(char const* const name, std::size_t iterations, Data const& data, Operation op)
{
    util::high_resolution_timer t;
    for (decltype(iterations) i = 0; i < iterations; ++i)
    {
        for (auto const& s: data)
            op(s);
    }
    auto const e = t.elapsed();
    std::cout << name << ":\t" << e << std::endl;
}

typedef std::tuple<char const* const, std::size_t, std::size_t> test_string;
typedef std::tuple<wchar_t const* const, std::size_t, std::size_t> test_wstring;

} // unnamed namespace

int main()
{
    {
        std::vector<test_string> data;
        data.emplace_back("abcdefghijklmnopqrstuvwxyz", 2, 24);
        data.emplace_back("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", 26, 3);
        data.emplace_back("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz", 26, 26);
        data.emplace_back("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", 0, 3*26);

        benchmark("substr(CStringA)", max_iterations, data, test_substr<CStringA>());
        benchmark("substr(string)", max_iterations, data, test_substr<std::string>());
        benchmark("substr(string_ref)", max_iterations, data, test_substr<boost::string_ref>());

        benchmark("add(CStringA)", max_iterations, data, test_add<CStringA>());
        benchmark("add(string)", max_iterations, data, test_add<std::string>());
        benchmark("add(string_ref)", max_iterations, data, test_add<boost::string_ref>());
    }

    {
        std::vector<test_wstring> data;
        data.emplace_back(L"abcdefghijklmnopqrstuvwxyz", 2, 24);
        data.emplace_back(L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", 26, 3);
        data.emplace_back(L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz", 26, 26);
        data.emplace_back(L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", 0, 3*26);
        
        benchmark("substr(CString)", max_iterations, data, test_substr<CString>());
        benchmark("substr(wstring)", max_iterations, data, test_substr<std::wstring>());
        benchmark("substr(wstring_ref)", max_iterations, data, test_substr<boost::wstring_ref>());

        benchmark("add(CString)", max_iterations, data, test_add<CString>());
        benchmark("add(wstring)", max_iterations, data, test_add<std::wstring>());
        benchmark("add(wstring_ref)", max_iterations, data, test_add<boost::wstring_ref>());
    }

    return 0;
}

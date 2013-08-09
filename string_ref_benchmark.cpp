//
// Copyright (C) 2013 Mateusz Loskot <mateusz@loskot.net>
// Copyright (c) 2011-2013 Adam Wulkiewicz, Lodz, Poland.
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

namespace stds {
    std::string extract_part(std::string s, std::size_t off, std::size_t n)
    {
        return s.substr(off, n);
    }
    std::string extract_part_ref(std::string const& s, std::size_t off, std::size_t n)
    {
        return s.substr(off, n);
    }
}

namespace mfcs {
    CString extract_part(CString s, std::size_t off, std::size_t n)
    {
        return s.Mid(int(off), int(n));
    }
    CString extract_part_ref(CString const& s, std::size_t off, std::size_t n)
    {
        return s.Mid(int(off), int(n));
    }
}

namespace bsref {
    boost::string_ref extract_part(boost::string_ref s, std::size_t off, std::size_t n)
    {
        return s.substr(int(off), int(n));
    }
    boost::string_ref extract_part_ref(boost::string_ref const& s, std::size_t off, std::size_t n)
    {
        return s.substr(int(off), int(n));
    }
}

namespace {
    std::size_t max_iterations = 1000000;

    template <typename Data, typename Operation>
    void benchmark(char const* const name, std::size_t iterations, Data const& data, Operation op)
    {
        util::high_resolution_timer t;
        for (decltype(iterations) i = 0; i < iterations; ++i)
        {
            for (auto const& s: data)
            {
                auto r = op(std::get<0>(s), std::get<1>(s), std::get<2>(s));
                assert(get_size(r) == std::get<2>(s));
            }
        }
        auto const e = t.elapsed();
        std::cout << name << ":\t" << e << std::endl;
    }

    template <typename T>
    std::size_t get_size(T const& s) { return s.size(); }

    std::size_t get_size(CString const& s) { return std::size_t(s.GetLength()); }

    template <typename T>
    void print(std::ostream& os, T const& t)
    {
        os << t << std::endl;
    }
}

int main()
{
    using std::get;
    typedef std::tuple<char const* const, std::size_t, std::size_t> test_string;

    std::vector<test_string> data;
    data.emplace_back("abcdefghijklmnopqrstuvwxyz", 2, 24);
    data.emplace_back("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", 26, 3);
    data.emplace_back("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz", 26, 26);
    data.emplace_back("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", 0, 3*26);
    
    benchmark("extract_part(std::string)", max_iterations, data, stds::extract_part);
    benchmark("extract_part_ref(std::string)", max_iterations, data, stds::extract_part_ref);
    benchmark("extract_part(CString)", max_iterations, data, mfcs::extract_part);
    benchmark("extract_part_ref(CString)", max_iterations, data, mfcs::extract_part_ref);
    benchmark("extract_part(string_ref)", max_iterations, data, bsref::extract_part);
    benchmark("extract_part_ref(string_ref)", max_iterations, data, bsref::extract_part_ref);
}

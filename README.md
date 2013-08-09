string_ref_benchmark
====================

Simple benchmark comparing string operations for std::string, ATL/MFC CString and boost::string_ref

Results
-------

Number of iterations: 1000000

### Visual C++ 11.0 (32-bit build)

HW: Intel(R) Xeon(R) CPU E5-2687W 0 @ 3.10GHz, 16 GB RAM; OS: Windows 7 64-bit
SW: Visual Studio 2012

```
extract_part(std::string):      0.831809
extract_part_ref(std::string):  0.818625
extract_part(CString):          1.13124
extract_part_ref(CString):      1.10292
extract_part(string_ref):       0.167021
extract_part_ref(string_ref):   0.165776
```
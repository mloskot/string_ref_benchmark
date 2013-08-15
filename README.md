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
substr(CStringA):       1.04746
substr(string):         0.80593
substr(string_ref):     0.15586

add(CStringA):          1.82378
add(string):            1.49661
add(string_ref):        1.48350

substr(CString):        0.97871
substr(wstring):        0.76421
substr(wstring_ref):    0.15399

add(CString):           1.88931
add(wstring):           1.53090
add(wstring_ref):       1.51123
```
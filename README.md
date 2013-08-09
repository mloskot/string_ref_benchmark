string_ref_benchmark
====================

Simple benchmark comparing string operations for std::string, ATL/MFC CString and boost::string_ref

Results
-------

### Visual C++ 11.0 (32-bit build)

HW: Intel(R) Xeon(R) CPU E5-2687W 0 @ 3.10GHz, 16 GB RAM; OS: Windows 7 64-bit
SW: Visual Studio 2012

```
extract_part(std::string):      0.0107073
extract_part_ref(CString):      0.00838718
extract_part_ref(CString):      0.0132433
extract_part_ref(std::string):  0.0103805
extract_part(string_re):        0.00179266
extract_part_ref(string_re):    0.00177578
```
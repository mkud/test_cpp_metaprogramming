[![Build Status](https://travis-ci.com/mkud/test_Matrix.svg?branch=master)](https://travis-ci.com/mkud/test_Matrix)

# The test task with a demonstration of C++ metaprogramming

## The task
To implement analog of the `boost::mpl::quoteN` for any N (N >= 1) with C++17 as `test::quote`.
Description of `boost::mpl::quote` you can see here - [https://www.boost.org/doc/libs/1_71_0/libs/mpl/doc/refmanual/quote.html](https://www.boost.org/doc/libs/1_71_0/libs/mpl/doc/refmanual/quote.html)
## Description
This project is an eclipse project - you can import it. I used version 2019-09 R (4.13.0) + CDT v9.9.0 Linux. your compiler should support the C++17 standard
file description
## Files/Directories description
- Eclispe setting:
 	- .settings;
	- .cproject;
	- .project;
- Debug - directory for building the application. To build you need to enter this directory and run `make`. Result will named `test_cpp_metaprogramming`;
- `src\test_cpp_metaprogramming.cpp` - the main source file.  

#include <type_traits>
#include <iostream>

// The task:
// to implement analog of the boost::mpl::quoteN for any N (N >= 1) with C++17 as test::quote
// Description of boost::mpl::quote you can see here - https://www.boost.org/doc/libs/1_71_0/libs/mpl/doc/refmanual/quote.html

// The new implementation quote should to be in "test" namespace
namespace test {

// 3 underlying templates are using to check
// does a metafunction have a type definition or not
// has_type<metafunc>::has = true; if "metafunc" has typedef X type
// has_type<metafunc>::has = false; otherwise
template<typename T>
using void_t = void;

template<typename T, typename = void>  // ::type is absent
struct has_type {
	static constexpr bool has = false;
};

template<typename T>
struct has_type<T, void_t<typename T::type>> { // ::type is present
	static constexpr bool has = true;
};

//-------------------------------
//-------------------------------
// 2 underlying templates are using for "apply" implementation (see boost::mpl::quote description)
// it got bool value to choose what kind of metaclass should be returned
// true - when metafunction has a nested type member ::type
// false - otherwise
template<typename T, bool has_type_>
struct apply_impl {
	typedef typename T::type type;
};

template<typename T>
struct apply_impl<T, false> {
	typedef T type;
};
//-------------------------------
//-------------------------------
// quote main implementation
template<template<typename ...Args> class f>
struct quote {
	template<typename ...Args> struct apply: apply_impl<f<Args ...>, has_type<f<Args ...>>::has> {
	};
};
// finish of the "test" namespace
}

//Unit testing part
// we should to check 1- and 3-ary functions with- and without- type definition inside
// with 30-ary should to work also...I think...
//-------------------------------
//-------------------------------
//test metafunction defenitions
template<typename T1>
struct test_one_ary {
	typedef T1 type;
};

template<typename T1>
struct test_one_ary_without_type {
};

//
template<typename T1, typename T2, typename T3>
struct test_three_ary {
	typedef T2 type;
};

template<typename T1, typename T2, typename T3>
struct test_three_ary_without_type {
};

//-------------------------------
//-------------------------------
//test implements. Divide 2 parts:
//1. test base helpers
//2. test main functional
int main() {
//1. test base helpers.
//a. test::has_type
	std::cout << "test_one_ary has a nested type member ::type so has_type::has should return true (or 1) - result="
			<< test::has_type<test_one_ary<int>>::has << std::endl;
	std::cout
			<< "test_one_ary_without_type hasn't a nested type member ::type so has_type::has should return false (or 0) - result="
			<< test::has_type<test_one_ary_without_type<int>>::has << std::endl;
//b. test::apply_impl
//compile-time test. If compiled - all is OK

//test_one_ary has nested ::type so pass true as second parameter
	typedef test::apply_impl<test_one_ary<double>, true> test2_check_type;
	static_assert(std::is_same<test2_check_type::type, test_one_ary<double>::type>::value);

//test_one_ary_without_type hasn't nested ::type so pass false as second parameter
	typedef test::apply_impl<test_one_ary_without_type<double>, false> test2_2_check_type;
	static_assert(std::is_same<test2_2_check_type::type, test_one_ary_without_type<double>>::value);

//-------------------------------
//-------------------------------
//2. test main functional
//compile-time test. If compiled - all is OK

//test3_1 = double because test_one_ary::type is T1
	typedef test::quote<test_one_ary>::apply<double>::type test3_1;
	static_assert(std::is_same<test3_1, double>::value);

//test3_2 = metafunction because test_one_ary_without_type hasn't nested ::type
	typedef test::quote<test_one_ary_without_type>::apply<double>::type test3_2;
	static_assert(std::is_same<test3_2, test_one_ary_without_type<double>>::value);
//-------------------------------
//test4_1 = int because test_three_ary::type is T2 (second parameter of the func)
	typedef test::quote<test_three_ary>::apply<double, int, float>::type test4_1;
	static_assert(std::is_same<test4_1, int>::value);

//test4_2 = metafunction because test_three_ary_without_type hasn't nested ::type
	typedef test::quote<test_three_ary_without_type>::apply<double, int, float>::type test4_2;
	static_assert(std::is_same<test4_2, test_three_ary_without_type<double, int, float>>::value);

	return 0;
}

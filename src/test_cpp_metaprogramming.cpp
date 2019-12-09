#include <type_traits>

// The task:
// to implement analog of the boost::mpl::quoteN for any N (N >= 1) with C++17 as test::quote
// Description of boost::mpl::quote you can see here - https://www.boost.org/doc/libs/1_71_0/libs/mpl/doc/refmanual/quote.html

// The new implementation quote should to be in "test" namespace
namespace test {

// 2 underlying templates are using for "apply" implementation (see boost::mpl::quote description)
// void_t approach is used 
// does a metafunction have a type definition or not
// return metaclass with metafunction::type when metafunction has a nested type member ::type
// and metaclass with metafunction as type otherwise

template<typename T, typename = void>  // ::type is absent
struct apply_impl {
	using type = T;
};

template<typename T>
struct apply_impl<T, std::void_t<typename T::type>> { // ::type is present
	using type = typename T::type;
};

//-------------------------------
//-------------------------------
// quote main implementation
template<template<typename ...Args> class f>
struct quote {
	template<typename ...Args>
	using apply = apply_impl<f<Args ...>>;

	template<typename ... ArgsT>
	using apply_t = typename apply<ArgsT ...>::type;
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
	using type = T1;
};

template<typename T1>
struct test_one_ary_without_type {
};

//
template<typename T1, typename T2, typename T3>
struct test_three_ary {
	using type = T2;
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

//b. test::apply_impl
//compile-time test. If compiled - all is OK

//test_one_ary has nested ::type so pass true as second parameter
	typedef test::apply_impl<test_one_ary<double>> test2_check_type;
	static_assert(std::is_same<test2_check_type::type, test_one_ary<double>::type>::value);

//test_one_ary_without_type hasn't nested ::type so pass false as second parameter
	typedef test::apply_impl<test_one_ary_without_type<double>> test2_2_check_type;
	static_assert(std::is_same<test2_2_check_type::type, test_one_ary_without_type<double>>::value);

//-------------------------------
//-------------------------------
//2. test main functional
//compile-time test. If compiled - all is OK

//test3_1 = double because test_one_ary::type is T1
	typedef test::quote<test_one_ary>::apply_t<double> test3_1;
	static_assert(std::is_same_v<test3_1, double>);

//test3_2 = metafunction because test_one_ary_without_type hasn't nested ::type
	typedef test::quote<test_one_ary_without_type>::apply_t<double> test3_2;
	static_assert(std::is_same_v<test3_2, test_one_ary_without_type<double>>);
//-------------------------------
//test4_1 = int because test_three_ary::type is T2 (second parameter of the func)
	typedef test::quote<test_three_ary>::apply_t<double, int, float> test4_1;
	static_assert(std::is_same_v<test4_1, int>);

//test4_2 = metafunction because test_three_ary_without_type hasn't nested ::type
	typedef test::quote<test_three_ary_without_type>::apply_t<double, int, float> test4_2;
	static_assert(std::is_same_v<test4_2, test_three_ary_without_type<double, int, float>>);

	return 0;
}

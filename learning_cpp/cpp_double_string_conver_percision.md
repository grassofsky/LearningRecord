# C++ string double互转精度问题

double转化为string，可以参见：https://www.cnblogs.com/chorulex/p/7660187.html。

string转化为double，可以参见：http://www.cplusplus.com/reference/string/stod/， https://www.cnblogs.com/coolbear/archive/2013/02/26/2933333.html

```c++
#include <iostream>
#include <stdio.h>
#include <sstream>
#include <limits>
#include <string>
#include <cassert>

std::string to_string_with_high_precision(double value, int precision = 20)
{
	std::stringstream ss;
	ss.precision(precision);

	ss << value;
	return ss.str();
}

void double_to_string_test()
{
	std::string strValue = "111.1111111111111111111111111111";
	double value = 111.1111111111111111111111111111;
	
	// original
	std::cout << "Original value: " << strValue << std::endl;
	
	// using to_string
	std::cout << "std::to_string: " << std::to_string(value) << std::endl;
	
	// using stringstream
	std::stringstream ss1;
	ss1 << value;
	std::cout << "stringstream  : " << ss1.str() << std::endl;
	
	// using stringstream with digits10 precision
	std::stringstream ss2;
	ss2.precision(std::numeric_limits<double>::digits10);
	ss2 << value;
	std::cout << "stringstream digits10: " << ss2.str() << std::endl;
}

void string_to_double_test()
{
	std::string strValue = "111.1111111111111111111111111111";
	double value = 111.1111111111111111111111111111;
	
	std::cout << "original:                               " << strValue << std::endl;
	
	double value1 = atof(strValue.c_str());
	std::cout << "atof:                                   " << to_string_with_high_precision(value1, strValue.length()) << std::endl;
	
	double value2;
	std::stringstream ss(strValue);
	ss.precision(strValue.length());
	ss >> value2;
	
	double value3;
	std::stringstream ss2(strValue);
	ss2 >> value3;

    // all assert passed
	assert(value2 == value1);
	assert(value1 == value);
	assert(value3 == value);
	std::cout << "stringstream precision with str length: " << to_string_with_high_precision(value2, strValue.length()) << std::endl;
	std::cout << "stringstream                          : " << to_string_with_high_precision(value3, strValue.length()) << std::endl;
}

int main()
{
	std::cout << "------double to string test------" << std::endl;
	double_to_string_test();
	
	std::cout << "\n" << "------string to double test------" << std::endl;
	string_to_double_test();
}
```

输出结果为：

```
------double to string test------
Original value: 111.1111111111111111111111111111
std::to_string: 111.111111
stringstream  : 111.111
stringstream digits10: 111.111111111111

------string to double test------
original:                               111.1111111111111111111111111111
atof:                                   111.11111111111111426907882560045
stringstream precision with str length: 111.11111111111111426907882560045
stringstream                          : 111.11111111111111426907882560045
```

## 小结

- 当double转换成string的时候回有精度损失的问题产生。

- 通常情况下string转换成double的时候，引入的精度损失和double的表达直接相关，而与小数点位数的不同无关。
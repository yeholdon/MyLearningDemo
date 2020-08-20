#include <cstddef>
#include <string>
#include <iostream>
class TextBlock
{
public:
	TextBlock(const std::string &s);
	~TextBlock();
	/* 两个函数如果只是常量性(constness)不同，可以被重载 */
	// const 函数表示函数不修改对象的内容，所以返回值也必须用const &，否则可以用返回值修改对象
	const char& operator[](std::size_t position) const
	{
		std::cout << "This is non-const operator[]" << std::endl;
		return text[position];
	}
	// 如果只是返回char则xb[0] = 'x'会报错——内置类型的返回值无法直接改动
	//char& operator[](std::size_t position)
	//{
	//	std::cout << "This is const operator[]" << std::endl;
	//	return text[position];
	//}

	// 当const和non-const成员函数有着实质等价的实现时，令non-const版本调用const版本可以避免代码重复
	char& operator[](std::size_t position)
	{
		return
			const_cast<char&>( //再将返回的const字符转成non-const
				static_cast<const TextBlock&>(*this) // 先把non-const对象转成const,这样调用的就是const版的[]()
				[position]
		);
	}

	void print_str()
	{
		std::cout << text << std::endl;
		std::cout << v << std::endl;
	}
private:
	std::string text;
	int v;
};

TextBlock::TextBlock(const std::string &s) : text(s)
{
}

TextBlock::~TextBlock()
{
}
//值初始化，在没有默认构造函数的情况下，会执行默认初始化，而对自定义类型来说默认初始化就是调用默认构造函数，因此报错。
//TextBlock t = TextBlock(); 
int main() 
{
	// 对象是否const 与其成员是否有const对象无关
	TextBlock tb("Hello");
	// 调用non-const operator[]
	std::cout << tb[0] << std::endl;
	// tb[0] = 'h';如果返回的是char会报错
	const TextBlock ctb("const Hello");
	// 调用const operator[], 调用哪个重载版本取决于TextBlock对象本身是否是const
	std::cout << ctb[0] << std::endl;

	
	//t.print_str();
	return 0;
}
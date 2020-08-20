#include <cstddef>
#include <string>
#include <iostream>
class TextBlock
{
public:
	TextBlock(const std::string &s);
	~TextBlock();
	/* �����������ֻ�ǳ�����(constness)��ͬ�����Ա����� */
	// const ������ʾ�������޸Ķ�������ݣ����Է���ֵҲ������const &����������÷���ֵ�޸Ķ���
	const char& operator[](std::size_t position) const
	{
		std::cout << "This is non-const operator[]" << std::endl;
		return text[position];
	}
	// ���ֻ�Ƿ���char��xb[0] = 'x'�ᱨ�����������͵ķ���ֵ�޷�ֱ�ӸĶ�
	//char& operator[](std::size_t position)
	//{
	//	std::cout << "This is const operator[]" << std::endl;
	//	return text[position];
	//}

	// ��const��non-const��Ա��������ʵ�ʵȼ۵�ʵ��ʱ����non-const�汾����const�汾���Ա�������ظ�
	char& operator[](std::size_t position)
	{
		return
			const_cast<char&>( //�ٽ����ص�const�ַ�ת��non-const
				static_cast<const TextBlock&>(*this) // �Ȱ�non-const����ת��const,�������õľ���const���[]()
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
//ֵ��ʼ������û��Ĭ�Ϲ��캯��������£���ִ��Ĭ�ϳ�ʼ���������Զ���������˵Ĭ�ϳ�ʼ�����ǵ���Ĭ�Ϲ��캯������˱���
//TextBlock t = TextBlock(); 
int main() 
{
	// �����Ƿ�const �����Ա�Ƿ���const�����޹�
	TextBlock tb("Hello");
	// ����non-const operator[]
	std::cout << tb[0] << std::endl;
	// tb[0] = 'h';������ص���char�ᱨ��
	const TextBlock ctb("const Hello");
	// ����const operator[], �����ĸ����ذ汾ȡ����TextBlock�������Ƿ���const
	std::cout << ctb[0] << std::endl;

	
	//t.print_str();
	return 0;
}
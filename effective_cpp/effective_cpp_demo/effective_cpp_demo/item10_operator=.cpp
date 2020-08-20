#include <iostream>
class Widget
{
public:
	Widget();
	Widget(int a);
	~Widget();
	Widget& operator=(const Widget& rhs);

	int v;
};

Widget::Widget() : v(0)
{
}
Widget::Widget(int a) : v(a)
{
}
Widget::~Widget()
{
}

Widget& Widget::operator=(const Widget& rhs)
{
	v = rhs.v;
	return *this;
}


int main()
{
	Widget w, a(2), b(4);
	(w = a) = b;
	std::cout << w.v << " " << a.v << " " << b.v << " " << std::endl;
	return 0;
}
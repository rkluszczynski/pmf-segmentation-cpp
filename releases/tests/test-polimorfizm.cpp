#include <iostream>

template <class TYPE> class A
{
    private :
        virtual void wewn(TYPE a) {
            std::cout << "A::wewn() " << a << std::endl;
        }
    public :
        void zewn(TYPE a) {
            wewn(a);
            std::cout << "A::zewn()" << std::endl;
        }
};

template <class REAL> class B  {
    REAL xx;

	friend std::ostream & operator << (std::ostream & out, const B<REAL> & b)
	{
		out << "B::xx == " << b.xx;
		return out;
	}
};

template <class T> class C : public A<B<T> >
{
    private :
        virtual void wewn(B<T> c) {
            std::cout << "C::wewn() " << c << std::endl;
        }
};


int main()
{
	std::cout << "Hello world!" << std::endl;

	A<int> a;
	B<long> b;
	C<long> c;

	a.zewn(7);
	std::cout << b << std::endl;
	c.zewn(b);

	return 0;
}

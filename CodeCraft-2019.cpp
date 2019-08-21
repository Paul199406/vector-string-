#define _SCL_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
#include<sstream>
#include<memory>

using namespace std;


class strVec
{
public:

	strVec() :elememts(nullptr), first_free(nullptr), cap(nullptr){}

	strVec(const strVec &);//�������캯��
	strVec& operator=(const strVec&);
	string& operator[](const int i);
	~strVec();//

	void push_back(const string&);

	size_t size() const { return first_free - elememts; }

	size_t capacity() const { return cap - elememts; }

	string * begin() const { return elememts; }

	string * end() const { return first_free; }

private:

	
	allocator<string> alloc;//����Ԫ��

	//�����Ԫ��ʱʹ��
	void chk_n_alloc()
	{
		if (size() == capacity())
		{
			reallocate();
		}
	}

	//���ߺ��������������캯������ֵ���캯��������������ʹ��
	pair<string *, string *> alloc_n_copy(const string *, const string *);

	void free();//����Ԫ�ز��ͷ��ڴ�
	void reallocate();//��ȡ�����ڴ沢��������Ԫ��
	string *elememts;
	string *first_free;
	string *cap;
};

void strVec::push_back(const string& s)
{
	chk_n_alloc();

	alloc.construct(first_free++, s);
}

pair<string *, string *> strVec::alloc_n_copy(const string *b, const string *e)
{
	auto data = alloc.allocate(e - b);

	return{ data, uninitialized_copy(b, e, data) };
}

void strVec::free()
{
	if (elememts)
	{
		for (auto p = first_free; p != elememts;)
		{
			alloc.destroy(--p);
		}

		alloc.deallocate(elememts, cap - elememts);
	}
}


strVec::strVec(const strVec&s)
{
	auto newdata = alloc_n_copy(s.begin(), s.end());
	
	elememts = newdata.first;
	cap = first_free = newdata.second;


}

strVec::~strVec()
{
	free();
}

strVec& strVec::operator=(const strVec&s)
{
	auto newdata = alloc_n_copy(s.begin(), s.end());

	free();
	elememts = newdata.first;
	cap = first_free = newdata.second;

	return *this;

}
void strVec::reallocate()
{
	auto newcapacity = size() ? 2 * size() : 1;
	auto newdata = alloc.allocate( newcapacity);

	auto dest = newdata;
	auto elem = elememts;
	for (size_t i = 0; i != size(); ++i)
	{
		alloc.construct(dest++,std::move(*elem++));

	}
	free();
	elememts = newdata;
	first_free = dest;
	cap = elememts + newcapacity;
}


string& strVec::operator[](const int i)
{
	if (i < size())
		return elememts[i];

}
int main()
{
	strVec sv;
	sv.push_back("I");
	cout << sv.capacity() << endl;

	sv.push_back("m");
	cout << sv.capacity() << endl;

	sv.push_back("a");
	cout << sv.capacity() << endl;

	sv.push_back("boy");
	cout << sv.capacity() << endl;

	size_t size= sv.size();
	for (int i = 0; i < size; ++i)
	{
		cout << sv[i] << endl;
	}
	return 0;
}
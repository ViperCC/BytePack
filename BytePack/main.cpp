#include "BytePack.h"

class AA
{
private:
	int x = 0;
	float y = 0;
public:
	AA(int _x, float _y) : x(_x), y(_y){}

	void Print()
	{
		printf("AA x:%d y:%f\n", x, y);
	}
};

int main()
{
	int a = 10;
	double b = 20.323;
	const char* c = "wocao";
	AA d(123, 4.44555f);

	char bytes[128];
	int ti = 12;
	float tf = 1.234f;
	memcpy(bytes, &ti, sizeof(int));
	memcpy(bytes + sizeof(int), &tf, sizeof(float));

	char* data = BytePack::ToBytes(a, b, c, d, BytePack::Bytes(bytes, sizeof(int) + sizeof(float)));

	int a1;
	double b1;
	char c1[128];
	AA d1(0, 0);
	AA d2(0, 0);
	if (BytePack::Unpack(data, &a1, &b1, &c1, &d1, &d2))
	{
		printf("Unpack Success\n");
		printf("%d %f %s\n", a1, b1, c1);
		d1.Print();
		d2.Print();
	}
	else
	{
		printf("Unpack Failed\n");
	}

	delete[] data;

	return 0;
}
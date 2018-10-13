#include <iostream>
#include <string.h>

using namespace std;

class A
{
public:
    char data[12];
    char *dataP;
    int intData;
    A()
    {
	memcpy(data,"good",5);
	dataP = new char[5];
	memcpy(dataP,"goodP",5);
	cout << "A constructor" << endl;
    };
    void printData()
    {
	cout << data << endl;
	cout << dataP << endl;
    }
};

int main()
{
    A a;
    a.printData();
    A b = a;
    b.printData();
}

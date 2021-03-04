#include "top.h"

int main(){
	data_t a = 5;
	data_t b = 6;
	data_t c;

	add(a,b,c);

	int err = 0;

	if (c != (a+b)){
		err +=1;
		cout << "Error, HW c =" << c <<endl;
	}
	return err;
}

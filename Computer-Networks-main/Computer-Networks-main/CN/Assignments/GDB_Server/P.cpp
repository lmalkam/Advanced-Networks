#include<iostream>
using namespace std;

int addition(int num1, int num2){
	return num1 + num2;
}

int subtraction(int num1, int num2){
	return num1 - num2;
}

int multiplication(int num1, int num2){
	return num1 * num2;
}

int main(){
	int num1, num2;
	
	cin>>num1 >> num2;
	cout<<addition(num1, num2)<<"\n";

	cin>>num1 >> num2;
	cout<<subtraction(num1, num2)<<"\n";

	cin>>num1 >> num2;
	cout<<multiplication(num1, num2)<<"\n";
	return 0;
}
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <bits/stdc++.h>
#pragma GCC optimize ("Ofast")
#pragma GCC optimize ("unroll-loops")
#pragma GCC target ("avx2")
using namespace std;
int fibo(int a) {
	if(a<=1) return 1;
	return fibo(a-1)+fibo(a-2);
}

int main() {
	for(int i = 0; i<50; i++) {
		clock_t st = clock();
		fibo(35);
		cout<<clock()-st<<"\n";
	}
}

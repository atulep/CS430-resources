
/* 
 * Generate all primes between 1 and n, where n is a command line argument.
 * Based on Eratosthenes number sieve algorithm.
 *
 * Start with all the odd numbers less than n; then successively strike out
 * the multiples (p_k)^2, p_k (p_k + 2), p_k (p_k + 4), ..., of the kth
 * prime number p_k for k=2,3,4,... until reaching a prime p_k with
 * (p_k)^2 > n.
 * 
 * We assume that n is even.
 */ 
#include <iostream>
#include <stdlib.h>

using namespace std;


int main(int argc, char **argv)
{
	if (argc <  2) {

			cerr << "Usage: " << argv[0] 
					<< " <n> (where n is even) [<debug level>]" << endl;
			exit(1);
	}
	
	int debug_level=0;
	if (argc == 3) {
			debug_level = atoi(argv[2]);
	}

	const int DEBUG = debug_level;
	
	int n = atoi(argv[1]);
	int m = n/2;
	
	
	int *S = new int[m]; // we will use 1..m-1 locations
	
	// initialize S to be {3,5,7,...,2m-1}
	S[0] = 2; //unused location
	if (DEBUG==1) {
			cout << S[0] << endl;
	}

	for (int i = 1; i < m; i++) {
		S[i] = 2*i+1;
	}
	
	int j = 1;
	int k = 1;
	int p = 3;
	int q = 4;
	
	while (q < m) {
		if (S[j] != 0) {
			if (DEBUG==1) {
					cout << S[j] << endl;
			}
			k = q;
			while (k <= m) {
				S[k] = 0;
				k = k + p;
			}
		}
		j = j + 1;
		p = p + 2;
		q = q + 2*p - 2;
	}

	if (DEBUG==1) {
		// output the remaining primes
		for (int i = j; i < m; i++) {
			if (S[i] != 0) {
				cout << S[i] << endl;
			}
		}
	}

	exit(0);
}

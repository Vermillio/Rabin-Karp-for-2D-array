#pragma once
#include <vector>
#include <utility>

using namespace std;

namespace strsrch
{
	class StringSearchEngine
	{

		int SymbolNumDegree = 8; //2^SymbolNumDegree = 256 - symbol number
		long long int HashBase = 9213693951; // simple number for Rabin Karp algo; 2^61-1; 9th Mersenne number; fits 64x architecture
		int hash(int *x, int sz);
		int hash(char *x, int sz);

		string getcd();
		void readFile(string filename, char **buff, int lineSize);
	public:

		vector<pair<int, int>> searchRabinKarp2D(char **haystack, int h1, int w1, char **needle, int h2, int w2);
		void demo();

	};
	ostream & operator<<(ostream & c, vector<pair<int, int>> x);
}
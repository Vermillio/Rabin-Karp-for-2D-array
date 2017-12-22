#include "RabinKarp.h"
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

ostream & strsrch::operator<<(ostream & c, vector<pair<int, int>> x)
{
	for (int i = 0; i < x.size(); ++i)
		c << x[i].first << "-" << x[i].second << " ";
	cout << endl;
	return c;
}


string strsrch::StringSearchEngine::getcd()
{
	wchar_t *w = nullptr;
	wstring ws = _wgetcwd(w, _MAX_PATH);
	return string(ws.begin(), ws.end());
}

void strsrch::StringSearchEngine::readFile(string filename, char **buff, int lineSize)
{
	ifstream fin(filename);
	if (!fin.is_open())
		return;
	fin.seekg(0, ios::beg);
	int sz = fin.tellg();
	fin.seekg(0, ios::end);
	sz = (int)fin.tellg() - sz;
	if (sz % lineSize != 0)
		sz-=sz%lineSize;
	sz /= lineSize;
	buff = new char*[sz];
	for (int i = 0; i < sz; ++i)
		fin.read(buff[i], lineSize);
}

void strsrch::StringSearchEngine::demo()
{
	char **haystack = nullptr, **needle = nullptr;
	int hWidth, hHeight, nWidth, nHeight;
	int choise;

	cout << "INPUT - 2, RANDOM - 1, READ FILE - 0 : " << endl;
	cin >> choise;
	if (choise == 2)
	{
		cout << "Enter height and width : " << endl;
		cin >> hHeight >> hWidth;
		cout << "ENTER TEXT : ";
		haystack = new char*[hHeight];
		string tmp;
		for (int i = 0; i < hHeight; ++i)
		{
			haystack[i] = new char[hWidth];
			cin.ignore(cin.rdbuf()->in_avail());
			getline(cin, tmp);
			tmp.resize(hWidth);
			tmp.copy(haystack[i], hWidth);
		}
		cin.ignore(cin.rdbuf()->in_avail());
		cout << "Enter height and width of pattern : ";
		cin >> nHeight >> nWidth;
		cout << "ENTER PATTERN  : ";
		needle = new char*[nHeight];
		for (int i = 0; i < nHeight; ++i)
		{
			needle[i] = new char[nWidth];
			cin.ignore(cin.rdbuf()->in_avail());
			getline(cin, tmp);
			tmp.resize(nWidth);
			tmp.copy(needle[i], nWidth);
		}
	
	}
	else if (choise == 1)
	{
		srand(clock());
		cout << "Enter height and width of text : ";
		cin >> hHeight;
		cin >> hWidth;
		cout << "Enter height and width of pattern : ";
		cin >> nHeight;
		cin >> nWidth;
		haystack = new char*[hHeight];
		needle = new char*[nHeight];

		cout << "TEXT IS : ";

		for (int i = 0; i < hHeight; ++i)
		{
			haystack[i] = new char[hWidth];
			for (int j = 0; j < hWidth; ++j)
			{
				haystack[i][j] = (char)rand();
				cout << haystack[i][j];
			}
			cout << endl;
		}
		cout << endl;
		if (nHeight*nWidth <= 700)
		{
			cout << "PATTERN IS : ";

			for (int i = 0; i < nHeight; ++i)
			{
				needle[i] = new char[nWidth];
				for (int j = 0; j < nWidth; ++j)
				{
					needle[i][j] = (char)rand();
					cout << needle[i][j];
				}
				cout << endl;
			}
		}
		cout << endl;
	}
	else {
		string hfilename = "example.txt";
		string PATH = getcd() + "\\";
		cout << "File must be stored here " << PATH << endl;
		cout << "Enter filename for text (example.txt by default): " << endl;
		cin >> hfilename;
		cout << "Enter number of symbols per line: ";
		cin >> hWidth;
		readFile(PATH + hfilename, haystack, hWidth); //!!!!!!!!1
		string nfilename;
		cout << "Enter filename for pattern: " << endl;
		cin >> nfilename;
		cout << "Enter number of symbols per line : ";
		cin >> nWidth;
		readFile(PATH + nfilename, needle, nWidth);
	}
	vector<pair<int, int>> res;

	time_t t = clock();
	res = searchRabinKarp2D(haystack, hHeight, hWidth, needle, nHeight, nWidth);
	t = clock() - t;
	cout << "Rabin Karp search worked " << t << "clocks." << endl;
	cout << "RESULT <row-column> (begin from 0) : " << res << endl;
	cout << endl;

	for (int i = 0; i < hHeight; ++i)
		delete[] haystack[i];
	delete[] haystack;
	for (int i = 0; i < nHeight; ++i)
		delete[] needle[i];
	delete[] needle;
}



vector<pair<int, int>> strsrch::StringSearchEngine::searchRabinKarp2D(char ** haystack, int h1, int w1, char ** needle, int h2, int w2)
{
	vector<pair<int, int>> res;

	int h = 1;

	for (int i = 0; i < w2 - 1; ++i)
		h = (h << SymbolNumDegree) % HashBase;

	int *needleHash = new int[h2];

	for (int i = 0; i < h2; ++i)
	{
		needleHash[i] = hash(needle[i], w2);
		while (needleHash[i] < 0)
			needleHash[i] += HashBase;
	}
	int hp = hash(needleHash, h2);
	if (hp < 0)
		hp += HashBase;

	int** tmpHash = new int*[w1 - w2 + 1];
	for (int i = 0; i <= w1 - w2; ++i)
		tmpHash[i] = new int[h1];

	//CALC PRIMARY HASH
	for (int i = 0; i < h1; ++i)
	{
		tmpHash[0][i] = hash(haystack[i], w2);
		while (tmpHash[0][i] < 0)
			tmpHash[0][i] += HashBase;
		for (int j = 1; j <= w1 - w2; ++j)
		{
			tmpHash[j][i] = (((tmpHash[j - 1][i] - haystack[i][j - 1] * h) << SymbolNumDegree) + haystack[i][j + w2 - 1]) % HashBase;
			while (tmpHash[j][i] < 0)
				tmpHash[j][i] += HashBase;
		}
	}

	//for (int i = 0; i <= w1 - w2; ++i)
	//{		
	//	for (int j = 0; j < h1; ++j)
	//		cout << tmpHash[i][j] << " ";
	//	cout << endl;
	//}

	//CALC SECONDARY HASH AND CHECK FOR EQUALITY
	for (int i = 0; i <= w1 - w2; ++i)
	{
		int cur = hash(tmpHash[i], h2);
		while (cur < 0)
			cur += HashBase;
		if (cur == hp)
			res.push_back(pair<int, int>({ 0, i }));
		for (int j = 1; j <= h1 - h2; ++j)
		{
			cur = (((cur - tmpHash[i][j - 1] * h) << SymbolNumDegree) + tmpHash[i][j + h2 - 1]) % HashBase;
			while (cur < 0)
				cur += HashBase;
			if (cur == hp)
				res.push_back(pair<int, int>({ j, i }));
		}
	}

	//DELETION
	delete[] needleHash;
	for (int i = 0; i <= w1-w2; ++i)
		delete[] tmpHash[i];
	delete[] tmpHash;
	return res;
}

int strsrch::StringSearchEngine::hash(int *x, int sz)
{
	int s = x[0];
	for (int i = 1; i < sz; ++i)
	{
		s <<= SymbolNumDegree;
		s += x[i];
		s %= HashBase;

		while (s < 0)
			s += HashBase;
	}
	while (s < 0)
		s += HashBase;
	return s;
}

int strsrch::StringSearchEngine::hash(char *x, int sz)
{
	int s = x[0];
	for (int i = 1; i < sz; ++i)
	{
		s <<= SymbolNumDegree;
		s += x[i];
		s %= HashBase;
		while (s < 0)
			s += HashBase;
	}
	while (s < 0)
		s += HashBase;
	return s;
}


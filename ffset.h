#ifndef F_H
#define F_H

#include <iostream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <map>

#include "terminals.h"

#define SUCCESS 1
#define FAILURE 0

#define for_it(a,b) for( auto a = b.begin() ; a != b.end() ; ++a )
#define hasNil(a) a.find(NIL) != a.end()

using std::map;
using std::vector;
using std::cout;
using std::endl;


class first_follow_set {
public:
	map<string,set<string>> first_set , follow_set;

	first_follow_set( vector<production>& , terminals& );
	void fill_first_set( vector<production>& , terminals& );
	set<string> find_first( vector<string>& );
	void fill_follow_set( vector<production>& , terminals& );
	void print();
	void printContainer( const string& , const map<string,set<string>>& );
	
};



#endif


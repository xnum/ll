#include "ll.h"

ll::ll( vector<production> &ps , terminals &t , first_follow_set &f )
{
	for_it( it , ps )
	{
		set<string> pset = find_predict_set( *it , t , f );

		for_it( iter , pset )
		{
			production &p = table[ it->LHS ][ *iter ];
			if( p.ruleNum != UNINIT_NUM )
			{
				cout << "ERROR find conflict entry(more than one):" << endl;
				cout << "ll table[" << (it->LHS) << "][" << (*iter) << "] already have rule:" << endl;
				cout << p.ruleNum << ":" << p.origin << endl;
				cout << "but I am trying to fill rule:" << endl;
				cout << (it->ruleNum) << ":" << (it->origin) << endl;
			}
			else
			{
				table[ it->LHS ][ *iter ] = *it;
			}
		}
	}
}

void ll::parse( vector<production> &ps , terminals &t , first_follow_set &f , deque<string> context )
{
	vector<string> pool;
	pool.push_back( ps[0].LHS );

	while( !pool.empty() && !context.empty() )
	{
		puts("----Status------------------------------------");
		printStack(pool);
		printDeque(context);

		string nt = pool.back();
		string code = context.front();
		cout << "current Non-terminal:(" << nt << ")\t" << "terminal symbol:(" << code << ")" << endl;
		puts("----Action------------------------------------");

		if( nt == code )
		{
			cout << "- Match symbol (" << code << ")" << endl;
			pool.pop_back();
			context.pop_front();
		}
		else if( !t.isTerminal(nt) &&
				table[nt].find(code) != table[nt].end() )
		{
			production &p = table[nt][code];
			cout << "- Apply rule (" << p.ruleNum << ")." << p.origin;
			pool.pop_back();

			for( auto it = p.RHS.rbegin() ; it != p.RHS.rend() ; ++it )
			{
				if( *it != NIL )
				{
					pool.push_back( *it );
				}
				else
				{
					cout << "--- Ignore null in rhs" << endl;
				}
			}
		}
		else if( table[nt].find( NIL ) != table[nt].end() || t.isNullable(nt) )
		{
			cout << "- REMOVE null (shouldn't do this)." << endl;
			pool.pop_back();
		}
		else
		{
			puts("- ERROR no action can do. exited.");
			return;
		}
		puts("----------------------------------------------\n");
	}

	while( !pool.empty() )
	{
		string nt = pool.back();
		cout << " WARN: still not empty in stack (" << nt << ")" << endl;
		if( table[nt].find( NIL ) != table[nt].end() || t.isNullable(nt) )
		{
			pool.pop_back();
			cout << "    " << nt << "has been removed." << endl;
		}
		else
		{
			cout << "    " << nt << "is not null. exited." << endl;
			break;
		}
	}

	if( context.size() == 0 && pool.size() == 0 )
		puts("context accepted.");
	else
	{
		puts("context NOT accepted.");
		puts("----Status-----------------------------------");
		printStack(pool);
		printDeque(context);
		puts("---------------------------------------------");
	}
}

set<string> ll::find_predict_set( production &p , terminals &t , first_follow_set &f )
{
	set<string> res;
	set<string> first = f.find_first( p.RHS );

	if( SUCCESS == first.erase( NIL ) )
		res.insert( f.follow_set[p.LHS].begin() , f.follow_set[p.LHS].end() );

	res.insert(first.begin() , first.end());

	return res;
}

void ll::print()
{
	set<string> column;

	for_it( it , table )
	{
		for_it( iter , it->second )
			column.insert( iter->first );
	}

	for( int i = 0 ; i < (column.size()+1)*9-1 ; ++i )
		putchar('-');
	putchar('\n');

	cout << "|\t\t";
	for_it( iter , column )
		cout << "|" << (*iter) << "\t";
	cout << "|" << endl;

	for( int i = 0 ; i < (column.size()+1)*9-1 ; ++i )
		putchar('-');
	putchar('\n');


	for_it( it , table )
	{
		cout << "|" << (it->first) << "\t\t";
		map<string,production>& smap = it->second;
		for_it( iter , column )
		{
			auto t = smap.find( *iter );
			if( t != smap.end() )
				cout << "|" << (smap[*iter].ruleNum) << "\t";
			else
				cout << "|" << "    "   << "\t";
		}
		cout << "|" << endl;
	}

	for( int i = 0 ; i < (column.size()+1)*9-1 ; ++i )
		putchar('-');
	putchar('\n');


}

void ll::printStack( vector<string> &stk )
{
	cout << "Stack :" << endl;
	cout << "bottom [ ";
	for_it( it , stk )
	{
		cout << "\t" << (*it);
	}
	cout << " ] top";
	cout << endl;
}

void ll::printDeque( deque<string> &que )
{
	cout << "Queue :" << endl;
	cout << "front [ ";
	for_it( it , que )
	{
		cout << "\t" << (*it);
	}
	cout << " ] back";
	cout << endl;
}





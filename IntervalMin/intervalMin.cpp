#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <set>
#include <list>
#include <forward_list>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>
#include <memory>
#include <stdexcept>
using namespace std;
#endif /* __PROGTEST__ */


template<typename T_, typename Cmp_ = less<T_>>
class CIntervalMin
{
public:
    using it = typename vector<T_>::const_iterator;

    explicit CIntervalMin(const Cmp_ & n_cmp = Cmp_ ()) : m_cmp(n_cmp){
        this->m_vec = { };
    }
    CIntervalMin(it start, it end, const Cmp_ & n_cmp = Cmp_ ()) {
        this->m_cmp = n_cmp;
        this->m_vec.clear();
        for(; start != end; start++)
            this->m_vec.push_back(*start);
    }


    CIntervalMin& push_back ( const T_& obj){
        this->m_vec.push_back(obj);
        return  *this;
    }

    CIntervalMin& pop_back ( ){
        this->m_vec.pop_back();
        return  *this;
    }

    it begin(){
        return this->m_vec.begin();
    }

    it end(){
        return this->m_vec.end();
    }

    size_t size ( ){
        return this->m_vec.size();
    }

    T_ min (it start, it end){
        if(start == end)
            throw invalid_argument("");
        T_ min =  *min_element(start, end, this->m_cmp);
        return min;
    }

private:
    vector<T_> m_vec;
    Cmp_ m_cmp;
};


#ifndef __PROGTEST__
//-------------------------------------------------------------------------------------------------
class CStrComparator
{
public:
    explicit CStrComparator ( bool byLength = true )
            : m_ByLength ( byLength )
    {
    }
    bool       operator () ( const string & a, const string & b ) const
    {
        return m_ByLength ? a . length () < b . length () : a < b;
    }
private:
    bool       m_ByLength;
};
//-------------------------------------------------------------------------------------------------
bool strCaseCmpFn ( const string & a, const string & b )
{
    return strcasecmp ( a . c_str (), b . c_str () ) < 0;
}
//-------------------------------------------------------------------------------------------------
int main ( )
{
    CIntervalMin <int> a1;
    for ( auto x : initializer_list<int> { 5, 15, 79, 62, -3, 0, 92, 16, 2, -4 } )
        a1 . push_back ( x );

    assert ( a1 . size () == 10 );

    ostringstream oss;
    for ( auto x : a1 )
        oss << x << ' ';
    assert ( oss . str () == "5 15 79 62 -3 0 92 16 2 -4 " );
    assert ( a1 . min ( a1 . begin (), a1 . end () ) == -4 );
    assert ( a1 . min ( a1 . begin () + 2, a1 . begin () + 3 ) == 79 );
    assert ( a1 . min ( a1 . begin () + 2, a1 . begin () + 9 ) == -3 );

    try
    {
        a1 . min ( a1 . begin (), a1 . begin () );
        assert ( "Missing an exception" == nullptr );
    }
    catch ( const invalid_argument & e )
    {
    }
    catch ( ... )
    {
        assert ( "Invalid exception" == nullptr );
    }

    a1 . pop_back ();
    assert ( a1 . size () == 9 );
    a1 . push_back ( 42 );

    assert ( a1 . min ( a1 . begin (), a1 . end () ) == -3 );

    vector<string> words{ "auto", "if", "void", "NULL" };
    CIntervalMin <string> a2 ( words . begin (), words . end () );
    assert ( a2 . min ( a2 . begin (), a2 . end () ) ==  "NULL" );

    CIntervalMin <string, bool(*)(const string &, const string &)> a3 ( words . begin (), words . end (), strCaseCmpFn );
    assert ( a3 . min ( a3 . begin (), a3 . end () ) == "auto" );

    CIntervalMin <string, CStrComparator> a4 ( words . begin (), words . end () );
    assert ( a4 . min ( a4 . begin (), a4 . end () ) == "if" );

    CIntervalMin <string, CStrComparator> a5 ( words . begin (), words . end (), CStrComparator ( false ) );
    assert ( a5 . min ( a5 . begin (), a5 . end () ) == "NULL" );

    CIntervalMin <string, function<bool(const string &, const string &)> > a6 ( [] ( const string & a, const string & b )
                                                                                {
                                                                                    return a > b;
                                                                                } );
    for ( const auto & w : words )
        a6 . push_back ( w );
    assert ( a6 . min ( a6 . begin (), a6 . end () ) == "void" );
    return 0;
}
#endif /* __PROGTEST__ */

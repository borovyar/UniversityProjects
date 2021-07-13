#ifndef __PROGTEST__
#include <cassert>
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <set>
#include <list>
#include <map>
#include <algorithm>
#include <cctype>
using namespace std;

class CResult
{
public:
    CResult     ( const string & name,
                  unsigned int   studentID,
                  const string & test,
                  int            result )
            : m_Name ( name ),
              m_StudentID ( studentID ),
              m_Test ( test ),
              m_Result ( result )
    {
    }
    bool           operator == ( const CResult& other ) const
    {
        return m_Name == other . m_Name
               && m_StudentID == other . m_StudentID
               && m_Test == other . m_Test
               && m_Result == other . m_Result;
    }
    string         m_Name;
    unsigned int   m_StudentID;
    string         m_Test;
    int            m_Result;
};
#endif /* __PROGTEST__ */
class CExam;

class CStudent{
    friend CExam;
private:
    string name;
    set<string> Cards;
public:

    CStudent(const vector<string> & names, const set<string> & Cards) : Cards(Cards){
        name = "";
        setName(names);
    }

    void setName(const vector<string> & names){
        for(const auto & it : names){
            name += it;
            name.push_back(' ');
        }
        name.pop_back();
    }

};

class CExam
{
public:
    static const int SORT_NONE   = 0;
    static const int SORT_ID     = 1;
    static const int SORT_NAME   = 2;
    static const int SORT_RESULT = 3;

    static bool CreateDB( istream & cardMap, map<unsigned int, CStudent> & m_dbs){
        map<unsigned int, CStudent> m_tmp;
        string text;
        while(getline(cardMap, text) && !text.empty()){
            set<string> cards;
            vector<string> fname;
            istringstream buffer (text);
            unsigned int id;
            string name;
            char c1 = 0, c2 = 0, c3 = 0;
            buffer >> ws >> id >> c1;
            if(m_dbs.find(id) != m_dbs.end()){return false;}
            while(true){
                buffer >> c2;
                if(c2 == ':')
                    break;
                else if( isupper(c2) && !name.empty() ){
                    fname.push_back(name);
                    name.clear();
                    name.push_back(c2);
                    continue;
                }
                name.push_back(c2);
            }
            fname.push_back(name);
            while(buffer.tellg() != -1){
                string card;
                while(true){
                    buffer >> ws >> c3;
                    if( c3 == ','){
                        break;
                    }
                    if(buffer.tellg() != -1)
                        card.push_back(c3);
                    else
                        break;
                }
                for(const auto & ch : m_dbs){
                    if(ch.second.Cards.find(card) != ch.second.Cards.end()){ return false;}
                }
                cards.insert(card);
            }
            m_tmp.insert({id, CStudent(fname, cards)});
        }
        m_dbs.insert(m_tmp.begin(), m_tmp.end());
        return true;
    }

    static void sortID (  list<CResult> & m_list ) {
        m_list.sort([](const CResult & obj1, const CResult & obj2){return obj1.m_StudentID < obj2.m_StudentID;});
    }

    static void sortName (  list<CResult> & m_list ) {
        m_list.sort([](const CResult & obj1, const CResult & obj2){return obj1.m_Name < obj2.m_Name;});
    }

    static void sortResult (  list<CResult> & m_list ) {
        m_list.sort([](const CResult & obj1, const CResult & obj2){return obj1.m_Result > obj2.m_Result;});
    }

    bool           Load        ( istream      & cardMap ){
        return CreateDB(cardMap, this->Load_stundent);
    }

    bool           Register    ( const string & cardID,
                                 const string & test ){
        unsigned int tmp = 0;
        for(const auto & ch : this->Load_stundent){
            if(ch.second.Cards.find(cardID) != ch.second.Cards.end()){ tmp = ch.first; }

        }
        if(tmp == 0){ return false;}
        for(auto & ch1 : this->regist){
            if(ch1.first == tmp && ch1.second.find(test) != ch1.second.end()) {
                return false;
            }
            else if(ch1.first == tmp){
                ch1.second.insert(test);
                return true;
            }
        }
        set<string> tests;
        tests.insert(test);
        this->regist.insert({tmp, tests});
        return true;
    }

    bool           Assess      ( unsigned int   studentID,
                                 const string & test,
                                 int            result ){
        auto name = this->Load_stundent.find(studentID);
        if(this->Load_stundent.find(studentID) == this->Load_stundent.end()) { return false; }
        if(this->regist.find(studentID) == this->regist.end()) { return false; }
        auto pos = this->regist.find(studentID);
        auto del = pos->second.find(test);
        if( del == pos->second.end()) { return false; }
        pos->second.erase(del);
        auto passed = this->assess.find(test);
        if(passed != this->assess.end()) {
            for (const auto &it : passed->second)
                if (it.first == studentID) { return false; }
            passed->second.emplace_back(studentID, make_pair(name->second.name, result));
            return true;
        }
        vector<pair<unsigned int, pair<string, int>>> tmp_vec;
        tmp_vec.emplace_back(studentID, make_pair(name->second.name, result));
        this->assess.insert({test,tmp_vec});
        return true;
    }

    list<CResult>  ListTest    ( const string & testName,
                                 int            sortBy ) const{
        list<CResult> tmp;
        auto test = this->assess.find(testName);
        if(test == this->assess.end()) { return tmp; }
        for(const auto & it : test->second){//// name, id, test, result
            tmp.emplace_back(it.second.first, it.first, testName, it.second.second);
        }
        switch(sortBy){
            case 0 :
                break;
            case 1:
                sortID(tmp);
                break;
            case 2:
                sortName(tmp);
                break;
            case 3:
                sortResult(tmp);
                break;
            default:
                return tmp;
        }
        return tmp;
    }

    set<unsigned int> ListMissing ( const string & testName ) const{
        set<unsigned int> tmp;
        for(const auto & it : regist){
            if(it.second.find(testName) != it.second.end()){ tmp.insert(it.first);}
        }
        return tmp;
    }

    /*void printLStudnets(){
        for(const auto & it : Load_stundent){
            cout << it.first << endl;
            cout << "!" << it.second.name << "!" << endl;
            for(const auto & x : it.second.Cards){
                cout << x << endl;
            }
        }
    }

    void printreg(){
        for(const auto & it : regist){
            cout << it.first << " " << endl;
            for(const auto & it2 : it.second){
                cout << it2 << endl;
            }
        }
    }

    void printass(){
        for(const auto & it : assess){
            cout << it.first << endl;
            for(const auto & x : it.second){
                cout << x.first << " " << x.second.first << " " << x.second.second << endl;
            }
        }
    }*/

private:
    map<unsigned int, CStudent> Load_stundent;
    map<unsigned int, set<string>> regist;
    map<string, vector<pair<unsigned int, pair<string, int>>>> assess;
};

#ifndef __PROGTEST__
int main ( )
{
    istringstream iss;
    CExam         m;
    iss . clear ();
    iss . str ( "123456:Smith John:er34252456hjsd2451451, 1234151asdfe5123416, asdjklfhq3458235\n"
                "654321:Nowak Jane: 62wtsergtsdfg34\n"
                "456789:Nowak Jane: okjer834d34\n"
                "987:West Peter Thomas:sdswertcvsgncse\n" );
    assert ( m . Load ( iss ) );
    assert (m . Register ( "62wtsergtsdfg34", "PA2 - #1" ));
    assert( m . Register ( "62wtsergtsdfg34", "PA2 - #2" ));
    assert ( m . Register ( "er34252456hjsd2451451", "PA2 - #1" ) );
    assert ( m . Register ( "er34252456hjsd2451451", "PA2 - #3" ) );
    assert ( m . Register ( "sdswertcvsgncse", "PA2 - #1" ) );
    assert ( !m . Register ( "1234151asdfe5123416", "PA2 - #1" ) );
    assert ( !m . Register ( "aaaaaaaaaaaa", "PA2 - #1" ) );
    assert(m . Assess ( 123456, "PA2 - #1", 50 ));
    assert ( m . Assess ( 654321, "PA2 - #1", 30 ) );
    assert ( m . Assess ( 654321, "PA2 - #2", 40 ) );
    assert ( m . Assess ( 987, "PA2 - #1", 100 ) );
    assert ( !m . Assess ( 654321, "PA2 - #1", 35 ) );
    assert ( !m . Assess ( 654321, "PA2 - #3", 35 ) );
    assert ( !m . Assess ( 999999, "PA2 - #1", 35 ) );
    assert ( m . ListTest ( "PA2 - #1", CExam::SORT_RESULT ) == (list<CResult>
            {
                    CResult ( "West Peter Thomas", 987, "PA2 - #1", 100 ),
                    CResult ( "Smith John", 123456, "PA2 - #1", 50 ),
                    CResult ( "Nowak Jane", 654321, "PA2 - #1", 30 )
            } ) );
    assert ( m . ListTest ( "PA2 - #1", CExam::SORT_NAME ) == (list<CResult>
            {
                    CResult ( "Nowak Jane", 654321, "PA2 - #1", 30 ),
                    CResult ( "Smith John", 123456, "PA2 - #1", 50 ),
                    CResult ( "West Peter Thomas", 987, "PA2 - #1", 100 )
            } ) );
    assert ( m . ListTest ( "PA2 - #1", CExam::SORT_NONE ) == (list<CResult>
            {
                    CResult ( "Smith John", 123456, "PA2 - #1", 50 ),
                    CResult ( "Nowak Jane", 654321, "PA2 - #1", 30 ),
                    CResult ( "West Peter Thomas", 987, "PA2 - #1", 100 )
            } ) );
    assert ( m . ListTest ( "PA2 - #1", CExam::SORT_ID ) == (list<CResult>
            {
                    CResult ( "West Peter Thomas", 987, "PA2 - #1", 100 ),
                    CResult ( "Smith John", 123456, "PA2 - #1", 50 ),
                    CResult ( "Nowak Jane", 654321, "PA2 - #1", 30 )
            } ) );
    assert ( m . ListMissing ( "PA2 - #3" ) == (set<unsigned int>{ 123456 }) );
    iss . clear ();
    iss . str ( "888:Watson Joe:25234sdfgwer52, 234523uio, asdf234235we, 234234234\n" );
    assert ( m . Load ( iss ) );

    assert ( m . Register ( "234523uio", "PA2 - #1" ) );
    assert ( m . Assess ( 888, "PA2 - #1", 75 ) );
    iss . clear ();
    iss . str ( "555:Gates Bill:ui2345234sdf\n"
                "888:Watson Joe:2345234634\n" );
    assert ( !m . Load ( iss ) );

    assert ( !m . Register ( "ui2345234sdf", "PA2 - #1" ) );
    iss . clear ();
    iss . str ( "555:Gates Bill:ui2345234sdf\n"
                "666:Watson Thomas:okjer834d34\n" );
    assert ( !m . Load ( iss ) );

    assert ( !m . Register ( "ui2345234sdf", "PA2 - #3" ) );
    iss . clear ();
    iss . str ( "555:Gates Bill:ui2345234sdf\n"
                "666:Watson Thomas:jer834d3sdf4\n" );
    assert ( m . Load ( iss ) );

    assert ( m . Register ( "ui2345234sdf", "PA2 - #3" ) );
    assert ( m . ListMissing ( "PA2 - #3" ) == (set<unsigned int>{ 555, 123456 }) );

    return 0;
}
#endif /* __PROGTEST__ */

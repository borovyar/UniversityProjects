#ifndef __PROGTEST__
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdint>
#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <utility>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <queue>
#include <stack>
#include <deque>
#include <algorithm>
#include <memory>
#include <functional>
#include <stdexcept>
using namespace std;
#endif /* __PROGTEST__ */

class CPerson{
public:
    CPerson( const int & n_ID, const char* n_Name ) : m_ID (n_ID), m_Name ( n_Name ) {};

    virtual int GetID() = 0;
    virtual size_t CountDescendants ()  = 0;

    void SetMother( const shared_ptr<CPerson>& n_mother ){
        this->mother = n_mother;
    }
    void SetFather( const shared_ptr<CPerson>& n_father ){
        this->father = n_father;
    }
    shared_ptr<CPerson>& GetMother(  ){
        return this->mother;
    }
    shared_ptr<CPerson>& GetFather(  ){
        return this->father;
    }
    virtual void UpdateDescedants( const shared_ptr<CPerson>& obj ) = 0;

    virtual shared_ptr<CPerson> clone () const = 0;
    virtual ~CPerson() = default;
    virtual void print (ostream& os) const{
        os << m_ID << ": ";
    }
    virtual bool FindPrefix( const string& Prefix ) = 0;

    bool operator < ( const shared_ptr<CPerson>& other ) const {
        return m_ID < other->m_ID;
    }
    bool operator == ( const shared_ptr<CPerson>& other ) const {
        return m_ID == other->m_ID;
    }
    friend ostream& operator << ( ostream& os, const CPerson& obj){
            obj.print(os);
        return os;
    }


protected:
    shared_ptr<CPerson> father = nullptr, mother = nullptr;
    vector<int> m_Descedants;
    int m_ID;
    string m_Name;
};


class CMan : public CPerson
{
public:
    CMan(const int& n_ID, const char *n_Name) : CPerson(n_ID, n_Name){};
    ~CMan() override = default;

    friend ostream& operator << (ostream& os, CMan& obj){//ID: name (man)
        return os << obj.GetID() << ": " << obj.m_Name << " (man)";
    }

    shared_ptr<CPerson> clone () const override {
        return make_shared<CMan>(*this);
    }
    bool FindPrefix( const string& Prefix ) override {
        auto check = mismatch( Prefix.begin(), Prefix.end(), m_Name.begin());
        return check.first == Prefix.end();
    }
    virtual void print (ostream& os) const{
        CPerson::print(os);
        os << m_Name << " (man)";
    }
    int GetID () override {
        return  m_ID;
    }
    size_t CountDescendants () override {
        set<int> tmp;
        for(const auto& it : m_Descedants){
            tmp.insert(it);
        }
        return tmp.size();
    }

    void UpdateDescedants ( const shared_ptr<CPerson>& obj ) override{
        this->m_Descedants.push_back(obj->GetID());
        if(this->mother != nullptr)
            this->mother->UpdateDescedants(obj);
        if(this->father != nullptr)
            this->father->UpdateDescedants(obj);
    }

private:
    // todo
};

class CWoman : public CPerson
{
public:
    CWoman(const int& n_ID, const char* n_Name) : CPerson(n_ID, n_Name), married(false), maidenName(""){};
    ~CWoman() override = default;

    friend ostream& operator << (ostream& os, CWoman& obj){// ID: name (woman) or ID: name [born: maidenName] (woman)
        if(!obj.married){
            os << obj.GetID() << ": " << obj.m_Name << " (woman)";
        }
        else{
            os << obj.GetID() << ": " << obj.m_Name << "[born: " << obj.maidenName << ']' <<" (woman)";
        }
        return os;
    }

    virtual void print (ostream& os) const{
        CPerson::print(os);
        if(!married)
            os << m_Name << " (woman)";
        else
            os << m_Name << " [born: " << maidenName << ']' <<" (woman)";
    }

    bool   Wedding ( const string& newName ) {
        if( this->married )
            return false;
        this->maidenName = m_Name;
        this->m_Name = newName;
        this->married = true;
        return true;
    }

    shared_ptr<CPerson> clone () const override {
        return make_shared<CWoman>(*this);
    }
    bool FindPrefix( const string& Prefix ) override {
        auto check1 = mismatch( Prefix.begin(), Prefix.end(), m_Name.begin());
        if( married ) {
            auto check2 = mismatch(Prefix.begin(), Prefix.end(), maidenName.begin());
            if ( check1.first == Prefix.end() || check2.first == Prefix.end() )
                return true;
            else
                return false;
        }
        return check1.first == Prefix.end();
    }
    int GetID () override {
        return  m_ID;
    }
    size_t CountDescendants () override {
        set<int> tmp;
        for(const auto& it : m_Descedants){
            tmp.insert(it);
        }
        return tmp.size();
        return m_Descedants.size();
    }

    void UpdateDescedants ( const shared_ptr<CPerson>& obj ) override{
        this->m_Descedants.push_back(obj->GetID());
        if(this->mother != nullptr)
            this->mother->UpdateDescedants(obj);
        if(this->father != nullptr)
            this->father->UpdateDescedants(obj);
    }
private:
    bool married;
    string maidenName;
};

class CRegister
{
public:
    CRegister() : m_Register({}) {};
    bool           Add           ( const shared_ptr<CPerson>& person,
                                   const shared_ptr<CPerson>& father = nullptr,
                                   const shared_ptr<CPerson>& mother = nullptr ){
        auto check = m_Register.lower_bound(person->GetID());
        if(check != m_Register.end() && (*check).first == person->GetID()){
            return false;
        }
        else {
            person->SetFather(father);
            person->SetMother(mother);
            if(father != nullptr) {
                father->UpdateDescedants(person);
            }
            if( mother != nullptr ){
                mother->UpdateDescedants(person);
            }
            m_Register.insert({person->GetID(), person->clone()});
        }
        return true;
    }

    shared_ptr<CPerson>  FindByID      ( const int& n_ID ) const{
        auto check = m_Register.lower_bound(n_ID);
        if(check == m_Register.end())
            return nullptr;
        return check->second;
    }

    vector<shared_ptr<CPerson>>      FindByName    ( const string& Prefix ) const{
        vector<shared_ptr<CPerson>> r_Vec = {};
        for(const auto &it : m_Register){
            if(it.second->FindPrefix(Prefix))
                r_Vec.push_back(it.second->clone());
        }
        return r_Vec;
    }

    list<shared_ptr<CPerson>>        FindRelatives ( const int& n_ID1, const int& n_ID2 ) const{
        list<shared_ptr<CPerson>> r_List;
        return r_List;
    }

private:
    map<int, shared_ptr<CPerson>> m_Register;
};

#ifndef __PROGTEST__
template <typename T_>
static bool        vectorMatch ( const vector<T_>     & res,
                                 const vector<string> & ref )
{
    vector<string> tmp;
    for ( const auto & x : res )
    {
        ostringstream oss;
        oss << *x;
        tmp . push_back ( oss . str () );
    }
    return tmp == ref;
}
template <typename T_>
static bool        listMatch ( const list<T_>     & res,
                               const list<string> & ref )
{
    list<string> tmp;
    for ( const auto & x : res )
    {
        ostringstream oss;
        oss << *x;
        tmp . push_back ( oss . str () );
    }
    return tmp == ref;
}
int main ( )
{
    ostringstream oss;
    CRegister r;
    assert ( r . Add ( make_shared<CMan> ( 1, "Peterson George" ),
                       nullptr, nullptr ) == true );
    assert ( r . Add ( make_shared<CMan> ( 2, "Watson Paul" ),
                       nullptr, nullptr ) == true );
    assert ( r . Add ( make_shared<CMan> ( 10, "Smith Samuel" ),
                       nullptr, nullptr ) == true );
    assert ( r . Add ( make_shared<CMan> ( 10, "Smith Samuel" ),
                       nullptr, nullptr ) == false);
    assert ( r . Add ( make_shared<CWoman> ( 11, "Peterson Jane" ),
                       r . FindByID ( 1 ), nullptr ) == true );
    assert ( r . Add ( make_shared<CWoman> ( 12, "Peterson Sue" ),
                       r . FindByID ( 1 ), nullptr ) == true );
    assert ( r . Add ( make_shared<CMan> ( 13, "Pershing John" ),
                       nullptr, nullptr ) == true );
    assert ( dynamic_cast<CWoman &> ( *r . FindByID ( 12 ) ) . Wedding ( "Smith Sue" ) == true );
    assert ( r . Add ( make_shared<CMan> ( 100, "Smith John" ),
                       r . FindByID ( 10 ), r . FindByID ( 11 ) ) == true );
    assert ( r . Add ( make_shared<CMan> ( 101, "Smith Roger" ),
                       r . FindByID ( 10 ), r . FindByID ( 11 ) ) == true );
    assert ( r . Add ( make_shared<CMan> ( 102, "Smith Daniel" ),
                       r . FindByID ( 10 ), r . FindByID ( 11 ) ) == true );
    assert ( r . Add ( make_shared<CWoman> ( 103, "Smith Eve" ),
                       r . FindByID ( 10 ), r . FindByID ( 11 ) ) == true );
    assert ( r . Add ( make_shared<CWoman> ( 103, "Smith Jane" ),
                       r . FindByID ( 10 ), r . FindByID ( 11 ) ) == false );
    assert ( r . Add ( make_shared<CMan> ( 150, "Pershing Joe" ),
                       r . FindByID ( 13 ), r . FindByID ( 12 ) ) == true );
    assert ( r . Add ( make_shared<CMan> ( 200, "Pershing Peter" ),
                       r . FindByID ( 150 ), r . FindByID ( 103 ) ) == true );
    assert ( r . Add ( make_shared<CWoman> ( 201, "Pershing Julia" ),
                       r . FindByID ( 150 ), r . FindByID ( 103 ) ) == true );
    assert ( r . Add ( make_shared<CWoman> ( 202, "Pershing Anne" ),
                       r . FindByID ( 150 ), r . FindByID ( 103 ) ) == true );
    assert ( vectorMatch ( r . FindByName ( "Peterson" ), vector<string>
            {
                    "1: Peterson George (man)",
                    "11: Peterson Jane (woman)",
                    "12: Smith Sue [born: Peterson Sue] (woman)"
            } ) );
    assert ( vectorMatch ( r . FindByName ( "Pe" ), vector<string>
            {
                    "1: Peterson George (man)",
                    "11: Peterson Jane (woman)",
                    "12: Smith Sue [born: Peterson Sue] (woman)",
                    "13: Pershing John (man)",
                    "150: Pershing Joe (man)",
                    "200: Pershing Peter (man)",
                    "201: Pershing Julia (woman)",
                    "202: Pershing Anne (woman)"
            } ) );
    assert ( vectorMatch ( r . FindByName ( "Smith" ), vector<string>
            {
                    "10: Smith Samuel (man)",
                    "12: Smith Sue [born: Peterson Sue] (woman)",
                    "100: Smith John (man)",
                    "101: Smith Roger (man)",
                    "102: Smith Daniel (man)",
                    "103: Smith Eve (woman)"
            } ) );
    assert ( r . FindByID ( 1 ) -> GetID () == 1 );
    oss . str ( "" );
    oss << * r . FindByID ( 1 );
    assert ( oss . str () == "1: Peterson George (man)" );
    assert ( r . FindByID ( 1 ) -> CountDescendants () == 10 );
    assert ( r . FindByID ( 2 ) -> GetID () == 2 );
    oss . str ( "" );
    oss << * r . FindByID ( 2 );
    assert ( oss . str () == "2: Watson Paul (man)" );
    assert ( r . FindByID ( 2 ) -> CountDescendants () == 0 );
    /*assert ( listMatch ( r . FindRelatives ( 100, 1 ), list<string>
            {
                    "100: Smith John (man)",
                    "11: Peterson Jane (woman)",
                    "1: Peterson George (man)"
            } ) );
    assert ( listMatch ( r . FindRelatives ( 100, 13 ), list<string>
            {
                    "100: Smith John (man)",
                    "10: Smith Samuel (man)",
                    "103: Smith Eve (woman)",
                    "200: Pershing Peter (man)",
                    "150: Pershing Joe (man)",
                    "13: Pershing John (man)"
            } ) );
    assert ( listMatch ( r . FindRelatives ( 100, 2 ), list<string>
            {

            } ) );
    assert ( listMatch ( r . FindRelatives ( 100, 100 ), list<string>
            {
                    "100: Smith John (man)"
            } ) );
    try
    {
        r . FindRelatives ( 100, 3 );
        assert ( "Missing an exception" == nullptr );
    }
    catch ( const invalid_argument & e )
    {
    }
    catch ( ... )
    {
        assert ( "An unexpected exception thrown" );
    }*/
    return 0;
}
#endif /* __PROGTEST__ */

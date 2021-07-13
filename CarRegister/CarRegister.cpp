#ifndef __PROGTEST__
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <string>
#include <memory>
#include <functional>
#include <vector>
#include <algorithm>
#include <ctime>
using namespace std;
#endif /* __PROGTEST__ */
class CRegister;
class CPersonList;
class CCarList;
class Person;
class Car;

//Class "Car" implemented as class for cars and each car has a unique rz and name&surname of owner
class Car{
    friend CRegister;
    friend CCarList;
private:
    string mRz, name, surname;
public:
    Car(const string & name, const string & surname, const string & nRz) :  mRz(nRz), name(name), surname(surname){}
    ~Car() = default;

    bool operator < (const Car & other){
        return mRz < other.mRz;
    }

};
//Class "Person" implemented as class of the unique persons and each person has a name, surname and list of the cars
class Person {
    friend CRegister;
    friend CPersonList;
private:
    vector<string> Mcars;
    string name , surname;

    bool noCars(){
        return Mcars.empty();
    }//method check if list is empty, so person doesn't have any car

    void fillCars(vector <string>& tmp, const string & rz){
        auto ptr1 = (lower_bound(Mcars.begin(), Mcars.end(), rz) - Mcars.begin());
        Mcars.insert(Mcars.begin() + ptr1, rz);
    }//the method helps to add cars to the list

    void deleteCar(vector <string>& tmp, const string & rz){
        if(!noCars()) {
            auto ptr1 = (lower_bound(Mcars.begin(), Mcars.end(), rz) - Mcars.begin());
            Mcars.erase(Mcars.begin() + ptr1);
        }
    }//the method helps to delete cars from the list

public:
    Person (const string & name1 , const string & surname1, const string & rz) : name(name1), surname(surname1){
        fillCars(Mcars, rz);
    };

    ~Person() = default;

    bool operator < ( const Person &other) const{
        if(surname < other.surname) return true;
        if(surname > other.surname) return false;
        return name < other.name;
    }

};

class CCarList
{
public:
    explicit CCarList(const vector<string> & cars) : tmp(cars){
        it2 = tmp.size();
    }

    const string & RZ      ( ) const{
        return tmp[it1];
    }

    bool           AtEnd   ( ) const{
        return it1 == it2;
    }

    void           Next    (  ){
        it1++;
    }

private:
    const vector <string> & tmp;
    int it2, it1 = 0;
};

class CPersonList
{
    friend CRegister;
public:
    explicit CPersonList(const vector<Person> & persons) : tmp( persons ){
        it2 = tmp.size();
    }

    const string & Name    ( ) const{
        return tmp[it1].name;
    }

    const string & Surname ( ) const{
        return tmp[it1].surname;
    }

    bool           AtEnd   ( ) const{
        return it1 == it2;
    }

    void           Next    ( ){
        it1++;
    }

private:
    const vector <Person> & tmp;
    int it1 = 0, it2;
};

class CRegister
{
public:
    CRegister  ( ) = default;
    ~CRegister  ( ) = default;
    CRegister  ( const CRegister & src ) = delete;
    CRegister & operator = ( const CRegister & src ) = delete;

    bool        AddCar     ( const string & rz, const string & name, const string & surname ){
        auto it1 = lower_bound(cars.begin(), cars.end(), Car("", "", rz));//car's searching
        if (it1 != cars.end() && (*it1).mRz == rz) {//if already exists
            return false;
        }
        cars.insert(it1, Car(name, surname, rz));//add car to cars vector
        auto it2 = lower_bound(persons.begin(), persons.end(), Person(name, surname, rz));//person's searching
        if(it2 != persons.end() && (*it2).name == name && it2->surname == surname){//if person already exists
            it2->fillCars(it2->Mcars, rz);
            return true;
        }
        persons.insert(it2, Person(name, surname, rz));//add new person
        return true;
    }

    bool        DelCar     ( const string & rz ){
        auto it1 = lower_bound(cars.begin(), cars.end(), Car("", "", rz));//car's searching
        if (it1 != cars.end() && (*it1).mRz == rz) {//if found out
            auto it2 = lower_bound(persons.begin(), persons.end(), Person(it1->name, it1->surname, rz));//person's searching
            it2->deleteCar(it2->Mcars, rz);
            if(it2->noCars()){//check if persons doesn't have cars
                persons.erase(it2);
            }
            cars.erase(it1);
            return true;
        }
        return false;
    }

    bool        Transfer   ( const string & rz, const string & nName, const string & nSurname){
        auto it1 = lower_bound(cars.begin(), cars.end(), Car("", "", rz));//car's searching
        if (it1 != cars.end() && (*it1).mRz == rz && it1->name != nName && it1->surname != nSurname) {//if found out
            DelCar(rz);
            AddCar(rz, nName, nSurname);
            it1->name = nName;//change name and surname of car's owner
            it1->surname = nSurname;
            return true;
        }
        return false;//if not
    }

    CCarList    ListCars   ( const string & name, const string & surname ) const{
        auto p_it1 = lower_bound(persons.begin(), persons.end(), Person(name, surname, ""));//person's searching
        if(p_it1 != persons.end() && (*p_it1).name == name && p_it1->surname == surname) {//if exists
            CCarList c_list2(p_it1->Mcars);
            return c_list2;
        }
        CCarList c_list1({});//pass empty list
        return c_list1;
    }

    int         CountCars  ( const string & name, const string & surname ) const {
        auto p_it1 = lower_bound(persons.begin(), persons.end(), Person(name, surname, ""));//person's searching
        if(p_it1 != persons.end() && (*p_it1).name == name && p_it1->surname == surname){//if exists
            return p_it1->Mcars.size();
        }
        return 0;
    }

    CPersonList ListPersons( ) const {
        CPersonList p_list(persons);//pass vector persons(there is unique persons )
        return p_list;
    }

private:
    vector<Person> persons;
    vector<Car> cars;
};

#ifndef __PROGTEST__
bool checkPerson           ( CRegister    & r,
                             const string & name,
                             const string & surname,
                             vector<string> result )
{
    for ( CCarList l = r . ListCars ( name, surname ); ! l . AtEnd (); l . Next () )
    {
        auto pos = find ( result . begin (), result . end (), l . RZ () );
        if ( pos == result . end () )
            return false;
        result . erase ( pos );
    }
    return result.empty();
}
int main ()
{
    return 0;
}
#endif /* __PROGTEST__ */
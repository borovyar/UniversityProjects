#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <climits>
#include <cmath>
#include <cfloat>
#include <cassert>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#if defined ( __cplusplus ) && __cplusplus > 199711L /* C++ 11 */
#include <memory>
#endif /* C++ 11 */
using namespace std;
#endif /* __PROGTEST__ */
class CPolynomial;

class CPolynomial
{
    friend bool dumpMatch(const CPolynomial &, const vector<double> &);
  public:
    //constructors with destructors
    CPolynomial() {
        this->polynomial.push_back(0);
    }

    CPolynomial(const CPolynomial& other){
        this->polynomial = other.polynomial;
    }

    CPolynomial &  operator = (const CPolynomial& other ){
        if(this == &other)
            return *this;
        this->polynomial = other.polynomial;
        return *this;
    }

    ~CPolynomial() = default;
    //arithmetic operators
    CPolynomial operator += ( const CPolynomial& obj1 ){
        for(auto i = this->polynomial.size(); i < obj1.polynomial.size(); i++){
            this->polynomial.push_back(0);
        }
        for(long unsigned int m_it = 0; m_it != obj1.polynomial.size(); m_it++){
            this->polynomial[m_it] += obj1.polynomial[m_it];
        }
        if(all_of(this->polynomial.begin(), this->polynomial.end(), [](const double & i){return i == 0;})){
            this->polynomial.clear();
            this->polynomial.push_back(0);
        }
        return *this;
    }

    CPolynomial operator -= ( const CPolynomial& obj1 ){
        for(auto i = this->polynomial.size(); i < obj1.polynomial.size(); i++){
            this->polynomial.push_back(0);
        }
        for(long unsigned int m_it = 0; m_it != obj1.polynomial.size(); m_it++){
            this->polynomial[m_it] -= obj1.polynomial[m_it];
        }
        if(all_of(this->polynomial.begin(), this->polynomial.end(), [](const double & i){return i == 0;})){
            this->polynomial.clear();
            this->polynomial.push_back(0);
        }
        return *this;
    }

    CPolynomial operator *= ( const double & value ){
        for(long unsigned int it = 0; it < this->polynomial.size(); it++){
            this->polynomial[it] *= value;
        }
        if(all_of(this->polynomial.begin(), this->polynomial.end(), [](const double & i){return i == 0;})){
            this->polynomial.clear();
            this->polynomial.push_back(0);
        }

        return *this;
    }

    CPolynomial operator *= ( const CPolynomial & other ){

        vector<double> result (this->polynomial.size() + other.polynomial.size() - 1, 0);

        for(long unsigned int m_it = 0; m_it != this->polynomial.size(); m_it++){
            for(long unsigned int  n_it = 0; n_it != other.polynomial.size(); n_it++){
                result[m_it + n_it] += this->polynomial[m_it] * other.polynomial[n_it];
            }
        }

        if(all_of(result.begin(), result.end(), [](const double & i){return i == 0;})){
            result.clear();
            result.push_back(0);
        }
        this->polynomial = result;
        return *this;
    }

    CPolynomial operator + ( const CPolynomial& obj1 ) const{
        CPolynomial tmp (*this);
        tmp += obj1;
        return tmp;
    }

    CPolynomial operator  - ( const CPolynomial& obj1 ) const {
        CPolynomial tmp (*this);
        tmp -= obj1;
        return tmp;
    }

    CPolynomial operator * ( const double & value ) const{
        CPolynomial tmp(*this);
        tmp *= value;
        return tmp;
    }

    CPolynomial operator * ( const CPolynomial & other ) const{
        CPolynomial tmp(*this);
        tmp *= other;
        return tmp;
    }

    // bool operators
    bool operator == ( const CPolynomial & obj) const{
        if(all_of(this->polynomial.begin(), this->polynomial.end(), [](const double & i){return i == 0;}) &&
                all_of(obj.polynomial.begin(), obj.polynomial.end(), [](const double & i){return i == 0;})){
            return true;
        }
        return this->polynomial == obj.polynomial;
    }

    bool operator != ( const CPolynomial & obj) const {
        if(all_of(this->polynomial.begin(), this->polynomial.end(), [](const double & i){return i == 0;}) &&
           all_of(obj.polynomial.begin(), obj.polynomial.end(), [](const double & i){return i == 0;})){
            return false;
        }
        return this->polynomial != obj.polynomial;
    }

    // assign operators
    double & operator [] (const int & index){
        if((int) this->polynomial.size() <= index){
            for(; (int) this->polynomial.size() <= index ;)
                this->polynomial.push_back(0);
        }
        return this->polynomial[index];
    }

    double operator [] ( const int & index) const{
        return this->polynomial[index];
    }
    //given a value of unknown variable
    double operator () (const double & x) const {
        double counter = 0;
        for( long unsigned int it = 0; it < polynomial.size(); it++){
            counter += pow(x,it) * this->polynomial[it];
        }
        //cout << counter << endl;
        return counter;
    }
    //degree of polynomial
    unsigned int Degree () const{
        unsigned int degree = 0;
        for(long unsigned int m_it = 0; m_it < polynomial.size(); m_it++){
            if(polynomial[m_it] != 0 && m_it != 0){
                degree = m_it;
            }
        }
        return degree;
    }
    //ostream operator
    friend ostream& operator << (ostream & out, const CPolynomial & obj) {//x^3 + 3.5*x^1 - 10

        if(obj.polynomial.size() == 1){
            if(obj.polynomial[0] >= 0){
                out << obj.polynomial[0];
            }
            else {
                out << "- " << obj.polynomial[0];
            }
            return out;
        }

        for(auto i = obj.polynomial.size() - 1; i != 0; i--){
            if(obj.polynomial[i] == 0){
                continue;
            }

            if(abs(obj.polynomial[i]) == 1 && i == obj.polynomial.size() - 1){
                if(obj.polynomial[i] == -1){
                    out << "- ";
                }
            }

            else if(abs(obj.polynomial[i]) == 1 && i != obj.polynomial.size() - 1){

                if(obj.polynomial[i] == -1 && out.tellp() == 0){
                    out << "- ";
                }

                else if(out.tellp() != 0){
                    if(obj.polynomial[i] == -1){
                        out << " - ";
                    }
                    else{
                        out << " + ";
                    }
                }
            }

            else{
                if(obj.polynomial[i] > 0 && i != obj.polynomial.size() - 1 && out.tellp() != 0){
                    out << " + " << obj.polynomial[i] << '*';
                }

                else if(obj.polynomial[i] < 0 && i != obj.polynomial.size() - 1 && out.tellp() != 0){
                    out << " - " << -1 * obj.polynomial[i] << '*';
                }

                else{
                    if( obj.polynomial[i] > 0 ){
                        out << obj.polynomial[i] << '*';
                    }
                    else{
                        out << "- " << -1 * obj.polynomial[i] << '*';
                    }
                }

            }

            out << "x^" << i;
        }

        if(out.tellp() == 0){
            if(obj.polynomial[0] >= 0){
                out << obj.polynomial[0];
            }
            else {
                out << "- " << obj.polynomial[0];
            }
            return out;
        }

        if(all_of(obj.polynomial.begin(), obj.polynomial.end(), [](const double & i){return i == 0;})){
            return out << '0';
        }

        if(obj.polynomial[0] > 0 ){
            out << " + " <<  obj.polynomial[0];
        }

        else if (obj.polynomial[0] < 0){
            out << " - " << -1 * obj.polynomial[0];
        }


        return out;
    }

    /*void gg(){
        for(const auto & x : polynomial){
            cout << x << endl;
        }
    }*/

  private:
    vector <double> polynomial;
};

#ifndef __PROGTEST__
bool               smallDiff  ( double            a, double            b ){
        return a == b;
}
bool               dumpMatch                               ( const CPolynomial & x,
                                                             const vector<double> & ref ){
    return x.polynomial == ref;
}
int                main                                    ( )
{
  CPolynomial a, b, c;
  ostringstream out;

  a[0] = 5;
  a[1] = 1;
  a[3] = 0;
  a[4] = 0;
  a[5] = 1;
  b[0] = -10;
  a = a - b;
  //assert ( smallDiff ( a ( 2 ), 5 ) );
  out . str ("");
  out << a;
  //assert ( out . str () == "x^3 + 3.5*x^1 - 10" );
  //a = a * -2;
  cout << a.Degree() << endl << out.str();
  /*assert ( a . Degree () == 3
           && dumpMatch ( a, vector<double>{ 20.0, -7.0, -0.0, -2.0 } ) );

  out . str ("");
  out << a;
  assert ( out . str () == "- 2*x^3 - 7*x^1 + 20" );
  out . str ("");
  out << b;
  assert ( out . str () == "0" );
  b[5] = -1;
  out . str ("");
  out << b;
  assert ( out . str () == "- x^5" );
  c = a + b;
  assert ( c . Degree () == 5
           && dumpMatch ( c, vector<double>{ 20.0, -7.0, 0.0, -2.0, 0.0, -1.0 } ) );

  out . str ("");
  out << c;
  assert ( out . str () == "- x^5 - 2*x^3 - 7*x^1 + 20" );
  c = a - b;
  assert ( c . Degree () == 5
           && dumpMatch ( c, vector<double>{ 20.0, -7.0, -0.0, -2.0, -0.0, 1.0 } ) );

  out . str ("");
  out << c;
  assert ( out . str () == "x^5 - 2*x^3 - 7*x^1 + 20" );
  c = a * b;
  assert ( c . Degree () == 8
           && dumpMatch ( c, vector<double>{ -0.0, -0.0, 0.0, -0.0, 0.0, -20.0, 7.0, -0.0, 2.0 } ) );

  out . str ("");
  out << c;
  assert ( out . str () == "2*x^8 + 7*x^6 - 20*x^5" );
  assert ( a != b );
  b[5] = 0;
  assert ( !(a == b) );
  a = a * 0;
  assert ( a . Degree () == 0
           && dumpMatch ( a, vector<double>{ 0.0 } ) );

  assert ( a == b );*/
  return 0;
}
#endif /* __PROGTEST__ */

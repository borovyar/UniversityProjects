The task is to develop a C++ class CPolynomial representing polynomials of arbitrary degree.

The class will store individual coefficients (floating point numbers - double precision), moreover, public interface will offer overloaded operators and methods to manipulate polynomials:

default constructor
the constructor will prepare an object representing polynomial 0,
copy constructor
the constructor will be implemented if your internal representation requires it,
destructor
destructor will be present if your internal representation requires it,
overloaded operator=
the operator will copy the contents from one instance to another (if the automatically generated op= cannot be used with your implementation).
operator <<
will output a textual form of the polynomial into an output stream. The output formatting must follow these rules:
the polynomial will be displayed in a decreasing order of powers, i.e. from the highest power of x,terms with zero coefficient are not displayed in the output,
terms with coefficient +1 and -1 are displayed without the coefficient (just a power of x),
there are not unnecessary - in the output (i.e. x - 9 or - x^2 + 4 is OK, whereas x + (-9) is not),
zero polynomial will be displayed as 0.
operator +
adds two polynomials,
operator -
subtracts two polynomials,
operator *
multiplies given polynomial either with a double, or with another polynomial,
operators == and !=
compare two polynomials,
operator []
is used to access (read / write) individual coefficients, the term is determined by the index (0 = absolute, 1 = x, 2 = x^2, ... ). The read form must be available even on const instances,
operator ()
evaluates the value of the polynomial for the given value x (x is a double),
Degree()
the method returns the degree of the polynomial (e.g. x^3+4 has degree of 3, 5 has degree 0, specifically 0 has degree 0).

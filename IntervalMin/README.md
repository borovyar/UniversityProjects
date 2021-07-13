The task is to develop class template CIntervalMin. The class will store values similarly to std::vector, moreover, it will provide an interface to quickly search for minimum values in subintervals.

To make the class more flexible, there will be two generic parameters:

T_ is the data type of the data stored in the instance. T_ may be an ordinary integer, floating point, std::string, or some other data type. Type T_ supports at least the following operations: copying/moving (move/copy constructible, move/copy assignable), comparison by operator <, output to a stream via operator <<, and destruction (destructible). The type may provide some further interface, however, your implementation cannot rely on it. Caution, the type does not have to implement default constructor!

Cmp_ is an optional generic parameter for the comparator. The comparator is either a function object, a function, a lambda function, or the generic parameter is omitted (in which case the comparator fallbacks to the std::less from the STL library).
The interface of the class:

Constructor (optionally with a comparator) creates an empty object instance.
Constructor with a pair of iterators (and with an optional comparator) initializes the instance with the data copied from the range defined by the input iterators.
Destructor (if needed)
Method push_back ( val ) appends the value to the end of the internal storage.
Method pop_back () removes the last value from the internal storage.
Method size () returns the number of elements stored in the instance.
Data type const_iterator declares an iterator suitable to access the values stored in a CIntervalMin instance. The iterator must provide random access iterator interface similar to the std::vector iterator. The iterator is a read only iterator (it does not provide an interface to modify the values stored in the CIntervalMin instance).
Method begin () returns a const_iterator referencing the first element in the store.
Method end () returns a const_iterator referencing one past the last element in the store.
Method min (st, en) finds the minimum element in the range defined by the iterators in parameters. The minimum element is then returned to the caller. If defined, the method compares the elements by means of the comparator passed to the constructor. Conversely, the elements will be compared by operator < if the instance does not have a special comparator defined. If there is more than one "minimum" in the range, the method returns the value closest to the iterator st. If the minimum is not defined (e.g., st == en, meaning an empty interval), then the method throws exception std::invalid_argument.
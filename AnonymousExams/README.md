The task is to develop class CExam. The class shall support anonymized testing.

Each CTU student is assigned an unique integer identifier. The identifier is unique through the student and staff database (while name and surname is not). Moreover, each student is given a card (ISIC), the card is identified by some string identifier (further, we assume the string consists of digits and letters). One student may have several cards (e.g. new card when old card has been lost).

The tests are organized in the following way: first, students are registered for the test. The registration uses the card as a student identifier. Next, the tests are assessed and the results are written to the database. Finally, there is an interface to list the result, and a method that reports students who do not have their tests assessed yet. The complete interface is:

default constructor
creates an empty instance.
Load (stream)
reads student and card list from the given input stream. The parameter of the call is an open input stream. The stream lists student information. Each line of the input describes one student. The line has the form:
  student_id:name_and_surname:card_id, card_id, ...
  
Student id is an integer, the id is unique through the whole student database. Name and surname is a string, the string consists of letters, digits and spaces. The names may be duplicate. Card identifiers are strings of letters and digits, the identifiers are separated by commas and optional spaces. Card IDs are unique through the whole student database.

Method Load reads the input stream and stores the data into its internal structures. If the input is correct, it returns true. If the input is invalid (duplicate student/card IDs), the method does not modify anything and returns false.

Method Register (card, test)
registers student for a test. The student is identified by card ID. The method returns true if successful or false for a failure (the is already registered for the test, unknown card).
Assess ( student, test, res )
the method completes the assessment of the student. The parameters are student ID, test identification and the result (points). Return value is true for success or false for a failure (the student was not registered for the test, the student has already been assessed, unknown student).
ListTest (test, sortBy)
the method lists the results of the given test. The returned value is a list of CResult structures, each structure describes one student and his/her result. The returned list shall list all assessed students for the given test. The list must be sorted according to the given sort criteria (see symbolic constants in the class declaration): SORT_RESULT (by the result, descending), SORT_NAME (by name, ascending), SORT_ID (by student ID, ascending), and SORT_NONE (unsorted, i.e. in the order the tests were assessed). if the sort option itself is not enough (e.g. SORT_RESULT and SORT_NAME), then the order the test were assessed shall be used as the second sort key. For instance SORT_RESULT is used and two students were both awarded 50 points. Then the student who was assessed earlier appears first in the list.
ListMissing (test)
find students registered for the given test whose tests were not assessed yet. The return value is the set of their IDs.
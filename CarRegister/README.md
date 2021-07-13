Your task is to implement class CRegister, which implements a car database.

The database is very simplified. We assume the database saves information about persons and the cars they own. Each person is described by his/her name and surname. We assume the pair (name, surname) is unique in the database. That is, there may exist many persons with the same name (e.g. "John"), there may be many persons registered with the same surname (e.g. "Smith"), however, there may be at most one "John Smith". Each car is identified by its license plate, the plate is unique. Finally, each car must be owned by exactly one person, however, a person may own an arbitrary number of cars.

The class shall implement public interface shown below. The interface consists of:

A constructor (without parameters). This constructor initializes a new empty database.
Destructor -- it shall free all resources used by the instance.
Method AddCar (rz, name, surname ) which adds a new record to the database. The parameters are name and surname of the person being registered. Parameter rz is the license plate of the registered car. The method returns true if it succeeds, or false if it fails (i.e. the rz is already present in the database).
Method DelCar (rz) removes the corresponding record from the database, the record is identified by the registration plate. The method returns true if it succeeds, or false if it fails (the corresponding record was not present). In addition to the car, the method may need to delete the person if it deleted the last car the person owned.
Method Transfer (rz, name, surname) modifies the database such that the original owner of car rz sold the car and the car now belongs to person identified by the name and surname. The method returns true if it succeeds, or false if it fails (the corresponding car was not found or the seller and the buyer is the same person). Finally, the method erases the previous owner of the car if that owner does not have any other car.
Method CountCars (name, surname) count the cars owned by person identified by the name and surname. If the person is unknown, the result is zero.
Method ListCars ( name, surname) lists the cars owned by the person identified by the name and surname. The result is an object of class CCarList. The object represents a list of cars that belong to the person. The object may be used to iterate through the list to access the individual cars (see below). If the person does not exist/is unknown, the resulting object represents an empty list.
Method ListPersons gives access to the persons in the register. It returns an object of class CPersonList, the object may be used to list the names and surnames of all persons in the database. If the database is empty, the resulting object represents an empty list.
Class CCarList represents a list of cars. The object may be used to iterate through the records and to obtain the registration plates of individual cars. The interface is:

RZ - return the registration plate of the active car in the list,
AtEnd - indicate whether we reached the end of the list (returns true), or not, i.e. there are further cars available (returns false),
Next - move one record forward in the car list.
Class CPersonList is used to iterate over the existing persons in the database. Exactly one person is accessed at a time, the object offers an interface to move forward in the list op persons. The persons are accessed in a sorted order - sorted primarily by surname, then (for equal surnames) sorted by names. The interface is:

method Name - retrieve the name of the currently accessed person,
method Surname - retrieve the surname of the currently accessed person,
method AtEnd - indicate whether we reached the end of the list (returns true), or not, i.e. there are further persons available (returns false),
method Next - move one record forward in the person list.
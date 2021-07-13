The task is to develop classes that implement a register of birth and death. The classes provide an interface to modify the register, search for people and search for relatives.

The register stores objects that represents men (CMan) and women (CWoman). Additional genders are not supported in this implementation, however, the class design is open for further extensions. Technically, the classes will be polymorphic, i.e., classes CMan and CWoman will be subclasses of some superclass. Therefore, when researchers in the field of gender studies invent another gender, the register just needs to implement a new class for that new gender. The exact class design is left on you, nevertheless, there is a certain public interface that must be supported:

CMan
The class represents a man. A man is described by his name (any string, not unique) and personal ID (an integer, unique). Next, the class may need to store some other information (a reference to father/mother/children, or some other information you find useful). The public interface must contain:

constructor with parameters (id, name) to initialize the instance,
method GetID() to return the personal ID,
method CountDescendants. The method returns the number of known unique descendants (children, grandchildren, ...) of the man,
operator << to print the man into an output stream, the output format is ID: name (man), see example runs,
other methods you add in your implementation.
CWoman
The class represents a woman. A woman is described by her name (any string, not unique), personal ID (an integer, unique), and (optional) maiden name. Next, the class may need to store some other information (a reference to father/mother/children, or some other information you find useful). The public interface must contain:

constructor with parameters (id, name) to initialize the instance,
method GetID() to return the personal ID,
method CountDescendants. The method returns the number of known unique descendants (children, grandchildren, ...) of the woman,
operator << to print the woman into an output stream. There are two output formats for a woman: ID: name (woman) or ID: name [born: maidenName] (woman) if the woman is married, see example runs,
method Wedding (newName) changes the name of the woman after a wedding. The method behavior is as follows:
if the woman is already married (i.e., there has already been a call to method Wedding), then the method does not change anything and returns false. Out data model allows only one wedding in a lifetime,
in the opposite case, the method copies the current name (as the instance was initialized) into the maiden name and sets the woman's name to the name in argument. Finally, the method returns true,
if Wedding method has not been called on an instance of CWoman, then the instance prints itself without the maiden name information (i.e., the without [born: ...]). Once the Wedding method is called, the instance outputs itself with the maiden name information (i.e., the format includes [born: ...]). There may be a situation where the maiden name and the name after the wedding is literally the same. Anyway, the output will contain the maiden name information, even in this case. Thus the output may look like 666: Novakova [born: Novakova] (woman).
other methods you add in your implementation.
CRegister
The class represents a register of birth and death. There are methods to add people and search for them. The public interface must contain the following methods:

a default constructor to initialize an empty register,
a destructor to release the resources,
method Add (person, father, mother) to add an new person to the register. Parameter person references the CMan/CWoman instance to add to the register. The instance is dynamically allocated and passed as a shared_ptr. The two following arguments are optional. The parameters refer to the father and mother of the person (if the parents are known), or are set to nullptr (if the parents are not known or are not present in the register). The method may be called even with one parent set and the other unknown. The method returns true to indicate success, or false for an error. An error is reported if the ID of the newly added person is already in use. Your implementation does not have to do other validation. In particular, you do not have to test whether the father is a reference to CMan (and mother is a reference to CWoman). The testing environment too reactionary to do that sort of things.
method FindByID (ID) to find the person with that given ID. The method returns a reference to the person object, or an invalid reference if the ID is unknown. The return value is a type supporting a dereference to access the instance of CMan/CWoman. In other words, the return value may be a raw pointer, a shared_ptr, or your own implementation of a smart pointer.
method FindByName(prefix) to search for persons whose names start with the given string prefix. Return value is an array of references to persons matching the prefix filter. The persons in the resulting vector must be sorted according to their IDs in an ascending order. If no person in the register matches the prefix, the resulting vector will be empty. The searching must match the prefix with the name of the person, moreover, it must test maiden names for married women. The resulting vector must not contain duplicates. For instance, the method may be invoked with parameter "Novakova". Suppose, there is a CWoman with both name and maiden name set to "Novakova". However, the person will be listed only once in the result. The comparison is case-sensitive.
method FindRelatives ( id1, id2 ) to test whether two persons are relative, or not. Moreover, the method must find the relationship of the two persons if they are actually relative. The result may be:
an empty list if the two persons are not relative,
a list of persons that define the relationship. The first person in the list is a reference to the person with id1. Following it, there are references to the intermediate relatives, ending with a reference to the person with id2. Any pair of persons in the list must be direct relatives, i.e., son/daughter/father/mother. Any more complex relationship must be decomposed, e.g., a brother must be defined by means of a father/mother.
Generally, there may exist many such lists defining the relationship of the persons id1 and id2. The method must find the shortest list possible. Even if the lists are as short as possible, there still may exist many such lists. However, any such list is accepted by the testing environment (the testing environment checks the relationship of the adjacent persons in the returned list),

std::invalid_argument exception is thrown if id1 or id2 is not known in the register.

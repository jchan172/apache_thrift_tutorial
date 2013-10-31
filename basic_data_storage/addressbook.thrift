namespace py addressbook
namespace cpp addressbook
namespace rb addressbook

struct Description
{
	1: required string description
	2: optional double height_in_inches
	3: optional i32 number_of_spouses
}

struct PhoneNumber
{
	1: required string number
	2: optional string country_code
	9: optional byte nonsensical_data
}

struct Person
{
	1: required string name
	2: required i32 id
	3: optional string email
	8: required PhoneNumber number
	9: optional Description description
}

struct ContactsList
{
	1: list<Person> list_of_people
}

service AddressBook
{
	string listAll(1:string filename)
	string addContact(1:Person person, 2:string filename)
}
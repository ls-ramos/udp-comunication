#include "unp.h"

int serializeStringItem(char* buffer, char* source);
int deserializeStringItem(char* buffer, char** string);

int serializeIntItem(char* buffer, int* source);
int deserializeIntItem(char* buffer, int* num);

int serializeList(char* buffer, char** source, int listSize);
int deserializeList(char* buffer, char*** list);

int serializePerson(char* buffer, Person* person);
int deserializePerson(char* buffer, Person* person);

int serializeMessage(char* buffer, Message* message);
int deserializeMessage(char* buffer, Message* message);


void test_serializeIntItem();
void test_serializeList();
void test_serializePerson();
void test_serializeMessage();

int comparePerson(Person* p1, Person* p2);
void freePerson(Person* p);

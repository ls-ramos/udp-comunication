#include "serialize.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int serializeStringItem(char* buffer, char* source){
   int finalPos = 0;
   
   // data size
   int dataSize = strlen(source);
   memcpy(&buffer[finalPos], &dataSize, sizeof(dataSize));
   finalPos += sizeof(dataSize);

   // data
   memcpy(&buffer[finalPos], source, dataSize);
   finalPos += dataSize;

   return finalPos;
}

int deserializeStringItem(char* buffer, char** string){
   int pos=0, size;
   
   memcpy(&size, &buffer[pos], sizeof(size));
   pos += sizeof(size);

   *string = malloc(sizeof(char)*(size+1)); // +1 because of the \0 in the end
   memcpy(*string, &buffer[pos], size);
   pos += size;
   return pos;
}

int serializeIntItem(char* buffer, int* source){
   int finalPos = 0;
   
   memcpy(&buffer[finalPos], source, sizeof(int));
   finalPos += sizeof(int);

   return finalPos;
}

int deserializeIntItem(char* buffer, int* num){
   int pos=0;

   memcpy(num, buffer, sizeof(int));
   pos += sizeof(int);

   return pos;
}

int serializeList(char* buffer, char** source, int listSize){
   int finalPos = 0;
   
   // list size
   memcpy(&buffer[finalPos], &listSize, sizeof(listSize));
   finalPos += sizeof(listSize);      

   for(int i=0;i<listSize;i++){
      finalPos += serializeStringItem(&buffer[finalPos], source[i]);
   }

   return finalPos;
}

int deserializeList(char* buffer, char*** list){
   int pos = 0, size;

   memcpy(&size, &buffer[pos], sizeof(int));
   pos += sizeof(int);

   *list = malloc(sizeof(char*)*size);
   for(int i=0;i<size;i++){
      pos += deserializeStringItem(&buffer[pos],&(*list)[i]);
   }
   return pos;
}

int serializePerson(char* buffer, Person* person){
   int finalPos = 0;
   char marker;
   finalPos += serializeStringItem(&buffer[finalPos], person->email);
   finalPos += serializeStringItem(&buffer[finalPos], person->name);
   finalPos += serializeStringItem(&buffer[finalPos], person->lastName);
   finalPos += serializeStringItem(&buffer[finalPos], person->residence);
   finalPos += serializeStringItem(&buffer[finalPos], person->graduation);
   finalPos += serializeIntItem(&buffer[finalPos], &person->graduationYear);
   
   finalPos += serializeIntItem(&buffer[finalPos], &person->sizeSkills);
   finalPos += serializeList(&buffer[finalPos], person->skills,person->sizeSkills);
   
   finalPos += serializeIntItem(&buffer[finalPos], &person->sizeExperiences);
   finalPos += serializeList(&buffer[finalPos], person->experiences,person->sizeExperiences);

   return finalPos;
}

int deserializePerson(char* buffer, Person* person){
   int pos = 0;

   pos += deserializeStringItem(&buffer[pos],&person->email);
   pos += deserializeStringItem(&buffer[pos],&person->name);
   pos += deserializeStringItem(&buffer[pos],&person->lastName);
   pos += deserializeStringItem(&buffer[pos],&person->residence);
   pos += deserializeStringItem(&buffer[pos],&person->graduation);
   pos += deserializeIntItem(&buffer[pos],&person->graduationYear);

   pos += deserializeIntItem(&buffer[pos],&person->sizeSkills);
   pos += deserializeList(&buffer[pos],&person->skills);

   pos += deserializeIntItem(&buffer[pos],&person->sizeExperiences);
   pos += deserializeList(&buffer[pos],&person->experiences);
   
   return pos;
}

int serializeMessage(char* buffer, Message* message){
   int finalPos = 0;

   finalPos += serializeIntItem(&buffer[finalPos], &message->operationCode);
   finalPos += serializeIntItem(&buffer[finalPos], &message->statusCode);
   finalPos += serializeIntItem(&buffer[finalPos], &message->peopleCount);

   for(int i=0;i<message->peopleCount; i++){
      finalPos += serializePerson(&buffer[finalPos],&message->peopleData[i]);
   }
   return finalPos;
}

int deserializeMessage(char* buffer, Message* message){
   int pos=0;
   pos += deserializeIntItem(&buffer[pos],&message->operationCode);
   pos += deserializeIntItem(&buffer[pos],&message->statusCode);
   pos += deserializeIntItem(&buffer[pos],&message->peopleCount);

   message->peopleData = malloc(sizeof(Person)*message->peopleCount);
   for(int i=0;i<message->peopleCount;i++){
      pos += deserializePerson(&buffer[pos],&message->peopleData[i]);
   }
   return pos;
}


void test_serializeStringItem(){
   char buffer[100];
   char org[] = "teste aleatorio fjh478q7842hui3bhfj afi qf da";  
   char *rec;

   serializeStringItem(buffer,org);
   deserializeStringItem(buffer,&rec);

   if (strcmp(org,rec) == 0) puts("1 - test_serializeStringItem PASSED");
   else puts("1 - test_serializeStringItem FAILED");

   free(rec);
}

void test_serializeIntItem(){
   char buffer[100];
   int org = 123456789;
   int rec;

   serializeIntItem(buffer, &org);
   deserializeIntItem(buffer, &rec);
   if (org == rec) puts("2 - test_serializeIntItem PASSED");
   else puts("2 - test_serializeIntItem FAILED");
}

void test_serializeList(){
   int listSize = 3;
   char buffer[100];
   char** org = malloc(sizeof(char*) * listSize);
   org[0] = malloc(sizeof(char) * 6);
   strcpy(org[0], "teste");
   org[1] = malloc(sizeof(char) * 41);
   strcpy(org[1], "teste bem maior para ver o comportamento");
   org[2] = malloc(sizeof(char) * 12);
   strcpy(org[2], " t e s t e ");
   char** rec;

   serializeList(buffer, org, listSize);
   deserializeList(buffer, &rec);
   for(int i=0;i<listSize;i++){
      int cmp = strcmp(org[0],rec[0]);
      if(cmp!=0){
         puts("3 - test_serializeList FAILED");
         return;
      } 
   }
   puts("3 - test_serializeList PASSED");
   for(int i=0;i<listSize;i++){
         free(org[i]);
         free(rec[i]);
   }
   free(org);
   free(rec);
}

void test_serializePerson(){
   char buffer[200];
   Person *p = malloc(sizeof(Person));
   Person *rec = malloc(sizeof(Person));
   int listSize = 3;
   
   p->email = malloc(sizeof(char)*16);
   strcpy(p->email,"teste@teste.com");
   p->name = malloc(sizeof(char)*6);
   strcpy(p->name, "teste");
   p->lastName = malloc(sizeof(char)*11);
   strcpy(p->lastName, "teste last");
   p->residence = malloc(sizeof(char)*11);
   strcpy(p->residence, "teste city");
   p->graduation = malloc(sizeof(char)*8);
   strcpy(p->graduation, "test UNI");
   
   p->graduationYear = 3000;

   p->sizeSkills = listSize;
   
   p->skills = malloc(sizeof(char*)*listSize);
   p->skills[0] = malloc(sizeof(char)*10);
   strcpy(p->skills[0], "test ski0");
   p->skills[1] = malloc(sizeof(char)*10);
   strcpy(p->skills[1], "test ski1");
   p->skills[2] = malloc(sizeof(char)*10);
   strcpy(p->skills[2], "test ski2");
   
   p->sizeExperiences = listSize;
   p->experiences = malloc(sizeof(char*)*listSize);
   p->experiences[0] = malloc(sizeof(char)*10);
   strcpy(p->experiences[0], "test exp0");
   p->experiences[1] = malloc(sizeof(char)*10);
   strcpy(p->experiences[1], "test exp1");
   p->experiences[2] = malloc(sizeof(char)*10);
   strcpy(p->experiences[2], "test exp2");

   serializePerson(buffer, p);
   deserializePerson(buffer,rec);

   if(comparePerson(p,rec) == 1){
      puts("4 - test_serializePerson PASSED");
   }else { 
      puts("4 - test_serializePerson FAILED");
   }
   freePerson(p);
   freePerson(rec);
}

void test_serializeMessage(){
   char buffer[1000];
   Message *org = malloc(sizeof(Message));
   Message *rec = malloc(sizeof(Message));

   org->operationCode = 100;
   org->statusCode = 200;
   org->peopleCount = 2;
   org->peopleData = malloc(sizeof(Person)*2);
   
   // fill person 1
   int listSize = 3;
   org->peopleData[0].email = malloc(sizeof(char)*16);
   strcpy(org->peopleData[0].email,"teste@teste.com");
   org->peopleData[0].name = malloc(sizeof(char)*6);
   strcpy(org->peopleData[0].name, "teste");
   org->peopleData[0].lastName = malloc(sizeof(char)*11);
   strcpy(org->peopleData[0].lastName, "teste last");
   org->peopleData[0].residence = malloc(sizeof(char)*11);
   strcpy(org->peopleData[0].residence, "teste city");
   org->peopleData[0].graduation = malloc(sizeof(char)*8);
   strcpy(org->peopleData[0].graduation, "test UNI");
   org->peopleData[0].graduationYear = 3000;

   org->peopleData[0].sizeSkills = listSize;
   org->peopleData[0].skills = malloc(sizeof(char*)*listSize);
   org->peopleData[0].skills[0] = malloc(sizeof(char)*10);
   strcpy(org->peopleData[0].skills[0], "test ski0");
   org->peopleData[0].skills[1] = malloc(sizeof(char)*10);
   strcpy(org->peopleData[0].skills[1], "test ski1");
   org->peopleData[0].skills[2] = malloc(sizeof(char)*10);
   strcpy(org->peopleData[0].skills[2], "test ski2");
   
   org->peopleData[0].sizeExperiences = listSize;
   org->peopleData[0].experiences = malloc(sizeof(char*)*listSize);
   org->peopleData[0].experiences[0] = malloc(sizeof(char)*10);
   strcpy(org->peopleData[0].experiences[0], "test exp0");
   org->peopleData[0].experiences[1] = malloc(sizeof(char)*10);
   strcpy(org->peopleData[0].experiences[1], "test exp1");
   org->peopleData[0].experiences[2] = malloc(sizeof(char)*10);
   strcpy(org->peopleData[0].experiences[2], "test exp2");

   // fill person 2
   org->peopleData[1].email = malloc(sizeof(char)*16);
   strcpy(org->peopleData[1].email,"teste@teste.com");
   org->peopleData[1].name = malloc(sizeof(char)*6);
   strcpy(org->peopleData[1].name, "teste");
   org->peopleData[1].lastName = malloc(sizeof(char)*11);
   strcpy(org->peopleData[1].lastName, "teste last");
   org->peopleData[1].residence = malloc(sizeof(char)*11);
   strcpy(org->peopleData[1].residence, "teste city");
   org->peopleData[1].graduation = malloc(sizeof(char)*8);
   strcpy(org->peopleData[1].graduation, "test UNI");
   org->peopleData[1].graduationYear = 3000;

   org->peopleData[1].sizeSkills = listSize;
   org->peopleData[1].skills = malloc(sizeof(char*)*listSize);
   org->peopleData[1].skills[0] = malloc(sizeof(char)*10);
   strcpy(org->peopleData[1].skills[0], "test ski0");
   org->peopleData[1].skills[1] = malloc(sizeof(char)*10);
   strcpy(org->peopleData[1].skills[1], "test ski1");
   org->peopleData[1].skills[2] = malloc(sizeof(char)*10);
   strcpy(org->peopleData[1].skills[2], "test ski2");
   
   org->peopleData[1].sizeExperiences = listSize;
   org->peopleData[1].experiences = malloc(sizeof(char*)*listSize);
   org->peopleData[1].experiences[0] = malloc(sizeof(char)*10);
   strcpy(org->peopleData[1].experiences[0], "test exp0");
   org->peopleData[1].experiences[1] = malloc(sizeof(char)*10);
   strcpy(org->peopleData[1].experiences[1], "test exp1");
   org->peopleData[1].experiences[2] = malloc(sizeof(char)*10);
   strcpy(org->peopleData[1].experiences[2], "test exp2");

   serializeMessage(buffer, org);
   deserializeMessage(buffer, rec);

   if(org->operationCode != rec->operationCode || org->statusCode != rec->statusCode 
   || org->peopleCount != rec->peopleCount){
      puts("5 - test_serializeMessage FAILED");
      return;
   }

   for (int i = 0; i < org->peopleCount; i++){
      if(comparePerson(&org->peopleData[i], &rec->peopleData[i]) != 1){
         puts("5 - test_serializeMessage FAILED");
         return;
      }
   }
   
   puts("5 - test_serializeMessage PASSED");
}


int comparePerson(Person* p1, Person* p2){
      if(strcmp(p1->email, p2->email) != 0 || strcmp(p1->name,p2->name) != 0 
   || strcmp(p1->lastName,p2->lastName) != 0 || strcmp(p1->residence,p2->residence) != 0 
   || strcmp(p1->graduation,p2->graduation) != 0 || p1->graduationYear != p2->graduationYear 
   || p1->sizeSkills != p2->sizeSkills || p1->sizeExperiences != p2->sizeExperiences){
      return 0;
   }
   for (int i = 0; i < p2->sizeSkills; i++){
      if(strcmp(p1->skills[i],p2->skills[i]) != 0){
         return 0;
      }
   }

   for (int i = 0; i < p2->sizeExperiences; i++){
      if(strcmp(p1->experiences[i], p2->experiences[i]) != 0){
         return 0;
      }
   }

   return 1;
}

void freePerson(Person* p){
   free(p->email);
   free(p->name);
   free(p->lastName);
   free(p->residence);
   free(p->graduation);

   for (int i = 0; i < p->sizeSkills; i++){
      free(p->skills[i]);  
   }

   for (int i = 0; i < p->sizeExperiences; i++){
      free(p->experiences[i]);   
   }

   free(p->skills);
   free(p->experiences);
   free(p);
}


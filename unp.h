#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h> 
#include <sys/select.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>

/* server config */
#define SERV_PORT 8889
#define LISTENQ 3
#define BD_NAME "people.txt"
#define MAX_PEOPLE_ANSWER 10

// operationCode
#define ADD_PERSON 1
#define ADD_EXPERIENCE 2
#define GET_ALL_PER_GRADUATION 3
#define GET_ALL_PER_SKILL 4
#define GET_ALL_PER_YEAR 5
#define GET_ALL 6
#define GET_PERSON 7
#define REMOVE_PERSON 8

// statusCode
#define ALL_DONE 1
#define SENDING 2
#define ERROR -1

#define MAX_INPUT 100
#define MAXLINE 3000
#define MAX_PERSON 1200

typedef struct {
   char *email;
   char *name;
   char *lastName;
   char *residence;
   char *graduation;
   int graduationYear;
   int sizeSkills;
   char **skills;
   int sizeExperiences;
   char **experiences;
} Person;

typedef struct {
   int operationCode;
   int statusCode;
   int peopleCount;
   Person *peopleData;
} Message;

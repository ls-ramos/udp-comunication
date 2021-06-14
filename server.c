#include "serialize.h"

// int readPeople(FILE *file, Person *peopleInFile){
//     Person person;
//     int peopleCount = 0;

//     while(fread(&person, sizeof(Person), 1, file)){
//       peopleInFile[peopleCount] = person;
//       peopleCount++;
//       printf ("Email = %s\n", person.email);
//     }

//     return peopleCount;
// }

// int createNewPerson (Person newPerson){
//     FILE *file;

//     file= fopen (BD_NAME, "a");
//     fwrite (&newPerson, sizeof(Person), 1, file);
//     fclose (file);

//     return ALL_DONE;
// }

// int addExperience (Person updatePerson){
//     FILE *file;
//     Person person;
//     Person *peopleResult = malloc( MAX_PEOPLE_ANSWER * sizeof (Person));
//     int peopleCount;

//     // get all people
//     file = fopen(BD_NAME, "r");
//     peopleCount = readPeople(file,peopleResult);
//     fclose(file);

//     // update specific person
//     for(int i=0;i<peopleCount;i++){
//         if(strcmp(peopleResult[i].email, updatePerson.email) == 0){
            
//             int found = -1;
//             for(int j=0;j<MAX_LIST;j++){
//                 if(peopleResult[i].experience[j][0] == '\0'){
//                     found = j;
//                     break;
//                 }
//             }

//             if(found >= 0){
//                 strcpy(peopleResult[i].experience[found], updatePerson.experience[0]);
//             }else{
//                 return ERROR;
//             }
//         }
//     }

//     // save all back
//     file = fopen(BD_NAME, "w");
//     fwrite(peopleResult, sizeof(Person), peopleCount, file);
//     fclose(file);

//     free(peopleResult);
//     return ALL_DONE;
// }

// int getAllPeopleWithGraduation(char *graduation, Person *peopleResult){
//     FILE *file;
//     Person person;
//     Person *peopleAux = malloc( MAX_PEOPLE_ANSWER * sizeof (Person));
//     int peopleCountAux;

//     file= fopen (BD_NAME, "r");
//     peopleCountAux = readPeople(file,peopleAux);
//     fclose (file);

//     int peopleCountResult = 0;
//     for(int i=0;i<peopleCountAux;i++){
//         if(strcmp(peopleAux[i].graduation,graduation) == 0){
//             peopleResult[peopleCountResult] = peopleAux[i];
//             peopleCountResult++;
//         }
//     }
    
//     free(peopleAux);
//     return peopleCountResult;
// }

// int getAllPeopleWithSkill(char *skill, Person *peopleResult){
//     FILE *file;
//     Person person;
//     Person *peopleAux = malloc( MAX_PEOPLE_ANSWER * sizeof (Person));
//     int peopleCountAux;

//     file= fopen (BD_NAME, "r");
//     peopleCountAux = readPeople(file,peopleAux);
//     fclose (file);

//     int peopleCountResult = 0;
//     for(int i=0;i<peopleCountAux;i++){
//         for(int j=0;j<MAX_LIST;j++){
//             if(strcmp(peopleAux[i].skills[j],skill) == 0){
//                 peopleResult[peopleCountResult] = peopleAux[i];
//                 peopleCountResult++;
//                 break;
//             }
//         }
//     }
        
//     free(peopleAux);
//     return peopleCountResult;
// }

// int getAllPeopleWithGraduationYear(int graduationYear, Person *peopleResult){
//     FILE *file;
//     Person person;
//     Person *peopleAux = malloc( MAX_PEOPLE_ANSWER * sizeof (Person));
//     int peopleCountAux;

//     file= fopen (BD_NAME, "r");
//     peopleCountAux = readPeople(file,peopleAux);
//     fclose (file);

//     int peopleCountResult = 0;
//     for(int i=0;i<peopleCountAux;i++){
//         if(peopleAux[i].graduationYear == graduationYear){
//             peopleResult[peopleCountResult] = peopleAux[i];
//             peopleCountResult++;
//         }
//     }
    
//     free(peopleAux);
//     return peopleCountResult;
// }

// int getAllPeople(Person *peopleResult){
//     FILE *file;
//     Person person;
//     int peopleCount;

//     file= fopen (BD_NAME, "r");
//     peopleCount = readPeople(file,peopleResult);
//     fclose (file);

//     return peopleCount;
// }

// int getPerson(char *email, Person *peopleResult){
//     FILE *file;
//     Person person;
//     Person *peopleAux = malloc( MAX_PEOPLE_ANSWER * sizeof (Person));
//     int peopleCountAux;

//     file= fopen (BD_NAME, "r");
//     peopleCountAux = readPeople(file,peopleAux);
//     fclose (file);

//     int peopleCountResult = 0;
//     for(int i=0;i<peopleCountAux;i++){
//         if(strcmp(peopleAux[i].email,email) == 0){
//             peopleResult[peopleCountResult] = peopleAux[i];
//             peopleCountResult++;
//         }
//     }

//     free(peopleAux);
//     return peopleCountResult;
// }

// int removePerson(char *email){
//     FILE *file;
//     Person person;
//     Person *peopleAux = malloc( MAX_PEOPLE_ANSWER * sizeof (Person));
//     Person *peopleResult = malloc( MAX_PEOPLE_ANSWER * sizeof (Person));
//     int peopleCountAux;

//     file= fopen (BD_NAME, "r");
//     peopleCountAux = readPeople(file,peopleAux);
//     fclose (file);

//     int peopleCountResult = 0;
//     for(int i=0;i<peopleCountAux;i++){
//         if(strcmp(peopleAux[i].email,email) != 0){
//             peopleResult[peopleCountResult] = peopleAux[i];
//             peopleCountResult++;
//         }
//     }

//     // save all back
//     file = fopen(BD_NAME, "w");
//     fwrite(peopleResult, sizeof(Person), peopleCountResult, file);
//     fclose(file);

//     free(peopleAux);
//     free(peopleResult);
//     return ALL_DONE;
// }

// int handleRequest(Message *request, Person *peopleAnswer, int *countAnswer){
//     int operation = request->operationCode;
//     switch (operation){
//         case ADD_PERSON:
//             return createNewPerson(request->personData);
//             break;
//         case ADD_EXPERIENCE:
//             return addExperience(request->personData);
//             break;
//         case GET_ALL_PER_GRADUATION:
//             *countAnswer = getAllPeopleWithGraduation(request->personData.graduation, peopleAnswer);
//             if(*countAnswer < 0) return ERROR;
//             else if(*countAnswer <= 1) return ALL_DONE;
//             else return SENDING;
//             break;
//         case GET_ALL_PER_SKILL:
//             *countAnswer = getAllPeopleWithSkill(request->personData.skills[0], peopleAnswer);
//             if(*countAnswer < 0) return ERROR;
//             else if(*countAnswer <= 1) return ALL_DONE;
//             else return SENDING;
//             break;
//         case GET_ALL_PER_YEAR:
//             *countAnswer = getAllPeopleWithGraduationYear(request->personData.graduationYear, peopleAnswer);
//             if(*countAnswer < 0) return ERROR;
//             else if(*countAnswer <= 1) return ALL_DONE;
//             else return SENDING;
//             break;
//         case GET_ALL:
//             *countAnswer = getAllPeople(peopleAnswer);
//             if(*countAnswer < 0) return ERROR;
//             else if(*countAnswer <= 1) return ALL_DONE;
//             else return SENDING;
//             break;
//         case GET_PERSON:
//             *countAnswer = getPerson(request->personData.email, peopleAnswer);
//             if(*countAnswer > 0) return ALL_DONE;
//             else return ERROR;
//             break;
//         case REMOVE_PERSON:
//             return removePerson(request->personData.email);
//             break;
//         default:
//             return ERROR;
//             break;
//     }
// }

// void process(int new_fd) { 
//     Message *request = malloc(sizeof (Message)); 
//     Message *answer = malloc(sizeof (Message));
//     Person *peopleAnswer = malloc( MAX_PEOPLE_ANSWER * sizeof (Person)); 
//     int peopleCount;

//     puts("Reading message");
//     recvfrom(new_fd, request, sizeof (Message),0,clientAddr, sizeof clientAddr); // TODO: fix here
//     puts("Read message");

//     answer->statusCode = handleRequest(request, peopleAnswer, &peopleCount);
//     answer->operationCode = request->operationCode;

//     puts("Writing answer");
//     if(answer->statusCode != ERROR && 
//         (answer->operationCode == GET_ALL_PER_GRADUATION || 
//         answer->operationCode == GET_ALL_PER_SKILL || 
//         answer->operationCode == GET_ALL_PER_YEAR ||
//         answer->operationCode == GET_ALL ||
//         answer->operationCode == GET_PERSON) ){
//         int i = 0;
//         while(i<peopleCount-1){
//             answer->personData = peopleAnswer[i];
//             write(new_fd, answer, sizeof (Message));
//             i++;
//         }

//         if(i==peopleCount-1){
//             // write last message
//             answer->statusCode = ALL_DONE;
//             answer->personData = peopleAnswer[i];
//             write(new_fd, answer, sizeof (Message));
//         }
        
//     }else{
//         write(new_fd, answer, sizeof (Message));   
//     }
//     puts("Answer written");

//     free(request);
//     free(answer);
//     free(peopleAnswer);
// }

void dg_echo(int sockfd, struct sockaddr *pcliaddr, socklen_t clilen){
    int n;
    socklen_t len;
    for ( ; ; ) {
        
        char rawMsg[MAXLINE];
        Message request;

        char rawAns[MAXLINE];
        Message answer;

        len = clilen;
        n = recvfrom(sockfd, rawMsg, MAXLINE, 0, pcliaddr, &len);
        deserializeMessage(rawMsg,&request);

        // sleep(7);

        serializeMessage(rawAns, &request);
        puts("Answering to client");
        sendto(sockfd, rawAns, n, 0, pcliaddr, len);
    }
}

int main(int argc, char **argv){
    int sock_fd, new_fd;
    struct sockaddr_in cliaddr, servaddr;

    sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock_fd < 0)
        printf("Error creating socket %d\n",sock_fd);
    printf("Socked created %d\n",sock_fd);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons (SERV_PORT);
    
    int bindResult = bind(sock_fd, (struct sockaddr *) &servaddr, sizeof(servaddr));
    if(bindResult < 0)
        printf("Error binding %d\n",bindResult);
    printf("Bind completed %d\n", bindResult);

    printf("Server will listen at addrs %s and port %d\n",inet_ntoa(servaddr.sin_addr),servaddr.sin_port);
    
    dg_echo (sock_fd, (struct sockaddr *) &cliaddr, sizeof(cliaddr));
}


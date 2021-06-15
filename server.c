#include "serialize.h"

int readPeople(FILE *file, Person *peopleInFile){
    char buffer[MAX_PERSON];

    Person* person = malloc(sizeof(Person));
    int peopleCount = 0;

    while(fread(buffer, MAX_PERSON, 1, file)){
        deserializePerson(buffer, person);

        peopleInFile[peopleCount] = person[0];
        peopleCount++;
        printf ("Email = %s\n", person->email);
    }
    
    // freePerson(person);
    return peopleCount;
}

int writePeople(Person* newPeople, int peopleCount, int append){
    FILE *file;
    if(append == 1){
        file= fopen (BD_NAME, "a");
    }else{
        file= fopen (BD_NAME, "w");
    }
    

    for (int i = 0; i < peopleCount; i++){
        char buffer[MAX_PERSON];
        serializePerson(buffer,&newPeople[i]);
        fwrite (buffer, MAX_PERSON, 1, file);    
    }

    fclose (file);
    return ALL_DONE;
}


int createNewPerson (Person* newPerson){
    return writePeople(newPerson, 1, 1);
}

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
//                 if(peopleResult[i].experiences[j][0] == '\0'){
//                     found = j;
//                     break;
//                 }
//             }

//             if(found >= 0){
//                 strcpy(peopleResult[i].experiences[found], updatePerson.experiences[0]);
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

int getAllPeopleWithGraduation(char *graduation, Person *peopleResult){
    FILE *file;

    Person *peopleAux = malloc( MAX_PEOPLE_ANSWER * sizeof (Person));
    int peopleCountAux;

    file= fopen (BD_NAME, "r");
    peopleCountAux = readPeople(file,peopleAux);
    fclose (file);

    int peopleCountResult = 0;
    for(int i=0;i<peopleCountAux;i++){
        if(strcmp(peopleAux[i].graduation,graduation) == 0){
            peopleResult[peopleCountResult] = peopleAux[i];
            peopleCountResult++;
        }
    }
    
    // freePeople(peopleAux,peopleCountAux);
    return peopleCountResult;
}

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

int getAllPeopleWithGraduationYear(int graduationYear, Person *peopleResult){
    FILE *file;

    Person *peopleAux = malloc( MAX_PEOPLE_ANSWER * sizeof (Person));
    int peopleCountAux;

    file= fopen (BD_NAME, "r");
    peopleCountAux = readPeople(file,peopleAux);
    fclose (file);

    int peopleCountResult = 0;
    for(int i=0;i<peopleCountAux;i++){
        if(peopleAux[i].graduationYear == graduationYear){
            peopleResult[peopleCountResult] = peopleAux[i];
            peopleCountResult++;
        }
    }
    
    // freePeople(peopleAux,peopleCountAux);
    return peopleCountResult;
}

int getAllPeople(Person *peopleResult){
    FILE *file;

    int peopleCount;

    file= fopen (BD_NAME, "r");
    peopleCount = readPeople(file,peopleResult);
    fclose (file);

    return peopleCount;
}

int getPerson(char *email, Person *peopleResult){
    FILE *file;

    Person *peopleAux = malloc( MAX_PEOPLE_ANSWER * sizeof (Person));
    int peopleCountAux;

    file= fopen (BD_NAME, "r");
    peopleCountAux = readPeople(file,peopleAux);
    fclose (file);

    int peopleCountResult = 0;
    for(int i=0;i<peopleCountAux;i++){
        if(strcmp(peopleAux[i].email,email) == 0){
            peopleResult[peopleCountResult] = peopleAux[i];
            peopleCountResult++;
        }
    }

    // freePeople(peopleAux,peopleCountAux);
    return peopleCountResult;
}

int removePerson(char *email){
    FILE *file;
    Person person;
    Person *peopleAux = malloc( MAX_PEOPLE_ANSWER * sizeof (Person));
    Person *peopleResult = malloc( MAX_PEOPLE_ANSWER * sizeof (Person));
    int peopleCountAux;

    file= fopen (BD_NAME, "r");
    peopleCountAux = readPeople(file,peopleAux);
    fclose (file);

    int peopleCountResult = 0;
    for(int i=0;i<peopleCountAux;i++){
        if(strcmp(peopleAux[i].email,email) != 0){
            peopleResult[peopleCountResult] = peopleAux[i];
            peopleCountResult++;
        }
    }

    // save all back
    writePeople(peopleResult, peopleCountResult, 0);

    // freePeople(peopleAux,peopleCountAux);
    freePeople(peopleResult, peopleCountResult);
    return ALL_DONE;
}

int handleRequest(Message *request, Person *peopleAnswer, int *countAnswer){
    int operation = request->operationCode;
    switch (operation){
        case ADD_PERSON:
            return createNewPerson(&request->peopleData[0]);
            break;
        // case ADD_EXPERIENCE:
            // return addExperience(request->peopleData[0]);
            // break;
        case GET_ALL_PER_GRADUATION:
            *countAnswer = getAllPeopleWithGraduation(request->peopleData[0].graduation, peopleAnswer);
            if(*countAnswer < 0) return ERROR;
            else return ALL_DONE;
            break;
        // case GET_ALL_PER_SKILL:
        //     *countAnswer = getAllPeopleWithSkill(request->peopleData[0].skills[0], peopleAnswer);
        //     if(*countAnswer < 0) return ERROR;
        //     else if(*countAnswer <= 1) return ALL_DONE;
        //     else return SENDING;
        //     break;
        case GET_ALL_PER_YEAR:
            *countAnswer = getAllPeopleWithGraduationYear(request->peopleData[0].graduationYear, peopleAnswer);
            if(*countAnswer < 0) return ERROR;
            else return ALL_DONE;
            break;
        case GET_ALL:
            *countAnswer = getAllPeople(peopleAnswer);
            if(*countAnswer < 0) return ERROR;
            else return ALL_DONE;
            break;
        case GET_PERSON:
            *countAnswer = getPerson(request->peopleData[0].email, peopleAnswer);
            if(*countAnswer > 0) return ALL_DONE;
            else return ERROR;
            break;
        case REMOVE_PERSON:
            return removePerson(request->peopleData[0].email);
            break;
        default:
            return ERROR;
            break;
    }
}

Message* process(Message* request) {  
    Message *answer = malloc(sizeof (Message));
    Person *peopleAnswer = malloc( MAX_PEOPLE_ANSWER * sizeof (Person)); 
    int peopleCount = 0;

    answer->statusCode = handleRequest(request, peopleAnswer, &peopleCount);
    answer->operationCode = request->operationCode;
    answer->peopleCount = peopleCount; 

    if(answer->statusCode != ERROR && answer->peopleCount > 0){
        answer->peopleData = malloc(sizeof(Person)*answer->peopleCount);
        for (int i = 0; i < answer->peopleCount; i++){
            memcpy(&answer->peopleData[i],&peopleAnswer[i],sizeof(Person));
        }
    }
    return answer;
}

void serverLoop(int sockfd, struct sockaddr *pcliaddr, socklen_t clilen){
    int n;
    socklen_t len;
    while(1) {
        char rawMsg[MAXLINE];
        Message* request = malloc(sizeof(Message));

        char rawAns[MAXLINE];
        Message* answer;

        len = clilen;
        n = recvfrom(sockfd, rawMsg, MAXLINE, 0, pcliaddr, &len);
        deserializeMessage(rawMsg,request);

        printf("Request received for operation %d", request->operationCode);

        answer = process(request);
        // sleep(7); - to force timeout

        serializeMessage(rawAns, answer);
        puts("Answering to client");
        sendto(sockfd, rawAns, MAXLINE, 0, pcliaddr, len);

        freeMessage(request);
        freeMessage(answer);
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
    
    serverLoop(sock_fd, (struct sockaddr *) &cliaddr, sizeof(cliaddr));
}


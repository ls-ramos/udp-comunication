#include "serialize.h"

int getUDPSocket(char *serverIP, struct sockaddr_in *servaddrReturn);
void clientUDPLoop();
void addPersonUDP(int sock_fd, const struct sockaddr *pservaddr,socklen_t servlen);
void addExperienceUDP(int sock_fd, const struct sockaddr *pservaddr,socklen_t servlen);
void getAllPeopleWithGraduationUDP(int sock_fd, const struct sockaddr *pservaddr,socklen_t servlen);
void getAllPeopleWithSkillUDP(int sock_fd, const struct sockaddr *pservaddr,socklen_t servlen);
void getAllPeopleWithGraduationYearUDP(int sock_fd, const struct sockaddr *pservaddr,socklen_t servlen);
void getAllPeopleUDP(int sock_fd, const struct sockaddr *pservaddr,socklen_t servlen);
void getPersonUDP(int sock_fd, const struct sockaddr *pservaddr,socklen_t servlen);
void removePersonUDP(int sock_fd, const struct sockaddr *pservaddr,socklen_t servlen);

int sendToServer(Message *request, char* recvline, int sockfd, const struct sockaddr *pservaddr,socklen_t servlen);

int getOperationFromUser();
int getAdmin();

void printPerson(Person* person);

int main(int argc, char **argv){
    if(argc < 2){
        puts("Missing positional argument");
        return 0;
    }

    struct sockaddr_in *servaddr = malloc(sizeof(struct sockaddr_in));
    int socket;

    printf("! Welcome to the UDP comunication demostration !\n");
    socket = getUDPSocket(argv[1], servaddr);
    clientUDPLoop(socket, (struct sockaddr *) servaddr, sizeof(*servaddr));

    return 1;
}

int getOperationFromUser(int isAdmin){
    char *opCodeString = malloc(MAX_INPUT*sizeof(char));
    int opCode;
    while(1){
        printf("Which action would like to perform:\n");
        
        if(isAdmin){
            printf("1 - Add person\n"
            "2 - Add expirience to person\n"
            "8 - Remove person\n");
        }

        printf("3 - Get all students with graduation\n"
        "4 - Get all students with skill\n"
        "5 - Get all students with graduation year\n"
        "6 - Get all students\n"
        "7 - Get person\n"
        "0 - Exit\n");

        printf("Enter the operation number: ");
        fgets(opCodeString, MAX_INPUT, stdin);
        opCode = atoi(opCodeString);

        if((isAdmin && opCode > 0 && opCode < 9) || (opCode > 2 && opCode < 8) || opCode == 0){
            break;            
        }
        printf("Invalid input ! Try again...\n\n");
    }
    return opCode;
}

int getAdmin(){
    char *isAdminString = malloc(MAX_INPUT*sizeof(char));
    int isAdmin;
    while(1){
        printf("Are you an admin (0-No / 1-Yes): ");
        fgets(isAdminString, MAX_INPUT, stdin);
        isAdmin = atoi(isAdminString);

        if(isAdmin == 0 || isAdmin == 1){
            break;
        }
        printf("Invalid input ! Try again...");
    }
    return isAdmin;
}

int sendToServer(Message *request, char* recvline, int sockfd, const struct sockaddr *pservaddr,socklen_t servlen){
    char sendline[MAXLINE];

    int n, selRes;
    fd_set rset;
    FD_ZERO(&rset);
    FD_SET(sockfd, &rset);

    int size = serializeMessage(sendline,request);
    sendto(sockfd,sendline,size,0,pservaddr,servlen);
    
    struct timeval timeout;    
    timeout.tv_sec = 1000;
    timeout.tv_usec = 0;
    selRes = select(sockfd+1, &rset, NULL,NULL,&timeout);

    if(selRes > 0){
        printf("Reading answer!\n");
        n = recvfrom(sockfd, recvline, MAXLINE, 0, NULL, NULL);
        recvline[n] = '\0'; /* null terminate */
        return 0;
    }else if(selRes == 0){
        printf("Timeout occured !\n");
        return 1;
    } else {
        printf("Erro !\n");
        return 2;
    }   

}

/** 
 * UDP functions
*/
int getUDPSocket(char *serverIP, struct sockaddr_in *servaddr){
    int sockfd;

    bzero(servaddr, sizeof(*servaddr));
    servaddr->sin_family = AF_INET;
    servaddr->sin_port = htons(SERV_PORT);
    
    inet_pton(AF_INET, serverIP, &servaddr->sin_addr);
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    
    return sockfd;
}

void clientUDPLoop(int sock_fd, const struct sockaddr *pservaddr,socklen_t servlen){
    
    int opCode, isAdmin;
    printf("! Welcome to the UDP comunication demonstration !\n");
    
    isAdmin = getAdmin();

    while(1){
        opCode = getOperationFromUser(isAdmin);

        switch (opCode){
            case 0:
                return; // user requested to end the program
                break;
            case ADD_PERSON:
                addPersonUDP(sock_fd, pservaddr, servlen);
                break;
            case ADD_EXPERIENCE:
                addExperienceUDP(sock_fd, pservaddr, servlen);
                break;
            case GET_ALL_PER_GRADUATION:
                getAllPeopleWithGraduationUDP(sock_fd, pservaddr, servlen);
                break;
            case GET_ALL_PER_SKILL:
                getAllPeopleWithSkillUDP(sock_fd, pservaddr, servlen);
                break;
            case GET_ALL_PER_YEAR:
                getAllPeopleWithGraduationYearUDP(sock_fd, pservaddr, servlen);
                break;
            case GET_ALL:
                getAllPeopleUDP(sock_fd, pservaddr, servlen);
                break;
            case GET_PERSON:
                getPersonUDP(sock_fd, pservaddr, servlen);
                break;
            case REMOVE_PERSON:
                removePersonUDP(sock_fd, pservaddr, servlen);
                break;
            default:
                break;
        }
    }
}

void addPersonUDP(int sock_fd, const struct sockaddr *pservaddr,socklen_t servlen){ 
    char rawAnswer[MAXLINE+1 ];

    Message *request = malloc(sizeof(Message));
    Message *answer = malloc(sizeof(Message));

    request->operationCode = ADD_PERSON;
    request->statusCode = 0;
    request->peopleCount = 1; 
    request->peopleData = malloc(sizeof(Person));

    request->peopleData[0].email = malloc(MAX_INPUT*sizeof(char));
    request->peopleData[0].name = malloc(MAX_INPUT*sizeof(char));
    request->peopleData[0].lastName = malloc(MAX_INPUT*sizeof(char));
    request->peopleData[0].residence = malloc(MAX_INPUT*sizeof(char));
    request->peopleData[0].graduation = malloc(MAX_INPUT*sizeof(char));
    request->peopleData[0].sizeSkills = 0;
    request->peopleData[0].sizeExperiences = 0;
    char *graduationYearString = malloc(MAX_INPUT*sizeof(char));
    int graduationYear;

    printf("Enter the email: ");
    fgets(request->peopleData[0].email, MAX_INPUT, stdin);

    printf("Enter the name: ");
    fgets(request->peopleData[0].name, MAX_INPUT, stdin);

    printf("Enter the last name: ");
    fgets(request->peopleData[0].lastName, MAX_INPUT, stdin);

    printf("Enter the residence: ");
    fgets(request->peopleData[0].residence, MAX_INPUT, stdin);
    
    printf("Enter the graduation: ");
    fgets(request->peopleData[0].graduation, MAX_INPUT, stdin);

    printf("Enter the graduation year: ");
    fgets(graduationYearString, MAX_INPUT, stdin);
    graduationYear = atoi(graduationYearString);
    request->peopleData[0].graduationYear = graduationYear;

    int error = sendToServer(request, rawAnswer, sock_fd, pservaddr, servlen);
    if(error == 0){
        deserializeMessage(rawAnswer, answer);

        puts("----------- Operation ADD_PERSON ----------------\n"); 
        printf("Status: %d\n", answer->statusCode);
        puts("------------------- End answer ------------------\n");
    }else{
        puts("----------- Operation ADD_PERSON ----------------\n"); 
        printf("Error or timeout\n");
        puts("------------------- End answer ------------------\n");
    }

    freeMessage(request);
    freeMessage(answer);
}

void addExperienceUDP(int sock_fd, const struct sockaddr *pservaddr,socklen_t servlen){
    char rawAnswer[MAXLINE+1];

    Message *request = malloc(sizeof(Message));
    Message *answer = malloc(sizeof(Message));

    request->operationCode = ADD_EXPERIENCE;
    request->statusCode = 0;
    request->peopleCount = 1; 
    request->peopleData = malloc(sizeof(Person));

    request->peopleData[0].email = malloc(MAX_INPUT*sizeof(char));
    strcpy(request->peopleData[0].email, "");
    request->peopleData[0].name = malloc(MAX_INPUT*sizeof(char));
    strcpy(request->peopleData[0].name, "");
    request->peopleData[0].lastName = malloc(MAX_INPUT*sizeof(char));
    strcpy(request->peopleData[0].lastName, "");
    request->peopleData[0].residence = malloc(MAX_INPUT*sizeof(char));
    strcpy(request->peopleData[0].residence, "");
    request->peopleData[0].graduation = malloc(MAX_INPUT*sizeof(char));
    strcpy(request->peopleData[0].graduation, "");
    request->peopleData[0].sizeSkills = 0;
    request->peopleData[0].sizeExperiences = 0;
    request->peopleData[0].graduationYear = 0;

    printf("Enter the email: ");
    fgets(request->peopleData[0].email, MAX_INPUT, stdin);

    request->peopleData[0].sizeExperiences = 1;
    request->peopleData[0].experiences = malloc(sizeof(char*));
    request->peopleData[0].experiences[0] = malloc(sizeof(char) * MAX_INPUT);
    
    printf("Enter the new experience: ");
    fgets(request->peopleData[0].experiences[0], MAX_INPUT, stdin);

    int error = sendToServer(request, rawAnswer, sock_fd, pservaddr, servlen);
    if(error == 0){
        deserializeMessage(rawAnswer, answer);

        puts("----------- Operation ADD_EXPERIENCE ----------------\n"); 
        printf("Status: %d\n", answer->operationCode);
        puts("------------------- End answer ------------------\n");
    }else{
        puts("----------- Operation ADD_EXPERIENCE ----------------\n"); 
        printf("Error or timeout\n");
        puts("------------------- End answer ------------------\n");
    }

    freeMessage(request);
    freeMessage(answer);
}

void getAllPeopleWithGraduationUDP(int sock_fd, const struct sockaddr *pservaddr,socklen_t servlen){
    char rawAnswer[MAXLINE+1];

    Message *request = malloc(sizeof(Message));
    Message *answer = malloc(sizeof(Message));

    request->operationCode = GET_ALL_PER_GRADUATION;
    request->statusCode = 0;
    request->peopleCount = 1; 
    request->peopleData = malloc(sizeof(Person));

    request->peopleData[0].email = malloc(MAX_INPUT*sizeof(char));
    strcpy(request->peopleData[0].email, "");
    request->peopleData[0].name = malloc(MAX_INPUT*sizeof(char));
    strcpy(request->peopleData[0].name, "");
    request->peopleData[0].lastName = malloc(MAX_INPUT*sizeof(char));
    strcpy(request->peopleData[0].lastName, "");
    request->peopleData[0].residence = malloc(MAX_INPUT*sizeof(char));
    strcpy(request->peopleData[0].residence, "");
    request->peopleData[0].graduation = malloc(MAX_INPUT*sizeof(char));
    strcpy(request->peopleData[0].graduation, "");
    request->peopleData[0].sizeSkills = 0;
    request->peopleData[0].sizeExperiences = 0;
    request->peopleData[0].graduationYear = 0;

    printf("Enter the graduation: ");
    fgets(request->peopleData[0].graduation, MAX_INPUT, stdin);    

    int error = sendToServer(request, rawAnswer, sock_fd, pservaddr, servlen);
    if(error == 0){
        deserializeMessage(rawAnswer, answer);

        puts("----------- Operation GET_ALL_PER_GRADUATION ----------------\n"); 
        for (int i = 0; i < answer->peopleCount; i++){
            printf("************ Person %d ***************\n", i);
            printPerson(&answer->peopleData[i]);
            printf("********** End Person %d *************\n\n", i);
        }
        puts("------------------- End answer ------------------\n");
    }else{
        puts("----------- Operation GET_ALL_PER_GRADUATION ----------------\n"); 
        printf("Error or timeout\n");
        puts("------------------- End answer ------------------\n");
    }

    freeMessage(request);
    freeMessage(answer);
}

void getAllPeopleWithSkillUDP(int sock_fd, const struct sockaddr *pservaddr,socklen_t servlen){
    char rawAnswer[MAXLINE+1];

    Message *request = malloc(sizeof(Message));
    Message *answer = malloc(sizeof(Message));

    request->operationCode = GET_ALL_PER_SKILL;
    request->statusCode = 0;
    request->peopleCount = 1; 
    request->peopleData = malloc(sizeof(Person));

    request->peopleData[0].email = malloc(MAX_INPUT*sizeof(char));
    strcpy(request->peopleData[0].email, "");
    request->peopleData[0].name = malloc(MAX_INPUT*sizeof(char));
    strcpy(request->peopleData[0].name, "");
    request->peopleData[0].lastName = malloc(MAX_INPUT*sizeof(char));
    strcpy(request->peopleData[0].lastName, "");
    request->peopleData[0].residence = malloc(MAX_INPUT*sizeof(char));
    strcpy(request->peopleData[0].residence, "");
    request->peopleData[0].graduation = malloc(MAX_INPUT*sizeof(char));
    strcpy(request->peopleData[0].graduation, "");
    request->peopleData[0].sizeSkills = 0;
    request->peopleData[0].sizeExperiences = 0;
    request->peopleData[0].graduationYear = 0;

    request->peopleData[0].sizeSkills = 1;
    request->peopleData[0].skills = malloc(sizeof(char*));
    request->peopleData[0].skills[0] = malloc(sizeof(char) * MAX_INPUT);
    
    printf("Enter the skill: ");
    fgets(request->peopleData[0].skills[0], MAX_INPUT, stdin);

    int error = sendToServer(request, rawAnswer, sock_fd, pservaddr, servlen);
    if(error == 0){
        deserializeMessage(rawAnswer, answer);

        puts("----------- Operation GET_ALL_PER_SKILL ----------------\n"); 
        for (int i = 0; i < answer->peopleCount; i++){
            printf("************ Person %d ***************\n", i);
            printPerson(&answer->peopleData[i]);
            printf("********** End Person %d *************\n\n", i);
        }
        puts("------------------- End answer ------------------\n");
    }else{
        puts("----------- Operation GET_ALL_PER_SKILL ----------------\n"); 
        printf("Error or timeout\n");
        puts("------------------- End answer ------------------\n");
    }

    freeMessage(request);
    freeMessage(answer);
}

void getAllPeopleWithGraduationYearUDP(int sock_fd, const struct sockaddr *pservaddr,socklen_t servlen){

    char *graduationYearString = malloc(MAX_INPUT*sizeof(char));
    int graduationYear;

    char rawAnswer[MAXLINE+1];

    Message *request = malloc(sizeof(Message));
    Message *answer = malloc(sizeof(Message));

    request->operationCode = GET_ALL_PER_YEAR;
    request->statusCode = 0;
    request->peopleCount = 1; 
    request->peopleData = malloc(sizeof(Person));

    request->peopleData[0].email = malloc(MAX_INPUT*sizeof(char));
    strcpy(request->peopleData[0].email, "");
    request->peopleData[0].name = malloc(MAX_INPUT*sizeof(char));
    strcpy(request->peopleData[0].name, "");
    request->peopleData[0].lastName = malloc(MAX_INPUT*sizeof(char));
    strcpy(request->peopleData[0].lastName, "");
    request->peopleData[0].residence = malloc(MAX_INPUT*sizeof(char));
    strcpy(request->peopleData[0].residence, "");
    request->peopleData[0].graduation = malloc(MAX_INPUT*sizeof(char));
    strcpy(request->peopleData[0].graduation, "");
    request->peopleData[0].sizeSkills = 0;
    request->peopleData[0].sizeExperiences = 0;
    request->peopleData[0].graduationYear = 0;

    printf("Enter the graduation year: ");
    fgets(graduationYearString, MAX_INPUT, stdin);
    graduationYear = atoi(graduationYearString);
    request->peopleData[0].graduationYear = graduationYear;

    int error = sendToServer(request, rawAnswer, sock_fd, pservaddr, servlen);
    if(error == 0){
        deserializeMessage(rawAnswer, answer);

        puts("----------- Operation GET_ALL_PER_YEAR ----------------\n"); 
        for (int i = 0; i < answer->peopleCount; i++){
            printf("************ Person %d ***************\n", i);
            printPerson(&answer->peopleData[i]);
            printf("********** End Person %d *************\n\n", i);
        }
        puts("------------------- End answer ------------------\n");
    }else{
        puts("----------- Operation GET_ALL_PER_YEAR ----------------\n"); 
        printf("Error or timeout\n");
        puts("------------------- End answer ------------------\n");
    }

    freeMessage(request);
    freeMessage(answer);
}

void getAllPeopleUDP(int sock_fd, const struct sockaddr *pservaddr,socklen_t servlen){
    char rawAnswer[MAXLINE+1];

    Message *request = malloc(sizeof(Message));
    Message *answer = malloc(sizeof(Message));

    request->operationCode = GET_ALL;
    request->statusCode = 0;
    request->peopleCount = 0; 

    int error = sendToServer(request, rawAnswer, sock_fd, pservaddr, servlen);
    if(error == 0){
        deserializeMessage(rawAnswer, answer);

        puts("----------- Operation GET_ALL_PEOPLE ----------------\n"); 
        for (int i = 0; i < answer->peopleCount; i++){
            printf("************ Person %d ***************\n", i);
            printPerson(&answer->peopleData[i]);
            printf("********** End Person %d *************\n\n", i);
        }
        
        puts("------------------- End answer ------------------\n");
    }else{
        puts("----------- Operation GET_ALL_PEOPLE ----------------\n"); 
        printf("Error or timeout\n");
        puts("------------------- End answer ------------------\n");
    }

    freeMessage(request);
    freeMessage(answer);
}

void getPersonUDP(int sock_fd, const struct sockaddr *pservaddr,socklen_t servlen){
    char rawAnswer[MAXLINE+1];

    Message *request = malloc(sizeof(Message));
    Message *answer = malloc(sizeof(Message));

    request->operationCode = GET_PERSON;
    request->statusCode = 0;
    request->peopleCount = 1; 
    request->peopleData = malloc(sizeof(Person));

    request->peopleData[0].email = malloc(MAX_INPUT*sizeof(char));
    strcpy(request->peopleData[0].email, "");
    request->peopleData[0].name = malloc(MAX_INPUT*sizeof(char));
    strcpy(request->peopleData[0].name, "");
    request->peopleData[0].lastName = malloc(MAX_INPUT*sizeof(char));
    strcpy(request->peopleData[0].lastName, "");
    request->peopleData[0].residence = malloc(MAX_INPUT*sizeof(char));
    strcpy(request->peopleData[0].residence, "");
    request->peopleData[0].graduation = malloc(MAX_INPUT*sizeof(char));
    strcpy(request->peopleData[0].graduation, "");
    request->peopleData[0].sizeSkills = 0;
    request->peopleData[0].sizeExperiences = 0;
    request->peopleData[0].graduationYear = 0;

    printf("Enter the email: ");
    fgets(request->peopleData[0].email, MAX_INPUT, stdin);

    int error = sendToServer(request, rawAnswer, sock_fd, pservaddr, servlen);
    if(error == 0){
        deserializeMessage(rawAnswer, answer);

        puts("----------- Operation GET_PERSON ----------------\n"); 
        for (int i = 0; i < answer->peopleCount; i++){
            printf("************ Person %d ***************\n", i);
            printPerson(&answer->peopleData[i]);
            printf("********** End Person %d *************\n\n", i);
        }
        puts("------------------- End answer ------------------\n");
    }else{
        puts("----------- Operation GET_PERSON ----------------\n"); 
        printf("Error or timeout\n");
        puts("------------------- End answer ------------------\n");
    }

    freeMessage(request);
    freeMessage(answer);
}

void removePersonUDP(int sock_fd, const struct sockaddr *pservaddr,socklen_t servlen){
    char rawAnswer[MAXLINE+1];

    Message *request = malloc(sizeof(Message));
    Message *answer = malloc(sizeof(Message));

    request->operationCode = REMOVE_PERSON;
    request->statusCode = 0;
    request->peopleCount = 1; 
    request->peopleData = malloc(sizeof(Person));

    request->peopleData[0].email = malloc(MAX_INPUT*sizeof(char));
    strcpy(request->peopleData[0].email, "");
    request->peopleData[0].name = malloc(MAX_INPUT*sizeof(char));
    strcpy(request->peopleData[0].name, "");
    request->peopleData[0].lastName = malloc(MAX_INPUT*sizeof(char));
    strcpy(request->peopleData[0].lastName, "");
    request->peopleData[0].residence = malloc(MAX_INPUT*sizeof(char));
    strcpy(request->peopleData[0].residence, "");
    request->peopleData[0].graduation = malloc(MAX_INPUT*sizeof(char));
    strcpy(request->peopleData[0].graduation, "");
    request->peopleData[0].sizeSkills = 0;
    request->peopleData[0].sizeExperiences = 0;
    request->peopleData[0].graduationYear = 0;

    printf("Enter the email: ");
    fgets(request->peopleData[0].email, MAX_INPUT, stdin);

    int error = sendToServer(request, rawAnswer, sock_fd, pservaddr, servlen);
    if(error == 0){
        deserializeMessage(rawAnswer, answer);

        puts("----------- Operation REMOVE_PERSON ----------------\n"); 
        printf("Status: %d\n", answer->statusCode);
        puts("------------------- End answer ------------------\n");
    }else{
        puts("----------- Operation REMOVE_PERSON ----------------\n"); 
        printf("Error or timeout\n");
        puts("------------------- End answer ------------------\n");
    }

    freeMessage(request);
    freeMessage(answer);
}


void printPerson(Person* person){
    printf("Email: %s\nName:%s\nLastName:%s\nResidence:%s\nGraduation:%s\nGraduationYear:%d\n"
        ,person->email,person->name,person->lastName,person->residence,person->graduation, person->graduationYear);
    
    printf("Skills: ");
    for(int i=0;i<person->sizeSkills;i++){
        printf("%s",person->skills[i]);
        printf(", ");
        
    }
    printf("\n");

    printf("Experience: ");
    for(int i=0;i<person->sizeExperiences;i++){
        printf("%s",person->experiences[i]);
        printf(", ");
    }
    printf("\n");

}
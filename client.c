#include "serialize.h"

int getUDPSocket(char *serverIP, struct sockaddr_in *servaddrReturn);
void clientUDPLoop();
void addPersonUDP(int sock_fd, const struct sockaddr *pservaddr,socklen_t servlen);
void addExperienceUDP(int sock_fd);
void getAllPeopleWithGraduationUDP(int sock_fd);
void getAllPeopleWithSkillUDP(int sock_fd);
void getAllPeopleWithGraduationYearUDP(int sock_fd);
void getAllPeopleUDP(int sock_fd);
void getPersonUDP(int sock_fd);
void removePersonUDP(int sock_fd);

int sendToServer(char *sendline, char* recvline, int sockfd, const struct sockaddr *pservaddr,socklen_t servlen);

int getOperationFromUser();
int getAdmin();

int main(int argc, char **argv){
    if(argc < 2){
        puts("Missing positional argument");
        return 0;
    }

    struct sockaddr_in *servaddr = malloc(sizeof(struct sockaddr_in));
    int socket;

    // int sockfd;

    // bzero(servaddr, sizeof(*servaddr));
    // servaddr->sin_family = AF_INET;
    // servaddr->sin_port = htons(SERV_PORT);
    
    // inet_pton(AF_INET, argv[1], &servaddr->sin_addr);
    // sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // int n, selRes;
    // fd_set rset;
    // FD_ZERO(&rset);
    // FD_SET(sockfd, &rset);

    // char sendline[MAXLINE] = "teste";
    // char recvline[MAXLINE];
    // sendto(sockfd,sendline,strlen(sendline),0,servaddr,sizeof(*servaddr));
    
    // struct timeval timeout;    
    // timeout.tv_sec = 5;
    // timeout.tv_usec = 0;
    // selRes = select(sockfd+1, &rset, NULL,NULL,&timeout);

    // if(selRes > 0){
    //     printf("Reading answer!\n");
    //     n = recvfrom (sockfd, recvline, MAXLINE, 0, NULL, NULL);
    //     // recvline[n] = '\0'; /* null terminate */
    //     fputs(recvline, stdout);
    //     return 0;
    // }else if(selRes == 0){
    //     printf("Timeout occured !\n");
    //     return 1;
    // } else {
    //     printf("Erro !\n");
    //     return 2;
    // }   


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

int sendToServer(char *request, char* recvline, int sockfd, const struct sockaddr *pservaddr,socklen_t servlen){
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
        n = recvfrom (sockfd, recvline, MAXLINE, 0, NULL, NULL);
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
                addExperienceUDP(sock_fd);
                break;
            case GET_ALL_PER_GRADUATION:
                getAllPeopleWithGraduationUDP(sock_fd);
                break;
            case GET_ALL_PER_SKILL:
                getAllPeopleWithSkillUDP(sock_fd);
                break;
            case GET_ALL_PER_YEAR:
                getAllPeopleWithGraduationYearUDP(sock_fd);
                break;
            case GET_ALL:
                getAllPeopleUDP(sock_fd);
                break;
            case GET_PERSON:
                getPersonUDP(sock_fd);
                break;
            case REMOVE_PERSON:
                removePersonUDP(sock_fd);
                break;
            default:
                break;
        }
    }
}

void addPersonUDP(int sock_fd, const struct sockaddr *pservaddr,socklen_t servlen){ 
    char rawRequest[MAXLINE];
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
}

void addExperienceUDP(int sock_fd){

    char *email = malloc(MAX_INPUT*sizeof(char));
    char *expirence = malloc(MAX_INPUT*sizeof(char));
    
    printf("Enter the email of the user: ");
    fgets(email, MAX_INPUT, stdin);

    printf("Enter the new expirence: ");
    fgets(expirence, MAX_INPUT, stdin);

    // puts("Sending message");
    // write(sock_fd, request, sizeof (Message));
    // read(sock_fd, answer, sizeof (Message));
    // puts("----------- Starting answer ----------------\n"); 
    // printf("Operation: %d Status: %d\n", answer->operationCode, answer->statusCode);
    // puts("----------- End answer ---------------------\n");

    // free(answer);
    // free(request);
}

void getAllPeopleWithGraduationUDP(int sock_fd){
    // Message *request = malloc(sizeof (Message));
    // Message *answer = malloc(sizeof (Message));
    // request->operationCode = GET_ALL_PER_GRADUATION;

    char *graduation = malloc(MAX_INPUT*sizeof(char));

    printf("Enter the graduation year: ");
    fgets(graduation, MAX_INPUT, stdin);

    // puts("Sending message");    
    // write(sock_fd, request, sizeof (Message));
    
    // puts("----------- Starting answer ----------------\n");
    // do{
    //     read(sock_fd, answer, sizeof (Message));
    //     printPerson(answer->personData);
    //     printf("******************************\n");
    // }while(answer->statusCode == SENDING);
    // printf("Operation: %d Status: %d\n",answer->operationCode,answer->statusCode);
    // puts("----------- End answer ---------------------\n");

    // free(answer);
    // free(request);
}

void getAllPeopleWithSkillUDP(int sock_fd){
    // Message *request = malloc(sizeof (Message));
    // Message *answer = malloc(sizeof (Message));
    // request->operationCode = GET_ALL_PER_SKILL;

    char *skill = malloc(MAX_INPUT*sizeof(char));

    printf("Enter the skill: ");
    fgets(skill, MAX_INPUT, stdin);

    // puts("Sending message");    
    // write(sock_fd, request, sizeof (Message));
    
    // puts("----------- Starting answer ----------------\n");
    // do{
    //     read(sock_fd, answer, sizeof (Message));
    //     printPerson(answer->personData);
    //     printf("******************************\n");
    // }while(answer->statusCode == SENDING);
    // printf("Operation: %d Status: %d\n",answer->operationCode,answer->statusCode);
    // puts("----------- End answer ---------------------\n");

    // free(answer);
    // free(request);
}

void getAllPeopleWithGraduationYearUDP(int sock_fd){
    // Message *request = malloc(sizeof (Message));
    // Message *answer = malloc(sizeof (Message));
    // request->operationCode = GET_ALL_PER_YEAR;

    char *graduationYearString = malloc(MAX_INPUT*sizeof(char));
    int graduationYear;

    printf("Enter the graduation year: ");
    fgets(graduationYearString, MAX_INPUT, stdin);
    graduationYear = atoi(graduationYearString);

    // puts("Sending message");    
    // write(sock_fd, request, sizeof (Message));
    
    // puts("----------- Starting answer ----------------\n");
    // do{
    //     read(sock_fd, answer, sizeof (Message));
    //     printPerson(answer->personData);
    //     printf("******************************\n");
    // }while(answer->statusCode == SENDING);
    // printf("Operation: %d Status: %d\n",answer->operationCode,answer->statusCode);
    // puts("----------- End answer ---------------------\n");

    // free(answer);
    // free(request);
}

void getAllPeopleUDP(int sock_fd){
    // Message *request = malloc(sizeof (Message));
    // Message *answer = malloc(sizeof (Message));

    // request->operationCode = GET_ALL;

    // puts("Sending message");    
    // write(sock_fd, request, sizeof (Message));
    
    // puts("----------- Starting answer ----------------\n");
    // do{
    //     read(sock_fd, answer, sizeof (Message));
    //     printPerson(answer->personData);
    //     printf("******************************\n");
    // }while(answer->statusCode == SENDING);
    // printf("Operation: %d Status: %d\n",answer->operationCode,answer->statusCode);
    // puts("----------- End answer ---------------------\n");
    
    // free(answer);
    // free(request);
}

void getPersonUDP(int sock_fd){

    // Message *request = malloc(sizeof (Message));
    // Message *answer = malloc(sizeof (Message));
    // request->operationCode = GET_PERSON;
    
    char *email = malloc(MAX_INPUT*sizeof(char));

    printf("Enter the email: ");
    fgets(email, MAX_INPUT, stdin);


    // puts("Sending message");    
    // write(sock_fd, request, sizeof (Message));
    
    // puts("----------- Starting answer ----------------\n");
    // do{
    //     read(sock_fd, answer, sizeof (Message));
    //     if(answer->statusCode != ERROR){
    //         printPerson(answer->personData);
    //         printf("******************************\n");
    //     }
    // }while(answer->statusCode == SENDING);
    // printf("Operation: %d Status: %d\n",answer->operationCode,answer->statusCode);
    // puts("----------- End answer ---------------------\n");

    // free(answer);
    // free(request);
}

void removePersonUDP(int sock_fd){
    // Message *request = malloc(sizeof (Message));
    // Message *answer = malloc(sizeof (Message));
    // request->operationCode = REMOVE_PERSON;
    
    char *email = malloc(MAX_INPUT*sizeof(char));

    printf("Enter the email: ");
    fgets(email, MAX_INPUT, stdin);

    // puts("Sending message");
    // write(sock_fd, request, sizeof (Message));
    // read(sock_fd, answer, sizeof (Message));
    // puts("----------- Starting answer ----------------\n"); 
    // printf("Operation: %d Status: %d\n", answer->operationCode, answer->statusCode);
    // puts("----------- End answer ---------------------\n");

    // free(answer);
    // free(request);
}
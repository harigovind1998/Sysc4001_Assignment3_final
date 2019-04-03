#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <sys/msg.h>
#include "LinkedList.h"
#include "employeeInfo.h"

#define SERVER_MSGQUEUE 1234
#define CLIENT_MSGQUEUE 4321

void handleMsg(int msgType);
struct ReceivedMsg
{
    long int msg_type;
    struct Employee ReceivedInfo;
};

struct SendMsgTxt{
    long int msg_type;
    char msg[12];
};

struct SendMsgArray
{
    long int msg_type;
    int EmpIds;
};

struct ReceivedMsg ReceivedMsg;
struct SendMsgTxt TxtMsg;
struct SendMsgArray ArrayMsg;
int ServerMsgQueue, ClientMsgQueue;
int *tempArr;

int running = 1;
int main(){
    
    ServerMsgQueue = msgget((key_t)SERVER_MSGQUEUE, 0666|IPC_CREAT);
    ClientMsgQueue = msgget((key_t)CLIENT_MSGQUEUE, 0666|IPC_CREAT);
    TxtMsg.msg_type = 1;
    ArrayMsg.msg_type = 1;
    if(ServerMsgQueue == -1 || ClientMsgQueue == -1){
        fprintf(stderr, "Server counldnt revieve message\n");
        exit(EXIT_FAILURE);
    }
    printf("%d\n", ClientMsgQueue);
    while(running){
        if(msgrcv(ServerMsgQueue,(void *) &ReceivedMsg,sizeof(struct Employee), 0,0)==-1){
            fprintf(stderr, "msgrcv failed with error: %d\n", errno);
            exit(EXIT_FAILURE);
        }
        printf("Handling Request...");
        handleMsg(ReceivedMsg.msg_type);
        
        if(((ReceivedMsg.msg_type) != 6)){
            printf("Sending respose...\n");
            if(msgsnd(ClientMsgQueue, (void *)&TxtMsg,BUFSIZ,0) == 1){
                fprintf(stderr, "Msgsnd failed with error: ", errno);
                exit(EXIT_FAILURE);
            }
        }else{
            printf("Sending respose...");
            ArrayMsg.EmpIds = *tempArr;
            if(msgsnd(ClientMsgQueue,(void *) &ArrayMsg, sizeof(int), 0)==-1){
                fprintf(stderr, "msgsnd GAYYYY failed with error: %d\n", errno);
                exit(EXIT_FAILURE);
            }
            ArrayMsg.EmpIds = tempArr[1];
            if(msgsnd(ClientMsgQueue,(void *) &ArrayMsg, sizeof(int), 0)==-1){
                fprintf(stderr, "msgsnd GAYYYY failed with error: %d\n", errno);
                exit(EXIT_FAILURE);
            }
            ArrayMsg.EmpIds = tempArr[2];
            if(msgsnd(ClientMsgQueue,(void *) &ArrayMsg, sizeof(int), 0)==-1){
                fprintf(stderr, "msgsnd GAYYYY failed with error: %d\n", errno);
                exit(EXIT_FAILURE);
            }
            ArrayMsg.EmpIds = tempArr[3];
            if(msgsnd(ClientMsgQueue,(void *) &ArrayMsg, sizeof(int), 0)==-1){
                fprintf(stderr, "msgsnd GAYYYY failed with error: %d\n", errno);
                exit(EXIT_FAILURE);
            }
            ArrayMsg.EmpIds = tempArr[4];
            if(msgsnd(ClientMsgQueue,(void *) &ArrayMsg, sizeof(int), 0)==-1){
                fprintf(stderr, "msgsnd GAYYYY failed with error: %d\n", errno);
                exit(EXIT_FAILURE);
            }
            printf("Done\n");
            strcpy(TxtMsg.msg , "Done");
           
            if(msgsnd(ClientMsgQueue, (void *)&TxtMsg,BUFSIZ,0) == 1){
                fprintf(stderr, "Msgsnd failed with error: ", errno);
                exit(EXIT_FAILURE);
            }
            
        }
    }
    
    if (msgctl(ServerMsgQueue, IPC_RMID, 0) == -1) {
		fprintf(stderr, "msgctl(IPC_RMID) failed\n");
		exit(EXIT_FAILURE);
	}
    if (msgctl(ClientMsgQueue, IPC_RMID, 0) == -1) {
		fprintf(stderr, "msgctl(IPC_RMID) failed\n");
		exit(EXIT_FAILURE);
	}
}

void handleMsg(int msgType){
    char temp[12];
    float Salary;
    
    switch(msgType){
        case 1:
            Insert(ReceivedMsg.ReceivedInfo);
            strcpy(TxtMsg.msg, "Inserted");
            break;
        case 2:
            strcpy(TxtMsg.msg, CheckName(ReceivedMsg.ReceivedInfo.EmpID));
            break;
        case 3:
            strcpy(TxtMsg.msg, CheckDeptName(ReceivedMsg.ReceivedInfo.EmpID));
            break;
        case 4:
            Salary = GetSalary(ReceivedMsg.ReceivedInfo.EmpID);
            gcvt(Salary,40,temp);
            strcpy(TxtMsg.msg,temp);
            break;
        case 5:
            sprintf(temp, "%d", GetEmpID(ReceivedMsg.ReceivedInfo.Name));
            strcpy(TxtMsg.msg, temp);
            break;
        case 6:
            tempArr = DeptEmployees(ReceivedMsg.ReceivedInfo.DepName);
            break;
        case 7:
            DeleteEmp(ReceivedMsg.ReceivedInfo.EmpID);
            strcpy(TxtMsg.msg, "Deleted");
            break;
        case 8:
            running =0;
            strcpy(TxtMsg.msg, "Exited");
            break;
    }
}
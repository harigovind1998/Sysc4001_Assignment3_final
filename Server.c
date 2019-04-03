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
    //Creates/links to the Server and Client message queues
    ServerMsgQueue = msgget((key_t)SERVER_MSGQUEUE, 0666|IPC_CREAT);
    ClientMsgQueue = msgget((key_t)CLIENT_MSGQUEUE, 0666|IPC_CREAT);
    TxtMsg.msg_type = 1;
    ArrayMsg.msg_type = 1;
    //checking for errors
    if(ServerMsgQueue == -1 || ClientMsgQueue == -1){
        fprintf(stderr, "Server counldnt revieve message\n");
        exit(EXIT_FAILURE);
    }
    while(running){
        //waits to receive a message in the Server message box
        if(msgrcv(ServerMsgQueue,(void *) &ReceivedMsg,sizeof(struct Employee), 0,0)==-1){
            fprintf(stderr, "msgrcv failed with error: %d\n", errno);
            exit(EXIT_FAILURE);
        }
        printf("Handling Request...");
        //Handles the message according to the Message type
        handleMsg(ReceivedMsg.msg_type);
        
        //Responds with the approprioate response
        if(((ReceivedMsg.msg_type) != 6)){
            printf("Sending respose...\n");
            if(msgsnd(ClientMsgQueue, (void *)&TxtMsg,BUFSIZ,0) == 1){
                fprintf(stderr, "Msgsnd failed with error: ", errno);
                exit(EXIT_FAILURE);
            }
        }else{
            //if requesting for employess with in a department, send 5 different messages with the employee ID's
            //I could have done this in a loop, but c was getting messed up, please read comments on Client side
            //I wanted the first message to send the number of eployees being passed so that if it is <5, i dont have unnecessary messages
            //But while loop was not working. 
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
    
    //deletes the Message queues
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
            //Insertes into the linkelist and responds with "Inserted"
            Insert(ReceivedMsg.ReceivedInfo);
            strcpy(TxtMsg.msg, "Inserted");
            break;
        case 2:
            //Gets the Name when given the ID, response is the Name
            strcpy(TxtMsg.msg, CheckName(ReceivedMsg.ReceivedInfo.EmpID));
            break;
        case 3:
            //Gets the Departnment when given the ID, response is the Department name
            strcpy(TxtMsg.msg, CheckDeptName(ReceivedMsg.ReceivedInfo.EmpID));
            break;
        case 4:
            //Gets the Salary when given the ID, response is the Salary stringified
            Salary = GetSalary(ReceivedMsg.ReceivedInfo.EmpID);
            gcvt(Salary,40,temp);
            strcpy(TxtMsg.msg,temp);
            break;
        case 5:
            //Gets the Name of the employee when given the ID, response is the Employee name
            sprintf(temp, "%d", GetEmpID(ReceivedMsg.ReceivedInfo.Name));
            strcpy(TxtMsg.msg, temp);
            break;
        case 6:
            //Stores the first 5 employess in the departnment in an array, and sends each employee through one at a time
            tempArr = DeptEmployees(ReceivedMsg.ReceivedInfo.DepName);
            break;
        case 7:
            //Deletes the Employee in the Linked list, responses in "Deleted" prompting the employee has been deleted
            //If the Employee does not exist, delets nothing
            DeleteEmp(ReceivedMsg.ReceivedInfo.EmpID);
            strcpy(TxtMsg.msg, "Deleted");
            break;
        case 8:
            //End program prompt, allows main while loop to exit
            running =0;
            strcpy(TxtMsg.msg, "Exited");
            break;
    }
}
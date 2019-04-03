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

//Message to send
struct ToSendMsg
{
    long int msg_type;
    struct Employee ReceivedInfo; //Employee struct
};

//received text message of length 12
struct RecTxtMsg
{
    long int msg_type;
    char msg[12];
};

//Used to display the employee's in a dept
struct RecArrayMsg
{
    long int msg_type;
    int EmpIds;
};

struct ToSendMsg MsgToSend;
struct RecTxtMsg TxtMsg;
struct RecArrayMsg ArrayMsg;

//Waits for an input, sends the message to the server Message box and waits for a response to be returned to the Client Message box
int main(){
    int running = 1;
    int ServerMsgQueue, ClientMsgQueue;
    ServerMsgQueue = msgget((key_t)SERVER_MSGQUEUE, 0666|IPC_CREAT);
    ClientMsgQueue = msgget((key_t)CLIENT_MSGQUEUE,0666|IPC_CREAT);

    if(ServerMsgQueue == -1 || ClientMsgQueue == -1){
        fprintf(stderr, "Server counldnt revieve message\n");
        exit(EXIT_FAILURE);
    }
    while(running){
        //Asks the admin what function they want to do
        printf("\n1: Insert, 2: CheckName, 3: CheckDeptName, 4: CheckSalary, 5: GetEmpId, 6: DeptEmployees, 7: DeleteEmp, 8:End \n");
        scanf("%ld", &(MsgToSend.msg_type));
        switch (MsgToSend.msg_type)
        {
            //Takes the approprioate inputs
            case 1:
                printf("\nEnter employee Name: ");
                scanf("%s", MsgToSend.ReceivedInfo.Name);
                printf("\nEnter employee Department: ");
                scanf("%s", MsgToSend.ReceivedInfo.DepName);
                printf("\nEnter employee ID: ");
                scanf("%d", &MsgToSend.ReceivedInfo.EmpID);
                printf("\nEnter employee Salary: ");
                scanf("%f", &MsgToSend.ReceivedInfo.Salary);
                break;
            case 2:
                printf("\nEnter employee ID: ");
                scanf("%d", &MsgToSend.ReceivedInfo.EmpID);
                break;
            case 3:
                printf("\nEnter employee ID: ");
                scanf("%d", &MsgToSend.ReceivedInfo.EmpID);
                break;
            case 4:
                printf("\nEnter employee ID: ");
                scanf("%d", &MsgToSend.ReceivedInfo.EmpID);
                break;  
            case 5:
                printf("Enter employee Name: ");
                scanf("%s", MsgToSend.ReceivedInfo.Name);
                break;  
            case 6:
                printf("Enter Department Name: ");
                scanf("%s", MsgToSend.ReceivedInfo.DepName);
                break;
            case 7:
                printf("Enter employee ID: ");
                scanf("%d", &MsgToSend.ReceivedInfo.EmpID);
                break;
            case 8:
                running =0;
                break;     
            default:
                break;
        }
        //Sends the message with the specified message type
        if (msgsnd(ServerMsgQueue, (void *)&MsgToSend, sizeof(struct Employee), 0) == -1) {
		 	fprintf(stderr, "msgsnd failed\n");
			exit(EXIT_FAILURE);
		}
        printf("Message sent, waiting for response...\n");
        if(((MsgToSend.msg_type) != 6)){ //If the message sent was not a department request, it just displays the response
            if (msgrcv(ClientMsgQueue, (void *)&TxtMsg, BUFSIZ,0, 0) == -1) {fprintf(stderr, "msgrcv failed with error: %d\n", errno);
			    fprintf(stderr, "msgrcv for norm msg failed with error: %d\n", errno);
                exit(EXIT_FAILURE);
		    }
            printf("Response: %s\n",TxtMsg.msg);
        }else{
            //If the message was a Department request, prints the first 5 emeployees Id in the server. I know that I could have done this in a 
            //for or while loop, but for some reason in the server side, it kept skipping the loop and only printed out the responses when the next
            //command was sent to the server to process. I can show this to you in person, and im not entierly sure it wasnt working as expected with
            //a loop.
            //This works.
            printf("Response: ");
            if(msgrcv(ClientMsgQueue,(void *) &ArrayMsg, sizeof(int),0 ,0)==-1){
                fprintf(stderr, "msgrcv for array failed with error: %d\n", errno);
                exit(EXIT_FAILURE);
            }
            printf("%d, ", ArrayMsg.EmpIds);
            if(msgrcv(ClientMsgQueue,(void *) &ArrayMsg, sizeof(int),0 ,0)==-1){
                fprintf(stderr, "msgrcv for array failed with error: %d\n", errno);
                exit(EXIT_FAILURE);
            }
            printf("%d, ", ArrayMsg.EmpIds);
            if(msgrcv(ClientMsgQueue,(void *) &ArrayMsg, sizeof(int),0 ,0)==-1){
                fprintf(stderr, "msgrcv for array failed with error: %d\n", errno);
                exit(EXIT_FAILURE);
            }
            printf("%d, ", ArrayMsg.EmpIds);
            if(msgrcv(ClientMsgQueue,(void *) &ArrayMsg, sizeof(int),0 ,0)==-1){
                fprintf(stderr, "msgrcv for array failed with error: %d\n", errno);
                exit(EXIT_FAILURE);
            }
            printf("%d, ", ArrayMsg.EmpIds);
            if(msgrcv(ClientMsgQueue,(void *) &ArrayMsg, sizeof(int),0 ,0)==-1){
                fprintf(stderr, "msgrcv for array failed with error: %d\n", errno);
                exit(EXIT_FAILURE);
            }
            printf("%d ", ArrayMsg.EmpIds);
            if (msgrcv(ClientMsgQueue, (void *)&TxtMsg, BUFSIZ,0, 0) == -1) {fprintf(stderr, "msgrcv failed with error: %d\n", errno);
			    fprintf(stderr, "msgrcv for norm msg failed with error: %d\n", errno);
                exit(EXIT_FAILURE);
		    }
            printf("%s.\n",TxtMsg.msg);
        }
    }
}
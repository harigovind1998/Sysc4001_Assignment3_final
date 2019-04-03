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

struct ReceivedMsg
{
    long int msg_type;
    struct Employee ReceivedInfo;
};

struct SendMsgTxt
{
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
        printf("\n1: Insert, 2: CheckName, 3: CheckDeptName, 4: CheckSalary, 5: GetEmpId, 6: DeptEmployees, 7: DeleteEmp, 8:End \n");
        scanf("%ld", &(ReceivedMsg.msg_type));
        switch (ReceivedMsg.msg_type)
        {
            case 1:
                printf("\nEnter employee Name: ");
                scanf("%s", ReceivedMsg.ReceivedInfo.Name);
                printf("\nEnter employee Department: ");
                scanf("%s", ReceivedMsg.ReceivedInfo.DepName);
                printf("\nEnter employee ID: ");
                scanf("%d", &ReceivedMsg.ReceivedInfo.EmpID);
                printf("\nEnter employee Salary: ");
                scanf("%f", &ReceivedMsg.ReceivedInfo.Salary);
                break;
            case 2:
                printf("\nEnter employee ID: ");
                scanf("%d", &ReceivedMsg.ReceivedInfo.EmpID);
                break;
            case 3:
                printf("\nEnter employee ID: ");
                scanf("%d", &ReceivedMsg.ReceivedInfo.EmpID);
                break;
            case 4:
                printf("\nEnter employee ID: ");
                scanf("%d", &ReceivedMsg.ReceivedInfo.EmpID);
                break;  
            case 5:
                printf("Enter employee Name: ");
                scanf("%s", ReceivedMsg.ReceivedInfo.Name);
                break;  
            case 6:
                printf("Enter Department Name: ");
                scanf("%s", ReceivedMsg.ReceivedInfo.DepName);
                break;
            case 7:
                printf("Enter employee ID: ");
                scanf("%d", &ReceivedMsg.ReceivedInfo.EmpID);
                break;
            case 8:
                running =0;
                break;     
            default:
                break;
        }
        if (msgsnd(ServerMsgQueue, (void *)&ReceivedMsg, sizeof(struct Employee), 0) == -1) {
		 	fprintf(stderr, "msgsnd failed\n");
			exit(EXIT_FAILURE);
		}
        printf("Message sent, waiting for response...\n");
        if(((ReceivedMsg.msg_type) != 6)){
            if (msgrcv(ClientMsgQueue, (void *)&TxtMsg, BUFSIZ,0, 0) == -1) {fprintf(stderr, "msgrcv failed with error: %d\n", errno);
			    fprintf(stderr, "msgrcv for norm msg failed with error: %d\n", errno);
                exit(EXIT_FAILURE);
		    }
            printf("Response: %s\n",TxtMsg.msg);
        }else{
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
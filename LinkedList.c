#include "employeeInfo.h"
#include "LinkedList.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
struct node{
    struct Employee Employee;
    struct node *next;
};

struct node *head = NULL;
int employees = 0;

void Insert(struct Employee toInsert){
    struct node *temp=malloc(sizeof(struct node));
    temp->Employee = toInsert;
    temp->next = NULL;
    if(head==NULL){
        head = temp;
    }else{
        struct node *curr = head;
        while(curr->next != NULL){
            curr = curr->next;
        }
        curr->next = temp;
    }
    employees++;
}

struct node *GetEmpDetsByNum(int empNum){
    if(head == NULL){
        return NULL;
    }
    struct node *curr = head;
    while((curr->Employee.EmpID != empNum) && curr->next != NULL){
        curr = curr->next;
    }

    if(curr->Employee.EmpID == empNum){
        return curr;
    }else{
        return NULL;
    }
}

char *CheckName(int empNum){
    struct node *temp = GetEmpDetsByNum(empNum);
    if (temp==NULL){
        return "N/A";
    }
    return temp->Employee.Name;
}

char *CheckDeptName(int empNum){
    struct node *temp = GetEmpDetsByNum(empNum);
    if (temp==NULL){
        return "N/A";
    }
    return temp->Employee.DepName;
}

float GetSalary(int empNum){
    struct node *temp = GetEmpDetsByNum(empNum);
    if (temp==NULL){
        return -1;
    }
    return temp->Employee.Salary;
}

int GetEmpID(char *name){
    if(head == NULL){
        return -1;
    }
    struct node *curr = head;
    while(!(strcmp(curr->Employee.Name,name)==0)&&curr->next != NULL){
        curr = curr->next;
    }
    if(strcmp(curr->Employee.Name, name)==0){
        return curr->Employee.EmpID;
    }else{
        return -1;
    }
}
int *DeptEmployees(char *DeptName){
    int *temp = calloc(5,sizeof(int));
    int i = 0;
    struct node *curr = head;
    if(curr==NULL){
        return temp;
    }

    while(curr!=NULL && i<5){
        if(strcmp(curr->Employee.DepName,DeptName)==0){
            temp[i]=curr->Employee.EmpID;
            i++;
        }
        curr = curr->next;
    }
    return temp;
}
void DeleteEmp(int empNum){
    if(head == NULL){
        return;
    }
    
    if(head->Employee.EmpID == empNum){
        head = head->next;
        return;
    }
    struct node *curr = head;
    while(curr->next!=NULL){
        if(curr->next->Employee.EmpID == empNum){
            
            struct node *toDel = curr->next;
            curr->next = curr->next->next;
            free(toDel);
            employees--;
            
            return;
        }
        curr = curr->next;
    }
}



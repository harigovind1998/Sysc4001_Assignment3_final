#ifndef LinkedList
#define LinkedList
#include "employeeInfo.h"
void Insert(struct Employee toInsert);

char *CheckName(int empNum);

char *CheckDeptName(int empNum);

float GetSalary(int empNum);

int GetEmpID(char *name);

int *DeptEmployees(char *DeptName);

void DeleteEmp(int empNum);

#endif
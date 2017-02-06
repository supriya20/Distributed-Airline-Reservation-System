/*Main code for the client side*/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include<arpa/inet.h>
#include <stdlib.h>

void at_select_fn1();
void add_agent();

void validate_sysadmin();
void edit_agent();

char sys_password[20];
char sys_pass_hash[50];
char sys_email[50]; 

char role; 

char buff[1050];
int csock;

char agent_name[10], agent_password[20],agent_pass_hash[50], agent_email[50], agent_phone[10], agent_age[3], agent_gender[2], agent_airline[4];

struct sockaddr_in servadd;
socklen_t addr_size;

int main()
{
 csock = socket(PF_INET, SOCK_STREAM, 0);
 servadd.sin_addr.s_addr = inet_addr("127.0.0.1");
 servadd.sin_port = htons(7802);
 servadd.sin_family = AF_INET; 
 memset(servadd.sin_zero, '\0', sizeof servadd.sin_zero);  
 addr_size = sizeof servadd;

 connect(csock, (struct sockaddr *) &servadd, addr_size);
 printf("%s\n", "Hello...!!! Welcome...!!!");
 
 role = '1';
 strcpy(buff, &role);
 send(csock,buff,strlen(buff),0);
 memset(buff, '\0', sizeof(buff));

 validate_sysadmin();

 return 0;
}


////////////////////////////////////////////// VALIDATE SYSTEM ADMIN FUNCTION ///////////////////////////////////////////////

void validate_sysadmin()
{
 printf("%s", "Please enter your credentials below:\n");
 
 printf("%s", "Email:");
 scanf("%s", sys_email);
 strcpy(buff, sys_email);
 send(csock, buff, strlen(sys_email), 0);
 memset(buff, '\0', sizeof(buff));
 
 printf("%s", "Password:");
 scanf("%s", sys_password);
 //SHA1(sys_password, strlen(sys_password), sys_pass_hash); 
 strcpy(buff, sys_password);
 send(csock, buff, strlen(sys_password), 0);
 memset(buff, '\0', sizeof(buff));

 char sys_validate[10];
 
 recv(csock, buff, sizeof(buff),0);
 strcpy(sys_validate, buff); 
 //printf("%s\n",sys_validate);
 //printf("%s\n",buff);
 memset(buff, '\0', sizeof(buff));
 //char right[10] = "Right";

 int uv = strcmp(sys_validate, sys_password);
 //printf("%d\n", uv);
 if(uv == 0)
 {
  printf("%s\n", "Authentication Successfull!!!");
  at_select_fn1();  
 }
 else
 {
  printf("%s\n", "Please check the username and password!!");
  exit(1);
 }
 
return;
}

/////////////////////////////////////////////// 1st SELECTION FUNCTION //////////////////////////////////////////////

void at_select_fn1()
{
 char at_select1[2];
 printf("Please select the operation you want to perform\n 1. Add an Agent\n 2. Modify Agent Info\n 3. Logout\n"); 
 scanf("%s",at_select1);
 strcpy(buff,at_select1);
 send(csock,buff,strlen(at_select1),0);
 memset(buff, '\0', sizeof(buff));
 
  if((strcmp(at_select1, "1")) == 0)
  {
  add_agent();
  }
  else if ((strcmp(at_select1, "2")) == 0)
  { 
   edit_agent();
  }
  else if((strcmp(at_select1,"3"))==0)
  {
   exit(1);
  }

return;
}

////////////////////////////////////////////// ADD AGENT //////////////////////////////////////////////

void add_agent()
{
 printf("Name:");
 scanf("%s", agent_name);
 strcpy(buff, agent_name);
 send(csock,buff,strlen(agent_name),0);

 printf("Password:");
 scanf("%s", agent_password);
 //SHA1(agent_password, strlen(agent_password), agent_pass_hash); 
 strcpy(buff, agent_password);
 send(csock,buff,strlen(agent_password),0);

 printf("Email ID:");
 scanf("%s", agent_email);
 strcpy(buff, agent_email);
 send(csock,buff,strlen(agent_email),0);

 printf("Phone No:");
 scanf("%s", agent_phone);
 strcpy(buff, agent_phone);
 send(csock,buff,strlen(agent_phone),0);

 printf("age:");
 scanf("%s", agent_age);
 strcpy(buff, agent_age);
 send(csock,buff,strlen(agent_age),0);

 printf("Gender:");
 scanf("%s", agent_gender);
 strcpy(buff, agent_gender);
 send(csock,buff,strlen(agent_gender),0); 


 char airline_info[200];
 
 recv(csock, buff, sizeof(buff),0);
 strcpy(airline_info, buff); 
 printf("%s\n",airline_info);
 //printf("%s\n",buff);
 memset(buff, '\0', sizeof(buff));

 printf("Airline to which the agent belongs to:");
 scanf("%s", agent_airline);
 strcpy(buff, agent_airline);
 send(csock,buff,strlen(agent_airline),0); 
 
 at_select_fn1();

 return;
}

/////////////////////////////////////////////////////// EDIT AGENT INFO /////////////////////////////////////////////////////


void edit_agent()
{
char i[2];

printf("\nEnter the agent email:");
   scanf("%s",agent_email);
   strcpy(buff,agent_email);
   send(csock,buff,strlen(agent_email),0);
   memset(buff, '\0', sizeof(buff));

char val[2];

 recv(csock, buff, sizeof(buff),0);
 strcpy(val, buff); 
 //printf("%s\n",val);
 //printf("%s\n",buff);
 memset(buff, '\0', sizeof(buff));

 int sim = strcmp(val,"1");
 printf("%d\n",sim);

if ((strcmp(val,"1"))==1)
{

 while(1)
 {
  printf("Select the parameter to be modified: \n 1. Name\n 2. Age\n 3. Phone Number \n 4. Airline\n 5. Exit\n");
  scanf("%s",i);
  if((strcmp(i,"1"))==0)
  {
   strcpy(buff,i);
   send(csock,buff,strlen(i),0);
   memset(buff, '\0', sizeof(buff));
   printf("\nEnter the new name:");
   scanf("%s",agent_name);
   strcpy(buff,agent_name);
   send(csock,buff,strlen(agent_name),0);
   memset(buff, '\0', sizeof(buff));
  }
  else if((strcmp(i,"2"))==0)
  {
   strcpy(buff,i);
   send(csock,buff,strlen(i),0);
   memset(buff, '\0', sizeof(buff));
   printf("\nEnter your age:");
   scanf("%s",agent_age);
   strcpy(buff,agent_age);
   send(csock,buff,strlen(agent_age),0);
   memset(buff, '\0', sizeof(buff));
  }
  else if((strcmp(i,"3"))==0)
  {
   strcpy(buff,i);
   send(csock,buff,strlen(i),0);
   memset(buff, '\0', sizeof(buff));
   printf("\nEnter the new phone number:");
   scanf("%s",agent_phone);
   strcpy(buff,agent_phone);
   send(csock,buff,strlen(agent_phone),0);
   memset(buff, '\0', sizeof(buff));
  }
  else if((strcmp(i,"4"))==0)
  {
   strcpy(buff,i);
   send(csock,buff,strlen(i),0);
   memset(buff, '\0', sizeof(buff));
   printf("\nEnter the new airline:");
   scanf("%s",agent_airline);
   strcpy(buff,agent_airline);
   send(csock,buff,strlen(agent_airline),0);
   memset(buff, '\0', sizeof(buff));
  }
  else if((strcmp(i,"5"))==0)
  {
   strcpy(buff,i);
   send(csock,buff,strlen(i),0);
   memset(buff, '\0', sizeof(buff));
   at_select_fn1();
  }

printf("The agent details have been updated\n");
			char agentupdate[600];
			recv(csock, buff, sizeof(buff),0);
			strcpy(agentupdate, buff);
			printf("%s\n",agentupdate);
			memset(buff, '\0', sizeof(buff));
	}
 
			edit_agent();


 }

else  if((strcmp(val,"0"))==1)
{
  printf("%s\n","Agent does not exist.");
  exit(1);
  
}

}


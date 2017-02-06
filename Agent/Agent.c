/*Main code for the client side*/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include<arpa/inet.h>
#include <stdlib.h>

void at_select_fn1();
void add_flight();
void validate_agent();
void update_flight();

char at_password[20];
char pass_hash[50];
char at_email[50]; 

char role; 

char buff[1050];
int csock;

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
 
 role = '2';
 strcpy(buff, &role);
 send(csock,buff,strlen(buff),0);
 memset(buff, '\0', sizeof(buff));

 validate_agent();

 return 0;
}


////////////////////////////////////////////// VALIDATE AGENT FUNCTION ///////////////////////////////////////////////

void validate_agent()
{
 printf("%s", "Please enter your credentials below:\n");
 
 printf("%s", "Email:");
 scanf("%s", at_email);
 strcpy(buff, at_email);
 send(csock, buff, strlen(at_email), 0);
 memset(buff, '\0', sizeof(buff));
 
 printf("%s", "Password:");
 scanf("%s", at_password);
 //SHA1(at_password, strlen(at_password), pass_hash); 
 strcpy(buff, at_password);
 send(csock, buff, strlen(at_password), 0);
 memset(buff, '\0', sizeof(buff));

 char at_validate[2];
 
 recv(csock, buff, sizeof(buff),0);
 strcpy(at_validate, buff); 
 //printf("%s\n",at_validate);
 //printf("%s\n",buff);
 memset(buff, '\0', sizeof(buff));
 //char right[10] = "Right";

 int uv = strcmp(at_validate, at_password);
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
 printf("Please select the operation you want to perform\n 1. Add new flight\n 2. Modify Flight Info\n 3. Logout"); 
 scanf("%s",at_select1);
     strcpy(buff,at_select1);
  send(csock,buff,strlen(at_select1),0);
  memset(buff, '\0', sizeof(buff));
 
  if((strcmp(at_select1, "1")) == 0)
  {
    add_flight();
  }
  else if ((strcmp(at_select1, "2")) == 0)
  { 
    char flightDetails[600];
    recv(csock, buff, sizeof(buff),0);
    strcpy(flightDetails, buff);
    printf("%s\n",flightDetails);
    memset(buff, '\0', sizeof(buff));
    update_flight();
  }
  else if ((strcmp(at_select1, "3")) == 0)
  {
    exit(1);
  }

return;
}

////////////////////////////////////////////// ADD FLIGHT //////////////////////////////////////////////

void add_flight()
{
 char flightNum[10],airlineName[20], airlineID[3],source[20],destination[20];
 char noOfSeats[5], fare[4], arrivalTime[9],departureTime[9], date[10];

 printf("Enter the Flight number:");
 scanf("%s",flightNum);
 strcpy(buff,flightNum);
 send(csock,buff,strlen(flightNum),0);
 //memset(buff, '\0', sizeof(buff));

 /*printf("Airline ID:");
 scanf("%s",airlineID);
 strcpy(buff,airlineID);
 send(csock,buff,strlen(airlineID),0);
 //memset(buff, '\0', sizeof(buff));

 printf("Airline Name:");
 scanf("%s",airlineName);
 strcpy(buff,airlineName);
 send(csock,buff,strlen(airlineName),0);
 //memset(buff, '\0', sizeof(buff));*/

 printf("Source:");
 scanf("%s",source);
 strcpy(buff,source);
 send(csock,buff,strlen(source),0);
 //memset(buff, '\0', sizeof(buff));

 printf("Destination:");
 scanf("%s",destination);
 strcpy(buff,destination);
 send(csock,buff,strlen(destination),0);
 //memset(buff, '\0', sizeof(buff));
 
 printf("Number of Seats:");
 scanf("%s",noOfSeats);
 strcpy(buff,noOfSeats);
 send(csock,buff,strlen(noOfSeats),0);
 //memset(buff, '\0', sizeof(buff));

 printf("Fare:");
 scanf("%s",fare);
 strcpy(buff,fare);
 send(csock,buff,strlen(fare),0);
 //memset(buff, '\0', sizeof(buff));

 printf("Departure Time:");
 scanf("%s",departureTime);
 strcpy(buff,departureTime);
 send(csock,buff,strlen(departureTime),0);
 //memset(buff, '\0', sizeof(buff));

 printf("Arrival Time:");
 scanf("%s",arrivalTime);
 strcpy(buff,arrivalTime);
 send(csock,buff,strlen(arrivalTime),0);
 //memset(buff, '\0', sizeof(buff));
 
 printf("Date:");
 scanf("%s",date);
 strcpy(buff,date);
 send(csock,buff,strlen(date),0);
 //memset(buff, '\0', sizeof(buff)); 

 printf("%s\n", "Flight has been added.");
 at_select_fn1();

 return;
}

///////////////////////////////////////////////////// MODIFY FLIGHT ///////////////////////////////////////////////

void update_flight()
{
 //printf("I am here inside update");

 char at_flight_id[3];
 char flightNum[10];
 char source[20];
 char destination[20];
 char arival[9];
 char dept[9];
 char fare[5];
 
	char i[2];

	printf("Enter the flight id to be modified\n");
	scanf("%s",at_flight_id);
	strcpy(buff,at_flight_id);
	send(csock,buff,strlen(at_flight_id),0);
	memset(buff, '\0', sizeof(buff));

	while(1){
		printf("Select the parameter to be modified: \n 1. Source\n 2. Destination\n 3. Fare\n 4. Arrival Time \n 5. Departure Time \n 6. Exit");
		scanf("%s",i);
		if((strcmp(i,"1"))==0){

			strcpy(buff,i);
			send(csock,buff,strlen(i),0);
			memset(buff, '\0', sizeof(buff));
			printf("\nEnter the new source city:");
			scanf("%s",source);
			strcpy(buff,source);
			send(csock,buff,strlen(source),0);
			memset(buff, '\0', sizeof(buff));
		}
		else if((strcmp(i,"2"))==0){

			strcpy(buff,i);
			send(csock,buff,strlen(i),0);
			memset(buff, '\0', sizeof(buff));
			printf("\nEnter the new destination city:");
			scanf("%s",destination);
			strcpy(buff,destination);
			send(csock,buff,strlen(destination),0);
			memset(buff, '\0', sizeof(buff));
                       }
		else if((strcmp(i,"3"))==0){

			strcpy(buff,i);
			send(csock,buff,strlen(i),0);
			memset(buff, '\0', sizeof(buff));
			printf("\nEnter the new fare:");
			scanf("%s",fare);
			strcpy(buff,fare);
			send(csock,buff,strlen(fare),0);
			memset(buff, '\0', sizeof(buff));
                       }
		else if((strcmp(i,"4"))==0){

			strcpy(buff,i);
			send(csock,buff,strlen(i),0);
			memset(buff, '\0', sizeof(buff));
			printf("\nEnter the arival time:");
			scanf("%s",arival);
			strcpy(buff,arival);
			send(csock,buff,strlen(arival),0);
			memset(buff, '\0', sizeof(buff));
                       }
               else if((strcmp(i,"5"))==0){

			strcpy(buff,i);
			send(csock,buff,strlen(i),0);
			memset(buff, '\0', sizeof(buff));
			printf("\nEnter the departure time:");
			scanf("%s",dept);
			strcpy(buff,dept);
			send(csock,buff,strlen(dept),0);
			memset(buff, '\0', sizeof(buff));
                       }
		else if((strcmp(i,"6"))==0){
                      at_select_fn1();
			
		}
printf("The flight details have been updated\n");
			char flightupdate[600];
			recv(csock, buff, sizeof(buff),0);
			strcpy(flightupdate, buff);
			printf("%s\n",flightupdate);
			memset(buff, '\0', sizeof(buff));
	}
 
			update_flight();
return;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

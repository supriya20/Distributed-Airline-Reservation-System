/*Server side main code*/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include<signal.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<stdlib.h>
#include<pthread.h> 
#include<unistd.h>

pthread_mutex_t count_mutex;
long long count;


//////////////////// USER VARIABLES //////////////////////////

void add_user(char * role, int sd);
void validate_user(char * role, int sd);
void booking(char * email, int sd);
void history(char * email, int sd);
void cancel_booking(char * email, int sd);
void edit_user(char * email, int sd);
void select_fn2(int sd);

char name[10];
char password[50];
char email[50];
char phone[11];
char age[3];
char gender[2];
char ID[2];
char No_seat[2];

//////////////////// AGENT VARIABLES //////////////////////////

void at_select_fn1(int sd);
void at_select_fn2(int sd, char * at_flight_id);
void validate_agent(int sd, char * at_email, char* at_password, char * role);
 void add_flight(int sd, char * at_email);
void modify_flight(int sd, char * at_email);

char at_email[50];
char at_password[50];
char at_select1[2];

char at_flightnum[10];
char at_airlinename[20];
char at_airlineID[3];
char at_source[20];
char at_destination[20];
char at_noofSeats[5];
char at_fare[4];
char at_arrivaltime[9];
char at_departuretime[9];
char at_date[10];

///////////////////// SYS ADMIN VARIABLES ///////////////////////

void validate_admin(int sd, char * role, char * sys_email);
void add_agent(int sd);
void modify_agent(int sd);
void sys_select_fn(int sd);
void agent_modify(int sd, char * agent_email);

char sys_email[50];
char sys_password[50];
char sys_select1[2];


char agent_name[10];
char agent_password[50];
char agent_email[50];
char agent_phone[11];
char agent_age[3];
char agent_gender[2];
char agent_ID[2];
char agent_airline[4];

///////////////////////////////////////////////////////////




#include<mysql.h>

char buff[20000];
int nBytes;
void *process(void *psock);
   MYSQL *conn;
   MYSQL_RES *res;
   MYSQL_RES *res1;
   MYSQL_ROW row;
   int i;
char query[10000];

int main(){

///////////////////////////////////////////////////////////////////////////////////////////////////////


   char *server = "localhost";
   char *user = "pallak";
   char *password = "pallak"; /* set me first */
   char *database = "Project_207";

   conn = mysql_init(NULL);

   /* Connect to database */
   if (!mysql_real_connect(conn, server,
         user, password, database, 0, NULL, 0)) {
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1);
   }

  
/////////////////////////////////////////////////////////////////////////////////////////////////////////

  int msock, nsock;
  pthread_t thread;

  
  struct sockaddr_in addr;
  struct sockaddr_storage;
  socklen_t addr_size;

  msock = socket(PF_INET, SOCK_STREAM, 0);
  
  addr.sin_family = AF_INET;
  addr.sin_port = htons(7802);
  addr.sin_addr.s_addr = inet_addr("127.0.0.1");

  int len=sizeof(addr);

  memset(addr.sin_zero, '\0', sizeof addr.sin_zero); 
 
 bind(msock, (struct sockaddr *) &addr, sizeof(addr));


  if(listen(msock,3)==0)
    printf("Listening...\n");
  else
    printf("Error Occured\n");

 while (1) {
        socklen_t size = sizeof(struct sockaddr_in);
        struct sockaddr_in their_addr;
        int nsock = accept(msock, (struct sockaddr*)&their_addr, &size);
        if (nsock == -1) {
            perror("accept");
        }
        else {
            /* Make a safe copy of nsock */
            int *sfsock = malloc(sizeof(int));
            if (sfsock) {
                *sfsock = nsock;
	

                if (pthread_create(&thread, NULL, process, sfsock) != 0) 
		{
                    
                }
            }
            else {
                perror("malloc");
            }
        }
    }
 
    close(msock);
    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////




void *process(void *psock)
{
  int sd = *(int*)psock;
  char role[2];
  
 recv(sd, buff, 2,0);
 //printf("%s\n",buff);
  strcpy(role, buff);
  printf("%s\n",role);
  memset(buff, '\0', sizeof(buff));

//////////////////////////////////////////////////////// SYSTEM ADMIN CODE /////////////////////////////////////////////////////////////

if((strcmp(role,"1"))==0)
{

recv(sd, buff, 50,0);
printf("%s\n",buff);
strcpy(sys_email, buff);
printf("%s\n",sys_email);
memset(buff, '\0', sizeof(buff));


recv(sd, buff, 20,0);
printf("%s\n",buff);
strcpy(sys_password, buff);
printf("%s\n",sys_password);
memset(buff, '\0', sizeof(buff));

validate_admin(sd,role,sys_email);

}
//////////////////////////////////////////////////////////////////// AGENT CODE ////////////////////////////////////////////////////////

else if((strcmp(role,"2"))== 0)
{
recv(sd, buff, 50,0);
printf("%s\n",buff);
strcpy(at_email, buff);
printf("%s\n",at_email);
memset(buff, '\0', sizeof(buff));

recv(sd, buff, 20,0);
printf("%s\n",buff);
strcpy(at_password, buff);
printf("%s\n",at_password);
memset(buff, '\0', sizeof(buff));

validate_agent(sd, at_email, at_password, role);


}
/////////////////////////////////////////////////////////// USER CODE ///////////////////////////////////////////////////////////

else if ((strcmp(role, "3"))== 0)

{

char select1_char[2];
recv(sd, buff, 2, 0 );
//printf("%s\n",buff);

strcpy(select1_char, buff);
//printf("%s\n",select1_char);

memset(buff, '\0', sizeof(buff));
char right[2] = "1";
printf("%s\n", right);
char wrong[2] = "2";
printf("%s\n", wrong);

int select1_1 = strcmp(select1_char, right);
printf("%d\n", select1_1);
int select1_2 = strcmp(select1_char, wrong);
printf("%d\n", select1_2);


if(select1_1 == 0)
{
 add_user(role, sd);
}


else if (select1_2 == 0)
{

validate_user(role, sd);

}

}

  free(psock);
  return NULL;
 }


////////////////////////////////////////////////////////// SYS ADMIN FUNCTIONS ////////////////////////////////////////////////


void validate_admin(int sd, char * role, char * sys_email)
{

sprintf(query,  "select case when exists (select *from User_Info where Email_ID=\"%s\" and Role_ID=\"%s\") then (select Password from User_Info where Email_ID=\"%s\") else \"Wrong\" end ;", sys_email,role,sys_email);
printf("%s\n", query);
if (mysql_real_query(conn, query, strlen(query))) {
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1); }

   res = mysql_use_result(conn);

   int numfields12 = mysql_num_fields(res);
   int o;

   /* output table name */
 while ((row = mysql_fetch_row(res)))
 {
  for(o=0; o<numfields12; o++)
  {
   printf("%s\n", row[o]);
   strcpy(buff, row[o]);
   send(sd,buff,strlen(buff),0);
   memset(buff, '\0', sizeof(buff));
  }
 }

while (mysql_fetch_row(res) != NULL)
{
	mysql_free_result(res);
}

sys_select_fn(sd);

}

void sys_select_fn(int sd)
{
char sys_select[2];
recv(sd, buff, 2, 0 );
printf("%s\n",buff);

strcpy(sys_select, buff);
printf("%s\n",sys_select);

memset(buff, '\0', sizeof(buff));
char right[2] = "1";
printf("%s\n", right);
char wrong[2] = "2";
printf("%s\n", wrong);

int sys_select_int = strcmp(sys_select, right);
printf("%d\n", sys_select_int);
int sys_select_int2 = strcmp(sys_select, wrong);
printf("%d\n", sys_select_int2);

if(sys_select_int == 0)
{

add_agent(sd);
sys_select_fn(sd);

}
else if(sys_select_int2 == 0)
{

 modify_agent(sd);
 sys_select_fn(sd);
}


}

void add_agent(int sd)
{
recv(sd, buff, 50,0);
//printf("%s\n",buff);
strcpy(agent_name, buff);
printf("%s\n",agent_name);
memset(buff, '\0', sizeof(buff));


recv(sd, buff, 20,0);
//printf("%s\n",buff);
strcpy(agent_password, buff);
printf("%s\n",agent_password);
memset(buff, '\0', sizeof(buff));
 

recv(sd, buff, 50,0);
//printf("%s\n",buff);
strcpy(agent_email, buff);
printf("%s\n",agent_email);
memset(buff, '\0', sizeof(buff));
 

recv(sd, buff, 10,0);
//printf("%s\n",buff);
strcpy(agent_phone, buff);
printf("%s\n",agent_phone);
memset(buff, '\0', sizeof(buff));

recv(sd, buff, 3,0);
//printf("%s\n",buff);
strcpy(agent_age, buff);
printf("%s\n",agent_age);
memset(buff, '\0', sizeof(buff));

recv(sd, buff, 2,0);
//printf("%s\n",buff);
strcpy(agent_gender, buff);
printf("%s\n",agent_gender);
memset(buff, '\0', sizeof(buff));


if (mysql_real_query(conn, "select * from System_Admin;", strlen(query))) {
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1); }

   res = mysql_use_result(conn);

   int numfields22 = mysql_num_fields(res);
    MYSQL_FIELD *field1;
   int ok;

   /* output table name */
 while ((row = mysql_fetch_row(res)))
 {
  for(ok=0; ok<numfields22; ok++)
  {
   if (ok == 0) 
          {              
             while(field1 = mysql_fetch_field(res)) 
             {
              printf("%s ", field1->name);
              sprintf(buff, "%s %s", field1->name, "\t" );
              send(sd,buff,strlen(buff),0);
             }

          }
   //printf("%s\t", row[ok]);
   sprintf(buff, "%s %s", row[ok], "\t" );
   send(sd,buff,strlen(buff),0);
  }
  sprintf(buff, "%s", "\n" );
   send(sd,buff,strlen(buff),0);
 }
  memset(buff, '\0', sizeof(buff));
  mysql_free_result(res);


recv(sd, buff, 2,0);
//printf("%s\n",buff);
strcpy(agent_airline, buff);
printf("%s\n",agent_airline);
memset(buff, '\0', sizeof(buff));

char a_role[2] = "2";

pthread_mutex_lock(&count_mutex);

sprintf(query,  "CALL add_user_info(\"%s\", \"%s\", \"%s\",\"%s\",\"%s\",\"%s\",\"%s\");", agent_name, agent_password, agent_email, agent_phone, agent_age, agent_gender, a_role);

if (mysql_real_query(conn, query, strlen(query))) {
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1);    }

sprintf(query,  "insert into Flight_Agent (Email_ID, Airline_ID) values (\"%s\",\"%s\") ;", agent_email, agent_airline);

if (mysql_real_query(conn, query, strlen(query))) {
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1);    }

  pthread_mutex_unlock(&count_mutex);

}

void modify_agent(int sd)
{
 recv(sd, buff, 50,0);
printf("%s\n",buff);
strcpy(agent_email, buff);
printf("%s\n",agent_email);
memset(buff, '\0', sizeof(buff));

sprintf(query, "select case when exists (select * from User_Info where Email_ID=\"%s\") then \"1\" else \"0\" end ;", agent_email);
printf("%s\n", query);
if (mysql_real_query(conn, query, strlen(query))) {
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1); }

   res = mysql_use_result(conn);

     
   int numfields25 = mysql_num_fields(res);
   int oh;

   /* output table name */
 while ((row = mysql_fetch_row(res)))
 {
  for(oh=0; oh<numfields25; oh++)
  {
   //printf("%s\t", row[ok]);
   sprintf(buff, "%s %s", row[oh], "\t" );
   send(sd,buff,strlen(buff),0);
  }
  sprintf(buff, "%s", "\n" );
   send(sd,buff,strlen(buff),0);
 }
  memset(buff, '\0', sizeof(buff));
  mysql_free_result(res);
  
  agent_modify(sd, agent_email);

}

void agent_modify(int sd, char * agent_email)
{

 char modify_agent_option[3];
recv(sd, buff, 3,0);
printf("%s\n",buff);
strcpy(modify_agent_option, buff);
printf("%s\n",modify_agent_option);
memset(buff, '\0', sizeof(buff));

 
 if((strcmp(modify_agent_option, "1"))==0)
{
 recv(sd, buff, 20,0);
 printf("%s\n",buff);
 strcpy(agent_name, buff);
  printf("%s\n",agent_name);
 memset(buff, '\0', sizeof(buff));
 
 sprintf(query, "update User_Info set Name=\"%s\" where Email_ID=\"%s\";",agent_name, agent_email); 
 printf("%s\n", query);
 if (mysql_real_query(conn, query, strlen(query))) {  
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1); }
}

else if((strcmp(modify_agent_option, "2"))==0)
{
 recv(sd, buff, 20,0);
 printf("%s\n",buff);
 strcpy(agent_age, buff);
  printf("%s\n",agent_age);
 memset(buff, '\0', sizeof(buff));
 
 sprintf(query, "update User_Info set Age=\"%s\" where Email_ID=\"%s\";",agent_age, agent_email); 
 printf("%s\n", query);
 if (mysql_real_query(conn, query, strlen(query))) {  
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1); }
}

else if((strcmp(modify_agent_option, "3"))==0)
{
 recv(sd, buff, 4,0);
 printf("%s\n",buff);
 strcpy(agent_phone, buff);
  printf("%s\n",agent_phone);
 memset(buff, '\0', sizeof(buff));
 
 sprintf(query, "update User_Info set Phone=\"%s\" where Email_ID=\"%s\";", agent_phone, agent_email); 
 printf("%s\n", query);
 if (mysql_real_query(conn, query, strlen(query))) {  
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1); }
}

else if((strcmp(modify_agent_option, "4"))==0)
{
 recv(sd, buff, 4,0);
 printf("%s\n",buff);
 strcpy(agent_airline, buff);
  printf("%s\n",agent_airline);
 memset(buff, '\0', sizeof(buff));
 
 sprintf(query, "update Flight_Agent set Airline_ID=\"%s\" where Email_ID=\"%s\";", agent_airline, agent_email); 
 printf("%s\n", query);
 if (mysql_real_query(conn, query, strlen(query))) {  
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1); }
}

else if((strcmp(modify_agent_option, "5"))==0)
{
 //printf("%s\n", "I am here");
 sys_select_fn(sd);
}

sprintf(query, "Select * from User_Info where Email_ID=\"%s\";",agent_email); 
 printf("%s\n", query);
 if (mysql_real_query(conn, query, strlen(query))) {  
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1); }

res = mysql_store_result(conn);
   MYSQL_FIELD * field2;
   int numfields89 = mysql_num_fields(res);
   int jk;

while ((row = mysql_fetch_row(res)))
 {
  for(jk=0; jk<numfields89; jk++)
  {
   if (jk == 0) 
          {              
             while(field2 = mysql_fetch_field(res)) 
             {
              printf("%s ", field2->name);
              sprintf(buff, "%s %s", field2->name, "\t" );
              send(sd,buff,strlen(buff),0);
             }

          }
   printf("%s\t", row[jk]);
   sprintf(buff, "%s %s", row[jk], "\t" );
   send(sd,buff,strlen(buff),0);
  }
 }

  memset(buff, '\0', sizeof(buff));
  mysql_free_result(res);
  agent_modify(sd, agent_email);

}

///////////////////////////////////////////////////////////////// AGENT FUNCTIONS ////////////////////////////////////////////////////

void at_select_fn1(int sd)
{
 recv(sd, buff, 2,0);
 printf("%s\n",buff);
 strcpy(at_select1, buff);
 printf("%s\n",at_select1);
 memset(buff, '\0', sizeof(buff));

if((strcmp(at_select1,"1"))==0)
{

add_flight(sd, at_email);
at_select_fn1(sd);
}


else if ((strcmp(at_select1,"2"))==0)
{
 modify_flight(sd, at_email);
 at_select_fn1(sd);
}

}

void validate_agent(int sd, char * at_email, char * at_password, char * role)
{

sprintf(query, "select case when exists (select *from User_Info where Email_ID=\"%s\" and Role_ID=\"%s\") then (select Password from User_Info where Email_ID=\"%s\") else \"Wrong\" end ;", at_email,role,at_email);
printf("%s\n", query);
if (mysql_real_query(conn, query, strlen(query))) {
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1); }

   res = mysql_use_result(conn);

   int numfields = mysql_num_fields(res);
   int i;

   /* output table name */
 while ((row = mysql_fetch_row(res)))
 {
  for(i=0; i<numfields; i++)
  {
   printf("%s\n", row[i]);
   strcpy(buff, row[i]);
   send(sd,buff,strlen(buff),0);
   memset(buff, '\0', sizeof(buff));
  }
 }

while (mysql_fetch_row(res) != NULL)
{
	mysql_free_result(res);
}

at_select_fn1(sd);

}

void add_flight(int sd, char * at_email)
{
  recv(sd, buff, 10,0);
printf("%s\n",buff);
strcpy(at_flightnum, buff);
printf("%s\n",at_flightnum);
memset(buff, '\0', sizeof(buff));

recv(sd, buff, 20,0);
printf("%s\n",buff);
strcpy(at_source, buff);
printf("%s\n",at_source);
memset(buff, '\0', sizeof(buff));

recv(sd, buff, 20,0);
printf("%s\n",buff);
strcpy(at_destination, buff);
printf("%s\n",at_destination);
memset(buff, '\0', sizeof(buff));

recv(sd, buff, 5,0);
printf("%s\n",buff);
strcpy(at_noofSeats, buff);
printf("%s\n",at_noofSeats);
memset(buff, '\0', sizeof(buff));

recv(sd, buff, 4,0);
printf("%s\n",buff);
strcpy(at_fare, buff);
printf("%s\n",at_fare);
memset(buff, '\0', sizeof(buff));

recv(sd, buff, 9,0);
printf("%s\n",buff);
strcpy(at_departuretime, buff);
printf("%s\n",at_departuretime);
memset(buff, '\0', sizeof(buff));

recv(sd, buff, 9,0);
printf("%s\n",buff);
strcpy(at_arrivaltime, buff);
printf("%s\n",at_arrivaltime);
memset(buff, '\0', sizeof(buff));

recv(sd, buff, 10,0);
printf("%s\n",buff);
strcpy(at_date, buff);
printf("%s\n",at_date);
memset(buff, '\0', sizeof(buff));

sprintf(query, "insert into Flight_Info(Flight_Number,Airline_Name,Airline_ID,Source,Destination,No_of_Seats,Fare,Departure_Time,Arrival_Time,Date) values(\"%s\",(select Airline_Name from Airline_Info where ID=(select Airline_ID from Flight_Agent where Email_ID=\"%s\")),(select Airline_ID from Flight_Agent where Email_ID=\"%s\") ,\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\");",at_flightnum,at_email,at_email,at_source,at_destination,at_noofSeats,at_fare,at_departuretime,at_arrivaltime, at_date);

printf("%s\n", query);
if (mysql_real_query(conn, query, strlen(query))) {
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1); }

}


void modify_flight(int sd, char * at_email)
{
sprintf(query, "select * from Flight_Info where Airline_ID=(select Airline_ID from Flight_Agent where Email_ID=\"%s\");",at_email);
 printf("%s\n", query);
 if (mysql_real_query(conn, query, strlen(query))) {  
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1); }

  res = mysql_store_result(conn);
  MYSQL_FIELD * field3;
   int numfields1 = mysql_num_fields(res);
   int j;

   /* output table name */
 while ((row = mysql_fetch_row(res)))
 {
  for(j=0; j<numfields1; j++)
  {
    if (j == 0) 
          {              
             while(field3 = mysql_fetch_field(res)) 
             {
              printf("%s ", field3->name);
              sprintf(buff, "%s %s", field3->name, "\t" );
              send(sd,buff,strlen(buff),0);
             }

          }
   printf("%s\t", row[j]);
   sprintf(buff, "%s %s", row[j], "\t" );
   send(sd,buff,strlen(buff),0);
  }
  sprintf(buff, "%s", "\n" );
   send(sd,buff,strlen(buff),0);
 }

  memset(buff, '\0', sizeof(buff));
  mysql_free_result(res);

char at_flight_id[3];
recv(sd, buff, 3,0);
printf("%s\n",buff);
strcpy(at_flight_id, buff);
printf("%s\n",at_flight_id);
memset(buff, '\0', sizeof(buff));

at_select_fn2(sd, at_flight_id);

}

void at_select_fn2(int sd, char * at_flight_id)
{
char modify_option[3];
recv(sd, buff, 3,0);
printf("%s\n",buff);
strcpy(modify_option, buff);
printf("%s\n",modify_option);
memset(buff, '\0', sizeof(buff));

if((strcmp(modify_option, "1"))==0)
{
 recv(sd, buff, 20,0);
 printf("%s\n",buff);
 strcpy(at_source, buff);
  printf("%s\n",at_source);
 memset(buff, '\0', sizeof(buff));
 
 sprintf(query, "update Flight_Info set Source=\"%s\" where ID=\"%s\";", at_source, at_flight_id); 
 printf("%s\n", query);
 if (mysql_real_query(conn, query, strlen(query))) {  
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1); }
}

else if((strcmp(modify_option, "2"))==0)
{
 recv(sd, buff, 20,0);
 printf("%s\n",buff);
 strcpy(at_destination, buff);
  printf("%s\n",at_destination);
 memset(buff, '\0', sizeof(buff));
 
 sprintf(query, "update Flight_Info set Destination=\"%s\" where ID=\"%s\";", at_destination, at_flight_id); 
 printf("%s\n", query);
 if (mysql_real_query(conn, query, strlen(query))) {  
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1); }
}

else if((strcmp(modify_option, "3"))==0)
{
 recv(sd, buff, 4,0);
 printf("%s\n",buff);
 strcpy(at_fare, buff);
  printf("%s\n",at_fare);
 memset(buff, '\0', sizeof(buff));
 
 sprintf(query, "update Flight_Info set Fare=\"%s\" where ID=\"%s\";", at_fare, at_flight_id); 
 printf("%s\n", query);
 if (mysql_real_query(conn, query, strlen(query))) {  
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1); }
}

else if((strcmp(modify_option, "4"))==0)
{
 recv(sd, buff, 9,0);
 printf("%s\n",buff);
 strcpy(at_arrivaltime, buff);
  printf("%s\n",at_arrivaltime);
 memset(buff, '\0', sizeof(buff));
 
 sprintf(query, "update Flight_Info set Arrival_Time=\"%s\" where ID=\"%s\";", at_arrivaltime, at_flight_id); 
 printf("%s\n", query);
 if (mysql_real_query(conn, query, strlen(query))) {  
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1); }
}

else if((strcmp(modify_option, "5"))==0)
{
 recv(sd, buff, 9,0);
 printf("%s\n",buff);
 strcpy(at_departuretime, buff);
  printf("%s\n",at_departuretime);
 memset(buff, '\0', sizeof(buff));
 
 sprintf(query, "update Flight_Info set Departure_Time=\"%s\" where ID=\"%s\";", at_departuretime, at_flight_id); 
 printf("%s\n", query);
 if (mysql_real_query(conn, query, strlen(query))) {  
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1); }
}

sprintf(query, "Select * from Flight_Info where ID=\"%s\";",at_flight_id); 
 printf("%s\n", query);
 if (mysql_real_query(conn, query, strlen(query))) {  
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1); }

res = mysql_store_result(conn);
MYSQL_FIELD * field4;
   int numfields19 = mysql_num_fields(res);
   int ji;

while ((row = mysql_fetch_row(res)))
 {
  for(ji=0; ji<numfields19; ji++)
  {
    if (ji == 0) 
          {              
             while(field4 = mysql_fetch_field(res)) 
             {
              printf("%s ", field4->name);
              sprintf(buff, "%s %s", field4->name, "\t" );
              send(sd,buff,strlen(buff),0);
             }

          }
   printf("%s\t", row[ji]);
   sprintf(buff, "%s %s", row[ji], "\t" );
   send(sd,buff,strlen(buff),0);
  }
 }

  memset(buff, '\0', sizeof(buff));
  mysql_free_result(res);

at_select_fn2(sd, at_flight_id);
}

////////////////////////////////////////////////////////////////// USER FUNCTIONS ///////////////////////////////////////////////////

void add_user(char * roleID, int sd1)
{

recv(sd1, buff, 50,0);
//printf("%s\n",buff);
strcpy(name, buff);
printf("%s\n",name);
memset(buff, '\0', sizeof(buff));


recv(sd1, buff, 20,0);
//printf("%s\n",buff);
strcpy(password, buff);
printf("%s\n",password);
memset(buff, '\0', sizeof(buff));
 

recv(sd1, buff, 50,0);
//printf("%s\n",buff);
strcpy(email, buff);
printf("%s\n",email);
memset(buff, '\0', sizeof(buff));
 

recv(sd1, buff, 10,0);
//printf("%s\n",buff);
strcpy(phone, buff);
printf("%s\n",phone);
memset(buff, '\0', sizeof(buff));

recv(sd1, buff, 3,0);
//printf("%s\n",buff);
strcpy(age, buff);
printf("%s\n",age);
memset(buff, '\0', sizeof(buff));

recv(sd1, buff, 2,0);
//printf("%s\n",buff);
strcpy(gender, buff);
printf("%s\n",gender);
memset(buff, '\0', sizeof(buff));


pthread_mutex_lock(&count_mutex);
sprintf(query,  "CALL add_user_info(\"%s\", \"%s\", \"%s\",\"%s\",\"%s\",\"%s\",\"%s\");", name, password, email, phone, age, gender, roleID);

if (mysql_real_query(conn, query, strlen(query))) {
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1);    }
  pthread_mutex_unlock(&count_mutex);
select_fn2(sd1);
}

void validate_user(char * role, int sd)
{
 recv(sd, buff, 50,0);
 printf("%s\n",buff);
 strcpy(email, buff);
 printf("%s\n",email);
 memset(buff, '\0', sizeof(buff));


 recv(sd, buff, 50,0);
 printf("%s\n",buff);
 strcpy(password, buff);
 printf("%s\n",password);
 memset(buff, '\0', sizeof(buff));


 sprintf(query, "select case when exists (select *from User_Info where Email_ID=\"%s\" and Role_ID=\"%s\") then (select Password from User_Info where Email_ID=\"%s\") else \"Wrong\" end;", email,role,email);
 printf("%s\n", query);
 if (mysql_real_query(conn, query, strlen(query))) {
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1); }

   res = mysql_use_result(conn);

   int numfields = mysql_num_fields(res);
   int co;

   /* output table name */
 while ((row = mysql_fetch_row(res)))
 {
  for(co=0; co<numfields; co++)
  {
   printf("%s\n", row[co]);
   strcpy(buff, row[co]);
   send(sd,buff,strlen(buff),0);
  }
    memset(buff, '\0', sizeof(buff));
 }

 while (mysql_fetch_row(res) != NULL)
 {
	mysql_free_result(res);
 }
select_fn2(sd);
}

void booking(char * email, int sd)
{
 char source[10];
 recv(sd, buff, 10,0);
 //printf("%s\n",buff);
 strcpy(source, buff);
 printf("%s\n",source);
 memset(buff, '\0', sizeof(buff));
 
 char destination[10];
 recv(sd, buff, 10,0);
 //printf("%s\n",buff);
 strcpy(destination, buff);
 printf("%s\n",destination);
 memset(buff, '\0', sizeof(buff));

 char seats[2];
 recv(sd, buff, 2,0);
 //printf("%s\n",buff);
 strcpy(seats, buff);
 printf("%s\n",seats);
 memset(buff, '\0', sizeof(buff));

 char trav_date[10];
 recv(sd, buff, 10,0);
 //printf("%s\n",buff);
 strcpy(trav_date, buff);
 printf("%s\n",trav_date);
 memset(buff, '\0', sizeof(buff));

 int seats_int = atoi(seats);
 int pass_info;

  sprintf(query, "SELECT * FROM Flight_Info WHERE Source = \"%s\" and Destination = \"%s\" and No_of_Seats >= \"%s\" and Date >= \"%s\";",source, destination, seats, trav_date);
printf("%s\n", query);
 if (mysql_query(conn,query)) {
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1);    
}

//printf("%s\n","Executed");

   res = mysql_store_result(conn);
   MYSQL_FIELD * field5;
   int numfields1 = mysql_num_fields(res);
   int j;

   /* output table name */
 while ((row = mysql_fetch_row(res)))
 {
  for(j=0; j<numfields1; j++)
  {
 if (j == 0) 
          {              
             while(field5 = mysql_fetch_field(res)) 
             {
              printf("%s ", field5->name);
              sprintf(buff, "%s %s", field5->name, "\t" );
              send(sd,buff,strlen(buff),0);
             }

          }
   printf("%s\t", row[j]);
   sprintf(buff, "%s %s", row[j], "\t" );
   send(sd,buff,strlen(buff),0);
  }
  sprintf(buff, "%s", "\n" );
   send(sd,buff,strlen(buff),0);
 }

  memset(buff, '\0', sizeof(buff));
  mysql_free_result(res);

  char flight_id[3];
 recv(sd, buff, 2,0);
 //printf("%s\n",buff);
 strcpy(flight_id, buff);
 printf("%s\n",flight_id);
 memset(buff, '\0', sizeof(buff));

 /*sprintf(query, "update Flight_Info set No_of_Seats=(No_of_Seats-\"%s\") where ID=\"%s\";insert into History_Info(Booking_Reference,Flight_Number,No_of_Seat_Chosen,Flight_ID,Email_ID,Source,Destination,Travel_Date,Booking_Date,Booking_Status) values(conv(floor(rand() * 99999999), 20, 36),((select Flight_Number from Flight_Info where ID=\"%s\")),\"%s\",\"%s\",\"%s\",((select Source from Flight_Info where ID=\"%s\")),((select Destination from Flight_Info where ID=\"%s\")),((select Date from Flight_Info where ID=\"%s\")),curdate(),'Booked');",seats, flight_id,flight_id,seats, flight_id, email,flight_id, flight_id, flight_id );
 if (mysql_query(conn,query)) {
 fprintf(stderr, "%s\n", mysql_error(conn));
 exit(1); }*/

sprintf(query, "CALL Seat_Reduction(\"%s\",\"%s\",\"%s\");", flight_id, seats, email);
 if (mysql_query(conn,query)) {
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1);  }  

 for (pass_info=0; pass_info < seats_int; pass_info++)
 {
  char p_name[10];
 recv(sd, buff, 10,0);
 //printf("%s\n",buff);
 strcpy(p_name, buff);
 printf("%s\n",p_name);
 memset(buff, '\0', sizeof(buff));
 
 char p_age[3];
 recv(sd, buff, 3,0);
 //printf("%s\n",buff);
 strcpy(p_age, buff);
 printf("%s\n",p_age);
 memset(buff, '\0', sizeof(buff));

 char p_gender[2];
 recv(sd, buff, 2,0);
 //printf("%s\n",buff);
 strcpy(p_gender, buff);
 printf("%s\n",p_gender);
 memset(buff, '\0', sizeof(buff));
 
 sprintf(query, "insert into Passenger_Info(Booking_Reference,Email_ID,Passenger_Name,Age,Gender) values((select Booking_Reference from History_Info where Booking_Time=(select max(Booking_Time) from History_Info where Email_ID=\"%s\" and Booking_Date=(select max(Booking_Date) from History_Info where Email_ID=\"%s\"))), \"%s\",\"%s\",\"%s\",\"%s\");", email,email, email, p_name, p_age, p_gender);
 if (mysql_query(conn,query)) {
 fprintf(stderr, "%s\n", mysql_error(conn));
 exit(1); }

 }

sprintf(query, "SELECT Fare FROM Flight_Info WHERE ID = \"%s\";", flight_id);
 if (mysql_query(conn,query)) {
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1);    
}

//printf("%s\n","Executed");

   res = mysql_store_result(conn);
   int numfields2 = mysql_num_fields(res);
   int g;

   /* output table name */
 while ((row = mysql_fetch_row(res)))
 {
  for(g=0; g<numfields2; g++)
  {
   printf("%s\t", row[g]);
   sprintf(buff, "%s %s", row[g], "\t" );
   send(sd,buff,strlen(buff),0);
  }
  sprintf(buff, "%s", "\n" );
   send(sd,buff,strlen(buff),0);
 }

  memset(buff, '\0', sizeof(buff));
  mysql_free_result(res);




sprintf(query, "select Booking_Reference from History_Info where Booking_Time=(select max(Booking_Time) from History_Info where Email_ID=\"%s\" and Booking_Date=(select max(Booking_Date) from History_Info where Email_ID=\"%s\"));", email,email);
 if (mysql_query(conn,query)) {
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1);    
}

//printf("%s\n","Executed");

   res = mysql_store_result(conn);
   int numfields29 = mysql_num_fields(res);
   int gi;

   /* output table name */
 while ((row = mysql_fetch_row(res)))
 {
  for(gi=0; gi<numfields29; gi++)
  {
   printf("%s\t", row[gi]);
   sprintf(buff, "%s %s", row[gi], "\t" );
   send(sd,buff,strlen(buff),0);
  }
  sprintf(buff, "%s", "\n" );
   send(sd,buff,strlen(buff),0);
 }

  memset(buff, '\0', sizeof(buff));
  mysql_free_result(res);



select_fn2(sd);
}

void history(char * email, int sd)
{
 sprintf(query, "SELECT * FROM History_Info WHERE Email_ID = \"%s\";",email);
 if (mysql_query(conn,query)) {
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1);    
}

//printf("%s\n","Executed");

   res = mysql_use_result(conn);
MYSQL_FIELD * field6;
   int numfields = mysql_num_fields(res);
   int st;

   /* output table name */
 while ((row = mysql_fetch_row(res)))
 {
  for(st=0; st<numfields; st++)
  {
 if (st == 0) 
          {              
             while(field6 = mysql_fetch_field(res)) 
             {
              printf("%s ", field6->name);
              sprintf(buff, "%s %s", field6->name, "\t" );
              send(sd,buff,strlen(buff),0);
             }

          }
   printf("%s\t", row[st]);
   sprintf(buff, "%s %s", row[st], "\t" );
   send(sd,buff,strlen(buff),0);
  }
  sprintf(buff, "%s", "\n" );
  send(sd,buff,strlen(row[st]),0);
  
  memset(buff, '\0', sizeof(buff));
 }

while (mysql_fetch_row(res) != NULL)
{
	mysql_free_result(res);
} 
memset(buff, '\0', sizeof(buff));

}

void cancel_booking(char * email, int sd)
{
 sprintf(query, "SELECT * FROM History_Info WHERE Email_ID = \"%s\";",email);
 if (mysql_query(conn,query)) {
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1);    
}

//printf("%s\n","Executed");

   res = mysql_use_result(conn);

   int numfields = mysql_num_fields(res);
    MYSQL_FIELD *field;
   int so;

   /* output table name */
 while ((row = mysql_fetch_row(res)))
 {
  for(so=0; so<numfields; so++)
  {
if (so == 0) 
          {              
             while(field = mysql_fetch_field(res)) 
             {
              printf("%s ", field->name);
              sprintf(buff, "%s %s", field->name, "\t" );
              send(sd,buff,strlen(buff),0);
             }

          }
    printf("%s\t", row[so]);
   sprintf(buff, "%s %s", row[so], "\t" );
   send(sd,buff,strlen(buff),0);
  }
  sprintf(buff, "%s", "\n" );
   send(sd,buff,strlen(buff),0);
   memset(buff, '\0', sizeof(buff));
 }
while (mysql_fetch_row(res) != NULL)
{
	mysql_free_result(res);
}
memset(buff, '\0', sizeof(buff));

char book_ref[8];
recv(sd, buff, 8,0);
printf("%s\n",buff);
strcpy(book_ref, buff);
printf("%s\n",book_ref);
memset(buff, '\0', sizeof(buff));

char status[10] = "Cancelled";

sprintf(query, "update History_Info set Booking_Status=\"%s\" where Booking_Reference=\"%s\";", status, book_ref); 
 printf("%s\n", query);
 if (mysql_real_query(conn, query, strlen(query))) {  
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1); }

sprintf(query, "select * from History_Info where Booking_Reference=\"%s\";", book_ref); 
 printf("%s\n", query);
 if (mysql_real_query(conn, query, strlen(query))) {  
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1); }

   res = mysql_use_result(conn);

   int numfieldsk = mysql_num_fields(res);
   int k;

   /* output table name */
 while ((row = mysql_fetch_row(res)))
 {
  for(k=0; k<numfieldsk; k++)
  {
    printf("%s\t", row[k]);
   sprintf(buff, "%s %s", row[k], "\t" );
   send(sd,buff,strlen(buff),0);
   memset(buff, '\0', sizeof(buff));
  }
  sprintf(buff, "%s", "\n" );
   send(sd,buff,strlen(buff),0);
    memset(buff, '\0', sizeof(buff));
 }

while (mysql_fetch_row(res) != NULL)
{
	mysql_free_result(res);
}
memset(buff, '\0', sizeof(buff));
select_fn2(sd);

}

void edit_user(char * email, int sd)
{
 char modify_user_option[3];
 recv(sd, buff, 3,0);
 printf("%s\n",buff);
 strcpy(modify_user_option, buff);
 printf("%s\n",modify_user_option);
 memset(buff, '\0', sizeof(buff));
 
 if((strcmp(modify_user_option, "1"))==0)
 {
  recv(sd, buff, 20,0);
  printf("%s\n",buff);
  strcpy(name, buff);
  printf("%s\n",name);
  memset(buff, '\0', sizeof(buff));
 
  sprintf(query, "update User_Info set Name=\"%s\" where Email_ID=\"%s\";",name, email); 
  printf("%s\n", query);
  if (mysql_real_query(conn, query, strlen(query))) {  
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1); }
 }

 else if((strcmp(modify_user_option, "2"))==0)
 {
  recv(sd, buff, 20,0);
  printf("%s\n",buff);
  strcpy(age, buff);
  printf("%s\n",age);
  memset(buff, '\0', sizeof(buff));
 
  sprintf(query, "update User_Info set Age=\"%s\" where Email_ID=\"%s\";",age, email); 
  printf("%s\n", query);
  if (mysql_real_query(conn, query, strlen(query))) {  
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1); }
 }

 else if((strcmp(modify_user_option, "3"))==0)
 {
  recv(sd, buff, 10,0);
  printf("%s\n",buff);
  strcpy(phone, buff);
  printf("%s\n",phone);
  memset(buff, '\0', sizeof(buff));
 
  sprintf(query, "update User_Info set Phone=\"%s\" where Email_ID=\"%s\";", phone, email); 
  printf("%s\n", query);
  if (mysql_real_query(conn, query, strlen(query))) {  
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1); }
 }

 else if((strcmp(modify_user_option, "4"))==0)
 {
  printf("%s\n", "Yeah going back!");
  select_fn2(sd);
  
 }


sprintf(query, "Select * from User_Info where Email_ID=\"%s\";",email); 
 printf("%s\n", query);
 if (mysql_real_query(conn, query, strlen(query))) {  
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1); }

res = mysql_store_result(conn);
   int numfields69 = mysql_num_fields(res);
   int gh;

while ((row = mysql_fetch_row(res)))
 {
  for(gh=0; gh<numfields69; gh++)
  {
   printf("%s\t", row[gh]);
   sprintf(buff, "%s %s", row[gh], "\t" );
   send(sd,buff,strlen(buff),0);
  }
 }

  memset(buff, '\0', sizeof(buff));
  mysql_free_result(res);

edit_user(email, sd);

}

void select_fn2(int sd)
{
 char select2[2];
 recv(sd, buff, 2, 0 );
 printf("%s\n",buff);
 strcpy(select2, buff);
 printf("%s\n",select2);
 memset(buff, '\0', sizeof(buff));

 if((strcmp(select2, "1")) == 0)
 {
  booking(email, sd);
 }
 else if((strcmp(select2, "2")) == 0)
 {
  history(email, sd);
  select_fn2(sd);
 }

 else if((strcmp(select2, "3")) == 0)
 {
  cancel_booking(email, sd);
 }
 else if((strcmp(select2, "4")) == 0)
 {
  edit_user(email, sd);
 }
}

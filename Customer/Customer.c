/*Main code for the client side*/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include<arpa/inet.h>
#include <stdlib.h>
#include <openssl/sha.h>


void select_fn1();
void add_user();
void validate_user();
void select_fn2();
void ticket_booking();
void booking_history();
void cancel_ticket();
void edit_profile();

char role;
char name[10];
char password[20];
char pass_hash[50];
char email[50]; 
char phone[11];
char age[3];
char gender[2]; 

char buff[2000000];
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

  
 role = '3';
 strcpy(buff, &role);
 send(csock,buff,strlen(buff),0);
 memset(buff, '\0', sizeof(buff));

 select_fn1();

 return 0;
}


//////////////////////////////////////////// 1st SELECTION FUNCTION ///////////////////////////////////////


void select_fn1()
{
 char select1[2];
 char conf1[2];

 printf("%s\n", "Please select the appropriate number\n 1. New User\n 2. Existing User\n");
 scanf("%s", select1);
 //printf("%s\n", select1);
 printf("Your selection is %s\n",select1);


 if((strcmp(select1, "1")) == 0)
 {
  printf("%s\n", "Type Y if correct or Type N to modify your selection");
  scanf("%s", conf1);

  if((strcmp(conf1, "Y"))== 0)
  {
   strcpy(buff, select1);
   send(csock,buff,strlen(select1),0);
   memset(buff, '\0', sizeof(buff));

   add_user();
   select_fn2();  
  }
  else if((strcmp(conf1, "N")) == 0)
  {
   printf("%s\n", "Please select again.");
   select_fn1();
  } 
  else
  {
   printf("%s\n", "Please enter the correct option.");
   select_fn1();
  }
 }

 else if((strcmp(select1, "2")) == 0)
 {
  printf("%s\n", "Type Y if correct or Type N to modify your selection");
  scanf("%s", conf1);

  if((strcmp(conf1, "Y"))== 0)
  {
   strcpy(buff, select1);
   send(csock,buff,strlen(select1),0);
   memset(buff, '\0', sizeof(buff));

   validate_user();  
  }
  else if((strcmp(conf1, "N")) == 0)
  {
   printf("%s\n", "Please select again.");
   select_fn1();
  }  
  else
  {
   printf("%s\n", "Please enter the correct option.");
   select_fn1();
  } 
 }
 
 else
 { 
  printf("%s\n", "Please enter the correct option.");
  select_fn1();
 }  

return;
}


///////////////////////////////////////////// ADD USER FUNCTION //////////////////////////////////////////

void add_user()
{
 printf("%s\n", "Welcome...\nEnter the following Details:\n");

 printf("Name:");
 scanf("%s", name);
 strcpy(buff, name);
 send(csock,buff,strlen(name),0);

 printf("Password:");
 scanf("%s", password);
 //SHA1(password, strlen(password), pass_hash); 
 //printf("%s\n", pass_hash);
 strcpy(buff, password);
 send(csock,buff,strlen(password),0);

 printf("Email ID:");
 scanf("%s", email);
 strcpy(buff, email);
 send(csock,buff,strlen(email),0);

 printf("Phone No:");
 scanf("%s", phone);
 strcpy(buff, phone);
 send(csock,buff,strlen(phone),0);

 printf("age:");
 scanf("%s", age);
 strcpy(buff, age);
 send(csock,buff,strlen(age),0);

 printf("Gender:");
 scanf("%s", gender);
 strcpy(buff, gender);
 send(csock,buff,strlen(gender),0); 

 return;
}

////////////////////////////////////////////// VALIDATE USER FUNCTION ///////////////////////////////////////////////

void validate_user()
{
 printf("%s", "Email:");
 scanf("%s", email);
 strcpy(buff, email);
 send(csock, buff, strlen(email), 0);
 
 printf("%s", "Password:");
 scanf("%s", password);
 //SHA1(password, strlen(password), pass_hash); 
 //printf("%s\n", pass_hash);
 strcpy(buff, password);
 send(csock, buff, strlen(password), 0);
 memset(buff, '\0', sizeof(buff));

 char user_validate[50];
 
 recv(csock, buff, sizeof(buff),0);
 strcpy(user_validate, buff); 
 //printf("%s\n",user_validate);
 //printf("%s\n",buff);
 //printf("%s\n", password);
 memset(buff, '\0', sizeof(buff));
 //char right[10] = "Right";

 int uv = strcmp(user_validate, password);
 //printf("%d\n", uv);
 if(uv == 0)
 {
  printf("%s\n", "Authentication Successfull!!!");
  select_fn2();  
 }
 else
 {
  printf("%s\n", "Please check the username and password!!");
  exit(1);
 }
 
return;
}

/////////////////////////////////////////////// 2nd SELECTION FUNCTION //////////////////////////////////////////////

void select_fn2()
{
 char select2[2];
 char conf2[2];
 printf("%s\n", "Please choose an option: \n 1. Book Tickets \n 2. View Booking History \n 3. Cancel/Modify Booking \n 4. Edit Profile \n 5. Logout");
 scanf("%s",select2 );
  
 if((strcmp(select2,"1"))== 0)
 {
  printf("%s\n", "Type Y if correct or Type N to modify your selection");
  scanf("%s", conf2);

  if((strcmp(conf2, "Y"))== 0)
  {
   strcpy(buff, select2);
   send(csock,buff,strlen(select2),0);

   ticket_booking();  
  }
  else if((strcmp(conf2, "N")) == 0)
  {
   printf("%s\n", "Please select again.");
   select_fn2();
  }  
  else
  {
   printf("%s\n", "Please enter the correct option.");
   select_fn2();
  } 
 } 

 else if((strcmp(select2,"2"))== 0)
 {
  printf("%s\n", "Type Y if correct or Type N to modify your selection");
  scanf("%s", conf2);

  if((strcmp(conf2, "Y"))== 0)
  {
   strcpy(buff, select2);
   send(csock,buff,strlen(select2),0);
   //memset(buff, '\0', sizeof(buff));
  
   booking_history(); 
    //select_fn2(); 
  }
  else if((strcmp(conf2, "N")) == 0)
  {
   printf("%s\n", "Please select again.");
   select_fn2();
  }  
  else
  {
   printf("%s\n", "Please enter the correct option.");
   select_fn2();
  } 
 } 

 else if((strcmp(select2,"3"))== 0)
 {
  printf("%s\n", "Type Y if correct or Type N to modify your selection");
  scanf("%s", conf2);

  if((strcmp(conf2, "Y"))== 0)
  {
   strcpy(buff, select2);
   send(csock,buff,strlen(select2),0);

   cancel_ticket(); 
  }
  else if((strcmp(conf2, "N")) == 0)
  {
   printf("%s\n", "Please select again.");
   select_fn2();
  }  
  else
  {
   printf("%s\n", "Please enter the correct option.");
   select_fn2();
  } 
 } 

 else if((strcmp(select2,"4"))== 0)
 {
  printf("%s\n", "Type Y if correct or Type N to modify your selection");
  scanf("%s", conf2);

  if((strcmp(conf2, "Y"))== 0)
  {
   strcpy(buff, select2);
   send(csock,buff,strlen(select2),0);

   edit_profile();  
  }
  else if((strcmp(conf2, "N")) == 0)
  {
   printf("%s\n", "Please select again.");
   select_fn2();
  }  
  else
  {
   printf("%s\n", "Please enter the correct option.");
   select_fn2();
  } 
 } 

 else if((strcmp(select2,"5"))== 0)
 {
  printf("%s\n", "Type Y if correct or Type N to modify your selection");
  scanf("%s", conf2);

  if((strcmp(conf2, "Y"))== 0)
  {
   exit(1);
  }
  else if((strcmp(conf2, "N")) == 0)
  {
   printf("%s\n", "Please select again.");
   select_fn2();
  }  
  else
  {
   printf("%s\n", "Please enter the correct option.");
   select_fn2();
  } 
 } 
 
 else
 {
  printf("%s\n", "Please select the correct option"); 
  select_fn2();
 }

return;
}

////////////////////////////////////////////// TICKET BOOKING FUNCTION //////////////////////////////////////////////

void ticket_booking()
{
 char source[10];
 char destination[10];
 char seats[2];
 char trav_date[10];
 char flightID[3];

 printf("%s\n", "Enter your trip details below:\n");

 printf("%s", "Source:");
 scanf("%s", source);
 strcpy(buff, source);
 send(csock,buff,strlen(source),0);

 printf("%s", "Destination:"); 
 scanf("%s", destination);
 strcpy(buff, destination);
 send(csock,buff,strlen(destination),0);

 printf("%s", "Number of seats:");
 scanf("%s", seats);
 strcpy(buff, seats);
 int seats_int = atoi(seats);
 //printf("%d\n", seats_int);
 send(csock,buff,strlen(seats),0);

 printf("%s", "Date of travel:");
 scanf("%s", trav_date);
 strcpy(buff, trav_date);
 //int seats_int = atoi(seats);
 //printf("%s\n", trav_date);
 send(csock,buff,strlen(trav_date),0);

 char fligts_avail[200];
 
 recv(csock, buff, sizeof(buff),0);
 strcpy(fligts_avail, buff); 
 printf("%s\n",fligts_avail);
 //printf("%s\n",buff);
 memset(buff, '\0', sizeof(buff));

 char flight_id[3];
 printf("%s\n","Please select the Flight ID of the flight you would wish to travel");
 scanf("%s", flight_id);
 strcpy(buff, flight_id);
 send(csock,buff,strlen(flight_id),0);
 
 printf("%s", "Please enter the Passenger information below: \n");

 int pass_info;
 for(pass_info = 0; pass_info < seats_int; pass_info++)
 {
  char p_name[10];
  char p_age[3];
  char p_gender[2];

  printf("%d\n", (pass_info+1));

  printf("Name:");
  scanf("%s", p_name);
  strcpy(buff, p_name);
  send(csock,buff,strlen(p_name),0);
  memset(buff, '\0', sizeof(buff));
 
  printf("age:");
  scanf("%s", p_age);
  strcpy(buff, p_age);
  send(csock,buff,strlen(p_age),0);
  memset(buff, '\0', sizeof(buff)); 

  printf("Gender:");
  scanf("%s", p_gender);
  strcpy(buff, p_gender);
  send(csock,buff,strlen(p_gender),0); 
  memset(buff, '\0', sizeof(buff));
  
  printf("%s\n", " ");
 }

char flight_fare_char[5];
recv(csock, buff, sizeof(buff),0);
strcpy(flight_fare_char, buff);

int flight_fare;
char s[2];

flight_fare = atoi(flight_fare_char);

int booking_amt = flight_fare*seats_int;
printf("Based on the flight you have chosen, the booking amount is %d\n",booking_amt);

/*printf("Enter Y to continue to payments\n");
   printf("\nPlease enter your card number:");
   printf("\nPlease enter the expiration date in mmyyyy format:");
   printf("\nPlease enter the security code:");*/
   
char booking_ref[10];
recv(csock, buff, sizeof(buff),0);
strcpy(booking_ref, buff);
printf("%s\n", booking_ref);

char sys[100];
sprintf(sys, "python mailpy.py \"%s\"", email);

 system(sys);
printf("%s\n", "Your Booking has been done. Please check for the confirmation email");

select_fn2();
return;
}

////////////////////////////////////////////////// VIEW BOOKING HISTORY FUNCTION /////////////////////////////////

void booking_history()
{ 
 char booking_info[10000];
 printf("%s\n", "Your Booking history is as below:");
 recv(csock, buff,sizeof(buff) ,0);
 strcpy(booking_info, buff);
 printf("%s\n", booking_info);
 memset(buff, '\0', sizeof(buff));
 select_fn2();
 
}

///////////////////////////////////////////////// CANCEL BOOKING ///////////////////////////////////////////////////////


void cancel_ticket()
{
 char c_booking_info[10000];
 recv(csock, buff, sizeof(buff),0);
 strcpy(c_booking_info, buff); 
 printf("%s\n",c_booking_info);
  memset(buff, '\0', sizeof(buff));

 char book_ref[8];
 printf("%s", "Select the booking reference number of the trip you would like to cancel:");
  scanf("%s", book_ref);
  strcpy(buff, book_ref);
  send(csock,buff,strlen(book_ref),0); 
  memset(buff, '\0', sizeof(buff));

 char status[2000];
recv(csock, buff, sizeof(buff),0);
 strcpy(status, buff); 
 printf("%s\n",status);
   memset(buff, '\0', sizeof(buff));
select_fn2(); 
}


//////////////////////////////////////////////// EDIT USER INFO ///////////////////////////////////////////////////////

void edit_profile()
{ 
 char i[2];
 while(1)
 {
  printf("Select the parameter to be modified: \n 1. Name\n 2. Age\n 3. Phone Number \n 4. Previous Menu");
  scanf("%s",i);
  if((strcmp(i,"1"))==0)
  {
   strcpy(buff,i);
   send(csock,buff,strlen(i),0);
   memset(buff, '\0', sizeof(buff));
   printf("\nEnter the new name:");
   scanf("%s",name);
   strcpy(buff,name);
   send(csock,buff,strlen(name),0);
   memset(buff, '\0', sizeof(buff));
  }
  else if((strcmp(i,"2"))==0)
  {
   strcpy(buff,i);
   send(csock,buff,strlen(i),0);
   memset(buff, '\0', sizeof(buff));
   printf("\nEnter your age:");
   scanf("%s",age);
   strcpy(buff,age);
   send(csock,buff,strlen(age),0);
   memset(buff, '\0', sizeof(buff));
  }
  else if((strcmp(i,"3"))==0)
  {
   strcpy(buff,i);
   send(csock,buff,strlen(i),0);
   memset(buff, '\0', sizeof(buff));
   printf("\nEnter the new phone number:");
   scanf("%s",phone);
   strcpy(buff,phone);
   send(csock,buff,strlen(phone),0);
   memset(buff, '\0', sizeof(buff));
  }
  else if((strcmp(i,"4"))==0)
  {
    strcpy(buff,i);
   send(csock,buff,strlen(i),0);
   memset(buff, '\0', sizeof(buff));
     //printf("%s\n", "Yeah going back!");
   select_fn2();
  }
printf("Your Personal details have been updated\n");
			char personaldetails[600];
			recv(csock, buff, sizeof(buff),0);
			strcpy(personaldetails, buff);
			printf("%s\n",personaldetails);
			memset(buff, '\0', sizeof(buff));
 }
edit_profile();
}


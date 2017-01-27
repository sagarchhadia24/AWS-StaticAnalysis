/****************************************************************************
 Program: Phone Service 2.0 (phonev2a.c)
 Programmer: Robert Chanphakeo (email: cdk_champs@hotmail.com)

 Configration control
   version	date		details
     1.0      01/06/01		Original code
     1.3      02/06/01		Deleting database entries method modifed.
     1.4      02/06/01		Auto-Recovery code implemented but not in use.
     1.7      03/06/01		Load database option added.
     1.8      03/06/01		Exit and save option added.
     1.9      03/06/01		Interface implemented.
     2.0      04/06/01		Load Database file valid check codes added.
				(ASCII file check only).
  Minor bugs found:
     AddEntry:
     While Rentering data for room or phone then leaving it blank
     it will not exit but instead storing/displaying your input
     as being ZERO.

 Files Included.
 ---------------
     samdb2a.txt - sample database
     userm2a.doc - user manual

TESTED/COMPILED - Windows 2000 Pro/Borland Turbo C++ 3.0
*****************************************************************************/
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <graphics.h>
#include <ctype.h>
#include <string.h>
#define MAXDB 500 /* Maximum number of Entries */


/* Globe variables */
int i; /*globe index*/
long int phone[MAXDB+1];
int room[MAXDB+1];
/* phone_tmp & room_tmp array's are temp storage used for delete recovery */
long int phone_tmp[MAXDB+1];
int room_tmp[MAXDB+1];
void AddEntry(int, long int);
int add_count=0; /* master counter for entries added */
int current_e_add; /* counter for current entries added within a giventime.*/
int DeleteEntry(int, long int);
int FindPhone(long int);
int FindRoom(int);
int phone_found,room_found;
int del_entry; /* counts del entry at a given time */
int tot_del_entry=0; /* master del counter */
int ListAll(void);
int SortAllEntries(char);
int GeTotalEntries(void);
int chkstrdig (char str[], int range);
char menu(void);
void LoadDB(void); /* load database from file function */
void exitmenu(void);
void drawscreen(void);
void refreshscreen(void);

char dbload[80]; /* loaded database */

void main(void)
{
char iroom[80],iphone[80],add_quit;
char option,sortopt,exit_opt; /* menu,sort and exit option*/
int phone_check,room_check,delete_check,sort_check,list_check;
int iroom_search,iroom_del;
int int_iroom,total_entries;
int error_iphone,error_iroom; /* used to check inputs error's  */
long int longint_iphone;
long int iphone_search;
long int iphone_del;

/* Init while no valid database file is loaded program will work in RAM! */
strcpy(dbload, "No database file loaded (RAM MODE!).");

/* MAIN MENU */
do
 {
  do
   { option = menu();
     if (option == '1') /* AddEntry Option */
     {  current_e_add=0; /*init current entries added to zero.*/
	for (i=add_count; i < MAXDB; i++)
	{   clrscr();
	    refreshscreen();
	    drawscreen();
	    gotoxy(1,4);
	    printf(">> Add Entry <<");
	    gotoxy(1,25);
	    cprintf("Please Add Your Entry, leave blank to quit to Main Menu");
	    gotoxy(1,6);
	    printf("Enter Room  Number[%3d]: ",i+1);
	    gets(iroom);

	    if (iroom[0] == '\0' ) /* user hits enter - quits */
	    {  gotoxy(1,25);
	       cprintf("You chose to quit: Entry %d was not added to the database.",i+1);
	       getch();
	       break;
	    }
	    printf("Enter Phone Number[%3d]: ",i+1);
	    gets(iphone);

	    if (iphone[0] == '\0') /* user hits enter - quits */
	    {  gotoxy(1,25);
	       cprintf("You chose to quit: Entry %d was not added to the database.",i+1);
	       getch();
	       break;
	    }
	    /* check the string for valid inputs */
	    error_iroom = chkstrdig(iroom,4);
	    error_iphone = chkstrdig(iphone,8);
	    /* loop's while room input error (out of range/character) */
	    while(error_iroom != 0)
	    {	if (error_iroom == -1)
		{  clrscr();
		   refreshscreen();
		   drawscreen();
		   gotoxy(1,4);
		   printf(">> Add Entry <<");
		   gotoxy(1,25);
		   cprintf("Error: Room  Number - out of Range, Your entry was greater than 4 digits. ");
		   gotoxy(1,6);
		   printf("Renter Room  Number[%3d]: ",i+1);
		   gets(iroom);
		}
		if (error_iroom == -2)
		{  clrscr();
		   refreshscreen();
		   drawscreen();
		   gotoxy(1,4);
		   printf("*** Add Entry ***");
		   gotoxy(1,25);
		   cprintf("Error: Room  Number - Character(s) detected, character(s) are not allowed.");
		   gotoxy(1,6);
		   printf("Renter Room  Number[%3d]: ",i+1);
		   gets(iroom);
		}/* checks string room input if valid */
		error_iroom = chkstrdig(iroom,4);
	    }/*loop's while phone input error (out of range/character) */
	    while(error_iphone !=0)
	    {	if (error_iphone == -1)
		{  clrscr();
		   refreshscreen();
		   drawscreen();
		   gotoxy(1,4);
		   printf(">> Add Entry <<");
		   gotoxy(1,25);
		   cprintf("Error: Phone Number - out of Range, Your entry was greater than 8 digits. ");
		   gotoxy(1,6);
		   printf("Room  Number[%3d] Entry: %s",i+1,iroom);
		   gotoxy(1,7);
		   printf("Renter Phone Number[%3d]: ",i+1);
		   gets(iphone);
		}
		if (error_iphone == -2)
		{  clrscr();
		   refreshscreen();
		   drawscreen();
		   gotoxy(1,4);
		   printf(">> Add Entry <<");
		   gotoxy(1,25);
		   cprintf("Error: Phone Number - Character(s) detected, character(s) are not allowed.");
		   gotoxy(1,6);
		   printf("Room  Number[%3d] Entry: %s",i+1,iroom);
		   gotoxy(1,7);
		   printf("Renter Phone Number[%3d]: ",i+1);
		   gets(iphone);
		}/* checks phone input valid */
		error_iphone = chkstrdig(iphone,8);
	    }
	    /* no room or phone input error - addentry */
	    if (error_iroom == 0 && error_iphone == 0)
	    {  int_iroom = atoi(iroom); /* converts string to int */
	       longint_iphone = atol(iphone); /* converts string to long int */
	       current_e_add++;
	       AddEntry(int_iroom,longint_iphone);
	    }
	}
	if (add_count == MAXDB) /* database full */
	{  gotoxy(1,25);
	   cprintf("\aDatabase is full!: %d entries were added, ",add_count);
	   cprintf("that is the Maximum No. I can hold.");
	   getch();
	}
     }
     else
     if (option == '2') /* DeleteEntry option */
     {  del_entry = 0; /* Initialize del_entry counter zero */
	clrscr();
	refreshscreen();
	drawscreen();
	gotoxy(1,4);
	printf(">> Delete Entry <<");
	gotoxy(1,6);
	printf("Enter room  number to delete: ");
	scanf("%d",&iroom_del);
	flushall(); /* clears buffer */

	printf("Enter phone number to delete: ");
	scanf("%ld",&iphone_del);
	flushall();

	delete_check = DeleteEntry(iroom_del,iphone_del);

	if (delete_check == 0)/*successfully found or deleted entries display*/
	{  gotoxy(1,25);
	   cprintf("Successful: There are currently %d entries in the database, ",add_count);
	   cprintf("deleted %d.",del_entry);
	   getch();
	}
	if (delete_check == -1) /* error: does not delete if db not found */
	{  gotoxy(1,25);
	   cprintf("Error: The Room No./Phone No. Your looking for was Not Found.                  ");
	   getch();
	}

     }
     else
     if (option == '3') /* FindPhone Option */
     {  phone_found = 0; /*initialize phone no. found to zero */
	clrscr();
	refreshscreen();
	drawscreen();
	gotoxy(1,4);
	printf(">> Find Room Number <<");

	gotoxy(1,6);
	printf("Enter the phone number to search for: ");
	scanf("%ld",&iphone_search);
	flushall(); /* clears buffer */

	phone_check = FindPhone(iphone_search);

	if (phone_check == 0) /* return = 0 Phone found */
	{  gotoxy(1,25);
	   cprintf("Successful: There are currently %d entries in the database, ",add_count);
	   /* phone_found(globe), counts phone no. found(within FindPhone function) */
	   printf("found %d.",phone_found);
	   getch();
	}
	if (phone_check == -1) /* return = -1 Phone not found */
	{  gotoxy(1,25);
	   cprintf("Error: The Phone No. Your looking for was Not Found.");
	   getch();
	}
     }
     else
     if (option == '4') /* FindRoom Option */
     {  room_found = 0;  /* initialize room no. found to zero */
	clrscr();
	refreshscreen();
	drawscreen();
	gotoxy(1,4);
	printf(">> Find Phone Number <<");

	gotoxy(1,6);
	printf("Enter the room number to search for: ");
	scanf("%d",&iroom_search);
	flushall();

	room_check = FindRoom(iroom_search);

	if (room_check == 0) /* return = 0 Room found */
	{  gotoxy(1,25);
	   cprintf("Successful: There are currently %d entries in the database, ",add_count);
	   /* room_found is globe it counts room no. found in FindRoom function */
	   cprintf("found %d.",room_found);
	   getch();
	}
	if (room_check == -1) /* return = -1 Room was not found */
	{  gotoxy(1,25);
	   cprintf("Error: The Room No. Your looking for was Not Found.");
	   getch();
	}

     }
     else
     if (option == '5') /* ListAll option */
     {  clrscr();
	refreshscreen();
	drawscreen();
	gotoxy(1,4);
	printf(">> ListAll <<\n\n");

	list_check = ListAll();

	if (list_check == 0) /* return 0 if entries are in database */
	{  gotoxy(1,25);
	   cprintf("List Sucuessful");
	   getch();
	}
	if (list_check == -1) /* return -1 - emptylist */
	{
	   gotoxy(1,25);
	   cprintf("Empty List");
	   getch();
	}
     }
     else
     if (option == '6') /* Getotalentries option */
     {  total_entries = GeTotalEntries();
	gotoxy(1,25);
	cprintf("There are currently %d entries stored in the Database.",total_entries);
	getch();
     }
     else
     if (option == '7') /* Sort Option */
     {  clrscr();
	refreshscreen();
	drawscreen();
	gotoxy(1,4);
	printf(">> Sort All Entries <<");
	gotoxy(1,6);
	printf("Press 'A' to sort database in [A]scending order");
	gotoxy(1,7);
	printf("Press 'D' to sort database in [D]escending order.");
	gotoxy(1,9);
	printf("Note: Database is sorted by phone no. entries.");
	sortopt = getch();
	flushall();

	sort_check = SortAllEntries(sortopt);
	getch();
	if (sort_check == 0) /* return = 0 - entries, in db & was sorted */
	{  gotoxy(1,25);
	   cprintf("Database was successfully Sorted.                                      ");
	   getch();
	}
	if (sort_check == -1) /* return = -1 - if db is empty */
	{  gotoxy(1,25);
	   cprintf("Database was not sorted - Database is empty!");
	   getch();
	}
     }
     else
     if (option == '8') /* Load Database from file option */
     {  clrscr();
	refreshscreen();
	drawscreen();
	gotoxy(1,4);
	printf(">> Load Database <<");
	LoadDB();
     }
     else
     if (option == '9') /* exit option */
     {  gotoxy(1,25);
	cprintf("Do you really want to exit?, Press 'Y' to confirm, anykey to cancel");
	exit_opt = getch();
	flushall();
	if (exit_opt == 'y' || exit_opt == 'Y')
	{  clrscr();
	   refreshscreen();
	   drawscreen();
	   gotoxy(1,4);
	   printf(">> Exit To system <<\n\n");
	   exitmenu();
	}
     }
     else /* user presses an invalid key display msg error */
     {  gotoxy(1,25);
	cprintf("Error: Invalid option! Select an option between 1 and 9");
	getch();
	flushall(); /* clears buffer */
     }
   }while  (option > '9' || option < '1' );
 }while (option != '`'); /* unlimated loop */
}
/****************************************************************************
	Main Functions
	--------------
	AddEntry, DeleteEntry, FindPhone, FindRoom,
	ListAll, GeTotalEntries, SortallEntries.
*****************************************************************************/
/*---------------------------------------------------------------------------
	AddEntry Function
	-----------------
	Does not return any values it is used to added valid inputs(only) into
	the database and display the entries which was added.
	* A valid inputs are positive numbers only.
	1. Room no. input with less than or equal to 4 digits only.
	2. Phone no. input with less than or equal to 8 digits only.
	3. Input of Zero for room no. or phone no. inputs is an valid.
----------------------------------------------------------------------------*/
void AddEntry(int r, long int p)
{
     room[i] = r;  /* store r(room) input into db */
     phone[i] = p; /* store r(room) input into db */
     add_count++;  /* keeps track of total entries added. */
     printf("\nRoom No.  [%-4d]\nPhone No. [%-8ld]\n%d entries added.",r,p,current_e_add);
     getch();
}
/*---------------------------------------------------------------------------
	DeleteEntry function
	--------------------
	Used to delete entrys in the database.
	Returns 0 if room no. & phone no. was found in the database.
	Returns -1 if room no. & phone no. is not found in the database.

	Note: Auto-Recovery was implemented into this function but was never
	used. room_tmp and phone_tmp arrays contain the deleted data
	which maybe used for recovery.
---------------------------------------------------------------------------*/
int DeleteEntry(int r, long int p)
{
int k,x,del_found_flag=-1,loop_mov_stop,loop_mov,count_del=0;
char del_me; /* Variable to confirm delete */

    for(k=0; k < add_count; k++)
    {  if (add_count != 0) /* checks if database is not empty */
       {  if (r == room[k] && p == phone[k])
	  {  gotoxy(1,8);
	     printf("Match Found: \n");
	     printf("Room No. [%-4d]\tPhone No. [%-8ld] was found in record No. [%3d  ]\n",room[k],phone[k],k+1);
	     del_found_flag = 0; /* when found, set's del_found_flag=0 */
	     gotoxy(1,25);
	     cprintf("Delete record [%3d  ]?, Press 'Y' to confirm, anykey to cancel.",k+1);
	     del_me = getch();
	     flushall();
	     if (del_me == 'y' || del_me == 'Y')
	     {  room_tmp[tot_del_entry] = room[k]; /* tmp array storage for room found */
		room[k] = -1; /* marks -1 for deleted */
		phone_tmp[tot_del_entry] = phone[k];
		phone[k] = -1;
		del_entry++; /* counter for deleted entry */
		tot_del_entry++; /* counter for temp storage */
	     }
	  }
       }
    }
    if (add_count !=0) /* if database is not empty process with delete */
    {/* keeps looping while move up position is not = to deleted entry */
       for (x=0; x < del_entry; x++)
       {   for (k=0; k < add_count; k++)
	   {/* When -1 is found it moves everything by one */
	      if (room[k] == -1 && phone[k] == -1)
	      {  loop_mov_stop=0;
		 loop_mov =0;
		 count_del++;
		 /* loop_mov_stop calculates moves needed */
		 loop_mov_stop = add_count-(k+1);
		 while (loop_mov_stop != loop_mov)
		 {     room[k+loop_mov] = room[(k+1)+loop_mov];
		       phone[k+loop_mov] = phone[(k+1)+loop_mov];
		       loop_mov++; /* counter for move */
		 }
	      }
	   }
       }
    }
    /* Calcalates total entry */
    add_count = add_count - del_entry;

    if (del_found_flag == 0) /* flag is 0 when delete entry input was found */
    { return(0);  } /* return sucessful */
    else
    { return(-1); } /* return not found */
}
/*---------------------------------------------------------------------------
	FindPhone function
	------------------
	Used to search for a phone number in the database.

	Returns 0 if phone no. was found.
	Returns -1 if phone no. is not found.
---------------------------------------------------------------------------*/
int FindPhone(long int p)
{
int k, phone_found_flag= -1;
    gotoxy(1,8);
    for(k=0; k < add_count; k++)
    {  if (add_count != 0) /* if database is not empty then run a search */
       {  if (k != 0 && (k%15) == 0)
	  {  gotoxy(1,8); /* moves cursor to beginning when screen filled */
	     getch();
	  }
	  if (p == phone[k])
	  {  printf("Phone No. [%-8ld] was found in record No. [%3d  ]\tRoom No. [%-4d]\n",phone[k],k+1,room[k]);
	     phone_found++;
	     phone_found_flag = 0;
	  }
       }
    }
    if (phone_found_flag == 0) /* flag is 0 if record was found */
    { return(0);  } /* return sucessful */
    else
    { return(-1); } /* return not found */
}
/*---------------------------------------------------------------------------
	FindRoom function
	------------------
	Used to search for a Room number in the database.

	Returns 0 if room no. was found.
	Returns -1 if room no. is not found.
----------------------------------------------------------------------------*/
int FindRoom(int r)
{
int k, room_found_flag=-1;

    gotoxy(1,8);
    for(k=0; k < add_count; k++)
    {  if (add_count != 0) /* if database is not empty then run a search */
       {  if (k != 0 && (k%15) == 0)
	  {  gotoxy(1,8); /* moves cursor to beginning when screen filled */
	     getch();
	  }
	  if (r == room[k])
	  {  printf("Room No. (%-4d) was found in record No. [%3d  ]\tPhone No. (%-8ld)\n",room[k],k+1,phone[k]);
	     room_found++;
	     room_found_flag = 0;
	  }
       }
    }

    if (room_found_flag == 0)
    { return(0);  } /* return sucessful */
    else
    { return(-1); } /* return not found */
}

/*---------------------------------------------------------------------------
	ListAll Function
	----------------
	Used for displaying data entered into the database.
	returns -1 if list is empty.
	returns 0 if sucessful (database contains valid entries)
---------------------------------------------------------------------------*/
int ListAll(void)
{
int k;
    gotoxy(1,6);
    for (k=0; k < add_count; k++)
    {
	if (k != 0 && (k%17) == 0)
	{  gotoxy(1,6); /* moves cursor to beginning when screen filled */
	   getch();
	}
	/* double checks - it will not print out delete entries(-1) */
	if (room[k] != -1 && phone[k] != -1)
	{  printf("Room Number [%3d  ]: %-4d\t",k+1,room[k]);
	   printf("Phone Number[%3d  ]: %-8ld\n",k+1,phone[k]);
	}
    }
    if (add_count == 0)
    {  return(-1); } /* Empty List */
    else
    {  return(0);  } /* Successful */
}
/*---------------------------------------------------------------------------
	GeTotalEntries function
	-----------------------
	Used to return total entries added to database.
---------------------------------------------------------------------------*/
int GeTotalEntries(void)
{
/* This function is not required but was created to make things clear. As
   the add_count(globe varible) already keeps track of total entries added.*/
   return(add_count);
}
/*---------------------------------------------------------------------------
	SortAllEntries function
	-----------------------
	Sort is done with the use of bubble sort.
	returns 0 if sort was successful.
	returns -1 if database is empty.
---------------------------------------------------------------------------*/

int SortAllEntries(char sel)
{
int k,room_str_tmp,sortalldone;
long int phone_str_tmp;
    if (add_count !=0) /* if list not empty continue with sort */
    {  do
       { sortalldone=0;
	 for (k = 0; k < add_count; k++)
	 {   /* sort in ascending order */
	     if ((phone[k] > phone[k + 1])&&(sel == 'a' || sel == 'A')&&(k != add_count -1))
	     {   phone_str_tmp = phone[k]; /* stores previous array to phone_str_tmp */
		 phone[k] = phone[k + 1]; /* copys next array to the previous array before it */
		 phone[k + 1] = phone_str_tmp; /* Previous array is copyed to next array */
		 /* same process is done here but with room no. */
		 room_str_tmp = room[k];
		 room[k] = room[k + 1];
		 room[k + 1] = room_str_tmp;
		 sortalldone =1; /* sets to 1 if sort is done */
	     }
	     /* same method used here but sorts in decending order */
	     if ((phone[k] < phone[k + 1])&&(sel == 'd' || sel == 'D')&&(k != add_count -1))
	     {   phone_str_tmp = phone[k];
		 phone[k] = phone[k + 1];
		 phone[k + 1] = phone_str_tmp;
		 room_str_tmp = room[k];
		 room[k] = room[k + 1];
		 room[k + 1] = room_str_tmp;
		 sortalldone =1;
	     }
	 }
       }while (sortalldone);
    }

    if ((sel == 'a' || sel == 'A')&&add_count !=0)
    {   gotoxy(1,25);
	printf("You have chosen to sort the database in [A]scending order. ");
	return(0);   /* sucessfully sorted */
    }
    else
    if ((sel == 'd' || sel == 'D')&&add_count !=0)
    {  gotoxy(1,25);
       printf("You have chosen to sort the database in [D]ecending order. ");
       return(0);    /* sucessfully sorted */
    }
    else
    if ((sel != 'a' || sel != 'A' || sel != 'd' || sel != 'D')&&add_count !=0)
    {  gotoxy(1,12);
       printf("Invalid option - database was not sorted!");
    }
    else
    { return(-1); } /* list empty */
}
/****************************************************************************
	Special functions
	-----------------
	Chkstrdig, LoadDB.
****************************************************************************/
/*---------------------------------------------------------------------------
	Chkstrdig(Check string for digits)
	----------------------------------
	Used to check string inputs, and lenght.
	returning:
	-1 = if string lenght is out of range.
	-2 = if a char is found within the string.
	0 = if successful

	NOTE:
	Addentry specifed that Room allows for 4 digits or less.
	Phone allowing 8 digits or less.
---------------------------------------------------------------------------*/
int chkstrdig (char str[], int range)
{
int lenght=0,k;
    lenght = strlen(str); /* get lenght of string */

    if (lenght > range)
    { 	return(-1);} /* out of range */
    if (lenght <= range) /* string is in range */
    {  for (k=0; k < lenght; k++)
       {   if (isdigit(str[k]) == 0)
	   { return(-2); } /* char detected return error msg */
       }
    return(0); /* successful - input string was in range and was digits */
    }
}
/*---------------------------------------------------------------------------
	Load up database from file function
	-----------------------------------
	Used to load a file into the database.
---------------------------------------------------------------------------*/
void LoadDB(void)
{
int count,nofilen,dbfilecount=0;
char finphone[80];
char finroom[80];
int error_junk;

long int l_finphone;
int i_finroom;

FILE *f1;
     gotoxy(1,6);
     printf("Enter the filename of the database: ");
     gotoxy(1,7);
     printf("Example: c:\\mydbfile.txt");
     gotoxy(37,6);
     gets(dbload);
     flushall(); /* clears all buffers */
     f1 = fopen (dbload,"r"); /* open file for reading */
     if (f1==NULL)
     {  gotoxy(1,25);
	fprintf(stderr,"There was an error reading your database file!");
	strcpy(dbload, "No database file loaded (RAM MODE!).");
	getch();
	exit;
     }
     else
     {	for (count=0; count < MAXDB; count++)
	{   if (!feof(f1)) /* if not end of file continue to input data */
	    { 	fscanf(f1,"%s\t%s\n",&finroom,&finphone);
		/* saves info to room, phone array */
	    error_junk = chkstrdig(finroom,4);
	    error_junk = chkstrdig(finphone,8);
         if (error_junk == -1 || error_junk == -2)
         {  printf("Sorry that was an invalid database\n");
            printf("Now working in RAM MODE!");
            getch();
            strcpy(dbload, "No database file loaded (RAM MODE!).");
	    break;
	 }
	     i_finroom = atoi(finroom); /* converts string to int */
	     l_finphone = atol(finphone); /* converts string to long int */
		room[count] = i_finroom;
		phone[count] = l_finphone;
		dbfilecount++; /* counts no. of records stored in file */
	    }
	}
       if (error_junk ==0)
       {
	  gotoxy(1,25);
	  printf("Database %s, was successfully loaded!",dbload);
	  getch();
	  /* copys no. of records in file into master counter*/
	  add_count = dbfilecount;
       }
     }
     fclose(f1);
}
/****************************************************************************
	MAIN function
	-------------
	Menu, ExitMenu, drawscreen and refreshscreen.
****************************************************************************/
/*---------------------------------------------------------------------------
	Menu function
	-------------
	Display valid options on the screen
---------------------------------------------------------------------------*/
char menu(void)
{
char optrtn;
     clrscr();
     window(1,1,80,25); /*Set position and screen mode*/
     refreshscreen();
     drawscreen();
     gotoxy(1,4);
     printf("[1] - Add entry\n");
     printf("[2] - Delete entry\n");
     printf("[3] - Find room number\n");
     printf("[4] - Find phone number\n");
     printf("[5] - List all entries\n");
     printf("[6] - Display total entries in database\n");
     printf("[7] - Sort entries\n");
     printf("[8] - Load database from file\n");
     printf("[9] - Exit");
     gotoxy(1,25);
     cprintf("Please select an option between 1 and 9.");
     gotoxy(1,15);
     printf("Database loaded: %s",dbload);
     gotoxy(1,14);
     printf("Select an option: ");
     optrtn = getch();
     return optrtn;
}
/*---------------------------------------------------------------------------
	ExitMenu function
	-----------------
	While exiting to system, askes user if he/she wants to save
	database into a file.
---------------------------------------------------------------------------*/
void exitmenu(void)
{    char filename[20],save_opt;
     int k;
     FILE *f1;
     gotoxy(1,6);
     printf("Do You want to Save database before exiting? ");
     gotoxy(1,25);
     cprintf("Press 'Y' to confirm, anykey to cancel.");
     save_opt = getch();
     flushall();
     if (save_opt == 'y' || save_opt == 'Y')
     {  gotoxy(1,8);
	printf("Please Enter the path and filename to save to:");
	gotoxy(1,10);
	printf("Example: c:\\mydbfile.txt");
	gotoxy(48,8); /* move cursor back to line 8 */
	gets(filename);
	flushall();
	f1 = fopen (filename,"a"); /*open file for appending mode */
	if (f1== NULL)
	{  gotoxy(1,12);
	   fprintf(stderr, "Error opening file %s.",filename);
	   gotoxy(1,25);
	   cprintf("Database was not saved!                                                   ");
	   getch();
	   exit;
	}
	else
	{   for (k=0; k < add_count; k++)
	    {   fprintf(f1, "%d\t%ld\n",room[k],phone[k]);}
		fclose(f1);
		gotoxy(1,25);
		cprintf("Database was successfully saved in %s",filename);
		getch();
	    }
     }
     else
     {   gotoxy(1,25);
	 cprintf("Database was not saved!                                                   ");
	 getch();
     }
     clrscr();
     gotoxy(23,10);
     printf("Thank you for using this program");
     gotoxy(23,11);
     printf("Coded by: Robert Chanphakeo!");
     gotoxy(23,13);
     printf("Email: cdk_champs@hotmail.com");
     getch();
     exit(0);
}
/*---------------------------------------------------------------------------
	Drawscreen function
	-------------------
	Draws program header.
---------------------------------------------------------------------------*/
void drawscreen(void)
{
     gotoxy(1,1);
     cprintf("-------------------------------------------------------------------------------");
     gotoxy(1,2);
     cprintf("                            *** PHONE SEVICE v2.0 ***                          ");
     gotoxy(1,3);
     cprintf("-------------------------------------------------------------------------------");
}
/*---------------------------------------------------------------------------
	Refreshscreen function
	----------------------
	used to refresh colour display.
----------------------------------------------------------------------------*/
void refreshscreen(void)
{    clrscr();
     textcolor(WHITE);
     textbackground(BLACK);
     gotoxy(1,25);
     cprintf("                                                                               ");
     clrscr();
     textcolor(WHITE);
     textbackground(BLUE);
     gotoxy(1,25);
     cprintf("                                                                               ");
}

/* EOF */
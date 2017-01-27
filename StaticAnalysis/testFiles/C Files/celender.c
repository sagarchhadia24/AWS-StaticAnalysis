#include <iostream>
#include <cstdlib>
#include <iomanip>
 
using namespace std;
 
bool isLeapYear (int); //good check for leap years
int firstDayofnewyearMonth (int );
int numOfDaysInMonth (int, bool); // takes the number of the month, a flag saying whether year is leap
void printHeader (int); // takes the number of the month, and the first day, prints, and updates
void printMonth (int, int&); // the first day of the next month
void skip (int);// prints the specified amount of spaces
void skipToDay (int);// prints leading spaces in monthly calendar
void disaster (); // terminates program in case of unrecoverable errors 
 
int main () 
{
    system ("color f1 ");
    int year, firstDayInCurrentMonth;
    int currentMonth = 1;
    int numDays;
    bool leap;
    cout << "What year do you want a calendar for? ";
    cin >>year;
    cout<<endl;
    firstDayInCurrentMonth=firstDayofnewyearMonth(year);
    leap = isLeapYear(year);
    skip(9); 
    cout << year << endl;
    while (currentMonth <= 12)
    {
    numDays = numOfDaysInMonth(currentMonth, leap);
    printHeader(currentMonth);
    printMonth(numDays, firstDayInCurrentMonth);
    cout << endl << endl << endl;
    currentMonth = currentMonth + 1;
    }
    cout << endl;
}
bool isLeapYear (int year) 
{  
    return   ((year%4==0) && (year%100 !=0))||(year%400==0)  ;       
}
int  firstDayofnewyearMonth(int year) 
{ 
    int day_start; 
    int x1, x2, x3; 
    x1 = (year - 1)/ 4;
    x2 = (year - 1)/ 100; 
    x3 = (year - 1)/ 400; 
    day_start = (year + x1 - x2 + x3) %7; 
    return day_start; 
} 
int numOfDaysInMonth (int m, bool leap)
{
    if (m == 1) return(31);
    else if (m == 2) if (leap) return(29);else return(28);  
    else if (m == 3) return(31);
    else if (m == 4) return(30);
    else if (m == 5) return(31);
    else if (m == 6) return(30);
    else if (m == 7) return(31);
    else if (m == 8) return(31);
    else if (m == 9) return(30);
    else if (m == 10) return(31);
    else if (m == 11) return(30);
    else if (m == 12) return(31);
    else disaster();
}
void printHeader (int m) 
{
    if (m == 1)
    { 
    skip(7);
    cout << "January" << endl;
    }
    else if (m == 2) { skip(7); cout << "February" << endl; } 
    else if (m == 3) { skip(7); cout << "March" << endl; }
    else if (m == 4) { skip(7); cout << "April" << endl; }
    else if (m == 5) { skip(7); cout << "May" << endl; }
    else if (m == 6) { skip(7); cout << "June" << endl; }
    else if (m == 7) { skip(7); cout << "July" << endl; }
    else if (m == 8) { skip(7); cout << "August" << endl; }
    else if (m == 9) { skip(7); cout << "September" << endl; }
    else if (m == 10) { skip(7); cout << "October" << endl; }
    else if (m == 11) { skip(7); cout << "November" << endl; }
    else if (m == 12) { skip(7); cout << "December" << endl; }
    else disaster();
 
    cout << " S  M  T  W  T  F  S" <<endl;
    cout << "____________________" << endl;
}
void skip (int i) 
{
    while (i > 0)
    {
    cout << " ";
    i = i - 1;
    }
}
void printMonth (int numDays, int &weekDay) 
{
    int day = 1;
    skipToDay(weekDay);
    while (day <= numDays) 
    {
    cout << setw(2) << day << " ";
    if (weekDay == 6)
    {
        cout << endl;
        weekDay = 0;
    }
    else weekDay = weekDay + 1;
    day = day + 1; 
    }
}
void skipToDay (int d)
{
    return skip(3*d);
}
void disaster () 
{
    cout << "Disaster! Exiting ..." << endl;
    exit ( -1);
}

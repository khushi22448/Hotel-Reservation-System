#include<iostream>
#include<fstream>
using namespace std;

int main()
{
    int choice=0;
     
    while(1)
    {
        cout<<"\n===Hotel Reservation System===\n";
        cout<<"1.Add Room\n";
        cout<<"2.Search Available Rooms\n";
        cout<<"3.Book a Room\n";
        cout<<"4.Cancel a Booking\n";
        cout<<"5.Occupancy Report\n";
        cout<<"6.Revenue Report\n";
        cout<<"7.Exit\n";
        cout<<"Enter your choice:";
        cin>>choice;
        
    }
    return 0;
}

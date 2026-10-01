#include<iostream>
#include<fstream>
#include<string>
using namespace std;

const int MAX_ROOOMS=50;
const int MAX_BOOKINGS=100;

class BookingDate
{
    protected:
        int day,month,year;

    public:
        BookingDate()
        {
            day=0;
            month=0;
            year=0;
        }

        BookingDate(int d,int m,int y)
        {
            day=d;
            month=m;
            year=y;
        }

        bool operator<(const BookingDate& other) const
        {
            if(year<other.year)
            {
                return true;
            }

            if(year==other.year && month<other.month)
            {
                return true;
            }

            if(year==other.year && month==other.month && day<other.day)
            {
                return true;
            }

            return false;
        }

        bool operator==(const BookingDate& other) const
        {
            return (day==other.day &&
                    month==other.month &&
                    year==other.year);
        }

        int operator-(const BookingDate& other) const
        {
            int days1=day+month*30+year*365;
            int days2=other.day+other.month*30+other.year*365;

            return days1-days2;
        }

        void display() const
        {
            cout<<day<<"/"<<month<<"/"<<year;
        }

        void saveToFile(ofstream& file) const
        {
            file<<day<<"/"<<month<<"/"<<year;
        }
};


class Booking
{
       protected:
        int roomNumber;
        string guestName;
        BookingDate checkIn,checkOut;

        public:
        Booking()
        {
            roomNumber=0;
            guestName=" ";
        }

        Booking(int room,string guest,BookingDate In,BookingDate Out)
        {
            roomNumber=room;
            guestName=guest;
            checkIn=In;
            checkOut=Out;
        }

        void input()
        {
            int d,m,y;

            cout<<"Enter room number:"<<endl;
            cin>>roomNumber;

            cin.ignore();

            cout<<"Enter guest name:"<<endl;
            getline(cin,guestName);

            cout<<"Enter check in date:"<<endl;
            cin>>d>>m>>y;
            checkIn=BookingDate(d,m,y);

            cout<<"Enter check out date:"<<endl;
            cin>>d>>m>>y;
            checkOut=BookingDate(d,m,y);    
        }
        void display()
        {
            cout<<"Room Number : "<<roomNumber<<endl;
            cout<<"Guest Name : "<<guestName<<endl;

            cout<<"Check-In : ";
            checkIn.display();
            cout<<endl;

            cout<<"Check-Out : ";
            checkOut.display();
            cout<<endl;
        }
        void savetofile()
        {
              ofstream file("booking.txt",ios::app);

               if(!file)
              {
                cout<<"Error! file not opened"<<endl;
                return;
              }

            file<<roomNumber<<endl;
            file<<guestName<<endl;

            file<<"Check-In: ";
            checkIn.saveToFile(file);
            file<<endl;

            file<<"Check-Out: ";
            checkOut.saveToFile(file);
            file<<endl;

            file.close();

            cout<<"Booking saved successfully!"<<endl;
         }
};


int main()
{
    int choice=0;
    Booking b;

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

        switch(choice)
        {
            case 3:
            {
                b.input();
                b.display();
                b.savetofile();
                break;
            }

            case 7:
            {
                return 0;
            }
        }
    }

    return 0;
}
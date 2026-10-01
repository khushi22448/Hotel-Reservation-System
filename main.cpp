#include<iostream>
#include<fstream>
using namespace std;

class Room{
    private:
        int room_no;
        string type;
        int price;
        bool isAvailable;
    public:
        void inputRoom()
            {
                cout<<"Enter Room Number: ";
                cin>>room_no;
                cout<<"Enter Room Type (Single/Double/Suite): ";
                cin>>type;
                cin.ignore();
                cout<<"Enter Price per Night: ";
                cin>>price;
                isAvailable=true;
            }
        void displayRoom()
        {
            cout<<"Room_no: "<<room_no<<endl;
            cout<<"Type: "<<type<<endl;
            cout<<"Price: "<<price<<endl;
            cout<<(isAvailable ?"Available":"Booked")<<endl;
        } 
        void savetofile()
        {
            ofstream file("rooms.txt",ios::app);
            if(!file)
            {
                cout<<"ERROR! File not opened!";
            }
            file<<room_no<<" "<<type<<" "<<price<<" "<<isAvailable<<endl;
            file.close();
        }     
};

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

        switch(choice)
        {
            case 1:
            Room obj1;
            obj1.inputRoom();
            obj1.savetofile();
            obj1.displayRoom();
            break;
        }
        
    }
    return 0;
}

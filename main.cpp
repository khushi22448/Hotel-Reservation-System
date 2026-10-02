#include<iostream>
#include<fstream>
#include<string>

using namespace std;

namespace uten{
	const string UserFile = "user.txt";
	
	void clear(){
		#ifdef _WIN32
		  system("cls");
		#else
		  system("clear");
		#endif
	}
	
    int getNextId() {
        ifstream in(UserFile);
        int maxId = 0;
        int id;
        string username, password, email, phone;

        while(in>>id>>username>>password>>email>>phone) {
            if(id>maxId){
              maxId = id;
            }
        }
        return maxId + 1;
    }
    
    bool usernameTaken(string name) {
        ifstream in(UserFile);
        int id;
        string username, password, email, phone;

        while(in>>id>>username>>password>>email>>phone){
            if(username==name){
			 return true;
            }
        }
        return false;
    }
    
    bool validEmail(string e) {
        size_t at = e.find('@');
        if(at==string::npos){
          return false;
		}
        if(at==0){	
		  return false;
		}	
        if(e.find('@',at + 1)!= string::npos){
		    return false;
		}

        size_t dot = e.rfind('.');
        
        if(dot==string::npos){
		  return false;
		}	
        if(dot< at + 2){
		  return false;
		}	
        if ((e.size()-dot-1) < 2){
		  return false;
		}
		
        return true;
    }
    
    bool validPhone(string p){
        if(p.size()!=10){
		  return false;
		}
        if(p[0]!='9' || p[1]!='8'){
		  return false;
		}
        for(size_t i=0;i<p.size();i++){
            if(p[i] < '0' || p[i] > '9'){
			  return false;
			}
        }
        return true;
    }
}

using namespace uten;

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
class RoomUnavailableException
{
    private:
        string message;
    public:
        RoomUnavailableException(string msg)
        {
            message=msg;
        }
        string getMessage() const
        {
            return message;
        }
};

class Booking
{
       protected:
        int roomNumber;
        string guestName;
        BookingDate checkIn,checkOut;
        double amount;
        int nights;
        public:
        Booking()
        {
            roomNumber=0;
            guestName=" ";
            amount=0;
            nights=0;
        }

        Booking(int room,string guest,BookingDate In,BookingDate Out,double amt)
        {
            roomNumber=room;
            guestName=guest;
            checkIn=In;
            checkOut=Out;
            amount=amt;
            nights=checkOut-checkIn;
        }
          int getRoom() const
         {
            return roomNumber;
         }
         double getAmount() const
         {
            return amount;
         }

        bool input()
        {
            int d,m,y;

            cout<<"Enter room number:"<<endl;
            cin>>roomNumber;

            cin.ignore();

            cout<<"Enter guest name:"<<endl;
            getline(cin,guestName);

            cout<<"Enter check in date: ( DD MM YYYY)"<<endl;
            cin>>d>>m>>y;
            if(d<1||d>31||m<1||m>12||y<2026)
            {
                cout<<"Invalid date!Try again"<<endl;
                return false;
            }
            checkIn=BookingDate(d,m,y);

            cout<<"Enter check out date: (DD MM YYYY)"<<endl;
            cin>>d>>m>>y;
            if(d<1||d>31||m<1||m>12||y<2026)
            {
                cout<<"Invalid date!Try again"<<endl;
                return false;
            }
            checkOut=BookingDate(d,m,y); 
            
            if(checkOut<checkIn ||checkOut==checkIn)
            {
                throw RoomUnavailableException("Invalid booking! Check-out date should come after check-in date");
            }
            nights=checkOut-checkIn;
            ifstream file("rooms.txt");
            if(!file)
            {
                cout<<"Error!File not opened"<<endl;
            }
            int no,price;
            string type;
            bool a;
            bool found=false;
            while(file>>no>>type>>price>>a)
            {
                if(no==roomNumber)
                {
                    amount=price*nights;
                    found=true;
                }
            }
            file.close();
            if(!found)
            {
                throw RoomUnavailableException("Room not found!");
            }
            cout<<"Number of nights:"<<nights<<endl;
            cout<<"Amount:"<<amount<<endl;
            return true;
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
            cout<<"Number of nights : "<<nights<<endl;
            cout<<"Amount : "<<amount<<endl;
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
            file<<"Number of nights:"<<nights<<endl;
            file<<"Amount:"<<amount<<endl;
            file.close();
            cout<<"Booking saved successfully!"<<endl;
         }
};
class Cancel_Booking{
    private:
        int room_no;
        char confirm;
        string guest_name;
        string gName;
        string checkIn;
        string checkout;
        string separator;
    public:
        void input()
        {
            cout<<"Enter Room_no: ";
            cin>>room_no;
            cout<<"Enter Guest Name: ";
            cin>>guest_name;
        }
        void cancel()
        {
            ifstream file("booking.txt");
            ofstream temp("temp.txt");
            bool found=false;
            string line;
            while(getline(file,line))
            {
                if(line==to_string(room_no))
                {
                    getline(file,gName);
                    getline(file,checkIn);
                    getline(file,checkout);
                    getline(file,separator);
                
                if(gName==guest_name)
                {
                    cout<<"\nBooking Found:\n";
                    cout<<"Room Number: "<<room_no<<endl;
                    cout<<"Guest Name: "<<gName<<endl;
                    cout<<checkIn<<endl;
                    cout<<checkout<<endl;
                    cout<<"\nAre you sure you want to cancel the booking? (y/n): ";
                    cin>>confirm;
                    if(confirm=='y'||confirm=='Y')
                    {
                        cout<<"Booking canceled sucessfully!\n";
                        found=true;
                        continue;
                    }
                    else
                    {
                         temp<<room_no<<endl<<gName<<endl<<checkIn<<endl<<checkout<<endl<<separator<<endl;
                    }
                }
                else 
                {
                    temp<<room_no<<endl<<gName<<endl<<checkIn<<endl<<checkout<<endl<<separator<<endl;
                }
            }
                else 
                {
                temp<<line<<endl; 
            }
        }

        file.close();
        temp.close();

        remove("booking.txt");
        rename("temp.txt","booking.txt");
        if(!found)
        {
            cout<<"Booking not found!";
        }
        }
};  
class SearchRoom{
    public:
    void searchbyroom_no(int searchNo)
    {
        ifstream file("rooms.txt");
        if(!file)
        {
            cout<<"ERROR!File not opened!"<<endl;
            return;
        }
        int room_no,price;
        string type;
        bool isAvailable;
        bool found = false;
        while (file >> room_no >> type >> price >> isAvailable) 
        {
            if (room_no == searchNo) {
                cout << "\nRoom Found:\n";
                cout << "Room_no: " << room_no << endl;
                cout << "Type: " << type << endl;
                cout << "Price: " << price << endl;
                cout << (isAvailable ? "Available" : "Booked") << endl;
                found = true;
                break;
            }
        }
        file.close();
        if(!found)
        {
            cout<<"File not found!"<<endl;
        }
    }
    
    void searchByName(string guestName)
    {
        ifstream file("booking.txt");
        if (!file)
        {
            cout << "ERROR! File not opened." << endl;
            return;
        }

        string roomNo, gName, checkIn, checkOut;
        bool found = false;

        while (getline(file, roomNo))
        {
            if (!getline(file, gName)) break;
            if (!getline(file, checkIn)) break;
            if (!getline(file, checkOut)) break;

            if (gName == guestName)
            {
                cout << "\nBooking Found:\n";
                cout << "Room Number: " << roomNo << endl;
                cout << "Guest Name: " << gName << endl;
                cout << checkIn << endl;
                cout << checkOut << endl;
                found = true;
                break;
            }
        }

        file.close();

        if (!found)
        {
            cout << "No booking found for guest " << guestName << endl;
        }
    }

};

namespace use{
	class User{
		protected:
			int user_id;
			string username,password,email,phone;
		    bool status=false;
		public:
			User(){}
			User(string username,string password,string email,string phone,int id){
				this->username=username;
				this->password=password;
				this->email=email;
				this->phone=phone;
				this->user_id=id;
			}
			friend void user_regis();
			friend void user_login();
	};
    void user_regis(){
    	clear();
    	string user,pass,re_pass,email,phone;
    	int id;
    	id=getNextId();
    	
    	cout<<"Enter 'Username(No Spaces)': ";
    	cin>>user;
    	
        while(usernameTaken(user)) {
            cout<<endl<<"That username is already taken!!"<<endl;
            cout<<endl<<"Re-enter 'Username': ";
            cin>>user;
        }
    	
    	do{   		
    	cout<<endl<<"Enter 'Password': ";
    	cin>>pass;
    	
    	cout<<endl<<"Re-enter 'Password': ";
    	cin>>re_pass;
    	
    	if(pass!=re_pass){
    		cout<<endl<<"Password does not match!!!"<<endl;
		}
		}while(pass!=re_pass);
    	
    	cout<<endl<<"Enter 'E-mail': ";
    	cin>>email;
    	while(!validEmail(email)){
    		cout<<endl<<"Wrong format of email!!"<<endl;
    		cout<<"enter 'E-mail': ";
    		cin>>email;
		}
    	
    	cout<<endl<<"Enter 'Phone Number': ";
    	cin>>phone;
    	while(!validPhone(phone)){
     		cout<<endl<<"Wrong format of email!!"<<endl;
    		cout<<"enter 'Phone Number': ";
    		cin>>phone;	
		}
    	
    	User o(user,pass,email,phone,id);
    	
    	fstream obj;
    	obj.open(UserFile,ios::app);
    	if(!obj){
    		cout<<endl<<"No directory!!";
    		return;
		}
		obj<<o.user_id<< " " <<o.username <<" " << o.password <<" " <<o.email <<" " <<o.phone <<"\n";
		obj.close();
		cout<<endl<<"Your Account is registered!!";
	}
	void user_login(){
		clear();
		string name,pass;
		int choice=0;
        cout<<"\n===Login===\n";
        cout<<"Username: ";
        cin>>name;
        cout<<"Password: ";
        cin>>pass;
    }
}; 
int main()
{
    int choice=0;
    int option=0;
        ifstream in(UserFile);
        if(!in){
            cout<<"No users registered yet!!";
            return;
        }

        User u;

        cout<<"Enter your choice:";
        cin>>choice;

        switch(choice)
        {
            case 1:
            {
                Room obj1;
                obj1.inputRoom();
                obj1.savetofile();
                obj1.displayRoom();
                break;
            }
            case 2:
            {

                cout<<"MENU"<<endl;
                cout<<"1.Search by Room Number."<<endl;
                cout<<"2.Search by Guest name."<<endl;
                cout<<"Enter your choice: ";
                cin>>option;
                SearchRoom obj;
                if(option==1)
                {
                    int number=0;
                    cout<<"Enter room number: ";
                    cin>>number;
                    obj.searchbyroom_no(number);
                    break;
                }
                else
                {
                    string name;
                    cout<<"Enter Guest Name: ";
                    cin.ignore();
                    getline(cin,name);
                    obj.searchByName(name);
                    break;
                }
            }
            case 3:
            {
                Booking b;
                if(b.input())
                {
                 b.display();
                 b.savetofile();
                }
        while(in>>u.user_id>>u.username>>u.password>>u.email>>u.phone){
            if(u.username == name && u.password == pass){
                u.status=true;
                clear();
                cout<<"Login successful.Welcome,"<<u.username<<"!\n";
                break;
            }
        }
    }
        if(u.status==true){
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
                 {
                   Room obj1;
                   obj1.inputRoom();
                   obj1.savetofile();
                   obj1.displayRoom();
                   break;
                 }
                 case 3:
                {
                    Booking b;

                      try
                     {
                        if(b.input())
                        {
                           b.display();
                           b.savetofile();
                           cout << "\nPress Enter to continue...";
                           cin.ignore();
                           cin.get();
                        }
                     }
                      catch(RoomUnavailableException& e)
                     {
                         cout << "Booking failed: " << e.getMessage() << endl;
                         cout << "\nPress Enter to continue...";
                         cin.ignore();
                         cin.get();
                    }
                    break;
                }
                 case 4:
                 {
                  Cancel_Booking obj;
                  obj.input();
                  obj.cancel();
                  break;
                 }
                 case 7:
                 {
                  exit(0);
                  break;
                 }
                }
            }	
		else{
        cout<<"Wrong username or password!!";
		in.close();
		}
	}	
}

using namespace use;

void login_menu(){
    	
    int choice=0;
    while(1)
    {
    	clear();
        cout<<"\n===Login===\n";
        cout<<"1.login\n";
        cout<<"2.Register\n";
        cout<<"3.Exit\n";
        cout<<"Enter your choice:";
        cin>>choice;
        switch(choice){
        	case 1:{
        		clear();
        		user_login();
				break;
			}
			case 2:{
				clear();
			    user_regis();
				break;
			}
			case 3:{
				exit(0);
				break;
			}
			default:{
				cout << "Wrong input!!";
				break;
			}
		}
    }	
    
}
int main()
{
    login_menu();
    return 0;
}
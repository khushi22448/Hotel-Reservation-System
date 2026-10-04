
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<limits>
#include<cstdio>
#include<cstdlib>

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

    void waitKey(){
        #ifdef _WIN32
          system("pause");
        #else
          cout<<"\nPress Enter to continue...";
          cin.ignore();
          cin.get();
        #endif
    }

    void clearBadInput(){
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
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

    bool roomExists(int roomNo){
        ifstream in("rooms.txt");
        int r, price, avail;
        string type;
        while(in>>r>>type>>price>>avail){
            if(r==roomNo){
                return true;
            }
        }
        return false;
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
        bool inputRoom()
        {
            cout<<"Enter Room Number: ";
            cin>>room_no;
            if(!cin){
                clearBadInput();
                cout<<"Invalid room number!"<<endl;
                return false;
            }
            if(roomExists(room_no)){
                cout<<"Room "<<room_no<<" already exists!"<<endl;
                return false;
            }
            cout<<"Enter Room Type (Single/Double/Suite): ";
            cin>>type;
            cout<<"Enter Price per Night: ";
            cin>>price;
            if(!cin || price<=0){
                clearBadInput();
                cout<<"Invalid price!"<<endl;
                return false;
            }
            isAvailable=true;
            return true;
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
                return;
            }
            file<<room_no<<" "<<type<<" "<<price<<" "<<isAvailable<<endl;
            file.close();
            cout<<"Room added successfully!"<<endl;
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

        static bool valid(int d,int m,int y,int minYear=1900)
        {
            if(y<minYear || y>9999 || m<1 || m>12 || d<1)
            {
                return false;
            }
            int dim[]={31,28,31,30,31,30,31,31,30,31,30,31};
            bool leap=(y%4==0 && y%100!=0) || (y%400==0);
            int maxDay=dim[m-1];
            if(m==2 && leap)
            {
                maxDay=29;
            }
            return d<=maxDay;
        }

        long long toDays() const
        {
            long long yy=year;
            if(month<=2)
            {
                yy--;
            }
            long long era=yy/400;
            long long yoe=yy-era*400;
            long long mp=(month>2)?(month-3):(month+9);
            long long doy=(153*mp+2)/5+day-1;
            long long doe=yoe*365+yoe/4-yoe/100+doy;
            return era*146097+doe;
        }

        bool operator<(const BookingDate& other) const
        {
            if(year!=other.year)
            {
                return year<other.year;
            }
            if(month!=other.month)
            {
                return month<other.month;
            }
            return day<other.day;
        }

        bool operator==(const BookingDate& other) const
        {
            return (day==other.day &&
                    month==other.month &&
                    year==other.year);
        }

        int operator-(const BookingDate& other) const
        {
            return (int)(toDays()-other.toDays());
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

bool readBooking(ifstream &f,int &room,string &guest,BookingDate &ci,BookingDate &co)
{
    string line;
    while(getline(f,line))
    {
        if(line.empty() || line.find_first_not_of("0123456789")!=string::npos)
        {
            continue;
        }
        room=stoi(line);
        string in,out;
        getline(f,guest);
        getline(f,in);
        getline(f,out);

        int d1,m1,y1,d2,m2,y2;
        if(sscanf(in.c_str(),"Check-In: %d/%d/%d",&d1,&m1,&y1)!=3)
        {
            continue;
        }
        if(sscanf(out.c_str(),"Check-Out: %d/%d/%d",&d2,&m2,&y2)!=3)
        {
            continue;
        }
        ci=BookingDate(d1,m1,y1);
        co=BookingDate(d2,m2,y2);
        return true;
    }
    return false;
}

void writeBooking(ofstream &f,int room,const string &guest,const BookingDate &ci,const BookingDate &co)
{
    f<<room<<endl;
    f<<guest<<endl;
    f<<"Check-In: ";
    ci.saveToFile(f);
    f<<endl;
    f<<"Check-Out: ";
    co.saveToFile(f);
    f<<endl;
    f<<"-----"<<endl;
}

bool roomFree(int roomNo,BookingDate ci,BookingDate co)
{
    ifstream f("booking.txt");
    int r;
    string g;
    BookingDate bi,bo;

    while(readBooking(f,r,g,bi,bo))
    {
        if(r==roomNo && ci<bo && bi<co)
        {
            return false;
        }
    }
    return true;
}

bool inputDate(BookingDate &dt,int minYear=1900)
{
    int d,m,y;
    cin>>d>>m>>y;
    if(!cin)
    {
        clearBadInput();
        cout<<"Invalid date format!"<<endl;
        return false;
    }
    if(!BookingDate::valid(d,m,y,minYear))
    {
        cout<<"Invalid date! Try again"<<endl;
        return false;
    }
    dt=BookingDate(d,m,y);
    return true;
}

class Booking
{
    protected:
        int roomNumber;
        string guestName;
        BookingDate checkIn,checkOut;
    public:
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
            cout<<"Enter room number: ";
            cin>>roomNumber;
            if(!cin)
            {
                clearBadInput();
                cout<<"Invalid room number!"<<endl;
                return false;
            }
            if(!roomExists(roomNumber))
            {
                cout<<"Room "<<roomNumber<<" does not exist!"<<endl;
                return false;
            }

            cin.ignore(numeric_limits<streamsize>::max(),'\n');

            cout<<"Enter guest name: ";
            getline(cin,guestName);
            if(guestName.empty())
            {
                cout<<"Guest name cannot be empty!"<<endl;
                return false;
            }

            cout<<"Enter check in date (DD MM YYYY): ";
            if(!inputDate(checkIn,2026))
            {
                return false;
            }

            cout<<"Enter check out date (DD MM YYYY): ";
            if(!inputDate(checkOut,2026))
            {
                return false;
            }

            if(!(checkIn<checkOut))
            {
                throw RoomUnavailableException("Invalid booking! Check-out date should come after check-in date");
            }

            if(!roomFree(roomNumber,checkIn,checkOut))
            {
                cout<<"Room is already booked for these dates!"<<endl;
                return false;
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
                    found=true;
                    if(a==false)
                  {
                     throw RoomUnavailableException("Room is already booked!");
                  }    
                   amount=price*nights;
                   break;
                    
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

            writeBooking(file,roomNumber,guestName,checkIn,checkOut);
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
    public:
        void input()
        {
            cout<<"Enter Room_no: ";
            cin>>room_no;
            if(!cin)
            {
                clearBadInput();
                room_no=-1;
            }
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            cout<<"Enter Guest Name: ";
            getline(cin,guest_name);
        }

        void cancel()
        {
            ifstream file("booking.txt");
            if(!file)
            {
                cout<<"\nNo bookings found!"<<endl;
                return;
            }

            vector<int> rn;
            vector<string> gn;
            vector<BookingDate> ins,outs;

            int r;
            string g;
            BookingDate ci,co;

            while(readBooking(file,r,g,ci,co))
            {
                rn.push_back(r);
                gn.push_back(g);
                ins.push_back(ci);
                outs.push_back(co);
            }
            file.close();

            int idx=-1;
            for(size_t i=0;i<rn.size();i++)
            {
                if(rn[i]==room_no && gn[i]==guest_name)
                {
                    idx=(int)i;
                    break;
                }
            }

            if(idx<0)
            {
                cout<<"\nBooking not found!"<<endl;
                return;
            }

            cout<<"\nBooking Found:\n";
            cout<<"Room Number: "<<rn[idx]<<endl;
            cout<<"Guest Name: "<<gn[idx]<<endl;
            cout<<"Check-In: ";
            ins[idx].display();
            cout<<endl;
            cout<<"Check-Out: ";
            outs[idx].display();
            cout<<endl;

            cout<<"\nAre you sure you want to cancel the booking? (y/n): ";
            cin>>confirm;

            if(confirm!='y' && confirm!='Y')
            {
                cout<<"Cancellation aborted."<<endl;
                return;
            }

            ofstream fo("booking.txt");
            for(size_t i=0;i<rn.size();i++)
            {
                if((int)i!=idx)
                {
                    writeBooking(fo,rn[i],gn[i],ins[i],outs[i]);
                }
            }
            fo.close();

            cout<<"Booking canceled successfully!"<<endl;
        }
};

class SearchRooms{
    public:
        void show()
        {
            BookingDate ci,co;

            cout<<"Enter check in date (DD MM YYYY): ";
            if(!inputDate(ci,2026))
            {
                return;
            }
            cout<<"Enter check out date (DD MM YYYY): ";
            if(!inputDate(co,2026))
            {
                return;
            }
            if(!(ci<co))
            {
                cout<<"Check-out date should come after check-in date"<<endl;
                return;
            }

            ifstream rooms("rooms.txt");
            if(!rooms)
            {
                cout<<"No rooms found!"<<endl;
                return;
            }

            int r,price,avail,found=0;
            string type;

            cout<<"\n===== AVAILABLE ROOMS =====\n";
            cout<<"Room\tType\tPrice\n";
            cout<<"-----------------------------\n";

            while(rooms>>r>>type>>price>>avail)
            {
                if(roomFree(r,ci,co))
                {
                    cout<<r<<"\t"<<type<<"\t"<<price<<endl;
                    found++;
                }
            }

            if(found==0)
            {
                cout<<"No rooms available for these dates."<<endl;
            }
        }
};

class OccupancyReport{
    private:
        bool isBooked(int roomNo,BookingDate a,string &guest){
            ifstream f("booking.txt");
            int b;
            string g;
            BookingDate ci,co;

            while(readBooking(f,b,g,ci,co)){
                if(b==roomNo && !(a<ci) && (a<co)){
                    guest=g;
                    return true;
                }
            }
            return false;
        }
    public:
        void show()
        {
            BookingDate target;
            cout<<"Enter date for report (DD MM YYYY): ";
            if(!inputDate(target)){
                return;
            }

            ifstream rooms("rooms.txt");
            if(!rooms){
                cout<<"No rooms found. Add rooms first!!"<<endl;
                return;
            }

            int roomNo, price, avail;
            string type;
            int total=0, occupied=0;

            cout<<"\n===== OCCUPANCY REPORT (";
            target.display();
            cout<<") =====\n";
            cout<<"Room\tType\tStatus\t\tGuest\n";
            cout<<"----------------------------------------------\n";

            while(rooms>>roomNo>>type>>price>>avail){
                total++;
                string guest="-";

                if(isBooked(roomNo,target,guest)){
                    occupied++;
                    cout<<roomNo<<"\t"<<type<<"\tOccupied\t"<<guest<<endl;
                }
                else{
                    cout<<roomNo<<"\t"<<type<<"\tVacant\t\t-"<<endl;
                }
            }
            rooms.close();

            cout<<"----------------------------------------------"<<endl;
            cout<<"Total Rooms    : "<<total<<endl;
            cout<<"Occupied Rooms : "<<occupied<<endl;
            cout<<"Vacant Rooms   : "<<total-occupied<<endl;

            if(total>0)
            {
                cout<<"Occupancy Rate : "<<(occupied*100.0/total)<<"%"<<endl;
            }
        }
};

class RevenueReport{
    private:
        int getPrice(int roomNo){
            ifstream rooms("rooms.txt");
            int r, price, avail;
            string type;

            while(rooms>>r>>type>>price>>avail){
                if(r==roomNo){
                    return price;
                }
            }
            return -1;
        }
    public:
        void show(){
            ifstream file("booking.txt");
            if(!file){
                cout<<endl<<"No bookings found!"<<endl;
                return;
            }

            long long totalRevenue=0;
            int count=0;

            cout<<endl<<"=====REVENUE REPORT====="<<endl;
            cout<<"Room\tGuest\t\tNights\tPrice\tAmount\n";
            cout<<"------------------------------------------------"<<endl;

            int roomNo;
            string guest;
            BookingDate ci,co;

            while(readBooking(file,roomNo,guest,ci,co)){
                int nights = co - ci;
                int price  = getPrice(roomNo);

                if(price<0 || nights<=0){
                    continue;
                }

                long long amount = (long long)nights * price;
                totalRevenue += amount;
                count++;

                cout<<roomNo<<"\t"<<guest<<"\t\t"<<nights<<"\t"<<price<<"\t"<<amount<<endl;
            }
            file.close();

            cout<<"------------------------------------------------\n";
            cout<<"Total Bookings : "<<count<<endl;
            cout<<"Total Revenue  : "<<totalRevenue<<endl;
        }
};
class SearchBooking{
    public:
        void searchByRoomNo(int roomno)
        {
            ifstream file("booking.txt");
            if(!file)
            {
                cout<<"EROR!File not opened."<<endl;
                return;
            }
            string line;
            bool found=false;
            while(getline(file,line))
            {
                if(line==to_string(roomno))
                {
                    cout<<"\nBooking Found: \n";
                    cout<<"Room Number: "<<line<<endl;
                    getline(file, line); cout << "Guest Name: " << line << endl;
                    getline(file, line); cout << line << endl; // Check-In
                    getline(file, line); cout << line << endl; // Check-Out
                    getline(file, line); cout << line << endl; // Nights
                    getline(file, line); cout << line << endl;//amount
                    found = true;
                    break;
                }
            }
            if(!found)
            {
                cout<<"No booking found for room "<<roomno<<endl;
            }
            file.close();
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
    class Admin{
        protected:
            string admin_username = "Admin123";
            string admin_password = "Admin123";
        public:
            friend void admin_login();
    };
    void admin_login(){
        clear();
        string user,pass;
        int choice=0;

        cout<<endl<<"Enter 'Admin Username': ";
        cin>>user;

        cout<<endl<<"Enter 'Admin Password': ";
        cin>>pass;

        Admin obj;
        if(pass == obj.admin_password && user == obj.admin_username){
            while(true){
                clear();
                cout<<"Login successful.Welcome,Admin"<<"!\n";
                cout<<"1.Add Room\n";
                cout<<"2.Occupancy Report\n";
                cout<<"3.Revenue Report\n";
                cout<<"4.Logout\n";
                cout<<"\nEnter your choice(1-4) : ";
                cin>>choice;

                if(!cin){
                    clearBadInput();
                    continue;
                }

                switch(choice){
                    case 1:{
                        Room obj1;
                        if(obj1.inputRoom()){
                            obj1.savetofile();
                            obj1.displayRoom();
                        }
                        waitKey();
                        break;
                    }
                    case 2:{
                        OccupancyReport rep;
                        rep.show();
                        waitKey();
                        break;
                    }
                    case 3:{
                        RevenueReport rep;
                        rep.show();
                        waitKey();
                        break;
                    }
                    case 4:{
                        return;
                    }
                    default:{
                        cout<<endl<<"Wrong input!!"<<endl;
                        waitKey();
                        break;
                    }
                }
            }
        }
        else{
            cout<<endl<<"Sorry Wrong username and password!!"<<endl;
            waitKey();
            return;
        }
    }
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
            cout<<endl<<"Wrong format of phone number!!"<<endl;
            cout<<"enter 'Phone Number': ";
            cin>>phone;
        }

        User o(user,pass,email,phone,id);

        fstream obj;
        obj.open(UserFile,ios::app);
        if(!obj){
            cout<<endl<<"No directory!!"<<endl;
            waitKey();
            return;
        }
        obj<<o.user_id<< " " <<o.username <<" " << o.password <<" " <<o.email <<" " <<o.phone <<"\n";
        obj.close();
        cout<<endl<<"Your Account is registered!!"<<endl;
        waitKey();
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

        ifstream in(UserFile);
        if(!in){
            cout<<"No users registered yet!!"<<endl;
            waitKey();
            return;
        }
         ifstream in(UserFile);  
    if(!in){  
        cout<<"No users registered yet!!";  
        return;  
    }  

    User u;  
        while(in>>u.user_id>>u.username>>u.password>>u.email>>u.phone){
            if(u.username == name && u.password == pass){
                u.status=true;
                break;
            }
        }
        in.close();

        if(!u.status){
            cout<<"Wrong username or password!!"<<endl;
            waitKey();
            return;
        }

        while(true){
            clear();
            cout<<"Login successful.Welcome,"<<u.username<<"!"<<endl;
            cout<<"\n===Hotel Reservation System===\n";
            cout<<"1.Search Available Rooms\n";
            cout<<"2.Book a Room\n";
            cout<<"3.Cancel a Booking\n";
            cout<<"4.Logout\n";
            cout<<"Enter your choice:";
            cin>>choice;

            if(!cin){
                clearBadInput();
                continue;
            }

            switch(choice)
            {
                case 1:
                {
                    SearchRooms s;
                    s.show();
                    waitKey();
                    break;
                }
                case 2:
                {
                    Booking b;
                    if(b.input())
                    {
                        b.display();
                        b.savetofile();
                    }
                    waitKey();
                    break;
                }
                case 3:
                {
                    Cancel_Booking obj;
                    obj.input();
                    obj.cancel();
                    waitKey();
                    break;
                }
                case 4:
                {
                    return;
                }
                default:
                {
                    cout<<"Wrong input!!"<<endl;
                    waitKey();
                    break;
                }
            }
        }
    }

    while(in>>u.user_id>>u.username>>u.password>>u.email>>u.phone){  
        if(u.username == name && u.password == pass){  
            u.status=true;  
            clear();  
            cout<<"Login successful.Welcome,"<<u.username<<"!\n";  
            break;  
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
             case 2:
             {
                SearchBooking obj;
                int option;
                cout<<"1.Search by Room Number."<<endl;
                cout<<"2.Search by Guest Name."<<endl;
                cout<<"Enter your choice: ";
                cin>>option;
                if(option==1)
                {
                    int rno;
                    cout<<"Enter Room number: ";
                    cin>>rno;
                    obj.searchByRoomNo(rno);
                }
                else if(option==2)
                {
                    string gname;
                    cin.ignore();
                    cout<<"Enter Guest Name: ";
                    getline(cin,gname);
                    
                }
                cout << "\nPress Enter to continue...";
                cin.ignore();
                cin.get();
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
	else{  cout<<"Wrong username or password!!";  
	in.close();  
	}  
}
}

using namespace use;

void login_menu(){

<<<<<<< HEAD
    int choice=0;
    while(1)
    {
        clear();
        cout<<"\n===Login===\n";
        cout<<"1.login\n";
        cout<<"2.Register\n";
        cout<<"3.Admin\n";
        cout<<"4.Exit\n";
        cout<<"Enter your choice:";
        cin>>choice;

        if(!cin){
            clearBadInput();
            continue;
        }

        switch(choice){
            case 1:{
                user_login();
                break;
            }
            case 2:{
                user_regis();
                break;
            }
            case 3:{
                admin_login();
                break;
            }
            case 4:{
                exit(0);
                break;
            }
            default:{
                cout << "Wrong input!!"<<endl;
                waitKey();
                break;
            }
        }
    }
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
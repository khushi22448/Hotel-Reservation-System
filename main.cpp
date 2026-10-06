#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<limits>
#include<cstdio>
#include<cstdlib>
#include<conio.h>
#include <ctype.h>
#define C "                     "

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

string getPassword() 
{
    string password;
    char ch;

    while ((ch = _getch()) != '\r') 
    { 
        if (ch == '\b') 
        { 
            if (!password.empty()) 
            {
                password.pop_back();
                cout << "\b \b"; 
            }
        } else 
        {
            password.push_back(ch);
            cout << '*'; 
        }
    }
    cout << endl;
    return password;
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
            cout<<C<<"\033[33mEnter Room Number: \033[0m";
            cin>>room_no;
            if(!cin){
                clearBadInput();
                cout<<C<<"\033[31mInvalid room number!\033[0m"<<endl;
                return false;
            }
            if(roomExists(room_no)){
                cout<<C<<"Room "<<room_no<<" already exists!"<<endl;
                return false;
            }
            cout<<C<<"\033[33mEnter Room Type (Single/Double/Suite): \033[0m";
            cin>>type;
            cout<<C<<"\033[33mEnter Price per Night: \033[0m";
            cin>>price;
            if(!cin || price<=0){
                clearBadInput();
                cout<<C<<"\033[31mInvalid price!\033[0m"<<endl;
                return false;
            }
            isAvailable=true;
            return true;
        }
        void displayRoom()
        {
            cout<<C<<"Room_no: "<<room_no<<endl;
            cout<<C<<"Type: "<<type<<endl;
            cout<<C<<"Price: "<<price<<endl;
            cout<<C<<(isAvailable ?"Available":"Booked")<<endl;
        }
        void savetofile()
        {
            ofstream file("rooms.txt",ios::app);
            if(!file)
            {
                cout<<C<<"\033[31mERROR! File not opened!\033[0m";
                return;
            }
            file<<room_no<<" "<<type<<" "<<price<<" "<<isAvailable<<endl;
            file.close();
            cout<<C<<"\033[32mRoom added successfully!\033[0m"<<endl;
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
            cout<<C<<day<<"/"<<month<<"/"<<year;
        }

        void saveToFile(ofstream& file) const
        {
            file<<day<<"/"<<month<<"/"<<year;
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
        cout<<C<<"Invalid date format!"<<endl;
        return false;
    }
    if(!BookingDate::valid(d,m,y,minYear))
    {
        cout<<C<<"Invalid date! Try again"<<endl;
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

        bool input()
        {
            cout<<C<<"\033[33mEnter room number: \033[0m";
            cin>>roomNumber;
            if(!cin)
            {
                clearBadInput();
                cout<<C<<"\033[31mInvalid room number!\033[0m"<<endl;
                return false;
            }
            if(!roomExists(roomNumber))
            {
                cout<<C<<"Room "<<roomNumber<<" does not exist!"<<endl;
                return false;
            }

            cin.ignore(numeric_limits<streamsize>::max(),'\n');

            cout<<C<<"\033[33mEnter guest name: \033[0m";
            getline(cin,guestName);
            if(guestName.empty())
            {
                cout<<C<<"\033[31mGuest name cannot be empty!\033[0m"<<endl;
                return false;
            }

            cout<<C<<"\033[33mEnter check in date (DD MM YYYY): \033[0m";
            if(!inputDate(checkIn,2026))
            {
                return false;
            }

            cout<<C<<"\033[33mEnter check out date (DD MM YYYY): \033[0m";
            if(!inputDate(checkOut,2026))
            {
                return false;
            }

            if(!(checkIn<checkOut))
            {
                cout<<C<<"\033[31mInvalid booking! Check-out date should come after check-in date\033[0m"<<endl;
                return false;
            }

            if(!roomFree(roomNumber,checkIn,checkOut))
            {
                cout<<C<<"\033[31mRoom is already booked for these dates!\033[0m"<<endl;
                return false;
            }
            return true;
        }

        void display()
        {
            cout<<C<<"Room Number : "<<roomNumber<<endl;
            cout<<C<<"Guest Name : "<<guestName<<endl;

            cout<<C<<"Check-In : ";
            checkIn.display();
            cout<<C<<endl;

            cout<<C<<"Check-Out : ";
            checkOut.display();
            cout<<endl;
        }

        void savetofile()
        {
            ofstream file("booking.txt",ios::app);

            if(!file)
            {
                cout<<C<<"\033[31mError! file not opened\033[0m"<<endl;
                return;
            }

            writeBooking(file,roomNumber,guestName,checkIn,checkOut);
            file.close();

            cout<<C<<"\033[32mBooking saved successfully!\033[0m"<<endl;
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
            cout<<C<<"\033[33mEnter Room_no: \033[0m";
            cin>>room_no;
            if(!cin)
            {
                clearBadInput();
                room_no=-1;
            }
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            cout<<C<<"\033[33mEnter Guest Name: \033[0m";
            getline(cin,guest_name);
        }

        void cancel()
        {
            ifstream file("booking.txt");
            if(!file)
            {
                cout<<C<<"\n\033[31mNo bookings found!\033[0m"<<endl;
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
                cout<<C<<"\n\033[31mBooking not found!\033[0m"<<endl;
                return;
            }

            cout<<C<<"\n\033[32mBooking Found:\033[0m\n";
            cout<<C<<"Room Number: "<<rn[idx]<<endl;
            cout<<C<<"Guest Name: "<<gn[idx]<<endl;
            cout<<C<<"Check-In: ";
            ins[idx].display();
            cout<<C<<endl;
            cout<<C<<"Check-Out: ";
            outs[idx].display();
            cout<<C<<endl;

            cout<<C<<"\n\033[33mAre you sure you want to cancel the booking? (y/n): \033[0m";
            cin>>confirm;

            if(confirm!='y' && confirm!='Y')
            {
                cout<<C<<"\033[31mCancellation aborted.\033[0m"<<endl;
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

            cout<<C<<"\033[32mBooking canceled successfully!\033[0m"<<endl;
        }
};

class SearchRooms{
    public:
        void show()
        {
            BookingDate ci,co;

            cout<<C<<"\033[33mEnter check in date (DD MM YYYY): \033[0m";
            if(!inputDate(ci,2026))
            {
                return;
            }
            cout<<C<<"\033[33mEnter check out date (DD MM YYYY): \033[0m";
            if(!inputDate(co,2026))
            {
                return;
            }
            if(!(ci<co))
            {
                cout<<C<<"\033[31mCheck-out date should come after check-in date\033[0m"<<endl;
                return;
            }

            ifstream rooms("rooms.txt");
            if(!rooms)
            {
                cout<<C<<"\033[31mNo rooms found!\033[0m"<<endl;
                return;
            }

            int r,price,avail,found=0;
            string type;

            cout<<C<<"\n\033[1;36m"<<C<<"===== AVAILABLE ROOMS =====\033[0m\n";
            cout<<C<<"Room\tType\tPrice\n";
            cout<<C<<"-----------------------------\n";

            while(rooms>>r>>type>>price>>avail)
            {
                if(roomFree(r,ci,co))
                {
                    cout<<C<<r<<"\t"<<type<<"\t"<<price<<endl;
                    found++;
                }
            }

            if(found==0)
            {
                cout<<C<<"033[31mNo rooms available for these dates.\033[0m"<<endl;
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
            cout<<C<<"\033[33mEnter date for report (DD MM YYYY): \033[0m";
            if(!inputDate(target)){
                return;
            }

            ifstream rooms("rooms.txt");
            if(!rooms){
                cout<<C<<"\033[31mNo rooms found. Add rooms first!!\033[0m"<<endl;
                return;
            }

            int roomNo, price, avail;
            string type;
            int total=0, occupied=0;

            cout<<C<<"\n\033[1;36m"<<C<<"===== OCCUPANCY REPORT (";
            target.display();
            cout<<C<<") =====\033[0m\n";
            cout<<C<<"Room\tType\tStatus\t\tGuest\n";
            cout<<C<<"----------------------------------------------\n";

            while(rooms>>roomNo>>type>>price>>avail){
                total++;
                string guest="-";

                if(isBooked(roomNo,target,guest)){
                    occupied++;
                    cout<<C<<roomNo<<"\t"<<type<<"\tOccupied\t"<<guest<<endl;
                }
                else{
                    cout<<C<<roomNo<<"\t"<<type<<"\tVacant\t\t-"<<endl;
                }
            }
            rooms.close();

            cout<<C<<"----------------------------------------------"<<endl;
            cout<<C<<"Total Rooms    : "<<total<<endl;
            cout<<C<<"Occupied Rooms : "<<occupied<<endl;
            cout<<C<<"Vacant Rooms   : "<<total-occupied<<endl;

            if(total>0)
            {
                cout<<C<<"Occupancy Rate : "<<(occupied*100.0/total)<<"%"<<endl;
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
                cout<<endl<<C<<"\033[31mNo bookings found!\033[0m"<<endl;
                return;
            }

            long long totalRevenue=0;
            int count=0;

            cout<<endl<<C<<"\033[1;36"<<C<<"=====REVENUE REPORT=====\033[0m"<<endl;
            cout<<C<<"Room\tGuest\t\tNights\tPrice\tAmount\n";
            cout<<C<<"------------------------------------------------"<<endl;

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

                cout<<C<<roomNo<<"\t"<<guest<<"\t\t"<<nights<<"\t"<<price<<"\t"<<amount<<endl;
            }
            file.close();

            cout<<C<<"------------------------------------------------\n";
            cout<<C<<"Total Bookings : "<<count<<endl;
            cout<<C<<"Total Revenue  : "<<totalRevenue<<endl;
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

        cout<<endl<<C<<"\033[33mEnter 'Admin Username': \033[0m";
        cin>>user;

        cout<<endl<<C<<"\033[33mEnter 'Admin Password': \033[0m";
        pass = getPassword();

        Admin obj;
        if(pass == obj.admin_password && user == obj.admin_username){
            while(true){
                clear();
                cout<<C<<"\033[1;32m"<<C<<"Login successful.Welcome,Admin"<<"!\033[0m\n";
                cout<<C<<"\033[34m1.Add Room\n";
                cout<<C<<"2.Occupancy Report\n";
                cout<<C<<"3.Revenue Report\n";
                cout<<C<<"4.Logout\n\033[0m";
                cout<<C<<"\n\033[33m"<<C<<"Enter your choice(1-4) : \033[0m";
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
                        cout<<endl<<C<<"\033[31mWrong input!!\033[0m"<<endl;
                        waitKey();
                        break;
                    }
                }
            }
        }
        else{
            cout<<endl<<C<<"\033[31mSorry Wrong username and password!!\033[0m"<<endl;
            waitKey();
            return;
        }
    }
    void user_regis(){
        clear();
        string user,pass,re_pass,email,phone;
        int id;
        id=getNextId();

        cout<<C<<"\033[33mEnter 'Username(No Spaces)': \033[0m";
        cin>>user;

        while(usernameTaken(user)) {
            cout<<endl<<C<<"\033[31mThat username is already taken!!\033[0m"<<endl;
            cout<<endl<<C<<"\033[33mRe-enter 'Username': \033[0m";
            cin>>user;
        }

        do{
            cout<<endl<<C<<"\033[33mEnter 'Password': \033[0m";
            pass = getPassword();

            cout<<endl<<C<<"\033[33mRe-enter 'Password': \033[0m";
            re_pass=getPassword();

            if(pass!=re_pass){
                cout<<endl<<C<<"\033[31mPassword does not match!!!\033[0m"<<endl;
            }
        }while(pass!=re_pass);

        cout<<endl<<C<<"\033[33mEnter 'E-mail': \033[0m";
        cin>>email;
        while(!validEmail(email)){
            cout<<endl<<C<<"\033[31mWrong format of email!!\033[0m"<<endl;
            cout<<C<<"\033[33mEnter 'E-mail': \033[0m";
            cin>>email;
        }

        cout<<endl<<C<<"\033[33mEnter 'Phone Number': \033[0m";
        cin>>phone;
        while(!validPhone(phone)){
            cout<<endl<<C<<"\033[31mWrong format of phone number!!\033[0m"<<endl;
            cout<<C<<"\033[33mEnter 'Phone Number': \033[0m";
            cin>>phone;
        }

        User o(user,pass,email,phone,id);

        fstream obj;
        obj.open(UserFile,ios::app);
        if(!obj){
            cout<<endl<<C<<"\033[31mNo directory!!\033[0m"<<endl;
            waitKey();
            return;
        }
        obj<<o.user_id<< " " <<o.username <<" " << o.password <<" " <<o.email <<" " <<o.phone <<"\n";
        obj.close();
        cout<<endl<<C<<"\033[32mYour Account is registered!!\033[0m"<<endl;
        waitKey();
    }
    void user_login(){
        clear();
        string name,pass;
        int choice=0;
        cout<<C<<"\n\033[1;36m"<<C<<"===Login===\n\033[0m";
        cout<<C<<"\033[33mUsername: \033[0m";
        cin>>name;
        cout<<C<<"\033[33mPassword: \033[0m";
        pass = getPassword();

        ifstream in(UserFile);
        if(!in){
            cout<<C<<"\033[31mNo users registered yet!!\033[0m"<<endl;
            waitKey();
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
            cout<<C<<"\033[31mWrong username or password!!\033[0m"<<endl;
            waitKey();
            return;
        }

        while(true){
            clear();
            cout<<C<<"\033[32mLogin successful.Welcome,\033[0m"<<u.username<<"!"<<endl;
            cout<<C<<"\n\033[1;36m"<<C<<"===Hotel Reservation System===\033[0m\n";
            cout<<C<<"\033[34m1.Search Available Rooms\n";
            cout<<C<<"2.Book a Room\n";
            cout<<C<<"3.Cancel a Booking\n";
            cout<<C<<"4.Logout\033[0m\n";
            cout<<C<<"\033[33mEnter your choice: \033[0m";
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
                    cout<<C<<"\033[31mWrong input!!\033[0m"<<endl;
                    waitKey();
                    break;
                }
            }
        }
    }
}

using namespace use;

void login_menu(){

    int choice=0;
    while(1)
    {
        clear();
        cout<<C<<"\033[1;36m\n"<<C<<"===Login===\033[0m\n";
        cout<<C<<"\033[34m1.login\n";
        cout<<C<<"2.Register\n";
        cout<<C<<"3.Admin\n";
        cout<<C<<"4.Exit\n\033[0m";
        cout<<C<<"\033[33mEnter your choice: \033[0m";
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
                cout <<C<< "\033[31mWrong input!!\033[0m"<<endl;
                waitKey();
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
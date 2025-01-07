#include <iostream>
#include <vector>
#include <memory>
#include <stdexcept>
#include <map>
#include <string>
#include <ctime>
#include <cstdlib>
#include<cmath>
using namespace std;



#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define RESET "\033[0m"


void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void printLargeTextLine(const char *text)
{
    std::cout << "\033[1;33;5m";
    std::cout << text << std::endl;
    std::cout << "\033[0m";
}


void welcomePage()
{
    clearScreen();

    for (int i = 0; i < 5; i++)
    {
        std::cout << std::endl;
    }

    printLargeTextLine("");
    printLargeTextLine("                                                                                                                          ");
    printLargeTextLine("                                                WELCOME TO NH4 HIGHWAY                                                ");
    printLargeTextLine("                                                        *");
    printLargeTextLine("");

    std::cout << std::endl;
    std::cout << "                                       AUTOMATIC TOLL COLLECTION SYSTEM VIA GPS                    " << std::endl;
    std::cout << std::endl
              << std::endl
              << std::endl
              << std::endl
              << std::endl
              << std::endl
              << std::endl
              << std::endl
              << std::endl
              << std::endl
              << std::endl
              << std::endl
              << std::endl
              << std::endl
              << std::endl;
    std::cout << "Press Enter to continue...";

    // Wait for the user to press Enter
    std::cin.ignore();
    std::cin.get();

    // Clear the screen again before displaying the menu
    clearScreen();
}













class InsufficientBalance:public runtime_error
{
public:
    InsufficientBalance(const string& msg):runtime_error(msg){}

};

class InvalidRegistrationNumber:public runtime_error{
public:
    InvalidRegistrationNumber(const string& msg):runtime_error(msg){}
};

class Exception
{
public:
    int errNo;
    string errMsg;

    Exception()
    {
        this->errNo=0;
        this->errMsg="";
    }

    Exception(int errNo,string errMsg)
    {
        this->errNo=errNo;
        this->errMsg=errMsg;
    }

    void printException()
    {
        cout<<errNo<<":"<<errMsg<<endl;
    }
};

class Location;
class Vehicle;


struct City
{
    string name;
    float lattitude;
    float longitude;

};

template<typename T>

class PaymentMethod
{
    string paymentId;
    string type;
    int accountNo;

public:
    PaymentMethod()
    {
        this->paymentId="";
        this->type="";
        this->accountNo=0;
    }
    PaymentMethod(string paymentId,string type,int accountNo)
    {
        this->paymentId=paymentId;
        this->type=type;
        this->accountNo=accountNo;
    }

    string getPaymentId()const
    {
        return paymentId;
    }

    string getType()const
    {
        return type;
    }


    float getAccNo()const
    {
        return accountNo;
    }

    virtual void makePayment(float amount)=0;
    virtual float getLoanAmt()=0;
    virtual void addPenalty(float amount)=0;
    virtual double getBalance()const =0;
};


class CreditCardPaymentMethod : public PaymentMethod<CreditCardPaymentMethod>
{
private:
    float cLoanAmt;

public:
    CreditCardPaymentMethod()
    {
        this->cLoanAmt=0;
    }
    CreditCardPaymentMethod(string id,string type,int accountNo):PaymentMethod(id,"Credit Card",accountNo)
    {
        this->cLoanAmt=cLoanAmt;
    }

    void makePayment(float amount)
    {
        cout<<GREEN<<"Paid Rupees "<<amount<<"using the Users Credit Card"<<RESET<<endl;
        cLoanAmt+=amount;
    }
    getLoanAmt()const
    {
        return cLoanAmt;
    }

};


class BankPaymentMethod : public PaymentMethod<BankPaymentMethod>
{
private:
    double balance;

public:

    BankPaymentMethod() : PaymentMethod("", "", 0), balance(0) {}
    BankPaymentMethod(string id, string type, int accountNo, double balance)
        : PaymentMethod(id, type, accountNo), balance(balance) {}

    void makePayment(float amount) override
    {
        if(balance >= amount)
        {
            cout <<GREEN<< "Paid Rupees " << amount << " Using Users Bank Account." <<RESET<< endl;
            balance -= amount;
        }
        else
        {
            throw InsufficientBalance("Insufficient Bank Balance. Please check your Bank Balance");
        }
    }

    double getBalance() const
    {
        return balance;
    }

    float getLoanAmt() override
    {
        return 0; // No loan amount for bank payment method
    }

    void addPenalty(float amount) override
    {
        float penalty = amount+amount * 0.1;
        balance -= penalty;
        cout<<RED << "Penalty of Rupees " << penalty << " should be paid that amount is added to your Bank Account" <<RESET<< endl;
    }
};


#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define RESET "\033[0m"

class User
{
private:
    string userId;
    string userName;
    string email;
    Vehicle* vehicle;
    PaymentMethod<BankPaymentMethod>* paymentMethod;


public:
    User()
    {
        this->userId="";
        this->userName="";
        this->email="";
    }

    User(string userId,string userName,string email)
    {
        this->userId=userId;
        this->userName=userName;
        this->email=email;
    }

    string getUserId()
    {
        return userId;
    }
    string getUserName()
    {
        return userName;
    }
    string getEmail()
    {
        return email;
    }

    Vehicle* getVehicle()
    {
        return vehicle;
    }
    void setVehicle(Vehicle* v)
    {
        vehicle=v;
    }

    PaymentMethod<BankPaymentMethod>* getPaymentMethod()
    {
        return paymentMethod;
    }
    void setPaymentMethod(PaymentMethod<BankPaymentMethod>* method)
    {
        paymentMethod=method;
    }

    void makePayment(float amount)
    {
        if(paymentMethod)
        {
            try
            {
                paymentMethod->makePayment(amount);
            }
            catch(const InsufficientBalance& e)
            {
                cout<<e.what()<<endl;
                cout<<CYAN<<"Do you want to continue with Credit Card....? (yes/no)"<<RESET<<endl;
                string choice;
                cin>>choice;
                if(choice=="yes")
                {
                    CreditCardPaymentMethod* creditcard = dynamic_cast<CreditCardPaymentMethod*>(paymentMethod);
                    if(creditcard && creditcard->getLoanAmt()>=amount)
                    {
                        creditcard->makePayment(amount);
                    }
                    else
                    {
                        cout<<RED<<"Credit Card Limit Exceeded. Payment Failed."<<RESET<<endl;
                        paymentMethod->addPenalty(amount);
                    }
                }

                else
                {
                    paymentMethod->addPenalty(amount);
                }
            }
        }
            else
            {
                cout<<RED<<"No payment method Set."<<RESET<<endl;
            }
        }
};

class GPSDevice
{
private:
    string deviceId;
    Location* currentLocation;
    static GPSDevice* instance;

    GPSDevice()
    {
        this->deviceId="";
    }
    GPSDevice(string id,Location* location): deviceId(id),currentLocation(location){}


public:
    static GPSDevice* getInstance(string id,Location* location)
    {
        if(instance == nullptr)
        {
            instance= new GPSDevice(id,location);
        }
        return instance;
    }

    string getDeviceId()
    {
        return deviceId;
    }

    Location* getCurrentLocation()
    {
        return currentLocation;
    }

    void updateLocation(Location* location)
    {
        currentLocation = location;
    }
};

GPSDevice* GPSDevice::instance=nullptr;


class Location
{
private:
    float latitude;
    float longitude;

public:
    Location()
    {
        this->latitude=0;
        this->longitude=0;
    }

    Location(float laitude,float longitude)
    {
        this->latitude=latitude;
        this->longitude=longitude;
    }
     float calculateDistance(const Location& other) const {
        float lat1Rad = latitude * M_PI / 180.0;
        float lat2Rad = other.latitude * M_PI / 180.0;
        float lon1Rad = longitude * M_PI / 180.0;
        float lon2Rad = other.longitude * M_PI / 180.0;

        float dlon = lon2Rad - lon1Rad;
        float dlat = lat2Rad - lat1Rad;

        float a = pow(sin(dlat / 2), 2) + cos(lat1Rad) * cos(lat2Rad) * pow(sin(dlon / 2), 2);
        float c = 2 * atan2(sqrt(a), sqrt(1 - a));
        float distance = 6371 * c;

        return distance;
    }
};

class Vehicle
{
private:
    string vehicleType;
    GPSDevice* gpsDevice;
    User* owner;

public:
    Vehicle()
    {

        this->vehicleType="";
    }
    Vehicle(string type,GPSDevice* device):vehicleType(type),gpsDevice(device),owner(nullptr){}



    string getVehicleType()
    {
        return vehicleType;
    }


    User* getOwner()
    {
        return owner;
    }


    void  setOwner(User* user)
    {
        owner=user;
    }

    virtual float calculateTollCharges(float distance)=0;
};

class Car:public Vehicle
{
  public:
      string licensePlate;

      Car()
      {
          this->licensePlate="";
      }

      Car(GPSDevice* device,string licensePlate):Vehicle("Car",device)
      {
          this->licensePlate=licensePlate;
      }

      float calculateTollCharges(float distance)
      {
          if(distance<=100)
          {
              return distance*15;
          }
          else if(distance>100 || distance<500)
          {
              return distance*10;
          }
          else
          {
              return distance*5;
          }
      }
};


class Truck:public Vehicle
{
  public:
      string licensePlate;

      Truck()
      {
          this->licensePlate="";
      }

      Truck(GPSDevice* device,string licensePlate):Vehicle("Truck",device)
      {
          this->licensePlate=licensePlate;
      }

      float calculateTollCharges(float distance)
      {
          if(distance<=100)
          {
              return distance*25;
          }
          else if(distance>100 || distance<500)
          {
              return distance*20;
          }
          else
          {
              return distance*15;
          }
      }
};


class MotorCycle:public Vehicle
{
  public:
      string licensePlate;

      MotorCycle()
      {
          this->licensePlate="";
      }

      MotorCycle(GPSDevice* device,string licensePlate):Vehicle("Truck",device)
      {
          this->licensePlate=licensePlate;
      }

      float calculateTollCharges(float distance)
      {
          if(distance<=100)
          {
              return distance*17;
          }
          else if(distance>100 || distance<500)
          {
              return distance*15;
          }
          else
          {
              return distance*10;
          }
      }
};




class Van:public Vehicle
{
  public:
      string licensePlate;

      Van()
      {
          this->licensePlate="";
      }

      Van(GPSDevice* device,string licensePlate):Vehicle("Truck",device)
      {
          this->licensePlate=licensePlate;
      }

      float calculateTollCharges(float distance)
      {
          if(distance<=100)
          {
              return distance*20;
          }
          else if(distance>100 || distance<500)
          {
              return distance*15;
          }
          else
          {
              return distance*10;
          }
      }
};

class VehicleFactory
{
public:
    static Vehicle* createVehicle(const string& vehicleType,const string& licensePlate,GPSDevice* gpsDevice)
    {
        if(vehicleType=="Car")
        {
            return new Car(gpsDevice,licensePlate);
        }
        else if( vehicleType == "Truck")
        {
            return new Truck(gpsDevice,licensePlate);
        }
          else if( vehicleType == "MotorCycle")
        {
            return new MotorCycle(gpsDevice,licensePlate);
        }
          else if( vehicleType == "Van")
        {
            return new Van(gpsDevice,licensePlate);
        }
        else
        {
            exit(0);
            cout<<RED<<"The Wrong Vehicle Type read By the Device:"<<RESET<<endl;
        }
    }
};
string vehicleInput()
        {
            string vehicleType;
            cout << "Enter vehicle type (Car/Truck/Motorcycle/Van): ";
            cin >> vehicleType;
            return vehicleType;
        }

string regInput()
{
    string registrationNumber;
        cin >> registrationNumber;
        return registrationNumber;

}

int main()
{
    welcomePage();
    cout<<CYAN<<"WELCOME To NH4 Highway of AUTOMATIC TOLL COLLECTION SYSTEM VIA GPS"<<RESET<<endl;
    try
    {
        vector<City>cities = {
            {"Mangalore", 12.9141267, 74.8542122},
            {"Kavoor", 12.9266153, 74.8545822},
            {"Kateelu", 13.0516549, 74.8502452},
            {"Theerthahalli", 13.6910092, 75.2436311},
            {"Shivamogga", 13.9322504, 75.5459433},
            {"Chitradurga", 14.2256497, 76.395358},
            {"Kushtagi", 15.7548853, 76.1955633},
            {"Ilkall", 15.9769623, 76.1062528},
            {"Vijaypur", 16.8299857, 75.7094724}
        };

        int sIndex, dIndex;

        cout << "List of cities along the highway:" << endl;
        for (int i = 0; i < 9; ++i)
        {
            cout << i << ". " << cities[i].name << endl;
        }
        cout << "Enter entry point of the vehicle: ";
        cin >> sIndex;

        Location* initialLocation;
        Location* destination;

        if(sIndex==0)
        {
            initialLocation = new Location(12.9141267, 74.8542122);
            cout << "Vehicle is Entered in Manguluru Highway point" << endl;
        }
        else if(sIndex==1)
        {
            initialLocation = new Location(12.9266153, 74.8545822);
            cout << "Vehicle is Entered in Kavoor Highway point" << endl;

        }
        else if(sIndex==2)
        {
            initialLocation = new Location(13.0516549, 74.8502452);
            cout << "Vehicle is Entered in Kateelu Highway point" << endl;

        }
        else if(sIndex==3)
        {
             initialLocation = new Location(13.6910092, 75.2436311);
             cout << "Vehicle is Entered in Theerthahalli Highway point" << endl;
        }
        else if(sIndex==4)
        {
            initialLocation = new Location(13.9322504, 75.5459433);
            cout << "Vehicle is Entered in Shivamogga Highway point" << endl;
        }
        else if(sIndex==5)
        {
             initialLocation = new Location(14.2256497, 76.395358);
             cout << "Vehicle is Entered in Chitradurga Highway point" << endl;
        }
        else if(sIndex==6)
        {
            initialLocation = new Location(15.7548853, 76.1955633);
            cout << "Vehicle is Entered in Kushtagi Highway point" << endl;

        }
        else if(sIndex==7)
        {
            initialLocation = new Location(15.9769623, 76.1062528);
            cout << "Vehicle is Entered in Ilkall Highway point" << endl;
        }
        else if(sIndex==8)
        {
             initialLocation = new Location(16.8299857, 75.7094724);
             cout << "Vehicle is Entered in Vijaypur Highway point" << endl;

        }
        else
        {
            cout<<RED<<"Invalid Coordinates read by GPS device"<<RESET<<endl;
            throw (Exception (101,"INvalid Entry point  Coordinates"));
        }



        string vehicleType, registrationNumber;


        vehicleType=vehicleInput();
        if(vehicleType=="Car")
        {
            cout<<"Enter Car Registration Number:"<<endl;
            registrationNumber=regInput();
        }
        else if(vehicleType=="Truck")
        {
            cout<<"Enter Truck Registration Number:"<<endl;
            registrationNumber=regInput();

        }
        else if(vehicleType=="Van")
        {
            cout<<"Enter Van Registration Number:"<<endl;
            registrationNumber=regInput();
        }
        else if(vehicleType=="Motorcycle")
        {
            cout<<"Enter MotorCycle Registration Number:"<<endl;
            registrationNumber=regInput();
        }
        else
        {
            throw(Exception(104,"INVALID VEHICLE TYPE"));
        }


        if (registrationNumber.length() > 10|| registrationNumber.length()<5)
        {
            throw InvalidRegistrationNumber("Invalid Vehicle Registation Number that should be between 5 to 10.");
            cout<<"Enter Valid REgistration Number"<<endl;
            registrationNumber=regInput();
        }

        cout << "Enter Exit point of the vehicle: ";
         cout << "List of cities along the highway:" << endl;
        for (int i = 0; i < 9; ++i)
        {
            cout << i << ". " << cities[i].name << endl;
        }

        cin >> dIndex;

        if(dIndex==0)
        {
            destination = new Location(12.9141267, 74.8542122);
            cout << "Vehicle"<<registrationNumber<<" is Exiting from Manguluru Highway point" << endl;
        }
        else if(dIndex==1)
        {
            destination = new Location(12.9266153, 74.8545822);
            cout << "Vehicle "<<registrationNumber<<" is Exiting from Kavoor Highway point" << endl;
        }
        else if(dIndex==2)
        {
             destination = new Location(13.0516549, 74.8502452);
            cout << "Vehicle "<<registrationNumber<<"is Exiting from Kateelu Highway point" << endl;
        }
        else if(dIndex==3)
        {
             destination = new Location(13.6910092, 75.2436311);
              cout << "Vehicle "<<registrationNumber<<" is Exiting from Theerthahalli Highway point" << endl;
        }
        else if(dIndex==4)
        {
             destination = new Location(13.9322504, 75.5459433);
             cout << "Vehicle"<<registrationNumber<<" is Exiting from Shivamogga Highway point" << endl;
        }
        else if(dIndex==5)
        {
             destination = new Location(14.2256497, 76.395358);
             cout << "Vehicle"<<registrationNumber<<" is Exiting from Chitradurga Highway point" << endl;

        }
        else if(dIndex==6)
        {
             destination = new Location(15.7548853, 76.1955633);
             cout << "Vehicle"<<registrationNumber<<" is Exiting from Kushtagi Highway point" << endl;
        }
        else if(dIndex==7)
        {
             destination = new Location(15.9769623, 76.1062528);
            cout << "Vehicle "<<registrationNumber<<" is Exiting from Ilkall Highway point" << endl;
        }
        else if(dIndex==8)
        {
             destination = new Location(16.8299857, 75.7094724);
             cout << "Vehicle "<<registrationNumber<<" is Exiting from Vijaypur Highway point" << endl;
        }
        else
        {
            cout<<RED<<"Invalid Coordinates read by GPS device"<<RESET<<endl;
            throw (Exception (102,"INvalid Exit Point Coordinates"));
        }


        GPSDevice* gpsDevice = GPSDevice::getInstance("GPS123", initialLocation);

        Vehicle* myCar = VehicleFactory::createVehicle(vehicleType, registrationNumber, gpsDevice);

        User* user = new User("543", "SAGAR S", "sagarshegunashi@gmail.com");
        user->setVehicle(myCar);

        BankPaymentMethod* bankPayment = new BankPaymentMethod("PM001", "Bank Account", 2000.0, 0.0);
        user->setPaymentMethod(bankPayment);

        float distance = initialLocation->calculateDistance(*destination);
        cout<<GREEN<<"The Total Distance  travelled is:"<<distance<<"km"<<RESET<<endl;
        float tollCharges = myCar->calculateTollCharges(distance);
        cout<<BLUE<<"The Toll Charge of the distance "<<distance<<"Km is: "<<tollCharges<<RESET<<endl;

        user->makePayment(tollCharges);

        cout <<YELLOW<< "Remaining Balance: Rupees " << user->getPaymentMethod()->getBalance() <<RESET<< endl;
    }
       catch(Exception& e)
        {
            e.printException();
        }
    catch (const InvalidRegistrationNumber& e)
    {
        cout <<RED<< e.what()<<RESET<< endl;

    }
    catch (const InsufficientBalance& e)
     {
        cout <<RED<< e.what() <<RESET<< endl;
    }

    cout<<endl;
    cout<<MAGENTA<<"THANKS FOR USING AUTOMATIC TOLL SYSTEM VIA GPS"<<endl;
    cout<<"HAPPY JOURNEY :)"<<RESET<<endl;

    return 0;
}
#include<iostream>
#include<vector>
#include<string>
#include<algorithm>

using namespace std;

//Making interface of the Notification 

//abstract class : Interface 
class INotification{
    public:
    //function k aage const laganen ka mtlb hai ki ye use read karega lekin use change nhi karega.(content ko)
    //---> no modification only constant.
    virtual string getContent() const  = 0;
    //Agar derived object ko base pointer se delete karenge to derived class ka destructor 
    //bhi call ho jayega 
    virtual ~INotification(){};
};

//concrete class: Simple text Notification (inherit karke).

class SimpleNotification : public INotification{

    private:
    string text;//the msg (content)

    public:
    SimpleNotification(const string& msg){
        text = msg;
        //private text ko msg k barabar kar diya 
    }
    // override karte time bhi hme const likhna padega kyunki parent ka change nhi kar skta 
    // liskov --> method rule --> pre-condition
    string getContent() const override{
        return text;
    }


};
//Ab ye hm hmesha hi user ko simple msg hi bhejenge bs frk yahi hoga ki hm use decorate kar denge 
//alag alag tarike.



 //                                                 <------------------DECORATOR---------->


class INotificationDecorator : public INotification{
    protected :
    INotification* notification;

    public: 
    INotificationDecorator(INotification* n ){
        notification = n;
    }

    virtual ~INotificationDecorator(){
        delete notification;
    }


};

//To add timestamp
class TimeStampDecorator : public INotificationDecorator{
      public:

      TimeStampDecorator(INotification* n) : INotificationDecorator(n){}
      //c++ me initialization k liye hota hai.

      string getContent() const override {
        return  "[2025-04-13 10:30:00]" + notification->getContent();
      }
};

//To add signature

class SignatureDecorator : public INotificationDecorator{
    public:
    SignatureDecorator(INotification* n) : INotificationDecorator(n){}

    string getContent() const override{
        return "Abes Engineering College" + notification->getContent();
    }
};

// We have successfully created the notication's different messages.
//We can add more decorators.

//                                                        ----->OBSERVER<----


class IObserver{
    public:
    virtual void update() = 0;
    //no const as this would be modified.

    virtual ~IObserver(){};

};

class IObservable{
    public:
    virtual void addObserver(IObserver* observer) = 0;//subscribe

    virtual void removeObserver(IObserver* observer) = 0;//unsubscribe

    virtual void notifyObservers() = 0;//update them 


};


class NotificationObservable : public IObservable{
    private :
    vector<IObserver*>observers;

    INotification* currentNotification;

    public:
    NotificationObservable(){
        currentNotification = nullptr;

    }

    void addObserver(IObserver* obs) override{
         observers.push_back(obs);

    }

    void removeObserver(IObserver* obs) override{
        observers.erase(remove(observers.begin(), observers.end(),obs ), observers.end());
        //is line ka mtlb hai ---remove delete nahi karta, unwanted ko (selected ko ) end me shift kar deta hai 
        //aur ek iterator return karta hai jaha se unwanted element suru hote hai 
    }

    void notifyObservers() override{
        for(unsigned int i = 0; i < observers.size(); i++){
            observers[i]->update();
        }
    }

    void setNotification(INotification * notification){
        if(currentNotification != nullptr){
            delete currentNotification;
        }
        currentNotification = notification;
        notifyObservers();
    }

    INotification* getNotification(){
        return currentNotification;

    }

    string getNotificationContent(){
       return currentNotification->getContent();
    }

    ~NotificationObservable(){
         if(currentNotification!= nullptr){
            delete currentNotification;
         }
    }
};
// Logger class to log the notifications
//logger ek observer hn jo har new notificattion ko console me log krta hn
class Logger:public IObserver{
    private:
    NotificationObservable* notificationObservable;
    public:
    Logger(NotificationObservable* observable){
        this->notificationObservable=observable;
    }
    void update(){
        cout<<"Logging New Notification : \n"<< notificationObservable->getNotificationContent();
    }
};
//ab second observer humara NotificationEngine hoga jo observable se notification content leta hai urr usse hrr tarike se send krta hn toh hum mail popup sms teen tarike ki strategy
//toh phle hum  notification ki strategy banayege taki notification engine use kr paye
class INotificationStrategy{
    public:
    virtual void sendNotifcation(string content) =0;
};
//ab phli strategy email ki bnayege
class EmailStrategy:public INotificationStrategy{
    private:
        string emailId;
    public:
        EmailStrategy(string emailId){
            this->emailId=emailId;
        }

        void sendNotifcation(string content) override{
            cout<<"Sending email notification to:"<<emailId<<"\n"<<content;
        }
    };


//SMS strategy
class  SMSStrategy:public INotificationStrategy{
    private:
        string mobileNumber;
    public:
        SMSStrategy(string mobileNumber){
            this->mobileNumber=mobileNumber;
        }     

        void sendNotifcation(string content){
            cout<<"Sending SMS Notification to:"<<mobileNumber<<"\n"<<content;
        }
};

//popup notification
class PopUpStrategy:public INotificationStrategy{
    public:
        void sendNotifcation(string content){
            cout<<"Sending Popup Notication: \n"<<content;
        }
};
//NOTIFICATION ENGINE
class NoticationEngine :public IObserver{
    private:
        NotificationObservable* notificationObservable;
        vector<INotificationStrategy*> notficationStrategies;

    public:
        NoticationEngine(NotificationObservable* observable){
            this->notificationObservable=observable;
        }

        void addNotificationStrategy(INotificationStrategy* ns){
            this->notficationStrategies.push_back(ns);
        }

        void update(){
            string noticationContent=notificationObservable->getNotificationContent();
            for(const auto notficationStrategy:notficationStrategies){
                notficationStrategy->sendNotifcation(noticationContent);
            }
        };


}







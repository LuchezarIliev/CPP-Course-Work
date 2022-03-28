#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <limits>
#include <string>

using namespace std;

enum GroupType { Default, Students, Teachers, Staff };

class Person {
    private:
        string name;
        string egn;
        GroupType type;
    public:
        Person() { name = "Unnamed User"; egn = "0000000000"; type = Default; }
        Person(const string& n, const string& e, const GroupType& t)
        {
            name = n;
            egn = e;
            type = t;
        }
        virtual ~Person()
        {
            cout << "Destructing Person..." << endl;
        }
        const string& getName() const { return name; }
        const string& getEGN() const { return egn; }
        const GroupType& getType() const { return type; }
        void setName(const string& newName) { name = newName; }
        void setEGN(const string& newEGN) { egn = newEGN; }
        virtual double getDiscount() = 0;
        virtual void print(ofstream& ofs)
        {
            ofs << "Name: " << name << " EGN: " << egn << endl;
        }
        virtual void printDiscount(ofstream& ofs)
        {
            double discountPercent = getDiscount()*100;
            if(discountPercent > 0) ofs << "Discount = ";
            else if (discountPercent < 0) ofs << "Surcharge = ";
            else ofs << "No discount! ";
            ofs << discountPercent << "%" << endl;
        }
        friend ofstream& operator<<(ofstream& ofs, Person& person)
        {
            person.print(ofs);
            return ofs;
        }
};

enum Behavior { Good, Moderate, Bad };

string getBehaviorString(const Behavior& behavior)
{
    string result;
    switch(behavior)
    {
        case Good: result = "Good"; break;
        case Moderate: result = "Moderate"; break;
        case Bad: result = "Bad"; break;
    }
    return result;
};

class Student : public Person
{
    private:
        Behavior behavior;
        double avgGrade;
    public:
        Student(): Person() { avgGrade = 0; behavior = Good; }
        Student(const string& n, const string& e, const GroupType& t, const Behavior& b, const double& g): Person(n, e, t)
        {
            behavior = b;
            avgGrade = g;
        }
        ~Student()
        {
            cout << "Destructing Student..." << endl;
        }
        const double& getAvgGrade() const { return avgGrade; }
        void setAvgGrade(const double& newGrade) { avgGrade = newGrade; }
        const Behavior& getBehavior() const { return behavior; }
        void setBehavior(const Behavior& newBehavior) { behavior = newBehavior; }
        double getDiscount()
        {
            double discount;
            if(avgGrade >= 5.50) discount = 0.25;
            else if (avgGrade >= 4 && avgGrade < 5.50) discount = 0.1;
            else discount = 0;
            switch(behavior)
            {
                case Good: discount += 0.1; break;
                case Moderate: break;
                case Bad: discount -= 0.1; break;
            }
            return discount;
        }
        void print(ofstream& ofs)
        {
            Person::print(ofs);
            ofs << "Additional info: " << endl;
            ofs << "Average grade: " << avgGrade << " Behavior: " << getBehaviorString(behavior) << endl;
            Person::printDiscount(ofs);
        }
};

class Teacher : public Person
{
    private:
        string subject;
        double rating;
    public:
        Teacher(): Person() { subject = "Unnamed subject"; rating = 0; }
        Teacher(const string& n, const string& e, const GroupType& t, const string& s, const double& r): Person(n, e, t)
        {
            subject = s;
            rating = r;
        }
        ~Teacher()
        {
            cout << "Destructing Teacher..." << endl;
        }
        const double& getRating() const { return rating; }
        void setRating(const double& newRating) { rating = newRating; }
        const string& getSubject() const { return subject; }
        void setSubject(const string& newSubject) { subject = newSubject; }
        double getDiscount()
        {
            double discount;
            if(rating >= 0 && rating < 2) discount = 0.1;
            else if (rating >= 2 && rating < 4) discount = 0.15;
            else discount = 0.2;
            return discount;
        }
        void print(ofstream& ofs)
        {
            Person::print(ofs);
            ofs << "Additional info: " << endl;
            ofs << "Subject: " << subject << " Rating: " << rating << endl;
            Person::printDiscount(ofs);
        }
};

class Personnel : public Person
{
    private:
        string position;
        int months;
    public:
        Personnel(): Person() { position = "Unnamed position"; months = 0; }
        Personnel(const string& n, const string& e, const GroupType& t, const string& p, const int& m): Person(n, e, t)
        {
            position = p;
            months = m;
        }
        ~Personnel()
        {
            cout << "Destructing Personnel..." << endl;
        }
        const string& getPosition() const { return position; }
        void setPosition(const string& newPosition) { position = newPosition; }
        const int& getMonths() const { return months; }
        void setMonths(const int& newMonths) { months = newMonths; }
        double getDiscount()
        {
            double discount;
            if(months >= 12 && months < 24) discount = 0.1;
            else if (months >= 24 && months < 48) discount = 0.15;
            else discount = 0.2;
            return discount;
        }
        void print(ofstream& ofs)
        {
            Person::print(ofs);
            ofs << "Additional info: " << endl;
            ofs << "Position: " << position << " Months experience: " << months << endl;
            Person::printDiscount(ofs);
        }
};

bool isValidEGN(const string& s)
{
    for (const char c : s)
        if (!isdigit(c))
        {
            cout << "Invalid EGN! Use only numbers: ";
            return false;
        }
    return true;
}

bool isValidName(const string& s)
{
    for (const char c : s)
        if (!isalpha(c) && !isspace(c))
        {
            cout << "Invalid name! Use only letters and spaces: ";
            return false;
        }
    return true;
}

double getValidDouble(const int lower, const int upper)
{
    double value;
    while (1) {
        if (cin >> value && value >= lower && value <= upper)
            break;
        else
        {
            cout << "Please input a value between [" << lower << "," << upper << "]: ";
            cin.clear();
            while (cin.get() != '\n');
        }
    }
    return value;
}

int getValidNumericInput()
{
    int number;
    bool validInput = false;
    do {
        cin >> number;

        if (!(validInput = cin.good()))
        {
            cout << "That input is invalid!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    } while (!validInput);
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return number;
}

void createNewUser(vector<Person*> &users)
{
    Person* user;
    string name, egn;
    int type;
    cout << "Enter name for new user: ";
    do {
        getline(cin, name);
    } while(!isValidName(name));
    cout << "Enter EGN: ";
    do
    {
        getline(cin, egn);
    } while (!isValidEGN(egn));
    cout << "\nNew user will be of group: [1-3]" << endl;
    cout << "1. Student\t2. Teachers\t3. Staff" << endl;
    do {
        cout << "Enter your choice: ";
        type = getValidNumericInput();
    } while (type < 1 || type > 3);

    switch(type)
    {
        case Students:
        {
            Behavior behavior;
            double avgGrade;
            cout << "Enter student behavior: [1-3]"<< endl;
            cout << "1. Good\t2. Moderate\t3. Bad" << endl;
            do {
                cout << "Enter your choice: ";
                behavior = static_cast<Behavior>(getValidNumericInput()-1);
            } while (behavior < 0 || behavior > 2);

            cout << "Enter average grade: ";
            avgGrade = getValidDouble(2, 6);
            user = new Student(name, egn, Students, behavior, avgGrade);
        } break;
        case Teachers:
        {
            string subject;
            double rating;
            cout << "Enter teacher subject: ";
            do {
                getline(cin, subject);
            } while(!isValidName(subject));

            cout << "Enter rating: ";
            rating = getValidDouble(0, 5);
            user = new Teacher(name, egn, Teachers, subject, rating);
        } break;
        case Staff:
        {
            string position;
            int months;
            cout << "Enter personnel position: ";
            do {
                getline(cin, position);
            } while(!isValidName(position));

            cout << "Enter months experience: ";
            months = getValidNumericInput();
            user = new Personnel(name, egn, Staff, position, months);
        } break;
    }
    users.push_back(user);
};

void removeUser(vector<Person*> &users)
{
    string egn ;
    cout << "Enter EGN of user: ";
    cin >> egn;
    auto position = find_if(users.begin(), users.end(), [&egn](Person* user) {return user->getEGN() == egn;});
    if (position != users.end())
        users.erase(position);
}

void readFromFile(const char* fileName)
{
    ifstream ifs(fileName);
    if (ifs.is_open())
    {
        cout << ifs.rdbuf() << endl;
        ifs.close();
    }
    else
        cout << "Error opening file!" << endl;
};

void writeToFile(const char* fileName, vector<Person*> &users)
{
    ofstream ofs;
    ofs.open(fileName, ios::out | ios::app);
    if(ofs.is_open())
    {
        for (auto& u : users)
            ofs << *u << endl;
        ofs.close();
		cout << "Successfully stored data to " << fileName << endl;
    }
	else
        cout << "Error opening file!" << endl;
    users.clear();
};

int main()
{
    cout << "\tSchool canteen system" << endl;
    const char* fileName = "Data.TXT";
    vector<Person*> users;
    int menuChoice = 0;
    bool runningState = true;

	while(runningState)
	{
	    cout << "\t\tMENU:\n\n";
		cout << "\t|1| Show contents of file\n";
		cout << "\t|2| Add user\n";
		cout << "\t|3| Remove user\n";
		cout << "\t|4| Save users to file: " << fileName << '\n';
		cout << "\t|5| Exit\n\n";
        cout << "Enter your choice: ";
		menuChoice = getValidNumericInput();

		switch(menuChoice)
		{
			case 1:
				cout << "Print file content...\n" << endl;
				readFromFile(fileName);
				break;
			case 2:
				cout << "Create user...\n" << endl;
				createNewUser(users);
				break;
			case 3:
			    cout << "Remove user...\n" << endl;
                removeUser(users);
				break;
			case 4:
                cout << "Save to file...\n" << endl;
                writeToFile(fileName, users);
				break;
			case 5:
			    cout << "Exiting...\n" << endl;
				runningState = false;
				break;
			default:
				cout << "Incorrect input. Choose integer in interval [1,5]\n";
				break;
		}
	}

    return 0;
}

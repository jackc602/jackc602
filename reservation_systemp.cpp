//Kevin McGrath, Nick Gribbon, and Jack Carroll all worked on this project

#include <iostream>
#include <deque>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <algorithm>
#include <chrono>

using namespace std;

using namespace std::chrono;



// Forward declarations
class ReservationManager;

class SystemUser {
public:
    string firstName;
    string lastName;
    string groupName;
    string username;
    string password;
    int userID;

    SystemUser(string fn, string ln, string gn, string un, string pw, int id)
        : firstName(fn), lastName(ln), groupName(gn), username(un), password(pw), userID(id) {}
};

//delimiter splitter
vector<string> split(const string& str, char delim) {
    stringstream ss(str);
    string item;
    vector<string> tokens;
    while (getline(ss, item, delim)) {
        tokens.push_back(item);
    }
    return tokens;
}

class Reservation;

//a reservation manager. This class does
//the main work of managing our reservations
class ReservationManager {
private:
    deque<Reservation> reservations;

public:
    ReservationManager() {}

    void loadReservations(const string& filename);
    bool checkAvailable(struct tm& dateTime, float length);
    void userDeleteReservation(SystemUser &user);
    void printSchedule();
    void changeUserReservation(SystemUser &user);
    void viewUserReservations(SystemUser &user);
    void displayReservationsOnDay(struct tm& date);
    void purchaseTicket(SystemUser& user);
    void updateReservationsFile();
    void viewUserTickets(SystemUser& user);
    void displayTicketedReservationsOnDay(struct tm& date, SystemUser& user);
};

//defined as a global variable
ReservationManager manager;
//forward dec
void mainMenu(SystemUser &user, ReservationManager &mgr) ;

//a reservation. Meant to show an event that has been reserved
class Reservation {
public:
    bool isPrivate;
    bool isPaid = false;
    int guestCount;
    float length;
    struct tm dateTime;
    string layout;
    int id;
    string group;
    vector<int> ticketHolders;

    //intended constructor
    Reservation(SystemUser& user) : id(user.userID), group(user.groupName), isPrivate(false), isPaid(false), guestCount(0), length(0.0) {}

    //functions
    void makeReservation(ReservationManager& mngr);
    void purchaseTicket(int userID);
    int calculatePayment();
};

//make the user one of the ticket holders within the reservation
void Reservation::purchaseTicket(int userID) {
    if (find(ticketHolders.begin(), ticketHolders.end(), userID) == ticketHolders.end()) {
        ticketHolders.push_back(userID);
    }
}

//the first step in purchasing a ticket, looks up all reservations to make sure
//that it is a vlid reservation
void ReservationManager::purchaseTicket(SystemUser& user) {
    int resID;
    cout << "Enter the reservation ID you want to purchase a ticket for: ";
    cin >> resID;

    bool found = false;
    for (auto& res : reservations) {
        if (res.id == resID) {
            res.purchaseTicket(user.userID);
            found = true;
            break;
        }
    }

    if (found) {
        updateReservationsFile();
        cout << "Ticket purchased successfully." << endl;
        mainMenu(user, manager);
    } else {
        cout << "Reservation ID not found." << endl;
    }
}

//update the reservations file after a ticket has been purchased. Use the '-' delimiter to separate
//users who have purchased tickets
void ReservationManager::updateReservationsFile() {
    ofstream outFile("reservations.csv");
    for (const auto& rez : reservations) {
        outFile << rez.id << "," << rez.group << "," << rez.dateTime.tm_mon + 1 << "," << rez.dateTime.tm_mday << ","
                << rez.dateTime.tm_year + 1900 << "," << rez.dateTime.tm_hour << "," << rez.dateTime.tm_min << "," 
                << rez.length << "," << rez.guestCount << "," << rez.layout << "," << rez.isPrivate << "," 
                << rez.isPaid;
        for (const auto& ticketHolder : rez.ticketHolders) {
            outFile << "-" << ticketHolder;
        }
        outFile << "\n";
    }
    outFile.close();
}

//DELETE a reservation. This will update the file and can't be undone
void ReservationManager::userDeleteReservation(SystemUser &user){
    int choice;
    deque<Reservation> userRez;
    // find the reservations pertaining to the user and display them
    for (auto rez : reservations){
        if (rez.id == user.userID){
            userRez.push_back(rez);
        }
    }
    for (size_t i = 0; i < userRez.size(); ++i) {
        cout << i + 1 << ". Reservation date/time: " << asctime(&userRez[i].dateTime) << endl;
    }
    // have user choose what reservation to delete
    cout << "Enter the number of the reservation you would like to cancel (1-" << userRez.size() << "): ";
    cin >> choice;
    while (choice < 1 || choice > static_cast<int>(userRez.size())){
        cout << "Invalid choice. Please enter a number between 1 and " << userRez.size() << ": ";
        cin >> choice;
    }
    
    // delete the reservation from the manager object
    choice--;
    for (auto it = reservations.begin(); it != reservations.end(); ++it){
        if ((*it).dateTime.tm_mon == userRez[choice].dateTime.tm_mon &&
            (*it).dateTime.tm_mday == userRez[choice].dateTime.tm_mday &&
            (*it).dateTime.tm_hour == userRez[choice].dateTime.tm_hour){
            reservations.erase(it);
            break;
        }
    }
    
    // rewrite the csv without the reservation that has been deleted
    ofstream outFile("reservations.csv");
    for (auto rez : reservations){
        outFile << rez.id << "," << rez.group << "," << rez.dateTime.tm_mon + 1 << "," << rez.dateTime.tm_mday << ","
                << rez.dateTime.tm_year + 1900 << "," << rez.dateTime.tm_hour << "," << rez.dateTime.tm_min << "," << rez.length 
                << "," << rez.guestCount << "," << rez.layout << "," << rez.isPrivate << "," << rez.isPaid << "\n";
    }
    outFile.close();
    cout << "Reservation deleted successfully." << endl;
}

//we will call on startup this function to laod in the reservations from 
// a csv file
void ReservationManager::loadReservations(const string& filename) {
    ifstream inFile(filename);

    if (!inFile) {
        cerr << "Error: Unable to open reservations file." << endl;
        return;
    }

    string line;
    while (getline(inFile, line)) {
        if (line.empty()) continue;

        istringstream ss(line);
        string userIDStr, groupName, monthStr, dayStr, yearStr, hourStr, minStr, lengthStr, guestCountStr, layout, isPrivateStr, isPaidStr, ticketHoldersStr;

        // Extract each field
        getline(ss, userIDStr, ',');
        getline(ss, groupName, ',');
        getline(ss, monthStr, ',');
        getline(ss, dayStr, ',');
        getline(ss, yearStr, ',');
        getline(ss, hourStr, ',');
        getline(ss, minStr, ',');
        getline(ss, lengthStr, ',');
        getline(ss, guestCountStr, ',');
        getline(ss, layout, ',');
        getline(ss, isPrivateStr, ',');
        getline(ss, isPaidStr, ',');

        try {
            // Parse the date and time
            struct tm dateTime = {};
            dateTime.tm_year = stoi(yearStr) - 1900;
            dateTime.tm_mon = stoi(monthStr) - 1;
            dateTime.tm_mday = stoi(dayStr);
            dateTime.tm_hour = stoi(hourStr);
            dateTime.tm_min = stoi(minStr);
            dateTime.tm_sec = 0;

            // Create a user object to initialize Reservation
            SystemUser user("", "", groupName, "", "", stoi(userIDStr));
            Reservation res(user);
            res.dateTime = dateTime; 
            res.length = stof(lengthStr); 
            res.guestCount = stoi(guestCountStr);
            res.layout = layout;
            res.isPrivate = (isPrivateStr == "1");
            res.isPaid = (isPaidStr == "1");

            // extract and process ticket holders
            if (getline(ss, ticketHoldersStr, ',')) {
                vector<int> ticketHolders;
                if (!ticketHoldersStr.empty()) {
                    vector<string> tickets = split(ticketHoldersStr, '-');
                    for (const auto& ticket : tickets) {
                        ticketHolders.push_back(stoi(ticket));
                    }
                }
                res.ticketHolders = ticketHolders;
            }

            reservations.push_back(res);
        } catch (const invalid_argument& e) {
            cerr << "Error: Invalid data in reservations file: " << e.what() << endl;
            continue;
        } catch (const out_of_range& e) {
            cerr << "Error: Data out of range in reservations file: " << e.what() << endl;
            continue;
        }
    }

    inFile.close();
    cout << "Reservations loaded successfully." << endl;
}

// Find and display the dates where the user has tickets
void ReservationManager::viewUserTickets(SystemUser& user) {
    vector<tm> ticketDates;
    for (const auto& res : reservations) {
        if (find(res.ticketHolders.begin(), res.ticketHolders.end(), user.userID) != res.ticketHolders.end()) {
            ticketDates.push_back(res.dateTime);
        }
    }

    if (ticketDates.empty()) {
        cout << "You have no tickets." << endl;
        cout << "Returning to the main menu." << endl;
        mainMenu(user, manager);
    } else {
        for (auto& date : ticketDates) {
            displayTicketedReservationsOnDay(date, user);
        }
    }
}

//display the tickets. This will show TICKETED on the daily schedule where there are tickets
void ReservationManager::displayTicketedReservationsOnDay(struct tm& date, SystemUser& user) {
    //make it midnight
    date.tm_hour = 0;
    date.tm_min = 0;
    date.tm_sec = 0;
    time_t date_time_t = mktime(&date);

    //time slots for day. unnecessary but whatever
    vector<string> timeSlots(24, "-------");

    // Mark the reserved slots and ticketed events
    for (const auto& res : manager.reservations) {
        struct tm resStart = res.dateTime;
        struct tm resEnd = resStart;
        resEnd.tm_min += res.length * 60;
        mktime(&resEnd);

        if (resStart.tm_year == date.tm_year && resStart.tm_mon == date.tm_mon && resStart.tm_mday == date.tm_mday) {
            for (int i = resStart.tm_hour; i < resEnd.tm_hour; ++i) {
                if (find(res.ticketHolders.begin(), res.ticketHolders.end(), user.userID) != res.ticketHolders.end()) {
                    timeSlots[i] = "TICKETED";
                } else {
                    timeSlots[i] = "RESERVED";
                }
            }
        }
    }

    cout << "Time slots for " << asctime(&date);
    for (int i = 0; i < 24; ++i) {
        cout << setw(2) << setfill('0') << i << ":00 - " << timeSlots[i] << endl;
    }
}

//check availability of a reservation
bool ReservationManager::checkAvailable(struct tm& dateTime, float length) {
    for (auto& res : reservations) {
        struct tm resStart = res.dateTime;
        struct tm resEnd = resStart;
        resEnd.tm_min += res.length * 60;
        mktime(&resEnd);

        struct tm end = dateTime;
        end.tm_min += length * 60;
        mktime(&end);

        if ((dateTime.tm_year == resStart.tm_year && dateTime.tm_mon == resStart.tm_mon && dateTime.tm_mday == resStart.tm_mday) &&
            !(mktime(&dateTime) >= mktime(&resEnd) || mktime(&end) <= mktime(&resStart))) {
            return false;
        }
    }
    return true;
}

void ReservationManager::displayReservationsOnDay(struct tm& date) {
    // Normalize the time part to 00:00 for comparison
    date.tm_hour = 0;
    date.tm_min = 0;
    date.tm_sec = 0;
    time_t date_time_t = mktime(&date);

    // Time slots for the entire day (assuming 24-hour slots)
    vector<string> timeSlots(24, "-------");

    // Mark the reserved slots
    for (const auto& res : manager.reservations) {
        struct tm resStart = res.dateTime;
        struct tm resEnd = resStart;
        resEnd.tm_min += res.length * 60;
        mktime(&resEnd);

        if (resStart.tm_year == date.tm_year && resStart.tm_mon == date.tm_mon && resStart.tm_mday == date.tm_mday) {
            for (int i = resStart.tm_hour; i < resEnd.tm_hour; ++i) {
                timeSlots[i] = "RESERVED";
            }
        }
    }
    
    cout << "Time slots for " << asctime(&date);
    for (int i = 0; i < 24; ++i) {
        cout << setw(2) << setfill('0') << i << ":00 - " << timeSlots[i] << endl;
    }
}

bool isPastDateTime(const tm& dateTime) {
    auto now = chrono::system_clock::now();
    time_t now_time_t = chrono::system_clock::to_time_t(now);
    tm now_tm = *localtime(&now_time_t);

    tm input_tm = dateTime;
    time_t input_time_t = mktime(&input_tm);

    return difftime(input_time_t, now_time_t) < 0;
}

void Reservation::makeReservation(ReservationManager& mngr) {
    // Existing code for taking input remains unchanged
    struct tm dateTime;
    float length;
    int guests;
    int privacy;
    int payChoice;

    // User inputs for reservation
    cout << "Enter reservation date (MM DD YYYY): ";
    cin >> get_time(&dateTime, "%m %d %Y");
    cout << "Please see the reservations for this date below" << endl;
    mngr.displayReservationsOnDay(dateTime);
    cout << "Enter reservation time (HH mm): ";
    cin >> get_time(&dateTime, "%H %M");
    cout << endl;
    cout << "Enter reservation length in hours: ";
    cin >> length;
    
    //check for an earlier than now() reservation or a reservation made during another time
    while (isPastDateTime(dateTime) || !mngr.checkAvailable(dateTime, length)) {
        if (isPastDateTime(dateTime)) {
            cout << "The entered date and time are in the past. Please enter a future date and time." << endl;
        } else {
            cout << "The requested time slot is not available. Please choose another time." << endl;
        }
        
        cout << "Enter reservation date (MM DD YYYY): ";
        cin >> get_time(&dateTime, "%m %d %Y");
        
        cout << "Enter reservation time (HH MM): ";
        cin >> get_time(&dateTime, "%H %M");

        cout << "Enter reservation length in hours: ";
        cin >> length;
    }

    cout << "Enter the max amount of guests you would like: ";
    cin >> guests;
    cout << "Enter your desired layout style for the event [meeting, lecture, wedding, dance]: ";
    cin.ignore();  
    getline(cin, layout);
    cout << "Enter 1 for public or 2 for private: ";
    cin >> privacy;

    // Assign values to the Reservation object
    this->dateTime = dateTime;
    this->length = length;
    this->guestCount = guests;
    this->isPrivate = (privacy == 2);

    int cost = calculatePayment();
    cout << "Created event at " << asctime(&dateTime) << endl;
    cout << "Cost of event is $" << cost << ". Enter 1 to pay now and confirm or 2 to pay later: ";
    cin >> payChoice;

    while (payChoice != 1 && payChoice != 2) {
        cout << "Invalid choice. Enter 1 to pay now and confirm or 2 to pay later: ";
        cin >> payChoice;
    }

    if (payChoice == 1) {
        isPaid = true;
    }

    // Debugging output to confirm writing to file
    cout << "Writing reservation to file..." << endl;
    
    ofstream outFile("reservations.csv", ios::app);
    if (outFile.is_open()) {
        outFile << id << "," << group << "," << dateTime.tm_mon + 1 << "," << dateTime.tm_mday << ","
                << dateTime.tm_year + 1900 << "," << dateTime.tm_hour << "," << dateTime.tm_min << "," << length << "," 
                << guestCount << "," << layout << "," << isPrivate << "," << isPaid << "\n";
        outFile.close();
        cout << "Reservation written to file successfully." << endl;
    } else {
        cerr << "Error: Unable to open reservations file for writing." << endl;
    }

    if (isPaid) {
        cout << "Reservation made and confirmed!" << endl;
    } else {
        cout << "Reservation created but not confirmed. Please pay for reservation to receive confirmation." << endl;
    }
}

int Reservation::calculatePayment() {
    if (group == "resident") {
        return static_cast<int>(10 * length + 10);
    } else if (group == "non-resident") {
        return static_cast<int>(15 * length + 10);
    } else if (group == "city") {
        return static_cast<int>(5 * length + 10);
    } else if (group == "organization") {
        return static_cast<int>(20 * length + 10);
    } else {
        return 0;
    }
}

void ReservationManager::printSchedule() {
    cout << "Current Schedule:" << endl;
    for (auto& rez : reservations) {
        cout << "Reservation ID: " << rez.id << ", Group: " << rez.group
             << ", Date/Time: " << asctime(&rez.dateTime) << ", Length: " << rez.length
             << ", Guests: " << rez.guestCount << ", Layout: " << rez.layout
             << ", Private: " << (rez.isPrivate ? "Yes" : "No") << ", Paid: " << (rez.isPaid ? "Yes" : "No") << endl;
    }
}

void ReservationManager::changeUserReservation(SystemUser &user) {
    int choice;
    deque<Reservation> userRez;
    
    for (auto& rez : reservations) {
        if (rez.id == user.userID) {
            userRez.push_back(rez);
        }
    }
    
    if (userRez.empty()) {
        cout << "You have no reservations to change." << endl;
        cout << "Returning to the main menu." << endl;
        mainMenu(user, manager);
    }
    
    for (size_t i = 0; i < userRez.size(); ++i) {
        cout << i + 1 << ". Reservation date/time: " << asctime(&userRez[i].dateTime) << endl;
    }

    cout << "Enter the number of the reservation you would like to change (1-" << userRez.size() << "): ";
    cin >> choice;
    while (choice < 1 || choice > static_cast<int>(userRez.size())) {
        cout << "Invalid choice. Please enter a number between 1 and " << userRez.size() << ": ";
        cin >> choice;
    }

    choice--;
    for (auto it = reservations.begin(); it != reservations.end(); ++it) {
        if ((*it).dateTime.tm_mon == userRez[choice].dateTime.tm_mon &&
            (*it).dateTime.tm_mday == userRez[choice].dateTime.tm_mday &&
            (*it).dateTime.tm_hour == userRez[choice].dateTime.tm_hour) {
            reservations.erase(it);
            break;
        }
    }

    ofstream outFile("reservations.csv");
    for (auto& rez : reservations) {
        outFile << rez.id << "," << rez.group << "," << rez.dateTime.tm_mon + 1 << "," << rez.dateTime.tm_mday << ","
                << rez.dateTime.tm_year + 1900 << "," << rez.dateTime.tm_hour << "," << rez.dateTime.tm_min << "," << rez.length 
                << "," << rez.guestCount << "," << rez.layout << "," << rez.isPrivate << "," << rez.isPaid << "\n";
    }
    outFile.close();
    
    cout << "Reservation deleted. Please make a new reservation with the desired changes." << endl;
    Reservation newReservation(user);
    newReservation.makeReservation(*this);
}

void ReservationManager::viewUserReservations(SystemUser &user) {
    bool hasReservations = false;

    for (auto& rez : reservations) {
        if (rez.id == user.userID) {
            hasReservations = true;
            cout << "Reservation date/time: " << asctime(&rez.dateTime)
                 << ", Length: " << rez.length << ", Guests: " << rez.guestCount
                 << ", Layout: " << rez.layout << ", Private: " << (rez.isPrivate ? "Yes" : "No")
                 << ", Paid: " << (rez.isPaid ? "Yes" : "No") << endl;
        }
    }

    if (!hasReservations) {
        cout << "You have no reservations." << endl;
        cout << "Returning to the main menu." << endl;
        mainMenu(user, manager);
    }
}

void mainMenu(SystemUser &user, ReservationManager &mgr) {
    int choice;
    cout << "Welcome!" << endl;
    cout << "1 to make a reservation." << endl;
    cout << "2 to cancel a reservation." << endl;
    cout << "3 to change details of a current reservation." << endl;
    cout << "4 to view your reservations." << endl;
    cout << "5 to view the current schedule." << endl;
    cout << "6 to purchase a ticket for an event." << endl;
    cout << "7 to view events you have purchased tickets for." << endl;
    cin >> choice; 
    while (choice < 1 || choice > 7){
        cout << "Invalid choice. Please enter a number between 1 and 7 corresponding to the options above." << endl;
        cin >> choice;
    }

    switch(choice) {
        case 1: {
            Reservation newReservation(user);
            newReservation.makeReservation(mgr);
            break;
        }
        case 2: {
            mgr.userDeleteReservation(user);
            break;
        }
        case 3: {
            mgr.changeUserReservation(user);
            break;
        }
        case 4: {
            mgr.viewUserReservations(user);
            break;
        }
        case 5: {
            mgr.printSchedule();
            break;
        }
        case 6: {
            mgr.purchaseTicket(user);
            break;
        }
        case 7: {
            mgr.viewUserTickets(user);
            break;
        }
    }
}

int main() {
    vector<SystemUser> users;
    string csvFile = "users.csv";

    // Read existing users from CSV
    ifstream file(csvFile);
    string line;
    while (getline(file, line)) {
        vector<string> tokens = split(line, ',');
        if (tokens.size() == 6) {
            int userID = stoi(tokens[5]);
            users.emplace_back(tokens[0], tokens[1], tokens[2], tokens[3], tokens[4], userID);
        }
    }
    file.close();

    // User login simulation
    string username, password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    // Check if user exists
    auto it = find_if(users.begin(), users.end(), [&](const SystemUser& u) {
        return u.username == username && u.password == password;
    });

    if (it != users.end()) {
        cout << "Welcome back, " << it->firstName << "!" << endl;
        cout << it->firstName << " " << it->lastName << " " << it->groupName << " " << it->userID << endl;

        // Create a reservation manager instance
        //ReservationManager manager;
        manager.loadReservations("reservations.csv");

        // Display main menu
        mainMenu(*it, manager);

    } else {
        // Adding new user
        string firstName, lastName, groupName;
        int userID;

        cout << "New user detected. Please enter the following details." << endl;
        cout << "First Name: ";
        cin >> firstName;
        cout << "Last Name: ";
        cin >> lastName;
        cout << "Group [resident, non-resident, organization, city]: ";
        cin >> groupName;
        while (groupName != "resident" && groupName != "non-resident" && groupName != "organization" && groupName != "city") {
            cout << "Invalid group name. Please enter one of the following [resident, non-resident, organization, city]: ";
            cin >> groupName;
        }
        cout << "User ID [any int]: ";
        cin >> userID;

        SystemUser newUser(firstName, lastName, groupName, username, password, userID);
        users.push_back(newUser);

        // Append new user to CSV file
        ofstream outFile(csvFile, ios::app);
        outFile << firstName << "," << lastName << "," << groupName << "," << username << "," << password << "," << userID << "\n";
        outFile.close();

        cout << "New user registered successfully!" << endl;

        // Create a reservation manager instance
        ReservationManager manager;
        manager.loadReservations("reservations.csv");

        // Display main menu
        mainMenu(newUser, manager);
    }
    
    return 0;
}

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <unistd.h> // For sleep function
using namespace std;

// Structure to store booking details
struct Booking {
    int bookingNo;
    string clientName;
    long long phoneNo;
    string eventDate;
    string location;
    float duration;
    float amount;
};

// Structure for Photography Data
struct PhotoData {
    int serialNo;
    string type;
    string size;
    string frame;
    string orientation;
    float price;
};

// Function to create a new booking
void booking() {
    ofstream outFile("STUDIO.dat", ios::app | ios::binary);
    Booking b;
    char ch = 'y';

    while (ch == 'y') {
        cout << "Enter Booking Number: ";
        cin >> b.bookingNo;
        cout << "Enter Client Name: ";
        cin.ignore();
        getline(cin, b.clientName);
        cout << "Enter Phone Number: ";
        cin >> b.phoneNo;
        cout << "Enter Date of Event: ";
        cin >> b.eventDate;
        cout << "Enter Location of Event: ";
        cin.ignore();
        getline(cin, b.location);
        cout << "Enter Duration (hrs): ";
        cin >> b.duration;
        cout << "Enter Amount: ";
        cin >> b.amount;

        outFile.write(reinterpret_cast<char*>(&b), sizeof(b));
        cout << "Details stored successfully!\n";
        cout << "Do you want to book another? (y/n): ";
        cin >> ch;
    }

    outFile.close();
}

// Function to display bookings
void displayBookings() {
    ifstream inFile("STUDIO.dat", ios::binary);
    Booking b;

    cout << "====================================================================================\n";
    cout << "BNo\tClient Name\tPhone\tDate\tLocation\tDuration\tAmount\n";
    cout << "=====================================================================================\n";

    while (inFile.read(reinterpret_cast<char*>(&b), sizeof(b))) {
        cout << b.bookingNo << "\t" << b.clientName << "\t" << b.phoneNo << "\t"
             << b.eventDate << "\t" << b.location << "\t" << b.duration << "\t" << b.amount << "\n";
    }

    inFile.close();
}

// Function to cancel a booking
void cancelBooking() {
    ifstream inFile("STUDIO.dat", ios::binary);
    ofstream outFile("Temp.dat", ios::binary);
    Booking b;
    int delNo;
    bool found = false;

    cout << "Enter Booking Number to Delete: ";
    cin >> delNo;

    while (inFile.read(reinterpret_cast<char*>(&b), sizeof(b))) {
        if (b.bookingNo != delNo) {
            outFile.write(reinterpret_cast<char*>(&b), sizeof(b));
        } else {
            found = true;
        }
    }

    inFile.close();
    outFile.close();
    remove("STUDIO.dat");
    rename("Temp.dat", "STUDIO.dat");

    if (found)
        cout << "Booking deleted successfully!\n";
    else
        cout << "Booking number not found!\n";
}

// Function to modify booking details
void modifyBooking() {
    fstream file("STUDIO.dat", ios::binary | ios::in | ios::out);
    Booking b;
    int modNo;
    bool found = false;

    cout << "Enter Booking Number to Modify: ";
    cin >> modNo;

    while (file.read(reinterpret_cast<char*>(&b), sizeof(b))) {
        if (b.bookingNo == modNo) {
            found = true;
            cout << "Enter New Client Name: ";
            cin.ignore();
            getline(cin, b.clientName);
            cout << "Enter New Phone Number: ";
            cin >> b.phoneNo;
            cout << "Enter New Date of Event: ";
            cin >> b.eventDate;
            cout << "Enter New Location: ";
            cin.ignore();
            getline(cin, b.location);
            cout << "Enter New Duration: ";
            cin >> b.duration;
            cout << "Enter New Amount: ";
            cin >> b.amount;

            file.seekp(-static_cast<int>(sizeof(b)), ios::cur);
            file.write(reinterpret_cast<char*>(&b), sizeof(b));
            cout << "Booking updated successfully!\n";
            break;
        }
    }

    if (!found)
        cout << "Booking number not found!\n";

    file.close();
}

// Function to calculate settlement
void settleBooking() {
    ifstream inFile("STUDIO.dat", ios::binary);
    Booking b;
    int settleNo;
    bool found = false;

    cout << "Enter Booking Number to Settle: ";
    cin >> settleNo;

    while (inFile.read(reinterpret_cast<char*>(&b), sizeof(b))) {
        if (b.bookingNo == settleNo) {
            found = true;
            cout << "Booking Found: \n";
            cout << "Amount to be paid (with 18% GST): " << b.amount + (b.amount * 0.18) << "\n";
            break;
        }
    }

    if (!found)
        cout << "Booking number not found!\n";

    inFile.close();
}

// Function to display Photography Data
void displayPhotoData() {
    ifstream inFile("photo.dat", ios::binary);
    PhotoData p;

    cout << "=========================================================\n";
    cout << "Si.No\tType\tSize\tFrame\tOrientation\tPrice\n";
    cout << "=========================================================\n";

    while (inFile.read(reinterpret_cast<char*>(&p), sizeof(p))) {
        cout << p.serialNo << "\t" << p.type << "\t" << p.size << "\t" 
             << p.frame << "\t" << p.orientation << "\t" << p.price << "\n";
    }

    inFile.close();
}

// Function for Videography Pricing
void displayVideoData() {
    cout << "Rs. 25000/hr (GST 18% Excluded)\n";
}

// Main menu
void mainMenu() {
    int choice;
    string password;
    char cont = 'y';

    while (cont == 'y') {
        cout << "\n1 - MANAGEMENT\n2 - CLIENT\nChoose user type: ";
        cin >> choice;

        if (choice == 1) {
            cout << "Enter Password: ";
            cin >> password;
            if (password == "HHSTUDIO") {
                int mgmtChoice;
                do {
                    cout << "\n1. New Registration\n2. Modify\n3. Cancel\n4. Settlement\n5. Display\n6. Exit\nChoose an option: ";
                    cin >> mgmtChoice;

                    switch (mgmtChoice) {
                        case 1: booking(); break;
                        case 2: modifyBooking(); break;
                        case 3: cancelBooking(); break;
                        case 4: settleBooking(); break;
                        case 5: displayBookings(); break;
                        case 6: return;
                        default: cout << "Invalid option!\n";
                    }
                } while (mgmtChoice != 6);
            } else {
                cout << "Wrong Password!\n";
            }
        } else if (choice == 2) {
            int clientChoice;
            do {
                cout << "\n1. Photography\n2. Videography\n3. Back\nChoose an option: ";
                cin >> clientChoice;

                switch (clientChoice) {
                    case 1: displayPhotoData(); break;
                    case 2: displayVideoData(); break;
                    case 3: return;
                    default: cout << "Invalid option!\n";
                }
            } while (clientChoice != 3);
        }

        cout << "\nDo you want to continue? (y/n): ";
        cin >> cont;
    }
}

int main() {
    mainMenu();
    return 0;
}
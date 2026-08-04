#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <iomanip>
#include <cstddef> // For size_t

using namespace std;

// ==========================================
//         CONSTANTS & CONFIGURATION
// ==========================================

const string FILENAME = "users.txt";
const string DELIMITER = "|";
const size_t MIN_USERNAME_LENGTH = 4;
const size_t MIN_PASSWORD_LENGTH = 6;

// ==========================================
//              USER CLASS
// ==========================================

class User
{
private:
    string username;
    string password;

public:
    // Default constructor
    User()
    {
        username = "";
        password = "";
    }

    // Parameterized constructor
    User(string uname, string pwd)
    {
        username = uname;
        password = pwd;
    }

    // Setters
    void setUsername(string uname)
    {
        username = uname;
    }

    void setPassword(string pwd)
    {
        password = pwd;
    }

    // Getters (marked as const to fix warnings)
    string getUsername() const
    {
        return username;
    }

    string getPassword() const
    {
        return password;
    }

    // Convert to file format: username|password
    string toFileString() const
    {
        return username + DELIMITER + password;
    }
};

// ==========================================
//         AUTHENTICATION MANAGER
// ==========================================

class AuthManager
{
private:
    // Count total registered users
    int getUserCount() const
    {
        ifstream file(FILENAME);
        int count = 0;
        string line;

        while (getline(file, line))
        {
            if (!line.empty())
            {
                count++;
            }
        }

        file.close();
        return count;
    }

    // Check if username already exists in file
    bool isUsernameTaken(string username) const
    {
        ifstream file(FILENAME);
        string line;

        while (getline(file, line))
        {
            size_t pos = line.find(DELIMITER);
            if (pos != string::npos)
            {
                string storedUsername = line.substr(0, pos);
                if (storedUsername == username)
                {
                    file.close();
                    return true; // Username found
                }
            }
        }

        file.close();
        return false; // Username not found
    }

    // Save new user to file
    bool saveUserToFile(User user)
    {
        ofstream file(FILENAME, ios::app); // Append mode

        if (file.is_open())
        {
            file << user.toFileString() << endl;
            file.close();
            return true;
        }

        return false;
    }

    // Find user in file and return credentials
    bool findUser(string username, string password) const
    {
        ifstream file(FILENAME);
        string line;

        while (getline(file, line))
        {
            size_t pos = line.find(DELIMITER);
            if (pos != string::npos)
            {
                string storedUsername = line.substr(0, pos);
                string storedPassword = line.substr(pos + 1);

                if (storedUsername == username && storedPassword == password)
                {
                    file.close();
                    return true; // Match found
                }
            }
        }

        file.close();
        return false; // No match
    }

    // Update password in file
    bool updatePassword(string username, string newPassword)
    {
        ifstream readFile(FILENAME);
        ofstream tempFile("temp.txt");

        bool found = false;
        string line;

        while (getline(readFile, line))
        {
            size_t pos = line.find(DELIMITER);
            if (pos != string::npos)
            {
                string storedUsername = line.substr(0, pos);

                if (storedUsername == username)
                {
                    tempFile << username << DELIMITER << newPassword << endl;
                    found = true;
                }
                else
                {
                    tempFile << line << endl;
                }
            }
        }

        readFile.close();
        tempFile.close();

        if (found)
        {
            remove(FILENAME.c_str());
            rename("temp.txt", FILENAME.c_str());
        }
        else
        {
            remove("temp.txt");
        }

        return found;
    }

public:
    // Display welcome banner
    void showBanner() const
    {
        cout << endl;
        cout << "============================================" << endl;
        cout << "     LOGIN & REGISTRATION SYSTEM            " << endl;
        cout << "     Developed for CodeAlpha Internship     " << endl;
        cout << "============================================" << endl;
        cout << "     Registered Users: " << getUserCount() << endl;
        cout << "============================================" << endl;
    }

    // ==========================================
    //          REGISTRATION FUNCTION
    // ==========================================

    void registerUser()
    {
        string username, password, confirmPassword;

        cout << endl;
        cout << "--------------------------------------------" << endl;
        cout << "              REGISTRATION                   " << endl;
        cout << "--------------------------------------------" << endl;

        // Get username
        cout << endl;
        cout << "Enter username (min " << MIN_USERNAME_LENGTH << " chars): ";
        getline(cin, username);

        // Validate username length
        while (username.length() < MIN_USERNAME_LENGTH)
        {
            cout << "[ERROR] Username must be at least " << MIN_USERNAME_LENGTH << " characters!" << endl;
            cout << "Enter username again: ";
            getline(cin, username);
        }

        // Validate username characters (alphanumeric only)
        bool validChars = true;
        for (size_t i = 0; i < username.length(); i++)
        {
            if (!isalnum(username[i]) && username[i] != '_')
            {
                validChars = false;
                break;
            }
        }

        while (!validChars)
        {
            cout << "[ERROR] Username can only contain letters, numbers, and underscores!" << endl;
            cout << "Enter username again: ";
            getline(cin, username);
            validChars = true;
            for (size_t i = 0; i < username.length(); i++)
            {
                if (!isalnum(username[i]) && username[i] != '_')
                {
                    validChars = false;
                    break;
                }
            }
        }

        // Check for duplicate username
        if (isUsernameTaken(username))
        {
            cout << endl;
            cout << "[ERROR] Username '" << username << "' is already taken!" << endl;
            cout << "Please choose a different username." << endl;
            return;
        }

        // Get password
        cout << "Enter password (min " << MIN_PASSWORD_LENGTH << " chars): ";
        getline(cin, password);

        // Validate password length
        while (password.length() < MIN_PASSWORD_LENGTH)
        {
            cout << "[ERROR] Password must be at least " << MIN_PASSWORD_LENGTH << " characters!" << endl;
            cout << "Enter password again: ";
            getline(cin, password);
        }

        // Confirm password
        cout << "Confirm password: ";
        getline(cin, confirmPassword);

        // Check if passwords match
        int attempts = 0;
        while (password != confirmPassword && attempts < 3)
        {
            attempts++;
            cout << endl;
            cout << "[ERROR] Passwords do not match! (Attempt " << attempts << "/3)" << endl;
            cout << "Enter password again: ";
            getline(cin, password);
            cout << "Confirm password: ";
            getline(cin, confirmPassword);
        }

        if (password != confirmPassword)
        {
            cout << endl;
            cout << "[ERROR] Too many failed attempts. Registration cancelled." << endl;
            return;
        }

        // Create user object and save
        User newUser(username, password);

        if (saveUserToFile(newUser))
        {
            cout << endl;
            cout << "============================================" << endl;
            cout << " [SUCCESS] Registration Complete!            " << endl;
            cout << "============================================" << endl;
            cout << " Username: " << username << endl;
            cout << " You can now login with your credentials.   " << endl;
            cout << "============================================" << endl;
        }
        else
        {
            cout << endl;
            cout << "[ERROR] Failed to save user data!" << endl;
        }
    }

    // ==========================================
    //            LOGIN FUNCTION
    // ==========================================

    bool loginUser()
    {
        string username, password;

        cout << endl;
        cout << "--------------------------------------------" << endl;
        cout << "                  LOGIN                      " << endl;
        cout << "--------------------------------------------" << endl;

        // Get username
        cout << endl;
        cout << "Enter username: ";
        getline(cin, username);

        // Get password
        cout << "Enter password: ";
        getline(cin, password);

        // Verify credentials
        if (findUser(username, password))
        {
            cout << endl;
            cout << "============================================" << endl;
            cout << " [SUCCESS] Login Successful!                " << endl;
            cout << "============================================" << endl;
            cout << " Welcome back, " << username << "!" << endl;
            cout << "============================================" << endl;
            return true;
        }
        else
        {
            cout << endl;
            cout << "[ERROR] Login Failed!" << endl;
            cout << "Invalid username or password." << endl;
            return false;
        }
    }

    // ==========================================
    //          USER DASHBOARD (After Login)
    // ==========================================

    void showDashboard(string username)
    {
        int choice;

        while (true)
        {
            cout << endl;
            cout << "--------------------------------------------" << endl;
            cout << "         USER DASHBOARD: " << username << endl;
            cout << "--------------------------------------------" << endl;
            cout << "  1. View Profile" << endl;
            cout << "  2. Change Password" << endl;
            cout << "  3. Logout" << endl;
            cout << "--------------------------------------------" << endl;
            cout << " Enter choice (1-3): ";

            while (!(cin >> choice) || choice < 1 || choice > 3)
            {
                cout << " [ERROR] Invalid choice! Enter 1, 2, or 3: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            cin.ignore();

            cout << endl;

            switch (choice)
            {
            case 1:
                cout << "============================================" << endl;
                cout << "             USER PROFILE                   " << endl;
                cout << "============================================" << endl;
                cout << " Username: " << username << endl;
                cout << " Status: Active" << endl;
                cout << "============================================" << endl;
                break;

            case 2:
                changePassword(username);
                break;

            case 3:
                cout << "[INFO] Logged out successfully." << endl;
                return;
            }
        }
    }

    // ==========================================
    //         CHANGE PASSWORD FUNCTION
    // ==========================================

    void changePassword(string username)
    {
        string currentPassword, newPassword, confirmPassword;

        cout << "--------------------------------------------" << endl;
        cout << "           CHANGE PASSWORD                   " << endl;
        cout << "--------------------------------------------" << endl;

        // Get current password
        cout << endl;
        cout << "Enter current password: ";
        getline(cin, currentPassword);

        // Verify current password
        if (!findUser(username, currentPassword))
        {
            cout << endl;
            cout << "[ERROR] Current password is incorrect!" << endl;
            return;
        }

        // Get new password
        cout << "Enter new password (min " << MIN_PASSWORD_LENGTH << " chars): ";
        getline(cin, newPassword);

        while (newPassword.length() < MIN_PASSWORD_LENGTH)
        {
            cout << "[ERROR] Password must be at least " << MIN_PASSWORD_LENGTH << " characters!" << endl;
            cout << "Enter new password again: ";
            getline(cin, newPassword);
        }

        // Confirm new password
        cout << "Confirm new password: ";
        getline(cin, confirmPassword);

        if (newPassword != confirmPassword)
        {
            cout << endl;
            cout << "[ERROR] Passwords do not match!" << endl;
            return;
        }

        // Update password
        if (updatePassword(username, newPassword))
        {
            cout << endl;
            cout << "============================================" << endl;
            cout << " [SUCCESS] Password Changed Successfully! " << endl;
            cout << "============================================" << endl;
        }
        else
        {
            cout << endl;
            cout << "[ERROR] Failed to change password!" << endl;
        }
    }

    // ==========================================
    //         VIEW ALL USERS (Admin Feature)
    // ==========================================

    void viewAllUsers() const
    {
        cout << endl;
        cout << "--------------------------------------------" << endl;
        cout << "           REGISTERED USERS                " << endl;
        cout << "--------------------------------------------" << endl;

        ifstream file(FILENAME);
        string line;
        int count = 0;

        cout << endl;
        cout << setw(5) << "#"
            << setw(20) << "Username"
            << "Status" << endl;

        cout << "------------------------------" << endl;

        while (getline(file, line))
        {
            if (!line.empty())
            {
                count++;
                size_t pos = line.find(DELIMITER);
                if (pos != string::npos)
                {
                    string username = line.substr(0, pos);
                    cout << setw(5) << count
                        << setw(20) << username
                        << "Active" << endl;
                }
            }
        }

        file.close();

        if (count == 0)
        {
            cout << " No users registered yet." << endl;
        }

        cout << "------------------------------" << endl;
        cout << " Total Users: " << count << endl;
    }
};

// ==========================================
//              MAIN FUNCTION
// ==========================================

int main()
{
    AuthManager auth;
    int choice;
    string loggedInUser = "";

    // Show welcome banner
    auth.showBanner();

    // Main loop
    while (true)
    {
        cout << endl;
        cout << "--------------------------------------------" << endl;
        cout << "              MAIN MENU                     " << endl;
        cout << "--------------------------------------------" << endl;
        cout << "  1. Register" << endl;
        cout << "  2. Login" << endl;
        cout << "  3. View Registered Users" << endl;
        cout << "  4. Exit" << endl;
        cout << "--------------------------------------------" << endl;
        cout << " Enter choice (1-4): ";

        // Input validation
        while (!(cin >> choice) || choice < 1 || choice > 4)
        {
            cout << " [ERROR] Invalid choice! Enter 1, 2, 3, or 4: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cin.ignore(); // Clear buffer after cin >>

        cout << endl;

        switch (choice)
        {
        case 1:
            auth.registerUser();
            break;

        case 2:
            if (auth.loginUser())
            {
                // Ask for username again for dashboard
                cout << endl;
                cout << "Enter your username for dashboard: ";
                getline(cin, loggedInUser);
                auth.showDashboard(loggedInUser);
            }
            break;

        case 3:
            auth.viewAllUsers();
            break;

        case 4:
            cout << "============================================" << endl;
            cout << "        Thank you for using the system!    " << endl;
            cout << "              Goodbye!                      " << endl;
            cout << "============================================" << endl;
            return 0;
        }
    }

    return 0;
}

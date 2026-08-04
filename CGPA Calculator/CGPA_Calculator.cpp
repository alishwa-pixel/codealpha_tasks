#include <iostream>
#include <string>
#include <iomanip>
#include <limits>

using namespace std;

// Function to convert grade letter to grade points
double getGradePoints(string grade) {
    if (grade == "A+" || grade == "a+") return 4.0;
    if (grade == "A" || grade == "a")  return 4.0;
    if (grade == "A-" || grade == "a-") return 3.7;
    if (grade == "B+" || grade == "b+") return 3.3;
    if (grade == "B" || grade == "b")  return 3.0;
    if (grade == "B-" || grade == "b-") return 2.7;
    if (grade == "C+" || grade == "c+") return 2.3;
    if (grade == "C" || grade == "c")  return 2.0;
    if (grade == "C-" || grade == "c-") return 1.7;
    if (grade == "D+" || grade == "d+") return 1.3;
    if (grade == "D" || grade == "d")  return 1.0;
    if (grade == "F" || grade == "f")  return 0.0;
    return -1; // Invalid grade
}

// Function to validate grade input
bool isValidGrade(string grade) {
    return getGradePoints(grade) != -1;
}

int main() {
    int numCourses;
    string* courseNames = nullptr;
    string* grades = nullptr;
    int* creditHours = nullptr;
    double* gradePoints = nullptr;

    cout << "========================================" << endl;
    cout << "       CGPA CALCULATOR                 " << endl;
    cout << "========================================" << endl;
    cout << endl;

    // Get number of courses
    cout << "Enter the number of courses: ";
    while (!(cin >> numCourses) || numCourses <= 0) {
        cout << "Invalid input. Please enter a positive number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(); // Clear the newline character

    // Allocate dynamic arrays
    courseNames = new string[numCourses];
    grades = new string[numCourses];
    creditHours = new int[numCourses];
    gradePoints = new double[numCourses];

    cout << endl;
    cout << "----------------------------------------" << endl;
    cout << "  ENTER COURSE DETAILS                 " << endl;
    cout << "----------------------------------------" << endl;

    // Input course details
    for (int i = 0; i < numCourses; i++) {
        cout << endl;
        cout << "Course " << (i + 1) << " of " << numCourses << ":" << endl;

        // Course name
        cout << "  Course Name: ";
        getline(cin, courseNames[i]);

        // Credit hours
        cout << "  Credit Hours: ";
        while (!(cin >> creditHours[i]) || creditHours[i] <= 0) {
            cout << "  Invalid input. Enter a positive number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cin.ignore();

        // Grade
        cout << "  Grade (A+, A, A-, B+, B, B-, C+, C, C-, D+, D, F): ";
        getline(cin, grades[i]);

        // Validate grade
        while (!isValidGrade(grades[i])) {
            cout << "  Invalid grade! Enter again (A+, A, A-, B+, B, B-, C+, C, C-, D+, D, F): ";
            getline(cin, grades[i]);
        }

        // Calculate grade points for this course
        gradePoints[i] = getGradePoints(grades[i]) * creditHours[i];
    }

    // Calculate totals
    double totalGradePoints = 0;
    int totalCreditHours = 0;

    for (int i = 0; i < numCourses; i++) {
        totalGradePoints += gradePoints[i];
        totalCreditHours += creditHours[i];
    }

    // Calculate GPA (semester)
    double gpa = totalGradePoints / totalCreditHours;

    // Display results
    cout << endl;
    cout << "========================================" << endl;
    cout << "         SEMESTER RESULT                " << endl;
    cout << "========================================" << endl;
    cout << endl;

    // Table header
    cout << left << setw(5) << "#"
        << left << setw(25) << "Course Name"
        << left << setw(12) << "Credit Hrs"
        << left << setw(8) << "Grade"
        << left << setw(15) << "Grade Points" << endl;

    cout << string(65, '-') << endl;

    // Table rows
    for (int i = 0; i < numCourses; i++) {
        cout << left << setw(5) << (i + 1)
            << left << setw(25) << courseNames[i]
            << left << setw(12) << creditHours[i]
            << left << setw(8) << grades[i]
            << left << setw(15) << fixed << setprecision(2) << gradePoints[i] << endl;
    }

    cout << string(65, '-') << endl;

    // Totals row
    cout << left << setw(30) << "TOTAL"
        << left << setw(12) << totalCreditHours
        << left << setw(8) << ""
        << left << setw(15) << fixed << setprecision(2) << totalGradePoints << endl;

    cout << "========================================" << endl;
    cout << endl;

    // Final GPA display
    cout << "  SEMESTER GPA: " << fixed << setprecision(2) << gpa << " / 4.00" << endl;
    cout << endl;

    // CGPA calculation (if multiple semesters)
    int numSemesters;
    cout << "----------------------------------------" << endl;
    cout << "  CGPA CALCULATION                     " << endl;
    cout << "----------------------------------------" << endl;
    cout << endl;
    cout << "How many previous semesters do you have GPA for? ";
    cin >> numSemesters;
    cin.ignore();

    if (numSemesters > 0) {
        double* previousGPAs = new double[numSemesters];
        int* previousCredits = new int[numSemesters];

        for (int i = 0; i < numSemesters; i++) {
            cout << "  Semester " << (i + 1) << " GPA: ";
            while (!(cin >> previousGPAs[i]) || previousGPAs[i] < 0 || previousGPAs[i] > 4.0) {
                cout << "  Invalid GPA. Enter between 0.00 and 4.00: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }

            cout << "  Semester " << (i + 1) << " Total Credit Hours: ";
            while (!(cin >> previousCredits[i]) || previousCredits[i] <= 0) {
                cout << "  Invalid input. Enter a positive number: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            cin.ignore();
        }

        // Calculate CGPA
        double cgpaTotalPoints = totalGradePoints;
        int cgpaTotalCredits = totalCreditHours;

        for (int i = 0; i < numSemesters; i++) {
            cgpaTotalPoints += (previousGPAs[i] * previousCredits[i]);
            cgpaTotalCredits += previousCredits[i];
        }

        double cgpa = cgpaTotalPoints / cgpaTotalCredits;

        cout << endl;
        cout << "========================================" << endl;
        cout << "  OVERALL CGPA: " << fixed << setprecision(2) << cgpa << " / 4.00" << endl;
        cout << "  Total Semesters: " << (numSemesters + 1) << endl;
        cout << "  Total Credit Hours Completed: " << cgpaTotalCredits << endl;
        cout << "========================================" << endl;

        delete[] previousGPAs;
        delete[] previousCredits;
    }
    else {
        cout << endl;
        cout << "  No previous semesters entered." << endl;
        cout << "  Your CGPA is the same as your GPA: " << fixed << setprecision(2) << gpa << " / 4.00" << endl;
        cout << "========================================" << endl;
    }

    // Free memory
    delete[] courseNames;
    delete[] grades;
    delete[] creditHours;
    delete[] gradePoints;

    cout << endl;
    cout << "Press Enter to exit...";
    cin.get();

    return 0;
}

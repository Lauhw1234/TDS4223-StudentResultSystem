#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <iomanip>
using namespace std;

const int MAX_NAME_LEN      = 50;
const int MAX_ID_LEN        = 15;
const int MAX_PASS_LEN      = 20;
const int MAX_COURSE_LEN    = 40;
const int MAX_STUDENTS      = 200;
const int MAX_STAFF         = 50;
const int MAX_COURSES       = 100;
const int MAX_RESULTS       = 300;
const int MAX_QUEUE_LOG     = 200;

const char STUDENT_FILE[]   = "students.txt";
const char STAFF_FILE[]     = "staff.txt";
const char COURSE_FILE[]    = "courses.txt";
const char RESULT_FILE[]    = "results.txt";

struct Date {
    int day, month, year;
    Date() { day = 1; month = 1; year = 2026; }
    Date(int d, int m, int y) { day = d; month = m; year = y; }
    string toString() const { stringstream ss; if (day < 10) ss << "0"; ss << day << "/"; if (month < 10) ss << "0"; ss << month << "/"; ss << year; return ss.str(); }
};

struct ContactInfo {
    char phone[20], email[60];
    ContactInfo() { strcpy(phone, "N/A"); strcpy(email, "N/A"); }
    ContactInfo(const char* p, const char* e) { strcpy(phone, p); strcpy(email, e); }
};

class Person {
protected:
    char id[MAX_ID_LEN], name[MAX_NAME_LEN], password[MAX_PASS_LEN]; ContactInfo contact; Date registeredOn;
public:
    Person() { strcpy(id, ""); strcpy(name, ""); strcpy(password, ""); }
    Person(const char* pid, const char* pname, const char* ppass) { strcpy(id, pid); strcpy(name, pname); strcpy(password, ppass); }
    virtual ~Person() {}
    void setId(const char* pid) { strcpy(id, pid); }
    void setName(const char* pname) { strcpy(name, pname); }
    void setPassword(const char* pw) { strcpy(password, pw); }
    void setContact(ContactInfo c) { contact = c; }
    string getId() const { return string(id); }
    string getName() const { return string(name); }
    string getPassword() const { return string(password); }
    bool checkPassword(const string &attempt) const { return attempt == string(password); }
    virtual void displayProfile() const { cout << "ID: " << id << " | Name: " << name << endl; }
};

class Record {
protected:
    int recordId; Date createdOn; bool active;
public:
    Record() { recordId = 0; active = true; }
    Record(int rid) { recordId = rid; active = true; }
    virtual ~Record() {}
    int getRecordId() const { return recordId; }
    void setRecordId(int rid) { recordId = rid; }
    bool isActive() const { return active; }
    void setActive(bool a) { active = a; }
    virtual void display() const { cout << "Record #" << recordId << endl; }
};

class Student : public Person {
private:
    char programme[MAX_COURSE_LEN]; float gpa; int totalCredits;
public:
    Student() : Person() { strcpy(programme, "Undeclared"); gpa = 0.0f; totalCredits = 0; }
    Student(const char* pid, const char* pname, const char* ppass, const char* prog) : Person(pid, pname, ppass) { strcpy(programme, prog); gpa = 0.0f; totalCredits = 0; }
    void setProgramme(const char* p) { strcpy(programme, p); }
    void setGpa(float g) { gpa = g; }
    void setTotalCredits(int c) { totalCredits = c; }
    string getProgramme() const { return string(programme); }
    float getGpa() const { return gpa; }
    int getTotalCredits() const { return totalCredits; }
    string toFileLine() const { stringstream ss; ss << id << "|" << name << "|" << password << "|" << programme << "|" << gpa << "|" << totalCredits << "|" << contact.phone << "|" << contact.email << "|" << registeredOn.toString(); return ss.str(); }
    static Student fromFileLine(const string &line) {
        Student s; char buf[300]; strcpy(buf, line.c_str()); char* tok = strtok(buf, "|"); int field = 0; char ph[20] = "N/A", em[60] = "N/A";
        while (tok != NULL) { switch (field) { case 0: s.setId(tok); break; case 1: s.setName(tok); break; case 2: s.setPassword(tok); break; case 3: s.setProgramme(tok); break; case 4: s.setGpa((float)atof(tok)); break; case 5: s.setTotalCredits(atoi(tok)); break; case 6: strcpy(ph, tok); break; case 7: strcpy(em, tok); break; } tok = strtok(NULL, "|"); field++; }
        s.setContact(ContactInfo(ph, em)); return s;
    }
};

class Staff : public Person {
private:
    char department[MAX_COURSE_LEN], position[30];
public:
    Staff() : Person() { strcpy(department, "General"); strcpy(position, "Lecturer"); }
    Staff(const char* pid, const char* pname, const char* ppass, const char* dept, const char* pos) : Person(pid, pname, ppass) { strcpy(department, dept); strcpy(position, pos); }
    string toFileLine() const { stringstream ss; ss << id << "|" << name << "|" << password << "|" << department << "|" << position << "|" << contact.phone << "|" << contact.email; return ss.str(); }
    static Staff fromFileLine(const string &line) {
        Staff s; char buf[300]; strcpy(buf, line.c_str()); char* tok = strtok(buf, "|"); int field = 0; char ph[20] = "N/A", em[60] = "N/A";
        while (tok != NULL) { switch (field) { case 0: s.setId(tok); break; case 1: s.setName(tok); break; case 2: s.setPassword(tok); break; case 3: strcpy(s.department, tok); break; case 4: strcpy(s.position, tok); break; case 5: strcpy(ph, tok); break; case 6: strcpy(em, tok); break; } tok = strtok(NULL, "|"); field++; }
        s.setContact(ContactInfo(ph, em)); return s;
    }
};

class CourseResult : public Record {
private:
    char studentId[MAX_ID_LEN], courseCode[15], courseName[MAX_COURSE_LEN]; float marks; char grade[3]; int creditHours;
public:
    CourseResult() : Record() { strcpy(studentId, ""); strcpy(courseCode, ""); strcpy(courseName, ""); marks = 0.0f; strcpy(grade, "-"); creditHours = 3; }
    CourseResult(int rid, const char* sid, const char* ccode, const char* cname, float m, int credits) : Record(rid) { strcpy(studentId, sid); strcpy(courseCode, ccode); strcpy(courseName, cname); marks = m; creditHours = credits; computeGrade(); }
    string getStudentId() const { return string(studentId); }
    string getCourseCode() const { return string(courseCode); }
    string getCourseName() const { return string(courseName); }
    float getMarks() const { return marks; }
    string getGrade() const { return string(grade); }
    int getCreditHours() const { return creditHours; }
    void setMarks(float m) { marks = m; computeGrade(); }
    void computeGrade() {
        if (marks >= 80) strcpy(grade, "A"); else if (marks >= 75) strcpy(grade, "A-"); else if (marks >= 70) strcpy(grade, "B+"); else if (marks >= 65) strcpy(grade, "B"); else if (marks >= 60) strcpy(grade, "B-"); else if (marks >= 55) strcpy(grade, "C+"); else if (marks >= 50) strcpy(grade, "C"); else if (marks >= 45) strcpy(grade, "D"); else if (marks >= 40) strcpy(grade, "E"); else strcpy(grade, "F");
    }
    float gradePoint() const {
        if (strcmp(grade, "A") == 0) return 4.0f; if (strcmp(grade, "A-") == 0) return 3.7f; if (strcmp(grade, "B+") == 0) return 3.3f; if (strcmp(grade, "B") == 0) return 3.0f; if (strcmp(grade, "B-") == 0) return 2.7f; if (strcmp(grade, "C+") == 0) return 2.3f; if (strcmp(grade, "C") == 0) return 2.0f; if (strcmp(grade, "D") == 0) return 1.0f; if (strcmp(grade, "E") == 0) return 0.5f; return 0.0f;
    }
    void display() const { cout << left << setw(5) << recordId << setw(10) << studentId << setw(8) << courseCode << setw(32) << courseName << setw(7) << marks << setw(7) << grade << creditHours << endl; }
    string toFileLine() const { stringstream ss; ss << recordId << "|" << studentId << "|" << courseCode << "|" << courseName << "|" << marks << "|" << grade << "|" << creditHours << "|" << (active ? 1 : 0); return ss.str(); }
    static CourseResult fromFileLine(const string &line) {
        CourseResult r; char buf[300]; strcpy(buf, line.c_str()); char* tok = strtok(buf, "|"); int field = 0;
        while (tok != NULL) { switch (field) { case 0: r.setRecordId(atoi(tok)); break; case 1: strcpy(r.studentId, tok); break; case 2: strcpy(r.courseCode, tok); break; case 3: strcpy(r.courseName, tok); break; case 4: r.marks = (float)atof(tok); break; case 5: strcpy(r.grade, tok); break; case 6: r.creditHours = atoi(tok); break; case 7: r.setActive(atoi(tok) == 1); break; } tok = strtok(NULL, "|"); field++; }
        return r;
    }
};

struct HashNode {
    char studentId[MAX_ID_LEN]; int arrayIndex; HashNode* next;
    HashNode(const char* sid, int idx) { strcpy(studentId, sid); arrayIndex = idx; next = NULL; }
};

class StudentHashTable {
private:
    static const int TABLE_SIZE = 101; HashNode* table[TABLE_SIZE];
    int hashFunction(const string &key) const { unsigned long h = 7; for (size_t i = 0; i < key.size(); i++) h = (h * 37) + (unsigned char)key[i] + i; return (int)(h % TABLE_SIZE); }
public:
    StudentHashTable() { for (int i = 0; i < TABLE_SIZE; i++) table[i] = NULL; }
    ~StudentHashTable() { clear(); }
    void insert(const string &sid, int arrayIndex) { int idx = hashFunction(sid); HashNode* newNode = new HashNode(sid.c_str(), arrayIndex); newNode->next = table[idx]; table[idx] = newNode; }
    int lookup(const string &sid) const { int idx = hashFunction(sid); HashNode* curr = table[idx]; while (curr != NULL) { if (sid == string(curr->studentId)) return curr->arrayIndex; curr = curr->next; } return -1; }
    void clear() { for (int i = 0; i < TABLE_SIZE; i++) { HashNode* curr = table[i]; while (curr != NULL) { HashNode* temp = curr; curr = curr->next; delete temp; } table[i] = NULL; } }
};

struct ActivityNode { char description[150]; Date when; ActivityNode* next; ActivityNode(const char* desc, Date d) { strcpy(description, desc); when = d; next = NULL; } };
class ActivityLogStack {
private: ActivityNode* top; int count;
public: ActivityLogStack() { top = NULL; count = 0; } ~ActivityLogStack() { while (top != NULL) { ActivityNode* temp = top; top = top->next; delete temp; } }
    void push(const string &desc, Date d) { ActivityNode* newNode = new ActivityNode(desc.c_str(), d); newNode->next = top; top = newNode; count++; }
};

class FileException { private: string message; public: FileException(string msg) { message = msg; } string what() const { return message; } };
class ValidationException { private: string message; public: ValidationException(string msg) { message = msg; } string what() const { return message; } };
class NotFoundException { private: string message; public: NotFoundException(string msg) { message = msg; } string what() const { return message; } };

class SystemManager {
private:
    Student students[MAX_STUDENTS]; int studentCount;
    Staff staffs[MAX_STAFF]; int staffCount;
    CourseResult results[MAX_RESULTS]; int resultCount;
    char courseCatalog[MAX_COURSES][MAX_COURSE_LEN], courseCodes[MAX_COURSES][15]; int courseCreditHrs[MAX_COURSES], courseCount;
    StudentHashTable studentIndex; ActivityLogStack activityLog; int nextResultId;
public:
    SystemManager() { studentCount = staffCount = resultCount = courseCount = 0; nextResultId = 1; }
    void loadAll() { loadStudents(); loadStaff(); loadResults(); loadCourses(); rebuildStudentIndex(); }
    void rebuildStudentIndex() { studentIndex.clear(); for (int i = 0; i < studentCount; i++) studentIndex.insert(students[i].getId(), i); }
    void loadStudents() { try { ifstream fin(STUDENT_FILE); if (!fin.is_open()) return; string line; studentCount = 0; while (getline(fin, line) && studentCount < MAX_STUDENTS) { if (line.length() == 0) continue; students[studentCount] = Student::fromFileLine(line); studentCount++; } fin.close(); } catch (...) {} }
    void loadStaff() { try { ifstream fin(STAFF_FILE); if (!fin.is_open()) return; string line; staffCount = 0; while (getline(fin, line) && staffCount < MAX_STAFF) { if (line.length() == 0) continue; staffs[staffCount] = Staff::fromFileLine(line); staffCount++; } fin.close(); } catch (...) {} }
    void loadResults() { try { ifstream fin(RESULT_FILE); if (!fin.is_open()) return; string line; resultCount = 0; int maxId = 0; while (getline(fin, line) && resultCount < MAX_RESULTS) { if (line.length() == 0) continue; results[resultCount] = CourseResult::fromFileLine(line); if (results[resultCount].getRecordId() > maxId) maxId = results[resultCount].getRecordId(); resultCount++; } fin.close(); nextResultId = maxId + 1; } catch (...) {} }
    void loadCourses() { try { ifstream fin(COURSE_FILE); if (!fin.is_open()) return; string line; courseCount = 0; while (getline(fin, line) && courseCount < MAX_COURSES) { if (line.length() == 0) continue; char buf[200]; strcpy(buf, line.c_str()); char* tok = strtok(buf, "|"); int field = 0; while (tok != NULL) { if (field == 0) strcpy(courseCodes[courseCount], tok); else if (field == 1) strcpy(courseCatalog[courseCount], tok); else if (field == 2) courseCreditHrs[courseCount] = atoi(tok); tok = strtok(NULL, "|"); field++; } courseCount++; } fin.close(); } catch (...) {} }
    
    void saveStudents() { ofstream fout(STUDENT_FILE); for (int i = 0; i < studentCount; i++) fout << students[i].toFileLine() << endl; fout.close(); }
    void saveResults() { ofstream fout(RESULT_FILE); for (int i = 0; i < resultCount; i++) fout << results[i].toFileLine() << endl; fout.close(); }
    
    bool isValidMarks(float marks) const { return marks >= 0.0f && marks <= 100.0f; }
    bool studentIdExists(const string &id) const { return studentIndex.lookup(id) != -1; }
    int findCourseIndex(const string &code) const { for (int i = 0; i < courseCount; i++) { if (string(courseCodes[i]) == code) return i; } return -1; }
    void logActivity(const string &desc) { time_t t = time(0); tm* now = localtime(&t); Date today(now->tm_mday, now->tm_mon + 1, now->tm_year + 1900); activityLog.push(desc, today); }

    Staff* getStaffArray() { return staffs; }

    // --- TONG RUI ZE CORE TASKS ---

    int loginStaff(const string &id, const string &pass) {
        for (int i = 0; i < staffCount; i++) {
            if (staffs[i].getId() == id) {
                if (!staffs[i].checkPassword(pass)) return -2; // Incorrect password
                logActivity("Staff login: " + id);
                return i;
            }
        }
        return -1; // Staff ID not found
    }

    bool addResult(const string &studentId, const string &courseCode, float marks) {
        try {
            if (!studentIdExists(studentId)) throw NotFoundException("Student ID not found.");
            int cIdx = findCourseIndex(courseCode);
            if (cIdx == -1) throw NotFoundException("Course code not found.");
            if (!isValidMarks(marks)) throw ValidationException("Marks must be between 0 and 100.");
            if (resultCount >= MAX_RESULTS) throw ValidationException("Result capacity full.");

            CourseResult r(nextResultId, studentId.c_str(), courseCode.c_str(), courseCatalog[cIdx], marks, courseCreditHrs[cIdx]);
            results[resultCount] = r; resultCount++; nextResultId++;
            saveResults(); recalcStudentGpa(studentId);
            logActivity("Result added for student " + studentId + " in course " + courseCode);
            return true;
        } catch (NotFoundException &e) { cout << "[Add Result Failed] " << e.what() << endl; return false;
        } catch (ValidationException &e) { cout << "[Add Result Failed] " << e.what() << endl; return false; }
    }

    bool editResult(int recordId, float newMarks) {
        try {
            int idx = -1;
            for (int i = 0; i < resultCount; i++) { if (results[i].getRecordId() == recordId) { idx = i; break; } }
            if (idx == -1) throw NotFoundException("Result record not found.");
            if (!isValidMarks(newMarks)) throw ValidationException("Marks must be between 0 and 100.");
            
            results[idx].setMarks(newMarks); saveResults(); recalcStudentGpa(results[idx].getStudentId());
            stringstream logss; logss << "Result edited: record #" << recordId << " new marks " << newMarks; logActivity(logss.str());
            return true;
        } catch (NotFoundException &e) { cout << "[Edit Result Failed] " << e.what() << endl; return false;
        } catch (ValidationException &e) { cout << "[Edit Result Failed] " << e.what() << endl; return false; }
    }

    bool deleteResult(int recordId) {
        try {
            int idx = -1;
            for (int i = 0; i < resultCount; i++) { if (results[i].getRecordId() == recordId) { idx = i; break; } }
            if (idx == -1) throw NotFoundException("Result record not found.");
            string sid = results[idx].getStudentId();
            for (int i = idx; i < resultCount - 1; i++) { results[i] = results[i + 1]; }
            resultCount--; saveResults(); recalcStudentGpa(sid);
            logActivity("Result deleted for student " + sid);
            return true;
        } catch (NotFoundException &e) { cout << "[Delete Result Failed] " << e.what() << endl; return false; }
    }

    void recalcStudentGpa(const string &studentId) {
        int sIdx = studentIndex.lookup(studentId);
        if (sIdx == -1) return;
        float totalPoints = 0; int totalCredits = 0;
        for (int i = 0; i < resultCount; i++) {
            if (results[i].getStudentId() == studentId && results[i].isActive()) {
                totalPoints += results[i].gradePoint() * results[i].getCreditHours(); totalCredits += results[i].getCreditHours();
            }
        }
        float gpa = (totalCredits > 0) ? (totalPoints / totalCredits) : 0.0f;
        students[sIdx].setGpa(gpa); students[sIdx].setTotalCredits(totalCredits); saveStudents();
    }
};

class StaffModule {
private:
    SystemManager &mgr; int loggedInStaffIndex;
    string readLine(const string &prompt) { string val; cout << prompt; getline(cin, val); return val; }
    float readFloat(const string &prompt) { float val; cout << prompt; while (!(cin >> val)) { cout << "Invalid input. Please enter a number: "; cin.clear(); cin.ignore(1000, '\n'); } cin.ignore(1000, '\n'); return val; }
    int readInt(const string &prompt) { int val; cout << prompt; while (!(cin >> val)) { cout << "Invalid input. Please enter a whole number: "; cin.clear(); cin.ignore(1000, '\n'); } cin.ignore(1000, '\n'); return val; }
public:
    StaffModule(SystemManager &m) : mgr(m) { loggedInStaffIndex = -1; }

    // --- TONG RUI ZE UI TASKS ---

    bool login() {
        cout << "\n=========== STAFF LOGIN ===========\n";
        string id = readLine("Staff ID: "); string pass = readLine("Password: ");
        int result = mgr.loginStaff(id, pass);
        if (result == -1) { cout << "[Login Failed] Staff ID not found.\n"; return false; }
        if (result == -2) { cout << "[Login Failed] Incorrect password.\n"; return false; }
        loggedInStaffIndex = result; cout << "\nLogin successful. Welcome, " << mgr.getStaffArray()[result].getName() << "!\n"; return true;
    }

    void addResultMenu() {
        cout << "\n--- Add New Result Record ---\n";
        string sid = readLine("Student ID: "); string code = readLine("Course Code: "); float marks = readFloat("Marks (0-100): ");
        if (mgr.addResult(sid, code, marks)) { cout << "Result added successfully! Grade auto-computed.\n"; }
    }

    void editResultMenu() {
        cout << "\n--- Edit Result ---\n";
        int rid = readInt("Enter Result Record ID to edit: "); float newMarks = readFloat("New Marks (0-100): ");
        if (mgr.editResult(rid, newMarks)) { cout << "Result updated successfully! GPA recalculated.\n"; }
    }

    void deleteResultMenu() {
        cout << "\n--- Delete Result ---\n";
        int rid = readInt("Enter Result Record ID to delete: ");
        if (mgr.deleteResult(rid)) { cout << "Result deleted successfully! GPA recalculated.\n"; }
    }

    void logout() { cout << "\nStaff logged out: " << mgr.getStaffArray()[loggedInStaffIndex].getName() << "\n"; loggedInStaffIndex = -1; }

    void run() {
        if (!login()) return;
        int choice = -1;
        while (choice != 0) {
            cout << "\n================ TONG RUI ZE'S MENU ================\n";
            cout << " 1. Key In Student Marks (Add Result)\n";
            cout << " 2. Edit Result Record\n";
            cout << " 3. Delete Result Record\n";
            cout << " 0. Logout\n";
            cout << "======================================================\n";
            choice = readInt("Enter choice: ");

            switch (choice) {
                case 1: addResultMenu(); break;
                case 2: editResultMenu(); break;
                case 3: deleteResultMenu(); break;
                case 0: logout(); break;
                default: cout << "Invalid choice. Try again.\n"; break;
            }
        }
    }
};

int main() {
    SystemManager manager; manager.loadAll();
    StaffModule staffModule(manager);
    int mainChoice = -1;
    while (mainChoice != 0) {
        cout << "\n########################################################\n";
        cout << "#      SRMS - PART 3 (TONG RUI ZE)                     #\n";
        cout << "########################################################\n";
        cout << "\n1. Staff Login (Access Tong's Features)\n0. Exit System\nEnter choice: ";
        while (!(cin >> mainChoice)) { cout << "Invalid input. Enter choice: "; cin.clear(); cin.ignore(1000, '\n'); } cin.ignore(1000, '\n');
        switch (mainChoice) {
            case 1: staffModule.run(); break;
            case 0: cout << "\nGoodbye!\n"; break; default: cout << "\nInvalid choice. Please try again.\n"; break;
        }
    }
    return 0;
}

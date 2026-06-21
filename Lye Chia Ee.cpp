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
const char SUMMARY_FILE[]   = "summary_report.txt";
const char ATTENDANCE_FILE[] = "attendance.txt";

struct Date {
    int day, month, year;
    Date() { day = 1; month = 1; year = 2026; }
    Date(int d, int m, int y) { day = d; month = m; year = y; }
    string toString() const {
        stringstream ss;
        if (day < 10) ss << "0"; ss << day << "/";
        if (month < 10) ss << "0"; ss << month << "/";
        ss << year; return ss.str();
    }
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
    void setRegisteredOn(Date d) { registeredOn = d; }
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
    void displayProfile() const { cout << "ID: " << id << " | Name: " << name << " | Prog: " << programme << endl; }
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

struct RemarkNode {
    char studentId[MAX_ID_LEN], courseCode[15], reason[120]; Date requestDate; RemarkNode* next;
    RemarkNode(const char* sid, const char* ccode, const char* r, Date d) { strcpy(studentId, sid); strcpy(courseCode, ccode); strcpy(reason, r); requestDate = d; next = NULL; }
};

class RequestList {
private:
    RemarkNode* frontPtr; RemarkNode* rearPtr; int count;
public:
    RequestList() { frontPtr = NULL; rearPtr = NULL; count = 0; }
    ~RequestList() { while (frontPtr != NULL) { RemarkNode* temp = frontPtr; frontPtr = frontPtr->next; delete temp; } }
    bool isEmpty() const { return frontPtr == NULL; }
    void enqueue(const char* sid, const char* ccode, const char* reason, Date d) {
        RemarkNode* newNode = new RemarkNode(sid, ccode, reason, d);
        if (rearPtr == NULL) { frontPtr = newNode; rearPtr = newNode; } else { rearPtr->next = newNode; rearPtr = newNode; } count++;
    }
    bool dequeue(RemarkNode &out) {
        if (isEmpty()) return false; RemarkNode* temp = frontPtr; strcpy(out.studentId, temp->studentId); strcpy(out.courseCode, temp->courseCode); strcpy(out.reason, temp->reason); out.requestDate = temp->requestDate; frontPtr = frontPtr->next; if (frontPtr == NULL) rearPtr = NULL; delete temp; count--; return true;
    }
    void displayAll() const {
        if (isEmpty()) { cout << "No pending remark requests.\n"; return; }
        RemarkNode* curr = frontPtr; int i = 1;
        while (curr != NULL) { cout << i << ". Student: " << curr->studentId << " | Course: " << curr->courseCode << " | Reason: " << curr->reason << endl; curr = curr->next; i++; }
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

class SystemManager {
private:
    Student students[MAX_STUDENTS]; int studentCount;
    CourseResult results[MAX_RESULTS]; int resultCount;
    char courseCatalog[MAX_COURSES][MAX_COURSE_LEN], courseCodes[MAX_COURSES][15]; int courseCreditHrs[MAX_COURSES], courseCount;
    RequestList requestList; StudentHashTable studentIndex; int nextResultId;
public:
    SystemManager() { studentCount = resultCount = courseCount = 0; nextResultId = 1; }
    void loadAll() { loadStudents(); loadResults(); loadCourses(); rebuildStudentIndex(); }
    void rebuildStudentIndex() { studentIndex.clear(); for (int i = 0; i < studentCount; i++) studentIndex.insert(students[i].getId(), i); }
    void loadStudents() { try { ifstream fin(STUDENT_FILE); if (!fin.is_open()) return; string line; studentCount = 0; while (getline(fin, line) && studentCount < MAX_STUDENTS) { if (line.length() == 0) continue; students[studentCount] = Student::fromFileLine(line); studentCount++; } fin.close(); } catch (...) {} }
    void loadResults() { try { ifstream fin(RESULT_FILE); if (!fin.is_open()) return; string line; resultCount = 0; int maxId = 0; while (getline(fin, line) && resultCount < MAX_RESULTS) { if (line.length() == 0) continue; results[resultCount] = CourseResult::fromFileLine(line); if (results[resultCount].getRecordId() > maxId) maxId = results[resultCount].getRecordId(); resultCount++; } fin.close(); nextResultId = maxId + 1; } catch (...) {} }
    void loadCourses() { try { ifstream fin(COURSE_FILE); if (!fin.is_open()) return; string line; courseCount = 0; while (getline(fin, line) && courseCount < MAX_COURSES) { if (line.length() == 0) continue; char buf[200]; strcpy(buf, line.c_str()); char* tok = strtok(buf, "|"); int field = 0; while (tok != NULL) { if (field == 0) strcpy(courseCodes[courseCount], tok); else if (field == 1) strcpy(courseCatalog[courseCount], tok); else if (field == 2) courseCreditHrs[courseCount] = atoi(tok); tok = strtok(NULL, "|"); field++; } courseCount++; } fin.close(); } catch (...) {} }
    
    int loginStudent(const string &id, const string &pass) { int idx = studentIndex.lookup(id); if (idx == -1) return -1; if (!students[idx].checkPassword(pass)) return -2; return idx; }
    Student* getStudentsArray() { return students; }
    int getResultsForStudent(const string &studentId, CourseResult outArr[], int maxOut) const { int n = 0; for (int i = 0; i < resultCount && n < maxOut; i++) { if (results[i].getStudentId() == studentId && results[i].isActive()) { outArr[n] = results[i]; n++; } } return n; }
    RequestList& getRequestList() { return requestList; }
    void displayAllCourses() const { cout << left << setw(8) << "Code" << setw(34) << "Course Name" << "Credit Hours" << endl; for (int i = 0; i < courseCount; i++) { cout << left << setw(8) << courseCodes[i] << setw(34) << courseCatalog[i] << courseCreditHrs[i] << endl; } }
};

// --- LYE CHIA EE ALGORITHMS ---

int linearSearchResultsByCourse(CourseResult arr[], int n, const string &code, int results[], int maxResults) {
    int found = 0;
    for (int i = 0; i < n && found < maxResults; i++) {
        if (arr[i].getCourseCode() == code) { results[found] = i; found++; }
    }
    return found;
}

void mergeTwoParts(CourseResult arr[], int left, int mid, int right) {
    int n1 = mid - left + 1; int n2 = right - mid;
    CourseResult* L = new CourseResult[n1]; CourseResult* R = new CourseResult[n2];
    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) { if (L[i].getMarks() >= R[j].getMarks()) { arr[k] = L[i]; i++; } else { arr[k] = R[j]; j++; } k++; }
    while (i < n1) { arr[k] = L[i]; i++; k++; }
    while (j < n2) { arr[k] = R[j]; j++; k++; }
    delete[] L; delete[] R;
}

void quickSortByMarks(CourseResult arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        quickSortByMarks(arr, left, mid); quickSortByMarks(arr, mid + 1, right); mergeTwoParts(arr, left, mid, right);
    }
}

void bubbleSortByCourseCode(CourseResult arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j].getCourseCode() > arr[j + 1].getCourseCode()) {
                CourseResult temp = arr[j]; arr[j] = arr[j + 1]; arr[j + 1] = temp; swapped = true;
            }
        }
        if (!swapped) break; 
    }
}

class CustomerModule {
private:
    SystemManager &mgr; int loggedInStudentIndex;
    string readLine(const string &prompt) { string val; cout << prompt; getline(cin, val); return val; }
    int readInt(const string &prompt) { int val; cout << prompt; while (!(cin >> val)) { cout << "Invalid input. Please enter a whole number: "; cin.clear(); cin.ignore(1000, '\n'); } cin.ignore(1000, '\n'); return val; }
public:
    CustomerModule(SystemManager &m) : mgr(m) { loggedInStudentIndex = -1; }
    
    bool login() {
        cout << "\n=========== STUDENT LOGIN ===========\n";
        string id = readLine("Student ID: "); string pass = readLine("Password: ");
        int result = mgr.loginStudent(id, pass);
        if (result == -1) { cout << "[Login Failed] Student ID not found.\n"; return false; }
        if (result == -2) { cout << "[Login Failed] Incorrect password.\n"; return false; }
        loggedInStudentIndex = result; cout << "\nLogin successful. Welcome, " << mgr.getStudentsArray()[result].getName() << "!\n"; return true;
    }
    
    void displayMyResults() {
        string sid = mgr.getStudentsArray()[loggedInStudentIndex].getId(); CourseResult myResults[MAX_RESULTS];
        int n = mgr.getResultsForStudent(sid, myResults, MAX_RESULTS);
        if (n == 0) { cout << "No results recorded yet.\n"; return; }
        cout << left << setw(13) << "Course Code" << setw(32) << "Course Name" << setw(7) << "Marks" << "Grade" << "\n";
        for (int i = 0; i < n; i++) { cout << left << setw(13) << myResults[i].getCourseCode() << setw(32) << myResults[i].getCourseName() << setw(7) << myResults[i].getMarks() << myResults[i].getGrade() << endl; }
    }

    // --- LYE CHIA EE UI TASKS ---

    void searchMyResultByCourseMenu() {
        cout << "\n--- Search My Results by Course Code (Linear Search) ---\n";
        cout << "Here are your current results:\n"; displayMyResults();
        string code = readLine("Enter Course Code from your results above: ");
        string sid = mgr.getStudentsArray()[loggedInStudentIndex].getId(); CourseResult myResults[MAX_RESULTS];
        int n = mgr.getResultsForStudent(sid, myResults, MAX_RESULTS);
        int* matches = new int[n > 0 ? n : 1];
        int count = linearSearchResultsByCourse(myResults, n, code, matches, n);
        if (count == 0) { cout << "No matching course found in your records.\n"; } else { for (int i = 0; i < count; i++) myResults[matches[i]].display(); }
        delete[] matches;
    }

    void sortMyResultsByMarksMenu() {
        cout << "\n--- Sort My Results by Marks (Merge Sort, Descending) ---\n";
        string sid = mgr.getStudentsArray()[loggedInStudentIndex].getId(); CourseResult myResults[MAX_RESULTS];
        int n = mgr.getResultsForStudent(sid, myResults, MAX_RESULTS);
        if (n > 0) quickSortByMarks(myResults, 0, n - 1);
        for (int i = 0; i < n; i++) myResults[i].display();
    }

    void sortMyResultsByCourseCodeMenu() {
        cout << "\n--- Sort My Results by Course Code (Bubble Sort) ---\n";
        string sid = mgr.getStudentsArray()[loggedInStudentIndex].getId(); CourseResult myResults[MAX_RESULTS];
        int n = mgr.getResultsForStudent(sid, myResults, MAX_RESULTS);
        bubbleSortByCourseCode(myResults, n);
        for (int i = 0; i < n; i++) myResults[i].display();
    }

    void submitRemarkRequest() {
        cout << "\n--- Submit Remark Request ---\n";
        cout << "Here are the available courses:\n"; mgr.displayAllCourses();
        string sid = mgr.getStudentsArray()[loggedInStudentIndex].getId();
        string code = readLine("Enter Course Code from the list above: "); string reason = readLine("Reason for remark: ");
        time_t t = time(0); tm* now = localtime(&t); Date today(now->tm_mday, now->tm_mon + 1, now->tm_year + 1900);
        mgr.getRequestList().enqueue(sid.c_str(), code.c_str(), reason.c_str(), today);
        cout << "Remark request submitted! It will appear in Staff's queue.\n";
    }

    void logout() { cout << "\nStudent logged out: " << mgr.getStudentsArray()[loggedInStudentIndex].getName() << "\n"; loggedInStudentIndex = -1; }

    void run() {
        if (!login()) return;
        int choice = -1;
        while (choice != 0) {
            cout << "\n================ LYE CHIA EE'S MENU ================\n";
            cout << " 1. Search My Results by Course Code\n";
            cout << " 2. Sort My Results by Marks\n";
            cout << " 3. Sort My Results by Course Code\n";
            cout << " 4. Submit Remark Request\n";
            cout << " 0. Logout\n";
            cout << "======================================================\n";
            choice = readInt("Enter choice: ");

            switch (choice) {
                case 1: searchMyResultByCourseMenu(); break;
                case 2: sortMyResultsByMarksMenu(); break;
                case 3: sortMyResultsByCourseCodeMenu(); break;
                case 4: submitRemarkRequest(); break;
                case 0: logout(); break;
                default: cout << "Invalid choice. Try again.\n"; break;
            }
        }
    }
};

int main() {
    SystemManager manager; manager.loadAll();
    CustomerModule customerModule(manager);
    int mainChoice = -1;
    while (mainChoice != 0) {
        cout << "\n########################################################\n";
        cout << "#      SRMS - PART 2 (LYE CHIA EE)                     #\n";
        cout << "########################################################\n";
        cout << "\n1. Student Login (Access Lye's Features)\n0. Exit System\nEnter choice: ";
        while (!(cin >> mainChoice)) { cout << "Invalid input. Enter choice: "; cin.clear(); cin.ignore(1000, '\n'); } cin.ignore(1000, '\n');
        switch (mainChoice) {
            case 1: customerModule.run(); break;
            case 0: cout << "\nGoodbye!\n"; break; default: cout << "\nInvalid choice. Please try again.\n"; break;
        }
    }
    return 0;
}

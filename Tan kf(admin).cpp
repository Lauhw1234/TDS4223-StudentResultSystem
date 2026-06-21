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
    
    void displayProfile(bool shortForm = false) const {
        if (shortForm) {
            stringstream gpaBuf; gpaBuf << fixed << setprecision(2) << gpa;
            cout << left << setw(10) << id << setw(28) << name << setw(36) << programme << "GPA: " << gpaBuf.str() << endl;
        } else {
            cout << "------------------------------------------------------\nStudent ID: " << id << "\nName: " << name << "\nProgramme: " << programme << "\nGPA: " << gpa << "\n------------------------------------------------------\n";
        }
    }
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

// --- TAN KAI FENG ALGORITHMS (Searching & Sorting) ---

void insertionSortById(Student arr[], int n) {
    for (int i = 1; i < n; i++) {
        Student key = arr[i]; int j = i - 1;
        while (j >= 0 && arr[j].getId() > key.getId()) { arr[j + 1] = arr[j]; j--; }
        arr[j + 1] = key;
    }
}

int binarySearchHelper(Student arr[], int low, int high, const string &targetId) {
    if (low > high) return -1;                       
    int mid = low + (high - low) / 2;                
    if (arr[mid].getId() == targetId) return mid;    
    else if (arr[mid].getId() < targetId) return binarySearchHelper(arr, mid + 1, high, targetId); 
    else return binarySearchHelper(arr, low, mid - 1, targetId);  
}

int binarySearchStudentById(Student arr[], int n, const string &targetId) { return binarySearchHelper(arr, 0, n - 1, targetId); }

string toLowerStr(const string &s) { string r = s; for (size_t i = 0; i < r.size(); i++) { if (r[i] >= 'A' && r[i] <= 'Z') r[i] = r[i] - 'A' + 'a'; } return r; }

int linearSearchStudentByName(Student arr[], int n, const string &keyword, int results[], int maxResults) {
    int found = 0; string lowKey = toLowerStr(keyword);
    for (int i = 0; i < n && found < maxResults; i++) {
        string lowName = toLowerStr(arr[i].getName());
         if (lowName.find(lowKey) == 0) { results[found] = i; found++; }
    }
    return found;
}

void bubbleSortByName(Student arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j].getName() > arr[j + 1].getName()) { Student temp = arr[j]; arr[j] = arr[j + 1]; arr[j + 1] = temp; swapped = true; }
        }
        if (!swapped) break;
    }
}

void selectionSortByGpa(Student arr[], int n) {
    for (int i = 1; i < n; i++) {
        Student current = arr[i]; int j = i - 1;
        while (j >= 0 && arr[j].getGpa() < current.getGpa()) { arr[j + 1] = arr[j]; j = j - 1; }
        arr[j + 1] = current; 
    }
}

void mergeTwoParts(CourseResult arr[], int left, int mid, int right) {
    int n1 = mid - left + 1; int n2 = right - mid;
    CourseResult* L = new CourseResult[n1]; CourseResult* R = new CourseResult[n2];
    for (int i = 0; i < n1; i++) L[i] = arr[left + i]; for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) { if (L[i].getMarks() >= R[j].getMarks()) { arr[k] = L[i]; i++; } else { arr[k] = R[j]; j++; } k++; }
    while (i < n1) { arr[k] = L[i]; i++; k++; } while (j < n2) { arr[k] = R[j]; j++; k++; }
    delete[] L; delete[] R;
}

void quickSortByMarks(CourseResult arr[], int left, int right) {
    if (left < right) { int mid = left + (right - left) / 2; quickSortByMarks(arr, left, mid); quickSortByMarks(arr, mid + 1, right); mergeTwoParts(arr, left, mid, right); }
}

class SystemManager {
private:
    Student students[MAX_STUDENTS]; int studentCount; Staff staffs[MAX_STAFF]; int staffCount;
    CourseResult results[MAX_RESULTS]; int resultCount;
    char courseCatalog[MAX_COURSES][MAX_COURSE_LEN], courseCodes[MAX_COURSES][15]; int courseCreditHrs[MAX_COURSES], courseCount;
    StudentHashTable studentIndex; int nextResultId;
public:
    SystemManager() { studentCount = staffCount = resultCount = courseCount = 0; nextResultId = 1; }
    void loadAll() { loadStudents(); loadStaff(); loadResults(); loadCourses(); rebuildStudentIndex(); }
    void rebuildStudentIndex() { studentIndex.clear(); for (int i = 0; i < studentCount; i++) studentIndex.insert(students[i].getId(), i); }
    void loadStudents() { try { ifstream fin(STUDENT_FILE); if (!fin.is_open()) return; string line; studentCount = 0; while (getline(fin, line) && studentCount < MAX_STUDENTS) { if (line.length() == 0) continue; students[studentCount] = Student::fromFileLine(line); studentCount++; } fin.close(); } catch (...) {} }
    void loadStaff() { try { ifstream fin(STAFF_FILE); if (!fin.is_open()) return; string line; staffCount = 0; while (getline(fin, line) && staffCount < MAX_STAFF) { if (line.length() == 0) continue; staffs[staffCount] = Staff::fromFileLine(line); staffCount++; } fin.close(); } catch (...) {} }
    void loadResults() { try { ifstream fin(RESULT_FILE); if (!fin.is_open()) return; string line; resultCount = 0; int maxId = 0; while (getline(fin, line) && resultCount < MAX_RESULTS) { if (line.length() == 0) continue; results[resultCount] = CourseResult::fromFileLine(line); if (results[resultCount].getRecordId() > maxId) maxId = results[resultCount].getRecordId(); resultCount++; } fin.close(); nextResultId = maxId + 1; } catch (...) {} }
    void loadCourses() { try { ifstream fin(COURSE_FILE); if (!fin.is_open()) return; string line; courseCount = 0; while (getline(fin, line) && courseCount < MAX_COURSES) { if (line.length() == 0) continue; char buf[200]; strcpy(buf, line.c_str()); char* tok = strtok(buf, "|"); int field = 0; while (tok != NULL) { if (field == 0) strcpy(courseCodes[courseCount], tok); else if (field == 1) strcpy(courseCatalog[courseCount], tok); else if (field == 2) courseCreditHrs[courseCount] = atoi(tok); tok = strtok(NULL, "|"); field++; } courseCount++; } fin.close(); } catch (...) {} }
    
    int loginStaff(const string &id, const string &pass) { for (int i = 0; i < staffCount; i++) { if (staffs[i].getId() == id) { if (!staffs[i].checkPassword(pass)) return -2; return i; } } return -1; }
    Student* getStudentsArray() { return students; } int getStudentCount() const { return studentCount; }
    Staff* getStaffArray() { return staffs; }
    int findCourseIndex(const string &code) const { for (int i = 0; i < courseCount; i++) { if (string(courseCodes[i]) == code) return i; } return -1; }

    // --- TAN KAI FENG CORE TASKS (Analytics & Charts) ---

    int getStudentRankInCourse(const string &studentId, const string &courseCode) const {
        CourseResult courseResults[MAX_RESULTS]; int n = 0;
        for (int i = 0; i < resultCount; i++) { if (results[i].getCourseCode() == courseCode && results[i].isActive()) { courseResults[n] = results[i]; n++; } }
        if (n == 0) return -1;
        CourseResult* sortable = new CourseResult[n]; for (int i = 0; i < n; i++) sortable[i] = courseResults[i];
        quickSortByMarks(sortable, 0, n - 1);
        int rank = -1; for (int i = 0; i < n; i++) { if (sortable[i].getStudentId() == studentId) { rank = i + 1; break; } }
        delete[] sortable; return rank;
    }

    int getCourseEnrollmentCount(const string &courseCode) const { int n = 0; for (int i = 0; i < resultCount; i++) { if (results[i].getCourseCode() == courseCode && results[i].isActive()) n++; } return n; }
    
    float getCourseAverageMarks(const string &courseCode) const { float sum = 0; int n = 0; for (int i = 0; i < resultCount; i++) { if (results[i].getCourseCode() == courseCode && results[i].isActive()) { sum += results[i].getMarks(); n++; } } return (n > 0) ? (sum / n) : 0.0f; }

    void displayCourseAnalytics(const string &courseCode) const {
        int idx = findCourseIndex(courseCode); if (idx == -1) { cout << "Course not found.\n"; return; }
        int enrolled = getCourseEnrollmentCount(courseCode); float avg = getCourseAverageMarks(courseCode);
        cout << "------------------------------------------------------\nCOURSE ANALYTICS: " << courseCodes[idx] << " - " << courseCatalog[idx] << "\n------------------------------------------------------\n";
        cout << "Enrolled Students : " << enrolled << "\nAverage Marks     : " << avg << endl;
        if (enrolled > 0) {
            CourseResult courseResults[MAX_RESULTS]; int n = 0;
            for (int i = 0; i < resultCount; i++) { if (results[i].getCourseCode() == courseCode && results[i].isActive()) { courseResults[n] = results[i]; n++; } }
            CourseResult* sortable = new CourseResult[n]; for (int i = 0; i < n; i++) sortable[i] = courseResults[i];
            quickSortByMarks(sortable, 0, n - 1);
            cout << "Top performer     : " << sortable[0].getStudentId() << " (" << sortable[0].getMarks() << " marks, grade " << sortable[0].getGrade() << ")\n";
            cout << "Lowest performer  : " << sortable[n - 1].getStudentId() << " (" << sortable[n - 1].getMarks() << " marks, grade " << sortable[n - 1].getGrade() << ")\n";
            delete[] sortable;
        }
        cout << "------------------------------------------------------\n";
    }

    void printOneBar(const string &label, int count) const { cout << label << " | "; for (int i = 0; i < count; i++) cout << "*"; cout << " (" << count << ")\n"; }

    void displayGradeChart() const {
        int countA = 0, countB = 0, countC = 0, countD = 0, countF = 0;
        for (int i = 0; i < resultCount; i++) {
            if (!results[i].isActive()) continue;
            string g = results[i].getGrade(); char first = g[0];
            if (first == 'A') countA++; else if (first == 'B') countB++; else if (first == 'C') countC++; else if (first == 'D') countD++; else countF++;
        }
        cout << "------------------------------------------------------\nGRADE DISTRIBUTION CHART (1 star = 1 result)\n------------------------------------------------------\n";
        printOneBar("A", countA); printOneBar("B", countB); printOneBar("C", countC); printOneBar("D", countD); printOneBar("F", countF);
        cout << "------------------------------------------------------\nTotal results counted: " << (countA + countB + countC + countD + countF) << "\n------------------------------------------------------\n";
    }
};

class StaffModule {
private:
    SystemManager &mgr; int loggedInStaffIndex;
    string readLine(const string &prompt) { string val; cout << prompt; getline(cin, val); return val; }
    int readInt(const string &prompt) { int val; cout << prompt; while (!(cin >> val)) { cout << "Invalid input. Please enter a whole number: "; cin.clear(); cin.ignore(1000, '\n'); } cin.ignore(1000, '\n'); return val; }
public:
    StaffModule(SystemManager &m) : mgr(m) { loggedInStaffIndex = -1; }

    bool login() {
        cout << "\n=========== STAFF LOGIN ===========\n";
        string id = readLine("Staff ID: "); string pass = readLine("Password: ");
        int result = mgr.loginStaff(id, pass);
        if (result == -1) { cout << "[Login Failed] Staff ID not found.\n"; return false; }
        if (result == -2) { cout << "[Login Failed] Incorrect password.\n"; return false; }
        loggedInStaffIndex = result; cout << "\nLogin successful. Welcome, " << mgr.getStaffArray()[result].getName() << "!\n"; return true;
    }

    // --- TAN KAI FENG UI TASKS ---

    void searchStudentByIdMenu() {
        cout << "\n--- Search Student by ID (Binary Search) ---\n";
        string id = readLine("Enter Student ID: ");
        int n = mgr.getStudentCount();
        Student* copyArr = new Student[n]; for (int i = 0; i < n; i++) copyArr[i] = mgr.getStudentsArray()[i];
        insertionSortById(copyArr, n);
        int idx = binarySearchStudentById(copyArr, n, id);
        if (idx == -1) cout << "Student not found.\n"; else copyArr[idx].displayProfile();
        delete[] copyArr;
    }

    void searchStudentByNameMenu() {
        cout << "\n--- Search Student by Name (Linear Search) ---\n";
        string keyword = readLine("Enter name keyword: ");
        Student* arr = mgr.getStudentsArray(); int n = mgr.getStudentCount(); int* matches = new int[n];
        int count = linearSearchStudentByName(arr, n, keyword, matches, n);
        if (count == 0) cout << "No matching students found.\n";
        else { cout << "Found " << count << " match(es):\n"; for (int i = 0; i < count; i++) arr[matches[i]].displayProfile(true); }
        delete[] matches;
    }

    void sortStudentsByNameMenu() {
        cout << "\n--- Sort Students by Name (Bubble Sort) ---\n";
        int n = mgr.getStudentCount();
        Student* copyArr = new Student[n]; for (int i = 0; i < n; i++) copyArr[i] = mgr.getStudentsArray()[i];
        bubbleSortByName(copyArr, n);
        for (int i = 0; i < n; i++) copyArr[i].displayProfile(true);
        delete[] copyArr;
    }

    void sortStudentsByGpaMenu() {
        cout << "\n--- Sort Students by GPA (Insertion Sort, Descending) ---\n";
        int n = mgr.getStudentCount();
        Student* copyArr = new Student[n]; for (int i = 0; i < n; i++) copyArr[i] = mgr.getStudentsArray()[i];
        selectionSortByGpa(copyArr, n);
        for (int i = 0; i < n; i++) copyArr[i].displayProfile(true);
        delete[] copyArr;
    }

    void courseAnalyticsMenu() {
        cout << "\n--- Course Analytics ---\n"; string code = readLine("Enter Course Code: "); mgr.displayCourseAnalytics(code);
    }

    void studentRankMenu() {
        cout << "\n--- Student Rank in a Course ---\n";
        string sid = readLine("Enter Student ID: "); string code = readLine("Enter Course Code: ");
        int rank = mgr.getStudentRankInCourse(sid, code);
        if (rank == -1) { cout << "No matching record found for that student/course combination.\n"; } 
        else { int enrolled = mgr.getCourseEnrollmentCount(code); cout << "Student " << sid << " is ranked #" << rank << " out of " << enrolled << " in course " << code << ".\n"; }
    }

    void logout() { cout << "\nStaff logged out: " << mgr.getStaffArray()[loggedInStaffIndex].getName() << "\n"; loggedInStaffIndex = -1; }

    void run() {
        if (!login()) return;
        int choice = -1;
        while (choice != 0) {
            cout << "\n================ TAN KAI FENG'S MENU ================\n";
            cout << " 1. Search Student by ID\n 2. Search Student by Name\n 3. Sort Students by Name\n 4. Sort Students by GPA\n 5. Course Analytics (avg, top, bottom)\n 6. Student Rank in a Course\n 7. Grade Distribution Chart\n 0. Logout\n";
            cout << "=======================================================\n";
            choice = readInt("Enter choice: ");
            switch (choice) {
                case 1: searchStudentByIdMenu(); break; case 2: searchStudentByNameMenu(); break;
                case 3: sortStudentsByNameMenu(); break; case 4: sortStudentsByGpaMenu(); break;
                case 5: courseAnalyticsMenu(); break; case 6: studentRankMenu(); break;
                case 7: mgr.displayGradeChart(); break; case 0: logout(); break;
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
        cout << "#      SRMS - PART 4 (TAN KAI FENG)                    #\n";
        cout << "########################################################\n";
        cout << "\n1. Staff Login (Access Tan's Features)\n0. Exit System\nEnter choice: ";
        while (!(cin >> mainChoice)) { cout << "Invalid input. Enter choice: "; cin.clear(); cin.ignore(1000, '\n'); } cin.ignore(1000, '\n');
        switch (mainChoice) {
            case 1: staffModule.run(); break;
            case 0: cout << "\nGoodbye!\n"; break; default: cout << "\nInvalid choice. Please try again.\n"; break;
        }
    }
    return 0;
}

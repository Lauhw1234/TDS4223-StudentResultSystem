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
const char SUMMARY_FILE[]   = "summary_report.txt";
const char ATTENDANCE_FILE[] = "attendance.txt";

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

// --- LEONG SZE KAI: Attendance Data Structure ---
class AttendanceEntry {
private:
    char studentId[MAX_ID_LEN], courseCode[15]; int classesHeld, classesAttended;
public:
    AttendanceEntry() { strcpy(studentId, ""); strcpy(courseCode, ""); classesHeld = 0; classesAttended = 0; }
    AttendanceEntry(const char* sid, const char* ccode, int held, int attended) { strcpy(studentId, sid); strcpy(courseCode, ccode); classesHeld = held; classesAttended = attended; }
    string getStudentId() const { return string(studentId); }
    string getCourseCode() const { return string(courseCode); }
    int getClassesHeld() const { return classesHeld; }
    int getClassesAttended() const { return classesAttended; }
    float attendancePercent() const { if (classesHeld == 0) return 0.0f; return ((float)classesAttended / (float)classesHeld) * 100.0f; }
    bool meetsMinimumAttendance() const { return attendancePercent() >= 80.0f; }
    void display() const {
        stringstream attBuf; attBuf << classesAttended << "/" << classesHeld;
        stringstream pctBuf; pctBuf << fixed << setprecision(1) << attendancePercent() << "%";
        cout << left << setw(10) << studentId << setw(8) << courseCode << setw(10) << attBuf.str() << setw(10) << pctBuf.str() << (meetsMinimumAttendance() ? "OK" : "WARNING") << endl;
    }
    string toFileLine() const { stringstream ss; ss << studentId << "|" << courseCode << "|" << classesHeld << "|" << classesAttended; return ss.str(); }
    static AttendanceEntry fromFileLine(const string &line) {
        char buf[150]; strcpy(buf, line.c_str()); char sid[MAX_ID_LEN] = "", code[15] = ""; int held = 0, attended = 0;
        char* tok = strtok(buf, "|"); int field = 0;
        while (tok != NULL) { if (field == 0) strcpy(sid, tok); else if (field == 1) strcpy(code, tok); else if (field == 2) held = atoi(tok); else if (field == 3) attended = atoi(tok); tok = strtok(NULL, "|"); field++; }
        return AttendanceEntry(sid, code, held, attended);
    }
};

void bubbleSortByAttendance(AttendanceEntry arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j].attendancePercent() < arr[j + 1].attendancePercent()) {
                AttendanceEntry temp = arr[j]; arr[j] = arr[j + 1]; arr[j + 1] = temp; swapped = true;
            }
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

class FileException { private: string message; public: FileException(string msg) { message = msg; } string what() const { return message; } };
class ValidationException { private: string message; public: ValidationException(string msg) { message = msg; } string what() const { return message; } };
class NotFoundException { private: string message; public: NotFoundException(string msg) { message = msg; } string what() const { return message; } };

class SystemManager {
private:
    Student students[MAX_STUDENTS]; int studentCount; Staff staffs[MAX_STAFF]; int staffCount;
    CourseResult results[MAX_RESULTS]; int resultCount;
    char courseCatalog[MAX_COURSES][MAX_COURSE_LEN], courseCodes[MAX_COURSES][15]; int courseCreditHrs[MAX_COURSES], courseCount;
    AttendanceEntry attendance[MAX_RESULTS]; int attendanceCount;
    StudentHashTable studentIndex; int nextResultId;
public:
    SystemManager() { studentCount = staffCount = resultCount = courseCount = attendanceCount = 0; nextResultId = 1; }
    void loadAll() { loadStudents(); loadStaff(); loadResults(); loadCourses(); loadAttendance(); rebuildStudentIndex(); }
    void rebuildStudentIndex() { studentIndex.clear(); for (int i = 0; i < studentCount; i++) studentIndex.insert(students[i].getId(), i); }
    void loadStudents() { try { ifstream fin(STUDENT_FILE); if (!fin.is_open()) return; string line; studentCount = 0; while (getline(fin, line) && studentCount < MAX_STUDENTS) { if (line.length() == 0) continue; students[studentCount] = Student::fromFileLine(line); studentCount++; } fin.close(); } catch (...) {} }
    void loadStaff() { try { ifstream fin(STAFF_FILE); if (!fin.is_open()) return; string line; staffCount = 0; while (getline(fin, line) && staffCount < MAX_STAFF) { if (line.length() == 0) continue; staffs[staffCount] = Staff::fromFileLine(line); staffCount++; } fin.close(); } catch (...) {} }
    void loadResults() { try { ifstream fin(RESULT_FILE); if (!fin.is_open()) return; string line; resultCount = 0; int maxId = 0; while (getline(fin, line) && resultCount < MAX_RESULTS) { if (line.length() == 0) continue; results[resultCount] = CourseResult::fromFileLine(line); if (results[resultCount].getRecordId() > maxId) maxId = results[resultCount].getRecordId(); resultCount++; } fin.close(); nextResultId = maxId + 1; } catch (...) {} }
    void loadCourses() { try { ifstream fin(COURSE_FILE); if (!fin.is_open()) return; string line; courseCount = 0; while (getline(fin, line) && courseCount < MAX_COURSES) { if (line.length() == 0) continue; char buf[200]; strcpy(buf, line.c_str()); char* tok = strtok(buf, "|"); int field = 0; while (tok != NULL) { if (field == 0) strcpy(courseCodes[courseCount], tok); else if (field == 1) strcpy(courseCatalog[courseCount], tok); else if (field == 2) courseCreditHrs[courseCount] = atoi(tok); tok = strtok(NULL, "|"); field++; } courseCount++; } fin.close(); } catch (...) {} }
    void loadAttendance() { try { ifstream fin(ATTENDANCE_FILE); if (!fin.is_open()) return; string line; attendanceCount = 0; while (getline(fin, line) && attendanceCount < MAX_RESULTS) { if (line.length() == 0) continue; attendance[attendanceCount] = AttendanceEntry::fromFileLine(line); attendanceCount++; } fin.close(); } catch (...) {} }
    
    void saveCourses() { ofstream fout(COURSE_FILE); for (int i = 0; i < courseCount; i++) fout << courseCodes[i] << "|" << courseCatalog[i] << "|" << courseCreditHrs[i] << endl; fout.close(); }
    void saveAttendance() { ofstream fout(ATTENDANCE_FILE); for (int i = 0; i < attendanceCount; i++) fout << attendance[i].toFileLine() << endl; fout.close(); }
    
    bool studentIdExists(const string &id) const { return studentIndex.lookup(id) != -1; }
    bool courseCodeExists(const string &code) const { for (int i = 0; i < courseCount; i++) { if (string(courseCodes[i]) == code) return true; } return false; }
    int findCourseIndex(const string &code) const { for (int i = 0; i < courseCount; i++) { if (string(courseCodes[i]) == code) return i; } return -1; }
    int loginStaff(const string &id, const string &pass) { for (int i = 0; i < staffCount; i++) { if (staffs[i].getId() == id) { if (!staffs[i].checkPassword(pass)) return -2; return i; } } return -1; }
    Staff* getStaffArray() { return staffs; }
    AttendanceEntry* getAttendanceArray() { return attendance; }
    int getAttendanceCount() const { return attendanceCount; }

    // --- LEONG SZE KAI CORE TASKS ---

    bool addCourse(const string &code, const string &name, int credits) {
        try {
            if (courseCount >= MAX_COURSES) throw ValidationException("Course capacity full.");
            if (courseCodeExists(code)) throw ValidationException("Course code already exists.");
            if (code.length() == 0 || name.length() == 0) throw ValidationException("Course code/name cannot be empty.");
            strcpy(courseCodes[courseCount], code.c_str()); strcpy(courseCatalog[courseCount], name.c_str()); courseCreditHrs[courseCount] = credits; courseCount++;
            saveCourses(); return true;
        } catch (ValidationException &e) { cout << "[Add Course Failed] " << e.what() << endl; return false; }
    }

    bool editCourse(const string &code, const string &newName, int newCredits) {
        try {
            int idx = findCourseIndex(code); if (idx == -1) throw NotFoundException("Course code not found.");
            strcpy(courseCatalog[idx], newName.c_str()); courseCreditHrs[idx] = newCredits;
            saveCourses(); return true;
        } catch (NotFoundException &e) { cout << "[Edit Course Failed] " << e.what() << endl; return false; }
    }

    bool deleteCourse(const string &code) {
        try {
            int idx = findCourseIndex(code); if (idx == -1) throw NotFoundException("Course code not found.");
            for (int i = idx; i < courseCount - 1; i++) { strcpy(courseCodes[i], courseCodes[i + 1]); strcpy(courseCatalog[i], courseCatalog[i + 1]); courseCreditHrs[i] = courseCreditHrs[i + 1]; }
            courseCount--; saveCourses(); return true;
        } catch (NotFoundException &e) { cout << "[Delete Course Failed] " << e.what() << endl; return false; }
    }

    bool recordAttendance(const string &sid, const string &courseCode, int held, int attended) {
        try {
            if (!studentIdExists(sid)) throw NotFoundException("Student ID not found.");
            if (findCourseIndex(courseCode) == -1) throw NotFoundException("Course code not found.");
            if (attended > held) throw ValidationException("Classes attended cannot exceed classes held.");
            if (held < 0 || attended < 0) throw ValidationException("Values cannot be negative.");
            if (attendanceCount >= MAX_RESULTS) throw ValidationException("Attendance capacity full.");

            for (int i = 0; i < attendanceCount; i++) {
                if (attendance[i].getStudentId() == sid && attendance[i].getCourseCode() == courseCode) {
                    attendance[i] = AttendanceEntry(sid.c_str(), courseCode.c_str(), held, attended);
                    saveAttendance(); return true;
                }
            }
            attendance[attendanceCount] = AttendanceEntry(sid.c_str(), courseCode.c_str(), held, attended); attendanceCount++; saveAttendance();
            return true;
        } catch (NotFoundException &e) { cout << "[Attendance Failed] " << e.what() << endl; return false;
        } catch (ValidationException &e) { cout << "[Attendance Failed] " << e.what() << endl; return false; }
    }

    bool isStudentAtRisk(const string &studentId) const {
        int sIdx = studentIndex.lookup(studentId); if (sIdx == -1) return false;
        if (students[sIdx].getGpa() < 2.0f && students[sIdx].getTotalCredits() > 0) return true;
        for (int i = 0; i < attendanceCount; i++) { if (attendance[i].getStudentId() == studentId && !attendance[i].meetsMinimumAttendance()) return true; }
        return false;
    }

    void displayAtRiskStudents() const {
        cout << "------------------------------------------------------\nAT-RISK STUDENTS (Low GPA and/or Low Attendance)\n------------------------------------------------------\n";
        int flagged = 0;
        for (int i = 0; i < studentCount; i++) {
            string sid = students[i].getId();
            if (isStudentAtRisk(sid)) {
                stringstream gpaBuf; gpaBuf << fixed << setprecision(2) << students[i].getGpa();
                cout << left << setw(10) << sid << setw(28) << students[i].getName() << "GPA: " << gpaBuf.str() << endl; flagged++;
            }
        }
        if (flagged == 0) cout << "No students currently flagged as at-risk.\n"; else cout << "------------------------------------------------------\nTotal flagged: " << flagged << " out of " << studentCount << " students.\n";
        cout << "------------------------------------------------------\n";
    }

    void generateStaffSummaryReport() {
        try {
            ofstream fout(SUMMARY_FILE); if (!fout.is_open()) throw FileException("Cannot write summary report file.");
            float highestMarks = -1, lowestMarks = 101, sumMarks = 0; string topStudent = "N/A"; int activeCount = 0;
            for (int i = 0; i < resultCount; i++) {
                if (!results[i].isActive()) continue; activeCount++; sumMarks += results[i].getMarks();
                if (results[i].getMarks() > highestMarks) { highestMarks = results[i].getMarks(); topStudent = results[i].getStudentId(); }
                if (results[i].getMarks() < lowestMarks) lowestMarks = results[i].getMarks();
            }
            float avgMarks = (activeCount > 0) ? (sumMarks / activeCount) : 0;

            fout << "========================================================\n      STUDENT RESULT MANAGEMENT SYSTEM - STAFF REPORT\n========================================================\n";
            fout << "Total Students Registered : " << studentCount << "\nTotal Courses Offered     : " << courseCount << "\nTotal Result Records      : " << activeCount << "\nAverage Marks (all)       : " << avgMarks << "\nHighest Marks Recorded    : " << highestMarks << " (Student " << topStudent << ")\n";
            
            int atRiskCount = 0; for (int i = 0; i < studentCount; i++) { if (isStudentAtRisk(students[i].getId())) atRiskCount++; }
            fout << "Students Flagged At-Risk  : " << atRiskCount << "\n--------------------------------------------------------\nTop 5 Students by GPA:\n";

            Student topList[MAX_STUDENTS]; for (int i = 0; i < studentCount; i++) topList[i] = students[i]; selectionSortByGpa(topList, studentCount);
            int limit = (studentCount < 5) ? studentCount : 5;
            for (int i = 0; i < limit; i++) fout << "  " << (i + 1) << ". " << topList[i].getName() << " (" << topList[i].getId() << ") - GPA: " << topList[i].getGpa() << endl;
            fout << "========================================================\n"; fout.close();
            cout << "Summary report saved to " << SUMMARY_FILE << endl;
        } catch (FileException &e) { cout << "[Error] " << e.what() << endl; }
    }

    void displaySummaryReportFromFile() {
        try {
            ifstream fin(SUMMARY_FILE); if (!fin.is_open()) throw FileException("No summary report found. Please generate one first.");
            string line; cout << endl; while (getline(fin, line)) cout << line << endl; fin.close();
        } catch (FileException &e) { cout << "[Error] " << e.what() << endl; }
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

    // --- LEONG SZE KAI UI TASKS ---

    void addCourseMenu() {
        cout << "\n--- Add New Course ---\n";
        string code = readLine("Course Code: "); string name = readLine("Course Name: "); int credits = readInt("Credit Hours: ");
        if (mgr.addCourse(code, name, credits)) cout << "Course added successfully!\n";
    }

    void editCourseMenu() {
        cout << "\n--- Edit Course ---\n";
        string code = readLine("Enter Course Code to edit: "); string newName = readLine("New Course Name: "); int newCredits = readInt("New Credit Hours: ");
        if (mgr.editCourse(code, newName, newCredits)) cout << "Course updated successfully!\n";
    }

    void deleteCourseMenu() {
        cout << "\n--- Delete Course ---\n"; string code = readLine("Enter Course Code to delete: ");
        if (mgr.deleteCourse(code)) cout << "Course deleted successfully!\n";
    }

    void recordAttendanceMenu() {
        cout << "\n--- Record / Update Attendance ---\n";
        string sid = readLine("Student ID: "); string code = readLine("Course Code: "); int held = readInt("Total Classes Held: "); int attended = readInt("Classes Attended: ");
        if (mgr.recordAttendance(sid, code, held, attended)) cout << "Attendance recorded successfully!\n";
    }

    void viewAllAttendanceSortedMenu() {
        cout << "\n--- All Attendance Records (Sorted by % Descending) ---\n";
        int n = mgr.getAttendanceCount(); if (n == 0) { cout << "No attendance records yet.\n"; return; }
        AttendanceEntry* copyArr = new AttendanceEntry[n]; for (int i = 0; i < n; i++) copyArr[i] = mgr.getAttendanceArray()[i];
        bubbleSortByAttendance(copyArr, n);
        cout << left << setw(10) << "StudentID" << setw(8) << "Course" << setw(10) << "Attended" << setw(10) << "%" << "Status" << "\n";
        for (int i = 0; i < n; i++) copyArr[i].display();
        delete[] copyArr;
    }

    void logout() { cout << "\nStaff logged out: " << mgr.getStaffArray()[loggedInStaffIndex].getName() << "\n"; loggedInStaffIndex = -1; }

    void run() {
        if (!login()) return;
        int choice = -1;
        while (choice != 0) {
            cout << "\n================ LEONG SZE KAI'S MENU ================\n";
            cout << " 1. Add Course\n 2. Edit Course\n 3. Delete Course\n 4. Record / Update Attendance\n 5. View All Attendance (Sorted)\n 6. At-Risk Students Report\n 7. Generate Summary Report (save to txt)\n 8. View Summary Report (load from txt)\n 0. Logout\n";
            cout << "========================================================\n";
            choice = readInt("Enter choice: ");
            switch (choice) {
                case 1: addCourseMenu(); break; case 2: editCourseMenu(); break;
                case 3: deleteCourseMenu(); break; case 4: recordAttendanceMenu(); break;
                case 5: viewAllAttendanceSortedMenu(); break; case 6: mgr.displayAtRiskStudents(); break;
                case 7: mgr.generateStaffSummaryReport(); break; case 8: mgr.displaySummaryReportFromFile(); break;
                case 0: logout(); break; default: cout << "Invalid choice. Try again.\n"; break;
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
        cout << "#      SRMS - PART 5 (LEONG SZE KAI)                   #\n";
        cout << "########################################################\n";
        cout << "\n1. Staff Login (Access Leong's Features)\n0. Exit System\nEnter choice: ";
        while (!(cin >> mainChoice)) { cout << "Invalid input. Enter choice: "; cin.clear(); cin.ignore(1000, '\n'); } cin.ignore(1000, '\n');
        switch (mainChoice) {
            case 1: staffModule.run(); break;
            case 0: cout << "\nGoodbye!\n"; break; default: cout << "\nInvalid choice. Please try again.\n"; break;
        }
    }
    return 0;
}

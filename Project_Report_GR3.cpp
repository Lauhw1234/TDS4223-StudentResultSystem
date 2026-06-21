/* ============================================================================
   PART 1: HEADERS, CONSTANTS, STRUCTS, BASE CLASSES
   ============================================================================ */
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <iomanip>
using namespace std;

//global constants
  
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

/* ----------------------------------------------------------------------
   STRUCT 1: Date
   A simple date structure used across student, staff and result records.
   ---------------------------------------------------------------------- */
struct Date {
    int day;
    int month;
    int year;

    Date() {                       // default constructor (overload 1 of Date)
        day = 1; month = 1; year = 2026;
    }
    Date(int d, int m, int y) {    // parameterised constructor (overload 2 of Date)
        day = d; month = m; year = y;
    }

    string toString() const {
        stringstream ss;
        if (day < 10) ss << "0";
        ss << day << "/";
        if (month < 10) ss << "0";
        ss << month << "/";
        ss << year;
        return ss.str();
    }
};


   //STRUCT 2: ContactInfo
   //Holds contact details shared by Student and Staff (used inside Person).

struct ContactInfo {
    char phone[20];
    char email[60];

    ContactInfo() {
        strcpy(phone, "N/A");
        strcpy(email, "N/A");
    }
    ContactInfo(const char* p, const char* e) {
        strcpy(phone, p);
        strcpy(email, e);
    }
};

/* ----------------------------------------------------------------------
   FORWARD DECLARATIONS (needed for friend functions across classes)
   ---------------------------------------------------------------------- */
class Person;
class Student;
class Staff;

/* Friend function declarations (will be defined after the classes below) */
void printPersonShort(const Person &p);                 // friend function 1
bool comparePersonByName(const Person &a, const Person &b); // friend function 2

/* ----------------------------------------------------------------------
   BASE CLASS 1: Person
   Common attributes for anyone in the system (Student or Staff).
   ---------------------------------------------------------------------- */
class Person {
protected:
    char id[MAX_ID_LEN];
    char name[MAX_NAME_LEN];
    char password[MAX_PASS_LEN];
    ContactInfo contact;
    Date registeredOn;

public:
    /* Constructor 1 (default) */
    Person() {
        strcpy(id, "");
        strcpy(name, "");
        strcpy(password, "");
    }
    /* Constructor 2 (overloaded - parameterised) */
    Person(const char* pid, const char* pname, const char* ppass) {
        strcpy(id, pid);
        strcpy(name, pname);
        strcpy(password, ppass);
    }

    /* Virtual destructor so derived class destructors run correctly */
    virtual ~Person() {
        // base destructor (Person)
    }

    void setId(const char* pid)       { strcpy(id, pid); }
    void setName(const char* pname)   { strcpy(name, pname); }
    void setPassword(const char* pw)  { strcpy(password, pw); }
    void setContact(ContactInfo c)    { contact = c; }
    void setRegisteredOn(Date d)      { registeredOn = d; }

    string getId() const        { return string(id); }
    string getName() const      { return string(name); }
    string getPassword() const  { return string(password); }
    ContactInfo getContact() const { return contact; }
    Date getRegisteredOn() const   { return registeredOn; }

    bool checkPassword(const string &attempt) const {
        return attempt == string(password);
    }

    /* Virtual function -> overridden by Student and Staff (polymorphism) */
    virtual void displayProfile() const {
        cout << "ID: " << id << " | Name: " << name << endl;
    }

    /* Declare friend functions so they can access protected members */
    friend void printPersonShort(const Person &p);
    friend bool comparePersonByName(const Person &a, const Person &b);
};

/* ----------------------------------------------------------------------
   BASE CLASS 2: Record
   Generic base for any storable record in the system (used by CourseResult).
   ---------------------------------------------------------------------- */
class Record {
protected:
    int recordId;
    Date createdOn;
    bool active;

public:
    Record() {
        recordId = 0;
        active = true;
    }
    Record(int rid) {
        recordId = rid;
        active = true;
    }
    virtual ~Record() {
        // base destructor (Record)
    }

    int getRecordId() const { return recordId; }
    void setRecordId(int rid) { recordId = rid; }
    bool isActive() const { return active; }
    void setActive(bool a) { active = a; }
    Date getCreatedOn() const { return createdOn; }
    void setCreatedOn(Date d) { createdOn = d; }

    virtual void display() const {
        cout << "Record #" << recordId << endl;
    }
};
/* ============================================================================
   PART 2: DERIVED CLASSES (Student, Staff, CourseResult)
   ============================================================================ */

/* ----------------------------------------------------------------------
   DERIVED CLASS 1: Student : public Person
   ---------------------------------------------------------------------- */
class Student : public Person {
private:
    char programme[MAX_COURSE_LEN];
    float gpa;
    int totalCredits;

public:
    /* Constructor (default) */
    Student() : Person() {
        strcpy(programme, "Undeclared");
        gpa = 0.0f;
        totalCredits = 0;
    }
    /* Constructor (overloaded - parameterised) - satisfies "1 constructor per module" */
    Student(const char* pid, const char* pname, const char* ppass, const char* prog)
        : Person(pid, pname, ppass) {
        strcpy(programme, prog);
        gpa = 0.0f;
        totalCredits = 0;
    }

    /* Destructor (overridden) - satisfies "1 destructor per module" */
    ~Student() {
        // cout << "Student object for " << name << " destroyed." << endl;
    }

    void setProgramme(const char* p) { strcpy(programme, p); }
    void setGpa(float g) { gpa = g; }
    void setTotalCredits(int c) { totalCredits = c; }
    string getProgramme() const { return string(programme); }
    float getGpa() const { return gpa; }
    int getTotalCredits() const { return totalCredits; }

    /* Overridden virtual function (polymorphism) */
    void displayProfile() const {
        cout << "------------------------------------------------------" << endl;
        cout << "Student ID   : " << id << endl;
        cout << "Name         : " << name << endl;
        cout << "Programme    : " << programme << endl;
        cout << "GPA          : " << gpa << endl;
        cout << "Credits      : " << totalCredits << endl;
        cout << "Phone        : " << contact.phone << endl;
        cout << "Email        : " << contact.email << endl;
        cout << "Registered   : " << registeredOn.toString() << endl;
        cout << "------------------------------------------------------" << endl;
    }

    // Overloaded display: short form vs full form (overload of displayProfile) 
    void displayProfile(bool shortForm) const {
        if (shortForm) {
            stringstream gpaBuf; gpaBuf << fixed << setprecision(2) << gpa;
            cout << left << setw(10) << id << setw(28) << name << setw(36) << programme << "GPA: " << gpaBuf.str() << endl;
        } else {
            displayProfile();
        }
    }

    string toFileLine() const {
        stringstream ss;
        ss << id << "|" << name << "|" << password << "|" << programme << "|"
           << gpa << "|" << totalCredits << "|" << contact.phone << "|"
           << contact.email << "|" << registeredOn.toString();
        return ss.str();
    }

    static Student fromFileLine(const string &line) {
        Student s;
        char buf[300];
        strcpy(buf, line.c_str());
        char* tok = strtok(buf, "|");
        int field = 0;
        char ph[20] = "N/A", em[60] = "N/A";
        while (tok != NULL) {
            switch (field) {
                case 0: s.setId(tok); break;
                case 1: s.setName(tok); break;
                case 2: s.setPassword(tok); break;
                case 3: s.setProgramme(tok); break;
                case 4: s.setGpa((float)atof(tok)); break;
                case 5: s.setTotalCredits(atoi(tok)); break;
                case 6: strcpy(ph, tok); break;
                case 7: strcpy(em, tok); break;
                default: break;
            }
            tok = strtok(NULL, "|");
            field++;
        }
        s.setContact(ContactInfo(ph, em));
        return s;
    }
};


   //DERIVED CLASS 2: Staff : public Person
 
class Staff : public Person {
private:
    char department[MAX_COURSE_LEN];
    char position[30];

public:
    Staff() : Person() {
        strcpy(department, "General");
        strcpy(position, "Lecturer");
    }
    Staff(const char* pid, const char* pname, const char* ppass,
          const char* dept, const char* pos)
        : Person(pid, pname, ppass) {
        strcpy(department, dept);
        strcpy(position, pos);
    }

    // Destructor (overridden) - satisfies "1 destructor per module" 
    ~Staff() {
        // cout << "Staff object for " << name << " destroyed." << endl;
    }

    void setDepartment(const char* d) { strcpy(department, d); }
    void setPosition(const char* p) { strcpy(position, p); }
    string getDepartment() const { return string(department); }
    string getPosition() const { return string(position); }

    void displayProfile() const {
        cout << "------------------------------------------------------" << endl;
        cout << "Staff ID     : " << id << endl;
        cout << "Name         : " << name << endl;
        cout << "Department   : " << department << endl;
        cout << "Position     : " << position << endl;
        cout << "Phone        : " << contact.phone << endl;
        cout << "Email        : " << contact.email << endl;
        cout << "------------------------------------------------------" << endl;
    }

    string toFileLine() const {
        stringstream ss;
        ss << id << "|" << name << "|" << password << "|" << department << "|"
           << position << "|" << contact.phone << "|" << contact.email;
        return ss.str();
    }

    static Staff fromFileLine(const string &line) {
        Staff s;
        char buf[300];
        strcpy(buf, line.c_str());
        char* tok = strtok(buf, "|");
        int field = 0;
        char ph[20] = "N/A", em[60] = "N/A";
        while (tok != NULL) {
            switch (field) {
                case 0: s.setId(tok); break;
                case 1: s.setName(tok); break;
                case 2: s.setPassword(tok); break;
                case 3: s.setDepartment(tok); break;
                case 4: s.setPosition(tok); break;
                case 5: strcpy(ph, tok); break;
                case 6: strcpy(em, tok); break;
                default: break;
            }
            tok = strtok(NULL, "|");
            field++;
        }
        s.setContact(ContactInfo(ph, em));
        return s;
    }
};

/* ----------------------------------------------------------------------
   DERIVED CLASS 3: CourseResult : public Record
   Represents one student's result entry for one course.
   ---------------------------------------------------------------------- */
class CourseResult : public Record {
private:
    char studentId[MAX_ID_LEN];
    char courseCode[15];
    char courseName[MAX_COURSE_LEN];
    float marks;
    char grade[3];
    int creditHours;

public:
    CourseResult() : Record() {
        strcpy(studentId, "");
        strcpy(courseCode, "");
        strcpy(courseName, "");
        marks = 0.0f;
        strcpy(grade, "-");
        creditHours = 3;
    }
    CourseResult(int rid, const char* sid, const char* ccode, const char* cname,
                 float m, int credits)
        : Record(rid) {
        strcpy(studentId, sid);
        strcpy(courseCode, ccode);
        strcpy(courseName, cname);
        marks = m;
        creditHours = credits;
        computeGrade();
    }

    ~CourseResult() {
        // result record destroyed
    }

    void setStudentId(const char* sid) { strcpy(studentId, sid); }
    void setCourseCode(const char* c) { strcpy(courseCode, c); }
    void setCourseName(const char* c) { strcpy(courseName, c); }
    void setCreditHours(int c) { creditHours = c; }
    void setMarks(float m) { marks = m; computeGrade(); }

    string getStudentId() const { return string(studentId); }
    string getCourseCode() const { return string(courseCode); }
    string getCourseName() const { return string(courseName); }
    float getMarks() const { return marks; }
    string getGrade() const { return string(grade); }
    int getCreditHours() const { return creditHours; }

//turn marks into grade letter using te grading scale
    void computeGrade() {
        if (marks >= 80)      strcpy(grade, "A");
        else if (marks >= 75) strcpy(grade, "A-");
        else if (marks >= 70) strcpy(grade, "B+");
        else if (marks >= 65) strcpy(grade, "B");
        else if (marks >= 60) strcpy(grade, "B-");
        else if (marks >= 55) strcpy(grade, "C+");
        else if (marks >= 50) strcpy(grade, "C");
        else if (marks >= 45) strcpy(grade, "D");
        else if (marks >= 40) strcpy(grade, "E");
        else                   strcpy(grade, "F");
    }

//give the point for each grade so that the cpa can be count
    float gradePoint() const {
        if (strcmp(grade, "A") == 0)  return 4.0f;
        if (strcmp(grade, "A-") == 0) return 3.7f;
        if (strcmp(grade, "B+") == 0) return 3.3f;
        if (strcmp(grade, "B") == 0)  return 3.0f;
        if (strcmp(grade, "B-") == 0) return 2.7f;
        if (strcmp(grade, "C+") == 0) return 2.3f;
        if (strcmp(grade, "C") == 0)  return 2.0f;
        if (strcmp(grade, "D") == 0)  return 1.0f;
        if (strcmp(grade, "E") == 0)  return 0.5f;
        return 0.0f;
    }

    /* Overridden virtual display from Record base class */
    void display() const {
        cout << left << setw(5) << recordId << setw(10) << studentId << setw(8) << courseCode
             << setw(32) << courseName << setw(7) << marks << setw(7) << grade
             << creditHours << endl;
    }

    string toFileLine() const {
        stringstream ss;
        ss << recordId << "|" << studentId << "|" << courseCode << "|"
           << courseName << "|" << marks << "|" << grade << "|" << creditHours
           << "|" << (active ? 1 : 0);
        return ss.str();
    }

    static CourseResult fromFileLine(const string &line) {
        CourseResult r;
        char buf[300];
        strcpy(buf, line.c_str());
        char* tok = strtok(buf, "|");
        int field = 0;
        while (tok != NULL) {
            switch (field) {
                case 0: r.setRecordId(atoi(tok)); break;
                case 1: r.setStudentId(tok); break;
                case 2: r.setCourseCode(tok); break;
                case 3: r.setCourseName(tok); break;
                case 4: r.marks = (float)atof(tok); break;
                case 5: strcpy(r.grade, tok); break;
                case 6: r.setCreditHours(atoi(tok)); break;
                case 7: r.setActive(atoi(tok) == 1); break;
                default: break;
            }
            tok = strtok(NULL, "|");
            field++;
        }
        return r;
    }
};

/* ----------------------------------------------------------------------
   FRIEND FUNCTION DEFINITIONS (1 and 2) - operate on Person's protected data
   ---------------------------------------------------------------------- */
void printPersonShort(const Person &p) {
    cout << "[" << p.id << "] " << p.name << endl;
}

bool comparePersonByName(const Person &a, const Person &b) {
    return strcmp(a.name, b.name) < 0;
}
/* ============================================================================
   PART 3: DYNAMIC NON-PRIMITIVE (DNP) DATA STRUCTURES
   - Linked Queue (manually implemented, no STL) -> used for Remark Requests
   - Hash Table (manually implemented, no STL)    -> used for fast Student lookup
   ============================================================================ */

/* ----------------------------------------------------------------------
   Friend function 3 & 4 will be attached to these structures below
   ---------------------------------------------------------------------- */

/* ---------------- Linked Queue Node (Remark Request) ---------------- */
struct RemarkNode {
    char studentId[MAX_ID_LEN];
    char courseCode[15];
    char reason[120];
    Date requestDate;
    RemarkNode* next;

    RemarkNode(const char* sid, const char* ccode, const char* r, Date d) {
        strcpy(studentId, sid);
        strcpy(courseCode, ccode);
        strcpy(reason, r);
        requestDate = d;
        next = NULL;
    }
};

/* ----------------------------------------------------------------------
   CLASS: RemarkQueue (Linked Queue implementation - the required DNP structure)
   FIFO queue built entirely with pointers and 'new'/'delete' (dynamic memory).
   ---------------------------------------------------------------------- */
   
// i rename this from RemarkQueue to RequestList because it hold the student request, and i think this name is more clear for me

class RequestList {
private:
    RemarkNode* frontPtr;
    RemarkNode* rearPtr;
    int count;

public:
    RequestList() {
        frontPtr = NULL;
        rearPtr = NULL;
        count = 0;
    }

    /* Destructor - frees every node (dynamic memory operation: delete) */
    ~RequestList() {
        while (frontPtr != NULL) {
            RemarkNode* temp = frontPtr;
            frontPtr = frontPtr->next;
            delete temp;          // dynamic memory operation 1
        }
    }

    bool isEmpty() const { return frontPtr == NULL; }
    int size() const { return count; }

    //enqueue is when you put the new request right at the back of the queue, then update the rear pointer to point there 
    void enqueue(const char* sid, const char* ccode, const char* reason, Date d) {
        RemarkNode* newNode = new RemarkNode(sid, ccode, reason, d);
        if (rearPtr == NULL) {
            frontPtr = newNode;
            rearPtr = newNode;
        } else {
            rearPtr->next = newNode;
            rearPtr = newNode;
        }
        count++;
    }
//take out the request from the front and remove it
    bool dequeue(RemarkNode &out) {
        if (isEmpty()) return false;
        RemarkNode* temp = frontPtr;
        strcpy(out.studentId, temp->studentId);
        strcpy(out.courseCode, temp->courseCode);
        strcpy(out.reason, temp->reason);
        out.requestDate = temp->requestDate;
        frontPtr = frontPtr->next;
        if (frontPtr == NULL) rearPtr = NULL;
        delete temp;
        count--;
        return true;
    }

    void displayAll() const {
        if (isEmpty()) {
            cout << "No pending remark requests." << endl;
            return;
        }
        RemarkNode* curr = frontPtr;
        int i = 1;
        cout << "------------------------------------------------------" << endl;
        cout << "PENDING REMARK REQUESTS (Linked Queue - FIFO)" << endl;
        cout << "------------------------------------------------------" << endl;
        while (curr != NULL) {
            cout << i << ". Student: " << curr->studentId
                 << " | Course: " << curr->courseCode
                 << " | Date: " << curr->requestDate.toString()
                 << "\n   Reason: " << curr->reason << endl;
            curr = curr->next;
            i++;
        }
        cout << "------------------------------------------------------" << endl;
    }

    /* Friend function 3: counts requests belonging to a given student */
    friend int countRequestsByStudent(const RequestList &q, const string &sid);
};

int countRequestsByStudent(const RequestList &q, const string &sid) {
    int n = 0;
    RemarkNode* curr = q.frontPtr;     // access via friendship
    while (curr != NULL) {
        if (sid == string(curr->studentId)) n++;
        curr = curr->next;
    }
    return n;
}

/* ----------------------------------------------------------------------
   CLASS: StudentHashTable
   A manually implemented hash table (separate chaining) for O(1) average
   lookup of students by ID. This goes beyond the minimum DNP requirement
   and demonstrates an additional algorithmic technique.
   ---------------------------------------------------------------------- */
struct HashNode {
    char studentId[MAX_ID_LEN];
    int arrayIndex;     // index into the studentArray held by the manager
    HashNode* next;

    HashNode(const char* sid, int idx) {
        strcpy(studentId, sid);
        arrayIndex = idx;
        next = NULL;
    }
};

class StudentHashTable {
private:
    static const int TABLE_SIZE = 101;   // prime number reduces collisions
    HashNode* table[TABLE_SIZE];

// the original one times by 31, i change it to times by 37 and add the position i, so the student ID spread out better and less clash in the table
    int hashFunction(const string &key) const {
        unsigned long h = 7;                              // i start at 7 instead of 0
        for (size_t i = 0; i < key.size(); i++) {
            h = (h * 37) + (unsigned char)key[i] + i;     // mix the letter and its position
        }
        return (int)(h % TABLE_SIZE);                     // keep it inside the table size
    }
public:
    StudentHashTable() {
        for (int i = 0; i < TABLE_SIZE; i++) table[i] = NULL;
    }

    ~StudentHashTable() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            HashNode* curr = table[i];
            while (curr != NULL) {
                HashNode* temp = curr;
                curr = curr->next;
                delete temp;
            }
        }
    }

    void insert(const string &sid, int arrayIndex) {
        int idx = hashFunction(sid);
        HashNode* newNode = new HashNode(sid.c_str(), arrayIndex);
        newNode->next = table[idx];
        table[idx] = newNode;
    }

    /* Returns array index of student, or -1 if not found */
    int lookup(const string &sid) const {
        int idx = hashFunction(sid);
        HashNode* curr = table[idx];
        while (curr != NULL) {
            if (sid == string(curr->studentId)) return curr->arrayIndex;
            curr = curr->next;
        }
        return -1;
    }

    void remove(const string &sid) {
        int idx = hashFunction(sid);
        HashNode* curr = table[idx];
        HashNode* prev = NULL;
        while (curr != NULL) {
            if (sid == string(curr->studentId)) {
                if (prev == NULL) table[idx] = curr->next;
                else prev->next = curr->next;
                delete curr;
                return;
            }
            prev = curr;
            curr = curr->next;
        }
    }

    void clear() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            HashNode* curr = table[i];
            while (curr != NULL) {
                HashNode* temp = curr;
                curr = curr->next;
                delete temp;
            }
            table[i] = NULL;
        }
    }
};

/* ============================================================================
   PART 4: MANUAL SORTING ALGORITHMS (no STL, no built-in sort)
   ============================================================================ */

// i change quick sort to merge sort. merge sort split the list into two half,sort each half, then join (merge) them back together in order.i keep the same name quickSortByMarks so the rest of the program still work.
void mergeTwoParts(CourseResult arr[], int left, int mid, int right) 
{
    int n1 = mid - left + 1;       // size of left part
    int n2 = right - mid;          // size of right part
    CourseResult* L = new CourseResult[n1];   // temp arrays
    CourseResult* R = new CourseResult[n2];
    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) 
	{
        // i sort highest marks first, so take the bigger one
        if (L[i].getMarks() >= R[j].getMarks()) { arr[k] = L[i]; i++; }
        else { arr[k] = R[j]; j++; }
        k++;
    }
    while (i < n1) { arr[k] = L[i]; i++; k++; }   // leftover left part
    while (j < n2) { arr[k] = R[j]; j++; k++; }   // leftover right part

    delete[] L;   // free the temp memory
    delete[] R;
}

// this still call the name quickSortByMarks but inside it is merge sort
void quickSortByMarks(CourseResult arr[], int left, int right)
{
    if (left < right) 
	{
        int mid = left + (right - left) / 2;   // find middle
        quickSortByMarks(arr, left, mid);       // sort left half
        quickSortByMarks(arr, mid + 1, right);  // sort right half
        mergeTwoParts(arr, left, mid, right);   // join them in order
    }
}

/* ---------------- BUBBLE SORT: sorts CourseResult[] by course code (ascending) ------------ */
void bubbleSortByCourseCode(CourseResult arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j].getCourseCode() > arr[j + 1].getCourseCode()) {
                CourseResult temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                swapped = true;
            }
        }
        if (!swapped) break;   // already sorted, stop early
    }
}

/* ---------------- BUBBLE SORT (overloaded): sorts Student[] by name (ascending) ----------- */
void bubbleSortByCourseCode(Student arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j].getName() > arr[j + 1].getName()) {
                Student temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}

// the original one is selection sort and i change it to insertion sort cause it is more faster when sort.It take one studetn then move to the left until the student in front has higher gpd so the top of the list is the highest gpa
void selectionSortByGpa(Student arr[], int n) {

    for (int i = 1; i < n; i++) 
	{
        Student current = arr[i];      // the student we want to put in place
        int j = i - 1;
        while (j >= 0 && arr[j].getGpa() < current.getGpa()) 
		{
            arr[j + 1] = arr[j];       // push the smaller GPA student to the right
            j = j - 1;
        }
        arr[j + 1] = current;          // put our student in the correct place
    }
}

/* ============================================================================
   PART 5: MANUAL SEARCHING ALGORITHMS (no STL)
   ============================================================================ */

// i change the binary search so the function call itself again and again.every time it look at the middle ID. if the middle is too small it search the right side, if too big it search the left side, until it find the ID.
int binarySearchHelper(Student arr[], int low, int high, const string &targetId) 
{
    if (low > high) return -1;                       // empty list, ID not here
    int mid = low + (high - low) / 2;                // find the middle
    if (arr[mid].getId() == targetId) return mid;    // found it
    else if (arr[mid].getId() < targetId)            // middle is too small
        return binarySearchHelper(arr, mid + 1, high, targetId);   // search right side
    else                                             // middle is too big
        return binarySearchHelper(arr, low, mid - 1, targetId);    // search left side
}

// this one start the search, the program still call this name
int binarySearchStudentById(Student arr[], int n, const string &targetId) {
    return binarySearchHelper(arr, 0, n - 1, targetId);
}

/* Helper: insertion sort by ID (ascending) - needed before binary search runs */
void insertionSortById(Student arr[], int n) {
    for (int i = 1; i < n; i++) {
        Student key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j].getId() > key.getId()) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

/* ---------------- LINEAR SEARCH: partial, case-insensitive name match --------------------- */
string toLowerStr(const string &s) {
    string r = s;
    for (size_t i = 0; i < r.size(); i++) {
        if (r[i] >= 'A' && r[i] <= 'Z') r[i] = r[i] - 'A' + 'a';
    }
    return r;
}
//search name by checking one by one from start
int linearSearchStudentByName(Student arr[], int n, const string &keyword, int results[], int maxResults) {
    int found = 0;
    string lowKey = toLowerStr(keyword);
    for (int i = 0; i < n && found < maxResults; i++) {
        string lowName = toLowerStr(arr[i].getName());
         if (lowName.find(lowKey) == 0) {  // i change it to match names that start with the keyword
            results[found] = i;
            found++;
        }
    }
    return found;
}

/* ---------------- LINEAR SEARCH (overloaded): search CourseResult[] by courseCode --------- */
int linearSearchResultsByCourse(CourseResult arr[], int n, const string &code, int results[], int maxResults) {
    int found = 0;
    for (int i = 0; i < n && found < maxResults; i++) {
        if (arr[i].getCourseCode() == code) {
            results[found] = i;
            found++;
        }
    }
    return found;
}
/* ============================================================================
   PART 3B: ATTENDANCE TRACKING FEATURE (Additional / Bonus Functionality)
   Adds a real, usable feature beyond the minimum requirements:
   - Struct: AttendanceRecord-style data stored via class AttendanceEntry
   - A second DNP structure: a Linked Stack used as an "Activity Log"
     (records the most recent system actions, LIFO order) - demonstrates
     understanding of stacks in addition to the required queue.
   - Extra sorting/searching practice on a second dataset (attendance %).
   ============================================================================ */

/* ---------------- Linked Stack Node (Activity Log) ---------------- */
struct ActivityNode {
    char description[150];
    Date when;
    ActivityNode* next;

    ActivityNode(const char* desc, Date d) {
        strcpy(description, desc);
        when = d;
        next = NULL;
    }
};

/* ----------------------------------------------------------------------
   CLASS: ActivityLogStack
   A manually implemented Linked Stack (LIFO). Every significant action
   (login, add record, delete record, etc.) can be pushed here so staff
   can review "what happened most recently" first. This is an additional
   DNP data structure beyond the single one required by the rubric.
   ---------------------------------------------------------------------- */
class ActivityLogStack {
private:
    ActivityNode* top;
    int count;
    static const int MAX_LOG_DISPLAY = 15;

public:
    ActivityLogStack() {
        top = NULL;
        count = 0;
    }

    ~ActivityLogStack() {
        while (top != NULL) {
            ActivityNode* temp = top;
            top = top->next;
            delete temp;          // dynamic memory cleanup
        }
    }

    bool isEmpty() const { return top == NULL; }
    int size() const { return count; }

    void push(const string &desc, Date d) {
        ActivityNode* newNode = new ActivityNode(desc.c_str(), d);  // dynamic memory
        newNode->next = top;
        top = newNode;
        count++;
    }

    bool pop(ActivityNode &out) {
        if (isEmpty()) return false;
        ActivityNode* temp = top;
        strcpy(out.description, temp->description);
        out.when = temp->when;
        top = top->next;
        delete temp;
        count--;
        return true;
    }

    void displayRecent() const {
        if (isEmpty()) {
            cout << "No activity logged yet.\n";
            return;
        }
        ActivityNode* curr = top;
        int shown = 0;
        cout << "------------------------------------------------------" << endl;
        cout << "RECENT ACTIVITY LOG (Linked Stack - LIFO, most recent first)" << endl;
        cout << "------------------------------------------------------" << endl;
        while (curr != NULL && shown < MAX_LOG_DISPLAY) {
            cout << "[" << curr->when.toString() << "] " << curr->description << endl;
            curr = curr->next;
            shown++;
        }
        cout << "------------------------------------------------------" << endl;
    }

    /* Friend function 4: checks whether a given description substring
       appears anywhere in the current log (e.g. audit search) */
    friend bool logContains(const ActivityLogStack &stack, const string &keyword);
};

bool logContains(const ActivityLogStack &stack, const string &keyword) {
    ActivityNode* curr = stack.top;     // accesses private member via friendship
    string lowKey = keyword;
    for (size_t i = 0; i < lowKey.size(); i++) {
        if (lowKey[i] >= 'A' && lowKey[i] <= 'Z') lowKey[i] = lowKey[i] - 'A' + 'a';
    }
    while (curr != NULL) {
        string desc = string(curr->description);
        string lowDesc = desc;
        for (size_t i = 0; i < lowDesc.size(); i++) {
            if (lowDesc[i] >= 'A' && lowDesc[i] <= 'Z') lowDesc[i] = lowDesc[i] - 'A' + 'a';
        }
        if (lowDesc.find(lowKey) != string::npos) return true;
        curr = curr->next;
    }
    return false;
}

/* ----------------------------------------------------------------------
   CLASS: AttendanceEntry
   Tracks a student's attendance percentage per course. Demonstrates an
   additional small record type with its own sort/search utilities,
   reinforcing the "additional features" bonus criteria.
   ---------------------------------------------------------------------- */
class AttendanceEntry {
private:
    char studentId[MAX_ID_LEN];
    char courseCode[15];
    int classesHeld;
    int classesAttended;

public:
    AttendanceEntry() {
        strcpy(studentId, "");
        strcpy(courseCode, "");
        classesHeld = 0;
        classesAttended = 0;
    }
    AttendanceEntry(const char* sid, const char* ccode, int held, int attended) {
        strcpy(studentId, sid);
        strcpy(courseCode, ccode);
        classesHeld = held;
        classesAttended = attended;
    }

    string getStudentId() const { return string(studentId); }
    string getCourseCode() const { return string(courseCode); }
    int getClassesHeld() const { return classesHeld; }
    int getClassesAttended() const { return classesAttended; }

    float attendancePercent() const {
        if (classesHeld == 0) return 0.0f;
        return ((float)classesAttended / (float)classesHeld) * 100.0f;
    }

    bool meetsMinimumAttendance() const {
        return attendancePercent() >= 80.0f;   // typical university policy threshold
    }

    void display() const {
        stringstream attBuf; attBuf << classesAttended << "/" << classesHeld;
        stringstream pctBuf; pctBuf << fixed << setprecision(1) << attendancePercent() << "%";
        cout << left << setw(10) << studentId << setw(8) << courseCode
             << setw(10) << attBuf.str() << setw(10) << pctBuf.str()
             << (meetsMinimumAttendance() ? "OK" : "WARNING") << endl;
    }

    string toFileLine() const {
        stringstream ss;
        ss << studentId << "|" << courseCode << "|" << classesHeld << "|" << classesAttended;
        return ss.str();
    }

    static AttendanceEntry fromFileLine(const string &line) {
        char buf[150];
        strcpy(buf, line.c_str());
        char sid[MAX_ID_LEN] = "", code[15] = "";
        int held = 0, attended = 0;
        char* tok = strtok(buf, "|");
        int field = 0;
        while (tok != NULL) {
            if (field == 0) strcpy(sid, tok);
            else if (field == 1) strcpy(code, tok);
            else if (field == 2) held = atoi(tok);
            else if (field == 3) attended = atoi(tok);
            tok = strtok(NULL, "|");
            field++;
        }
        return AttendanceEntry(sid, code, held, attended);
    }
};

/* ---------------- Sorting: Bubble Sort on AttendanceEntry[] by percentage ---------------- */
void bubbleSortByAttendance(AttendanceEntry arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j].attendancePercent() < arr[j + 1].attendancePercent()) {
                AttendanceEntry temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}

/* ---------------- Searching: Linear Search on AttendanceEntry[] by studentId ---------------- */
int linearSearchAttendanceByStudent(AttendanceEntry arr[], int n, const string &sid, int results[], int maxOut) {
    int found = 0;
    for (int i = 0; i < n && found < maxOut; i++) {
        if (arr[i].getStudentId() == sid) {
            results[found] = i;
            found++;
        }
    }
    return found;
}
/* ============================================================================
   PART 6: SYSTEM MANAGER
   Central class that owns all in-memory arrays, loads/saves all 5 .txt files,
   and provides the operations used by both the Staff and Customer (Student)
   modules. Keeping this logic in one place is what guarantees "record
   consistency" between modules (Requirement: updates visible to both sides).
   ============================================================================ */

/* Custom exception classes used with try/catch (Requirement 10) */
class FileException {
private:
    string message;
public:
    FileException(string msg) { message = msg; }
    string what() const { return message; }
};

class ValidationException {
private:
    string message;
public:
    ValidationException(string msg) { message = msg; }
    string what() const { return message; }
};

class NotFoundException {
private:
    string message;
public:
    NotFoundException(string msg) { message = msg; }
    string what() const { return message; }
};

class SystemManager {
private:
    Student   students[MAX_STUDENTS];
    int       studentCount;

    Staff     staffs[MAX_STAFF];
    int       staffCount;

    CourseResult results[MAX_RESULTS];
    int       resultCount;

    char      courseCatalog[MAX_COURSES][MAX_COURSE_LEN];
    char      courseCodes[MAX_COURSES][15];
    int       courseCreditHrs[MAX_COURSES];
    int       courseCount;

    RequestList requestList;
    StudentHashTable studentIndex;
    ActivityLogStack activityLog;

    AttendanceEntry attendance[MAX_RESULTS];
    int       attendanceCount;

    int       nextResultId;

public:
    SystemManager() {
        studentCount = 0;
        staffCount = 0;
        resultCount = 0;
        courseCount = 0;
        attendanceCount = 0;
        nextResultId = 1;
    }

    ~SystemManager() {
        // All member objects (arrays, RequestList, StudentHashTable)
        // clean up automatically via their own destructors.
    }

    /* ============================ FILE LOADING ============================ */

    void loadAll() {
        loadStudents();
        loadStaff();
        loadCourses();
        loadResults();
        loadAttendance();
        rebuildStudentIndex();
    }

    void rebuildStudentIndex() {
        studentIndex.clear();
        for (int i = 0; i < studentCount; i++) {
            studentIndex.insert(students[i].getId(), i);
        }
    }

    void loadStudents() {
        try {
            ifstream fin(STUDENT_FILE);
            if (!fin.is_open()) {
                throw FileException("Could not open " + string(STUDENT_FILE) + " (will be created on save).");
            }
            string line;
            studentCount = 0;
            while (getline(fin, line) && studentCount < MAX_STUDENTS) {
                if (line.length() == 0) continue;
                students[studentCount] = Student::fromFileLine(line);
                studentCount++;
            }
            fin.close();
        } catch (FileException &e) {
            cout << "[Notice] " << e.what() << endl;
        }
    }

    void loadStaff() {
        try {
            ifstream fin(STAFF_FILE);
            if (!fin.is_open()) {
                throw FileException("Could not open " + string(STAFF_FILE) + " (will be created on save).");
            }
            string line;
            staffCount = 0;
            while (getline(fin, line) && staffCount < MAX_STAFF) {
                if (line.length() == 0) continue;
                staffs[staffCount] = Staff::fromFileLine(line);
                staffCount++;
            }
            fin.close();
        } catch (FileException &e) {
            cout << "[Notice] " << e.what() << endl;
        }
    }

    void loadCourses() {
        try {
            ifstream fin(COURSE_FILE);
            if (!fin.is_open()) {
                throw FileException("Could not open " + string(COURSE_FILE) + " (will be created on save).");
            }
            string line;
            courseCount = 0;
            while (getline(fin, line) && courseCount < MAX_COURSES) {
                if (line.length() == 0) continue;
                char buf[200];
                strcpy(buf, line.c_str());
                char* tok = strtok(buf, "|");
                int field = 0;
                while (tok != NULL) {
                    if (field == 0) strcpy(courseCodes[courseCount], tok);
                    else if (field == 1) strcpy(courseCatalog[courseCount], tok);
                    else if (field == 2) courseCreditHrs[courseCount] = atoi(tok);
                    tok = strtok(NULL, "|");
                    field++;
                }
                courseCount++;
            }
            fin.close();
        } catch (FileException &e) {
            cout << "[Notice] " << e.what() << endl;
        }
    }

    void loadResults() {
        try {
            ifstream fin(RESULT_FILE);
            if (!fin.is_open()) {
                throw FileException("Could not open " + string(RESULT_FILE) + " (will be created on save).");
            }
            string line;
            resultCount = 0;
            int maxId = 0;
            while (getline(fin, line) && resultCount < MAX_RESULTS) {
                if (line.length() == 0) continue;
                results[resultCount] = CourseResult::fromFileLine(line);
                if (results[resultCount].getRecordId() > maxId) maxId = results[resultCount].getRecordId();
                resultCount++;
            }
            fin.close();
            nextResultId = maxId + 1;
        } catch (FileException &e) {
            cout << "[Notice] " << e.what() << endl;
        }
    }

    void loadAttendance() {
        try {
            ifstream fin(ATTENDANCE_FILE);
            if (!fin.is_open()) {
                throw FileException("Could not open " + string(ATTENDANCE_FILE) + " (will be created on save).");
            }
            string line;
            attendanceCount = 0;
            while (getline(fin, line) && attendanceCount < MAX_RESULTS) {
                if (line.length() == 0) continue;
                attendance[attendanceCount] = AttendanceEntry::fromFileLine(line);
                attendanceCount++;
            }
            fin.close();
        } catch (FileException &e) {
            cout << "[Notice] " << e.what() << endl;
        }
    }

    /* ============================ FILE SAVING ============================ */

    void saveStudents() {
        try {
            ofstream fout(STUDENT_FILE);
            if (!fout.is_open()) throw FileException("Cannot write to " + string(STUDENT_FILE));
            for (int i = 0; i < studentCount; i++) {
                fout << students[i].toFileLine() << endl;
            }
            fout.close();
        } catch (FileException &e) {
            cout << "[Error] " << e.what() << endl;
        }
    }

    void saveStaff() {
        try {
            ofstream fout(STAFF_FILE);
            if (!fout.is_open()) throw FileException("Cannot write to " + string(STAFF_FILE));
            for (int i = 0; i < staffCount; i++) {
                fout << staffs[i].toFileLine() << endl;
            }
            fout.close();
        } catch (FileException &e) {
            cout << "[Error] " << e.what() << endl;
        }
    }

    void saveCourses() {
        try {
            ofstream fout(COURSE_FILE);
            if (!fout.is_open()) throw FileException("Cannot write to " + string(COURSE_FILE));
            for (int i = 0; i < courseCount; i++) {
                fout << courseCodes[i] << "|" << courseCatalog[i] << "|" << courseCreditHrs[i] << endl;
            }
            fout.close();
        } catch (FileException &e) {
            cout << "[Error] " << e.what() << endl;
        }
    }

    void saveResults() {
        try {
            ofstream fout(RESULT_FILE);
            if (!fout.is_open()) throw FileException("Cannot write to " + string(RESULT_FILE));
            for (int i = 0; i < resultCount; i++) {
                fout << results[i].toFileLine() << endl;
            }
            fout.close();
        } catch (FileException &e) {
            cout << "[Error] " << e.what() << endl;
        }
    }

    void saveAttendance() {
        try {
            ofstream fout(ATTENDANCE_FILE);
            if (!fout.is_open()) throw FileException("Cannot write to " + string(ATTENDANCE_FILE));
            for (int i = 0; i < attendanceCount; i++) {
                fout << attendance[i].toFileLine() << endl;
            }
            fout.close();
        } catch (FileException &e) {
            cout << "[Error] " << e.what() << endl;
        }
    }

    void saveAll() {
        saveStudents();
        saveStaff();
        saveCourses();
        saveResults();
        saveAttendance();
    }

    /* ============================ VALIDATION HELPERS ============================ */

    bool isValidId(const string &id) const {
        if (id.length() < 3 || id.length() >= MAX_ID_LEN) return false;
        return true;
    }

    /* Manual validation: phone number should contain only digits and be 8-15 chars */
    bool isValidPhone(const string &phone) const {
        if (phone.length() < 8 || phone.length() > 15) return false;
        for (size_t i = 0; i < phone.length(); i++) {
            if (phone[i] < '0' || phone[i] > '9') return false;
        }
        return true;
    }

    /* Manual validation: email must contain exactly one '@' and at least one '.' after it */
    bool isValidEmail(const string &email) const {
        int atPos = -1;
        int atCount = 0;
        for (size_t i = 0; i < email.length(); i++) {
            if (email[i] == '@') {
                atPos = (int)i;
                atCount++;
            }
        }
        if (atCount != 1) return false;
        if (atPos <= 0 || atPos >= (int)email.length() - 1) return false;
        bool dotAfterAt = false;
        for (size_t i = atPos + 1; i < email.length(); i++) {
            if (email[i] == '.') dotAfterAt = true;
        }
        return dotAfterAt;
    }

    /* Manual validation: marks must be a number between 0 and 100 inclusive */
    bool isValidMarks(float marks) const {
        return marks >= 0.0f && marks <= 100.0f;
    }

    /* Manual validation: name should contain only letters, spaces, '/', and '.' (for titles like Dr.) */
    bool isValidName(const string &name) const {
        if (name.length() == 0 || name.length() >= MAX_NAME_LEN) return false;
        for (size_t i = 0; i < name.length(); i++) {
            char c = name[i];
            bool isLetter = (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
            bool isAllowedSymbol = (c == ' ' || c == '/' || c == '.');
            if (!isLetter && !isAllowedSymbol) return false;
        }
        return true;
    }

    bool studentIdExists(const string &id) const {
        return studentIndex.lookup(id) != -1;
    }

    bool staffIdExists(const string &id) const {
        for (int i = 0; i < staffCount; i++) {
            if (staffs[i].getId() == id) return true;
        }
        return false;
    }

    bool courseCodeExists(const string &code) const {
        for (int i = 0; i < courseCount; i++) {
            if (string(courseCodes[i]) == code) return true;
        }
        return false;
    }

    int findCourseIndex(const string &code) const {
        for (int i = 0; i < courseCount; i++) {
            if (string(courseCodes[i]) == code) return i;
        }
        return -1;
    }
    
     string generateStudentId() 
	 {
        int maxNum = -1;
        for (int i = 0; i < studentCount; i++) {
            string sid = students[i].getId();
            if (sid.length() > 1 && sid[0] == 'S') {
                int num = atoi(sid.substr(1).c_str());
                if (num > maxNum) maxNum = num;
            }
        }
        int next = maxNum + 1;
        stringstream ss;
        ss << "S";
        if (next < 1000) ss << "0";
        if (next < 100)  ss << "0";
        if (next < 10)   ss << "0";
        ss << next;
        return ss.str();
    }

    /* ============================ REGISTRATION ============================ */
//make new student, check input valid and no same ID
    bool registerStudent(const string &id, const string &name, const string &pass,
                          const string &programme, const string &phone, const string &email) {
        try {
            if (!isValidId(id)) throw ValidationException("Invalid ID format (min 3 characters).");
            if (studentIdExists(id)) throw ValidationException("Student ID already exists.");
            if (!isValidName(name)) throw ValidationException("Name must contain only letters and spaces.");
            if (pass.length() < 4) throw ValidationException("Password must be at least 4 characters.");
            if (!isValidPhone(phone)) throw ValidationException("Phone number must be 8-15 digits.");
            if (!isValidEmail(email)) throw ValidationException("Email format is invalid.");
            if (studentCount >= MAX_STUDENTS) throw ValidationException("Student capacity full.");

            Student s(id.c_str(), name.c_str(), pass.c_str(), programme.c_str());
            s.setContact(ContactInfo(phone.c_str(), email.c_str()));
            time_t t = time(0);
            tm* now = localtime(&t);
            s.setRegisteredOn(Date(now->tm_mday, now->tm_mon + 1, now->tm_year + 1900));

            students[studentCount] = s;
            studentIndex.insert(id, studentCount);
            studentCount++;
            saveStudents();
            logActivity("New student registered: " + id + " (" + name + ")");
            return true;
        } catch (ValidationException &e) {
            cout << "[Registration Failed] " << e.what() << endl;
            return false;
        }
    }

    bool registerStaff(const string &id, const string &name, const string &pass,
                        const string &dept, const string &position) {
        try {
            if (!isValidId(id)) throw ValidationException("Invalid ID format (min 3 characters).");
            if (staffIdExists(id)) throw ValidationException("Staff ID already exists.");
            if (name.length() == 0) throw ValidationException("Name cannot be empty.");
            if (pass.length() < 4) throw ValidationException("Password must be at least 4 characters.");
            if (staffCount >= MAX_STAFF) throw ValidationException("Staff capacity full.");

            Staff s(id.c_str(), name.c_str(), pass.c_str(), dept.c_str(), position.c_str());
            staffs[staffCount] = s;
            staffCount++;
            saveStaff();
            return true;
        } catch (ValidationException &e) {
            cout << "[Registration Failed] " << e.what() << endl;
            return false;
        }
    }

    /* ============================ LOGIN ============================ */

    int loginStudent(const string &id, const string &pass) {
        int idx = studentIndex.lookup(id);
        if (idx == -1) return -1;
        if (!students[idx].checkPassword(pass)) return -2;
        logActivity("Student login: " + id);
        return idx;
    }
//check staff ID and password is correct or not
    int loginStaff(const string &id, const string &pass) {
        for (int i = 0; i < staffCount; i++) {
            if (staffs[i].getId() == id) {
                if (!staffs[i].checkPassword(pass)) return -2;
                logActivity("Staff login: " + id);
                return i;
            }
        }
        return -1;
    }

    /* ============================ COURSE MANAGEMENT (Staff) ============================ */

    bool addCourse(const string &code, const string &name, int credits) {
        try {
            if (courseCount >= MAX_COURSES) throw ValidationException("Course capacity full.");
            if (courseCodeExists(code)) throw ValidationException("Course code already exists.");
            if (code.length() == 0 || name.length() == 0) throw ValidationException("Course code/name cannot be empty.");
            strcpy(courseCodes[courseCount], code.c_str());
            strcpy(courseCatalog[courseCount], name.c_str());
            courseCreditHrs[courseCount] = credits;
            courseCount++;
            saveCourses();
            logActivity("Course added: " + code + " (" + name + ")");
            return true;
        } catch (ValidationException &e) {
            cout << "[Add Course Failed] " << e.what() << endl;
            return false;
        }
    }

    bool editCourse(const string &code, const string &newName, int newCredits) {
        try {
            int idx = findCourseIndex(code);
            if (idx == -1) throw NotFoundException("Course code not found.");
            strcpy(courseCatalog[idx], newName.c_str());
            courseCreditHrs[idx] = newCredits;
            saveCourses();
            logActivity("Course edited: " + code);
            return true;
        } catch (NotFoundException &e) {
            cout << "[Edit Course Failed] " << e.what() << endl;
            return false;
        }
    }

    bool deleteCourse(const string &code) {
        try {
            int idx = findCourseIndex(code);
            if (idx == -1) throw NotFoundException("Course code not found.");
            for (int i = idx; i < courseCount - 1; i++) {
                strcpy(courseCodes[i], courseCodes[i + 1]);
                strcpy(courseCatalog[i], courseCatalog[i + 1]);
                courseCreditHrs[i] = courseCreditHrs[i + 1];
            }
            courseCount--;
            saveCourses();
            logActivity("Course deleted: " + code);
            return true;
        } catch (NotFoundException &e) {
            cout << "[Delete Course Failed] " << e.what() << endl;
            return false;
        }
    }

    void displayAllCourses() const {
        cout << "------------------------------------------------------" << endl;
        cout << left << setw(8) << "Code" << setw(34) << "Course Name" << "Credit Hours" << endl;
        cout << "------------------------------------------------------" << endl;
        for (int i = 0; i < courseCount; i++) {
            cout << left << setw(8) << courseCodes[i] << setw(34) << courseCatalog[i] << courseCreditHrs[i] << endl;
        }
        cout << "------------------------------------------------------" << endl;
    }

    /* ============================ RESULT MANAGEMENT (Staff) ============================ */

    bool addResult(const string &studentId, const string &courseCode, float marks) {
        try {
            if (!studentIdExists(studentId)) throw NotFoundException("Student ID not found.");
            int cIdx = findCourseIndex(courseCode);
            if (cIdx == -1) throw NotFoundException("Course code not found.");
            if (!isValidMarks(marks)) throw ValidationException("Marks must be between 0 and 100.");
            if (resultCount >= MAX_RESULTS) throw ValidationException("Result capacity full.");

            CourseResult r(nextResultId, studentId.c_str(), courseCode.c_str(),
                            courseCatalog[cIdx], marks, courseCreditHrs[cIdx]);
            results[resultCount] = r;
            resultCount++;
            nextResultId++;
            saveResults();
            recalcStudentGpa(studentId);
            logActivity("Result added for student " + studentId + " in course " + courseCode);
            return true;
        } catch (NotFoundException &e) {
            cout << "[Add Result Failed] " << e.what() << endl;
            return false;
        } catch (ValidationException &e) {
            cout << "[Add Result Failed] " << e.what() << endl;
            return false;
        }
    }

    bool editResult(int recordId, float newMarks) {
        try {
            int idx = -1;
            for (int i = 0; i < resultCount; i++) {
                if (results[i].getRecordId() == recordId) { idx = i; break; }
            }
            if (idx == -1) throw NotFoundException("Result record not found.");
            if (!isValidMarks(newMarks)) throw ValidationException("Marks must be between 0 and 100.");
            results[idx].setMarks(newMarks);
            saveResults();
            recalcStudentGpa(results[idx].getStudentId());
            {
                stringstream logss;
                logss << "Result edited: record #" << recordId << " new marks " << newMarks;
                logActivity(logss.str());
            }
            return true;
        } catch (NotFoundException &e) {
            cout << "[Edit Result Failed] " << e.what() << endl;
            return false;
        } catch (ValidationException &e) {
            cout << "[Edit Result Failed] " << e.what() << endl;
            return false;
        }
    }

    bool deleteResult(int recordId) {
        try {
            int idx = -1;
            for (int i = 0; i < resultCount; i++) {
                if (results[i].getRecordId() == recordId) { idx = i; break; }
            }
            if (idx == -1) throw NotFoundException("Result record not found.");
            string sid = results[idx].getStudentId();
            for (int i = idx; i < resultCount - 1; i++) {
                results[i] = results[i + 1];
            }
            resultCount--;
            saveResults();
            recalcStudentGpa(sid);
            logActivity("Result deleted for student " + sid);
            return true;
        } catch (NotFoundException &e) {
            cout << "[Delete Result Failed] " << e.what() << endl;
            return false;
        }
    }
//count the gpa again after the mark change
    void recalcStudentGpa(const string &studentId) {
        int sIdx = studentIndex.lookup(studentId);
        if (sIdx == -1) return;
        float totalPoints = 0;
        int totalCredits = 0;
        for (int i = 0; i < resultCount; i++) {
            if (results[i].getStudentId() == studentId && results[i].isActive()) {
                totalPoints += results[i].gradePoint() * results[i].getCreditHours();
                totalCredits += results[i].getCreditHours();
            }
        }
        float gpa = (totalCredits > 0) ? (totalPoints / totalCredits) : 0.0f;
        students[sIdx].setGpa(gpa);
        students[sIdx].setTotalCredits(totalCredits);
        saveStudents();
    }

    /* ============================ GETTERS for module classes ============================ */

    Student* getStudentsArray() { return students; }
    int getStudentCount() const { return studentCount; }

    Staff* getStaffArray() { return staffs; }
    int getStaffCount() const { return staffCount; }

    CourseResult* getResultsArray() { return results; }
    int getResultCount() const { return resultCount; }

    int getCourseCount() const { return courseCount; }
    string getCourseCodeAt(int i) const { return string(courseCodes[i]); }
    string getCourseNameAt(int i) const { return string(courseCatalog[i]); }
    int getCourseCreditAt(int i) const { return courseCreditHrs[i]; }

    RequestList& getRequestList() { return requestList; }
    StudentHashTable& getStudentIndex() { return studentIndex; }

    int findStudentIndexById(const string &id) const {
        return studentIndex.lookup(id);
    }

    int getResultsForStudent(const string &studentId, CourseResult outArr[], int maxOut) const {
        int n = 0;
        for (int i = 0; i < resultCount && n < maxOut; i++) {
            if (results[i].getStudentId() == studentId && results[i].isActive()) {
                outArr[n] = results[i];
                n++;
            }
        }
        return n;
    }

    /* ============================ ATTENDANCE MANAGEMENT (additional feature) ============================ */

    bool recordAttendance(const string &sid, const string &courseCode, int held, int attended) {
        try {
            if (!studentIdExists(sid)) throw NotFoundException("Student ID not found.");
            if (findCourseIndex(courseCode) == -1) throw NotFoundException("Course code not found.");
            if (attended > held) throw ValidationException("Classes attended cannot exceed classes held.");
            if (held < 0 || attended < 0) throw ValidationException("Values cannot be negative.");
            if (attendanceCount >= MAX_RESULTS) throw ValidationException("Attendance capacity full.");

            /* Update existing entry if one already exists for this student+course */
            for (int i = 0; i < attendanceCount; i++) {
                if (attendance[i].getStudentId() == sid && attendance[i].getCourseCode() == courseCode) {
                    attendance[i] = AttendanceEntry(sid.c_str(), courseCode.c_str(), held, attended);
                    saveAttendance();
                    logActivity("Updated attendance for " + sid + " in " + courseCode);
                    return true;
                }
            }
            attendance[attendanceCount] = AttendanceEntry(sid.c_str(), courseCode.c_str(), held, attended);
            attendanceCount++;
            saveAttendance();
            logActivity("Recorded attendance for " + sid + " in " + courseCode);
            return true;
        } catch (NotFoundException &e) {
            cout << "[Attendance Failed] " << e.what() << endl;
            return false;
        } catch (ValidationException &e) {
            cout << "[Attendance Failed] " << e.what() << endl;
            return false;
        }
    }

    int getAttendanceForStudent(const string &sid, AttendanceEntry outArr[], int maxOut) const {
        int n = 0;
        for (int i = 0; i < attendanceCount && n < maxOut; i++) {
            if (attendance[i].getStudentId() == sid) {
                outArr[n] = attendance[i];
                n++;
            }
        }
        return n;
    }

    AttendanceEntry* getAttendanceArray() { return attendance; }
    int getAttendanceCount() const { return attendanceCount; }

    /* ============================ ACTIVITY LOG (Linked Stack helper) ============================ */

    void logActivity(const string &desc) {
        time_t t = time(0);
        tm* now = localtime(&t);
        Date today(now->tm_mday, now->tm_mon + 1, now->tm_year + 1900);
        activityLog.push(desc, today);
    }

    ActivityLogStack& getActivityLog() { return activityLog; }

    /* ============================ CLASS RANKING & ANALYTICS (additional feature) ============================ */

    /* Computes the rank (1 = highest) of a student within a specific course,
       based on marks, using the manually implemented quick sort above. */
    int getStudentRankInCourse(const string &studentId, const string &courseCode) const {
        CourseResult courseResults[MAX_RESULTS];
        int n = 0;
        for (int i = 0; i < resultCount; i++) {
            if (results[i].getCourseCode() == courseCode && results[i].isActive()) {
                courseResults[n] = results[i];
                n++;
            }
        }
        if (n == 0) return -1;

        CourseResult* sortable = new CourseResult[n];
        for (int i = 0; i < n; i++) sortable[i] = courseResults[i];
        quickSortByMarks(sortable, 0, n - 1);   // descending by marks

        int rank = -1;
        for (int i = 0; i < n; i++) {
            if (sortable[i].getStudentId() == studentId) {
                rank = i + 1;
                break;
            }
        }
        delete[] sortable;
        return rank;
    }

    int getCourseEnrollmentCount(const string &courseCode) const {
        int n = 0;
        for (int i = 0; i < resultCount; i++) {
            if (results[i].getCourseCode() == courseCode && results[i].isActive()) n++;
        }
        return n;
    }

    float getCourseAverageMarks(const string &courseCode) const {
        float sum = 0;
        int n = 0;
        for (int i = 0; i < resultCount; i++) {
            if (results[i].getCourseCode() == courseCode && results[i].isActive()) {
                sum += results[i].getMarks();
                n++;
            }
        }
        return (n > 0) ? (sum / n) : 0.0f;
    }

    /* Computes the percentile standing of a student's GPA among all students,
       using the manually implemented selection sort above. Percentile here
       means "percentage of students this student's GPA is greater than or
       equal to" - a common academic ranking statistic. */
    float getStudentGpaPercentile(const string &studentId) const {
        if (studentCount == 0) return 0.0f;
        Student* sortable = new Student[studentCount];
        for (int i = 0; i < studentCount; i++) sortable[i] = students[i];
        selectionSortByGpa(sortable, studentCount);   // descending by GPA

        int position = -1;
        for (int i = 0; i < studentCount; i++) {
            if (sortable[i].getId() == studentId) {
                position = i;
                break;
            }
        }
        delete[] sortable;
        if (position == -1) return 0.0f;

        float studentsBelow = (float)(studentCount - position - 1);
        return (studentsBelow / (float)studentCount) * 100.0f;
    }

    void displayCourseAnalytics(const string &courseCode) const {
        int idx = findCourseIndex(courseCode);
        if (idx == -1) {
            cout << "Course not found.\n";
            return;
        }
        int enrolled = getCourseEnrollmentCount(courseCode);
        float avg = getCourseAverageMarks(courseCode);

        cout << "------------------------------------------------------" << endl;
        cout << "COURSE ANALYTICS: " << courseCodes[idx] << " - " << courseCatalog[idx] << endl;
        cout << "------------------------------------------------------" << endl;
        cout << "Enrolled Students : " << enrolled << endl;
        cout << "Average Marks     : " << avg << endl;

        if (enrolled > 0) {
            CourseResult courseResults[MAX_RESULTS];
            int n = 0;
            for (int i = 0; i < resultCount; i++) {
                if (results[i].getCourseCode() == courseCode && results[i].isActive()) {
                    courseResults[n] = results[i];
                    n++;
                }
            }
            CourseResult* sortable = new CourseResult[n];
            for (int i = 0; i < n; i++) sortable[i] = courseResults[i];
            quickSortByMarks(sortable, 0, n - 1);

            cout << "Top performer     : " << sortable[0].getStudentId()
                 << " (" << sortable[0].getMarks() << " marks, grade " << sortable[0].getGrade() << ")" << endl;
            cout << "Lowest performer  : " << sortable[n - 1].getStudentId()
                 << " (" << sortable[n - 1].getMarks() << " marks, grade " << sortable[n - 1].getGrade() << ")" << endl;
            delete[] sortable;
        }
        cout << "------------------------------------------------------" << endl;
    }

    /* ============================ AT-RISK STUDENT DETECTION (additional feature) ============================ */

       
       //check if student weak (low GPA or attendance)
    bool isStudentAtRisk(const string &studentId) const {
        int sIdx = studentIndex.lookup(studentId);
        if (sIdx == -1) return false;

        if (students[sIdx].getGpa() < 2.0f && students[sIdx].getTotalCredits() > 0) {
            return true;
        }
        for (int i = 0; i < attendanceCount; i++) {
            if (attendance[i].getStudentId() == studentId && !attendance[i].meetsMinimumAttendance()) {
                return true;
            }
        }
        return false;
    }

    void displayAtRiskStudents() const {
        cout << "------------------------------------------------------" << endl;
        cout << "AT-RISK STUDENTS (Low GPA and/or Low Attendance)" << endl;
        cout << "------------------------------------------------------" << endl;
        int flagged = 0;
        for (int i = 0; i < studentCount; i++) {
            string sid = students[i].getId();
            if (isStudentAtRisk(sid)) {
                stringstream gpaBuf; gpaBuf << fixed << setprecision(2) << students[i].getGpa();
                cout << left << setw(10) << sid << setw(28) << students[i].getName()
                     << "GPA: " << gpaBuf.str() << endl;
                flagged++;
            }
        }
        if (flagged == 0) cout << "No students currently flagged as at-risk." << endl;
        else cout << "------------------------------------------------------" << endl
                   << "Total flagged: " << flagged << " out of " << studentCount << " students." << endl;
        cout << "------------------------------------------------------" << endl;
    }
    
    /* ============================ GRADE DISTRIBUTION CHART (my own feature) ============================ */
    // This is my own added feature. It count how many results got each grade
    // (A, B, C, D, F) and draw a simple bar chart using star symbols, so the
    // staff can see the whole class performance in one quick view.
    void displayGradeChart() const {
        // i use 5 counters for the 5 main grade groups
        int countA = 0, countB = 0, countC = 0, countD = 0, countF = 0;

        for (int i = 0; i < resultCount; i++) {
            if (!results[i].isActive()) continue;     // skip deleted records
            string g = results[i].getGrade();
            char first = g[0];                        // look at the first letter
            if (first == 'A') countA++;
            else if (first == 'B') countB++;
            else if (first == 'C') countC++;
            else if (first == 'D') countD++;
            else countF++;                            // E and F go here
        }

        cout << "------------------------------------------------------" << endl;
        cout << "GRADE DISTRIBUTION CHART (1 star = 1 result)" << endl;
        cout << "------------------------------------------------------" << endl;
        printOneBar("A", countA);
        printOneBar("B", countB);
        printOneBar("C", countC);
        printOneBar("D", countD);
        printOneBar("F", countF);
        cout << "------------------------------------------------------" << endl;
        cout << "Total results counted: "
             << (countA + countB + countC + countD + countF) << endl;
        cout << "------------------------------------------------------" << endl;
    }

    // small helper that print one row of the chart
    void printOneBar(const string &label, int count) const {
        cout << label << " | ";
        for (int i = 0; i < count; i++) cout << "*";   // draw the stars
        cout << " (" << count << ")" << endl;
    }

    /* ============================ SUMMARY REPORTS ============================ */

    /* Staff-side summary: overall system report (saved to + read from txt) */
    void generateStaffSummaryReport() {
        try {
            ofstream fout(SUMMARY_FILE);
            if (!fout.is_open()) throw FileException("Cannot write summary report file.");

            float highestMarks = -1, lowestMarks = 101, sumMarks = 0;
            string topStudent = "N/A";
            int activeCount = 0;

            for (int i = 0; i < resultCount; i++) {
                if (!results[i].isActive()) continue;
                activeCount++;
                sumMarks += results[i].getMarks();
                if (results[i].getMarks() > highestMarks) {
                    highestMarks = results[i].getMarks();
                    topStudent = results[i].getStudentId();
                }
                if (results[i].getMarks() < lowestMarks) lowestMarks = results[i].getMarks();
            }
            float avgMarks = (activeCount > 0) ? (sumMarks / activeCount) : 0;

            fout << "========================================================" << endl;
            fout << "      STUDENT RESULT MANAGEMENT SYSTEM - STAFF REPORT" << endl;
            fout << "========================================================" << endl;
            fout << "Total Students Registered : " << studentCount << endl;
            fout << "Total Courses Offered     : " << courseCount << endl;
            fout << "Total Result Records      : " << activeCount << endl;
            fout << "Average Marks (all)       : " << avgMarks << endl;
            fout << "Highest Marks Recorded    : " << highestMarks << " (Student " << topStudent << ")" << endl;
            fout << "Lowest Marks Recorded     : " << (activeCount > 0 ? lowestMarks : 0) << endl;
            fout << "Pending Remark Requests   : " << requestList.size() << endl;

            int atRiskCount = 0;
            for (int i = 0; i < studentCount; i++) {
                if (isStudentAtRisk(students[i].getId())) atRiskCount++;
            }
            fout << "Students Flagged At-Risk  : " << atRiskCount << endl;

            fout << "--------------------------------------------------------" << endl;
            fout << "Top 5 Students by GPA:" << endl;

            Student topList[MAX_STUDENTS];
            for (int i = 0; i < studentCount; i++) topList[i] = students[i];
            selectionSortByGpa(topList, studentCount);
            int limit = (studentCount < 5) ? studentCount : 5;
            for (int i = 0; i < limit; i++) {
                fout << "  " << (i + 1) << ". " << topList[i].getName()
                     << " (" << topList[i].getId() << ") - GPA: " << topList[i].getGpa() << endl;
            }

            fout << "--------------------------------------------------------" << endl;
            fout << "Bottom 5 Students by GPA (require academic support):" << endl;
            int bottomStart = (studentCount > 5) ? (studentCount - 5) : 0;
            for (int i = studentCount - 1; i >= bottomStart; i--) {
                fout << "  " << topList[i].getName() << " (" << topList[i].getId()
                     << ") - GPA: " << topList[i].getGpa() << endl;
            }

            fout << "========================================================" << endl;
            fout.close();
            cout << "Summary report saved to " << SUMMARY_FILE << endl;
        } catch (FileException &e) {
            cout << "[Error] " << e.what() << endl;
        }
    }

    void displaySummaryReportFromFile() {
        try {
            ifstream fin(SUMMARY_FILE);
            if (!fin.is_open()) throw FileException("No summary report found. Please generate one first.");
            string line;
            cout << endl;
            while (getline(fin, line)) {
                cout << line << endl;
            }
            fin.close();
        } catch (FileException &e) {
            cout << "[Error] " << e.what() << endl;
        }
    }

    /* Student-side summary: personal transcript summary (saved + retrieved per student) */
    void generateStudentSummaryReport(const string &studentId) {
        try {
            int sIdx = studentIndex.lookup(studentId);
            if (sIdx == -1) throw NotFoundException("Student not found.");

            string filename = "summary_" + studentId + ".txt";
            ofstream fout(filename.c_str());
            if (!fout.is_open()) throw FileException("Cannot write personal summary file.");

            CourseResult myResults[MAX_RESULTS];
            int n = getResultsForStudent(studentId, myResults, MAX_RESULTS);

            fout << "========================================================" << endl;
            fout << "        PERSONAL ACADEMIC SUMMARY - " << studentId << endl;
            fout << "========================================================" << endl;
            fout << "Name        : " << students[sIdx].getName() << endl;
            fout << "Programme   : " << students[sIdx].getProgramme() << endl;
            fout << "Current GPA : " << students[sIdx].getGpa() << endl;
            fout << "Credits     : " << students[sIdx].getTotalCredits() << endl;
            fout << "--------------------------------------------------------" << endl;
            fout << left << setw(13) << "Course Code" << setw(32) << "Course Name" << setw(7) << "Marks" << "Grade" << endl;
            for (int i = 0; i < n; i++) {
                fout << left << setw(13) << myResults[i].getCourseCode() << setw(32) << myResults[i].getCourseName()
                     << setw(7) << myResults[i].getMarks() << myResults[i].getGrade() << endl;
            }
            fout << "========================================================" << endl;
            fout.close();
            cout << "Personal summary saved to " << filename << endl;
        } catch (NotFoundException &e) {
            cout << "[Error] " << e.what() << endl;
        } catch (FileException &e) {
            cout << "[Error] " << e.what() << endl;
        }
    }

    void displayStudentSummaryFromFile(const string &studentId) {
        try {
            string filename = "summary_" + studentId + ".txt";
            ifstream fin(filename.c_str());
            if (!fin.is_open()) throw FileException("No saved summary found. Please generate one first.");
            string line;
            cout << endl;
            while (getline(fin, line)) {
                cout << line << endl;
            }
            fin.close();
        } catch (FileException &e) {
            cout << "[Error] " << e.what() << endl;
        }
    }

    /* ============================ SEEDING (for 50+ record requirement) ============================ */
    void seedDataIfEmpty();
};
/* ============================================================================
   PART 7: SEED DATA
   Provides 50+ baseline student records (with results) so the system has
   meaningful data to demonstrate sorting/searching/reporting on first run.
   This satisfies the "minimum 50 records" requirement. Data is only seeded
   if the student file is empty (first run), after which everything persists
   in the .txt files and survives across program restarts.
   ============================================================================ */

void SystemManager::seedDataIfEmpty() {
    if (studentCount > 0) return;   // already has data, do not reseed

    cout << "[System] No existing data found. Seeding initial demo dataset..." << endl;

    /* ---------- Seed Courses (10 courses) ---------- */
    // i change to my own course code
    const char* cCodes[10]   = {"CSC101","CSC102","CSC201","CSC202","CSC301",
                                 "CSC302","CSC401","CSC402","CSC501","CSC502"};
    const char* cNames[10]   = {"Data Structure and Algorithms","Database Systems",
                                 "Data Mining","Mobile App Development","Operating Systems",
                                 "Software Engineering","Web Programming","Artificial Intelligence",
                                 "Computer Networks","Cloud Computing"};
    int cCredits[10] = {3,3,3,3,3,4,3,3,3,3};
    for (int i = 0; i < 10; i++) {
        addCourse(cCodes[i], cNames[i], cCredits[i]);
    }

    // my own staff data
    registerStaff("AD001", "Dr. Tan Wei Ming",   "admin123", "Computer Science", "Senior Lecturer");
    registerStaff("AD002", "Mr. Lim Chee Keong", "admin123", "Software Engineering", "Lecturer");
    registerStaff("AD003", "Ms. Nurul Ain",      "admin123", "Data Science", "Coordinator");

    /* ---------- Seed Students (55 students = exceeds 50 minimum) ---------- */
    const char* firstNames[] = {"Aiman","Siti","Wei","Mei","Kumar","Priya","Hafiz","Nur","Chong","Farah",
        "Daniel","Aisyah","Wong","Lim","Tan","Rashid","Aminah","Hong","Sze","Iqbal",
        "Lina","Faiz","Mariam","Goh","Yusof","Hana","Vincent","Kavi","Suresh","Zainab",
        "Adam","Diana","Eric","Fiona","Grace","Hassan","Irfan","Joyce","Kelly","Leon",
        "Maya","Nabil","Olivia","Putri","Qistina","Ravi","Salma","Tariq","Umar","Vera",
        "Wati","Xin","Yasmin","Zack","Arif"};
    const char* lastNames[] = {"Lee","bin Ahmad","Tan","binti Ismail","Wong","a/l Raman","Ibrahim","Chua",
        "Yap","binti Hassan","Goh","a/p Suresh","Hashim","Loh","Mokhtar","Chin","Yusuf",
        "Tee","Aziz","Ooi"};
    const char* programmes[] = {"Bachelor of Computer Science","Bachelor of Information Technology",
        "Bachelor of Software Engineering","Bachelor of Data Science"};

//i change teh student start with 00001 eg.00001,00002
    int totalStudents = 55;
    for (int i = 0; i < totalStudents; i++) {
        stringstream idss;
        idss << "S";
        if (i < 1000) idss << "0";   // pad so 1-digit/2-digit/3-digit numbers
        if (i < 100)  idss << "0";   // all become 4 digits like 0000, 0001, 0042
        if (i < 10)   idss << "0";
        idss << i;                    // gives S0000, S0001, ... S0054
        string sid = idss.str();      // save the finished ID into sid

        string fullName = string(firstNames[i % 55]) + " " + string(lastNames[i % 20]);
        string prog = programmes[i % 4];

        stringstream phss;
        phss << "01" << (1000000 + i * 37 % 9000000);
        string phone = phss.str();

        stringstream emss;
        string emName = firstNames[i % 55];
        for (size_t k = 0; k < emName.size(); k++) emName[k] = tolower(emName[k]);
        emss << emName << i << "@student.edu.my";
        string email = emss.str();

        registerStudent(sid, fullName, "pass1234", prog, phone, email);
    }

    /* ---------- Seed Results: each student gets 3-5 course results ---------- */
    unsigned int seed = 12345;
    for (int i = 0; i < totalStudents; i++) {
        stringstream idss;
        idss << "S";
        if (i < 1000) idss << "0";   // same padding as the student loop above
        if (i < 100)  idss << "0";
        if (i < 10)   idss << "0";
        idss << i;
        string sid = idss.str();

        int numResults = 3 + (i % 3);   // 3,4 or 5 results per student
        for (int j = 0; j < numResults; j++) {
            int courseIdx = (i + j) % 10;
            /* simple deterministic pseudo-random marks generator (no rand() dependency on libs beyond cstdlib,
               but implemented manually here for reproducibility without STL) */
            seed = seed * 1103515245 + 12345;
            int marksRaw = (int)((seed / 65536) % 61) + 40;   // range 40-100
            float marks = (float)marksRaw;
            addResult(sid, cCodes[courseIdx], marks);
        }
    }

    /* ---------- Seed a few remark requests into the linked queue ---------- */
    requestList.enqueue("S0000", "TDS4223", "Requesting recheck for final exam marks.", Date(10, 6, 2026));
    requestList.enqueue("S0005", "TDS3251", "Believe marks were entered incorrectly.", Date(12, 6, 2026));
    requestList.enqueue("S0012", "TDS3301", "Assignment marks not reflected.", Date(15, 6, 2026));

    /* ---------- Seed Attendance: each student gets attendance for their enrolled courses ---------- */
    unsigned int attSeed = 99991;
    for (int i = 0; i < resultCount; i++) {
        string sid = results[i].getStudentId();
        string code = results[i].getCourseCode();
        attSeed = attSeed * 1103515245 + 12345;
        int held = 12 + (int)((attSeed / 65536) % 4);        // 12-15 classes held
        attSeed = attSeed * 1103515245 + 12345;
        int missed = (int)((attSeed / 65536) % 5);            // 0-4 classes missed
        int attended = held - missed;
        if (attended < 0) attended = 0;
        recordAttendance(sid, code, held, attended);
    }

    cout << "[System] Seed complete: " << studentCount << " students, "
         << resultCount << " results, " << courseCount << " courses, "
         << attendanceCount << " attendance entries." << endl;
}
/* ============================================================================
   PART 8: STAFF / ADMIN MODULE
   Handles all menu-driven interaction for the Staff side of the system.
   ============================================================================ */

class StaffModule {
private:
    SystemManager &mgr;
    int loggedInStaffIndex;

    string readLine(const string &prompt) {
        string val;
        cout << prompt;
        getline(cin, val);
        return val;
    }

    float readFloat(const string &prompt) {
        float val;
        cout << prompt;
        while (!(cin >> val)) {
            cout << "Invalid input. Please enter a number: ";
            cin.clear();
            cin.ignore(1000, '\n');
        }
        cin.ignore(1000, '\n');
        return val;
    }

    int readInt(const string &prompt) {
        int val;
        cout << prompt;
        while (!(cin >> val)) {
            cout << "Invalid input. Please enter a whole number: ";
            cin.clear();
            cin.ignore(1000, '\n');
        }
        cin.ignore(1000, '\n');
        return val;
    }

public:
    StaffModule(SystemManager &m) : mgr(m) {
        loggedInStaffIndex = -1;
    }

    ~StaffModule() {
        // staff module session ended
    }

    bool login() {
        cout << "\n=========== STAFF LOGIN ===========\n";
        string id = readLine("Staff ID: ");
        string pass = readLine("Password: ");
        int result = mgr.loginStaff(id, pass);
        try {
            if (result == -1) throw NotFoundException("Staff ID not found.");
            if (result == -2) throw ValidationException("Incorrect password.");
            loggedInStaffIndex = result;
            cout << "\nLogin successful. Welcome, " << mgr.getStaffArray()[result].getName() << "!\n";
            return true;
        } catch (NotFoundException &e) {
            cout << "[Login Failed] " << e.what() << endl;
            return false;
        } catch (ValidationException &e) {
            cout << "[Login Failed] " << e.what() << endl;
            return false;
        }
    }

    void registerNewStaff() {
        cout << "\n=========== STAFF REGISTRATION ===========\n";
        string id = readLine("New Staff ID: ");
        string name = readLine("Full Name: ");
        string pass = readLine("Password (min 4 chars): ");
        string dept = readLine("Department: ");
        string pos = readLine("Position: ");
        if (mgr.registerStaff(id, name, pass, dept, pos)) {
            cout << "Staff registered successfully!\n";
        }
    }

    void logout() {
        cout << "\nStaff logged out: " << mgr.getStaffArray()[loggedInStaffIndex].getName() << "\n";
        loggedInStaffIndex = -1;
    }

    /* -------------------- Add Record (Course + Result) -------------------- */
    void addCourseMenu() {
        cout << "\n--- Add New Course ---\n";
        string code = readLine("Course Code: ");
        string name = readLine("Course Name: ");
        int credits = readInt("Credit Hours: ");
        if (mgr.addCourse(code, name, credits)) {
            cout << "Course added successfully!\n";
        }
    }

    void addResultMenu() {
        cout << "\n--- Add New Result Record ---\n";
        string sid = readLine("Student ID: ");
        string code = readLine("Course Code: ");
        float marks = readFloat("Marks (0-100): ");
        if (mgr.addResult(sid, code, marks)) {
            cout << "Result added successfully! Grade auto-computed.\n";
        }
    }

    /* -------------------- Edit Record -------------------- */
    void editCourseMenu() {
        cout << "\n--- Edit Course ---\n";
        string code = readLine("Enter Course Code to edit: ");
        string newName = readLine("New Course Name: ");
        int newCredits = readInt("New Credit Hours: ");
        if (mgr.editCourse(code, newName, newCredits)) {
            cout << "Course updated successfully!\n";
        }
    }

    void editResultMenu() {
        cout << "\n--- Edit Result ---\n";
        int rid = readInt("Enter Result Record ID to edit: ");
        float newMarks = readFloat("New Marks (0-100): ");
        if (mgr.editResult(rid, newMarks)) {
            cout << "Result updated successfully! GPA recalculated.\n";
        }
    }

    /* -------------------- Display Record -------------------- */
    void displayAllStudents() {
        cout << "\n--- All Students ---\n";
        Student* arr = mgr.getStudentsArray();
        for (int i = 0; i < mgr.getStudentCount(); i++) {
            arr[i].displayProfile(true);
        }
    }

    void displayAllResults() {
        cout << "\n--- All Result Records ---\n";
        CourseResult* arr = mgr.getResultsArray();
        cout << left << setw(5) << "ID" << setw(10) << "StudentID" << setw(8) << "Course" << setw(32) << "CourseName" << setw(7) << "Marks" << setw(7) << "Grade" << "Credits" << "\n";
        for (int i = 0; i < mgr.getResultCount(); i++) {
            arr[i].display();
        }
    }

    /* -------------------- Search Record (2 criteria) -------------------- */
    void searchStudentByIdMenu() {
        cout << "\n--- Search Student by ID (Binary Search) ---\n";
        string id = readLine("Enter Student ID: ");

        int n = mgr.getStudentCount();
        Student* copyArr = new Student[n];
        for (int i = 0; i < n; i++) copyArr[i] = mgr.getStudentsArray()[i];
        insertionSortById(copyArr, n);

        int idx = binarySearchStudentById(copyArr, n, id);
        if (idx == -1) {
            cout << "Student not found.\n";
        } else {
            copyArr[idx].displayProfile();
        }
        delete[] copyArr;   // dynamic memory cleanup
    }

    void searchStudentByNameMenu() {
        cout << "\n--- Search Student by Name (Linear Search) ---\n";
        string keyword = readLine("Enter name keyword: ");
        Student* arr = mgr.getStudentsArray();
        int n = mgr.getStudentCount();
        int* matches = new int[n];
        int count = linearSearchStudentByName(arr, n, keyword, matches, n);
        if (count == 0) {
            cout << "No matching students found.\n";
        } else {
            cout << "Found " << count << " match(es):\n";
            for (int i = 0; i < count; i++) {
                arr[matches[i]].displayProfile(true);
            }
        }
        delete[] matches;
    }

    /* -------------------- Sort Records (2 criteria) -------------------- */
    void sortStudentsByNameMenu() {
        cout << "\n--- Sort Students by Name (Bubble Sort) ---\n";
        int n = mgr.getStudentCount();
        Student* copyArr = new Student[n];
        for (int i = 0; i < n; i++) copyArr[i] = mgr.getStudentsArray()[i];
        bubbleSortByCourseCode(copyArr, n);   // overloaded bubble sort for Student[]
        for (int i = 0; i < n; i++) copyArr[i].displayProfile(true);
        delete[] copyArr;
    }

    void sortStudentsByGpaMenu() {
        cout << "\n--- Sort Students by GPA (Insertion Sort, Descending) ---\n";
        int n = mgr.getStudentCount();
        Student* copyArr = new Student[n];
        for (int i = 0; i < n; i++) copyArr[i] = mgr.getStudentsArray()[i];
        selectionSortByGpa(copyArr, n);
        for (int i = 0; i < n; i++) copyArr[i].displayProfile(true);
        delete[] copyArr;
    }

    void sortResultsByMarksMenu() {
        cout << "\n--- Sort Results by Marks (Merge Sort, Descending) ---\n";
        int n = mgr.getResultCount();
        CourseResult* copyArr = new CourseResult[n];
        for (int i = 0; i < n; i++) copyArr[i] = mgr.getResultsArray()[i];
        if (n > 0) quickSortByMarks(copyArr, 0, n - 1);
        for (int i = 0; i < n; i++) copyArr[i].display();
        delete[] copyArr;
    }

    /* -------------------- Delete Record -------------------- */
    void deleteCourseMenu() {
        cout << "\n--- Delete Course ---\n";
        string code = readLine("Enter Course Code to delete: ");
        if (mgr.deleteCourse(code)) {
            cout << "Course deleted successfully!\n";
        }
    }

    void deleteResultMenu() {
        cout << "\n--- Delete Result ---\n";
        int rid = readInt("Enter Result Record ID to delete: ");
        if (mgr.deleteResult(rid)) {
            cout << "Result deleted successfully! GPA recalculated.\n";
        }
    }

    /* -------------------- Remark Queue (DNP structure demo) -------------------- */
    void viewRequestList() {
        cout << "\n--- Pending Remark Requests ---\n";
        mgr.getRequestList().displayAll();
    }

    void processNextRemarkRequest() {
        RemarkNode out("", "", "", Date());
        if (mgr.getRequestList().dequeue(out)) {
            cout << "\nProcessed remark request for Student " << out.studentId
                 << " (Course " << out.courseCode << "). Removed from queue.\n";
        } else {
            cout << "\nNo remark requests to process.\n";
        }
    }

    /* -------------------- Reports -------------------- */
    void generateReportMenu() {
        mgr.generateStaffSummaryReport();
    }

    void viewReportMenu() {
        mgr.displaySummaryReportFromFile();
    }

    /* -------------------- Attendance (additional feature) -------------------- */
    void recordAttendanceMenu() {
        cout << "\n--- Record / Update Attendance ---\n";
        string sid = readLine("Student ID: ");
        string code = readLine("Course Code: ");
        int held = readInt("Total Classes Held: ");
        int attended = readInt("Classes Attended: ");
        if (mgr.recordAttendance(sid, code, held, attended)) {
            cout << "Attendance recorded successfully!\n";
        }
    }

    void viewAllAttendanceSortedMenu() {
        cout << "\n--- All Attendance Records (Sorted by % Descending) ---\n";
        int n = mgr.getAttendanceCount();
        if (n == 0) {
            cout << "No attendance records yet.\n";
            return;
        }
        AttendanceEntry* copyArr = new AttendanceEntry[n];
        for (int i = 0; i < n; i++) copyArr[i] = mgr.getAttendanceArray()[i];
        bubbleSortByAttendance(copyArr, n);
        cout << left << setw(10) << "StudentID" << setw(8) << "Course" << setw(10) << "Attended" << setw(10) << "%" << "Status" << "\n";
        for (int i = 0; i < n; i++) copyArr[i].display();
        delete[] copyArr;
    }

    void searchAttendanceByStudentMenu() {
        cout << "\n--- Search Attendance by Student ID (Linear Search) ---\n";
        string sid = readLine("Enter Student ID: ");
        int n = mgr.getAttendanceCount();
        if (n == 0) {
            cout << "No attendance records yet.\n";
            return;
        }
        int* matches = new int[n];
        int count = linearSearchAttendanceByStudent(mgr.getAttendanceArray(), n, sid, matches, n);
        if (count == 0) {
            cout << "No attendance records found for this student.\n";
        } else {
            cout << left << setw(10) << "StudentID" << setw(8) << "Course" << setw(10) << "Attended" << setw(10) << "%" << "Status" << "\n";
            for (int i = 0; i < count; i++) mgr.getAttendanceArray()[matches[i]].display();
        }
        delete[] matches;
    }

    /* -------------------- Activity Log (Linked Stack demo) -------------------- */
    void viewActivityLogMenu() {
        cout << "\n--- Recent Activity Log ---\n";
        mgr.getActivityLog().displayRecent();
    }

    void searchActivityLogMenu() {
        cout << "\n--- Search Activity Log ---\n";
        string keyword = readLine("Enter keyword: ");
        if (logContains(mgr.getActivityLog(), keyword)) {
            cout << "Match found in activity log for keyword \"" << keyword << "\".\n";
        } else {
            cout << "No match found.\n";
        }
    }

    /* -------------------- Course Analytics (additional feature) -------------------- */
    void courseAnalyticsMenu() {
        cout << "\n--- Course Analytics ---\n";
        string code = readLine("Enter Course Code: ");
        mgr.displayCourseAnalytics(code);
    }

    void studentRankMenu() {
        cout << "\n--- Student Rank in a Course ---\n";
        string sid = readLine("Enter Student ID: ");
        string code = readLine("Enter Course Code: ");
        int rank = mgr.getStudentRankInCourse(sid, code);
        if (rank == -1) {
            cout << "No matching record found for that student/course combination.\n";
        } else {
            int enrolled = mgr.getCourseEnrollmentCount(code);
            cout << "Student " << sid << " is ranked #" << rank << " out of " << enrolled
                 << " in course " << code << ".\n";
        }
    }

    void atRiskStudentsMenu() {
        cout << "\n--- At-Risk Students Report ---\n";
        mgr.displayAtRiskStudents();
    }

    /* -------------------- Main Staff Menu Loop -------------------- */
    void run() {
        if (!login()) return;
        int choice = -1;
        while (choice != 0) {
            cout << "\n================ STAFF / ADMIN MENU ================\n";
            cout << " 1. Register New Staff\n";
            cout << " 2. Add Course\n";
            cout << " 3. Key In Student Marks\n";
            cout << " 4. Edit Course\n";
            cout << " 5. Edit Result Record\n";
            cout << " 6. Show All Student List\n";
            cout << " 7. Show All Marks Record\n";
            cout << " 8. Display All Courses\n";
            cout << " 9. Search Student by ID (Binary Search)\n";
            cout << "10. Search Student by Name (Linear Search)\n";
            cout << "11. Sort Students by Name\n";
            cout << "12. Sort Students by GPA\n";
            cout << "13. Sort Results by Marks\n";
            cout << "14. Delete Course\n";
            cout << "15. Delete Result Record\n";
            cout << "16. View Remark Request Queue\n";
            cout << "17. Process Next Remark Request\n";
            cout << "18. Generate Summary Report (save to txt)\n";
            cout << "19. View Summary Report (load from txt)\n";
            cout << "20. Record / Update Attendance\n";
            cout << "21. View All Attendance (Sorted)\n";
            cout << "22. Search Attendance by Student ID\n";
            cout << "23. View Recent Activity Log\n";
            cout << "24. Search Activity Log\n";
            cout << "25. Course Analytics (avg, top, bottom)\n";
            cout << "26. Student Rank in a Course\n";
            cout << "27. At-Risk Students Report\n";
            cout << "28. Grade Distribution Chart\n";
            cout << " 0. Logout\n";
            cout << "======================================================\n";
            choice = readInt("Enter choice: ");

            switch (choice) {
                case 1: registerNewStaff(); break;
                case 2: addCourseMenu(); break;
                case 3: addResultMenu(); break;
                case 4: editCourseMenu(); break;
                case 5: editResultMenu(); break; 
                case 6: displayAllStudents(); break;
                case 7: displayAllResults(); break;
                case 8: mgr.displayAllCourses(); break;
                case 9: searchStudentByIdMenu(); break;
                case 10: searchStudentByNameMenu(); break;
                case 11: sortStudentsByNameMenu(); break;
                case 12: sortStudentsByGpaMenu(); break;
                case 13: sortResultsByMarksMenu(); break;
                case 14: deleteCourseMenu(); break;
                case 15: deleteResultMenu(); break;
                case 16: viewRequestList(); break;
                case 17: processNextRemarkRequest(); break;
                case 18: generateReportMenu(); break;
                case 19: viewReportMenu(); break;
                case 20: recordAttendanceMenu(); break;
                case 21: viewAllAttendanceSortedMenu(); break;
                case 22: searchAttendanceByStudentMenu(); break;
                case 23: viewActivityLogMenu(); break;
                case 24: searchActivityLogMenu(); break;
                case 25: courseAnalyticsMenu(); break;
                case 26: studentRankMenu(); break;
                case 27: atRiskStudentsMenu(); break;
                case 28: mgr.displayGradeChart(); break;
                case 0: logout(); break;
                default: cout << "Invalid choice. Try again.\n"; break;
            }
        }
    }
};
/* ============================================================================
   PART 9: CUSTOMER (STUDENT) MODULE
   Handles all menu-driven interaction for the Student side of the system.
   "Booking" concept here = a Remark Request (the student's own action that
   the Staff module can view/process), keeping both modules in sync via the
   shared SystemManager and its txt files.
   ============================================================================ */

class CustomerModule {
private:
    SystemManager &mgr;
    int loggedInStudentIndex;

    string readLine(const string &prompt) {
        string val;
        cout << prompt;
        getline(cin, val);
        return val;
    }

    float readFloat(const string &prompt) {
        float val;
        cout << prompt;
        while (!(cin >> val)) {
            cout << "Invalid input. Please enter a number: ";
            cin.clear();
            cin.ignore(1000, '\n');
        }
        cin.ignore(1000, '\n');
        return val;
    }

    int readInt(const string &prompt) {
        int val;
        cout << prompt;
        while (!(cin >> val)) {
            cout << "Invalid input. Please enter a whole number: ";
            cin.clear();
            cin.ignore(1000, '\n');
        }
        cin.ignore(1000, '\n');
        return val;
    }

public:
    CustomerModule(SystemManager &m) : mgr(m) {
        loggedInStudentIndex = -1;
    }

    ~CustomerModule() {
        // customer module session ended
    }

    bool login() {
        cout << "\n=========== STUDENT LOGIN ===========\n";
        string id = readLine("Student ID: ");
        string pass = readLine("Password: ");
        int result = mgr.loginStudent(id, pass);
        try {
            if (result == -1) throw NotFoundException("Student ID not found.");
            if (result == -2) throw ValidationException("Incorrect password.");
            loggedInStudentIndex = result;
            cout << "\nLogin successful. Welcome, " << mgr.getStudentsArray()[result].getName() << "!\n";
            return true;
        } catch (NotFoundException &e) {
            cout << "[Login Failed] " << e.what() << endl;
            return false;
        } catch (ValidationException &e) {
            cout << "[Login Failed] " << e.what() << endl;
            return false;
        }
    }
// i removed the "type your own ID" part. the system will give the student an ID automatically after the registration is done.
    void registerNewStudent() {
        cout << "\n=========== STUDENT REGISTRATION ===========\n";
        string id = mgr.generateStudentId();
        string name = readLine("Full Name: ");
        string pass = readLine("Password (min 4 chars): ");
        string prog = readLine("Programme: ");
        string phone = readLine("Phone Number: ");
        string email = readLine("Email: ");
        if (mgr.registerStudent(id, name, pass, prog, phone, email)) {
            cout << "Registration successful!\n";
            cout << "Your student ID is: " << id << "\n";
            cout << "Please remember this ID to log in.\n";
        }
    }

    void logout() {
        cout << "\nStudent logged out: " << mgr.getStudentsArray()[loggedInStudentIndex].getName() << "\n";
        loggedInStudentIndex = -1;
    }

    /* -------------------- Add Record: submit Remark Request (student "booking") -------------------- */

        void submitRemarkRequest() {
        cout << "\n--- Submit Remark Request ---\n";
        // i show the course list first so the student can see what code to type
       cout << "Here are the available courses:\n";
        mgr.displayAllCourses();
        string sid = mgr.getStudentsArray()[loggedInStudentIndex].getId();
        string code = readLine("Enter Course Code from the list above: ");
        string reason = readLine("Reason for remark: ");
        time_t t = time(0);
        tm* now = localtime(&t);
        Date today(now->tm_mday, now->tm_mon + 1, now->tm_year + 1900);
        mgr.getRequestList().enqueue(sid.c_str(), code.c_str(), reason.c_str(), today);
        cout << "Remark request submitted! It will appear in Staff's queue.\n";
    }

    /* -------------------- Add Record: update own contact info -------------------- */
    void updateContactInfo() {
        cout << "\n--- Update Contact Info ---\n";
        string phone = readLine("New Phone Number: ");
        string email = readLine("New Email: ");
        mgr.getStudentsArray()[loggedInStudentIndex].setContact(ContactInfo(phone.c_str(), email.c_str()));
        mgr.saveStudents();
        cout << "Contact info updated successfully!\n";
    }

    /* -------------------- Edit Record -------------------- */
    void editOwnPassword() {
        cout << "\n--- Change Password ---\n";
        string newPass = readLine("New Password (min 4 chars): ");
        if (newPass.length() < 4) {
            cout << "[Error] Password too short.\n";
            return;
        }
        mgr.getStudentsArray()[loggedInStudentIndex].setPassword(newPass.c_str());
        mgr.saveStudents();
        cout << "Password updated successfully!\n";
    }

    void editOwnProgramme() {
        cout << "\n--- Update Programme ---\n";
        string prog = readLine("New Programme: ");
        mgr.getStudentsArray()[loggedInStudentIndex].setProgramme(prog.c_str());
        mgr.saveStudents();
        cout << "Programme updated successfully!\n";
    }

    /* -------------------- Display Record -------------------- */
    void displayMyProfile() {
        cout << "\n--- My Profile ---\n";
        mgr.getStudentsArray()[loggedInStudentIndex].displayProfile();
    }

    void displayMyResults() {
        cout << "\n--- My Results ---\n";
        string sid = mgr.getStudentsArray()[loggedInStudentIndex].getId();
        CourseResult myResults[MAX_RESULTS];
        int n = mgr.getResultsForStudent(sid, myResults, MAX_RESULTS);
        if (n == 0) {
            cout << "No results recorded yet.\n";
            return;
        }
        cout << left << setw(13) << "Course Code" << setw(32) << "Course Name" << setw(7) << "Marks" << "Grade" << "\n";
        for (int i = 0; i < n; i++) {
            cout << left << setw(13) << myResults[i].getCourseCode() << setw(32) << myResults[i].getCourseName()
                 << setw(7) << myResults[i].getMarks() << myResults[i].getGrade() << endl;
        }
    }

    /* -------------------- Search Record (2 criteria) -------------------- */
    void searchMyResultByCourseMenu() {
        cout << "\n--- Search My Results by Course Code (Linear Search) ---\n";
        // i show my own results first so i know which course code to search
        cout << "Here are your current results:\n";
        displayMyResults();
        string code = readLine("Enter Course Code from your results above: ");
        string sid = mgr.getStudentsArray()[loggedInStudentIndex].getId();
        CourseResult myResults[MAX_RESULTS];
        int n = mgr.getResultsForStudent(sid, myResults, MAX_RESULTS);
        int* matches = new int[n > 0 ? n : 1];
        int count = linearSearchResultsByCourse(myResults, n, code, matches, n);
        if (count == 0) {
            cout << "No matching course found in your records.\n";
        } else {
            for (int i = 0; i < count; i++) myResults[matches[i]].display();
        }
        delete[] matches;
    }

    void searchAvailableCourseMenu() {
        cout << "\n--- Search Available Course by Code ---\n";
        string code = readLine("Enter Course Code: ");
        int idx = mgr.findCourseIndex(code);
        if (idx == -1) {
            cout << "Course not found.\n";
        } else {
            cout << "Code: " << mgr.getCourseCodeAt(idx) << " | Name: " << mgr.getCourseNameAt(idx)
                 << " | Credits: " << mgr.getCourseCreditAt(idx) << endl;
        }
    }

    /* -------------------- Sort Records (2 criteria) -------------------- */
    void sortMyResultsByMarksMenu() {
        cout << "\n--- Sort My Results by Marks (Merge Sort, Descending) ---\n";
        string sid = mgr.getStudentsArray()[loggedInStudentIndex].getId();
        CourseResult myResults[MAX_RESULTS];
        int n = mgr.getResultsForStudent(sid, myResults, MAX_RESULTS);
        if (n > 0) quickSortByMarks(myResults, 0, n - 1);
        for (int i = 0; i < n; i++) myResults[i].display();
    }

    void sortMyResultsByCourseCodeMenu() {
        cout << "\n--- Sort My Results by Course Code (Bubble Sort) ---\n";
        string sid = mgr.getStudentsArray()[loggedInStudentIndex].getId();
        CourseResult myResults[MAX_RESULTS];
        int n = mgr.getResultsForStudent(sid, myResults, MAX_RESULTS);
        bubbleSortByCourseCode(myResults, n);
        for (int i = 0; i < n; i++) myResults[i].display();
    }

    /* -------------------- Delete Record -------------------- */
    void cancelRemarkRequest() {
        cout << "\n--- Cancel My Oldest Remark Request ---\n";
        string sid = mgr.getStudentsArray()[loggedInStudentIndex].getId();
        int pending = countRequestsByStudent(mgr.getRequestList(), sid);
        if (pending == 0) {
            cout << "You have no pending remark requests.\n";
            return;
        }
        cout << "Note: requests are processed in FIFO order; this removes the queue's front item if it is yours,\n";
        cout << "otherwise please ask staff to process the queue first.\n";
        RemarkNode out("", "", "", Date());
        if (mgr.getRequestList().dequeue(out)) {
            cout << "Removed request for course " << out.courseCode << " (was Student " << out.studentId << ").\n";
        }
    }

    /* -------------------- Attendance (additional feature) -------------------- */
    void viewMyAttendance() {
        cout << "\n--- My Attendance ---\n";
        string sid = mgr.getStudentsArray()[loggedInStudentIndex].getId();
        AttendanceEntry myAttendance[MAX_RESULTS];
        int n = mgr.getAttendanceForStudent(sid, myAttendance, MAX_RESULTS);
        if (n == 0) {
            cout << "No attendance records found yet.\n";
            return;
        }
        cout << left << setw(10) << "StudentID" << setw(8) << "Course" << setw(10) << "Attended" << setw(10) << "%" << "Status" << "\n";
        for (int i = 0; i < n; i++) myAttendance[i].display();
    }

    void sortMyAttendanceMenu() {
        cout << "\n--- Sort My Attendance by Percentage (Bubble Sort, Descending) ---\n";
        string sid = mgr.getStudentsArray()[loggedInStudentIndex].getId();
        AttendanceEntry myAttendance[MAX_RESULTS];
        int n = mgr.getAttendanceForStudent(sid, myAttendance, MAX_RESULTS);
        if (n == 0) {
            cout << "No attendance records found yet.\n";
            return;
        }
        bubbleSortByAttendance(myAttendance, n);
        cout << left << setw(10) << "StudentID" << setw(8) << "Course" << setw(10) << "Attended" << setw(10) << "%" << "Status" << "\n";
        for (int i = 0; i < n; i++) myAttendance[i].display();
    }

    /* -------------------- Reports -------------------- */
    void generateReportMenu() {
        string sid = mgr.getStudentsArray()[loggedInStudentIndex].getId();
        mgr.generateStudentSummaryReport(sid);
    }

    void viewReportMenu() {
        string sid = mgr.getStudentsArray()[loggedInStudentIndex].getId();
        mgr.displayStudentSummaryFromFile(sid);
    }

    /* -------------------- Ranking / Analytics (additional feature) -------------------- */
    void viewMyRankInCourseMenu() {
        cout << "\n--- My Rank in a Course ---\n";
        string code = readLine("Enter Course Code: ");
        string sid = mgr.getStudentsArray()[loggedInStudentIndex].getId();
        int rank = mgr.getStudentRankInCourse(sid, code);
        if (rank == -1) {
            cout << "You do not have a result recorded for this course.\n";
        } else {
            int enrolled = mgr.getCourseEnrollmentCount(code);
            float avg = mgr.getCourseAverageMarks(code);
            cout << "Your rank: #" << rank << " out of " << enrolled << " students.\n";
            cout << "Course average marks: " << avg << endl;
        }
    }

    void viewMyGpaPercentileMenu() {
        cout << "\n--- My GPA Percentile ---\n";
        string sid = mgr.getStudentsArray()[loggedInStudentIndex].getId();
        float percentile = mgr.getStudentGpaPercentile(sid);
        cout << "Your GPA is higher than or equal to " << percentile
             << "% of all students in the system.\n";
    }

    /* -------------------- Main Customer Menu Loop -------------------- */
    void run() {
        if (!login()) return;
        int choice = -1;
        while (choice != 0) {
            cout << "\n================ STUDENT MENU ================\n";
            cout << " 1. Submit Remark Request\n";
            cout << " 2. Update Contact Info\n";
            cout << " 3. Change Password\n";
            cout << " 4. Update Programme\n";
            cout << " 5. See My Detail\n";
            cout << " 6. See My Result\n";
            cout << " 7. Search My Results by Course Code\n";
            cout << " 8. Search Available Course by Code\n";
            cout << " 9. Sort My Results by Marks\n";
            cout << "10. Sort My Results by Course Code\n";
            cout << "11. Cancel a Remark Request (FIFO)\n";
            cout << "12. Generate My Summary Report (save to txt)\n";
            cout << "13. View My Summary Report (load from txt)\n";
            cout << "14. View My Attendance\n";
            cout << "15. Sort My Attendance by Percentage\n";
            cout << "16. View My Rank in a Course\n";
            cout << "17. Check My GPA Standing\n";
            cout << " 0. Logout\n";
            cout << "================================================\n";
            choice = readInt("Enter choice: ");

            switch (choice) {
                case 1: submitRemarkRequest(); break;
                case 2: updateContactInfo(); break;
                case 3: editOwnPassword(); break;
                case 4: editOwnProgramme(); break;
                case 5: displayMyProfile(); break;
                case 6: displayMyResults(); break;
                case 7: searchMyResultByCourseMenu(); break;
                case 8: searchAvailableCourseMenu(); break;
                case 9: sortMyResultsByMarksMenu(); break;
                case 10: sortMyResultsByCourseCodeMenu(); break;
                case 11: cancelRemarkRequest(); break;
                case 12: generateReportMenu(); break;
                case 13: viewReportMenu(); break;
                case 14: viewMyAttendance(); break;
                case 15: sortMyAttendanceMenu(); break;
                case 16: viewMyRankInCourseMenu(); break;
                case 17: viewMyGpaPercentileMenu(); break;
                case 0: logout(); break;
                default: cout << "Invalid choice. Try again.\n"; break;
            }
        }
    }
};
/* ============================================================================
   PART 10: MAIN PROGRAM ENTRY POINT
   ============================================================================ */

void printWelcomeBanner() {
    cout << "\n";
    cout << "########################################################\n";
    cout << "#                                                      #\n";
    cout << "#      STUDENT RESULT MANAGEMENT SYSTEM (SRMS)         #\n";
    cout << "#      TDS4223 Data Structure and Algorithms           #\n";
    cout << "#                                                      #\n";
    cout << "########################################################\n";
}

int main() {
    SystemManager manager;

    try {
        manager.loadAll();
        manager.seedDataIfEmpty();
    } catch (...) {
        cout << "[Warning] Unexpected error during startup data load. Continuing with empty data.\n";
    }

    StaffModule staffModule(manager);
    CustomerModule customerModule(manager);

    int mainChoice = -1;
    while (mainChoice != 0) {
        printWelcomeBanner();
        cout << "\n1. Staff / Admin Module\n";
        cout << "2. Student (Customer) Module\n";
        cout << "3. New Student Registration\n";
        cout << "0. Exit System\n";
        cout << "Enter choice: ";

        while (!(cin >> mainChoice)) {
            cout << "Invalid input. Enter choice: ";
            cin.clear();
            cin.ignore(1000, '\n');
        }
        cin.ignore(1000, '\n');

        switch (mainChoice) {
            case 1:
                staffModule.run();
                break;
            case 2:
                customerModule.run();
                break;
            case 3:
                customerModule.registerNewStudent();
                break;
            case 0:
                cout << "\nThank you for using SRMS. Goodbye!\n";
                break;
            default:
                cout << "\nInvalid choice. Please try again.\n";
                break;
        }
    }

    return 0;
}

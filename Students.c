#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_STUDENTS 100
#define MAX_TEACHERS 50
#define MAX_CLASSES 50
#define MAX_LEN_NAME 30
#define MAX_LEN_ID 10
#define MAX_LEN_EMAIL 30
#define MAX_LEN_PHONE 20
#define MAX_LEN_PASS 20

#define ADMIN_EMAIL "admin@gmail.com"
#define ADMIN_PASS "123"

typedef struct { int day, month, year; } Date;
typedef struct {
    char studentId[MAX_LEN_ID], classroomId[MAX_LEN_ID];
    char name[MAX_LEN_NAME]; Date dob;
    int gender;
    char email[MAX_LEN_EMAIL], phone[MAX_LEN_PHONE], password[MAX_LEN_PASS];
} Student;
typedef struct {
    char teacherId[MAX_LEN_ID], name[MAX_LEN_NAME];
    Date dob;
    char email[MAX_LEN_EMAIL], phone[MAX_LEN_PHONE], password[MAX_LEN_PASS];
    char classIds[MAX_CLASSES][MAX_LEN_ID]; int numClasses;
} Teacher;
typedef struct {
    char classId[MAX_LEN_ID], teacherId[MAX_LEN_ID];
    char className[MAX_LEN_NAME];
    char studentIds[MAX_STUDENTS][MAX_LEN_ID]; int numStudents;
} ClassRoom;

Student students[MAX_STUDENTS];
Teacher teachers[MAX_TEACHERS];
ClassRoom classes[MAX_CLASSES];

int studentCount = 0;
int teacherCount = 0;
int classCount = 0;

int nextStudentId = 1;
int nextTeacherId = 1;
int nextClassId = 1;

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void pause_screen() {
    printf("\nPress ENTER to continue...");
    clear_input_buffer();
}

void trim_whitespace(char *s) {
    if (!s) return;
    char *start = s;
    while (*start && isspace((unsigned char)*start)) start++;
    if (start != s) memmove(s, start, strlen(start) + 1);
    int len = (int)strlen(s);
    while (len > 0 && isspace((unsigned char)s[len - 1])) s[--len] = '\0';
}

int ci_stricmp(const char *a, const char *b) {
    for (; *a || *b; a++, b++) {
        int ca = tolower((unsigned char)*a);
        int cb = tolower((unsigned char)*b);
        if (ca != cb) return ca - cb;
    }
    return 0;
}

void get_required_input(const char *prompt, char *out, int maxlen) {
    do {
        printf("%s", prompt);
        if (fgets(out, maxlen, stdin) == NULL) { out[0] = '\0'; break; }
        out[strcspn(out, "\n")] = '\0';
        trim_whitespace(out);
    } while (strlen(out) == 0);
}

int is_valid_date(int d, int m, int y) {
    if (y < 1900 || y > 2025 || m < 1 || m > 12 || d < 1 || d > 31) return 0;
    if (m == 2) {
        if ((y % 4 == 0 && y % 100 != 0) || (y % 400 == 0)) { return (d <= 29); }
        else { return (d <= 28); }
    }
    if (m == 4 || m == 6 || m == 9 || m == 11) { return (d <= 30); }
    return 1;
}

void input_date_of_birth(Date *d) {
    int day, month, year;
    do {
        printf("Enter Date of Birth (dd/mm/yyyy): ");
        if (scanf("%d/%d/%d", &day, &month, &year) != 3) {
            printf("Error: Format error. Please re-enter (dd/mm/yyyy): ");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();
        if (!is_valid_date(day, month, year)) {
            printf("Error: Invalid date. Please re-enter (dd/mm/yyyy).\n");
        } else {
            d->day = day; d->month = month; d->year = year;
            break;
        }
    } while(1);
}

int is_valid_email(const char *email) { return (strchr(email, '@') != NULL && strchr(email, '.') != NULL); }
int is_valid_phone(const char *phone) {
    int len = (int)strlen(phone);
    if (len < 10 || len > 11) return 0;
    for (int i = 0; i < len; i++) { if (!isdigit(phone[i])) return 0; }
    return 1;
}

int find_student_index_by_id(const char *id) {
    for (int i = 0; i < studentCount; ++i) { if (strcmp(students[i].studentId, id) == 0) return i; }
    return -1;
}
int find_teacher_index_by_id(const char *id) {
    for (int i = 0; i < teacherCount; ++i) { if (strcmp(teachers[i].teacherId, id) == 0) return i; }
    return -1;
}
int find_class_index_by_id(const char *id) {
    for (int i = 0; i < classCount; ++i) { if (strcmp(classes[i].classId, id) == 0) return i; }
    return -1;
}

void print_student_table_separator() {
    printf("|-------|----------------------|------------|--------------|---------------------------|--------------|-----------|\n");
}
void print_student_table_header() {
    printf("================================================================================================================================\n");
    printf("| %-5s | %-20s | %-10s | %-12s | %-25s | %-12s | %-9s |\n", "ID", "Name", "Class", "DOB", "Email", "Phone", "Gender");
    print_student_table_separator();
}
void print_student_row(Student s) {
    printf("| %-5s | %-20s | %-10s | %02d/%02d/%-6d | %-25s | %-12s | %-9s |\n",
        s.studentId, s.name, strlen(s.classroomId) > 0 ? s.classroomId : "Unassigned",
        s.dob.day, s.dob.month, s.dob.year, s.email, s.phone,
        s.gender == 1 ? "Male" : "Female"
    );
}
void display_students_in_list(Student *list, int count, const char *title) {
    printf("\n--- %s ---\n", title);
    print_student_table_header();
    if (count == 0) {
        printf("| %-113s |\n", "No students found.");
    } else {
        for (int i = 0; i < count; ++i) {
            print_student_row(list[i]);
        }
    }
    printf("================================================================================================================================\n");
}
void display_all_students() {
    display_students_in_list(students, studentCount, "LIST OF ALL STUDENTS");
}

void print_teacher_table_separator() {
    printf("|-------|---------------------------|--------------|---------------------------|--------------|-------|\n");
}
void print_teacher_table_header() {
    printf("================================================================================================================\n");
    printf("| %-5s | %-25s | %-12s | %-25s | %-12s | %-5s |\n", "ID", "Name", "DOB", "Email", "Phone", "Classes");
    print_teacher_table_separator();
}
void print_teacher_row(Teacher t) {
    printf("| %-5s | %-25s | %02d/%02d/%-6d | %-25s | %-12s | %-5d |\n",
        t.teacherId, t.name, t.dob.day, t.dob.month, t.dob.year, t.email, t.phone, t.numClasses
    );
}
void display_all_teachers() {
    printf("\n--- LIST OF ALL TEACHERS ---\n");
    print_teacher_table_header();
    if (teacherCount == 0) { printf("| %-108s |\n", "No teachers found."); }
    else { for (int i = 0; i < teacherCount; ++i) print_teacher_row(teachers[i]); }
    printf("================================================================================================================\n");
}

void print_class_table_separator() {
    printf("|-------|----------------------|------------|--------|\n");
}
void print_class_table_header() {
    printf("========================================================\n");
    printf("| %-5s | %-20s | %-10s | %-5s |\n", "ID", "Class Name", "Teacher ID", "Students");
    print_class_table_separator();
}
void print_class_row(ClassRoom c) {
    printf("| %-5s | %-20s | %-10s | %-5d |\n",
        c.classId, c.className, strlen(c.teacherId) > 0 ? c.teacherId : "Unassigned", c.numStudents
    );
}
void display_all_classes() {
    printf("\n--- LIST OF ALL CLASSES ---\n");
    print_class_table_header();
    if (classCount == 0) { printf("| %-56s |\n", "No classes found."); }
    else { for (int i = 0; i < classCount; ++i) print_class_row(classes[i]); }
    printf("========================================================\n");
}

int check_student_duplicate(const char *email, const char *phone, const char *currentId) {
    for (int i = 0; i < studentCount; ++i) {
        if (strcmp(students[i].studentId, currentId) != 0) {
            if (ci_stricmp(students[i].email, email) == 0 && strlen(email) > 0) { return 1; }
            if (strcmp(students[i].phone, phone) == 0 && strlen(phone) > 0) { return 2; }
        }
    } return 0;
}

void input_student_email_phone(Student *s, const char *currentId) {
    char temp_email[MAX_LEN_EMAIL], temp_phone[MAX_LEN_PHONE];
    do {
        get_required_input("Enter Email: ", temp_email, MAX_LEN_EMAIL);
        if (!is_valid_email(temp_email)) { printf("Error: Invalid email format! Please re-enter email.\n"); continue; }
        if (check_student_duplicate(temp_email, "", currentId) == 1) {
            printf("Error: Email **%s** already exists! Please re-enter email.\n", temp_email);
            continue;
        }
        break;
    } while(1);

    do {
        get_required_input("Enter Phone number (10-11 digits): ", temp_phone, MAX_LEN_PHONE);
        if (!is_valid_phone(temp_phone)) { printf("Error: Phone must be 10 or 11 digits and contain only numbers! Please re-enter phone.\n"); continue; }
        if (check_student_duplicate("", temp_phone, currentId) == 2) {
             printf("Error: Phone number **%s** already exists! Please re-enter phone.\n", temp_phone);
             continue;
        }
        break;
    } while (1);

    strcpy(s->email, temp_email);
    strcpy(s->phone, temp_phone);
}

void input_student_gender(Student *s) {
    char genderStr[5];
    do {
        printf("Enter Gender (1=Male, 0=Female): ");
        fgets(genderStr, sizeof(genderStr), stdin);
        trim_whitespace(genderStr);
        if (strcmp(genderStr, "1") == 0) { s->gender = 1; break; }
        else if (strcmp(genderStr, "0") == 0) { s->gender = 0; break; }
        else { printf("Error: Invalid gender. Please re-enter (1 or 0).\n"); }
    } while (1);
}

void input_student_info(Student *s, const char *currentId) {
    get_required_input("Enter Name: ", s->name, MAX_LEN_NAME);
    input_date_of_birth(&s->dob);
    input_student_email_phone(s, currentId);
    input_student_gender(s);
    strcpy(s->password, "111");
}

void add_student() {
    if (studentCount >= MAX_STUDENTS) { printf("Student list is full!\n"); return; }
    Student newStudent = {0};
    printf("\n--- ADD NEW STUDENT ---\n");

    input_student_info(&newStudent, "");

    sprintf(newStudent.studentId, "SV%d", nextStudentId++);
    students[studentCount++] = newStudent;

    printf("\nSuccessfully added student **%s**! Student ID: **%s**, Default Password: 111\n", students[studentCount-1].name, students[studentCount-1].studentId);
}

void get_student_id_to_edit(char *id, int *index) {
    do {
        get_required_input("Enter Student ID to edit: ", id, MAX_LEN_ID);
        *index = find_student_index_by_id(id);
        if (*index == -1) {
            printf("Error: Student ID **%s** does not exist! Please re-enter ID.\n", id);
        } else {
            break;
        }
    } while(1);
}

void edit_student() {
    char id[MAX_LEN_ID]; int index;
    printf("\n--- EDIT STUDENT INFORMATION ---\n");
    get_student_id_to_edit(id, &index);

    printf("\n--- Current information for student %s ---\n", students[index].name);
    print_student_table_header();
    print_student_row(students[index]);
    printf("================================================================================================================================\n");

    printf("\nEnter new information:\n");
    input_student_info(&students[index], students[index].studentId);
    printf("\nSuccessfully updated information for student **%s**!\n", students[index].studentId);
}

void remove_student_from_class_on_delete(const char *studentId, const char *classId) {
    int classIndex = find_class_index_by_id(classId);
    if (classIndex != -1) {
        for (int i = 0; i < classes[classIndex].numStudents; i++) {
            if (strcmp(classes[classIndex].studentIds[i], studentId) == 0) {
                for (int j = i; j < classes[classIndex].numStudents - 1; j++) {
                    strcpy(classes[classIndex].studentIds[j], classes[classIndex].studentIds[j+1]);
                }
                classes[classIndex].numStudents--;
                break;
            }
        }
    }
}

void delete_student() {
    char id[MAX_LEN_ID], confirm[3]; int index;
    printf("\n--- DELETE STUDENT ---\n");
    do {
        get_required_input("Enter Student ID to delete: ", id, MAX_LEN_ID);
        index = find_student_index_by_id(id);
        if (index == -1) {
            printf("Error: Student ID **%s** does not exist! Please re-enter ID.\n", id);
        } else {
            break;
        }
    } while(1);

    printf("Are you sure you want to delete student **%s** (%s)? (Y/N): ", students[index].name, id);
    fgets(confirm, sizeof(confirm), stdin); trim_whitespace(confirm);

    if (ci_stricmp(confirm, "Y") == 0) {
        if (strlen(students[index].classroomId) > 0) {
            remove_student_from_class_on_delete(id, students[index].classroomId);
        }

        for (int i = index; i < studentCount - 1; i++) { students[i] = students[i+1]; }
        studentCount--;
        printf("Successfully deleted student **%s**!\n", id);
    } else {
        printf("Deletion operation cancelled.\n");
    }
}

void search_student_by_name() {
    char key[MAX_LEN_NAME];
    get_required_input("Enter student name (or part of the name) to search: ", key, MAX_LEN_NAME);
    Student results[MAX_STUDENTS]; int resCount = 0;

    for (int i = 0; i < studentCount; ++i) {
        if (strstr(students[i].name, key) != NULL || ci_stricmp(students[i].name, key) == 0) {
            results[resCount++] = students[i];
        }
    }
    display_students_in_list(results, resCount, "SEARCH RESULTS BY NAME");
}
void search_student_by_gender() {
    char genderStr[5]; int gender;
    do {
        printf("Enter Gender to search (1=Male, 0=Female): "); fgets(genderStr, sizeof(genderStr), stdin); trim_whitespace(genderStr);
        if (strcmp(genderStr, "1") == 0) { gender = 1; break; }
        else if (strcmp(genderStr, "0") == 0) { gender = 0; break; }
        else { printf("Error: Invalid gender. Please re-enter (1 or 0).\n"); }
    } while (1);

    Student results[MAX_STUDENTS]; int resCount = 0;
    for (int i = 0; i < studentCount; ++i) {
        if (students[i].gender == gender) { results[resCount++] = students[i]; }
    }
    display_students_in_list(results, resCount, "SEARCH RESULTS BY GENDER");
}

void student_search_menu() {
    int ch;
    do {
        system("cls");
        printf("\n--- STUDENT SEARCH ---\n");
        printf("1. Search by Name (case-insensitive)\n");
        printf("2. Search by Gender\n");
        printf("0. Back\n");
        printf("Choose: ");
        if (scanf("%d", &ch) != 1) { clear_input_buffer(); ch = -1; continue; }
        clear_input_buffer();
        switch (ch) {
            case 1: search_student_by_name(); pause_screen(); break;
            case 2: search_student_by_gender(); pause_screen(); break;
            case 0: return;
            default: break;
        }
    } while (ch != 0);
}

int cmp_s_name_asc(const void *a, const void *b) { return ci_stricmp(((const Student *)a)->name, ((const Student *)b)->name); }
int cmp_s_name_desc(const void *a, const void *b) { return ci_stricmp(((const Student *)b)->name, ((const Student *)a)->name); }
void student_sort_menu() {
    int ch;
    do {
        system("cls");
        printf("\n--- SORT STUDENT LIST ---\n");
        printf("1. Sort by Name (A->Z)\n");
        printf("2. Sort by Name (Z->A)\n");
        printf("0. Back\n");
        printf("Choose: ");
        if (scanf("%d", &ch) != 1) { clear_input_buffer(); ch = -1; continue; }
        clear_input_buffer();
        switch (ch) {
            case 1:
                qsort(students, studentCount, sizeof(Student), cmp_s_name_asc);
                printf("Successfully sorted by Name (A->Z)!\n");
                display_all_students(); pause_screen(); break;
            case 2:
                qsort(students, studentCount, sizeof(Student), cmp_s_name_desc);
                printf("Successfully sorted by Name (Z->A)!\n");
                display_all_students(); pause_screen(); break;
            case 0: return;
            default: printf("Invalid choice.\n"); pause_screen(); break;
        }
    } while (ch != 0);
}

void student_management_menu() {
    int ch;
    do {
        system("cls");
        printf("\n===========================================\n");
        printf("MANAGEMENT SYSTEM - STUDENT MANAGEMENT\n");
        printf("1. Display All Students\n");
        printf("2. Add New Student\n");
        printf("3. Edit Student Information\n");
        printf("4. Delete Student\n");
        printf("5. Search Students\n");
        printf("6. Sort Student List\n");
        printf("0. Back\n");
        printf("Choose: ");
        if (scanf("%d", &ch) != 1) { clear_input_buffer(); ch = -1; continue; }
        clear_input_buffer();
        switch (ch) {
            case 1: display_all_students(); pause_screen(); break;
            case 2: add_student(); pause_screen(); break;
            case 3: edit_student(); pause_screen(); break;
            case 4: delete_student(); pause_screen(); break;
            case 5: student_search_menu(); break;
            case 6: student_sort_menu(); break;
            case 0: return;
            default: break;
        }
    } while (ch != 0);
}

int check_teacher_duplicate(const char *email, const char *phone, const char *currentId) {
    for (int i = 0; i < teacherCount; ++i) {
        if (strcmp(teachers[i].teacherId, currentId) != 0) {
            if (ci_stricmp(teachers[i].email, email) == 0 && strlen(email) > 0) { return 1; }
            if (strcmp(teachers[i].phone, phone) == 0 && strlen(phone) > 0) { return 2; }
        }
    } return 0;
}

void input_teacher_email_phone(Teacher *t, const char *currentId) {
    char temp_email[MAX_LEN_EMAIL], temp_phone[MAX_LEN_PHONE];

    do {
        get_required_input("Enter Email: ", temp_email, MAX_LEN_EMAIL);
        if (!is_valid_email(temp_email)) { printf("Error: Invalid email format! Please re-enter email.\n"); continue; }
        if (check_teacher_duplicate(temp_email, "", currentId) == 1) {
            printf("Error: Email **%s** already exists! Please re-enter email.\n", temp_email);
            continue;
        }
        break;
    } while (1);

    do {
        get_required_input("Enter Phone number (10-11 digits): ", temp_phone, MAX_LEN_PHONE);
        if (!is_valid_phone(temp_phone)) { printf("Error: Phone must be 10 or 11 digits and contain only numbers! Please re-enter phone.\n"); continue; }
        if (check_teacher_duplicate("", temp_phone, currentId) == 2) {
             printf("Error: Phone number **%s** already exists! Please re-enter phone.\n", temp_phone);
             continue;
        }
        break;
    } while (1);

    strcpy(t->email, temp_email);
    strcpy(t->phone, temp_phone);
}

void input_teacher_info(Teacher *t, const char *currentId) {
    get_required_input("Enter Name: ", t->name, MAX_LEN_NAME);
    input_date_of_birth(&t->dob);
    input_teacher_email_phone(t, currentId);
    strcpy(t->password, "111");
}

void add_teacher() {
    if (teacherCount >= MAX_TEACHERS) { printf("Teacher list is full!\n"); return; }
    Teacher newTeacher = {0};
    printf("\n--- ADD NEW TEACHER ---\n");

    input_teacher_info(&newTeacher, "");

    sprintf(newTeacher.teacherId, "GV%d", nextTeacherId++);
    teachers[teacherCount++] = newTeacher;

    printf("\nSuccessfully added teacher **%s**! Teacher ID: **%s**, Default Password: 111\n", teachers[teacherCount-1].name, teachers[teacherCount-1].teacherId);
}

void get_teacher_id_to_edit(char *id, int *index) {
    do {
        get_required_input("Enter Teacher ID to edit: ", id, MAX_LEN_ID);
        *index = find_teacher_index_by_id(id);
        if (*index == -1) {
            printf("Error: Teacher ID **%s** does not exist! Please re-enter ID.\n", id);
        } else {
            break;
        }
    } while(1);
}

void edit_teacher() {
    char id[MAX_LEN_ID]; int index;
    printf("\n--- EDIT TEACHER INFORMATION ---\n");
    get_teacher_id_to_edit(id, &index);

    printf("\n--- Current information for teacher %s ---\n", teachers[index].name);
    print_teacher_table_header();
    print_teacher_row(teachers[index]);
    printf("================================================================================================================\n");

    printf("\nEnter new information:\n");
    input_teacher_info(&teachers[index], teachers[index].teacherId);
    printf("\nSuccessfully updated information for teacher **%s**!\n", teachers[index].teacherId);
}

void delete_teacher() {
    char id[MAX_LEN_ID], confirm[3]; int index;
    printf("\n--- DELETE TEACHER ---\n");
    do {
        get_required_input("Enter Teacher ID to delete: ", id, MAX_LEN_ID);
        index = find_teacher_index_by_id(id);
        if (index == -1) {
            printf("Error: Teacher ID **%s** does not exist! Please re-enter ID.\n", id);
        } else {
            break;
        }
    } while(1);

    if (teachers[index].numClasses > 0) {
        printf("Error: Teacher **%s** is currently assigned to %d classes. Please unassign classes first!\n", id, teachers[index].numClasses);
        pause_screen();
        return;
    }

    printf("Are you sure you want to delete teacher **%s** (%s)? (Y/N): ", teachers[index].name, id);
    fgets(confirm, sizeof(confirm), stdin); trim_whitespace(confirm);

    if (ci_stricmp(confirm, "Y") == 0) {
        for (int i = index; i < teacherCount - 1; i++) { teachers[i] = teachers[i+1]; }
        teacherCount--;
        printf("Successfully deleted teacher **%s**!\n", id);
    } else {
        printf("Deletion operation cancelled.\n");
    }
}

void display_teacher_assigned_classes(int index) {
    printf("\n--- ASSIGNED CLASSES (%d total) ---\n", teachers[index].numClasses);
    if (teachers[index].numClasses == 0) {
        printf("This teacher has no classes assigned.\n");
    } else {
        print_class_table_header();
        for (int i = 0; i < teachers[index].numClasses; i++) {
            int classIndex = find_class_index_by_id(teachers[index].classIds[i]);
            if (classIndex != -1) { print_class_row(classes[classIndex]); }
        }
        printf("========================================================\n");
    }
}

void view_teacher_details() {
    char id[MAX_LEN_ID]; int index;
    printf("\n--- TEACHER DETAILS ---\n");
    do {
        get_required_input("Enter Teacher ID to view details: ", id, MAX_LEN_ID);
        index = find_teacher_index_by_id(id);
        if (index == -1) { printf("Error: Teacher ID **%s** does not exist! Please re-enter ID.\n", id); } else { break; }
    } while(1);

    system("cls");
    printf("\n===========================================\n");
    printf("--- TEACHER DETAILS: %s (ID: %s) ---\n", teachers[index].name, id);
    print_teacher_table_header();
    print_teacher_row(teachers[index]);
    printf("================================================================================================================\n");

    display_teacher_assigned_classes(index);
}

void search_teacher_by_name() {
    char key[MAX_LEN_NAME];
    get_required_input("Enter teacher name (or part of the name) to search: ", key, MAX_LEN_NAME);
    Teacher results[MAX_TEACHERS]; int resCount = 0;

    for (int i = 0; i < teacherCount; ++i) {
        if (strstr(teachers[i].name, key) != NULL || ci_stricmp(teachers[i].name, key) == 0) {
            results[resCount++] = teachers[i];
        }
    }

    printf("\n--- SEARCH RESULTS FOR TEACHER NAME '%s' (%d results) ---\n", key, resCount);
    if (resCount == 0) {
        printf("No teachers found containing the string '%s'.\n", key);
    } else {
        print_teacher_table_header();
        for(int i=0; i<resCount; i++) print_teacher_row(results[i]);
        printf("================================================================================================================\n");
    }
}

int get_class_id_to_assign(char *classId) {
    int classIndex;
    do {
        get_required_input("Enter Class ID to assign: ", classId, MAX_LEN_ID);
        classIndex = find_class_index_by_id(classId);
        if (classIndex == -1) { printf("Error: Class ID **%s** does not exist! Please re-enter Class ID.\n", classId); continue; }
        if (strlen(classes[classIndex].teacherId) > 0) {
            printf("Error: Class **%s** already has a teacher (ID: %s)! Please unassign first.\n", classId, classes[classIndex].teacherId);
            classIndex = -1; continue;
        }
        break;
    } while(1);
    return classIndex;
}

int get_teacher_id_to_assign(char *teacherId) {
    int teacherIndex;
    do {
        get_required_input("Enter Teacher ID to assign to the class: ", teacherId, MAX_LEN_ID);
        teacherIndex = find_teacher_index_by_id(teacherId);
        if (teacherIndex == -1) { printf("Error: Teacher ID **%s** does not exist! Please re-enter Teacher ID.\n", teacherId); } else { break; }
    } while(1);
    return teacherIndex;
}

void assign_class_to_teacher() {
    char classId[MAX_LEN_ID], teacherId[MAX_LEN_ID];
    int classIndex, teacherIndex;

    printf("\n--- ASSIGN CLASS TO TEACHER ---\n");

    classIndex = get_class_id_to_assign(classId);
    if (classIndex == -1) return;

    teacherIndex = get_teacher_id_to_assign(teacherId);
    if (teacherIndex == -1) return;

    strcpy(classes[classIndex].teacherId, teacherId);
    strcpy(teachers[teacherIndex].classIds[teachers[teacherIndex].numClasses++], classId);
    printf("Successfully assigned class **%s** to teacher **%s**!\n", classes[classIndex].className, teachers[teacherIndex].name);
}

void teacher_management_menu() {
    int ch;
    do {
        system("cls");
        printf("\n===========================================\n");
        printf("MANAGEMENT SYSTEM - TEACHER MANAGEMENT\n");
        printf("1. Display All Teachers\n");
        printf("2. Add New Teacher\n");
        printf("3. Edit Teacher Information\n");
        printf("4. Delete Teacher\n");
        printf("5. Teacher Details (View Assigned Classes)\n");
        printf("6. Search Teacher by Name\n");
        printf("7. Assign Class to Teacher\n");
        printf("0. Back\n");
        printf("Choose: ");
        if (scanf("%d", &ch) != 1) { clear_input_buffer(); ch = -1; continue; }
        clear_input_buffer();
        switch (ch) {
            case 1: display_all_teachers(); pause_screen(); break;
            case 2: add_teacher(); pause_screen(); break;
            case 3: edit_teacher(); pause_screen(); break;
            case 4: delete_teacher(); pause_screen(); break;
            case 5: view_teacher_details(); pause_screen(); break;
            case 6: search_teacher_by_name(); pause_screen(); break;
            case 7: assign_class_to_teacher(); pause_screen(); break;
            case 0: return;
            default: break;
        }
    } while (ch != 0);
}

int is_class_name_duplicate(const char *name, const char *currentId) {
    for (int i = 0; i < classCount; ++i) if (strcmp(classes[i].classId, currentId) != 0 && ci_stricmp(classes[i].className, name) == 0) { return 1; } return 0;
}

void input_class_name(ClassRoom *c, const char *currentId) {
    do {
        get_required_input("Enter Class Name: ", c->className, MAX_LEN_NAME);
        if (is_class_name_duplicate(c->className, currentId)) {
            printf("Error: Class name **%s** already exists! Please re-enter Class Name.\n", c->className);
        } else {
            break;
        }
    } while(1);
}

void add_class_room() {
    if (classCount >= MAX_CLASSES) { printf("Class list is full!\n"); return; }
    ClassRoom newClass = {0};
    printf("\n--- ADD NEW CLASS ---\n");

    input_class_name(&newClass, "");

    sprintf(newClass.classId, "Lop%d", nextClassId++);
    classes[classCount++] = newClass;

    printf("\nSuccessfully added class **%s**! Class ID: **%s**\n", classes[classCount-1].className, classes[classCount-1].classId);
}

void get_class_id_to_edit(char *id, int *index) {
    do {
        get_required_input("Enter Class ID to edit: ", id, MAX_LEN_ID);
        *index = find_class_index_by_id(id);
        if (*index == -1) { printf("Error: Class ID **%s** does not exist! Please re-enter ID.\n", id); } else { break; }
    } while(1);
}

void edit_class_room() {
    char id[MAX_LEN_ID]; int index;
    printf("\n--- EDIT CLASS INFORMATION ---\n");
    get_class_id_to_edit(id, &index);

    printf("\n--- Current information for class %s ---\n", classes[index].className);
    print_class_table_header();
    print_class_row(classes[index]);
    printf("========================================================\n");

    printf("\nEnter new Class Name:\n");
    input_class_name(&classes[index], classes[index].classId);
    printf("\nSuccessfully updated information for class **%s**!\n", classes[index].classId);
}

void unassign_teacher_from_class_on_delete(const char *classId, const char *teacherId) {
    int teacherIndex = find_teacher_index_by_id(teacherId);
    if (teacherIndex != -1) {
        for(int i = 0; i < teachers[teacherIndex].numClasses; i++) {
            if (strcmp(teachers[teacherIndex].classIds[i], classId) == 0) {
                for(int j = i; j < teachers[teacherIndex].numClasses - 1; j++) {
                    strcpy(teachers[teacherIndex].classIds[j], teachers[teacherIndex].classIds[j+1]);
                }
                teachers[teacherIndex].numClasses--;
                break;
            }
        }
    }
}

void remove_students_from_class_on_delete(int classIndex) {
    for (int i = 0; i < classes[classIndex].numStudents; i++) {
        int studentIndex = find_student_index_by_id(classes[classIndex].studentIds[i]);
        if (studentIndex != -1) {
            strcpy(students[studentIndex].classroomId, "");
        }
    }
}

void delete_class_room() {
    char id[MAX_LEN_ID], confirm[3]; int index;
    printf("\n--- DELETE CLASS ---\n");
    do {
        get_required_input("Enter Class ID to delete: ", id, MAX_LEN_ID);
        index = find_class_index_by_id(id);
        if (index == -1) { printf("Error: Class ID **%s** does not exist! Please re-enter ID.\n", id); } else { break; }
    } while(1);

    if (classes[index].numStudents > 0) {
        printf("Warning: Class **%s** still has %d students. All students will be removed from this class upon deletion. Continue? (Y/N): ", classes[index].className, classes[index].numStudents);
        fgets(confirm, sizeof(confirm), stdin); trim_whitespace(confirm);
        if (ci_stricmp(confirm, "Y") != 0) {
            printf("Deletion operation cancelled.\n");
            return;
        }
    }

    printf("Are you sure you want to delete class **%s** (%s)? (Y/N): ", classes[index].className, id);
    fgets(confirm, sizeof(confirm), stdin); trim_whitespace(confirm);

    if (ci_stricmp(confirm, "Y") == 0) {
        remove_students_from_class_on_delete(index);

        if (strlen(classes[index].teacherId) > 0) {
            unassign_teacher_from_class_on_delete(id, classes[index].teacherId);
        }

        for (int i = index; i < classCount - 1; i++) { classes[i] = classes[i+1]; }
        classCount--;
        printf("Successfully deleted class **%s**!\n", id);
    } else {
        printf("Deletion operation cancelled.\n");
    }
}

void display_students_in_class(int classIndex) {
    Student sList[MAX_STUDENTS]; int sListCount = 0;
    for (int i = 0; i < classes[classIndex].numStudents; i++) {
        int sIdx = find_student_index_by_id(classes[classIndex].studentIds[i]);
        if (sIdx != -1) { sList[sListCount++] = students[sIdx]; }
    }
    display_students_in_list(sList, sListCount, "LIST OF STUDENTS IN CLASS");
}

int get_student_id_to_add(const char *classId) {
    char studentId[MAX_LEN_ID]; int studentIndex;
    do {
        get_required_input("Enter Student ID to add: ", studentId, MAX_LEN_ID);
        studentIndex = find_student_index_by_id(studentId);

        if (studentIndex == -1) { printf("Error: Student ID **%s** does not exist! Please re-enter ID.\n", studentId); continue; }
        if (strlen(students[studentIndex].classroomId) > 0) {
            printf("Error: Student **%s** is already in class **%s**! Please remove them from the old class first.\n", studentId, students[studentIndex].classroomId);
            studentIndex = -1; continue;
        }
        break;
    } while(1);
    return studentIndex;
}

void add_student_to_class_logic(int classIndex) {
    printf("\n--- ADD STUDENT TO CLASS %s ---\n", classes[classIndex].className);

    if (classes[classIndex].numStudents >= MAX_STUDENTS) {
        printf("Error: Class is full, cannot add student!\n");
        return;
    }

    int studentIndex = get_student_id_to_add(classes[classIndex].classId);
    if (studentIndex == -1) return;

    strcpy(students[studentIndex].classroomId, classes[classIndex].classId);
    strcpy(classes[classIndex].studentIds[classes[classIndex].numStudents++], students[studentIndex].studentId);
    printf("Successfully added student **%s** to class **%s**!\n", students[studentIndex].name, classes[classIndex].className);
}

int get_student_id_to_remove(int classIndex) {
    char studentId[MAX_LEN_ID]; int studentIndex;
    do {
        get_required_input("Enter Student ID to remove from class: ", studentId, MAX_LEN_ID);
        studentIndex = find_student_index_by_id(studentId);
        if (studentIndex == -1) { printf("Error: Student ID **%s** does not exist! Please re-enter ID.\n", studentId); continue; }
        if (strcmp(students[studentIndex].classroomId, classes[classIndex].classId) != 0) {
            printf("Error: Student ID **%s** does not belong to class **%s**! Please re-enter ID.\n", studentId, classes[classIndex].className); studentIndex = -1; continue;
        }
        break;
    } while(1);
    return studentIndex;
}

void remove_student_from_class_logic(int classIndex) {
    char confirm[3];
    printf("\n--- REMOVE STUDENT FROM CLASS %s ---\n", classes[classIndex].className);

    int studentIndex = get_student_id_to_remove(classIndex);
    if (studentIndex == -1) return;

    int sv_in_class_index = -1;
    const char *studentId = students[studentIndex].studentId;

    for (int i = 0; i < classes[classIndex].numStudents; i++) {
        if (strcmp(classes[classIndex].studentIds[i], studentId) == 0) { sv_in_class_index = i; break; }
    }

    printf("Are you sure you want to REMOVE student **%s** (%s) from the class? (Y/N): ", students[studentIndex].name, studentId);
    fgets(confirm, sizeof(confirm), stdin); trim_whitespace(confirm);

    if (ci_stricmp(confirm, "Y") == 0) {
        strcpy(students[studentIndex].classroomId, "");
        if (sv_in_class_index != -1) {
            for (int i = sv_in_class_index; i < classes[classIndex].numStudents - 1; i++) {
                strcpy(classes[classIndex].studentIds[i], classes[classIndex].studentIds[i+1]);
            }
            classes[classIndex].numStudents--;
        }
        printf("Successfully REMOVED student **%s** from class **%s**!\n", studentId, classes[classIndex].className);
    } else { printf("Student removal operation cancelled.\n"); }
}

void class_detail() {
    char id[MAX_LEN_ID]; int index;
    printf("\n--- CLASS DETAILS ---\n");
    do {
        get_required_input("Enter Class ID to view details: ", id, MAX_LEN_ID);
        index = find_class_index_by_id(id);
        if (index == -1) { printf("Error: Class ID **%s** does not exist! Please re-enter ID.\n", id); } else { break; }
    } while(1);

    int sub_ch;
    do {
        system("cls");
        printf("\n===========================================\n");
        printf("MANAGEMENT SYSTEM - CLASS DETAILS\n");
        printf("--- CLASS: %s (ID: %s) ---\n", classes[index].className, id);
        printf("Teacher: %s | Total Students: %d\n", strlen(classes[index].teacherId) > 0 ? classes[index].teacherId : "Unassigned", classes[index].numStudents);
        display_students_in_class(index);

        printf("\n1. Add Student to Class\n");
        printf("2. Remove Student from Class\n");
        printf("0. Return to Class Management Menu\n");
        printf("Choose: ");

        if (scanf("%d", &sub_ch) != 1) { clear_input_buffer(); sub_ch = -1; continue; }
        clear_input_buffer();

        if (sub_ch == 1) add_student_to_class_logic(index);
        else if (sub_ch == 2) remove_student_from_class_logic(index);
        else if (sub_ch != 0) printf("Invalid choice!\n");

        if (sub_ch != 0) pause_screen();
    } while (sub_ch != 0);
}

int cmp_c_name_asc(const void *a, const void *b) { return ci_stricmp(((const ClassRoom *)a)->className, ((const ClassRoom *)b)->className); }
int cmp_c_name_desc(const void *a, const void *b) { return ci_stricmp(((const ClassRoom *)b)->className, ((const ClassRoom *)a)->className); }
void class_sort_menu() {
    int ch;
    do {
        system("cls");
        printf("\n--- SORT CLASS LIST ---\n");
        printf("1. Sort by Class Name (A->Z)\n");
        printf("2. Sort by Class Name (Z->A)\n");
        printf("0. Back\n");
        printf("Choose: ");
        if (scanf("%d", &ch) != 1) { clear_input_buffer(); ch = -1; continue; }
        clear_input_buffer();
        switch (ch) {
            case 1:
                qsort(classes, classCount, sizeof(ClassRoom), cmp_c_name_asc);
                printf("Successfully sorted by Class Name (A->Z)!\n");
                display_all_classes(); pause_screen(); break;
            case 2:
                qsort(classes, classCount, sizeof(ClassRoom), cmp_c_name_desc);
                printf("Successfully sorted by Class Name (Z->A)!\n");
                display_all_classes(); pause_screen(); break;
            case 0: return;
            default: printf("Invalid choice.\n"); pause_screen(); break;
        }
    } while (ch != 0);
}

void class_management_menu() {
    int ch;
    do {
        system("cls");
        printf("\n===========================================\n");
        printf("MANAGEMENT SYSTEM - CLASS MANAGEMENT\n");
        printf("1. Display All Classes\n");
        printf("2. Add New Class\n");
        printf("3. Edit Class Information\n");
        printf("4. Delete Class\n");
        printf("5. Class Details (Add/Remove Students)\n");
        printf("6. Sort Class List\n");
        printf("0. Back\n");
        printf("Choose: ");
        if (scanf("%d", &ch) != 1) { clear_input_buffer(); ch = -1; continue; }
        clear_input_buffer();
        switch (ch) {
            case 1: display_all_classes(); pause_screen(); break;
            case 2: add_class_room(); pause_screen(); break;
            case 3: edit_class_room(); pause_screen(); break;
            case 4: delete_class_room(); pause_screen(); break;
            case 5: class_detail(); break;
            case 6: class_sort_menu(); break;
            case 0: return;
            default: break;
        }
    } while (ch != 0);
}

void admin_main_menu() {
    int choice;
    do {
        system("cls");
        printf("\n===========================================\n");
        printf("STUDENT MANAGEMENT SYSTEM - ADMIN MENU\n");
        printf("1. Student Management\n");
        printf("2. Teacher Management\n");
        printf("3. Class Management\n");
        printf("0. Logout\n");
        printf("Choose: ");

        if (scanf("%d", &choice) != 1) { clear_input_buffer(); choice = -1; continue; }
        clear_input_buffer();

        switch (choice) {
            case 1: student_management_menu(); break;
            case 2: teacher_management_menu(); break;
            case 3: class_management_menu(); break;
            case 0: return;
            default: break;
        }
    } while (choice != 0);
}

int admin_login() {
    char email[MAX_LEN_EMAIL];
    char password[MAX_LEN_PASS];
    int attempts = 3;

    while (attempts > 0) {
        system("cls");
        printf("\n===========================================\n");
        printf("               ADMIN LOGIN\n");
        printf("===========================================\n");

        get_required_input("Enter Admin Email (Default: admin@gmail.com): ", email, MAX_LEN_EMAIL);
        get_required_input("Enter Password (Default: 123): ", password, MAX_LEN_PASS);

        if (ci_stricmp(email, ADMIN_EMAIL) == 0 && strcmp(password, ADMIN_PASS) == 0) {
            printf("\nLogin successful! Entering Admin Menu...\n");
            return 1;
        } else {
            attempts--;
            printf("\nInvalid login credentials! %d attempts remaining.\n", attempts);
            pause_screen();
        }
    }
    return 0;
}

void initialize_data() {
    strcpy(students[0].studentId, "SV1"); strcpy(students[0].name, "Nguyen Van A"); students[0].dob = (Date){10, 5, 2000}; strcpy(students[0].email, "nguyena@mail.com"); strcpy(students[0].phone, "0161111111"); students[0].gender = 1; strcpy(students[0].password, "111"); studentCount++; nextStudentId++;
    strcpy(students[1].studentId, "SV2"); strcpy(students[1].name, "Tran Thi B"); students[1].dob = (Date){15, 8, 2001}; strcpy(students[1].email, "tranb@mail.com"); strcpy(students[1].phone, "0161111112"); students[1].gender = 0; strcpy(students[1].password, "111"); studentCount++; nextStudentId++;
    strcpy(students[2].studentId, "SV3"); strcpy(students[2].name, "Le Van C"); students[2].dob = (Date){20, 12, 2000}; strcpy(students[2].email, "lec@mail.com"); strcpy(students[2].phone, "0161111113"); students[2].gender = 1; strcpy(students[2].password, "111"); studentCount++; nextStudentId++;
    strcpy(students[3].studentId, "SV4"); strcpy(students[3].name, "Pham Thi D"); students[3].dob = (Date){5, 3, 2002}; strcpy(students[3].email, "phamd@mail.com"); strcpy(students[3].phone, "0161111114"); students[3].gender = 0; strcpy(students[3].classroomId, "Lop2"); strcpy(students[3].password, "111"); studentCount++; nextStudentId++;
    strcpy(students[4].studentId, "SV5"); strcpy(students[4].name, "Hoang Van E"); students[4].dob = (Date){1, 1, 2003}; strcpy(students[4].email, "hoange@mail.com"); strcpy(students[4].phone, "0161111115"); students[4].gender = 1; strcpy(students[4].classroomId, "Lop1"); strcpy(students[4].password, "111"); studentCount++; nextStudentId++;
    strcpy(students[5].studentId, "SV6"); strcpy(students[5].name, "Nguyen Thi G"); students[5].dob = (Date){10, 10, 2000}; strcpy(students[5].email, "nguyeng@mail.com"); strcpy(students[5].phone, "0161111116"); students[5].gender = 0; strcpy(students[5].classroomId, "Lop1"); strcpy(students[5].password, "111"); studentCount++; nextStudentId++;
    strcpy(students[6].studentId, "SV7"); strcpy(students[6].name, "Tran Van H"); students[6].dob = (Date){1, 1, 2000}; strcpy(students[6].email, "tranh@mail.com"); strcpy(students[6].phone, "0161111117"); students[6].gender = 1; strcpy(students[6].classroomId, "Lop2"); strcpy(students[6].password, "111"); studentCount++; nextStudentId++;
    strcpy(students[7].studentId, "SV8"); strcpy(students[7].name, "Le Thi K"); students[7].dob = (Date){5, 5, 2001}; strcpy(students[7].email, "lek@mail.com"); strcpy(students[7].phone, "0161111118"); students[7].gender = 0; strcpy(students[7].classroomId, "Lop1"); strcpy(students[7].password, "111"); studentCount++; nextStudentId++;

    strcpy(teachers[0].teacherId, "GV1"); strcpy(teachers[0].name, "Thay Nguyen"); teachers[0].dob = (Date){1, 1, 1985}; strcpy(teachers[0].email, "nguyen@giao.com"); strcpy(teachers[0].phone, "0901234567"); strcpy(teachers[0].password, "111"); teachers[0].numClasses = 1; teacherCount++; nextTeacherId++;
    strcpy(teachers[1].teacherId, "GV2"); strcpy(teachers[1].name, "Co Tran"); teachers[1].dob = (Date){12, 10, 1978}; strcpy(teachers[1].email, "tran@giao.com"); strcpy(teachers[1].phone, "0901234568"); strcpy(teachers[1].password, "111"); teachers[1].numClasses = 1; teacherCount++; nextTeacherId++;

    strcpy(classes[0].classId, "Lop1"); strcpy(classes[0].className, "Lap Trinh C"); strcpy(classes[0].teacherId, "GV1"); classes[0].numStudents = 3; classCount++; nextClassId++;
    strcpy(classes[1].classId, "Lop2"); strcpy(classes[1].className, "Cau Truc Du Lieu"); strcpy(classes[1].teacherId, "GV2"); classes[1].numStudents = 2; classCount++; nextClassId++;

    strcpy(classes[0].studentIds[0], "SV5");
    strcpy(classes[0].studentIds[1], "SV6");
    strcpy(classes[0].studentIds[2], "SV8");
    strcpy(classes[1].studentIds[0], "SV4");
    strcpy(classes[1].studentIds[1], "SV7");

    strcpy(teachers[0].classIds[0], "Lop1");
    strcpy(teachers[1].classIds[0], "Lop2");
}

int main() {
    initialize_data();
    int ch;
    while (1) {
        system("cls");
        printf("\n*** STUDENT MANAGEMENT SYSTEM (C Language) ***\n");
        printf("\nSELECT ROLE\n");
        printf("===================================\n");
        printf("[1] Admin.\n");
        printf("[2] Student (Function under development).\n");
        printf("[3] Teacher (Function under development).\n");
        printf("[0] Exit Program.\n");
        printf("===================================\n");
        printf("\nEnter choice: ");

        if (scanf("%d", &ch) != 1) { clear_input_buffer(); pause_screen(); continue; }
        clear_input_buffer();

        switch (ch) {
            case 1:
                if (admin_login()) {
                    admin_main_menu();
                }
                break;
            case 2:
            case 3:
                printf("Login function for this role is under development.\n");
                pause_screen();
                break;
            case 0:
                printf("\n--- Thank you for using the program ---\n");
                return 0;
            default:
                printf("Invalid choice.\n");
                pause_screen();
                break;
        }
    }
    return 0;
}
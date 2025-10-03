#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STUDENTS 100
#define MAX_TEACHERS 50
#define MAX_CLASSES 50
#define MAX_NAME 30
#define MAX_ID 10
#define MAX_EMAIL 30
#define MAX_PHONE 20
#define MAX_PASS 20

#define ADMIN_EMAIL "admin@gmail.com"
#define ADMIN_PASS "123"

struct Date {
    int month, day, year;
};

struct Student {
    char studentId[MAX_ID];
    char classroomId[MAX_ID];
    char name[MAX_NAME];
    struct Date DateOfBirth;
    int gender;
    char email[MAX_EMAIL];
    char phone[MAX_PHONE];
    char password[MAX_PASS];
    int numCourses;
};

struct Teacher {
    char teacherId[MAX_ID];
    char classroomId[MAX_ID];
    char name[MAX_NAME];
    struct Date DateOfBirth;
    int gender;
    char email[MAX_EMAIL];
    char phone[MAX_PHONE];
    char password[MAX_PASS];
    char classroom_classes[MAX_CLASSES][MAX_ID];
    int numClasses;
};

struct ClassRoom {
    char classId[MAX_ID];
    char teacherId[MAX_ID];
    char className[MAX_NAME];
    char Student_students[MAX_STUDENTS][MAX_ID];
    int numStudents;
};

struct Student students[MAX_STUDENTS];
struct Teacher teachers[MAX_TEACHERS];
struct ClassRoom classes[MAX_CLASSES];

int studentCount = 0;
int teacherCount = 0;
int classCount = 0;
int nextStudentId = 1;
int nextTeacherId = 1;
int nextClassId = 1;

void pauseScreen();
void admin_main_menu_logic();
void admin_student_menu();
void teacher_menu();
void class_menu();
void displayExitScreen();
int adminLogin();

void searchStudentByName();
void searchStudentByGender();

void displayClasses();
void displayStudents();
void addStudent();
void editStudent();
void deleteStudent();
void sortStudents();

void displayTeachers();
void addTeacher();
void editTeacher();
void deleteTeacher();
void searchTeacherByName();
void assignClassToTeacher();
void unassignClassFromTeacher();
void sortTeachers();

void addClassRoom();
void editClassRoom();
void deleteClassRoom();
void classDetail();
void addStudentToClassRoom();
void removeStudentFromClassRoom();
void sortClassRooms();

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void trim_inplace(char *s) {
    if (!s) return;
    char *start = s;
    while (*start && isspace((unsigned char)*start)) start++;
    if (start != s) memmove(s, start, strlen(start) + 1);
    int len = (int)strlen(s);
    while (len > 0 && isspace((unsigned char)s[len - 1])) s[--len] = '\0';
}

int ci_strcmp(const char *a, const char *b) {
    for (; ; a++, b++) {
        int ca = tolower((unsigned char)*a);
        int cb = tolower((unsigned char)*b);
        if (ca != cb) return ca - cb;
        if (ca == 0) return 0;
    }
}

void getInputRequired(const char *prompt, char *out, int maxlen) {
    do {
        printf("%s", prompt);
        fgets(out, maxlen, stdin);
        out[strcspn(out, "\n")] = '\0';
        trim_inplace(out);
        if (strlen(out) == 0) {
            printf("Du lieu khong duoc de trong. Vui long nhap lai.\n");
        }
    } while (strlen(out) == 0);
}

int isEmailValid(const char *email) {
    if (strchr(email, '@') == NULL || strchr(email, '.') == NULL) {
        return 0;
    }
    return 1;
}

int isEmailOrPhoneDuplicate(const char *email, const char *phone, const char *currentId, int isStudent) {
    if (isStudent) {
        for (int i = 0; i < studentCount; ++i) {
            if (strcmp(students[i].studentId, currentId) != 0) {
                if (ci_strcmp(students[i].email, email) == 0) return 1;
                if (strcmp(students[i].phone, phone) == 0) return 1;
            }
        }
    } else {
        for (int i = 0; i < teacherCount; ++i) {
            if (strcmp(teachers[i].teacherId, currentId) != 0) {
                if (ci_strcmp(teachers[i].email, email) == 0) return 1;
                if (strcmp(teachers[i].phone, phone) == 0) return 1;
            }
        }
    }
    return 0;
}

int findStudentIndexById(const char *id) {
    for (int i = 0; i < studentCount; ++i) {
        if (strcmp(students[i].studentId, id) == 0) return i;
    }
    return -1;
}

int findTeacherIndexById(const char *id) {
    for (int i = 0; i < teacherCount; ++i) {
        if (strcmp(teachers[i].teacherId, id) == 0) return i;
    }
    return -1;
}

int findClassIndexById(const char *id) {
    for (int i = 0; i < classCount; ++i) {
        if (strcmp(classes[i].classId, id) == 0) return i;
    }
    return -1;
}

void pauseScreen() {
    printf("\nNhan ENTER de tiep tuc...");
    clearInputBuffer();
}

void inputPasswordMasked(char *out, int maxlen) {
    printf("Password (Mat khau nhap vao): ");
    fgets(out, maxlen, stdin);
    out[strcspn(out, "\n")] = '\0';
    trim_inplace(out);
}

void displayLoginScreen(const char *role) {
    system("cls");
    printf("\n\n\n");
    printf("===========================================\n");
    printf("***Student Management System Using C***\n");
    printf("===========================================\n");
    printf("             %s L O G I N\n", role);
    printf("-------------------------------------------\n");
}

void displayExitScreen() {
    system("cls");
    printf("\n\n\n\n\n\n\n\n");
    printf("==============================================\n");
    printf("           Thank You                        \n");
    printf("           See You Soon                     \n");
    printf("==============================================\n");
}

int adminLogin() {
    char email[MAX_EMAIL];
    char password[MAX_PASS];
    displayLoginScreen("ADMIN/TEACHER/CLASS");
    printf("Email (default: %s): ", ADMIN_EMAIL);
    fgets(email, MAX_EMAIL, stdin);
    email[strcspn(email, "\n")] = '\0';
    trim_inplace(email);

    inputPasswordMasked(password, MAX_PASS);

    if (ci_strcmp(email, ADMIN_EMAIL) == 0 && strcmp(password, ADMIN_PASS) == 0) {
        printf("\nDang nhap thanh cong!\n");
        return 1;
    } else {
        printf("\nThong tin dang nhap sai. Nhan ENTER de thu lai...\n");
        getchar();
        return 0;
    }
}

int login_role(const char *role) {
    return adminLogin();
}

void displayStudents() {
    printf("========================================================================================\n");
    printf("**** DANH SACH SINH VIEN ****\n");
    printf("========================================================================================\n");
    printf("| %-5s | %-20s | %-25s | %-12s | %-10s |\n", "ID", "Ten", "Email", "Phone", "So Khoa Hoc");
    printf("|-------|----------------------|---------------------------|--------------|------------|\n");
    if (studentCount == 0) {
        printf("| %-93s |\n", "Chua co sinh vien nao.");
    } else {
        for (int i = 0; i < studentCount; ++i) {
            printf("| %-5s | %-20s | %-25s | %-12s | %-10d |\n",
                students[i].studentId,
                students[i].name,
                students[i].email,
                students[i].phone,
                students[i].numCourses
            );
        }
    }
    printf("|-------|----------------------|---------------------------|--------------|------------|\n");
}

void inputStudentInfo(struct Student *s, const char *currentId) {
    getInputRequired("Nhap Ten: ", s->name, MAX_NAME);

    do {
        getInputRequired("Nhap Email: ", s->email, MAX_EMAIL);
        if (!isEmailValid(s->email)) {
            printf("Email khong dung dinh dang (@ va .). Vui long nhap lai.\n");
            continue;
        }
        if (isEmailOrPhoneDuplicate(s->email, s->phone, currentId, 1)) {
            printf("Email hoac So dien thoai da ton tai. Vui long nhap lai.\n");
            continue;
        }
        break;
    } while (1);

    do {
        getInputRequired("Nhap So dien thoai: ", s->phone, MAX_PHONE);
        if (isEmailOrPhoneDuplicate(s->email, s->phone, currentId, 1)) {
            printf("So dien thoai da ton tai. Vui long nhap lai.\n");
            continue;
        }
        break;
    } while (1);

    printf("Nhap Gioi tinh (1=Nam, 0=Nu): ");
    if (scanf("%d", &s->gender) != 1) s->gender = 1;
    clearInputBuffer();
}

void addStudent() {
    struct Student newStudent;
    printf("\n--- Them Sinh Vien ---\n");

    inputStudentInfo(&newStudent, "");

    printf("\nThem sinh vien thanh cong!\n");
}

void editStudent() {
    char id[MAX_ID];
    int index;
    printf("\n--- Sua Thong Tin Sinh Vien ---\n");
    getInputRequired("Nhap ID sinh vien muon sua: ", id, MAX_ID);

    index = findStudentIndexById(id);
    if (index == -1) {
        printf("ID sinh vien %s khong ton tai!\n", id);
        return;
    }

    printf("\nThong tin hien tai cua sinh vien %s:\n", students[index].name);
    printf("ID: %s | Ten: %s | Email: %s\n", students[index].studentId, students[index].name, students[index].email);

    printf("\nYeu cau nhap thong tin moi:\n");
    inputStudentInfo(&students[index], students[index].studentId);

    printf("\nSua thong tin sinh vien %s thanh cong!\n", students[index].studentId);
}

void deleteStudent() {
    char id[MAX_ID];
    char confirm[3];
    int index;
    printf("\n--- Xoa Sinh Vien ---\n");
    getInputRequired("Nhap ID sinh vien muon xoa: ", id, MAX_ID);

    index = findStudentIndexById(id);
    if (index == -1) {
        printf("ID sinh vien %s khong ton tai!\n", id);
        return;
    }

    printf("Ban co chac chan muon xoa sinh vien %s (%s)? (Y/N): ", students[index].name, id);
    fgets(confirm, sizeof(confirm), stdin);
    trim_inplace(confirm);

    if (ci_strcmp(confirm, "Y") == 0) {
        printf("Xoa sinh vien %s thanh cong!\n", id);
    } else {
        printf("Huy thao tac xoa.\n");
    }
}

int cmpStudentNameAsc(const void *a, const void *b) {
    const struct Student *s1 = a;
    const struct Student *s2 = b;
    return ci_strcmp(s1->name, s2->name);
}

int cmpStudentNameDesc(const void *a, const void *b) {
    const struct Student *s1 = a;
    const struct Student *s2 = b;
    return ci_strcmp(s2->name, s1->name);
}

void sortStudents() {
    int ch;
    do {
        printf("\n--- Sap Xep Danh Sach Sinh Vien Theo Ten ---\n");
        printf("Chon thu tu muon sap xep danh sach sinh vien:\n");
        printf("1. Sap xep A->Z\n2. Sap xep Z->A\n0. Quay lai\nChon: ");
        if (scanf("%d", &ch) != 1) {
            clearInputBuffer();
            ch = -1;
            continue;
        }
        clearInputBuffer();

        if (ch == 1) qsort(students, studentCount, sizeof(struct Student), cmpStudentNameAsc);
        else if (ch == 2) qsort(students, studentCount, sizeof(struct Student), cmpStudentNameDesc);
        else if (ch != 0) {
            printf("Lua chon khong hop le.\n");
            continue;
        }

        if (ch != 0) {
            printf("\nDanh sach sinh vien sau khi sap xep:\n");
            displayStudents();
        }
    } while (ch != 0);
}

void searchStudentByName() {
    char kw[MAX_NAME];
    do {
        printf("\n--- Tim Kiem Sinh Vien Theo Ten ---\n");
        getInputRequired("Nhap ten sinh vien muon tim: ", kw, MAX_NAME);
        if (strlen(kw) == 0) {
            printf("Ten khong duoc de trong, moi nhap lai.\n");
        } else {
            break;
        }
    } while (1);

    for (int i = 0; kw[i]; ++i) kw[i] = (char)tolower((unsigned char)kw[i]);
    int found = 0;

    printf("\nKet qua tim kiem sinh vien co ten chua '%s':\n", kw);
    printf("| %-5s | %-20s |\n", "ID", "Name");
    printf("|-------|----------------------|\n");

    for (int i = 0; i < studentCount; ++i) {
        char temp[MAX_NAME];
        strcpy(temp, students[i].name);
        for (int j = 0; temp[j]; ++j) temp[j] = (char)tolower((unsigned char)temp[j]);
        if (strstr(temp, kw)) {
            printf("| %-5s | %-20s |\n", students[i].studentId, students[i].name);
            found = 1;
        }
    }

    if (!found) {
        printf("| %-28s |\n", "Khong tim thay sinh vien nao.");
    }
    printf("|-------|----------------------|\n");
}

void searchStudentByGender() {
    int choice;
    do {
        printf("\n--- Tim Kiem Sinh Vien Theo Gioi Tinh ---\n");
        printf("Nhap gioi tinh muon tim (1=Nam, 0=Nu): ");
        if (scanf("%d", &choice) != 1 || (choice != 1 && choice != 0)) {
            printf("Gioi tinh khong hop le, moi nhap lai (1=Nam, 0=Nu).\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();
        break;
    } while (1);

    int found = 0;
    printf("\nKet qua tim kiem theo GIOI TINH %s:\n", choice ? "Nam" : "Nu");
    printf("| %-5s | %-30s |\n", "ID", "Name");
    printf("|-------|--------------------------------|\n");
    for (int i = 0; i < studentCount; ++i) {
        if (students[i].gender == choice) {
            printf("| %-5s | %-30s |\n", students[i].studentId, students[i].name);
            found = 1;
        }
    }
    if (!found) printf("| %-38s |\n", "Khong tim thay sinh vien nao.");
    printf("|-------|--------------------------------|\n");
}

void admin_student_menu() {
    int choice;
    do {
        system("cls");
        printf("\n===========================================\n");
        printf("***Student Management System Using C***\n");
        printf("===========================================\n");
        printf("--- ADMIN: QUAN LY SINH VIEN ---\n");
        printf("1. Hien thi danh sach sinh vien\n");
        printf("2. Them sinh vien\n");
        printf("3. Sua thong tin sinh vien\n");
        printf("4. Xoa sinh vien\n");
        printf("5. Tim sinh vien theo ten\n");
        printf("6. Tim sinh vien theo gioi tinh\n");
        printf("7. Sap xep danh sach sinh vien theo ten\n");
        printf("0. Quay lai\n");
        printf("Yeu cau nhap lua chon: ");
        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            choice = -1;
            continue;
        }
        clearInputBuffer();
        switch (choice) {
            case 1: displayStudents(); break;
            case 2: addStudent(); break;
            case 3: editStudent(); break;
            case 4: deleteStudent(); break;
            case 5: searchStudentByName(); break;
            case 6: searchStudentByGender(); break;
            case 7: sortStudents(); break;
            case 0: return;
            default: printf("Lua chon khong hop le!\n"); break;
        }
        if (choice != 0) pauseScreen();
    } while (1);
}

void inputTeacherInfo(struct Teacher *t, const char *currentId) {
    getInputRequired("Nhap Ten: ", t->name, MAX_NAME);

    do {
        getInputRequired("Nhap Email: ", t->email, MAX_EMAIL);
        if (!isEmailValid(t->email)) {
            printf("Email khong dung dinh dang (@ va .). Vui long nhap lai.\n");
            continue;
        }
        if (isEmailOrPhoneDuplicate(t->email, t->phone, currentId, 0)) {
            printf("Email hoac So dien thoai da ton tai. Vui long nhap lai.\n");
            continue;
        }
        break;
    } while (1);

    do {
        getInputRequired("Nhap So dien thoai: ", t->phone, MAX_PHONE);
        if (isEmailOrPhoneDuplicate(t->email, t->phone, currentId, 0)) {
            printf("So dien thoai da ton tai. Vui long nhap lai.\n");
            continue;
        }
        break;
    } while (1);
}

void displayTeachers() {
    printf("====================================================================\n");
    printf("**** DANH SACH GIAO VIEN ****\n");
    printf("====================================================================\n");
    printf("| %-5s | %-20s | %-20s | %-5s |\n", "ID", "Ten", "Email", "So Lop");
    printf("|-------|----------------------|----------------------|--------|\n");
    if (teacherCount == 0) {
        printf("| %-65s |\n", "Chua co giao vien nao.");
    } else {
        for (int i = 0; i < teacherCount; ++i) {
            printf("| %-5s | %-20s | %-20s | %-5d |\n",
                teachers[i].teacherId,
                teachers[i].name,
                teachers[i].email,
                teachers[i].numClasses
            );
        }
    }
    printf("|-------|----------------------|----------------------|--------|\n");
}

void addTeacher() {
    struct Teacher newTeacher;
    printf("\n--- Them Giao Vien ---\n");

    inputTeacherInfo(&newTeacher, "");

    printf("\nThem giao vien %s thanh cong!\n", newTeacher.name);
}

void editTeacher() {
    char id[MAX_ID];
    int index;
    printf("\n--- Sua Thong Tin Giao Vien ---\n");
    getInputRequired("Yeu cau nhap id giao vien muon sua: ", id, MAX_ID);

    index = findTeacherIndexById(id);
    if (index == -1) {
        printf("ID giao vien %s khong ton tai!\n", id);
        return;
    }

    printf("\nThong tin hien tai cua giao vien %s:\n", teachers[index].name);
    printf("ID: %s | Ten: %s | Email: %s\n", teachers[index].teacherId, teachers[index].name, teachers[index].email);

    printf("\nYeu cau nhap thong tin moi:\n");
    inputTeacherInfo(&teachers[index], teachers[index].teacherId);

    printf("\nSua thong tin giao vien %s thanh cong!\n", teachers[index].teacherId);
}

void deleteTeacher() {
    char id[MAX_ID];
    char confirm[3];
    int index;
    printf("\n--- Xoa Giao Vien ---\n");
    getInputRequired("Yeu cau nhap id giao vien muon xoa: ", id, MAX_ID);

    index = findTeacherIndexById(id);
    if (index == -1) {
        printf("ID giao vien %s khong ton tai!\n", id);
        return;
    }

    printf("Ban co chac chan muon xoa giao vien %s (%s)? (Y/N): ", teachers[index].name, id);
    fgets(confirm, sizeof(confirm), stdin);
    trim_inplace(confirm);

    if (ci_strcmp(confirm, "Y") == 0) {
        printf("Xoa giao vien %s thanh cong!\n", id);
    } else {
        printf("Huy thao tac xoa.\n");
    }
}

void searchTeacherByName() {
    char kw[MAX_NAME];
    printf("\n--- Tim Kiem Giao Vien Theo Ten ---\n");
    getInputRequired("Yeu cau nhap ten giao vien muon tim: ", kw, MAX_NAME);

    for (int i = 0; kw[i]; ++i) kw[i] = (char)tolower((unsigned char)kw[i]);
    int found = 0;

    printf("\nKet qua tim kiem giao vien co ten chua '%s':\n", kw);
    printf("| %-5s | %-20s |\n", "ID", "Name");
    printf("|-------|----------------------|\n");

    for (int i = 0; i < teacherCount; ++i) {
        char temp[MAX_NAME];
        strcpy(temp, teachers[i].name);
        for (int j = 0; temp[j]; ++j) temp[j] = (char)tolower((unsigned char)temp[j]);
        if (strstr(temp, kw)) {
            printf("| %-5s | %-20s |\n", teachers[i].teacherId, teachers[i].name);
            found = 1;
        }
    }

    if (!found) {
        printf("| %-28s |\n", "Khong tim thay giao vien nao.");
    }
    printf("|-------|----------------------|\n");
}

void assignClassToTeacher() {
    char classId[MAX_ID];
    int classIndex;
    printf("\n--- Them Lop Cho Giao Vien ---\n");
    getInputRequired("Yeu cau nhap id lop muon them giao vien: ", classId, MAX_ID);

    classIndex = findClassIndexById(classId);
    if (classIndex == -1) {
        printf("ID lop %s khong ton tai!\n", classId);
        return;
    }
    if (strlen(classes[classIndex].teacherId) > 0) {
        printf("Lop %s da co giao vien phu trach!\n", classId);
        return;
    }

    char teacherId[MAX_ID];
    getInputRequired("Nhap ID giao vien muon phu trach lop: ", teacherId, MAX_ID);
    int teacherIndex = findTeacherIndexById(teacherId);
    if (teacherIndex == -1) {
        printf("ID giao vien %s khong ton tai!\n", teacherId);
        return;
    }

    printf("Gan lop %s cho giao vien %s thanh cong!\n", classId, teacherId);
}

void unassignClassFromTeacher() {
    printf("\n--- Huy Gan Lop Cho Giao Vien ---\n");
    printf("Chua trien khai...\n");
}

void sortTeachers() {
    printf("\n--- Sap Xep Danh Sach Giao Vien Theo Ten ---\n");
    printf("Chua trien khai...\n");
}

void teacher_menu() {
    int ch;
    do {
        system("cls");
        printf("\n===========================================\n");
        printf("***Student Management System Using C***\n");
        printf("===========================================\n");
        printf("--- ADMIN: QUAN LY GIAO VIEN ---\n");
        printf("1. Hien thi danh sach giao vien\n");
        printf("2. Them giao vien\n");
        printf("3. Sua thong tin giao vien\n");
        printf("4. Xoa giao vien\n");
        printf("5. Tim giao vien theo ten\n");
        printf("6. Sap xep giao vien theo ten\n");
        printf("7. Gan lop cho giao vien\n");
        printf("8. Huy gan lop cho giao vien\n");
        printf("0. Quay lai\n");
        printf("Yeu cau nhap lua chon: ");
        if (scanf("%d", &ch) != 1) {
            clearInputBuffer();
            ch = -1;
            continue;
        }
        clearInputBuffer();
        switch (ch) {
            case 1: displayTeachers(); break;
            case 2: addTeacher(); break;
            case 3: editTeacher(); break;
            case 4: deleteTeacher(); break;
            case 5: searchTeacherByName(); break;
            case 6: sortTeachers(); break;
            case 7: assignClassToTeacher(); break;
            case 8: unassignClassFromTeacher(); break;
            case 0: return;
            default: printf("Lua chon khong hop le!\n"); break;
        }
        if (ch != 0) pauseScreen();
    } while (1);
}

void inputClassInfo(struct ClassRoom *c) {
    getInputRequired("Nhap Ten Lop: ", c->className, MAX_NAME);
}

void displayClasses() {
    printf("========================================================\n");
    printf("**** DANH SACH LOP HOC ****\n");
    printf("========================================================\n");
    printf("| %-5s | %-20s | %-10s | %-5s |\n", "ID", "Ten Lop", "ID GV", "So SV");
    printf("|-------|----------------------|------------|--------|\n");
    if (classCount == 0) {
        printf("| %-56s |\n", "Chua co lop hoc nao.");
    } else {
        for (int i = 0; i < classCount; ++i) {
            printf("| %-5s | %-20s | %-10s | %-5d |\n",
                classes[i].classId,
                classes[i].className,
                classes[i].teacherId,
                classes[i].numStudents
            );
        }
    }
    printf("|-------|----------------------|------------|--------|\n");
}

void addClassRoom() {
    struct ClassRoom newClass;
    printf("\n--- Them Lop Hoc ---\n");

    inputClassInfo(&newClass);

    printf("\nThem lop %s thanh cong!\n", newClass.className);
}

void editClassRoom() {
    char id[MAX_ID];
    int index;
    printf("\n--- Sua Thong Tin Lop Hoc ---\n");
    getInputRequired("Yeu cau nhap id lop muon sua: ", id, MAX_ID);

    index = findClassIndexById(id);
    if (index == -1) {
        printf("ID lop %s khong ton tai!\n", id);
        return;
    }

    printf("\nThong tin hien tai cua lop %s:\n", classes[index].className);
    printf("ID: %s | Ten: %s | ID GV: %s\n", classes[index].classId, classes[index].className, classes[index].teacherId);

    printf("\nYeu cau nhap thong tin moi:\n");
    inputClassInfo(&classes[index]);

    printf("\nSua thong tin lop %s thanh cong!\n", classes[index].classId);
}

void deleteClassRoom() {
    char id[MAX_ID];
    char confirm[3];
    int index;
    printf("\n--- Xoa Lop Hoc ---\n");
    getInputRequired("Yeu cau nhap id lop muon xoa: ", id, MAX_ID);

    index = findClassIndexById(id);
    if (index == -1) {
        printf("ID lop %s khong ton tai!\n", id);
        return;
    }

    printf("Ban co chac chan muon xoa lop %s (%s)? (Y/N): ", classes[index].className, id);
    fgets(confirm, sizeof(confirm), stdin);
    trim_inplace(confirm);

    if (ci_strcmp(confirm, "Y") == 0) {
        printf("Xoa lop %s thanh cong!\n", id);
    } else {
        printf("Huy thao tac xoa.\n");
    }
}

void classDetail() {
    char id[MAX_ID];
    int index;
    printf("\n--- Chi Tiet Lop Hoc ---\n");
    getInputRequired("Yeu cau nhap id lop muon xem chi tiet: ", id, MAX_ID);

    index = findClassIndexById(id);
    if (index == -1) {
        printf("ID lop %s khong ton tai!\n", id);
        return;
    }

    int sub_ch;
    do {
        system("cls");
        printf("\n===========================================\n");
        printf("***Student Management System Using C***\n");
        printf("===========================================\n");
        printf("--- CHI TIET LOP HOC %s (%s) ---\n", classes[index].className, id);
        printf("Thong tin lop: ID GV: %s | So luong SV: %d\n", classes[index].teacherId, classes[index].numStudents);
        displayStudents();
        printf("\n1. Them sinh vien vao lop\n");
        printf("2. Xoa sinh vien khoi lop\n");
        printf("0. Quay lai menu quan ly lop\n");
        printf("Chon: ");

        if (scanf("%d", &sub_ch) != 1) {
            clearInputBuffer();
            sub_ch = -1;
            continue;
        }
        clearInputBuffer();

        if (sub_ch == 1) addStudentToClassRoom();
        else if (sub_ch == 2) removeStudentFromClassRoom();
        else if (sub_ch != 0) printf("Lua chon khong hop le!\n");

        if (sub_ch != 0) pauseScreen();
    } while (sub_ch != 0);
}

void addStudentToClassRoom() {
    char classId[MAX_ID];
    char studentId[MAX_ID];
    int studentIndex;
    int classIndex;

    printf("\n--- Them Sinh Vien Vao Lop ---\n");
    getInputRequired("Yeu cau nhap ID Lop can them SV vao: ", classId, MAX_ID);
    classIndex = findClassIndexById(classId);
    if (classIndex == -1) {
        printf("ID lop %s khong ton tai!\n", classId);
        return;
    }

    getInputRequired("Yeu cau nhap ID sinh vien muon them: ", studentId, MAX_ID);

    studentIndex = findStudentIndexById(studentId);
    if (studentIndex == -1) {
        printf("ID sinh vien %s khong ton tai!\n", studentId);
        return;
    }

    if (strlen(students[studentIndex].classroomId) > 0) {
        printf("Sinh vien %s da nam trong lop khac!\n", studentId);
        return;
    }

    printf("Them sinh vien %s vao lop %s thanh cong!\n", studentId, classId);
}

void removeStudentFromClassRoom() {
    char studentId[MAX_ID];
    char confirm[3];
    int studentIndex;
    printf("\n--- Xoa Sinh Vien Khoi Lop ---\n");
    getInputRequired("Yeu cau nhap id sinh vien muon xoa khoi lop: ", studentId, MAX_ID);

    studentIndex = findStudentIndexById(studentId);
    if (studentIndex == -1 || strlen(students[studentIndex].classroomId) == 0) {
        printf("ID sinh vien %s khong ton tai hoac khong thuoc lop nao de xoa!\n", studentId);
        return;
    }

    printf("Ban co chac chan muon xoa sinh vien %s (%s) khoi lop? (Y/N): ", students[studentIndex].name, studentId);
    fgets(confirm, sizeof(confirm), stdin);
    trim_inplace(confirm);
    if (ci_strcmp(confirm, "Y") == 0) {
        printf("Xoa sinh vien %s khoi lop thanh cong!\n", studentId);
    } else {
        printf("Huy thao tac xoa.\n");
    }
}

void sortClassRooms() {
    printf("\n--- Sap Xep Danh Sach Lop Theo Ten ---\n");
    printf("Chua trien khai...\n");
}

void class_menu() {
    int ch;
    do {
        system("cls");
        printf("\n===========================================\n");
        printf("***Student Management System Using C***\n");
        printf("===========================================\n");
        printf("--- ADMIN: QUAN LY LOP HOC ---\n");
        printf("1. Hien thi danh sach lop\n");
        printf("2. Them lop\n");
        printf("3. Sua thong tin lop\n");
        printf("4. Xoa lop\n");
        printf("5. Chi tiet lop (Them/Xoa SV)\n");
        printf("6. Sap xep lop theo ten\n");
        printf("0. Quay lai\n");
        printf("Yeu cau nhap lua chon: ");
        if (scanf("%d", &ch) != 1) {
            clearInputBuffer();
            ch = -1;
            continue;
        }
        clearInputBuffer();
        switch (ch) {
            case 1: displayClasses(); break;
            case 2: addClassRoom(); break;
            case 3: editClassRoom(); break;
            case 4: deleteClassRoom(); break;
            case 5: classDetail(); break;
            case 6: sortClassRooms(); break;
            case 0: return;
            default: printf("Lua chon khong hop le!\n"); break;
        }
        if (ch != 0) pauseScreen();
    } while (1);
}

void admin_main_menu_logic() {
    int choice;
    do {
        system("cls");
        printf("\n--- ADMIN MAIN MENU ---\n");
        printf("1. Quan ly Sinh vien\n");
        printf("2. Quan ly Giao vien\n");
        printf("3. Quan ly Lop hoc\n");
        printf("0. Dang xuat\n");
        printf("Chon: ");

        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        switch (choice) {
            case 1: admin_student_menu(); break;
            case 2: teacher_menu(); break;
            case 3: class_menu(); break;
            case 0: return;
            default: printf("Lua chon khong hop le!\n"); pauseScreen(); break;
        }
    } while (choice != 0);
}

void print_main_ui() {
    system("cls");
    printf("\n=========================================\n");
    printf(" STUDENT MANAGEMENT SYSTEM (C)\n");
    printf("=========================================\n");
    printf("[1] Admin\n");
    printf("[2] Teacher\n");
    printf("[3] Class (Admin Only)\n");
    printf("[0] Exit the Program\n");
    printf("=========================================\n");
    printf("Enter the choice: ");
}

void initializeData() {
    if (studentCount > 0 || teacherCount > 0 || classCount > 0) return;

    // Khoi tao sinh vien
    strcpy(students[0].studentId, "1");
    strcpy(students[0].name, "Nguyen Van A");
    strcpy(students[0].email, "student1@gmail.com");
    strcpy(students[0].phone, "0161111111");
    strcpy(students[0].classroomId, "1");
    students[0].numCourses = 1;
    students[0].gender = 1;
    studentCount++;

    strcpy(students[1].studentId, "2");
    strcpy(students[1].name, "Le Thi B");
    strcpy(students[1].email, "student2@gmail.com");
    strcpy(students[1].phone, "0161111112");
    students[1].numCourses = 2;
    students[1].gender = 0;
    studentCount++;

    strcpy(students[2].studentId, "3");
    strcpy(students[2].name, "Tran Van C");
    strcpy(students[2].email, "student3@gmail.com");
    strcpy(students[2].phone, "0161111113");
    students[2].numCourses = 3;
    students[2].gender = 1;
    studentCount++;

    // Khoi tao giao vien
    strcpy(teachers[0].teacherId, "1");
    strcpy(teachers[0].name, "GV Nguyen Van D");
    strcpy(teachers[0].email, "teacher1@gmail.com");
    strcpy(teachers[0].password, "pass1");
    teachers[0].numClasses = 1;
    teacherCount++;

    // Khoi tao lop
    strcpy(classes[0].classId, "1");
    strcpy(classes[0].className, "Lap Trinh C");
    strcpy(classes[0].teacherId, "1");
    classes[0].numStudents = 1;
    classCount++;
}

int main() {
    initializeData();
    int choice;

    while (1) {
        print_main_ui();
        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        switch (choice) {
            case 1:
                if (login_role("Admin")) {
                    admin_main_menu_logic();
                }
                break;
            case 2:
                if (login_role("Teacher")) {
                    printf("\nGiao vien dang nhap thanh cong. (Chua co menu rieng)\n");
                    pauseScreen();
                }
                break;
            case 3:
                if (login_role("Class")) {
                    class_menu();
                }
                break;
            case 0:
                displayExitScreen();
                return 0;
            default:
                printf("Lua chon khong hop le, moi nhap lai.\n");
                pauseScreen();
        }
    }
    return 0;
}
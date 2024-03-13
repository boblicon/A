#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib> 
#include <ctime>
#include <fstream>
#include <filesystem>
#include <windows.h>
#pragma execution_character_set("utf-8")
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib> // Для функции rand()
#include <ctime> // Для функции time()

class Student {
public:
    Student(const std::string& name, const std::string& groupCode, int rating)
        : name_(name), groupCode_(groupCode), rating_(rating) {}

    std::string GetName() const { return name_; }
    std::string GetGroupCode() const { return groupCode_; }
    int GetRating() const { return rating_; }

    void PrintInfo() const {
        std::cout << "Имя: " << name_ << ", Шифр группы: " << groupCode_ << ", Рейтинг: " << rating_ << std::endl;
    }

private:
    std::string name_;
    std::string groupCode_;
    int rating_;
};

struct Node {
    int rating;
    int index;
    Node* left;
    Node* right;

    Node(int rating, int index) : rating(rating), index(index), left(nullptr), right(nullptr) {}
};

class BinaryTree {
public:
    BinaryTree() : root(nullptr) {}

    void Insert(int rating, int index) {
        root = Insert(root, rating, index);
    }

    void Delete(int rating) {
        root = Delete(root, rating);
    }

    Node* Search(int rating) const {
        return Search(root, rating);
    }

    void PrintInOrder() const {
        PrintInOrder(root);
    }

private:
    Node* root;

    Node* Insert(Node* node, int rating, int index) {
        if (node == nullptr) {
            return new Node(rating, index);
        }
        if (rating < node->rating) {
            node->left = Insert(node->left, rating, index);
        }
        else if (rating > node->rating) {
            node->right = Insert(node->right, rating, index);
        }
        return node;
    }

    Node* Delete(Node* node, int rating) {
        if (node == nullptr) {
            return node;
        }
        if (rating < node->rating) {
            node->left = Delete(node->left, rating);
        }
        else if (rating > node->rating) {
            node->right = Delete(node->right, rating);
        }
        else {
            if (node->left == nullptr) {
                Node* temp = node->right;
                delete node;
                return temp;
            }
            else if (node->right == nullptr) {
                Node* temp = node->left;
                delete node;
                return temp;
            }
            Node* temp = FindMin(node->right);
            node->rating = temp->rating;
            node->index = temp->index;
            node->right = Delete(node->right, temp->rating);
        }
        return node;
    }

    Node* Search(Node* node, int rating) const {
        if (node == nullptr || node->rating == rating) {
            return node;
        }
        if (rating < node->rating) {
            return Search(node->left, rating);
        }
        return Search(node->right, rating);
    }

    void PrintInOrder(Node* node) const {
        if (node != nullptr) {
            PrintInOrder(node->left);
            std::cout << "Рейтинг: " << node->rating << ", Индекс: " << node->index << std::endl;
            PrintInOrder(node->right);
        }
    }

    Node* FindMin(Node* node) const {
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }
};

class StudentDatabase {
public:
    void AddStudent(const Student& student) {
        students.push_back(student);
        tree.Insert(student.GetRating(), students.size() - 1);
    }

    void PrintStudents() const {
        for (const auto& student : students) {
            student.PrintInfo();
            std::cout << std::endl;
        }
    }

    void PrintStudentsByRating() const {
        tree.PrintInOrder();
    }

    void SearchStudentByRating(int rating) const {
        Node* node = tree.Search(rating);
        if (node) {
            students[node->index].PrintInfo();
        }
        else {
            std::cout << "Студент с рейтингом " << rating << " не найден." << std::endl;
        }
    }

    void EditStudent(int rating, const std::string& newName, const std::string& newGroupCode, int newRating) {
        Node* node = tree.Search(rating);
        if (node) {
            students[node->index] = Student(newName, newGroupCode, newRating);
            tree.Delete(rating);
            tree.Insert(newRating, node->index);
        }
        else {
            std::cout << "Студент с рейтингом " << rating << " не найден." << std::endl;
        }
    }

    void RemoveStudent(int rating) {
        Node* node = tree.Search(rating);
        if (node) {
            students.erase(students.begin() + node->index);
            tree.Delete(rating);
        }
        else {
            std::cout << "Студент с рейтингом " << rating << " не найден." << std::endl;
        }
    }

    std::vector<Student>& GetStudents() {
        return students;
    }

private:
    std::vector<Student> students;
    BinaryTree tree;
};

void BubbleSortByName(std::vector<Student>& students) {
    int n = students.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (students[j].GetName() > students[j + 1].GetName()) {
                std::swap(students[j], students[j + 1]);
            }
        }
    }
}

void BubbleSortByRating(std::vector<Student>& students) {
    int n = students.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (students[j].GetRating() > students[j + 1].GetRating()) {
                std::swap(students[j], students[j + 1]);
            }
        }
    }
}

void InputStudentsManually(StudentDatabase& db, int n) {
    for (int i = 0; i < n; ++i) {
        std::string name;
        std::string groupCode;
        int rating;

        std::cout << "Введите имя студента: ";
        std::cin >> name;
        std::cout << "Введите шифр группы: ";
        std::cin >> groupCode;
        std::cout << "Введите рейтинг студента: ";
        std::cin >> rating;

        db.AddStudent(Student(name, groupCode, rating));
    }
}

void FillArrayRandomly(StudentDatabase& db, int n) {
    srand(static_cast<unsigned>(time(0)));
    for (int i = 0; i < n; ++i) {
        std::string name = "Student" + std::to_string(i + 1);
        std::string groupCode = "Group" + std::to_string(rand() % 100 + 1);
        int rating = rand() % 100 + 1;
        db.AddStudent(Student(name, groupCode, rating));
    }
}

int main() {
    StudentDatabase db;
    int n;
    SetConsoleOutputCP(65001);
    std::cout << "Введите количество студентов: ";
    std::cin >> n;

    char choice;
    std::cout << "Выберите метод заполнения данных (r - случайное, m - ручное): ";
    std::cin >> choice;

    if (choice == 'r' || choice == 'R') {
        FillArrayRandomly(db, n);
    }
    else if (choice == 'm' || choice == 'M') {
        InputStudentsManually(db, n);
    }
    else {
        std::cout << "Неверный выбор. Попробуйте еще раз." << std::endl;
        return 1;
    }

    // Вывод всех студентов
    db.PrintStudents();

    // Сортировка студентов по имени методом пузырька
    BubbleSortByName(db.GetStudents());
    std::cout << "Студенты, отсортированные по имени:" << std::endl;
    db.PrintStudents();

    // Сортировка студентов по рейтингу методом пузырька
    BubbleSortByRating(db.GetStudents());
    std::cout << "Студенты, отсортированные по рейтингу:" << std::endl;
    db.PrintStudents();

    // Вывод студентов, отсортированных по рейтингу
    db.PrintStudentsByRating();

    // Поиск студента по рейтингу
    int searchRating;
    std::cout << "Введите рейтинг студента для поиска: ";
    std::cin >> searchRating;
    db.SearchStudentByRating(searchRating);

    // Редактирование студента
    int editRating;
    std::cout << "Введите рейтинг студента для редактирования: ";
    std::cin >> editRating;
    std::string newName, newGroupCode;
    int newRating;
    std::cout << "Введите новое имя, шифр группы и рейтинг: ";
    std::cin >> newName >> newGroupCode >> newRating;
    db.EditStudent(editRating, newName, newGroupCode, newRating);

    // Удаление студента
    int deleteRating;
    std::cout << "Введите рейтинг студента для удаления: ";
    std::cin >> deleteRating;
    db.RemoveStudent(deleteRating);

    // Повторный вывод всех студентов после изменений
    std::cout << "После изменений:" << std::endl;
    db.PrintStudents();

    return 0;
}

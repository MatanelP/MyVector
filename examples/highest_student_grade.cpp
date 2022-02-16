/**
 * \brief The following program reads from the CLI a list of students. Each student has a first name and grades average.
 * Then, the program outputs the student that has the highest grades average. If there're more than one student, it'll
 * output the first one.
 */
#include <iostream>
#include <sstream>
#include "../solution/vl_vector.h"

/** \brief The vector static Capacity. */
#define STATIC_CAPACITY 16

// region The student class implementation

/**
 * \brief Describes a student.
 */
class Student {
 public:
  /**
   * Default constructor (used with std::cin).
   */
  Student() : name_(""), average_(0.0) { };

  /**
   * Initializes a new student object.
   * @param name The student full name.
   * @param average The student grades average.
   */
  Student(std::string const& name, double average) : name_(name), average_(average) { };

  /**
   * Gets the student name.
   * @return The student name.
   */
  std::string getName() const { return name_; }

  /**
   * Gets the student grades average.
   * @return The student average.
   */
  double getAverage() const { return average_; }

  friend std::istream &operator>>(std::istream &is, Student& s);
  friend std::ostream &operator<<(std::ostream &os, Student const& s);
 private:
  /** \brief The student full name. */
  std::string name_;

  /** \brief The student grades average. */
  double average_;
};

/**
 * Reads a student object from the given stream.
 * @param is The stream object.
 * @param s The output student instance.
 * @return The modified input stream.
 */
std::istream& operator>>(std::istream& is, Student& s) {
  std::string name;
  double average;

  is >> name >> average;

  s = Student(name, average);
  return is;
}

/**
 * Fills the given output stream with the information about the given student.
 * @param os The output stream.
 * @param s The student to write.
 * @return The modified output stream.
 */
std::ostream& operator<<(std::ostream& os, Student const& s) {
  return os << s.getName() << " (average: " << s.getAverage() << ")";
}

// endregion

/**
 * @brief The main function that runs the program.
 * @param argc Non-negative value representing the number of arguments passed
 * to the program from the environment in which the program is run.
 * @param argv Pointer to the first element of an array of pointers to null-terminated
 * multi-byte strings that represent the arguments passed to the program from the execution
 * environment. The value of argv[argc] is guaranteed to be a null pointer.
 * @return 0, to tell the system the execution ended without errors, or 1, to
 * tell the system that the code has execution errors.
 */
int main(int argc, const char* argv[]) {
  /* Setup */
  vl_vector<Student, STATIC_CAPACITY> students;  // The 2'nd generic argument is optional
  std::string line;

  /* Read until the line is Empty */
  while (true) {
    /* Read the entire line */
    std::cout << "Enter a student in the format \"<name> <average>\" or an Empty string to stop:" << std::endl;
    std::getline(std::cin, line);
    if (line.empty()) {
      break;
    }

    /* Convert to std::stringstream (string-stream) so that we can activate operator>>. This will be the same
     * operator>> that we saw in std::cin. As "Student" implements an override of operator>>,
     * it will get called. Yes. It will get called for std::stringstream as it would be called for std::cin. Why? */
    std::stringstream ss(line);
    Student s;
    ss >> s;

    /* Add to the VLVector */
    students.push_back (s);
  }

  /* Find the student with the highest grade */
  Student highest_student_grade;
  for (const auto& student : students) {
    if (student.getAverage() > highest_student_grade.getAverage()) {
      highest_student_grade = student;
    }
  }

  std::cout << "-----------------------------------------------------------------" << std::endl;
  std::cout << "Total Students: " << students.size() << std::endl;
  std::cout << "Student with highest grade: " << highest_student_grade << std::endl;

  return EXIT_SUCCESS;
}

/**
 * \brief The following program reads from the CLI a list of students. Each student has a first name and grades average.
 * Then, the program outputs the student that has the highest grades average. If there're more than one student, it'll
 * output the first one.
 */
#include <iostream>
#include <sstream>
#include "../solution/vl_string.h"

/** \brief The vector static Capacity. */
#define STATIC_CAPACITY 16

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
  vl_string<> str;
  std::string line;

  /* Read until the line is Empty */
  while (true) {
    /* Read the entire line */
    std::cout << "Enter a string to append, or an Empty string to stop:" << std::endl;
    std::getline(std::cin, line);
    if (line.empty()) {
      break;
    }

    /* Add to the VLString */
//    students.PushBack(s);
  }

  std::cout << "-----------------------------------------------------------------" << std::endl;
  std::cout << "String: " << (std::string)str << std::endl;

  return EXIT_SUCCESS;
}

#ifndef JSON_H
#define JSON_H

#include <string>
#include <vector>

struct Task
{
    int id;
    std::string description;
    std::string status;
    std::string createdAt;
    std::string updatedAt;
};

void write_tasks(std::vector<Task> tasks);
std::vector<std::string> read_tasks();
std::vector<Task> fillTasksVector();
std::vector<std::string> format_lines(std::string raw_line);

#endif
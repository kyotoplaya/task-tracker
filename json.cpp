#include "utils.h"
#include "json.h"

#include <iostream>
#include <fstream>
#include <vector>

#define CREATED_AT_OFFSET 13
#define UPDATED_AT_OFFSET 13
#define STATUS_OFFSET 10
#define DESCRIPTION_OFFSET 15

void write_tasks(std::vector<Task> tasks)
{
    std::ofstream out("tasks.json");

    if (out.is_open())
    {
        out << "{" << std::endl;
        out << "    \"tasks\": [" << std::endl;

        for (size_t i = 0; i < tasks.size(); i++)
        {
            out << "        {" << std::endl;
            out << "            \"id\": " << tasks[i].id << "," << std::endl;
            out << "            \"description\": \"" << tasks[i].description << "\"," << std::endl;
            out << "            \"status\": \"" << tasks[i].status << "\"," << std::endl;
            out << "            \"createdAt\": \"" << tasks[i].createdAt << "\"," << std::endl;
            out << "            \"updatedAt\": \"" << tasks[i].updatedAt << "\"" << std::endl;
            out << "        }";

            if (i < tasks.size() - 1)
            {
                out << "," << std::endl;
            }
            else
            {
                out << std::endl;
            }
        }

        out << "    ]" << std::endl;
        out << "}" << std::endl;
    }

    out.close();
}

std::vector<std::string> read_tasks()
{
    std::string raw_line;

    if (fileExists())
    {
        std::ifstream in("tasks.json");

        if (in.is_open())
        {
            std::string temp;
            while (std::getline(in, temp))
            {
                raw_line += temp;
            }
        }

        in.close();

        return format_lines(raw_line);
    }

    std::ofstream out("tasks.json");
    out.close();
    std::vector<std::string> formated_lines;

    return formated_lines;
}

std::vector<Task> fillTasksVector()
{
    std::vector<std::string> all_tasks = read_tasks();

    std::vector<Task> Tasks;

    if (all_tasks.empty())
        return Tasks;

    for (std::string task : all_tasks)
    {
        Task temp_task;

        int sep = 0;

        int id = task.find("id\": ", 0) + 5;

        sep = task.find(",", id);
        temp_task.id = std::stoi(task.substr(id, sep - id));

        int description = task.find("description\": ") + DESCRIPTION_OFFSET;
        sep = task.find(",", description);
        std::string desc_str = task.substr(description, sep - description - 1);
        temp_task.description = desc_str;

        int status = task.find("status\": ") + STATUS_OFFSET;
        sep = task.find(",", status);
        std::string status_str = task.substr(status, sep - status - 1);
        temp_task.status = status_str;

        int createdAt = task.find("createdAt\": ") + CREATED_AT_OFFSET;
        sep = task.find(",", createdAt);
        std::string createdAt_str = task.substr(createdAt, sep - createdAt - 1);
        temp_task.createdAt = createdAt_str;

        int updatedAt = task.find("updatedAt\": ") + UPDATED_AT_OFFSET;
        sep = task.find("\"", updatedAt);
        std::string updatedAt_str = task.substr(updatedAt, sep - updatedAt);
        temp_task.updatedAt = updatedAt_str;

        Tasks.push_back(temp_task);
    }

    return Tasks;
}

std::vector<std::string> format_lines(std::string raw_line)
{
    std::vector<std::string> tasks;

    if (raw_line.empty())
    {
        return tasks;
    }

    int start, end = 0;

    for (size_t i = 0; i < raw_line.length() - 1; i++)
    {
        if (raw_line.at(i) == '{')
            start = i + 8;
        if (raw_line.at(i) == '}')
            end = i;

        if (end != 0)
        {

            tasks.push_back(raw_line.substr(start, end - start));
            start = end;
            end = 0;
        }
    }

    return tasks;
}
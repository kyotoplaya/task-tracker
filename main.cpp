#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>

#define CREATED_AT_OFFSET 13
#define UPDATED_AT_OFFSET 13
#define STATUS_OFFSET 10
#define DESCRIPTION_OFFSET 15

#define RED "\033[31m"
#define YELLOW "\033[33m"
#define GREEN "\033[32m"
#define RESET "\033[0m"

struct Task
{
    int id;
    std::string description;
    std::string status;
    std::string createdAt;
    std::string updatedAt;
};

enum TaskStatus
{
    ALL_TASKS = 0,   // все задачи
    TODO = 1,        // только to-do
    IN_PROGRESS = 2, // только in-progress
    DONE = 3         // только done
};

std::string getDate()
{
    const time_t tm = time(nullptr);
    char buf[64];
    strftime(buf, std::size(buf), "%d.%m.%Y", localtime(&tm));

    return std::string(buf);
}

bool fileExists()
{
    std::ifstream file("tasks.json");
    return file.good();
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

void listTasks(TaskStatus filter = ALL_TASKS) // 0 - все, 1 - todo, 2 - in-progress, 3 - done
{

    std::vector<Task> tasks = fillTasksVector();

    if (tasks.empty())
    {
        std::cout << "No tasks found." << std::endl;
        return;
    }

    std::cout << std::endl;
    std::string color;

    for (Task task : tasks)
    {
        if (filter == ALL_TASKS ||
            (filter == TODO && task.status == "to-do") ||
            (filter == IN_PROGRESS && task.status == "in-progress") ||
            (filter == DONE && task.status == "done"))
        {
            std::cout << "ID: " << task.id << std::endl;
            std::cout << "Description: " << task.description << std::endl;

            if (task.status == "to-do")
                color = RED;
            if (task.status == "in-progress")
                color = YELLOW;
            if (task.status == "done")
                color = GREEN;

            std::cout << "Status: " << color << task.status << RESET << std::endl;
            std::cout << "Created at: " << task.createdAt << std::endl;
            std::cout << "Updated At: " << task.updatedAt << std::endl;
            std::cout << std::endl;
        }
    }
}

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

void add_task(std::string task_description)
{
    std::vector<Task> tasks = fillTasksVector();

    int free_id;

    if (tasks.empty())
    {
        free_id = 0; // первая задача
    }
    else
    {
        free_id = tasks.back().id + 1;
    }

    std::string date = getDate();

    Task task{free_id, task_description, "to-do", date, ""};

    tasks.push_back(task);

    write_tasks(tasks);

    std::cout << "Task was added!" << std::endl;
}

int updateTask(int id, std::string new_description)
{
    std::vector<Task> tasks = fillTasksVector();

    if (id >= tasks.size() or id < 0)
        return 0;

    for (Task &task : tasks)
    {
        if (task.id == id)
        {
            task.description = new_description;
            task.updatedAt = getDate();
            break;
        }
    }

    write_tasks(tasks);

    std::cout << "Task description was updated!" << std::endl;

    return 1;
}

int mark(int id, std::string mark_str)
{
    std::vector<Task> tasks = fillTasksVector();

    if (id >= tasks.size() or id < 0)
        return 0;

    for (Task &task : tasks)
    {
        if (task.id == id)
        {
            task.status = mark_str;
            task.updatedAt = getDate();
            break;
        }
    }

    write_tasks(tasks);

    std::cout << "Task status was updated!" << std::endl;

    return 1;
}

int deleteTask(int id)
{
    std::vector<Task> tasks = fillTasksVector();

    if (id >= tasks.size() or id < 0)
        return 0;

    tasks.erase(tasks.begin() + id);

    for (size_t i = id; i < tasks.size(); i++)
    {
        tasks[i].id = i;
    }

    write_tasks(tasks);

    std::cout << "Task with ID = " << id << " was deleted!";

    return 1;
}

void deleteAllTasks()
{

    std::cout << std::endl;

    char confirm = 'x';

    while (confirm != 'Y' && confirm != 'N')
    {
        std::cout << "Are you SURE you want to " << RED << "delete all tasks? " << RESET << "(Y/N) >> ";
        std::cin >> confirm;
    }
    
    std::cout << std::endl;

    if (confirm == 'Y')
    {
        if (fileExists())
        {
            std::ifstream in("tasks.json");
            in.close();
            std::cout << YELLOW << "All tasks was deleted!" << RESET << std::endl ;
        }
        else
        {
            std::cout << GREEN << "The task list was already empty." << RESET << std::endl;
        }
    }

    else
    {
        std::cout << GREEN << "Clearing the task list has been cancelled." << RESET << std::endl;
    }
        
    std::cout << std::endl;
}

int main(int argc, char *argv[])
{

    if (argc > 1)
    {
        if (std::string(argv[1]) == "add")
        {
            std::string task_description = std::string(argv[2]);
            add_task(task_description);
        }

        else if (std::string(argv[1]) == "list")
        {
            if (argc > 2)
            {
                if (std::string(argv[2]) == "todo")
                    listTasks(TODO);
                else if (std::string(argv[2]) == "in-progress")
                    listTasks(IN_PROGRESS);
                else
                    listTasks(DONE);
            }
            else
                listTasks(ALL_TASKS);
        }

        else if (std::string(argv[1]) == "update")
        {
            int id = std::stoi(std::string(argv[2]));
            std::string new_description = std::string(argv[3]);

            if (!updateTask(id, new_description))
                std::cout << "ID incorrect!";
        }

        else if (std::string(argv[1]) == "mark-in-progress")
        {
            int id = std::stoi(std::string(argv[2]));
            if (!mark(id, "in-progress"))
                std::cout << "ID incorrect!";
        }

        else if (std::string(argv[1]) == "mark-done")
        {
            int id = std::stoi(std::string(argv[2]));
            if (!mark(id, "done"))
                std::cout << "ID incorrect!";
        }

        else if (std::string(argv[1]) == "delete")
        {
            int id = std::stoi(std::string(argv[2]));
            if (!deleteTask(id))
                std::cout << "ID incorrect!";
        }

        else if (std::string(argv[1]) == "deleteAllTasks")
        {
            deleteAllTasks();
        }
    }

    return 0;
}
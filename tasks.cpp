#include "tasks.h"
#include "json.h"
#include "utils.h"
#include "task_status.h"
#include "colors.h"

#include <iostream>
#include <vector>
#include <fstream>

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
            std::cout << YELLOW << "All tasks was deleted!" << RESET << std::endl;
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
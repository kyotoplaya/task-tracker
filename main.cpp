#include "tasks.h"
#include "utils.h"
#include "json.h"
#include "task_status.h"
#include "colors.h"

#include <iostream>

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        printHelp();
        return 0;
    }

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
            int id;

            try
            {
                id = std::stoi(std::string(argv[2]));
                if (!mark(id, "in-progress"))
                    std::cout << "ID incorrect!";
            }
            catch (...)
            {
                std::cout << "Invalid ID format";
            }
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

        else
        {
            printHelp();
        }
    }

    return 0;
}
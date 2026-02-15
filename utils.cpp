#include "utils.h"
#include "colors.h"

#include <iostream>
#include <fstream>
#include <ctime>

void printHelp()
{
    std::cout << std::endl;
    std::cout << GREEN << "╔════════════════════════════════════════════════════════╗" << RESET << std::endl;
    std::cout << GREEN << "║              TASK TRACKER - COMMANDS                   ║" << RESET << std::endl;
    std::cout << GREEN << "╚════════════════════════════════════════════════════════╝" << RESET << std::endl;
    std::cout << std::endl;

    std::cout << YELLOW << "ADD TASK:" << RESET << std::endl;
    std::cout << "  ./main.exe add \"task description\"" << std::endl;
    std::cout << "  " << GREEN << "Example:" << RESET << " ./main.exe add \"Buy groceries\"" << std::endl;
    std::cout << std::endl;

    std::cout << YELLOW << "LIST TASKS:" << RESET << std::endl;
    std::cout << "  ./main.exe list              " << GREEN << "- show all tasks" << RESET << std::endl;
    std::cout << "  ./main.exe list todo         " << GREEN << "- show only todo tasks" << RESET << std::endl;
    std::cout << "  ./main.exe list in-progress  " << GREEN << "- show only in-progress tasks" << RESET << std::endl;
    std::cout << "  ./main.exe list done         " << GREEN << "- show only completed tasks" << RESET << std::endl;
    std::cout << std::endl;

    std::cout << YELLOW << "UPDATE TASK:" << RESET << std::endl;
    std::cout << "  ./main.exe update <id> \"new description\"" << std::endl;
    std::cout << "  " << GREEN << "Example:" << RESET << " ./main.exe update 0 \"Buy milk and bread\"" << std::endl;
    std::cout << std::endl;

    std::cout << YELLOW << "CHANGE STATUS:" << RESET << std::endl;
    std::cout << "  ./main.exe mark-in-progress <id>   " << GREEN << "- mark task as in progress" << RESET << std::endl;
    std::cout << "  ./main.exe mark-done <id>          " << GREEN << "- mark task as completed" << RESET << std::endl;
    std::cout << "  " << GREEN << "Example:" << RESET << " ./main.exe mark-done 2" << std::endl;
    std::cout << std::endl;

    std::cout << YELLOW << "DELETE TASKS:" << RESET << std::endl;
    std::cout << "  ./main.exe delete <id>            " << GREEN << "- delete specific task" << RESET << std::endl;
    std::cout << "  ./main.exe deleteAllTasks         " << GREEN << "- " << RED << "DELETE ALL TASKS" << GREEN << " (with confirmation)" << RESET << std::endl;
    std::cout << std::endl;

    std::cout << YELLOW << "OTHER:" << RESET << std::endl;
    std::cout << "  ./main.exe help                " << GREEN << "- show this help message" << RESET << std::endl;
    std::cout << std::endl;

    std::cout << GREEN << "══════════════════════════════════════════════════════════" << RESET << std::endl;
    std::cout << "Remember to put descriptions with spaces in " << YELLOW << "quotes" << RESET << "!" << std::endl;
    std::cout << std::endl;
}

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
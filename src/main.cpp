#include "task_tracker.hpp"

#include <charconv>
#include <iostream>
#include <string>

namespace {
void usage() {
    std::cout << "Masseuse Task Tracker\n\n"
              << "Usage:\n"
              << "  masseuse_tracker add <client> <service> <YYYY-MM-DD> <priority 1-5> [notes]\n"
              << "  masseuse_tracker list [all]\n"
              << "  masseuse_tracker complete <id>\n"
              << "  masseuse_tracker remove <id>\n"
              << "\nData is saved in masseuse_tasks.db in the current directory.\n";
}

bool parseInt(const std::string& value, int& result) {
    const auto [ptr, ec] = std::from_chars(value.data(), value.data() + value.size(), result);
    return ec == std::errc{} && ptr == value.data() + value.size();
}
}

int main(int argc, char* argv[]) {
    TaskTracker tracker("masseuse_tasks.db");
    if (!tracker.load()) {
        std::cerr << "Could not read the task database.\n";
        return 1;
    }
    if (argc < 2) {
        usage();
        return 0;
    }

    const std::string command = argv[1];
    if (command == "add") {
        if (argc < 6) {
            std::cerr << "Missing fields for add.\n";
            usage();
            return 2;
        }
        int priority{};
        if (!parseInt(argv[5], priority) || priority < 1 || priority > 5) {
            std::cerr << "Priority must be a number from 1 (highest) to 5 (lowest).\n";
            return 2;
        }
        const auto& task = tracker.add(argv[2], argv[3], argv[4], priority, argc >= 7 ? argv[6] : "");
        if (!tracker.save()) {
            std::cerr << "Could not save the task.\n";
            return 1;
        }
        std::cout << "Added task #" << task.id << ".\n";
    } else if (command == "list") {
        if (argc >= 3 && std::string(argv[2]) == "all") printTasks(tracker.tasks(), std::cout);
        else printTasks(tracker.pendingByPriority(), std::cout);
    } else if (command == "complete" || command == "remove") {
        int id{};
        if (argc != 3 || !parseInt(argv[2], id)) {
            std::cerr << "A numeric task ID is required.\n";
            return 2;
        }
        const bool changed = command == "complete" ? tracker.complete(id) : tracker.remove(id);
        if (!changed) {
            std::cerr << "Task #" << id << " was not found.\n";
            return 3;
        }
        if (!tracker.save()) {
            std::cerr << "Could not save changes.\n";
            return 1;
        }
        std::cout << (command == "complete" ? "Completed" : "Removed") << " task #" << id << ".\n";
    } else {
        std::cerr << "Unknown command: " << command << '\n';
        usage();
        return 2;
    }
}

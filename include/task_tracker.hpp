#pragma once

#include <iosfwd>
#include <string>
#include <vector>

enum class TaskStatus { Pending, Completed };

struct Task {
    int id{};
    std::string client;
    std::string service;
    std::string dueDate;
    int priority{3};
    TaskStatus status{TaskStatus::Pending};
    std::string notes;
};

class TaskTracker {
public:
    explicit TaskTracker(std::string storagePath);

    bool load();
    bool save() const;
    const Task& add(std::string client, std::string service, std::string dueDate,
                    int priority, std::string notes);
    bool complete(int id);
    bool remove(int id);
    const std::vector<Task>& tasks() const;
    std::vector<Task> pendingByPriority() const;

private:
    std::string storagePath_;
    std::vector<Task> tasks_;
    int nextId_{1};
};

std::string statusText(TaskStatus status);
void printTasks(const std::vector<Task>& tasks, std::ostream& out);

#include "task_tracker.hpp"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <utility>

namespace {
std::string clean(std::string value) {
    std::replace(value.begin(), value.end(), '\t', ' ');
    std::replace(value.begin(), value.end(), '\n', ' ');
    std::replace(value.begin(), value.end(), '\r', ' ');
    return value;
}
}

TaskTracker::TaskTracker(std::string storagePath) : storagePath_(std::move(storagePath)) {}

bool TaskTracker::load() {
    std::ifstream input(storagePath_);
    if (!input) return true;

    tasks_.clear();
    nextId_ = 1;
    std::string line;
    while (std::getline(input, line)) {
        std::istringstream row(line);
        Task task;
        std::string status;
        if (!(row >> task.id >> std::quoted(task.client) >> std::quoted(task.service)
                  >> std::quoted(task.dueDate) >> task.priority >> status
                  >> std::quoted(task.notes))) {
            return false;
        }
        task.status = status == "completed" ? TaskStatus::Completed : TaskStatus::Pending;
        nextId_ = std::max(nextId_, task.id + 1);
        tasks_.push_back(std::move(task));
    }
    return true;
}

bool TaskTracker::save() const {
    std::ofstream output(storagePath_, std::ios::trunc);
    if (!output) return false;
    for (const auto& task : tasks_) {
        output << task.id << '\t' << std::quoted(clean(task.client)) << '\t'
               << std::quoted(clean(task.service)) << '\t' << std::quoted(clean(task.dueDate))
               << '\t' << task.priority << '\t' << statusText(task.status) << '\t'
               << std::quoted(clean(task.notes)) << '\n';
    }
    return static_cast<bool>(output);
}

const Task& TaskTracker::add(std::string client, std::string service, std::string dueDate,
                             int priority, std::string notes) {
    priority = std::clamp(priority, 1, 5);
    tasks_.push_back({nextId_++, clean(std::move(client)), clean(std::move(service)),
                      clean(std::move(dueDate)), priority, TaskStatus::Pending,
                      clean(std::move(notes))});
    return tasks_.back();
}

bool TaskTracker::complete(int id) {
    auto it = std::find_if(tasks_.begin(), tasks_.end(), [id](const Task& task) { return task.id == id; });
    if (it == tasks_.end()) return false;
    it->status = TaskStatus::Completed;
    return true;
}

bool TaskTracker::remove(int id) {
    const auto oldSize = tasks_.size();
    std::erase_if(tasks_, [id](const Task& task) { return task.id == id; });
    return tasks_.size() != oldSize;
}

const std::vector<Task>& TaskTracker::tasks() const { return tasks_; }

std::vector<Task> TaskTracker::pendingByPriority() const {
    std::vector<Task> result;
    std::copy_if(tasks_.begin(), tasks_.end(), std::back_inserter(result),
                 [](const Task& task) { return task.status == TaskStatus::Pending; });
    std::stable_sort(result.begin(), result.end(), [](const Task& a, const Task& b) {
        if (a.priority != b.priority) return a.priority < b.priority;
        return a.dueDate < b.dueDate;
    });
    return result;
}

std::string statusText(TaskStatus status) {
    return status == TaskStatus::Completed ? "completed" : "pending";
}

void printTasks(const std::vector<Task>& tasks, std::ostream& out) {
    if (tasks.empty()) {
        out << "No tasks found.\n";
        return;
    }
    out << "ID | Priority | Status    | Due        | Client | Service | Notes\n";
    out << "---+----------+-----------+------------+--------+---------+------\n";
    for (const auto& task : tasks) {
        out << task.id << "  | " << task.priority << "        | "
            << std::left << std::setw(9) << statusText(task.status) << std::right << " | "
            << std::setw(10) << task.dueDate << " | " << task.client << " | "
            << task.service << " | " << task.notes << '\n';
    }
}

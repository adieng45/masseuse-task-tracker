#include "task_tracker.hpp"

#include <cassert>
#include <cstdio>
#include <string>

int main() {
    const std::string path = "tracker_test.db";
    std::remove(path.c_str());
    {
        TaskTracker tracker(path);
        assert(tracker.load());
        const int first = tracker.add("Jordan", "Deep tissue", "2026-09-13", 2, "Shoulders").id;
        const int second = tracker.add("Taylor", "Sports massage", "2026-09-12", 1, "Leg recovery").id;
        assert(second == first + 1);
        assert(tracker.pendingByPriority().front().id == second);
        assert(tracker.complete(first));
        assert(!tracker.complete(999));
        assert(tracker.save());
    }
    {
        TaskTracker tracker(path);
        assert(tracker.load());
        assert(tracker.tasks().size() == 2);
        assert(tracker.tasks().front().status == TaskStatus::Completed);
        assert(tracker.remove(2));
        assert(tracker.tasks().size() == 1);
    }
    std::remove(path.c_str());
}

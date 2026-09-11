# Masseuse Task Tracker

A small C++20 command-line tracker for massage appointments and follow-up work. Tasks persist locally and include a client, service, due date, priority, status, and notes.

## Build and test

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

## Use

```bash
./build/masseuse_tracker add "Jordan Lee" "Deep tissue" 2026-09-13 1 "Focus on shoulders"
./build/masseuse_tracker list
./build/masseuse_tracker complete 1
./build/masseuse_tracker list all
./build/masseuse_tracker remove 1
```

Priority `1` is highest and `5` is lowest. `list` shows pending tasks in priority/due-date order; `list all` includes completed tasks. Data is stored in `masseuse_tasks.db` in the working directory.

## License

MIT

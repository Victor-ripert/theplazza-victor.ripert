# G-CCP-400-BAR-4-1-theplazza-2

## Testing

There is a stress test to validate the thread-safety of the Mutex, Thread, and Logger implementations.
The test program spawns multiple threads concurrently writing to the Logger to ensure no data
corruption or crashes occur under high contention.

### Running the test
```bash
g++ -o infra_test Tests/infra_test.cpp src/utils/Logger.cpp src/utils/Mutex.cpp src/utils/Thread.cpp -I./include -lpthread && ./infra_test  
```

### Interpreting the tests
```bash
wc -l test_log.txt && grep "Thread" test_log.txt | sort | uniq | wc -l 
```
### What it should mean

Test Results:
   * Threads: 50
   * Messages per thread: 100
   * Total log entries: 5,000 (verified by `wc -l`)
   * Integrity: 5,000 unique messages found (verified by `sort | uniq`), meaning zero data loss or "interleaved" characters from race conditions.
   * Stability: Zero crashes or segmentation faults.
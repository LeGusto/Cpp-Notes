# False Sharing Explained

## What is False Sharing?

**False sharing** occurs when multiple threads write to **different variables** that happen to be located on the **same cache line**.

## Why is it a Problem?

### Cache Line Coherency

Modern CPUs use cache coherency protocols (like MESI) to keep caches consistent:

1. **Cache line size**: Typically 64 bytes on modern x86 CPUs
2. **Coherency granularity**: The protocol works on entire cache lines, not individual bytes
3. **Cache line states**:
   - **Modified (M)**: Only this core has it, it's been modified
   - **Exclusive (E)**: Only this core has it, clean
   - **Shared (S)**: Multiple cores have it, clean
   - **Invalid (I)**: Not in cache or stale

### The Problem

When Thread 1 writes to variable A and Thread 2 writes to variable B, but both A and B are on the same cache line:

1. Thread 1's core marks the cache line as **Modified**
2. Thread 2's core needs to write to variable B on the same cache line
3. Thread 2's cache line is **Invalidated**
4. Thread 2 must fetch the cache line from Thread 1's core (or main memory)
5. Thread 1's cache line is invalidated
6. This causes excessive **cache line bouncing** between cores

### Why "False" Sharing?

It's called "false" sharing because:
- The threads are accessing **different variables** (they don't actually share data)
- But the cache coherency protocol treats them as if they're sharing (same cache line)
- This creates unnecessary overhead

## Reading vs Writing

- **Multiple threads READING from the same cache line**: ✅ Fine! Cache line can be in "Shared" state
- **Multiple threads WRITING to different variables on same cache line**: ❌ Bad! Causes cache line bouncing

## Example Scenario

```
Cache Line (64 bytes):
[Thread 1 writes → counter1 (4 bytes)] [Thread 2 writes → counter2 (4 bytes)] [unused...]
```

Even though Thread 1 only touches `counter1` and Thread 2 only touches `counter2`, they're on the same cache line, so:
- Writing to `counter1` invalidates the cache line in Thread 2's core
- Writing to `counter2` invalidates the cache line in Thread 1's core
- This creates a ping-pong effect

## Solution

Separate the variables so each is on its own cache line:
- Use `alignas(64)` to align each variable to a cache line boundary
- Or use padding to ensure at least 64 bytes between variables




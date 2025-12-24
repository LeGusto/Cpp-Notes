## Type Declarations (and optimization):
- `long`, `int`, `short` - Machine-specific sizes
- Safer: `std::int64_t`, `std::int32_t`, `std::int16_t` (from `<cstdint>`)
- Fast types: `std::int_fast64_t`, `std::int_fast32_t`, `std::int_fast16_t`
  - At least as big as requested, may be larger for performance

### Note
- `char` may be signed or unsigned (implementation-defined)
- `std::int8_t` is a typedef for `signed char` 

## reduce() vs accumulate():
- **reduce()** supports execution policies (C++17)
- **accumulate()** is sequential by default
- **reduce()** is usually much faster if used with a parallel execution policy
- **reduce()** performs worse with compiler optimizations (O2+) than **accumulate()**


## Volatile
- Prevents compiler optimizations (variable may change unexpectedly)


## Vtables
- Virtual methods create a vtable and vptr (vtable pointer)
- vptr size: 8 bytes on 64-bit, 4 bytes on 32-bit
- Vtable stores function pointers, declared as void*

## "Breaking" Private Members 
- Can access private members via pointer casting to matching struct
- Relies on memory layout assumptions, might not work on different machines

## Friend Functions/Classes
- Friend functions can access private members
- Friend classes can also access private members
- Must be declared in the class

## Padding
- Padding aligns struct members to their alignment requirements
- Alignment is usually to word boundaries (4/8 bytes), not cache lines
- Members aligned to largest member's alignment requirement
- `#pragma pack(1)` removes padding, but may be slower due to needing multiple cache line segments for one variable

## Random
- **rand()** - Old C function, low period, modulo bias issues
- **std::random_device** - Hardware-based, secure but slow
- **std::mt19937** - Mersenne Twister, period 2^19937 - 1
- **std::uniform_int_distribution** - Wrapper that eliminates modulo bias

## Vector
- Contains: pointer (8 bytes), size (8 bytes), capacity (8 bytes) = 24 bytes (64-bit)
- Implementation-dependent
- Doubles in size each time capacity is about to be exceeded

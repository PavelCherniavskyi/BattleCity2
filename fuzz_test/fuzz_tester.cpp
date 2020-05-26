#include <utility>
#include <iostream>

[[nodiscard]] uint64_t sum_values(const uint8_t *Data, size_t Size)
{
   constexpr int16_t scale = 1000;

   uint64_t value = 0;
   for (std::size_t offset = 0; offset < Size; ++offset) {
      value += static_cast<uint64_t>(*std::next(Data, static_cast<long>(offset))) * scale;
   }
   return value;
}

// Fuzzer that attempts to invoke undefined behavior for signed integer overflow
// cppcheck-suppress unusedFunction symbolName=LLVMFuzzerTestOneInput
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size)
{
  std::cout << "Value sum: " << sum_values(Data,Size) << ", len: " << Size << '\n';
  //fmt::print("Value sum: {}, len{}\n", sum_values(Data,Size), Size);
  return 0;
}
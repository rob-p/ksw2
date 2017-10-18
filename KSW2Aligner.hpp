#ifndef __KSW2_ALIGNER_HPP__
#define __KSW2_ALIGNER_HPP__

#include <memory>
#include <vector>

#include "kalloc.h"
#include "ksw2.h"

namespace ksw2pp {

/**
 * When we use a unique_ptr to hold a kalloc allocator, this is the
 * deleter we use to call the appropriate function to free / destroy the
 *allocator.
 **/
class KallocDeleter {
public:
  void operator()(void* p) { km_destroy(p); }
};

enum class KSW2AlignmentType : uint8_t { GLOBAL = 1, EXTENSION = 2 };

// Just like Int2Type from https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Int-To-Type
template <KSW2AlignmentType I> struct EnumToType {
  enum { value = static_cast<uint8_t>(I) };
};

// A structure to hold the relvant parameters for the aligner
struct KSW2Config {
  int8_t gapo = -1;
  int8_t gape = -1;
  int bandwidth = -1;
  int dropoff = -1;
  int flag = 0;
  KSW2AlignmentType atype = KSW2AlignmentType::GLOBAL;
};

class KSW2Aligner {

public:
  KSW2Aligner(int match = 2, int mismatch = -4);

  int operator()(const char* const queryOriginal, const int queryLength,
                 const char* const targetOriginal, const int targetLength,
                 ksw_extz_t* ez, EnumToType<KSW2AlignmentType::GLOBAL>);

  int operator()(const char* const queryOriginal, const int queryLength,
                 const char* const targetOriginal, const int targetLength,
                 ksw_extz_t* ez, EnumToType<KSW2AlignmentType::EXTENSION>);

  int operator()(const uint8_t* const queryOriginal, const int queryLength,
                 const uint8_t* const targetOriginal, const int targetLength,
                 ksw_extz_t* ez, EnumToType<KSW2AlignmentType::GLOBAL>);

  int operator()(const uint8_t* const queryOriginal, const int queryLength,
                 const uint8_t* const targetOriginal, const int targetLength,
                 ksw_extz_t* ez, EnumToType<KSW2AlignmentType::EXTENSION>);

  KSW2Config& config() { return config_; }
  const ksw_extz_t& result() { return result_; }
  void freeCIGAR(ksw_extz_t* ez) {
    if (ez->cigar and kalloc_allocator_) {
      kfree(kalloc_allocator_.get(), ez->cigar);
    }
  }
  // const ksw_extz_t& result() { return result_; }

private:
  std::vector<uint8_t> query_;
  std::vector<uint8_t> target_;
  ksw_extz_t result_;
  std::unique_ptr<void, KallocDeleter> kalloc_allocator_{nullptr,
                                                         KallocDeleter()};
  std::vector<int8_t> mat_;
  KSW2Config config_;
};
} // namespace ksw2pp
#endif //__KSW2_ALIGNER_HPP__

#ifndef __KSW2_ALIGNER_HPP__
#define __KSW2_ALIGNER_HPP__

#include <memory>
#include <vector>

#include "ksw2.h"
#include "kalloc.h"

class KallocDeleter {
public:
  void operator()(void* p) {
    km_destroy(p);
  }
};

enum class KSW2AlignmentType : uint8_t { GLOBAL = 1, EXTENSION = 2 };

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
    KSW2Aligner();
    int operator()(const char* const queryOriginal, const int queryLength,
                    const char* const targetOriginal, const int targetLength,
                    /*const KSW2Config& config,*/ksw_extz_t* ez);
  int operator()(const uint8_t* const queryOriginal, const int queryLength,
                  const uint8_t* const targetOriginal, const int targetLength,
                  /*const KSW2Config& config,*/ksw_extz_t* ez);
  KSW2Config& config() { return config_; }
  const ksw_extz_t& result() { return result_; }
  void freeCIGAR(ksw_extz_t* ez) {
    if (ez->cigar and kalloc_allocator_) {
      kfree(kalloc_allocator_.get(), ez->cigar);
    }
  }
  //const ksw_extz_t& result() { return result_; }

  private:
    std::vector<uint8_t> query_;
    std::vector<uint8_t> target_;
    ksw_extz_t result_;
  std::unique_ptr<void, KallocDeleter> kalloc_allocator_{nullptr, KallocDeleter()};
  std::vector<int8_t> mat_;
  KSW2Config config_;
  //std::vector<Block> blocks_;
  //PeqTable peq_;
};

#endif //__KSW2_ALIGNER_HPP__

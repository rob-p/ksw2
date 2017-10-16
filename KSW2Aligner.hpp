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

struct KSW2Config {
  
};

class KSW2Aligner {

  public:
    KSW2Aligner();
    void operator()(const char* const queryOriginal, const int queryLength,
                    const char* const targetOriginal, const int targetLength,
                    /*const KSW2Config& config,*/ksw_extz_t* ez);
  void operator()(const uint8_t* const queryOriginal, const int queryLength,
                  const uint8_t* const targetOriginal, const int targetLength,
                  /*const KSW2Config& config,*/ksw_extz_t* ez);

  const ksw_extz_t& result() { return result_; }
  //const ksw_extz_t& result() { return result_; }
  private:
    std::vector<uint8_t> query_;
    std::vector<uint8_t> target_;
    ksw_extz_t result_;
  std::unique_ptr<void, KallocDeleter> kalloc_allocator_{nullptr, KallocDeleter()};
  std::vector<int8_t> mat_;
  //std::vector<Block> blocks_;
  //PeqTable peq_;
};

#endif //__KSW2_ALIGNER_HPP__

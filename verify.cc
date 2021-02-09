#include <climits>
#include <exception>
#include <fstream>
#include <string>

#include "sample_generated.h"

namespace fb = org::apache::arrow::flatbuf;

int main(int argc, char** argv) {
  const size_t kDataLen = 1536;
  std::string data(kDataLen, '\0');

  std::ifstream fin("footer.bin", std::ios::binary);
  fin.read(&data[0], kDataLen);
  if (fin.gcount() != kDataLen) {
    throw std::runtime_error("read from file failed");
  }

  flatbuffers::Verifier verifier(reinterpret_cast<const uint8_t*>(data.data()),
                                 data.size(),
                                 /*max_depth=*/128,
                                 /*max_tables=*/100000000);
  if (!fb::VerifyFooterBuffer(verifier)) {
    throw std::runtime_error("Footer verification failed");
  }

  return 0;
}

#include "coding_schemes/bi.hpp"

void bi::encoder(std::istream &in, std::ostream &out) {

  // mask
  uint32_t mask = (1 << bitwidth) - 1;

  // value buffer
  uint32_t val;

  // queue (depth = 1)
  std::queue<uint32_t> fifo;

  // fill buffer, and send first value
  in >> val;
  fifo.push(val);
  out << val << std::endl;

  // iterate over the rest of the stream
  while (in >> val) {
    // get delayed value
    uint32_t val_delay = fifo.front();
    fifo.pop();
    bool invert = false;
    // check hamming distance
    if (hamming_distance(val, val_delay) > bitwidth / 2) {
      invert = true;
      val = ~val & mask;
    }
    // add side channel signal
    if (invert) {
      val |= 1 << bitwidth;
    }
    fifo.push(val);
    out << val << std::endl;
  }
}

bi::bi(unsigned int bitwidth) : coding_scheme(bitwidth){}

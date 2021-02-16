#include "coding_schemes/def.hpp"

namespace pommel {

int def::int_to_smint(int in) {

  // mask
  uint32_t val_mask = (1 << platform.bitwidth) - 1;
  uint32_t sign_mask = (1 << (platform.bitwidth - 1));

  uint32_t sign = ((in & sign_mask) >> (platform.bitwidth - 1)) & 1;
  uint32_t val = ((in & val_mask));

  if (sign && (val != 0)) {
    val = (~val & val_mask) + 1;
  }

  return (val & val_mask) | (sign << (platform.bitwidth - 1));
}

void def::diff_encoder(std::istream &in, std::ostream &out) {

  // value buffer
  uint32_t val;

  // queue (depth = channels)
  std::queue<uint32_t> fifo;

  // fill buffer, and send first value
  for (int i = 0; i < channels; i++) {
    in >> val;
    fifo.push(val);
    out << val << std::endl;
  }

  // iterate over the rest of the stream
  while (in >> val) {
    // get delayed value
    uint32_t val_delay = fifo.front();
    fifo.pop();
    // fill buffer
    fifo.push(val);
    // write difference to output
    out << int_to_smint(val - val_delay) << std::endl;
  }
}

void def::encoder(std::istream &in, std::ostream &out) {
    // run encoder
    std::stringstream buf;
    diff_encoder(in, buf);
    decorrelator(buf, out);
}

def::def(platform_config_t platform, unsigned int channels) : coding_scheme(platform), channels(channels){}

}

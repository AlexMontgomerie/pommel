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
    // break if empty early
    if( !in.rdbuf()->in_avail() ) {
      break;
    }
    // fill input buffer
    in >> val;
    fifo.push(val);
    out << int_to_smint(val) << std::endl;
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

  // send rest of the buffer

}

void def::encoder(std::istream &data_in, std::ostream &data_out, std::istream &addr_in, std::ostream &addr_out) {

    // send addresses in to addresses out
    addr_out << addr_in.rdbuf();

    // setup string streams
    std::stringstream diff_in;
    std::stringstream diff_interleave;
    std::stringstream def_out;

    // de-interleave stream in
    deinterleave(data_in, diff_in, platform.bitwidth, platform.packing_factor); 

    // diff encoder 
    diff_encoder(diff_in, diff_interleave);

    // interleave stream again
    interleave(diff_interleave, def_out, platform.bitwidth, platform.packing_factor); 

    // decorrelator
    decorrelator(def_out, data_out);
 
}

def::def(platform_config_t platform, unsigned int channels) : coding_scheme(platform), channels(channels){}

}

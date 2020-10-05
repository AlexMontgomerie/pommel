#include "common.hpp"

int main(int argc, char *argv[]) {

    /*
  // parameters
  int opt;
  string config_path;

  while ((opt = getopt(argc, argv, "c:")) != -1) {
    switch (opt) {
    case 'c':
      config_path = optarg;
      break;
    }
  }

  // load config
  ifstream config_file(config_path);
  json config;
  config_file >> config;

  int bitwidth = (int)config["bitwidth"];
  int channels = (int)config["channels"];

  // run encoder
  stringstream buf;
  encoder(cin, buf, bitwidth, channels);
  decorrelator(buf, cout, bitwidth);
    */

    printf("hello world\n");
    return 0;
}

#!/bin/bash

# generate all the networks
#python -m scripts.generate_network_config -n config/network/alexnet.xml -a config/accelerator/eyeriss.xml    -f featuremaps/alexnet_8b.h5 -o alexnet_eyeriss
#python -m scripts.generate_network_config -n config/network/alexnet.xml -a config/accelerator/tpu.xml        -f featuremaps/alexnet_8b.h5 -o alexnet_tpu
#python -m scripts.generate_network_config -n config/network/alexnet.xml -a config/accelerator/shidiannao.xml -f featuremaps/alexnet_8b.h5 -o alexnet_shidiannao
#python -m scripts.generate_network_config -n config/network/alexnet.xml -a config/accelerator/scnn.xml       -f featuremaps/alexnet_8b.h5 -o alexnet_scnn

#python -m scripts.generate_network_config -n config/network/vgg11.xml -a config/accelerator/eyeriss.xml    -f featuremaps/vgg11_8b.h5 -o vgg11_eyeriss
#python -m scripts.generate_network_config -n config/network/vgg11.xml -a config/accelerator/tpu.xml        -f featuremaps/vgg11_8b.h5 -o vgg11_tpu
#python -m scripts.generate_network_config -n config/network/vgg11.xml -a config/accelerator/shidiannao.xml -f featuremaps/vgg11_8b.h5 -o vgg11_shidiannao
#python -m scripts.generate_network_config -n config/network/vgg11.xml -a config/accelerator/scnn.xml       -f featuremaps/vgg11_8b.h5 -o vgg11_scnn

python -m scripts.generate_network_config -n config/network/mobilenet_v2.xml -a config/accelerator/eyeriss.xml    -f featuremaps/mobilenet_v2_8b.h5 -o mobilenet_v2_eyeriss
python -m scripts.generate_network_config -n config/network/mobilenet_v2.xml -a config/accelerator/tpu.xml        -f featuremaps/mobilenet_v2_8b.h5 -o mobilenet_v2_tpu
python -m scripts.generate_network_config -n config/network/mobilenet_v2.xml -a config/accelerator/shidiannao.xml -f featuremaps/mobilenet_v2_8b.h5 -o mobilenet_v2_shidiannao
python -m scripts.generate_network_config -n config/network/mobilenet_v2.xml -a config/accelerator/scnn.xml       -f featuremaps/mobilenet_v2_8b.h5 -o mobilenet_v2_scnn

#python -m scripts.generate_network_config -n config/network/resnet18.xml -a config/accelerator/eyeriss.xml    -f featuremaps/resnet18_8b.h5 -o resnet18_eyeriss
#python -m scripts.generate_network_config -n config/network/resnet18.xml -a config/accelerator/tpu.xml        -f featuremaps/resnet18_8b.h5 -o resnet18_tpu
#python -m scripts.generate_network_config -n config/network/resnet18.xml -a config/accelerator/shidiannao.xml -f featuremaps/resnet18_8b.h5 -o resnet18_shidiannao
#python -m scripts.generate_network_config -n config/network/resnet18.xml -a config/accelerator/scnn.xml       -f featuremaps/resnet18_8b.h5 -o resnet18_scnn



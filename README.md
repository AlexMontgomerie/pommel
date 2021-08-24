# POMMEL: Exploring Off-Chip Memory Energy & Power Consumption in Convolutional Neural Network Accelerators

The purpose of the POMMEL tool is to enable the rapid analysis of the power consumption of the memory subsystem within modern Convolutional Neural Network (CNN) accelerators. This in turn enables research into methods of reducing power consumption through architecture design, as well as on-line compression methods.

## Installation

The first step is to download the repository. You can do this by cloning the repo:

```
git clone https://github.com/AlexMontgomerie/pommel
```

This tool requires the following packages to be installed on your system:

 - `libboost-system-dev`
 - `libboost-filesystem-dev`
 - `libboost-program-options-dev`

To build the tool, first use cmake:

 ```
 mkdir build && cd build
 cmake ..
 ```

Then create the executable:

 ```
 make -j 8
 make install
 ```

## Usage

To use the tool, the following arguments are required:

```
./build/POMMEL -h

    Allowed Options:
      -h [ --help ]         help message
      --baseline            compute baseline power readings (no encoding)
      --memory arg          file path for memory config (.xml)
      --encoder arg         file path for encoding scheme config (.xml)
      --controller arg      controller type
      --network arg         file path for network config (.xml)
      --accelerator arg     accelerator config path (.xml)
      --featuremap arg      featuremap data path (.h5)
      --output arg          output directory path
```


An example of running the tool is as follows. To run the tool, you must download the example featuremap from [here](https://imperialcollegelondon.box.com/shared/static/r8za7qx35yw0x2wib7dbd1deuyqb34yg.h5). To generate featuremap data, please look at [distiller-featuremap](https://github.com/AlexMontgomerie/distiller-featuremap).

```

./build/POMMEL \
    --baseline \
    --memory example/memory.xml \
    --featuremap example/featuremap.h5 \
    --network example/network.xml \
    --accelerator example/accelerator.xml \
    --output outputs \
    --controller standard
```

The tool generates traces for each partition as well as the `report.json` file, which contains a layer-wise breakdown of power consumption. This includes other energy, power and performance metrics.

## Citation

If you use POMMEL in your work, please cite the following:

 ```BibTex
 @inproceedings{montgomerie-corcoran_def_2021,
	title = {POMMEL: Exploring Off-Chip Memory Energy & Power Consumption in Convolutional Neural Network Accelerators},
	booktitle = {24th Euromicro Conference on Digital System Design, DSD 2021},
	publisher = {IEEE},
	author = {Montgomerie-Corcoran, A. and Bouganis, C.},
	year = {2021},
}
 ```
___

Feel free to post an issue if you have any questions or problems!

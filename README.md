# gr-rpi_iot

This is the repository storing all the OOT modules designed for RPI ECSE 4964/6964 Physical Layer lab. It is forked from https://github.com/qizhu8/gr-rpi_iot; however, the build and make scripts have been modified to compile with python3 rather than puthon 2, and to work with newer Gnuradio versions like 3.10.1

## What is in this repository?

### BPSK and QPSK decoding block
Two heuristic decoders implemented in OOT with C++. Use them as a ground truth block to check the correctness of your block.
Note that, since we don't implement grey's code, this block works different from the gr-digit::constellation block.

### BER block
BER block takes in two byte streams and compute the ratio of bit error rate. This block only exams the most recent window\_size symbols.

## How to use?

``` bash
git clone https://github.com/ChibuikemEzemaduka/gr-rpi_iot.git
cd gr-rpi_iot
mkdir build
cd build
cmake .. -DENABLE_TESTING=OFF -DPYTHON_EXECUTABLE=/usr/bin/python3 -DPYTHON_INCLUDE_DIR=/usr/include/python3.10 -DPYTHON_LIBRARY=/usr/lib/x86_64-linux-gnu/libpython3.10.so
make
sudo make install
sudo ldconfig
sudo mkdir /usr/local/lib/python3/dist-packages/gnuradio
sudo cp -a /usr/local/lib/python3/dist-packages/rpi_iot /usr/local/lib/python3/dist-packages/gnuradio
```

Open gnuradio-companion or click the "refresh" button on the top-right corner. If everthing is correct, you should find the following blocks in section rpi_iot

* BER_bbf
* BPSK_Decoder_cb
* QPSK_Decoder_cb
* SER

## Troubleshooting
### Why I cannot find the block?
1. Have you linked the library? I.e. ```sudo ldconfig```
2. Has the block installation directory in gnuradio-companion's search list? You are supposed to check the return value when executing ```sudo make install```. The path to pay attention to is the destination that XML file named "rpi\_iot\_SER.xml" is installed. Also, the search directories for gnuradio-companion are in ~/.gnuradio/config.conf under "[grc]" variables "global\_blocks\_path" and "local\_blocks\_path".
The installation path should be either the "global\_blocks\_path" or "local\_blocks\_path"

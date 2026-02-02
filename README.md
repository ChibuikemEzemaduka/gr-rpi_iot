# gr-rpi_iot

 
## This branch has been modified from the master branch to compile for Ubuntu24.04+Gnuradio 3.10.9.2

Please read the "How to use" section on the master branch to get better details of this repo.


## How to use?

``` bash
sudo apt install cmake
sudo snap install cmake --classic
sudo apt-get install -y doxygen
sudo apt-get install -y python3-setuptools
git clone https://github.com/ChibuikemEzemaduka/gr-rpi_iot.git
cd gr-rpi_iot
mkdir build
cd build
cmake .. -DENABLE_TESTING=OFF -DCMAKE_INSTALL_PREFIX=/usr
make
sudo make install
sudo ldconfig
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

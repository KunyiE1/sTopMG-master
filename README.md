# sTopMG
sTopMG is implemented by changing original topmg which is included in TopPIC. 
For further information and detailed manual, please visit https://www.toppic.org/software/toppic/

## System requirements

* GCC version higher than 5.5.0 for C++14 support
* CMake (>= 3.1)

### Linux (Ubuntu 22.04):

```sh
# install compiling tools
sudo apt install build-essential cmake

# install other dependencies
sudo apt install zlib1g-dev 
sudo apt install libboost-filesystem-dev 
sudo apt install libboost-program-options-dev 
sudo apt install libboost-system-dev 
sudo apt install libboost-thread-dev 
sudo apt install libboost-iostreams-dev 
sudo apt install libboost-chrono-dev 

sudo apt install libxerces-c-dev  
sudo apt install libeigen3-dev 
sudo apt install nlohmann-json3-dev


# install the catch unit test framework (https://github.com/philsquared/Catch)
sudo apt install catch

# Qt5 for GUI
sudo apt install qtbase5-dev

# building
mkdir build
cd build
cmake ..
make -j$(nproc)
make install
```

### Linux (CentOS Stream 8):

```sh
# install Extra Packages for Enterprise Linux (EPEL)
sudo dnf install 'dnf-command(config-manager)'
sudo dnf config-manager --set-enabled powertools
sudo dnf install epel-release 

# install compiling tools
sudo dnf install gcc gcc-c++ make cmake

# install dependencies
sudo dnf install zlib-devel
sudo dnf install boost-devel 
sudo dnf install xerces-c-devel
sudo dnf install eigen3-devel
sudo dnf install json-devel

# Qt5 for GUI
sudo dnf install qt5-qtbase-devel

# building
mkdir build
cd build
cmake ..
make -j$(nproc)
make install
```

## Simple Manual for Getting Started with sTopMG
### Run sTopMG
After TopPIC is successfully built(or you can only build topmg by run topmg in IDE like Clion)
You can simply run sTopMG with command line:
topmg -i MODFICATION_FILE SEQUENCE_DATABASE_FILE SPECTRUM_FILE >RESULTS_FILE

take searching 100 simulated spectra data as an example, run the command line:
topmg -i database_and_modsfile/variable_mods_EC.txt database_and_modsfile/EC_canonical.fasta MSDataset/100SimulatedDataset/100_sim_ms2.msalign > MSDataset/100SimulatedDataset/results

according to the building, the command line could also be:
bin/topmg -i database_and_modsfile/variable_mods_EC.txt database_and_modsfile/EC_canonical.fasta MSDataset/100SimulatedDataset/100_sim_ms2.msalign > MSDataset/100SimulatedDataset/results

### Change Settings for sTopMG




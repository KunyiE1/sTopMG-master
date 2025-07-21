# sTopMG
sTopMG is implemented by changing original topmg which is included in TopPIC. 
For further information and detailed manual, please visit https://www.toppic.org/software/toppic/

## System Requirements

* GCC version higher than 5.5.0 for C++14 support
* CMake (>= 3.1)

## Memory Requirements
* The expirements in the paper are conducted in Linux Ubuntu 22.04 with an usage of memory less than 4GB.
* For users who need to consider more occurances of modifications or isotopes, we believe that a memory of 8GB is capable.
* To modify or degub the programme with IDE like Clion, we recommand a memory of at least 16GB. 

## Installation on Linux (Ubuntu 22.04):

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

### Installation on Linux (CentOS Stream 8):

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

The installation time is about 1 minutes. 

## Manual for Getting Started with sTopMG
### Run sTopMG on a toy dataset
After TopPIC is successfully built(or you can only build topmg by run topmg in IDE like Clion)
You can simply run sTopMG on a toy(demo) dataset with command line:
```sh
topmg -i database_and_modsfile/variable_mods_EC.txt database_and_modsfile/Test.fasta MSDataset/Test/test_ms2.msalign > MSDataset/Test/results
```
according to the building, the command line could also be:
```sh
bin/topmg -i database_and_modsfile/variable_mods_EC.txt database_and_modsfile/Test.fasta MSDataset/Test/test_ms2.msalign > MSDataset/Test/results
```
The toy dataset contains 2 spectra and the toy database contains 60 protein sequences. The expected running time is about 3 seconds. 

There will be two ouput files: \
LCSA_scoreboard: contains spectrum id, identified protein name, number of matches in the result alignment, \
LCSA_results: contains specific alignments construct by sTopMG
LCSA_results: contains specific alignments constructed by sTopMG

### Run sTopMG on Simulated Dataset
You can simply run sTopMG with command line:
```sh
topmg -i MODFICATION_FILE SEQUENCE_DATABASE_FILE SPECTRUM_FILE >RESULTS_FILE
```
take searching 100 simulated spectra data as an example, run the command line:
```sh
topmg -i database_and_modsfile/variable_mods_EC.txt database_and_modsfile/EC_canonical.fasta MSDataset/100SimulatedDataset/100_sim_ms2.msalign > MSDataset/100SimulatedDataset/results
```
according to the building, the command line could also be:
```sh
bin/topmg -i database_and_modsfile/variable_mods_EC.txt database_and_modsfile/EC_canonical.fasta MSDataset/100SimulatedDataset/100_sim_ms2.msalign > MSDataset/100SimulatedDataset/results
```
### FDR control for sTopMG
1. In **src/concole/topmg_argument.cpp**, line 51: set **arguments["searchType"] = "TARGET+DECOY"**;
2. In **src/concole/topmg_argument.cpp**, line 81: set  **arguments["objFDR"]** to be the objective FDR.
You can also use command line to with **-F objective FDR** for FDR control, but make sure the **searchType** is set to **"TARGET+DECOY"**. The identified PSM will be shown in file "LCSA_scoreboard_FDR"\
Example for setting objective FDR to be 0.01:
```sh
bin/topmg -F 0.01 -i database_and_modsfile/variable_mods_EC.txt database_and_modsfile/EC_canonical.fasta MSDataset/100SimulatedDataset/100_sim_ms2.msalign > MSDataset/100SimulatedDataset/results
```

### Change Settings for sTopMG
The defaul settings of sTopMG is using fixed error tolerance = 0.1 Dalton and used for proteoform segment searching.\
There are various searching schemes which can be decided by users.

Options are provided to change the settings when using command line to run sTopMG
* **-e mass-error-tolerance** (an integer) \
Set the error tolerance for precursor and fragment masses in ppm. Default value: 10 ppm.\
**-e 0** means use the fixed error tolerance = 0.1 Dalton for each peak and the precursor mass

* **-S consider disulfide bond or not** (true or false)\
Decide if the disulfide bond is considered during the identification.\
**-S false** means we don't consider disulfide bond during the searching. 

* **-W only consider whole protein sequence or not** (true or false)\
**-W false** means we consider the truncation of the protein sequence when searching the query spectrum.

* **-I the maximum number of isotope considered during the search** (an integer)\
**-I 1** means the number of isotope mass shift we consider during the search is one.

* **-N the number of reported alignments with top scores for one spectrum** (an integer > 0)\
**-N 5** means for each spectrum, we report top 5 alignments with largest scores.

Example of setting parameters using command line:
```sh
bin/topmg -F 0.01 -e 0 -S false -W false - I 1 -N 5 -i database_and_modsfile/variable_mods_EC.txt database_and_modsfile/EC_canonical.fasta MSDataset/100SimulatedDataset/100_sim_ms2.msalign > MSDataset/100SimulatedDataset/results
```

User can also change the parameter settings by modify the code.\
In **src/filter/mng/LCS_filter_mng.hpp**, line 137\
**bool whole_protein_only = false**: means we consider the truncation of the protein sequence when searching the query spectrum.

In **src/filter/mng/LCS_filter_mng.hpp**, line 145,146,147\
To set the number of isotope mass shift we consider during the search.

In **src/console/topmg_process.cpp**, line 522, 523:\
**bool disulfide_bond = false**: means we don't consider disulfide bond during the searching. Change it to true when searching antibody query spectra.\
**bool isotope_shift = true** : means we consider itsotope mass shift during the searching.

In **src/concole/topmg_argument.cpp**, line 54,79:\
**arguments["massErrorTolerance"] = "0"**: means use the fixed error tolerance = 0.1 Dalton for each peak and the precursor mass, when the parameter is set to be larger than 0, we use peak-dependent tolerance.\
**arguments["useLCSFiltering"] = "false"**: means we use mass-filter + sTopMG. Change it to "true" if you need to use Filtering 2 described in paper.







# MPI + HDF5 Parallel Hello World

This project demonstrates parallel writing to an HDF5 file using **MPI** and the **HDF5 C++ API**, and includes a verification step to validate the written data.

## Overview

- `main.cpp`: Uses MPI to run multiple processes, each writing its rank to a shared HDF5 dataset (`rank_data`) in parallel using **collective I/O**.
- `verify.cpp`: Reads the resulting HDF5 file and verifies that each rank wrote the correct value.
- `CMakeLists.txt`: CMake configuration to build both programs and run tests.

## Requirements

- CMake ≥ 3.20
- MPI (e.g., OpenMPI or MPICH)
- HDF5 with C++ support

## Building

```bash
mkdir build
cd build
cmake ..
make
```

This builds the following executables:

`hello_mpi_hdf5`: Writes an HDF5 file in parallel using MPI

`verify_hdf5`: Verifies the contents of the HDF5 file

## Running
You can run the parallel writer manually with a specified number of MPI processes:
```bash
mpirun -np 4 ./hello_mpi_hdf5
```
This will generate a file named parallel_hello.h5, with each process writing its rank to the dataset.

Then run the verifier:
```bash
./verify_hdf5
```
It will read the file and confirm that the data is correct.

### Optional: Custom File Name
You may provide a custom filename as the first argument:
```bash
Copy
Edit
mpirun -np 4 ./hello_mpi_hdf5 my_output_file.h5
```

## Testing
You can run the built-in CMake tests using:
```bash
ctest
```
This performs:

* A 4-process MPI run that writes the file.
* A verification step to ensure the output is correct.

## Installation
To install the executables:
```bash
make install
```
They will be placed in your system's binary directory (e.g., `/usr/local/bin` or `${CMAKE_INSTALL_PREFIX}/bin`).

## License

This project is licensed under the GNU General Public License v3.0 (GPL-3.0).

See the [LICENSE](./LICENSE) file for full license text.


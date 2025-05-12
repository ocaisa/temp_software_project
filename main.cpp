#include <mpi.h>
#include <H5Cpp.h>
#include <H5public.h>
#include <H5Ppublic.h>
#include <iostream>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    MPI_Comm comm = MPI_COMM_WORLD;
    MPI_Info info = MPI_INFO_NULL;
    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    // Use filename from argv[1] or default
    std::string filename = "parallel_hello.h5";
    if (argc > 1) {
        filename = argv[1];
    }

    const std::string dataset_name = "rank_data";
    const hsize_t total_size = size;

    try {
        // Set up MPI-IO property list using C API
        hid_t plist_id = H5Pcreate(H5P_FILE_ACCESS);
        H5Pset_fapl_mpio(plist_id, comm, info);

        // Use the C++ API to create file with this FAPL
        H5::H5File file(filename, H5F_ACC_TRUNC, H5::FileCreatPropList::DEFAULT, H5::FileAccPropList(plist_id));
        H5Pclose(plist_id); // Clean up

        // Create dataset
        hsize_t dims[1] = {total_size};
        H5::DataSpace dataspace(1, dims);
        H5::DataSet dataset = file.createDataSet(dataset_name, H5::PredType::NATIVE_INT, dataspace);

        // Each rank writes one value
        int value = rank;
        hsize_t start[1] = {static_cast<hsize_t>(rank)};
        hsize_t count[1] = {1};
        H5::DataSpace memspace(1, count);
        dataspace.selectHyperslab(H5S_SELECT_SET, count, start);

        // Use collective I/O
        hid_t xfer_plist_id = H5Pcreate(H5P_DATASET_XFER);
        H5Pset_dxpl_mpio(xfer_plist_id, H5FD_MPIO_COLLECTIVE);
        dataset.write(&value, H5::PredType::NATIVE_INT, memspace, dataspace,
                      H5::DSetMemXferPropList(xfer_plist_id));
        H5Pclose(xfer_plist_id);

        std::cout << "Rank " << rank << " wrote value " << value << " to file '" << filename << "'\n";
    } catch (H5::Exception& e) {
        std::cerr << "HDF5 Error on rank " << rank << ": " << e.getDetailMsg() << std::endl;
    }

    MPI_Finalize();
    return 0;
}

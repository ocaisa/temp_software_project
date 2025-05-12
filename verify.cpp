#include <H5Cpp.h>
#include <iostream>
#include <vector>
#include <cstdlib>

int main() {
    const std::string filename = "parallel_hello.h5";
    const std::string dataset_name = "rank_data";

    try {
        H5::H5File file(filename, H5F_ACC_RDONLY);
        H5::DataSet dataset = file.openDataSet(dataset_name);
        H5::DataSpace dataspace = dataset.getSpace();

        hsize_t dims[1];
        dataspace.getSimpleExtentDims(dims);
        std::vector<int> data(dims[0]);

        dataset.read(data.data(), H5::PredType::NATIVE_INT);

        std::cout << "Verifying dataset: ";
        for (hsize_t i = 0; i < dims[0]; ++i) {
            std::cout << data[i] << " ";
            if (data[i] != static_cast<int>(i)) {
                std::cerr << "\nVerification failed at index " << i
                          << ": expected " << i << ", got " << data[i] << std::endl;
                return EXIT_FAILURE;
            }
        }
        std::cout << "\nVerification successful." << std::endl;

    } catch (H5::Exception& e) {
        std::cerr << "HDF5 Error: " << e.getDetailMsg() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

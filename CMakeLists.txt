cmake_minimum_required(VERSION 3.20)
project(MPI_HDF5_Hello CXX)

# Required packages
find_package(MPI REQUIRED)
find_package(HDF5 REQUIRED COMPONENTS CXX)

enable_testing()

# Executable: parallel HDF5 writer
add_executable(hello_mpi_hdf5 main.cpp)
target_include_directories(hello_mpi_hdf5 PRIVATE ${MPI_CXX_INCLUDE_PATH} ${HDF5_INCLUDE_DIRS})
target_link_libraries(hello_mpi_hdf5 PRIVATE ${MPI_CXX_LIBRARIES} ${HDF5_CXX_LIBRARIES})

# Executable: verification step
add_executable(verify_hdf5 verify.cpp)
target_include_directories(verify_hdf5 PRIVATE ${HDF5_INCLUDE_DIRS})
target_link_libraries(verify_hdf5 PRIVATE ${HDF5_CXX_LIBRARIES})

# Test: Run MPI program (creates HDF5 file)
add_test(NAME RunMPIProgram
         COMMAND ${MPIEXEC_EXECUTABLE} ${MPIEXEC_NUMPROC_FLAG} 4 $<TARGET_FILE:hello_mpi_hdf5>
         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# Test: Verify output file after it is created
add_test(NAME VerifyHDF5Output
         COMMAND $<TARGET_FILE:verify_hdf5>
         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# Ensure verification runs only after MPI program
set_tests_properties(VerifyHDF5Output PROPERTIES DEPENDS RunMPIProgram)

# -------------------------------
# Installation Instructions
# -------------------------------
include(GNUInstallDirs)

# Install binaries
install(TARGETS hello_mpi_hdf5 verify_hdf5
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

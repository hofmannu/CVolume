set(BUILD_SHARED_LIBS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# for reading from and writing to h5 files
# Prefer the Conan-provided HDF5 C++ target (hdf5::hdf5_cpp, carries the
# include/hdf5 path for H5Cpp.h); fall back to a system-installed libhdf5_cpp.
find_package(HDF5 QUIET)
if (TARGET hdf5::hdf5_cpp)
	set(HDF5_CXX_LIBS hdf5::hdf5_cpp)
else()
	find_library(H5CPP_LIB hdf5_cpp REQUIRED)
	find_library(H5_LIB hdf5 REQUIRED)
	set(HDF5_CXX_LIBS "${H5CPP_LIB}" "${H5_LIB}")
endif()

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# for reading from and writing to nii files

add_library(Volume volume.cpp)
target_link_libraries(Volume PUBLIC
	BaseClass
	BasicMathOp
	VtkWriter
	GriddedData
	Threads::Threads
	${HDF5_CXX_LIBS}
)

add_library(BaseClass baseClass.cpp)
add_library(BasicMathOp basicMathOp.cpp)


add_library(GriddedData griddedData.cpp)
add_library(VtkWriter vtkwriter.cpp)
target_link_libraries(VtkWriter PUBLIC
	GriddedData
	)
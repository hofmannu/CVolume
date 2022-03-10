set(BUILD_SHARED_LIBS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# for reading from and writing to h5 files
find_library(H5CPP_LIB hdf5_cpp)
find_library(H5_LIB hdf5)

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
	"${H5CPP_LIB}" "${H5_LIB}"
)

add_library(BaseClass baseClass.cpp)
add_library(BasicMathOp basicMathOp.cpp)


add_library(GriddedData griddedData.cpp)
add_library(VtkWriter vtkwriter.cpp)
target_link_libraries(VtkWriter PUBLIC
	GriddedData
	)
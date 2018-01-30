cmake_minimum_required (VERSION 3.10)
set(name "socket")
project ( ${name} )

#cuda
find_package(CUDA)
set(CUDA_NVCC_FLAGS
    ${CUDA_NVCC_FLAGS}
#    -O3 
	-gencode=arch=compute_61,code=\"sm_61,compute_61\"
    )

cuda_add_executable(${name}
    main.cpp
    Socket.h
    Socket.cpp
)
# Define the target system
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Specify the cross-compilers
set(CMAKE_C_COMPILER /Applications/ARM/bin/arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER /Applications/ARM/bin/arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER /Applications/ARM/bin/arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER_WORKS TRUE)

# Specify the GDB debugger
set(CMAKE_GDB /Applications/ARM/bin/arm-none-eabi-gdb)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Prevent CMake from searching for host system tools
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)


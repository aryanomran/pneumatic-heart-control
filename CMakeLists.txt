cmake_minimum_required(VERSION 3.28)

# Project setup
project(firmware C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# MCU and compiler flags
set(MCU_FLAGS "-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard")
set(COMMON_FLAGS "-O0 -g3 -Wall -fdata-sections -ffunction-sections --specs=nosys.specs --specs=nano.specs")

set(CMAKE_C_FLAGS "${MCU_FLAGS} ${COMMON_FLAGS}")
set(CMAKE_CXX_FLAGS "${MCU_FLAGS} ${COMMON_FLAGS} -fno-exceptions -fno-rtti")
set(CMAKE_ASM_FLAGS "${MCU_FLAGS} -g")

# Linker script and flags
set(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/STM32G474RETX_FLASH.ld")
# Ensure the linker script exists
if(NOT EXISTS ${LINKER_SCRIPT})
    message(FATAL_ERROR "Linker script not found at: ${LINKER_SCRIPT}")
endif()


set(CMAKE_EXE_LINKER_FLAGS
        "-T\"${LINKER_SCRIPT}\" -Wl,--gc-sections -static ${MCU_FLAGS} -fno-exceptions -fno-rtti -lstdc++ -lsupc++"
)

# Source files
# Find all source files
file(GLOB_RECURSE C_SOURCES
        "Core/Src/*.c"
        "Drivers/STM32G4xx_HAL_Driver/Src/*.c"
        "Middlewares/Third_Party/FreeRTOS/Source/*.c"
        "Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/*.c"
        "Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/*.c"
)
file(GLOB_RECURSE CPP_SOURCES "Core/Src/main.cpp")
file(GLOB_RECURSE ASM_SOURCES "startup_stm32g474xx.s")


list(FILTER C_SOURCES EXCLUDE REGEX ".*main\\.c$")

add_executable(${PROJECT_NAME}
        ${ASM_SOURCES}
        ${CPP_SOURCES}
        ${C_SOURCES}
        Hardware/Inc/Interfaces/IAnalogSensor.h
        Hardware/Inc/Interfaces/IPressureControl.h
        Hardware/Inc/Interfaces/ISolenoidValve.h
        Hardware/Inc/Interfaces/IPressureSensor.h
        Hardware/Inc/PressureRegulatorDriver.h
        Hardware/Inc/Peripherals.h
        Hardware/Inc/Interfaces/IAnalogActuator.h
        Hardware/Inc/Interfaces/IDigitalActuator.h
)

# Include directories

target_include_directories(${PROJECT_NAME} PRIVATE
        Core/Inc
        Drivers/CMSIS/Device/ST/STM32G4xx/Include
        Drivers/CMSIS/Include
        Drivers/STM32G4xx_HAL_Driver/Inc
        Drivers/STM32G4xx_HAL_Driver/Inc/Legacy
        "Middlewares/Third_Party/FreeRTOS/Source/include"
        "Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2"
        "Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F"
)

# Preprocessor definitions
target_compile_definitions(${PROJECT_NAME} PRIVATE
        USE_HAL_DRIVER
        STM32G474xx
)

# Generate HEX file
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${PROJECT_NAME}> ${PROJECT_NAME}.hex
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Generating ${PROJECT_NAME}.hex"
)

# Output naming
set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_NAME ${PROJECT_NAME}.elf
        LINKER_LANGUAGE CXX
)


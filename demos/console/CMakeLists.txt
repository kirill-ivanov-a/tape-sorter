add_executable(console_demo main.cpp)

find_package(Boost REQUIRED COMPONENTS program_options)

target_link_libraries(
        console_demo
        PUBLIC
        ${LIBRARY_NAME}
        Boost::program_options
)

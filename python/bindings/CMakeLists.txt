# Build pybind11 python module for GNU Radio 3.10+

find_package(pybind11 REQUIRED)

include_directories(
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_BINARY_DIR}/include
  ${GNURADIO_ALL_INCLUDE_DIRS}
)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/docstrings)

set(PYDOC_TEMPLATES
  BER_bbf
  BPSK_Decoder_cb
  QPSK_Decoder_cb
  SER
)



foreach(name IN LISTS PYDOC_TEMPLATES)
  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/docstrings/${name}_pydoc.h
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_SOURCE_DIR}/docstrings/${name}_pydoc_template.h
            ${CMAKE_CURRENT_BINARY_DIR}/docstrings/${name}_pydoc.h
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/docstrings/${name}_pydoc_template.h
    VERBATIM
  )
endforeach()

add_custom_target(rpi_iot_pydoc_headers ALL
  DEPENDS
    ${CMAKE_CURRENT_BINARY_DIR}/docstrings/BER_bbf_pydoc.h
    ${CMAKE_CURRENT_BINARY_DIR}/docstrings/BPSK_Decoder_cb_pydoc.h
    ${CMAKE_CURRENT_BINARY_DIR}/docstrings/QPSK_Decoder_cb_pydoc.h
    ${CMAKE_CURRENT_BINARY_DIR}/docstrings/SER_pydoc.h
)


# This will produce a module named "rpi_iot_python" (a .so)
pybind11_add_module(rpi_iot_python MODULE
  BER_bbf_python.cc
  BPSK_Decoder_cb_python.cc
  QPSK_Decoder_cb_python.cc
  SER_python.cc
  rpi_iot_python.cc
)

set_target_properties(rpi_iot_python PROPERTIES PREFIX "")

# Link against your C++ block library and GNU Radio runtime
target_link_libraries(rpi_iot_python PRIVATE
  gnuradio-rpi_iot
  ${GR_RUNTIME_LIBRARIES}
)

if(GR_PMT_FOUND)
  target_link_libraries(rpi_iot_python PRIVATE ${GR_PMT_LIBRARIES})
else()
  # If PMT doesn't come via pkg-config, linking the C++ lib already pulls it in,
  # but keeping this is harmless if it exists.
  target_link_libraries(rpi_iot_python PRIVATE gnuradio-pmt)
endif()

# Ensure the module can find symbols at runtime
set_target_properties(rpi_iot_python PROPERTIES
  OUTPUT_NAME "rpi_iot_python"
)


target_include_directories(rpi_iot_python PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/docstrings
  ${CMAKE_CURRENT_BINARY_DIR}/docstrings
)

add_dependencies(rpi_iot_python rpi_iot_pydoc_headers)

# Install into the python package directory
install(TARGETS rpi_iot_python
  LIBRARY DESTINATION ${GR_PYTHON_DIR}/gnuradio/rpi_iot
)

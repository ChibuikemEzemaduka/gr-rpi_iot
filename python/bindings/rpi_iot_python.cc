#include <pybind11/pybind11.h>

namespace py = pybind11;

// Forward-declare the per-block binders (implemented in *_python.cc files)
void bind_SER(py::module& m);
void bind_BER_bbf(py::module& m);
void bind_QPSK_Decoder_cb(py::module& m);
void bind_BPSK_Decoder_cb(py::module& m);

PYBIND11_MODULE(rpi_iot_python, m)
{
    // Optional: module docstring
    m.doc() = "GNU Radio rpi_iot Python bindings";

    // Register blocks
    bind_SER(m);
    bind_BER_bbf(m);
    bind_QPSK_Decoder_cb(m);
    bind_BPSK_Decoder_cb(m);
}


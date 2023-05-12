#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include "AesCmac.h"

namespace py = pybind11;

py::array_t<unsigned char> aes128_cmac(py::array_t<unsigned char> key, py::array_t<unsigned char> msg) {
    py::buffer_info kinfo = key.request();
    auto K = static_cast<unsigned char*>(kinfo.ptr);
    AesCmac aes(K);

    py::buffer_info minfo = msg.request();
    auto M = static_cast<unsigned char*>(minfo.ptr);
    aes.append(M, minfo.shape[0]);

    size_t sz = sizeof(unsigned char);
    auto fmt = py::format_descriptor<unsigned char>::value;
    auto result = py::array(py::buffer_info(nullptr, sz, fmt, 1, {16}, {sz}));
    
    py::buffer_info cminfo = result.request();
    auto CMAC = static_cast<unsigned char*>(cminfo.ptr);
    aes.finalize(CMAC);
    
    return result;
}

PYBIND11_MODULE(pyaescmac, m) {
    m.def("aes128_cmac", &aes128_cmac);
}

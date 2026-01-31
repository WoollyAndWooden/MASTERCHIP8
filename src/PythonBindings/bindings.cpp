#include <pybind11/pybind11.h>
#include "SuperChip8.h"

namespace py = pybind11;

PYBIND11_MODULE(masterchip8_py, m)
{
    py::class_<SuperChip8::SuperChip8>(m, "SuperChip8")
            .def(py::init<std::string>())
            .def("tick", &SuperChip8::SuperChip8::Tick)
            .def("change_cycles_per_tick", &SuperChip8::SuperChip8::change_cycles_per_tick)
            .def("press_key", [](SuperChip8::SuperChip8& self, uint8_t key) {
                self.keyboard.press_key(key);
            })
            .def("release_key", [](SuperChip8::SuperChip8& self, uint8_t key) {
                self.keyboard.release_key(key);
            })
            .def("get_pixel", [](SuperChip8::SuperChip8& self, int x, int y)
            {
                return self.display.getPixel(x, y);
            })
            .def("get_register_V", [](SuperChip8::SuperChip8& self, int x) {
            return self.registers.get_V(x);
            });
}

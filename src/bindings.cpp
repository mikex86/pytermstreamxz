#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include <pybind11/buffer_info.h>
#include <termstreamxz/termstream.h>
#include <termstreamxz/bytestream.h>

namespace py = pybind11;

class TerminalFrameWrapper {
private:
    TerminalFrame frame;

public:
    TerminalFrameWrapper() {
        frame.width = 0;
        frame.height = 0;
        frame.cells = nullptr;
    }
    
    TerminalFrameWrapper(int width, int height) {
        frame.width = width;
        frame.height = height;
        if (width > 0 && height > 0) {
            frame.cells = new Cell[width * height]();
        } else {
            frame.cells = nullptr;
        }
    }

    TerminalFrameWrapper(const TerminalFrameWrapper& other) {
        frame.width = other.frame.width;
        frame.height = other.frame.height;
        if (other.frame.cells != nullptr && frame.width > 0 && frame.height > 0) {
            frame.cells = new Cell[frame.width * frame.height];
            for (int i = 0; i < frame.width * frame.height; ++i) {
                frame.cells[i] = other.frame.cells[i];
            }
        } else {
            frame.cells = nullptr;
        }
    }

    TerminalFrameWrapper& operator=(const TerminalFrameWrapper& other) {
        if (this != &other) {
            if (frame.cells != nullptr) {
                delete[] frame.cells;
            }
            
            frame.width = other.frame.width;
            frame.height = other.frame.height;
            if (other.frame.cells != nullptr && frame.width > 0 && frame.height > 0) {
                frame.cells = new Cell[frame.width * frame.height];
                for (int i = 0; i < frame.width * frame.height; ++i) {
                    frame.cells[i] = other.frame.cells[i];
                }
            } else {
                frame.cells = nullptr;
            }
        }
        return *this;
    }

    ~TerminalFrameWrapper() {
        if (frame.cells != nullptr) {
            delete[] frame.cells;
            frame.cells = nullptr;
        }
    }

    int getWidth() const { return frame.width; }
    int getHeight() const { return frame.height; }
    void setWidth(int width) { frame.width = width; }
    void setHeight(int height) { frame.height = height; }

    Cell getCell(int x, int y) const {
        if (frame.cells != nullptr && x >= 0 && y >= 0 && x < frame.width && y < frame.height) {
            return frame.cells[y * frame.width + x];
        }
        return Cell{};
    }
    
    void setCell(int x, int y, const Cell& cell) {
        if (frame.cells != nullptr && x >= 0 && y >= 0 && x < frame.width && y < frame.height) {
            frame.cells[y * frame.width + x] = cell;
        }
    }

    std::vector<Cell> getCells() const {
        std::vector<Cell> cells_vector;
        if (frame.cells != nullptr && frame.width > 0 && frame.height > 0) {
            cells_vector.reserve(frame.width * frame.height);
            for (int i = 0; i < frame.width * frame.height; ++i) {
                cells_vector.push_back(frame.cells[i]);
            }
        }
        return cells_vector;
    }

    void setCells(const std::vector<Cell>& cells) {
        if (frame.cells != nullptr) {
            delete[] frame.cells;
            frame.cells = nullptr;
        }
        if (!cells.empty()) {
            frame.cells = new Cell[cells.size()];
            for (size_t i = 0; i < cells.size(); ++i) {
                frame.cells[i] = cells[i];
            }
        }
    }

    const TerminalFrame& getFrame() const { return frame; }
    TerminalFrame& getFrame() { return frame; }
};

PYBIND11_MODULE(pytermstreamxz, m) {
    m.doc() = "Python bindings for termstreamxz - Terminal and byte stream processing library";

    py::class_<Cell>(m, "Cell")
        .def(py::init<>())
        .def_readwrite("codepoint", &Cell::codepoint)
        .def_readwrite("bold", &Cell::bold)
        .def_readwrite("italic", &Cell::italic)
        .def_readwrite("inverse", &Cell::inverse)
        .def_readwrite("standout", &Cell::standout)
        .def_readwrite("strikethrough", &Cell::strikethrough)
        .def_readwrite("altfont", &Cell::altfont)
        .def_readwrite("underline", &Cell::underline)
        .def_readwrite("undercurl", &Cell::undercurl)
        .def_readwrite("underdouble", &Cell::underdouble)
        .def_readwrite("underdotted", &Cell::underdotted)
        .def_readwrite("underdashed", &Cell::underdashed)
        .def_readwrite("fg_r", &Cell::fg_r)
        .def_readwrite("fg_g", &Cell::fg_g)
        .def_readwrite("fg_b", &Cell::fg_b)
        .def_readwrite("bg_r", &Cell::bg_r)
        .def_readwrite("bg_g", &Cell::bg_g)
        .def_readwrite("bg_b", &Cell::bg_b)
        .def_readwrite("ul_r", &Cell::ul_r)
        .def_readwrite("ul_g", &Cell::ul_g)
        .def_readwrite("ul_b", &Cell::ul_b);

    // TerminalFrame structure with proper memory management using wrapper
    py::class_<TerminalFrameWrapper>(m, "TerminalFrame")
        .def(py::init<>())
        .def(py::init<int, int>())
        .def_property("width", &TerminalFrameWrapper::getWidth, &TerminalFrameWrapper::setWidth)
        .def_property("height", &TerminalFrameWrapper::getHeight, &TerminalFrameWrapper::setHeight)
        .def_property("cells", &TerminalFrameWrapper::getCells, &TerminalFrameWrapper::setCells)
        .def("get_cell", &TerminalFrameWrapper::getCell)
        .def("set_cell", &TerminalFrameWrapper::setCell);

    // byte_output_stream class
    py::class_<byte_output_stream>(m, "ByteOutputStream")
        .def(py::init<>())
        .def("write_byte", &byte_output_stream::writeByte)
        .def("write_int16", &byte_output_stream::writeUInt16)
        .def("write_int32", &byte_output_stream::writeUInt32)
        .def("write_int64", &byte_output_stream::writeUInt64)
        .def("write_bits", &byte_output_stream::writeBits)
        .def("get_buffer", &byte_output_stream::getBuffer, 
             py::return_value_policy::reference_internal);

    // byte_input_stream class
    py::class_<byte_input_stream>(m, "ByteInputStream")
        .def(py::init<const std::vector<uint8_t>>())
        .def("read_byte", &byte_input_stream::readByte)
        .def("read_int16", &byte_input_stream::readUInt16)
        .def("read_int32", &byte_input_stream::readUInt32)
        .def("read_int64", &byte_input_stream::readUInt64)
        .def("read_bits", &byte_input_stream::readBits)
        .def("has_more_data", &byte_input_stream::hasMoreData);

    // TermDeflateStream class
    py::class_<TermDeflateStream>(m, "TermDeflateStream")
        .def(py::init<>())
        .def(py::init<byte_output_stream&>())
        .def("write_frame", [](TermDeflateStream& stream, TerminalFrameWrapper &wrapper) {
            TerminalFrame raw_frame{
                .width = wrapper.getWidth(),
                .height = wrapper.getHeight(),
                .cells = nullptr
            };
            if (raw_frame.width > 0 && raw_frame.height > 0) {
                raw_frame.cells = new Cell[raw_frame.width * raw_frame.height];
                for (int i = 0; i < raw_frame.width * raw_frame.height; ++i) {
                    raw_frame.cells[i] = wrapper.getFrame().cells[i];
                }
            }
            stream.writeFrame(raw_frame);
            // frame is managed by TermDeflateStream, no need to delete it here
        });

    // TermInflateStream class
    py::class_<TermInflateStream>(m, "TermInflateStream")
        .def(py::init<byte_input_stream&>())
        .def("read_frame", [](TermInflateStream& stream) -> TerminalFrameWrapper {
            TerminalFrame raw_frame = stream.readFrame();
            TerminalFrameWrapper wrapper(raw_frame.width, raw_frame.height);
            if (raw_frame.cells != nullptr && raw_frame.width > 0 && raw_frame.height > 0) {
                for (int i = 0; i < raw_frame.width * raw_frame.height; ++i) {
                    wrapper.getFrame().cells[i] = raw_frame.cells[i];
                }
            }
            // frame is managed by TermInflateStream, no need to delete it here
            return wrapper;
        })
        .def("has_next_frame", &TermInflateStream::hasNextFrame)
        .def("seek", &TermInflateStream::seek)
        .def("get_total_num_frames", &TermInflateStream::getTotalNumFrames);

    // Frame type constants
    m.attr("I_FRAME_GLOBAL") = I_FRAME_GLOBAL;
    m.attr("I_FRAME_NO_FRAME_SIZE_TYPE") = I_FRAME_NO_FRAME_SIZE_TYPE;
    m.attr("S_FRAME_TYPE") = S_FRAME_TYPE;
    m.attr("P_FRAME_TYPE") = P_FRAME_TYPE;
    m.attr("R_FRAME_TYPE_I8") = R_FRAME_TYPE_I8;
    m.attr("R_FRAME_TYPE_I16") = R_FRAME_TYPE_I16;
    m.attr("MAX_SLIDING_WINDOW_SIZE") = MAX_SLIDING_WINDOW_SIZE;
} 
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "Board.h"

namespace py = pybind11;

PYBIND11_MODULE(chess_engine_module, m) {
	m.doc() = "A fast C++ chess engine for Python";

	// Bind the Move struct so Python can see it
	py::class_<Move>(m, "Move")

		// Expose the two constructors we made
		.def(py::init<int, int, int, int>())
		.def(py::init<int, int, int, int, int>())

		// Make the members readable from python
		.def_readonly("from_row", &Move::from_row)
		.def_readonly("from_col", &Move::from_col)
		.def_readonly("to_row", &Move::to_row)
		.def_readonly("to_col", &Move::to_col)
		.def_readonly("promotion_piece", &Move::promotion_piece);

	// Bind the main Board class
	py::class_<Board>(m, "Board")
		.def(py::init<>()) // Expose the constructor

		// Expose all the public methods we want Python to use
		.def("print", &Board::print)
		.def("makeMove", &Board::makeMove)
		.def("getLegalMoves", &Board::getLegalMoves)
		.def("isKingInCheck", &Board::isKingInCheck)
		.def("isSquareAttacked", &Board::isSquareAttacked)
        .def("get_board_state", &Board::getBoardState);
}

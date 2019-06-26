#pragma once

#include <vector>

#include "data/IDataReader.h"
#include "parse/ByteOrder.h"
#include "parse/Interpretation.h"
#include "print/IConsolePrinter.h"

namespace hx {

class Interpreter {
public: //----------------------- Public Interface -----------------------------
  // Constructor
  //    data_reader:        caller owned IDataReader providing access to the data to be interpreted.
  //                        It must outlive the Interpreter.
  //    interpretations:    caller owned vector of interpretations specifying how the data is to be interpreted.
  //                        It must outlive the Interpreter.
  //    target_byte_order:  byte order of the data to be read.
  Interpreter(IDataReader* data_reader, const std::vector<Interpretation>* interpretations, ByteOrder::Enum target_byte_order);

  // This method performs the interpretation of the data as specified by the arguments provided
  // at construction time. The passed IConsolePrinter is a caller owned object taking care
  // displaying the interpretation work.
  void performInterpretation(IConsolePrinter* printer);

private: //---------------------- Public Interface -----------------------------
  IDataReader* const                          data_reader_;      // Pointer to the data reader
  const std::vector<Interpretation>* const    interpretations_;  // Pointer to the set of interpretations to perform of the data
  const bool                                  swap_byte_order_;  // Boolean determining if the byte order of the data needs to be swapped to match the host.
};

} // namespace hx

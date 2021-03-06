// Copyright 2015 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_WASM_MODULE_H_
#define V8_WASM_MODULE_H_

#include "wasm-opcodes.h"
#include "wasm-result.h"

namespace v8 {
namespace internal {

namespace compiler {
class CallDescriptor;
}

namespace wasm {
const size_t kMinModuleSize = 8;
const size_t kMaxModuleSize = 1024 * 1024 * 1024;
const size_t kMaxFunctionSize = 128 * 1024;
const size_t kMaxStringSize = 256;

// Static representation of a wasm function.
struct WasmFunction {
  FunctionSig* sig;      // signature of the function.
  uint32_t name_offset;  // offset in the module bytes of the name, if any.
  uint32_t code_start_offset;    // offset in the module bytes of code start.
  uint32_t code_end_offset;      // offset in the module bytes of code end.
  uint16_t local_int32_count;    // number of int32 local variables.
  uint16_t local_int64_count;    // number of int64 local variables.
  uint16_t local_float32_count;  // number of float32 local variables.
  uint16_t local_float64_count;  // number of float64 local variables.
  bool exported;                 // true if this function is exported.
  bool external;  // true if this function is externally supplied.
};

struct ModuleEnv;  // forward declaration of decoder interface.

// Static representation of a wasm global variable.
struct WasmGlobal {
  uint32_t name_offset;  // offset in the module bytes of the name, if any.
  MemType type;          // type of the global.
  uint32_t offset;       // offset from beginning of globals area.
  bool exported;         // true if this global is exported.
};

// Static representation of a wasm data segment.
struct WasmDataSegment {
  uint32_t dest_addr;      // destination memory address of the data.
  uint32_t source_offset;  // start offset in the module bytes.
  uint32_t source_size;    // end offset in the module bytes.
  bool init;               // true if loaded upon instantiation.
};

// Static representation of a module.
struct WasmModule {
  static const uint8_t kMinMemSize = 12;  // Minimum memory size = 4kb
  static const uint8_t kMaxMemSize = 30;  // Maximum memory size = 1gb

  Isolate* shared_isolate;   // isolate for storing shared code.
  const byte* module_start;  // starting address for the module bytes.
  const byte* module_end;    // end address for the module bytes.
  uint8_t mem_size_log2;     // size of the memory (log base 2).
  bool mem_export;           // true if the memory is exported.
  bool mem_external;         // true if the memory is external.
  std::vector<WasmFunction>* functions;         // functions in this module.
  std::vector<WasmGlobal>* globals;             // globals in this module.
  std::vector<WasmDataSegment>* data_segments;  // data segments in this module.

  // Get a pointer to a string stored in the module bytes representing a name.
  const char* GetName(uint32_t offset) {
    CHECK(BoundsCheck(offset, offset + 1));
    if (offset == 0) return "<?>";  // no name.
    return reinterpret_cast<const char*>(module_start + offset);
  }

  // Checks the given offset range is contained within the module bytes.
  bool BoundsCheck(uint32_t start, uint32_t end) {
    size_t size = module_end - module_start;
    return start < size && end < size;
  }

  // Creates a new instantiation of the module in the given isolate.
  MaybeHandle<JSObject> Instantiate(Isolate* isolate, Handle<JSObject> ffi);
};

// forward declaration.
class WasmLinker;

// Interface provided to the decoder/graph builder which contains only
// minimal information about the globals, functions, and function tables.
struct ModuleEnv {
  uintptr_t globals_area;  // address of the globals area.
  uintptr_t mem_start;     // address of the start of linear memory.
  uintptr_t mem_end;       // address of the end of linear memory.

  WasmModule* module;
  WasmLinker* linker;
  std::vector<Handle<Code>>* function_code;

  bool IsValidGlobal(uint32_t index) {
    return module && index < module->globals->size();
  }
  bool IsValidFunction(uint32_t index) {
    return module && index < module->functions->size();
  }
  MemType GetGlobalType(uint32_t index) {
    DCHECK(IsValidGlobal(index));
    return module->globals->at(index).type;
  }
  FunctionSig* GetFunctionSignature(uint32_t index) {
    DCHECK(IsValidFunction(index));
    return module->functions->at(index).sig;
  }

  FunctionSig* GetFunctionTableSignature(uint32_t index) {
    return nullptr;  // TODO(titzer): implement function tables
  }

  Handle<Code> GetFunctionCode(uint32_t index);

  compiler::CallDescriptor* GetWasmCallDescriptor(Zone* zone, FunctionSig* sig);
  compiler::CallDescriptor* GetCallDescriptor(Zone* zone, uint32_t index);
};

std::ostream& operator<<(std::ostream& os, const WasmModule& module);
std::ostream& operator<<(std::ostream& os, const WasmFunction& function);

typedef Result<WasmModule*> ModuleResult;
typedef Result<WasmFunction*> FunctionResult;


ModuleResult DecodeWasmModule(Isolate* isolate, Zone* zone,
                              const byte* module_start, const byte* module_end,
                              bool verify_functions = true);

FunctionResult DecodeWasmFunction(Isolate* isolate, Zone* zone, ModuleEnv* env,
                                  const byte* function_start,
                                  const byte* function_end);

// For testing. Decode, verify, and run the last exported function in the
// given encoded module.
int32_t CompileAndRunWasmModule(Isolate* isolate, const byte* module_start,
                                const byte* module_end);

// For testing. Decode, verify, and run the last exported function in the
// given decoded module.
int32_t CompileAndRunWasmModule(Isolate* isolate, WasmModule* module);

// Exposed for testing. Decodes a single function signature, allocating it
// in the given zone. Returns {nullptr} upon failure.
FunctionSig* DecodeFunctionSignatureForTesting(Zone* zone, const byte* start,
                                               const byte* end);
}
}
}

#endif  // V8_WASM_MODULE_H_

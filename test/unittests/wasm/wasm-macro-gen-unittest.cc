// Copyright 2015 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "test/unittests/test-utils.h"

#include "src/wasm/wasm-macro-gen.h"

namespace v8 {
namespace internal {
namespace wasm {

class MacroGenTest : public TestWithZone {};

#define EXPECT_SIZE(size, ...)     \
  do {                             \
    byte code[] = {__VA_ARGS__};   \
    EXPECT_EQ(size, sizeof(code)); \
  } while (false)


TEST_F(MacroGenTest, Constants) {
  EXPECT_SIZE(2, WASM_ONE);
  EXPECT_SIZE(2, WASM_ZERO);

  EXPECT_SIZE(2, WASM_INT8(122));
  EXPECT_SIZE(2, WASM_INT8(254));

  EXPECT_SIZE(5, WASM_INT32(1));
  EXPECT_SIZE(5, WASM_INT32(10000));
  EXPECT_SIZE(5, WASM_INT32(-9828934));

  EXPECT_SIZE(9, WASM_INT64(1));
  EXPECT_SIZE(9, WASM_INT64(10000));
  EXPECT_SIZE(9, WASM_INT64(-9828934));
  EXPECT_SIZE(9, WASM_INT64(0x123456789abcdef0ULL));

  EXPECT_SIZE(5, WASM_FLOAT32(1.0f));
  EXPECT_SIZE(5, WASM_FLOAT32(10000.0f));
  EXPECT_SIZE(5, WASM_FLOAT32(-9828934.0f));

  EXPECT_SIZE(9, WASM_FLOAT64(1.5));
  EXPECT_SIZE(9, WASM_FLOAT64(10200.0));
  EXPECT_SIZE(9, WASM_FLOAT64(-9818934.0));
}


TEST_F(MacroGenTest, Statements) {
  EXPECT_SIZE(4, WASM_SET_LOCAL(0, WASM_ZERO));

  EXPECT_SIZE(4, WASM_STORE_GLOBAL(0, WASM_ZERO));

  EXPECT_SIZE(6, WASM_STORE_MEM(kMemInt32, WASM_ZERO, WASM_ZERO));

  EXPECT_SIZE(4, WASM_IF(WASM_ZERO, WASM_NOP));

  EXPECT_SIZE(5, WASM_IF_THEN(WASM_ZERO, WASM_NOP, WASM_NOP));

  EXPECT_SIZE(1, WASM_NOP);

  EXPECT_SIZE(3, WASM_BLOCK(1, WASM_NOP));
  EXPECT_SIZE(4, WASM_BLOCK(2, WASM_NOP, WASM_NOP));
  EXPECT_SIZE(5, WASM_BLOCK(3, WASM_NOP, WASM_NOP, WASM_NOP));

  EXPECT_SIZE(2, WASM_INFINITE_LOOP);

  EXPECT_SIZE(3, WASM_LOOP(1, WASM_NOP));
  EXPECT_SIZE(4, WASM_LOOP(2, WASM_NOP, WASM_NOP));
  EXPECT_SIZE(5, WASM_LOOP(3, WASM_NOP, WASM_NOP, WASM_NOP));
  EXPECT_SIZE(4, WASM_LOOP(1, WASM_BREAK(0)));
  EXPECT_SIZE(4, WASM_LOOP(1, WASM_CONTINUE(0)));

  EXPECT_SIZE(1, WASM_RETURN0);

  EXPECT_SIZE(3, WASM_RETURN(WASM_ZERO));
  EXPECT_SIZE(5, WASM_RETURN(WASM_ZERO, WASM_ONE));
  EXPECT_SIZE(7, WASM_RETURN(WASM_ZERO, WASM_ONE, WASM_ZERO));

  EXPECT_SIZE(4, WASM_SWITCH(0, WASM_ZERO));
  EXPECT_SIZE(5, WASM_SWITCH(1, WASM_ZERO, WASM_NOP));
  EXPECT_SIZE(6, WASM_SWITCH(2, WASM_ZERO, WASM_NOP, WASM_NOP));

  EXPECT_SIZE(4, WASM_SWITCH_NF(0, WASM_ZERO));
  EXPECT_SIZE(5, WASM_SWITCH_NF(1, WASM_ZERO, WASM_NOP));
  EXPECT_SIZE(6, WASM_SWITCH_NF(2, WASM_ZERO, WASM_NOP, WASM_NOP));
}


TEST_F(MacroGenTest, MacroStatements) {
  EXPECT_SIZE(9, WASM_WHILE(WASM_INT8(0), WASM_NOP));
  EXPECT_SIZE(7, WASM_INC_LOCAL(0));
  EXPECT_SIZE(7, WASM_INC_LOCAL_BY(0, 3));
}


TEST_F(MacroGenTest, Expressions) {
  EXPECT_SIZE(2, WASM_GET_LOCAL(0));
  EXPECT_SIZE(2, WASM_GET_LOCAL(1));
  EXPECT_SIZE(2, WASM_GET_LOCAL(12));
  EXPECT_SIZE(2, WASM_LOAD_GLOBAL(0));
  EXPECT_SIZE(2, WASM_LOAD_GLOBAL(1));
  EXPECT_SIZE(2, WASM_LOAD_GLOBAL(12));
  EXPECT_SIZE(4, WASM_LOAD_MEM(kMemInt32, WASM_ZERO));
  EXPECT_SIZE(4, WASM_LOAD_MEM(kMemFloat64, WASM_ZERO));
  EXPECT_SIZE(4, WASM_LOAD_MEM(kMemFloat32, WASM_ZERO));

  EXPECT_SIZE(7, WASM_TERNARY(WASM_ZERO, WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_COMMA(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(3, WASM_NOT(WASM_ZERO));
}


TEST_F(MacroGenTest, FunctionCalls) {
  EXPECT_SIZE(2, WASM_CALL_FUNCTION0(0));
  EXPECT_SIZE(2, WASM_CALL_FUNCTION0(1));
  EXPECT_SIZE(2, WASM_CALL_FUNCTION0(11));

  EXPECT_SIZE(4, WASM_CALL_FUNCTION(0, WASM_ZERO));
  EXPECT_SIZE(6, WASM_CALL_FUNCTION(1, WASM_ZERO, WASM_ZERO));

  EXPECT_SIZE(4, WASM_CALL_INDIRECT0(0, WASM_ZERO));
  EXPECT_SIZE(4, WASM_CALL_INDIRECT0(1, WASM_ZERO));
  EXPECT_SIZE(4, WASM_CALL_INDIRECT0(11, WASM_ZERO));

  EXPECT_SIZE(6, WASM_CALL_INDIRECT(0, WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(8, WASM_CALL_INDIRECT(1, WASM_ZERO, WASM_ZERO, WASM_ZERO));
}


TEST_F(MacroGenTest, Int32Ops) {
  EXPECT_SIZE(5, WASM_INT32_ADD(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_INT32_SUB(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_INT32_MUL(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_INT32_SDIV(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_INT32_UDIV(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_INT32_SREM(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_INT32_UREM(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_INT32_AND(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_INT32_IOR(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_INT32_XOR(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_INT32_SHL(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_INT32_SHR(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_INT32_SAR(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_INT32_EQ(WASM_ZERO, WASM_ZERO));

  EXPECT_SIZE(5, WASM_INT32_SLT(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_INT32_SLE(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_INT32_ULT(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_INT32_ULE(WASM_ZERO, WASM_ZERO));

  EXPECT_SIZE(5, WASM_INT32_SGT(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_INT32_SGE(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_INT32_UGT(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_INT32_UGE(WASM_ZERO, WASM_ZERO));

  EXPECT_SIZE(3, WASM_INT32_CLZ(WASM_ZERO));
  EXPECT_SIZE(3, WASM_INT32_CTZ(WASM_ZERO));
  EXPECT_SIZE(3, WASM_INT32_POPCNT(WASM_ZERO));
}


TEST_F(MacroGenTest, Int64Ops) {
  EXPECT_SIZE(5, WASM_INT64_ADD(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_INT64_SUB(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_INT64_MUL(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_INT64_SDIV(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_INT64_UDIV(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_INT64_SREM(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_INT64_UREM(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_INT64_AND(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_INT64_IOR(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_INT64_XOR(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_INT64_SHL(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_INT64_SHR(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_INT64_SAR(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_INT64_EQ(WASM_ZERO, WASM_ZERO));

  EXPECT_SIZE(5, WASM_INT64_SLT(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_INT64_SLE(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_INT64_ULT(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_INT64_ULE(WASM_ZERO, WASM_ZERO));

  EXPECT_SIZE(5, WASM_INT64_SGT(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_INT64_SGE(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_INT64_UGT(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_INT64_UGE(WASM_ZERO, WASM_ZERO));

  EXPECT_SIZE(3, WASM_INT64_CLZ(WASM_ZERO));
  EXPECT_SIZE(3, WASM_INT64_CTZ(WASM_ZERO));
  EXPECT_SIZE(3, WASM_INT64_POPCNT(WASM_ZERO));
}


TEST_F(MacroGenTest, Float32Ops) {
  EXPECT_SIZE(5, WASM_FLOAT32_ADD(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_FLOAT32_SUB(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_FLOAT32_MUL(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_FLOAT32_DIV(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_FLOAT32_MIN(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_FLOAT32_MAX(WASM_ZERO, WASM_ZERO));

  EXPECT_SIZE(3, WASM_FLOAT32_ABS(WASM_ZERO));
  EXPECT_SIZE(3, WASM_FLOAT32_NEG(WASM_ZERO));
  EXPECT_SIZE(3, WASM_FLOAT32_COPYSIGN(WASM_ZERO));
  EXPECT_SIZE(3, WASM_FLOAT32_CEIL(WASM_ZERO));
  EXPECT_SIZE(3, WASM_FLOAT32_FLOOR(WASM_ZERO));
  EXPECT_SIZE(3, WASM_FLOAT32_TRUNC(WASM_ZERO));
  EXPECT_SIZE(3, WASM_FLOAT32_NEARESTINT(WASM_ZERO));
  EXPECT_SIZE(3, WASM_FLOAT32_SQRT(WASM_ZERO));

  EXPECT_SIZE(5, WASM_FLOAT32_EQ(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_FLOAT32_LT(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_FLOAT32_LE(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_FLOAT32_GT(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_FLOAT32_GE(WASM_ZERO, WASM_ZERO));
}


TEST_F(MacroGenTest, Float64Ops) {
  EXPECT_SIZE(5, WASM_FLOAT64_ADD(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_FLOAT64_SUB(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_FLOAT64_MUL(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_FLOAT64_DIV(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_FLOAT64_MIN(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_FLOAT64_MAX(WASM_ZERO, WASM_ZERO));

  EXPECT_SIZE(3, WASM_FLOAT64_ABS(WASM_ZERO));
  EXPECT_SIZE(3, WASM_FLOAT64_NEG(WASM_ZERO));
  EXPECT_SIZE(3, WASM_FLOAT64_COPYSIGN(WASM_ZERO));
  EXPECT_SIZE(3, WASM_FLOAT64_CEIL(WASM_ZERO));
  EXPECT_SIZE(3, WASM_FLOAT64_FLOOR(WASM_ZERO));
  EXPECT_SIZE(3, WASM_FLOAT64_TRUNC(WASM_ZERO));
  EXPECT_SIZE(3, WASM_FLOAT64_NEARESTINT(WASM_ZERO));
  EXPECT_SIZE(3, WASM_FLOAT64_SQRT(WASM_ZERO));

  EXPECT_SIZE(5, WASM_FLOAT64_EQ(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_FLOAT64_LT(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_FLOAT64_LE(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_FLOAT64_GT(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_FLOAT64_GE(WASM_ZERO, WASM_ZERO));
}


TEST_F(MacroGenTest, Conversions) {
  EXPECT_SIZE(3, WASM_INT32_SCONVERT_FLOAT32(WASM_ZERO));
  EXPECT_SIZE(3, WASM_INT32_SCONVERT_FLOAT64(WASM_ZERO));
  EXPECT_SIZE(3, WASM_INT32_UCONVERT_FLOAT32(WASM_ZERO));
  EXPECT_SIZE(3, WASM_INT32_UCONVERT_FLOAT64(WASM_ZERO));
  EXPECT_SIZE(3, WASM_INT32_CONVERT_INT64(WASM_ZERO));
  EXPECT_SIZE(3, WASM_INT64_SCONVERT_FLOAT32(WASM_ZERO));
  EXPECT_SIZE(3, WASM_INT64_SCONVERT_FLOAT64(WASM_ZERO));
  EXPECT_SIZE(3, WASM_INT64_UCONVERT_FLOAT32(WASM_ZERO));
  EXPECT_SIZE(3, WASM_INT64_UCONVERT_FLOAT64(WASM_ZERO));
  EXPECT_SIZE(3, WASM_INT64_SCONVERT_INT32(WASM_ZERO));
  EXPECT_SIZE(3, WASM_INT64_UCONVERT_INT32(WASM_ZERO));
  EXPECT_SIZE(3, WASM_FLOAT32_SCONVERT_INT32(WASM_ZERO));
  EXPECT_SIZE(3, WASM_FLOAT32_UCONVERT_INT32(WASM_ZERO));
  EXPECT_SIZE(3, WASM_FLOAT32_SCONVERT_INT64(WASM_ZERO));
  EXPECT_SIZE(3, WASM_FLOAT32_UCONVERT_INT64(WASM_ZERO));
  EXPECT_SIZE(3, WASM_FLOAT32_CONVERT_FLOAT64(WASM_ZERO));
  EXPECT_SIZE(3, WASM_FLOAT32_REINTERPRET_INT32(WASM_ZERO));
  EXPECT_SIZE(3, WASM_FLOAT64_SCONVERT_INT32(WASM_ZERO));
  EXPECT_SIZE(3, WASM_FLOAT64_UCONVERT_INT32(WASM_ZERO));
  EXPECT_SIZE(3, WASM_FLOAT64_SCONVERT_INT64(WASM_ZERO));
  EXPECT_SIZE(3, WASM_FLOAT64_UCONVERT_INT64(WASM_ZERO));
  EXPECT_SIZE(3, WASM_FLOAT64_CONVERT_FLOAT32(WASM_ZERO));
  EXPECT_SIZE(3, WASM_FLOAT64_REINTERPRET_INT64(WASM_ZERO));
}

static const MemType kMemTypes[] = {
    kMemInt8,   kMemUint8, kMemInt16,  kMemUint16,  kMemInt32,
    kMemUint32, kMemInt64, kMemUint64, kMemFloat32, kMemFloat64};

TEST_F(MacroGenTest, LoadsAndStores) {
  for (size_t i = 0; i < arraysize(kMemTypes); i++) {
    EXPECT_SIZE(4, WASM_LOAD_MEM(kMemTypes[i], WASM_ZERO));
  }
  for (size_t i = 0; i < arraysize(kMemTypes); i++) {
    EXPECT_SIZE(6, WASM_STORE_MEM(kMemTypes[i], WASM_ZERO, WASM_GET_LOCAL(0)));
  }
}
}
}
}

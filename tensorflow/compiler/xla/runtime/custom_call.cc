/* Copyright 2022 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include "tensorflow/compiler/xla/runtime/custom_call.h"

#include <string>
#include <string_view>

#include "llvm/Support/raw_ostream.h"
#include "tensorflow/compiler/xla/runtime/type_id.h"

namespace xla {
namespace runtime {

using llvm::ArrayRef;
using llvm::raw_ostream;

template <typename T>
using TensorRef = CustomCall::TensorRef<T>;

static void PrintArr(raw_ostream& os, std::string_view name,
                     llvm::ArrayRef<int64_t> arr) {
  os << " " << name << ": [";
  auto i64_to_string = [](int64_t v) { return std::to_string(v); };
  os << llvm::join(llvm::map_range(arr, i64_to_string), ", ");
  os << "]";
}

static raw_ostream& operator<<(raw_ostream& os, PrimitiveType type) {
  return os << primitive_util::LowercasePrimitiveTypeName(type);
}

raw_ostream& operator<<(raw_ostream& os, const StridedMemrefView& view) {
  os << "StridedMemrefView: dtype: " << view.dtype;
  PrintArr(os, "sizes", view.sizes);
  PrintArr(os, "strides", view.strides);
  return os;
}

raw_ostream& operator<<(raw_ostream& os, const MemrefView& view) {
  os << "MemrefView: dtype: " << view.dtype;
  PrintArr(os, "sizes", view.sizes);
  return os;
}

raw_ostream& operator<<(raw_ostream& os, const FlatMemrefView& view) {
  return os << "FlatMemrefView: dtype: " << view.dtype
            << " size_in_bytes: " << view.size_in_bytes;
}

void PopulateCustomCallTypeIdNames(TypeIDNameRegistry& r) {
  r.Register<Tagged<std::string_view>>("__type_id_string");

  r.Register<Tagged<uint8_t>>("__type_id_uint8");
  r.Register<Tagged<uint32_t>>("__type_id_uint32");
  r.Register<Tagged<uint64_t>>("__type_id_uint64");
  r.Register<Tagged<bool>>("__type_id_bool");
  r.Register<Tagged<int32_t>>("__type_id_int32");
  r.Register<Tagged<int64_t>>("__type_id_int64");
  r.Register<Tagged<Eigen::half>>("__type_id_half");
  r.Register<Tagged<float>>("__type_id_float");
  r.Register<Tagged<double>>("__type_id_double");

  r.Register<Tagged<MemrefView>>("__type_id_memref_view");
  r.Register<Tagged<StridedMemrefView>>("__type_id_strided_memref_view");
  r.Register<Tagged<EmptyArrayRef>>("__type_id_empty_array");

  r.Register<Tagged<ArrayRef<int8_t>>>("__type_id_arrayr_int8");
  r.Register<Tagged<ArrayRef<int16_t>>>("__type_id_array_int16");
  r.Register<Tagged<ArrayRef<int32_t>>>("__type_id_array_int32");
  r.Register<Tagged<ArrayRef<int64_t>>>("__type_id_array_int64");
  r.Register<Tagged<ArrayRef<float>>>("__type_id_array_float");
  r.Register<Tagged<ArrayRef<double>>>("__type_id_array_double");

  r.Register<Tagged<TensorRef<int32_t>>>("__type_id__tensor_int32_t");
  r.Register<Tagged<TensorRef<int64_t>>>("__type_id_tensor_int64_t");
  r.Register<Tagged<TensorRef<float>>>("__type_id_tensor_float");
  r.Register<Tagged<TensorRef<double>>>("__type_id_tensor_double");
}

}  // namespace runtime
}  // namespace xla

XLA_RUNTIME_DEFINE_EXPLICIT_TYPE_ID(std::string_view);
XLA_RUNTIME_DEFINE_EXPLICIT_TYPE_ID(xla::runtime::StridedMemrefView);
XLA_RUNTIME_DEFINE_EXPLICIT_TYPE_ID(xla::runtime::MemrefView);
XLA_RUNTIME_DEFINE_EXPLICIT_TYPE_ID(xla::runtime::FlatMemrefView);
XLA_RUNTIME_DEFINE_EXPLICIT_TYPE_ID(int32_t);
XLA_RUNTIME_DEFINE_EXPLICIT_TYPE_ID(int64_t);
XLA_RUNTIME_DEFINE_EXPLICIT_TYPE_ID(float);
XLA_RUNTIME_DEFINE_EXPLICIT_TYPE_ID(double);
XLA_RUNTIME_DEFINE_EXPLICIT_TYPE_ID(ArrayRef<int32_t>);
XLA_RUNTIME_DEFINE_EXPLICIT_TYPE_ID(ArrayRef<int64_t>);
XLA_RUNTIME_DEFINE_EXPLICIT_TYPE_ID(ArrayRef<float>);
XLA_RUNTIME_DEFINE_EXPLICIT_TYPE_ID(ArrayRef<double>);

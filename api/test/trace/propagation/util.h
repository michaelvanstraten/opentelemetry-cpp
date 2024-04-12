// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <string>

template <typename T>
static nostd::string Hex(const T &id_item)
{
  char buf[T::kSize * 2];
  id_item.ToLowerBase16(buf);
  return nostd::string(buf, sizeof(buf));
}

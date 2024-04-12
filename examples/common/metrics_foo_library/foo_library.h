// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <string>

class foo_library
{
public:
  static void counter_example(const nostd::string &name);
  static void histogram_example(const nostd::string &name);
  static void observable_counter_example(const nostd::string &name);
};

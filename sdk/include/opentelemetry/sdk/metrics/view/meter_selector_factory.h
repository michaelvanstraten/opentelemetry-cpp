// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <memory>

#include "opentelemetry/version.h"
#include "opentelemetry/nostd/string.h"


OPENTELEMETRY_BEGIN_NAMESPACE

namespace sdk
{
namespace metrics
{

class MeterSelector;

class OPENTELEMETRY_EXPORT MeterSelectorFactory
{
public:
  static std::unique_ptr<MeterSelector> Create(const nostd::string &name,
                                               const nostd::string &version,
                                               const nostd::string &schema);
};

}  // namespace metrics
}  // namespace sdk
OPENTELEMETRY_END_NAMESPACE

// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "opentelemetry/nostd/mozString.h"
#include "opentelemetry/version.h"

OPENTELEMETRY_BEGIN_NAMESPACE
namespace nostd
{

using string = ::mozilla::basic_string<char>;

}  // namespace nostd
OPENTELEMETRY_END_NAMESPACE

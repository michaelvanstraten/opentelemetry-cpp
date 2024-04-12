// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#include "opentelemetry/sdk/resource/resource_detector.h"
#include "opentelemetry/sdk/common/env_variables.h"
#include "opentelemetry/sdk/resource/resource.h"
#include "opentelemetry/sdk/resource/semantic_conventions.h"

#include <sstream>
#include <string>

OPENTELEMETRY_BEGIN_NAMESPACE
namespace sdk
{
namespace resource
{

const char *OTEL_RESOURCE_ATTRIBUTES = "OTEL_RESOURCE_ATTRIBUTES";
const char *OTEL_SERVICE_NAME        = "OTEL_SERVICE_NAME";

Resource OTELResourceDetector::Detect() noexcept
{
  nostd::string attributes_str, service_name;

  bool attributes_exists = opentelemetry::sdk::common::GetStringEnvironmentVariable(
      OTEL_RESOURCE_ATTRIBUTES, attributes_str);
  bool service_name_exists =
      opentelemetry::sdk::common::GetStringEnvironmentVariable(OTEL_SERVICE_NAME, service_name);

  if (!attributes_exists && !service_name_exists)
  {
    return Resource();
  }

  ResourceAttributes attributes;

  if (attributes_exists)
  {
    std::istringstream iss(attributes_str);
    nostd::string token;
    while (std::getline(iss, token, ','))
    {
      size_t pos = token.find('=');
      if (pos != nostd::string::npos)
      {
        nostd::string key   = token.substr(0, pos);
        nostd::string value = token.substr(pos + 1);
        attributes[key]   = value;
      }
    }
  }

  if (service_name_exists)
  {
    attributes[SemanticConventions::kServiceName] = service_name;
  }

  return Resource(attributes);
}

}  // namespace resource
}  // namespace sdk
OPENTELEMETRY_END_NAMESPACE

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <functional>

extern "C" {
#include "stinger_core/xmalloc.h"
#include "stinger_core/stinger_error.h"
}

#include "rapidjson/document.h"
#include "rapidjson/filestream.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "json_rpc.h"

void
process_json_rpc_request ();

rapidjson::Document *
json_rpc_error (int32_t error_code)
{
  rapidjson::Document * document = new rapidjson::Document();
  rapidjson::Document::AllocatorType& allocator = document->GetAllocator();
  document->SetObject();

  document->AddMember("jsonrpc", "2.0", allocator);

  rapidjson::Value err_obj (rapidjson::kObjectType);
  rapidjson::Value code, message;
  code.SetInt(error_code);

  switch (error_code) {
    case (-32700):
      {
	message.SetString("Parse error");
      } break;

    case (-32600):
      {
	message.SetString("Invalid request");
      } break;

    case (-32601):
      {
	message.SetString("Method not found");
      } break;

    case (-32602):
      {
	message.SetString("Invalid params");
      } break;

    case (-32603):
      {
	message.SetString("Internal error");
      } break;
  }

  if ( (error_code <= -32000) && (error_code >= -32099)) {
    message.SetString("Server error");
  }

  err_obj.AddMember("code", code, allocator);
  err_obj.AddMember("message", message, allocator);
  document->AddMember("error", err_obj, allocator);

  return document;
}

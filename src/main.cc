#include <iostream>
#include <functional>

#include <node.h>
#include <nan.h>

using namespace std;
using namespace v8;

// #define DEBUG true

#include "tcp-checker.cc"

#define STR(s) Nan::New(s).ToLocalChecked()

void tcpCheck(const FunctionCallbackInfo<Value>& args) {

  Nan::HandleScope scope;

  // Convert js arguments to C++ variables
  String::Utf8Value ip_ptr(args[0]);
  char *ip = *ip_ptr;
  uint32_t port = Local<Number>::Cast(args[1])->Uint32Value();

  // Create a new Persistent<Function> to avoid GC for js callback function
  auto fn_p = new Nan::Persistent<Function>(Local<Function>::Cast(args[2]));

  // Create a callback function, it will be used on TcpChecker
  auto callback = [fn_p](int status) {

    Nan::HandleScope scope;

    // Convert status to a js object, that contains 'name', and 'message' props
    Local<Object> result = Nan::New<Object>();
    if (status) {
      result->Set(STR("name"), STR(uv_err_name(status)));
      result->Set(STR("message"), STR(uv_strerror(status)));
    } else {
      result->Set(STR("name"), STR("OK"));
      result->Set(STR("message"), STR("ok"));
    }

    // Call the js callback function
    auto context = Nan::GetCurrentContext();
    auto fn = fn_p->Get(context->GetIsolate());
    auto global = context->Global();
    Local<Value> argv[] = { result };
    Nan::MakeCallback(global, fn, 1, argv);

    // Delete the Persistent<Function>
    fn_p->Reset();
    delete fn_p;

  };

  // Create a new TcpChecker, it will be deleted by itsef after callback
  new TcpChecker(ip, port, callback);

}

void init(Local<Object> exports, Local<Object> module) {
  NODE_SET_METHOD(module, "exports", tcpCheck);
}

NODE_MODULE(addon, init)

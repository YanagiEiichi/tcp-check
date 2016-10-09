#include <iostream>
#include <functional>

#include <node.h>
#include <nan.h>

using namespace std;
using namespace v8;

#ifndef DEBUG
#define DEBUG false
#endif

#include "tcp-checker.cc"

#define STR(s) Nan::New(s).ToLocalChecked()

void tcpCheck(const FunctionCallbackInfo<Value>& args) {

  Nan::HandleScope scope;

  String::Utf8Value ip_ptr(args[0]);
  char *ip = *ip_ptr;
  uint32_t port = Local<Number>::Cast(args[1])->Uint32Value();

  auto fn_p = new Nan::Persistent<Function>(Local<Function>::Cast(args[2]));
  auto callback = [fn_p](int status) {
    Nan::HandleScope scope;
    auto context = Nan::GetCurrentContext();
    auto isolate = context->GetIsolate();
    auto global = context->Global();
    auto fn = fn_p->Get(isolate);
    if (fn->IsFunction()) {
      if (status) {
        Local<String> name = STR(uv_err_name(status));
        Local<String> message = STR(uv_strerror(status)); 
        Local<Object> result = Nan::New<Object>();
        result->Set(STR("name"), name);
        result->Set(STR("message"), message);
        Local<Value> argv[] = { result };
	Nan::MakeCallback(global, fn, 1, argv);
      } else {
	Nan::MakeCallback(global, fn, 0, NULL);
      }
    } else {
      if (DEBUG) cout << "\033[35m" << "`fn` is not a js function" << endl;
    }
    delete fn_p;
  };

  new TcpChecker(ip, port, callback);

}

void init(Local<Object> exports, Local<Object> module) {
  NODE_SET_METHOD(module, "exports", tcpCheck);
}

NODE_MODULE(addon, init)

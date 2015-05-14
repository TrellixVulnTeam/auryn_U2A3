// Copyright David Corticchiato
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
// OR OTHER DEALINGS IN THE SOFTWARE.

#ifndef BASTIAN_FCONTEXT_H_
#define BASTIAN_FCONTEXT_H_

#include <vector>
#include "src/handle.h"
#include "src/value.h"


namespace bastian {

class FunctionContext {
 public:
    virtual int ArgsCount() = 0;
    virtual Handle<Value> GetArgument(int index) = 0;
    virtual void SetResult(Handle<Value> result) = 0;
};

#define WRAPPED_FUNC_NAME(FuncName) Wrapped ## FuncName


//
// V8 Function Context
//

#ifdef BASTIAN_V8

#include <v8.h>

class V8FunctionContext : FunctionContext {
 public:
    explicit V8FunctionContext(const v8::FunctionCallbackInfo<v8::Value>&);
    int ArgsCount();
    Handle<Value> GetArgument(int index);
    void SetResult(Handle<Value> result);

 private:
    const v8::FunctionCallbackInfo<v8::Value>* infos_;
};


#define BASTIAN_FUNC(FuncName) \
void WRAPPED_FUNC_NAME(FuncName)(bastian::V8FunctionContext* ctx); \
void FuncName(const v8::FunctionCallbackInfo<v8::Value>& infos) { \
  bastian::V8FunctionContext* ctx = new bastian::V8FunctionContext(infos); \
  WRAPPED_FUNC_NAME(FuncName)(ctx); \
} \
void WRAPPED_FUNC_NAME(FuncName)(bastian::V8FunctionContext* ctx)


#endif

//
// JavascriptCore Function Context
//

#ifdef BASTIAN_JSC

#include <JavascriptCore/JavascriptCore.h>

class JSCFunctionContext : FunctionContext {
 public:
    JSCFunctionContext(
      JSContextRef,
      JSObjectRef,
      JSObjectRef,
      size_t,
      const JSValueRef*,
      JSValueRef*);
    int ArgsCount();
    Handle<Value> GetArgument(int index);
    void SetResult(Handle<Value> result);
    JSValueRef ResultRef();

 private:
    JSContextRef context_ref_;
    JSObjectRef function_ref_;
    JSObjectRef this_ref_;
    size_t argument_count_;
    const JSValueRef* arguments_ref_;
    JSValueRef* exception_ref_;
    JSValueRef result_ref_;
};

#define BASTIAN_FUNC(FuncName) \
void WRAPPED_FUNC_NAME(FuncName)(bastian::JSCFunctionContext* ctx); \
JSValueRef FuncName( \
    JSContextRef context_ref, \
    JSObjectRef function_ref, \
    JSObjectRef this_ref, \
    size_t argument_count, \
    const JSValueRef* arguments_ref, \
    JSValueRef* exception_ref) { \
  bastian::JSCFunctionContext* ctx = new bastian::JSCFunctionContext( \
    context_ref, \
    function_ref, \
    this_ref, \
    argument_count, \
    arguments_ref, \
    exception_ref); \
  WRAPPED_FUNC_NAME(FuncName)(ctx); \
  return ctx->ResultRef(); \
} \
void WRAPPED_FUNC_NAME(FuncName)(bastian::JSCFunctionContext* ctx)

#endif

}  // namespace bastian

#endif  // BASTIAN_FCONTEXT_H_

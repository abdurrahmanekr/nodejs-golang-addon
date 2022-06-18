#include <chrono>
#include <thread>
#include <assert.h>
#include "napi.h"

#include "awesome-lib.h"

struct awesome_options
{
    GoInt64 second;
};

struct AwesomeContext
{
    AwesomeContext(Napi::Env env) : deferred(Napi::Promise::Deferred::New(env)){};

    // Native Promise returned to JavaScript
    Napi::Promise::Deferred deferred;

    // Native thread
    std::thread nativeThread;

    // options
    awesome_options *opts;

    int output;

    Napi::ThreadSafeFunction tsfn;
};

// This callback transforms the native addon data (int *data) to JavaScript
// values. It also receives the treadsafe-function's registered callback, and
// may choose to call it.
void JSCallback(Napi::Env env, Napi::Function jsCallback, std::string log) {
    jsCallback.Call({Napi::String::New(env, log)});
}

void GoCallback(const char *msg, void *ptr)
{
    AwesomeContext *c = reinterpret_cast<AwesomeContext *>(ptr);

    // Perform a call into JavaScript.
    napi_status status =
        c->tsfn.BlockingCall(msg, JSCallback);

    if (status != napi_ok)
    {
        Napi::Error::Fatal(
            "ThreadEntry",
            "Napi::ThreadSafeNapi::Function.BlockingCall() failed");
    }
}

// The thread entry point. This takes as its arguments the specific
// threadsafe-function context created inside the main thread.
void awesomeJobThreadEntry(AwesomeContext *context)
{
    awesome_options *opts = context->opts;

    AwesomeJob(
        opts->second,
        (void*)GoCallback,
        (void*)context
    );

    context->output = 1;

    // Release the thread-safe function. This decrements the internal thread
    // count, and will perform finalization since the count will reach 0.
    context->tsfn.Release();
}

void FinalizerCallback(Napi::Env env,
                       void *finalizeData,
                       AwesomeContext *context)
{
    // Join the thread
    context->nativeThread.join();

    // Resolve the Promise previously returned to JS via the AwesomeJob method.
    context->deferred.Resolve(Napi::Number::New(env, context->output));
    delete context;
}

// Exported JavaScript function. Creates the thread-safe function and native
// thread. Promise is resolved in the thread-safe function's finalizer.
Napi::Value AwesomeJobBridge(const Napi::CallbackInfo &info)
{
    assert(info[0].IsNumber());
    assert(info[1].IsFunction());

    Napi::Env env = info.Env();

    // Construct context data
    auto awesomeData = new AwesomeContext(env);

    double second = info[0].As<Napi::Number>().DoubleValue();
    Napi::Function callback = info[1].As<Napi::Function>();

    awesome_options *opts = new awesome_options;
    opts->second = second;

    awesomeData->opts = opts;

    // Create a new ThreadSafeFunction.
    awesomeData->tsfn = Napi::ThreadSafeFunction::New(
        env,               // Environment
        callback,          // JS function from caller
        "AWESOME_JOB",     // Resource name
        0,                 // Max queue size (0 = unlimited).
        1,                 // Initial thread count
        awesomeData,       // Context,
        FinalizerCallback, // Finalizer
        (void *)nullptr    // Finalizer data
    );

    awesomeData->nativeThread = std::thread(awesomeJobThreadEntry, awesomeData);

    // Return the deferred's Promise. This Promise is resolved in the thread-safe
    // function's finalizer callback.
    return awesomeData->deferred.Promise();
}

// Addon entry point
Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    exports["awesomeJob"] = Napi::Function::New(env, AwesomeJobBridge);
    return exports;
}

NODE_API_MODULE(addon, Init)
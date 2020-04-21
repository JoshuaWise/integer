#include <cmath>
#include <string>
#include <node.h>
#include <node_object_wrap.h>
template <class T> using CopyablePersistent = v8::Persistent<T, v8::CopyablePersistentTraits<T>>;

struct Addon {
	CopyablePersistent<v8::Function> constructor;
	CopyablePersistent<v8::FunctionTemplate> constructorTemplate;
	struct { bool privileges; int64_t value; } controller;

	static void Cleanup(void* ptr) {
		delete static_cast<Addon*>(ptr);
	}
};

#include "result.cpp"
#include "macros.cpp"
#include "integer.cpp"

NODE_MODULE_INIT(/* exports, context */) {
	v8::Isolate* isolate = context->GetIsolate();
	v8::HandleScope scope(isolate);

	// Initialize addon instance.
	Addon* addon = new Addon();
	v8::Local<v8::External> data = v8::External::New(isolate, addon);
	node::AddEnvironmentCleanupHook(isolate, Addon::Cleanup, addon);

	// Create constructor template and constructor.
	v8::Local<v8::FunctionTemplate> t = Integer::Init(isolate, data);
	t->PrototypeTemplate()->SetInternalFieldCount(1);
	v8::Local<v8::Function> c = t->GetFunction(context).ToLocalChecked();
	v8::Local<v8::Object>::Cast(c->Get(context, InternalizedFromLatin1(isolate, "prototype")).ToLocalChecked())->SetAlignedPointerInInternalField(0, &addon->controller);

	// Store addon instance data.
	addon->constructor.Reset(isolate, c);
	addon->constructorTemplate.Reset(isolate, t);
	addon->controller.privileges = false;
	addon->controller.value = 0;

	// Export constructor.
	exports->Set(context, InternalizedFromLatin1(isolate, "Integer"), c).FromJust();
}

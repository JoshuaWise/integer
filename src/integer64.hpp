#include <node.h>
#include <node_object_wrap.h>
#include "macros.hpp"

inline void NODE_SET_PROTOTYPE_GETTER(v8::Local<v8::FunctionTemplate> recv, const char* name, v8::AccessorGetterCallback getter) {
	v8::Isolate* isolate = v8::Isolate::GetCurrent();
	v8::HandleScope scope(isolate);
	recv->InstanceTemplate()->SetAccessor(
		StringFromLatin1(isolate, name),
		getter,
		0,
		v8::Local<v8::Value>(),
		v8::AccessControl::ALL_CAN_READ,
		v8::PropertyAttribute::None,
		v8::AccessorSignature::New(isolate, recv)
	);
}

struct Error {
	explicit Error(const char* _message, bool range_error = false) : message(_message),
		Type(range_error ? v8::Exception::RangeError : v8::Exception::TypeError) {}
	const char* const message;
	const ErrorType Type;
};

class Result {
public:
	explicit Result(const char* message, bool range_error = false) : error(new Error(message, range_error)) {}
	explicit Result(int64_t _value) : value(_value), error(NULL) {}
	~Result() {delete error;}
	inline int64_t Checked() {assert(error == NULL); return value;}
	const Error* const error;
private:
	int64_t value;
};

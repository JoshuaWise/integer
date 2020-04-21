class Integer : public node::ObjectWrap {
public:

	static v8::Local<v8::FunctionTemplate> Init(v8::Isolate* isolate, v8::Local<v8::External> data) {
		v8::Local<v8::FunctionTemplate> t = NewConstructorTemplate(isolate, data, New, "Integer");
		SetPrototypeGetter(isolate, data, t, "low", Low);
		SetPrototypeGetter(isolate, data, t, "high", High);
		SetPrototypeMethod(isolate, data, t, "add", Add);
		SetPrototypeMethod(isolate, data, t, "subtract", Subtract);
		SetPrototypeMethod(isolate, data, t, "multiply", Multiply);
		SetPrototypeMethod(isolate, data, t, "divide", Divide);
		SetPrototypeMethod(isolate, data, t, "modulo", Modulo);
		SetPrototypeMethod(isolate, data, t, "negate", Negate);
		SetPrototypeMethod(isolate, data, t, "abs", Abs);
		SetPrototypeMethod(isolate, data, t, "and", And);
		SetPrototypeMethod(isolate, data, t, "or", Or);
		SetPrototypeMethod(isolate, data, t, "xor", Xor);
		SetPrototypeMethod(isolate, data, t, "not", Not);
		SetPrototypeMethod(isolate, data, t, "shiftLeft", ShiftLeft);
		SetPrototypeMethod(isolate, data, t, "shiftRight", ShiftRight);
		SetPrototypeMethod(isolate, data, t, "equals", Equals);
		SetPrototypeMethod(isolate, data, t, "notEquals", NotEquals);
		SetPrototypeMethod(isolate, data, t, "greaterThan", GreaterThan);
		SetPrototypeMethod(isolate, data, t, "greaterThanOrEquals", GreaterThanOrEquals);
		SetPrototypeMethod(isolate, data, t, "lessThan", LessThan);
		SetPrototypeMethod(isolate, data, t, "lessThanOrEquals", LessThanOrEquals);
		SetPrototypeMethod(isolate, data, t, "compare", Compare);
		SetPrototypeMethod(isolate, data, t, "bitSizeAbs", BitSizeAbs);
		SetPrototypeMethod(isolate, data, t, "isEven", IsEven);
		SetPrototypeMethod(isolate, data, t, "isOdd", IsOdd);
		SetPrototypeMethod(isolate, data, t, "isPositive", IsPositive);
		SetPrototypeMethod(isolate, data, t, "isNegative", IsNegative);
		SetPrototypeMethod(isolate, data, t, "isZero", IsZero);
		SetPrototypeMethod(isolate, data, t, "isNonZero", IsNonZero);
		SetPrototypeMethod(isolate, data, t, "isSafe", IsSafe);
		SetPrototypeMethod(isolate, data, t, "isUnsafe", IsUnsafe);
		SetPrototypeMethod(isolate, data, t, "toNumberUnsafe", ToNumberUnsafe);
		SetPrototypeMethod(isolate, data, t, "toString", ToString);
		SetPrototypeMethod(isolate, data, t, "valueOf", ValueOf);
		SetStaticMethod(isolate, data, t, "fromString", FromString);
		SetStaticMethod(isolate, data, t, "fromNumber", FromNumber);
		SetStaticMethod(isolate, data, t, "fromBits", FromBits);
		SetStaticMethod(isolate, data, t, "isInstance", IsInstance);
		return t;
	}

private:
	explicit Integer(int64_t _value) : node::ObjectWrap(), value(_value) {}

	NODE_METHOD(New) {
		if (info.IsConstructCall()) {
			UseAddon;
			if (!addon->controller.privileges) {
				return ThrowTypeError(info, "Disabled constructor (use fromString, fromNumber, or fromBits)");
			}
			addon->controller.privileges = false;
			(new Integer(addon->controller.value))->Wrap(info.This());
			return info.GetReturnValue().Set(info.This());
		}
		if (info.Length() == 0) return ReturnNew(info, 0);
		Result cast = Cast(info, info[0]);
		// TODO: ideally if info[0] is an Integer, that same object is returned.
		cast.error ? ThrowException(info, *cast.error) : ReturnNew(info, cast.Checked());
	}

	NODE_METHOD(FromBits) {
		int32_t low;
		int32_t high = 0;
		REQUIRE_ARGUMENT_INT32(first, low);
		if (info.Length() > 1) { REQUIRE_ARGUMENT_INT32(second, high); }
		ReturnNew(info, (int64_t)((((uint64_t)((uint32_t)high)) << 32) | (uint32_t)low));
	}

	NODE_METHOD(FromNumber) {
		REQUIRE_ARGUMENT_NUMBER(first, v8::Local<v8::Number> number);
		Result cast = Cast(number);
		if (!cast.error) return ReturnNew(info, cast.Checked());
		if (info.Length() == 1) return ThrowException(info, *cast.error);
		v8::Local<v8::Value> defValue = info[1];
		if (HasInstance(info, defValue)) return info.GetReturnValue().Set(defValue);
		if (!defValue->IsNumber()) return ThrowTypeError(info, "Expected the default value to be a number or Integer");
		Result def = Cast(v8::Local<v8::Number>::Cast(defValue));
		if (!def.error) return ReturnNew(info, def.Checked());
		ThrowTypeError(info, "The default value could not be converted to an Integer");
	}

	NODE_METHOD(FromString) {
		REQUIRE_ARGUMENT_STRING(first, v8::Local<v8::String> string);
		uint32_t radix = 10;
		if (info.Length() > 1) {
			REQUIRE_ARGUMENT_UINT32(second, radix);
			if (radix < 2 || radix > 36) return ThrowRangeError(info, "Radix argument must be within 2 - 36");
		}
		Result cast = Cast(info, string, (uint8_t)radix);
		if (!cast.error) return ReturnNew(info, cast.Checked());
		if (info.Length() < 3) return ThrowException(info, *cast.error);
		v8::Local<v8::Value> defValue = info[2];
		if (HasInstance(info, defValue)) return info.GetReturnValue().Set(defValue);
		if (!defValue->IsString()) return ThrowTypeError(info, "Expected the default value to be a string or Integer");
		Result def = Cast(info, v8::Local<v8::String>::Cast(defValue), (uint8_t)radix);
		if (!def.error) return ReturnNew(info, def.Checked());
		ThrowTypeError(info, "The default value could not be converted to an Integer");
	}

	NODE_METHOD(IsInstance) {
		Return(info, info.Length() != 0 && HasInstance(info, info[0]));
	}

	NODE_GETTER(Low) { UseValue;
		Return(info, (int32_t)((uint32_t)(((uint64_t)value) & U32_in_U64)));
	}

	NODE_GETTER(High) { UseValue;
		Return(info, (int32_t)((uint32_t)(((uint64_t)value) >> 32)));
	}

	NODE_METHOD(Add) { UseValue; UseArgument;
		if ((arg > 0 && value > MAX_VALUE - arg) || (arg < 0 && value < MIN_VALUE - arg))
			return ThrowRangeError(info, "Integer overflow");
		ReturnNew(info, value + arg);
	}

	NODE_METHOD(Subtract) { UseValue; UseArgument;
		if ((arg < 0 && value > MAX_VALUE + arg) || (arg > 0 && value < MIN_VALUE + arg))
			return ThrowRangeError(info, "Integer overflow");
		ReturnNew(info, value - arg);
	}

	NODE_METHOD(Multiply) { UseValue; UseArgument;
		if (value > 0
			? (arg > 0 ? value > MAX_VALUE / arg : arg < MIN_VALUE / value)
			: (arg > 0 ? value < MIN_VALUE / arg : (value != 0 && arg < MAX_VALUE / value)))
			return ThrowRangeError(info, "Integer overflow");
		ReturnNew(info, value * arg);
	}

	NODE_METHOD(Divide) { UseValue; UseArgument;
		if (arg == 0) return ThrowRangeError(info, "Divide by zero");
		if (arg == -1 && value == MIN_VALUE) return ThrowRangeError(info, "Integer overflow");
		ReturnNew(info, value / arg);
	}

	NODE_METHOD(Modulo) { UseValue; UseArgument;
		if (arg == 0) return ThrowRangeError(info, "Divide by zero");
		ReturnNew(info, arg == -1 ? 0 : value % arg);
	}

	NODE_METHOD(Negate) { UseValue;
		if (value == MIN_VALUE) return ThrowRangeError(info, "Integer overflow");
		ReturnNew(info, -value);
	}

	NODE_METHOD(Abs) { UseValue;
		if (value == MIN_VALUE) return ThrowRangeError(info, "Integer overflow");
		ReturnNew(info, value >= 0 ? value : -value);
	}

	NODE_METHOD(And) { UseValue; UseArgument;
		ReturnNew(info, value & arg);
	}

	NODE_METHOD(Or) { UseValue; UseArgument;
		ReturnNew(info, value | arg);
	}

	NODE_METHOD(Xor) { UseValue; UseArgument;
		ReturnNew(info, value ^ arg);
	}

	NODE_METHOD(Not) { UseValue;
		ReturnNew(info, ~value);
	}

	NODE_METHOD(ShiftLeft) { UseValue;
		REQUIRE_ARGUMENT_UINT32(first, uint32_t shift);
		ReturnNew(info, (int64_t)((uint64_t)value << (shift & 63)));
	}

	NODE_METHOD(ShiftRight) { UseValue;
		REQUIRE_ARGUMENT_UINT32(first, uint32_t shift);
		ReturnNew(info, value >> (shift & 63));
	}

	NODE_METHOD(Equals) { UseValue; UseArgument;
		Return(info, value == arg);
	}

	NODE_METHOD(NotEquals) { UseValue; UseArgument;
		Return(info, value != arg);
	}

	NODE_METHOD(GreaterThan) { UseValue; UseArgument;
		Return(info, value > arg);
	}

	NODE_METHOD(GreaterThanOrEquals) { UseValue; UseArgument;
		Return(info, value >= arg);
	}

	NODE_METHOD(LessThan) { UseValue; UseArgument;
		Return(info, value < arg);
	}

	NODE_METHOD(LessThanOrEquals) { UseValue; UseArgument;
		Return(info, value <= arg);
	}

	NODE_METHOD(Compare) { UseValue; UseArgument;
		Return(info, value < arg ? -1 : value > arg ? 1 : 0);
	}

	NODE_METHOD(BitSizeAbs) { UseValue;
		if (value < 0) {
			if (value == MIN_VALUE) return Return(info, (uint32_t)64);
			value = -value;
		}
		uint64_t uvalue = (uint64_t)value;
		uint32_t bits = 1;
		while (uvalue >>= 1) { ++bits; }
		Return(info, bits);
	}

	NODE_METHOD(IsEven) { UseValue;
		Return(info, (value & 1) == 0);
	}

	NODE_METHOD(IsOdd) { UseValue;
		Return(info, (value & 1) != 0);
	}

	NODE_METHOD(IsPositive) { UseValue;
		Return(info, value >= 0);
	}

	NODE_METHOD(IsNegative) { UseValue;
		Return(info, value < 0);
	}

	NODE_METHOD(IsZero) { UseValue;
		Return(info, value == 0);
	}

	NODE_METHOD(IsNonZero) { UseValue;
		Return(info, value != 0);
	}

	NODE_METHOD(IsSafe) { UseValue;
		Return(info, value <= MAX_SAFE && value >= MIN_SAFE);
	}

	NODE_METHOD(IsUnsafe) { UseValue;
		Return(info, value > MAX_SAFE || value < MIN_SAFE);
	}

	NODE_METHOD(ToNumberUnsafe) { UseValue;
		Return(info, (double)value);
	}

	NODE_METHOD(ToString) { UseValue;
		uint32_t radix = 10;
		if (info.Length()) {
			REQUIRE_ARGUMENT_UINT32(first, radix);
			if (radix < 2 || radix > 36) return ThrowRangeError(info, "Radix argument must be within 2 - 36");
		}
		char buffer[STRING_BUFFER_LENGTH];
		info.GetReturnValue().Set(StringFromLatin1(info.GetIsolate(), WriteString(buffer, value, (uint8_t)radix)));
	}

	NODE_METHOD(ValueOf) { UseValue;
		if (value <= MAX_SAFE && value >= MIN_SAFE) return Return(info, (double)value);
		char buffer[STRING_BUFFER_LENGTH];
		std::string message = "Cannot losslessly convert ";
		message += WriteString(buffer, value, 10);
		message += " to a number";
		ThrowRangeError(info, message.c_str());
	}

	static inline bool HasInstance(NODE_ARGUMENTS info, v8::Local<v8::Value> value) { UseAddon;
		return v8::Local<v8::FunctionTemplate>::New(info.GetIsolate(), addon->constructorTemplate)->HasInstance(value);
	}

	static inline void Return(NODE_GETTER_ARGUMENTS info, int32_t value) { info.GetReturnValue().Set(value); }
	static inline void Return(NODE_ARGUMENTS info, int32_t value) { info.GetReturnValue().Set(value); }
	static inline void Return(NODE_ARGUMENTS info, uint32_t value) { info.GetReturnValue().Set(value); }
	static inline void Return(NODE_ARGUMENTS info, double value) { info.GetReturnValue().Set(value); }
	static inline void Return(NODE_ARGUMENTS info, bool value) { info.GetReturnValue().Set(value); }

	static inline void ReturnNew(NODE_ARGUMENTS info, int64_t value) { UseAddon;
		v8::Isolate* isolate = info.GetIsolate();
		addon->controller.privileges = true;
		addon->controller.value = value;
		info.GetReturnValue().Set(v8::Local<v8::Function>::New(isolate, addon->constructor)->NewInstance(isolate->GetCurrentContext()).ToLocalChecked());
	}

	static Result Cast(NODE_ARGUMENTS info, v8::Local<v8::Value> value) {
		if (value->IsNumber()) return Cast(v8::Local<v8::Number>::Cast(value));
		if (value->IsString()) return Cast(info, v8::Local<v8::String>::Cast(value), 10);
		if (HasInstance(info, value)) return Result(node::ObjectWrap::Unwrap<Integer>(v8::Local<v8::Object>::Cast(value))->value);
		return Result("Expected a number, string, or Integer");
	}

	static Result Cast(v8::Local<v8::Number> number) {
		double value = number->Value();
		if (!std::isfinite(value) || std::floor(value) != value) return Result("The given number is not an integer");
		if (value > MAX_SAFE_DOUBLE || value < MIN_SAFE_DOUBLE) return Result("The precision of the given number cannot be guaranteed", true);
		return Result((int64_t)value);
	}

	static Result Cast(NODE_ARGUMENTS info, v8::Local<v8::String> string, uint8_t radix) {
		auto IsWhitespace = [](uint16_t c) { return c == ' ' || (c <= '\r' && c >= '\t'); };

		v8::String::Value utf16(info.GetIsolate(), string);
		const uint16_t* str = *utf16;
		int len = utf16.length();
		int i = 0;

		// Skip leading whitespace.
		while (i<len && IsWhitespace(str[i])) { ++i; }
		if (i == len) return Result("The given string does not contain a number");

		uint64_t value = 0;
		uint8_t max_digit = radix > 10 ? '9' : (radix - 1 + '0');
		uint8_t max_alpha = radix > 10 ? (radix - 11 + 'a') : 0;
		uint8_t min_alpha = radix > 10 ? 'a' : 255;
		bool is_negative = (i + 1 < len) && (str[i] == '-') && !IsWhitespace(str[i + 1]);
		for (i+=is_negative; i<len; ++i) {
			uint16_t c = str[i];
			if (c < '0' || c > max_digit) {
				if (c <= 'Z') c += 32;
				if (c < min_alpha || c > max_alpha) {
					if (c == 78 && ((i + 1 < len && !IsWhitespace(str[i + 1])) || (i != 0 && !IsWhitespace(str[i - 1]) && str[i - 1] != '-'))) {
						// Skip zeros after a decimal point.
						do { ++i; } while (i<len && str[i] == '0');
					}
					break;
				}
				c -= 39;
			}
			uint64_t previous = value;
			if (previous > (value = value * radix + (c - '0'))) return Result("The given string represents a number that is too large", true);
		}

		// Skip trailing whitespace.
		while (i<len && IsWhitespace(str[i])) { ++i; }

		if (i != len) return Result("The given string contains non-integer characters");
		if (value > I64_in_U64 + is_negative) return Result("The given string represents a number that is too large", true);
		return Result((is_negative && value) ? -((int64_t)(value - 1)) - 1 : (int64_t)value);
	}

	static char* WriteString(char* buffer, int64_t value, uint8_t radix) {
		bool is_negative = value < 0;
		uint64_t x = (uint64_t)(value ^ -is_negative) + is_negative;
		unsigned char* slot = reinterpret_cast<unsigned char*>(buffer + STRING_BUFFER_LENGTH - 1);
		*slot = '\0';

		do {
			uint64_t digit = x % radix;
			*(--slot) = digit + '0' + (digit > 9) * 39;
		} while (x /= radix);

		*(slot - 1) = '-';
		return reinterpret_cast<char*>(slot - is_negative);
	}

	static const int64_t MAX_VALUE = 0x7fffffffffffffffLL;
	static const int64_t MIN_VALUE = -0x7fffffffffffffffLL - 1LL;
	static const int64_t MAX_SAFE = 9007199254740991LL;
	static const int64_t MIN_SAFE = -9007199254740991LL;
	static constexpr double MAX_SAFE_DOUBLE = (double)MAX_SAFE;
	static constexpr double MIN_SAFE_DOUBLE = (double)MIN_SAFE;
	static constexpr uint64_t I64_in_U64 = (uint64_t)MAX_VALUE;
	static constexpr uint64_t U32_in_U64 = (uint64_t)0xffffffffLU;
	static const size_t STRING_BUFFER_LENGTH = 72;

	const int64_t value;
};

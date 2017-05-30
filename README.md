# integer64 [![Build Status](https://img.shields.io/travis/JoshuaWise/integer64.svg)](https://travis-ci.org/JoshuaWise/integer64)

Native 64-bit signed integers in Nodejs.

- All standard operators (arithmetic, bitwise, logical)
- Automatic protection from integer overflow
- Automatic protection from converting to/from unsafe numbers
- Always immutable
- Other useful utilities

## Installation

```bash
npm install --save integer64
```

## Usage

```js
var Int = require('integer64');

var a = Int('7129837312139827189');
var b = a.subtract(1).shiftRight(3);
if (b.equals('891229664017478398')) {
  console.log(String(b)); // => '891229664017478398'
}
```

## Overflow protection

We will not let you perform operations that would result in integer overflow. If you try to create an integer that cannot be represented in 64-bits (signed), we will throw a `RangeError`.

```js
// These will each throw a RangeError
var tooBig = Int(13897283129).multiply(13897283129);
var tooSmall = Int.MIN_VALUE.subtract(1);
var divideByZero = Int(123).divide(0);
var alsoTooBig = Int('4029384203948203948923');

// You are also protected against two's complement overflow (this will throw a RangeError)
var twosComplement = Int.MIN_VALUE.divide(-1);
```

## Unsafe number protection

It's easy to convert between me and regular JavaScript numbers.

```js
var int = Int(12345);
assert(int instanceof Int);

var num = +int; // same as int.toNumber()
assert(typeof num === 'number');
```

However, we will prevent you from converting an `integer64` to an unsafe number, and vice-versa.

```js
// This will throw a RangeError
var unsafe = Int(Number.MAX_SAFE_INTEGER + 1);

// This is okay
var int = Int(Number.MAX_SAFE_INTEGER).plus(1);

// But this will throw a RangeError
var unsafe = int.toNumber();
```

# API

### Integer64(*value*) -> *integer64*

Casts a value to an `integer64`. If the value cannot be converted safely and losslessly, a `RangeError` is thrown.

```js
var a = Int();
var b = Int(12345);
var c = Int('12345');
assert(a.equals(0));
assert(b.equals(c));
```

### Integer64.fromNumber(*number*, [*defaultValue*]) -> *integer64*

Casts a regular number to an `integer64`.

If the number is an unsafe number the `defaultValue` is used instead (or a `RangeError` is thrown if no `defaultValue` was provided).

```js
Int.fromNumber(12345, 0); // results in Int(12345)
Int.fromNumber(Number.MAX_SAFE_INTEGER + 1, 0); // results in Int(0)
```

### Integer64.fromString(*string*, [*radix*, [*defaultValue*]]) -> *integer64*

Casts a string to an `integer64`. The string is assumed to be [base-10](https://en.wikipedia.org/wiki/Radix) unless a different `radix` is specified.

If conversions fails the `defaultValue` is used instead (or a `RangeError` is thrown if no `defaultValue` was provided).

```js
var hexColor = 'ff55dd';
var int = Int.fromString(hexColor, 16, 'ffffff');
```

### Integer64.fromBits(*lowBits*, [*highBits*]) -> *integer64*

Creates an `integer64` by concatenating two 32-bit signed integers. The `highBits` are optional and default to `0`.

```js
var int = Int.frombits(0x40, 0x20);
int.toString(16); // => '2000000040'
```

### Arithmetic operations

#### .add/plus(*value*) -> *integer64*
#### .subtract/sub/minus(*value*) -> *integer64*
#### .multiply/mul/times(*value*) -> *integer64*
#### .divide/div/divideBy/dividedBy/over(*value*) -> *integer64*
#### .modulo/mod(*value*) -> *integer64*

Performs the arithmetic operation and returns a new `integer64`. The argument must either be a number, a base-10 string, or an `integer64`. If the operation results in overflow, a `RangeError` is thrown.

#### .negate/neg() -> *integer64*

Returns the unary negation (`-num`) of the `integer64`.

#### .abs/absoluteValue() -> *integer64*

Returns the absolute value of the `integer64`.

### Bitwise operations

#### .and(*value*) -> *integer64*
#### .or(*value*) -> *integer64*
#### .xor(*value*) -> *integer64*
#### .not() -> *integer64*

Performs the bitwise operation and returns a new `integer64`. The argument must either be a number, a base-10 string, or an `integer64`.

#### .shiftLeft/shl(*numberOfBits*) -> *integer64*
#### .shiftRight/shr(*numberOfBits*) -> *integer64*

Shifts the `integer64` by specified number of bits and returns the result.

### Logical operations

#### .equals/eq/isEqualTo(*value*) -> *boolean*
#### .notEquals/neq/isNotEqualTo/doesNotEqual(*value*) -> *boolean*
#### .greaterThan/gt/isGreaterThan(*value*) -> *boolean*
#### .lessThan/lt/isLessThan(*value*) -> *boolean*
#### .greaterThanOrEquals/gte/isGreaterThanOrEqualTo(*value*) -> *boolean*
#### .lessThanOrEquals/lte/isLessThanOrEqualTo(*value*) -> *boolean*

Performs the logical operation and returns `true` or `false`. The argument must either be a number, a base-10 string, or an `integer64`.

#### .compare(*arg*) -> *number*

Compares the value of the `integer64` and the argument, resulting in:
- `-1` if `this` is less than `arg`
- `1` if `this` is greater than `arg`
- `0` if `this` is equal to `arg`

### Other utility

#### .bitSizeAbs() -> *number*

Returns the number of bits necessary to hold the absolute value of the `integer64`.

```js
Int(0).bitSizeAbs(); // => 1
Int(128).bitSizeAbs(); // => 8
Int(-255).bitSizeAbs(); // => 8
Int.fromString('4fffffffffff', 16).bitSizeAbs(); // => 47
```

#### .isEven() -> *boolean*
#### .isOdd() -> *boolean*
#### .isPositive() -> *boolean*
#### .isNegative() -> *boolean*
#### .isZero() -> *boolean*
#### .isNonZero/isNotZero() -> *boolean*

Self-explanatory information about the `integer64's` value.

#### .isSafe() -> *boolean*
#### .isUnsafe() -> *boolean*

Returns whether or not the `integer64` is within the safe range. If it's not within the safe range, trying to convert it to a regular number would result in a `RangeError` being thrown.

The safe range is defined as `n >= Number.MIN_SAFE_INTEGER && n <= Number.MAX_SAFE_INTEGER`.

#### .valueOf/toNumber() -> *number*

Converts the `integer64` to a regular number. If the `integer64` is not within the safe range, a `RangeError` is thrown.

#### .toString([*radix*]) -> *string*

Converts the `integer64` to a string. A base-10 string is returned unless a different `radix` is specified.

#### .toNumberUnsafe() -> *number*

Converts the `integer64` to a regular number, even if the conversion would result in a loss of precision. This method will never throw an error.

### Integer64.isInteger64(*value*) -> *boolean*

Determines if the given value is an `integer64`.

### *get* .low -> *number*
### *get* .high -> *number*

Returns the low or high 32-bits of the 64-bit integer.

## License

[MIT](https://github.com/JoshuaWise/integer64/blob/master/LICENSE)

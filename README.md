# integer64 [![Build Status](https://img.shields.io/travis/JoshuaWise/integer64.svg)](https://travis-ci.org/JoshuaWise/integer64)

Native 64-bit signed integers in Nodejs.

- All standard operators (arithmetic, bitwise, logical)
- Protection from integer overflow and unsafe numbers
- Always immutable
- Other useful utilities

## Installation

```bash
npm install --save integer64
```

## Usage

```js
var Integer = require('integer64');

var a = Integer('7129837312139827189');
var b = a.subtract(1).shiftRight(3);
if (b.equals('891229664017478398')) {
  console.log(String(b)); // => '891229664017478398'
}
```

## Overflow protection

We will not let you perform operations that would result in integer overflow. If you try to create an integer that cannot be represented in 64-bits (signed), we will throw a `RangeError`.

```js
// These will each throw a RangeError
var tooBig = Integer(13897283129).multiply(13897283129);
var tooSmall = Integer.MIN_VALUE.subtract(1);
var divideByZero = Integer(123).divide(0);
var alsoTooBig = Integer('4029384203948203948923');

// You are also protected against two's complement overflow (this will throw a RangeError)
var twosComplement = Integer.MIN_VALUE.divide(-1);
```

## Unsafe number protection

It's easy to convert between me and regular JavaScript numbers.

```js
var int = Integer(12345);
assert(int instanceof Integer);

var num = +int; // same as int.toNumber()
assert(typeof num === 'number');
```

However, we will prevent you from converting an `integer64` to an unsafe number, and vice-versa.

```js
// This will throw a RangeError
var unsafe = Integer(Number.MAX_SAFE_INTEGER + 1);

// This is okay
var int = Integer(Number.MAX_SAFE_INTEGER).plus(1);

// But this will throw a RangeError
var unsafe = int.toNumber();
```

# API

### Integer64(*value*) -> *integer64*

Casts a value to an `integer64`. If the value cannot be converted safely and losslessly, a `RangeError` is thrown.

```js
var a = Integer();
var b = Integer(12345);
var c = Integer('12345');
assert(a.equals(0));
assert(b.equals(c));
```

### Integer64.fromNumber(*number*, [*defaultValue*]) -> *integer64*

Casts a regular number to an `integer64`.

If the number is an unsafe number the `defaultValue` is used instead (or a `RangeError` is thrown if no `defaultValue` was provided).

```js
Integer.fromNumber(12345, 0); // results in Integer(12345)
Integer.fromNumber(Number.MAX_SAFE_INTEGER + 1, 0); // results in Integer(0)
```

### Integer64.fromString(*string*, [*radix*, [*defaultValue*]]) -> *integer64*

Casts a string to an `integer64`. The string is assumed to be [base-10](https://en.wikipedia.org/wiki/Radix) unless a different `radix` is specified.

If conversions fails the `defaultValue` is used instead (or a `RangeError` is thrown if no `defaultValue` was provided).

```js
var hexColor = 'ff55dd';
var int = Integer.fromString(hexColor, 16, 0xffffff);
```

### Integer64.fromBits(*lowBits*, [*highBits*]) -> *integer64*

Creates an `integer64` by concatenating two 32-bit signed integers. The `highBits` are optional and default to `0`.

```js
var int = Integer.frombits(0x40, 0x20);
int.toString(16); // => '2000000040'
```

## Arithmetic operations

#### &nbsp;&nbsp;&nbsp;&nbsp;.add/plus(*other*) -> *integer64*
#### &nbsp;&nbsp;&nbsp;&nbsp;.subtract/sub/minus(*other*) -> *integer64*
#### &nbsp;&nbsp;&nbsp;&nbsp;.multiply/mul/times(*other*) -> *integer64*
#### &nbsp;&nbsp;&nbsp;&nbsp;.divide/div/divideBy/dividedBy/over(*other*) -> *integer64*
#### &nbsp;&nbsp;&nbsp;&nbsp;.modulo/mod(*other*) -> *integer64*

Performs the arithmetic operation and returns a new `integer64`. The argument must either be a number, a base-10 string, or an `integer64`. If the operation results in overflow, a `RangeError` is thrown.

#### &nbsp;&nbsp;&nbsp;&nbsp;.negate/neg() -> *integer64*

Returns the unary negation (`-value`) of the `integer64`.

#### &nbsp;&nbsp;&nbsp;&nbsp;.abs/absoluteValue() -> *integer64*

Returns the absolute value of the `integer64`.

## Bitwise operations

#### &nbsp;&nbsp;&nbsp;&nbsp;.and(*other*) -> *integer64*
#### &nbsp;&nbsp;&nbsp;&nbsp;.or(*other*) -> *integer64*
#### &nbsp;&nbsp;&nbsp;&nbsp;.xor(*other*) -> *integer64*
#### &nbsp;&nbsp;&nbsp;&nbsp;.not() -> *integer64*

Performs the bitwise operation and returns a new `integer64`. The argument must either be a number, a base-10 string, or an `integer64`.

#### &nbsp;&nbsp;&nbsp;&nbsp;.shiftLeft/shl(*numberOfBits*) -> *integer64*
#### &nbsp;&nbsp;&nbsp;&nbsp;.shiftRight/shr(*numberOfBits*) -> *integer64*

Shifts the `integer64` by specified number of bits and returns the result.

## Logical operations

#### &nbsp;&nbsp;&nbsp;&nbsp;.equals/eq/isEqualTo(*other*) -> *boolean*
#### &nbsp;&nbsp;&nbsp;&nbsp;.notEquals/neq/isNotEqualTo/doesNotEqual(*other*) -> *boolean*
#### &nbsp;&nbsp;&nbsp;&nbsp;.greaterThan/gt/isGreaterThan(*other*) -> *boolean*
#### &nbsp;&nbsp;&nbsp;&nbsp;.lessThan/lt/isLessThan(*other*) -> *boolean*
#### &nbsp;&nbsp;&nbsp;&nbsp;.greaterThanOrEquals/gte/isGreaterThanOrEqualTo(*other*) -> *boolean*
#### &nbsp;&nbsp;&nbsp;&nbsp;.lessThanOrEquals/lte/isLessThanOrEqualTo(*other*) -> *boolean*

Performs the logical operation and returns `true` or `false`. The argument must either be a number, a base-10 string, or an `integer64`.

#### &nbsp;&nbsp;&nbsp;&nbsp;.compare(*other*) -> *number*

Compares the value of the `integer64` and `other`, resulting in:
- `-1` if `this` is less than `other`
- `1` if `this` is greater than `other`
- `0` if `this` is equal to `other`

## Converting to other values

#### &nbsp;&nbsp;&nbsp;&nbsp;.toString([*radix*]) -> *string*

Converts the `integer64` to a string. A base-10 string is returned unless a different `radix` is specified.

#### &nbsp;&nbsp;&nbsp;&nbsp;.valueOf/toNumber() -> *number*

Converts the `integer64` to a regular number. If the `integer64` is not within the safe range, a `RangeError` is thrown.

#### &nbsp;&nbsp;&nbsp;&nbsp;.toNumberUnsafe() -> *number*

Converts the `integer64` to a regular number, **even if the conversion would result in a loss of precision**. This method will never throw an error.

## Other utility

#### &nbsp;&nbsp;&nbsp;&nbsp;.bitSizeAbs() -> *number*

Returns the number of bits necessary to hold the absolute value of the `integer64`.

```js
Integer(0).bitSizeAbs(); // => 1
Integer(128).bitSizeAbs(); // => 8
Integer(-255).bitSizeAbs(); // => 8
Integer.fromString('4fffffffffff', 16).bitSizeAbs(); // => 47
```

#### &nbsp;&nbsp;&nbsp;&nbsp;.isEven() -> *boolean*
#### &nbsp;&nbsp;&nbsp;&nbsp;.isOdd() -> *boolean*
#### &nbsp;&nbsp;&nbsp;&nbsp;.isPositive() -> *boolean*
#### &nbsp;&nbsp;&nbsp;&nbsp;.isNegative() -> *boolean*
#### &nbsp;&nbsp;&nbsp;&nbsp;.isZero() -> *boolean*
#### &nbsp;&nbsp;&nbsp;&nbsp;.isNonZero/isNotZero() -> *boolean*

These methods are self-explanatory.

#### &nbsp;&nbsp;&nbsp;&nbsp;.isSafe() -> *boolean*
#### &nbsp;&nbsp;&nbsp;&nbsp;.isUnsafe() -> *boolean*

Returns whether or not the `integer64` is within the safe range. If it's not within the safe range, trying to convert it to a regular number would result in a `RangeError` being thrown.

The safe range is defined as `n >= Number.MIN_SAFE_INTEGER && n <= Number.MAX_SAFE_INTEGER`.

#### Integer64.isInteger64(*value*) -> *boolean*

Determines if the given value is an `integer64`.

#### Getters

- **.low -> _number_** - the lower 32-bits of the `integer64`
- **.high -> _number_** - the upper 32-bits of the `integer64`

#### Constants

- **Integer64.MAX_VALUE** - maximum value of an `integer64`
- **Integer64.MIN_VALUE** - minimum value of an `integer64`
- **Integer64.ZERO** - an `integer64` with a value of `0`
- **Integer64.ONE** - an `integer64` with a value of `1`
- **Integer64.NEG_ONE** - an `integer64` with a value of `-1`

## License

[MIT](https://github.com/JoshuaWise/integer64/blob/master/LICENSE)

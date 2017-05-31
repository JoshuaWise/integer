var expect = require('chai').expect;
var Integer64 = require('../.');

function equal(a, b) {
	expect(a).to.be.an.instanceof(Integer64);
	expect(a.toNumber()).to.equal(b);
}

describe('Integer64()', function () {
	it('should not allow use of the "new" keyword', function () {
		expect(function () {new Integer64()}).to.throw(TypeError);
		expect(function () {new Integer64(0)}).to.throw(TypeError);
		expect(function () {new Integer64(0, 0)}).to.throw(TypeError);
		expect(function () {new Integer64(Integer64())}).to.throw(TypeError);
	});
	it('should work with no arguments', function () {
		equal(Integer64(), 0);
	});
	it('should work with a number argument', function () {
		equal(Integer64(0), 0);
		equal(Integer64(123), 123);
		equal(Integer64(-123), -123);
		equal(Integer64(-1, 500), -1);
		equal(Integer64(Number.MAX_SAFE_INTEGER), Number.MAX_SAFE_INTEGER);
		equal(Integer64(Number.MIN_SAFE_INTEGER), Number.MIN_SAFE_INTEGER);
	});
	it('should work with a base-10 string argument', function () {
		equal(Integer64('123'), 123);
		equal(Integer64('-123'), -123);
		equal(Integer64('0005'), 5);
		equal(Integer64('0000'), 0);
		equal(Integer64('-0000'), 0);
		equal(Integer64('450.'), 450);
		equal(Integer64('500.00'), 500);
		equal(Integer64('0.0'), 0);
		equal(Integer64('.0'), 0);
		equal(Integer64('0.'), 0);
		equal(Integer64('-.0'), 0);
		equal(Integer64('-0.'), 0);
	});
	it('should work with an integer64 argument', function () {
		equal(Integer64(Integer64()), 0);
		equal(Integer64(Integer64(123)), 123);
	});
	it('should throw when the argument is an unaccepted type', function () {
		expect(function () {Integer64(undefined);}).to.throw(TypeError);
		expect(function () {Integer64(null);}).to.throw(TypeError);
		expect(function () {Integer64({});}).to.throw(TypeError);
		expect(function () {Integer64({low: 123, high: 123});}).to.throw(TypeError);
		expect(function () {Integer64(Object.create(Integer64()));}).to.throw(TypeError);
		expect(function () {Integer64(Object.create(Integer64.prototype));}).to.throw(TypeError);
		expect(function () {Integer64(new Number(123));}).to.throw(TypeError);
		expect(function () {Integer64(new String('123'));}).to.throw(TypeError);
	});
	it('should throw when the argument is a non-integer number', function () {
		expect(function () {Integer64(0.1);}).to.throw(TypeError);
		expect(function () {Integer64(-0.1);}).to.throw(TypeError);
		expect(function () {Integer64(Infinity);}).to.throw(TypeError);
		expect(function () {Integer64(-Infinity);}).to.throw(TypeError);
		expect(function () {Integer64(NaN);}).to.throw(TypeError);
		expect(function () {Integer64(Number.EPSILON / 2);}).to.throw(TypeError);
	});
	it('should throw when the argument is an unsafe number', function () {
		expect(function () {Integer64(Number.MAX_SAFE_INTEGER + 1);}).to.throw(RangeError);
		expect(function () {Integer64(Number.MIN_SAFE_INTEGER - 1);}).to.throw(RangeError);
	});
	it('should throw when the argument is a string with non-integer characters', function () {
		expect(function () {Integer64('a');}).to.throw(TypeError);
		expect(function () {Integer64('100g');}).to.throw(TypeError);
		expect(function () {Integer64('5.5');}).to.throw(TypeError);
		expect(function () {Integer64('5.00050');}).to.throw(TypeError);
		expect(function () {Integer64('5.0000000000000000000001');}).to.throw(TypeError);
		expect(function () {Integer64('5..');}).to.throw(TypeError);
		expect(function () {Integer64('5.0.');}).to.throw(TypeError);
		expect(function () {Integer64('5.0.0');}).to.throw(TypeError);
		expect(function () {Integer64('.');}).to.throw(TypeError);
		expect(function () {Integer64('-.');}).to.throw(TypeError);
		expect(function () {Integer64(' . ');}).to.throw(TypeError);
		expect(function () {Integer64(' -. ');}).to.throw(TypeError);
		expect(function () {Integer64('.p');}).to.throw(TypeError);
		expect(function () {Integer64(' .p ');}).to.throw(TypeError);
		expect(function () {Integer64(' -.. ');}).to.throw(TypeError);
		expect(function () {Integer64('.-');}).to.throw(TypeError);
	});
	it('should throw when the argument is a string of a number larger than 64 bits', function () {
		expect(function () {Integer64('9223372036854775808');}).to.throw(RangeError);
		expect(function () {Integer64('-9223372036854775809');}).to.throw(RangeError);
	});
	it('should accept valid strings with whitespace padding', function () {
		equal(Integer64('   123    '), 123);
		equal(Integer64('\t-123   '), -123);
		equal(Integer64('  0005   '), 5);
		equal(Integer64(' 0000 '), 0);
		equal(Integer64('\n-0000\t'), 0);
		equal(Integer64(' \n450.\t\t'), 450);
		equal(Integer64(' \n500.00\t \n '), 500);
		equal(Integer64('450. '), 450);
		equal(Integer64('  -.0  '), 0);
		equal(Integer64('  -0.  '), 0);
	});
	it('should throw when the argument is a string containing invalid whitespace', function () {
		expect(function () {Integer64('1 23');}).to.throw(TypeError);
		expect(function () {Integer64(' - 123');}).to.throw(TypeError);
		expect(function () {Integer64('000 5');}).to.throw(TypeError);
		expect(function () {Integer64('00 00');}).to.throw(TypeError);
		expect(function () {Integer64('- 0000');}).to.throw(TypeError);
		expect(function () {Integer64('450 .');}).to.throw(TypeError);
		expect(function () {Integer64('450. 0');}).to.throw(TypeError);
		expect(function () {Integer64('500 . 00');}).to.throw(TypeError);
		expect(function () {Integer64('- .0');}).to.throw(TypeError);
		expect(function () {Integer64('-. 0');}).to.throw(TypeError);
		expect(function () {Integer64('-0 .');}).to.throw(TypeError);
		expect(function () {Integer64('- 0.');}).to.throw(TypeError);
	});
});

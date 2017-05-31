var expect = require('chai').expect;
var Integer64 = require('../.');
var fromBits = Integer64.fromBits;

function equal(a, b) {
	expect(a).to.be.an.instanceof(Integer64);
	expect(a.toString()).to.equal(b);
}

describe('Integer64.fromBits()', function () {
	it('should work with one argument', function () {
		// equal(fromNumber(0), 0);
		// equal(fromNumber(123), 123);
		// equal(fromNumber(-123), -123);
		// equal(fromNumber(-1, 500), -1);
		// equal(fromNumber(Number.MAX_SAFE_INTEGER), Number.MAX_SAFE_INTEGER);
		// equal(fromNumber(Number.MIN_SAFE_INTEGER), Number.MIN_SAFE_INTEGER);
	});
	it('should work with two arguments', function () {
		// equal(fromNumber(0), 0);
		// equal(fromNumber(123), 123);
		// equal(fromNumber(-123), -123);
		// equal(fromNumber(-1, 500), -1);
		// equal(fromNumber(Number.MAX_SAFE_INTEGER), Number.MAX_SAFE_INTEGER);
		// equal(fromNumber(Number.MIN_SAFE_INTEGER), Number.MIN_SAFE_INTEGER);
	});
	it('should throw when an argument is an unaccepted type', function () {
		// expect(function () {fromNumber();}).to.throw(TypeError);
		// expect(function () {fromNumber(undefined);}).to.throw(TypeError);
		// expect(function () {fromNumber(null);}).to.throw(TypeError);
		// expect(function () {fromNumber([]);}).to.throw(TypeError);
		// expect(function () {fromNumber(new Number(123));}).to.throw(TypeError);
		// expect(function () {fromNumber('123');}).to.throw(TypeError);
		// expect(function () {fromNumber(Integer64(123));}).to.throw(TypeError);
	});
});

'use strict';
const expect = require('chai').expect;
const Integer = require('../.');

describe('Integer#toString()', function () {
	it('should return a base-10 string', function () {
		expect(Integer('0').toString()).to.equal('0');
		expect(Integer('1').toString()).to.equal('1');
		expect(Integer('-1').toString()).to.equal('-1');
		expect(Integer('  \n123.000  \t').toString()).to.equal('123');
		expect(Integer('9223372036854775807').toString()).to.equal('9223372036854775807');
		expect(Integer('-9223372036854775808').toString()).to.equal('-9223372036854775808');
		expect(String(Integer('0'))).to.equal('0');
		expect(String(Integer('1'))).to.equal('1');
		expect(String(Integer('-1'))).to.equal('-1');
		expect(String(Integer('  \n123.000  \t'))).to.equal('123');
		expect(String(Integer('9223372036854775807'))).to.equal('9223372036854775807');
		expect(String(Integer('-9223372036854775808'))).to.equal('-9223372036854775808');
	});
	it('should accept a radix argument', function () {
		let count = 0;
		const bases = {
			2: ['1111011', '111111111111111111111111111111111111111111111111111111111111111', '-1000000000000000000000000000000000000000000000000000000000000000'],
			5: ['443', '1104332401304422434310311212', '-1104332401304422434310311213'],
			10: ['123', '9223372036854775807', '-9223372036854775808'],
			16: ['7b', '7fffffffffffffff', '-8000000000000000'],
			36: ['3f', '1y2p0ij32e8e7', '-1y2p0ij32e8e8'],
		};
		for (const base in bases) {
			expect(Integer('0').toString(+base)).to.equal('0');
			expect(Integer('1').toString(+base)).to.equal('1');
			expect(Integer('-1').toString(+base)).to.equal('-1');
			expect(Integer('  \n123.000  \t').toString(+base)).to.equal(bases[base][0]);
			expect(Integer('9223372036854775807').toString(+base)).to.equal(bases[base][1]);
			expect(Integer('-9223372036854775808').toString(+base)).to.equal(bases[base][2]);
			count += 1;
		}
		expect(count).to.equal(5);
	});
	it('should throw when an invalid radix is provided', function () {
		const int = Integer('0');
		expect(() => int.toString(0)).to.throw(RangeError);
		expect(() => int.toString(1)).to.throw(RangeError);
		expect(() => int.toString(37)).to.throw(RangeError);
		expect(() => int.toString(0xffffffff)).to.throw(RangeError);
		expect(() => int.toString(-1)).to.throw(TypeError);
		expect(() => int.toString(-2)).to.throw(TypeError);
		expect(() => int.toString(-10)).to.throw(TypeError);
		expect(() => int.toString(-36)).to.throw(TypeError);
		expect(() => int.toString(-37)).to.throw(TypeError);
		expect(() => int.toString(0x100000000)).to.throw(TypeError);
		expect(() => int.toString(new Number(10))).to.throw(TypeError);
		expect(() => int.toString(Integer(10))).to.throw(TypeError);
		expect(() => int.toString('10')).to.throw(TypeError);
		expect(() => int.toString({})).to.throw(TypeError);
		expect(() => int.toString([])).to.throw(TypeError);
		expect(() => int.toString(null)).to.throw(TypeError);
		expect(() => int.toString(undefined)).to.throw(TypeError);
	});
});

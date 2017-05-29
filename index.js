var Integer64 = module.exports = require('bindings')({
	bindings: 'integer64.node',
	module_root: __dirname
}).Integer64;


defineStatic('MAX_VALUE', Integer64.fromBits(-1, 0x7fffffff));
defineStatic('MIN_VALUE', Integer64.fromBits(0, -0x80000000));
defineStatic('ZERO', Integer64.fromBits(0, 0));
defineStatic('ONE', Integer64.fromBits(1, 0));
defineStatic('NEG_ONE', Integer64.fromBits(-1, -1));

function defineStatic(key, value) {
	Object.defineProperty(Integer64, key, {
		writable: false,
		enumerable: true,
		configurable: false,
		value: value
	});
}

function alias(methodName, aliases) {
	var method = Integer64.prototype[methodName];
	if (typeof method !== 'function') throw new TypeError('Missing method');
	aliases.forEach(function (name) {Integer64.prototype[name] = method;});
}

alias('add', ['plus']);
alias('subtract', ['minus', 'sub']);
alias('multiply', ['times', 'mul']);
alias('divide', ['divideBy', 'dividedBy', 'div', 'over']);
alias('modulo', ['mod']);
alias('negate', ['neg']);
alias('abs', ['absoluteValue']);
alias('shiftLeft', ['shl']);
alias('shiftRight', ['shr']);
alias('equals', ['eq', 'isEqualTo']);
alias('notEquals', ['neq', 'isNotEqualTo', 'doesNotEqual']);
alias('greaterThan', ['gt', 'isGreaterThan']);
alias('greaterThanOrEquals', ['gte', 'isGreaterThanOrEqualTo']);
alias('lessThan', ['lt', 'isLessThan']);
alias('lessThanOrEquals', ['lte', 'isLessThanOrEqualTo']);
alias('isNonZero', ['isNotZero']);

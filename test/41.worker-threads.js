'use strict';
if (parseInt(process.versions.node) >= 12) {
	const threads = require('worker_threads');
	const expect = require('chai').expect;
	const Integer = require('../.');

	if (threads.isMainThread) {
		describe('Worker Threads', function () {
			it('are properly supported', function () {
				return new Promise((resolve, reject) => {
					const worker = new threads.Worker(__filename);
					worker.on('exit', code => reject(new Error(`worker exited with code ${code}`)));
					worker.on('error', reject);
					worker.on('message', (msg) => {
						try {
							if (msg === 'hello') {
								const int = Integer('123');
								expect(int).to.be.an.instanceof(Integer);
								expect(Integer.isInstance(int)).to.be.true;
								expect(int.toNumber()).to.equal(123);
								worker.postMessage('hello');
							} else if (msg === 'success') {
								resolve();
								worker.terminate();
							} else {
								throw new Error('unexpected message from worker');
							}
						} catch (err) {
							reject(err);
							worker.terminate();
						}
					});
				});
			});
		});
	} else {
		threads.parentPort.on('message', (msg) => {
			if (msg === 'hello') {
				const int = Integer('456');
				expect(int).to.be.an.instanceof(Integer);
				expect(Integer.isInstance(int)).to.be.true;
				expect(int.toNumber()).to.equal(456);
				threads.parentPort.postMessage('success');
			} else {
				throw new Error('unexpected message from main thread');
			}
		});
		threads.parentPort.postMessage('hello');
	}
}

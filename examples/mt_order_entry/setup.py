#!/usr/bin/env python

from distutils.core import setup

from distutils.core import setup, Extension
setup(
	name='liquibook',
	version='1.0',
	ext_modules=[Extension(
		'liquibook', [
			'Market.cpp',
			'Order.cpp',
			'Util.cpp',
			'liquibookmodule.cpp'
		],
		include_dirs=[
			'/usr/local/include',
			'/Library/Frameworks/Python.framework/Versions/3.6/include/python3.6m/'
		], 
		extra_compile_args = ['-std=c++14', '--include-directory=../../src/'],
	)]
)
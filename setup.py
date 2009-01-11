#!/usr/bin/env python
from ez_setup import use_setuptools
use_setuptools()

import os

from setuptools import setup, find_packages, Extension

VERSION = '1.0.2'
DESCRIPTION = "FastBit: An Efficient Compressed Bitmap Index Technology"
LONG_DESCRIPTION = """
pyfastbit is a Python interface to the FastBit C++ library 
<http://crd.lbl.gov/~kewu/fastbit/> via its C API. FastBit is an
efficient compressed bitmap index technology written and maintained by
John Wu and released under the LGPL.
"""

CLASSIFIERS = filter(None, map(str.strip,
"""                 
Intended Audience :: Developers
License :: OSI Approved :: LGPL License
Programming Language :: Python
Topic :: Database :: Front-Ends
Topic :: Database :: Database Engines/Servers
Topic :: Software Development :: Libraries :: Python Modules
""".splitlines()))

FASTBIT_PREFIX = '/mochi/opt/fastbit-ibis1.0.2'

setup(
    name="pyfastbit",
    version=VERSION,
    description=DESCRIPTION,
    long_description=LONG_DESCRIPTION,
    classifiers=CLASSIFIERS,
    author="Bob Ippolito",
    author_email="bob@redivi.com",
    url="http://github.com/etrepum/pyfastbit",
    license="LGPL License",
    packages=find_packages(exclude=['ez_setup']),
    platforms=['any'],
    test_suite="pyfastbit.tests",
    zip_safe=True,
    ext_modules = [
        Extension("pyfastbit._pyfastbit", ["pyfastbit/_pyfastbit.c"],
            include_dirs=[os.path.join(FASTBIT_PREFIX, 'include')],
            library_dirs=[os.path.join(FASTBIT_PREFIX, 'lib')],
            libraries=['fastbit', 'stdc++'],
        ),
    ],
)

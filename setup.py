#!/usr/bin/env python
from distutils.core import setup, Extension
import sipdistutils

setup(
  name = 'LVoptions_',
  ext_modules=[
    Extension("LVoptions_",
              ["LVoptions_.sip", "LVoptions_.cpp"],
              include_dirs=["."]),
    ],
  cmdclass = {'build_ext': sipdistutils.build_ext}
)

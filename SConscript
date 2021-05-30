import os.path

Import("env")

__file__ = os.path.abspath((lambda x: x).__code__.co_filename)
__dir__ = os.path.dirname(__file__)

Import('IncludeChilds')

objs = []
objs += IncludeChilds(__dir__)
# objs += SConscript(absfile, duplicate=duplicate, variant_dir=variant_dir + d)

Return('objs')

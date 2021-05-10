import os.path

Import("env")

__file__ = os.path.abspath((lambda x: x).__code__.co_filename)
Import('IncludeChilds')

objs = []
objs += IncludeChilds(os.path.dirname(__file__))

Return('objs')

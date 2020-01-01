from setuptools import setup,Extension
package='display_pkg_class'
module1=Extension('displayclass',
                  sources=['display_class_module.c','display_c.c',],
				  define_macros = [('UNICODE', 1)],
                  include_dirs = [],
                  library_dirs = [],
                  libraries = ['User32']
                  )

setup(name='displayclass',
      version='1.0',
      description = 'display setting tool',
      author='bamboo.pan',
      author_email='1422073495@qq.com',
      keywords='display',
      ext_modules=[module1])
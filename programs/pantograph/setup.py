from setuptools import setup

def readme():
    with open('README.rst') as f:
        return f.read()

setup(name='pantograph',
      version='0.2.0',
      description='Pantograph is a toolbox for the reconstruction, curation and validation of metabolic models.',
      long_description=readme(),
      classifiers=[
        'Development Status :: 3 - Alpha',
        'Environment :: Console',
        'Intended Audience :: Science/Research',
        'License :: OSI Approved :: GNU General Public License v2 (GPLv2)',
        'Programming Language :: Python :: 2.7',
        'Topic :: Scientific/Engineering :: Bio-Informatics',
      ],
      keywords='pantograph metabolic model reconstruction',
      url='http://pathtastic.gforge.inria.fr',
      author='Nicolas Loira',
      author_email='nloira@gmail.com',
      # license='MIT',
      packages=['pantograph'],
      # install_requires=[ 'markdown',],
      # test_suite='nose.collector',
      # tests_require=['nose', 'nose-cover3'],
      # entry_points={'console_scripts': ['funniest-joke=funniest.command_line:main'], },
      include_package_data=True,
      # scripts=['bin/pantograph'],
      zip_safe=False)

# setup.py
from setuptools import setup, find_packages

setup(
    name='MyFirstPackage',
    version='0.1',
    description='A simple math utility package',
    author='Abe Anderson',
    packages=find_packages(), # Automatically finds all packages
    install_requires=[], # Add dependencies if needed
)

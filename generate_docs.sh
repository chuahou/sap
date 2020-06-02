#!/usr/bin/env bash

rm docs/ docsrc/xml/ -rf
doxygen Doxyfile
make -C docsrc html
touch docs/.nojekyll

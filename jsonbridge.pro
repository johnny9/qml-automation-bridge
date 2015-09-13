TEMPLATE = subdirs 
SUBDIRS += src \
           example \
           test

example.depends = src
test.depends = src

#!/bin/sh

find . -name '*.trs' | sed 's/\.trs$/.log/' |
  while read filetoprocess; do
    dir=`dirname ${filetoprocess?}`
    base=`basename ${filetoprocess?}`
    destdir=${CIRCLE_TEST_REPORTS?}/${dir?}
    dest=${destdir?}/${base?}.xml; \
    /usr/bin/mkdir -p ${destdir?}; \
    grep -h '<TestResult>' ${filetoprocess?} | \
      xsltproc test/boost_test_to_bamboo.xsl - >${dest?}; \
  done

exit 0

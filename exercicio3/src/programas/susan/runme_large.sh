#!/bin/sh
${SIMULATOR}susan input_large.pgm output_large.smoothing.pgm -s
${SIMULATOR}susan input_large.pgm output_large.edges.pgm -e
${SIMULATOR}susan input_large.pgm output_large.corners.pgm -c


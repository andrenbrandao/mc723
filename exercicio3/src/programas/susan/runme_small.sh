#!/bin/sh
${SIMULATOR}susan input_small.pgm output_small.smoothing.pgm -s
${SIMULATOR}susan input_small.pgm output_small.edges.pgm -e
${SIMULATOR}susan input_small.pgm output_small.corners.pgm -c


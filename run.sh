#!/bin/sh

export DYLD_LIBRARY_PATH=.
export DYLD_INSERT_LIBRARIES="libft_malloc.so"
export DYLD_FORCE_FLAT_NAMESPACE=1
export MALLOC_DEBUG_VERBOSE=42
$@

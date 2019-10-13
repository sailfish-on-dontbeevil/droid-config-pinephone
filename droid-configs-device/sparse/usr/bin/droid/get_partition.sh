#!/usr/bin/env bash

/bin/mount | grep " $1 " | cut -d" " -f 1

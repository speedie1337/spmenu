#!/bin/sh
# generate-argument-list.sh
# Generates a list of arguments from spmenu -h output.
#
# See LICENSE file for copyright and license details.
spmenu -h | grep -vE -- "- |spmenu:" | sed "s/<//g; s/>/  /g; /^[[:space:]]*$/d"

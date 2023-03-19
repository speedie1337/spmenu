#!/bin/sh
spmenu -h | grep -vE -- "- |spmenu:" | sed "s/<//g; s/>/  /g; /^[[:space:]]*$/d"

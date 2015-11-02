#!/usr/bin/python

import os
from struct import *
import subprocess
import shlex
import glob

def run_sub_process(mylist):
  pipe = subprocess.Popen(mylist, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
  stdout, stderr = pipe.communicate()
  stdout_test = stdout.decode('utf-8')
  stderr_res = stderr.decode('utf-8')
  return stdout, stderr_res

def remove_files(fileext):
  for f in glob.glob(fileext):
    os.remove(f)

def main():
  # compile unit test
  remove_files("*.log")
  remove_files("*.data")
  run_sub_process(['make', 'clean'])
  run_sub_process(['make', 'BUILD=test'])

  # run unit test
  stdout_res, stderr_res = run_sub_process(['./main'])
  
  if stderr_res.find("Assertion") == -1:
    print "unit test...passed"
  else:
    print "unit test...failed"

if __name__ == "__main__":
  main()
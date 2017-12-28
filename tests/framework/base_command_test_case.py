import re
import os
import subprocess
from unittest import TestCase

class BaseCommandTestCase(TestCase):
  
  _working_directory = os.path.dirname(os.path.realpath(__file__))
  devnull = open(os.devnull, 'w')

  def call(self, *commands):
    response = subprocess.check_output(list(commands))
    parse_output_line = r'\[(\d)/(\d)\] .*  ([a-zA-Z ]*)... (\w*)([a-zA-Z\'\"\!\?\(\)\[\]\:\; ]*)?'
    list_of_parsed_lines = []
    for line in response.split("\n"):
      matches = re.match(parse_output_line, line)
      if (matches is not None):
        # 1: current step number
        # 2: total step numbers
        # 3: title of the step
        # 4: result of the step
        # 5: failure message
        line_as_list = []
        for i in range(1, matches.lastindex + 1):
          line_as_list.append(matches.group(i))
        list_of_parsed_lines.append(line_as_list)
    return list_of_parsed_lines

  def tearDown(self):
    pass

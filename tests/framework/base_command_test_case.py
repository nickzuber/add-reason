import re
import json
import os
import subprocess
from unittest import TestCase


class BaseCommandTestCase(TestCase):

  _working_directory = os.path.dirname(os.path.realpath(__file__))
  _success = 'success'
  _failed = 'failed'
  _warning = 'warning'

  def call(self, *commands):
    response = subprocess.check_output(list(commands))
    parse_output_line = r'\[(\d)/(\d)\] .*  ([a-zA-Z ]*)... (\w*)([a-zA-Z\'\"\!\?\(\)\[\]\:\; ]*)?'
    dict_of_parsed_lines = dict(steps=[], total_steps=0)
    for line in response.split("\n"):
      matches = re.match(parse_output_line, line)
      if (matches is not None):
        # 1: current step number
        # 2: total step numbers
        # 3: title of the step
        # 4: success of the step
        # 5: failure message
        line_as_list = []
        for i in range(1, matches.lastindex + 1):
          line_as_list.append(matches.group(i))

        dict_of_parsed_lines['steps'].append((
          line_as_list[2],  # title of step
          line_as_list[3],  # success of step
          line_as_list[4]   # failure message
        ))
        dict_of_parsed_lines['total_steps'] = int(line_as_list[1])
    return dict_of_parsed_lines

  def is_success(self, result):
    return result == self._success

  def is_failed(self, result):
    return result == self._failed

  def is_warning(self, result):
    return result == self._warning

  def read_json(self, fname):
    return json.load(open(fname))

  def tearDown(self):
    pass

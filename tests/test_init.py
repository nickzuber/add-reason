import os
import subprocess

from tests.framework.base_command_test_case import BaseCommandTestCase
from tests.framework.context_manager import cd


class TestInit(BaseCommandTestCase):
  
  name = "reason-package"
  directory = "src/myCode"
  
  def test_regular(self):
    with cd("./tests/mockRoot"):
      response = self.call("add-reason", "init", self.directory, self.name)
      for line in response:
        print(line)
      self.assertEqual(1, 1)

  def test_no_linking(self):
    with cd("./tests/mockRoot"):
      response = self.call("add-reason", "init", self.directory, self.name, "--no-linking")
      for line in response:
        print(line)
      self.assertEqual(1, 2)

  def tearDown(self):
    with cd("./tests/mockRoot"):
      self.call("rm", "-f", "bsconfig.json")
      self.call("rm", "-f", ".merlin")
      self.call("rm", "-rf", "lib")
      self.call("rm", "-f", "node_modules/reason-package")
    
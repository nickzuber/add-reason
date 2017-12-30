import os
import subprocess

from tests.framework.base_command_test_case import BaseCommandTestCase
from tests.framework.context_manager import cd


class TestInit(BaseCommandTestCase):
  
  name = "reason-package"
  directory = "src/myCode"
  
  def test_steps_pass(self):
    with cd('./tests/mockRoot'):
      results = self.call("add-reason", "init", self.directory, self.name)
      all_steps_passed = True
      for _, result, _ in results['steps']:
        if not self.is_success(result):
          all_steps_passed = False
      self.assertTrue(all_steps_passed, 'All steps did not pass successfully')
      self.assertEqual(results['total_steps'], 5, 'The total number of steps was not 5')

  def test_steps_pass_no_linking(self):
    with cd('./tests/mockRoot'):
      results = self.call("add-reason", "init", self.directory, self.name, "--no-linking")
      all_steps_passed = True
      for _, result, _ in results['steps']:
        if not self.is_success(result):
          all_steps_passed = False
      self.assertTrue(all_steps_passed, 'All steps did not pass successfully')
      self.assertEqual(results['total_steps'], 3, 'The total number of steps was not 3')

  def test_bsconfig_was_created(self):
    with cd('./tests/mockRoot'):
      results = self.call("add-reason", "init", self.directory, self.name)
      exists = os.path.isfile('bsconfig.json')
      self.assertTrue(exists, 'bsconfig.json was never created')

  def test_merlin_was_created(self):
    with cd('./tests/mockRoot'):
      results = self.call("add-reason", "init", self.directory, self.name)
      exists = os.path.isfile('.merlin')
      self.assertTrue(exists, '.merlin was never created')

  def tearDown(self):
    with cd('./tests/mockRoot'):
      self.call("rm", "-f", "bsconfig.json")
      self.call("rm", "-f", ".merlin")
      self.call("rm", "-rf", "lib")
      self.call("rm", "-f", "node_modules/reason-package")
    
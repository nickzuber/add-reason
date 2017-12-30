import os
import subprocess

from tests.framework.base_command_test_case import BaseCommandTestCase
from tests.framework.context_manager import cd


class TestInit(BaseCommandTestCase):

  # This is a portion of the postinstall script we generate that should always be there only once, since
  # our script should never write itself more than once.
  should_exist_once = "fs=require('fs');if(fs.existsSync(d)===false){fs.symlinkSync(s,d,'dir')}"
  name = "reason-package"
  directory = "src/myCode"
  
  def test_steps_pass(self):
    with cd('./tests/root_for_testing'):
      results = self.call("add-reason", "init", self.directory, self.name)
      all_steps_passed = True
      for _, result, _ in results['steps']:
        if not self.is_success(result):
          all_steps_passed = False
      self.assertTrue(all_steps_passed, 'All steps did not pass successfully\n{}'.format(results))
      self.assertEqual(results['total_steps'], 5, 'The total number of steps was not 5')

  def test_steps_pass_no_linking_flag(self):
    with cd('./tests/root_for_testing'):
      results = self.call("add-reason", "init", self.directory, self.name, "--no-linking")
      all_steps_passed = True
      for _, result, _ in results['steps']:
        if not self.is_success(result):
          all_steps_passed = False
      self.assertTrue(all_steps_passed, 'All steps did not pass successfully\n{}'.format(results))
      self.assertEqual(results['total_steps'], 3, 'The total number of steps was not 3')

  def test_bsconfig_was_created(self):
    with cd('./tests/root_for_testing'):
      results = self.call("add-reason", "init", self.directory, self.name)
      exists = os.path.isfile('bsconfig.json')
      self.assertTrue(exists, 'bsconfig.json was never created')

  def test_merlin_was_created(self):
    with cd('./tests/root_for_testing'):
      results = self.call("add-reason", "init", self.directory, self.name)
      exists = os.path.isfile('.merlin')
      self.assertTrue(exists, '.merlin was never created')

  def test_postinstall_was_correctly_added_to_package_file_with_no_scripts(self):
    with cd('./tests/root_for_testing'):
      self.call("cp", "package.no_scripts.json", "package.json")
      self.call("add-reason", "init", self.directory, self.name)
      contents = self.read_json('package.json')
      self.assertIn('scripts', contents, 'Could not find scripts key in the package.json file')
      self.assertIn('postinstall', contents['scripts'], 'Could not find postinstall key in the package.json scripts key')
      postinstall_script = contents['scripts']['postinstall']
      self.assertEqual(postinstall_script.count(self.should_exist_once), 1, 'Found more than one instance of our postinstall script')

  def test_postinstall_was_correctly_added_to_package_file_with_empty_scripts(self):
    with cd('./tests/root_for_testing'):
      self.call("cp", "package.empty_scripts.json", "package.json")
      self.call("add-reason", "init", self.directory, self.name)
      contents = self.read_json('package.json')
      self.assertIn('scripts', contents, 'Could not find scripts key in the package.json file')
      self.assertIn('postinstall', contents['scripts'], 'Could not find postinstall key in the package.json scripts key')
      postinstall_script = contents['scripts']['postinstall']
      self.assertEqual(postinstall_script.count(self.should_exist_once), 1, 'Found more than one instance of our postinstall script')

  def test_postinstall_was_correctly_added_to_package_file_with_other_script(self):
    with cd('./tests/root_for_testing'):
      self.call("cp", "package.other_script.json", "package.json")
      self.call("add-reason", "init", self.directory, self.name)
      contents = self.read_json('package.json')
      self.assertIn('scripts', contents, 'Could not find scripts key in the package.json file')
      self.assertIn('postinstall', contents['scripts'], 'Could not find postinstall key in the package.json scripts key')
      postinstall_script = contents['scripts']['postinstall']
      self.assertEqual(postinstall_script.count(self.should_exist_once), 1, 'Found more than one instance of our postinstall script')

  def test_postinstall_was_correctly_added_to_package_file_with_other_postinstall(self):
    with cd('./tests/root_for_testing'):
      self.call("cp", "package.other_postinstall.json", "package.json")
      self.call("add-reason", "init", self.directory, self.name)
      contents = self.read_json('package.json')
      self.assertIn('scripts', contents, 'Could not find scripts key in the package.json file')
      self.assertIn('postinstall', contents['scripts'], 'Could not find postinstall key in the package.json scripts key')
      postinstall_script = contents['scripts']['postinstall']
      self.assertIn(' && ', postinstall_script, 'Our postinstall script was probably not integrated correctly with an existing postinstall')
      self.assertEqual(postinstall_script.count(self.should_exist_once), 1, 'Found more than one instance of our postinstall script')

  def test_postinstall_was_correctly_added_to_package_file_with_no_scripts_called_twice(self):
    with cd('./tests/root_for_testing'):
      self.call("add-reason", "init", self.directory, self.name)
      self.call("add-reason", "init", self.directory, self.name)
      contents = self.read_json('package.json')
      self.assertIn('scripts', contents, 'Could not find scripts key in the package.json file')
      self.assertIn('postinstall', contents['scripts'], 'Could not find postinstall key in the package.json scripts key')
      postinstall_script = contents['scripts']['postinstall']
      self.assertEqual(postinstall_script.count(self.should_exist_once), 1, 'Found more than one instance of our postinstall script')

  def test_postinstall_was_not_added_to_package_file_with_no_linking_flag(self):
    with cd('./tests/root_for_testing'):
      # This uses our default package.json copy which has a scripts key and no postinstall key
      self.call("add-reason", "init", self.directory, self.name, "--no-linking")
      contents = self.read_json('package.json')
      self.assertNotIn('postinstall', contents['scripts'], 'Postinstall was created when it should not have been')

  def tearDown(self):
    with cd('./tests/root_for_testing'):
      self.call("rm", "-f", "bsconfig.json")
      self.call("rm", "-f", ".merlin")
      self.call("rm", "-rf", "lib")
      self.call("rm", "-f", "node_modules/reason-package")
      self.call("cp", "package.empty_scripts.json", "package.json")
    
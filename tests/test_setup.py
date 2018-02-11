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
      result = self.call("node", "../../index.js", "setup", self.directory, self.name)
      self.assertTrue(result, 'Standard setup call did not pass successfully.')

  def test_steps_pass_no_linking_flag(self):
    with cd('./tests/root_for_testing'):
      result = self.call("node", "../../index.js", "setup", self.directory, self.name, "--no-linking")
      self.assertTrue(result, 'Standard --no-linking setup call did not pass successfully.')

  def test_bsconfig_was_created(self):
    with cd('./tests/root_for_testing'):
      self.call("node", "../../index.js", "setup", self.directory, self.name)
      exists = os.path.isfile('bsconfig.json')
      self.assertTrue(exists, 'bsconfig.json was never created')

  def test_merlin_was_created(self):
    with cd('./tests/root_for_testing'):
      self.call("node", "../../index.js", "setup", self.directory, self.name)
      exists = os.path.isfile('.merlin')
      self.assertTrue(exists, '.merlin was never created')

  def test_postinstall_was_correctly_added_to_package_file_with_no_scripts(self):
    with cd('./tests/root_for_testing'):
      self.call("cp", "package.no_scripts.json", "package.json")
      self.call("node", "../../index.js", "setup", self.directory, self.name)
      contents = self.read_json('package.json')
      self.assertIn('scripts', contents, 'Could not find scripts key in the package.json file')
      self.assertIn('postinstall', contents['scripts'], 'Could not find postinstall key in the package.json scripts key')
      postinstall_script = contents['scripts']['postinstall']
      self.assertEqual(postinstall_script.count(self.should_exist_once), 1, 'Found more than one instance of our postinstall script')

  def test_postinstall_was_correctly_added_to_package_file_with_empty_scripts(self):
    with cd('./tests/root_for_testing'):
      self.call("cp", "package.empty_scripts.json", "package.json")
      self.call("node", "../../index.js", "setup", self.directory, self.name)
      contents = self.read_json('package.json')
      self.assertIn('scripts', contents, 'Could not find scripts key in the package.json file')
      self.assertIn('postinstall', contents['scripts'], 'Could not find postinstall key in the package.json scripts key')
      postinstall_script = contents['scripts']['postinstall']
      self.assertEqual(postinstall_script.count(self.should_exist_once), 1, 'Found more than one instance of our postinstall script')

  def test_postinstall_was_correctly_added_to_package_file_with_other_script(self):
    with cd('./tests/root_for_testing'):
      self.call("cp", "package.other_script.json", "package.json")
      self.call("node", "../../index.js", "setup", self.directory, self.name)
      contents = self.read_json('package.json')
      self.assertIn('scripts', contents, 'Could not find scripts key in the package.json file')
      self.assertIn('postinstall', contents['scripts'], 'Could not find postinstall key in the package.json scripts key')
      postinstall_script = contents['scripts']['postinstall']
      self.assertEqual(postinstall_script.count(self.should_exist_once), 1, 'Found more than one instance of our postinstall script')

  def test_postinstall_was_correctly_added_to_package_file_with_other_postinstall(self):
    with cd('./tests/root_for_testing'):
      self.call("cp", "package.other_postinstall.json", "package.json")
      self.call("node", "../../index.js", "setup", self.directory, self.name)
      contents = self.read_json('package.json')
      self.assertIn('scripts', contents, 'Could not find scripts key in the package.json file')
      self.assertIn('postinstall', contents['scripts'], 'Could not find postinstall key in the package.json scripts key')
      postinstall_script = contents['scripts']['postinstall']
      self.assertIn(' && ', postinstall_script, 'Our postinstall script was probably not integrated correctly with an existing postinstall')
      self.assertEqual(postinstall_script.count(self.should_exist_once), 1, 'Found more than one instance of our postinstall script')

  def test_postinstall_was_correctly_added_to_package_file_with_no_scripts_called_twice(self):
    with cd('./tests/root_for_testing'):
      self.call("node", "../../index.js", "setup", self.directory, self.name)
      self.call("node", "../../index.js", "setup", self.directory, self.name)
      contents = self.read_json('package.json')
      self.assertIn('scripts', contents, 'Could not find scripts key in the package.json file')
      self.assertIn('postinstall', contents['scripts'], 'Could not find postinstall key in the package.json scripts key')
      postinstall_script = contents['scripts']['postinstall']
      self.assertEqual(postinstall_script.count(self.should_exist_once), 1, 'Found more than one instance of our postinstall script')

  def test_postinstall_was_not_added_to_package_file_with_no_linking_flag(self):
    with cd('./tests/root_for_testing'):
      # This uses our default package.json copy which has a scripts key and no postinstall key
      self.call("node", "../../index.js", "setup", self.directory, self.name, "--no-linking")
      contents = self.read_json('package.json')
      self.assertNotIn('postinstall', contents['scripts'], 'Postinstall was created when it should not have been')

  def test_config_was_not_created_when_given_bad_target(self):
    with cd('./tests/root_for_testing'):
      self.call("node", "../../index.js", "setup", 'some/bad/target', self.name)
      existsMerlin = self.exists('.merlin')
      existsBsconfig = self.exists('bsconfig.json')
      self.assertFalse(existsMerlin, '.merlin file was created even though we gave a bad target')
      self.assertFalse(existsBsconfig, 'bsconfig.json file was created even though we gave a bad target')

  def test_config_was_not_created_when_given_bad_target_no_linking(self):
    with cd('./tests/root_for_testing'):
      self.call("node", "../../index.js", "setup", 'some/bad/target', self.name, "--no-linking")
      existsMerlin = self.exists('.merlin')
      existsBsconfig = self.exists('bsconfig.json')
      self.assertFalse(existsMerlin, '.merlin file was created even though we gave a bad target')
      self.assertFalse(existsBsconfig, 'bsconfig.json file was created even though we gave a bad target')

  def test_postinstall_was_not_created_when_given_bad_target(self):
    with cd('./tests/root_for_testing'):
      self.call("cp", "package.other_postinstall.json", "package.json")
      contents = self.read_json('package.json')
      postinstall_before = contents['scripts']['postinstall']
      self.call("node", "../../index.js", "setup", 'some/bad/target', self.name)
      contents = self.read_json('package.json')
      postinstall_after = contents['scripts']['postinstall']
      self.assertEqual(postinstall_before, postinstall_after, 'The postinstall script was altered even though we gave bad target')

  def tearDown(self):
    with cd('./tests/root_for_testing'):
      self.call("rm", "-f", "bsconfig.json")
      self.call("rm", "-f", ".merlin")
      self.call("rm", "-rf", "lib")
      self.call("rm", "-f", "node_modules/reason-package")
      self.call("cp", "package.empty_scripts.json", "package.json")

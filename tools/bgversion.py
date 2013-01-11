# Module bulmages_version.py
import re
import sys
import os
import time
import shutil
import tempfile
import fileinput
import subprocess

import git

class BgVersion:
    # You can test the regexp with
    # /usr/share/doc/python2.7/examples/Tools/scripts/redemo.py
    # example_str:  "SET( BULMAGES_VERSION 0.14.0 )"
    regexp_ver_str = r"BULMAGES_VERSION ([^\s]+)"
    regexp_ver = re.compile(regexp_ver_str)
    regexp_ver_repl = "BULMAGES_VERSION %s" # To be % with version value

    # Just to detect comments
    regexp_comm_str = r'^\s*#'
    regexp_comm = re.compile(regexp_comm_str)

    # Git Data
    repo = None
    active_commit = ""
    active_branch = ""


    def __init__(self, git_dir = '.'):
        # We see current git parameters

        # See http://packages.python.org/GitPython/0.3.1/tutorial.html
        try:
            self.repo = git.Repo(git_dir)
            self.active_commit = self.repo.commit().hexsha

            if self.repo.head.is_detached:
                self.active_branch = None
            else:
                self.active_branch = self.repo.active_branch.name
        except git.InvalidRepositoryError:
            self.repo = None
            self.active_commit = ""
            self.active_branch = ""

    def get_git_info(self):
        return (self.active_branch, self.active_commit)

    def obtain_cmakelists_version(self, filename, target_commit = None):
        """Get cmakelists_version from CMakeLists.txt in this commit or another"""

        # By default we use the active_commit
        if not target_commit:
            target_commit = self.active_commit

        tempdir = ""
        if self.active_commit != target_commit :
            # We do a temporary checkout of bulmages/CMakeLists.txt
            #
            # There is no way to checkout the file in a external dir
            # without modifying the local copy.  Attemps tried:
            #
            # -  --work-tree=tempdir
            # -  cd tempdir && --git-dir=<curdir/.git> --work-tree=tempdir
            # -  cd tempdir &&  (as above) with --bare
            #
            # So what we do is temporary copy CMakeLists to the external
            # dir, re-checkout and then copying it again
            tempdir = tempfile.mkdtemp()
            shutil.copy2(filename, tempdir)

            # python-git does not support this yet
            # Cmd:
            # git checkout 7f7787fc9bc4a3f01bb89e9ebf3d0ea0921b157c -- bulmages/CMakeLists.txt
            checkout_cmd = "git checkout %s -- %s" % (target_commit, filename)
            _do_command_or_exit(checkout_cmd)

        cmakelists_version = ""

        with open(filename, 'r') as file:
            for line in file:
                if BgVersion.regexp_comm.match(line):
                    # Comment line
                    continue
                match = BgVersion.regexp_ver.search(line) # search instead of match because it is not 
                                                    # anchored at the beginning of the line
                if match :
                    cmakelists_version = match.groups()[0]
                    break
            file.close()

        # We backtrack the commit and restore a possibly changed work cmakelists_version
        if tempdir:
            checkout_cmd = "git checkout %s -- %s" % (self.active_commit, filename)
            _do_command_or_exit(checkout_cmd)

            temp_filename = os.path.join(tempdir, os.path.basename(filename) )
            shutil.copy2(temp_filename, filename)

            shutil.rmtree(tempdir)

        if not cmakelists_version:
            print("CMakeLists cmakelists_version no encontrada en %s" % filename)
            sys.exit(-1)

        return cmakelists_version

    def obtain_git_version_info(self, target_commit = None):
        if not target_commit:
            target_commit = self.active_commit

        if not self.repo :
            return ("", "", "")

        commit = self.repo.commit(target_commit)
        timestamp_gmt = time.gmtime(commit.authored_date)
        ts_date = time.strftime("%Y%m%d", timestamp_gmt)
        ts_time = time.strftime("%H%M", timestamp_gmt)
        short_hash = commit.hexsha[:8]

        return (ts_date, ts_time, short_hash)
    
    def set_cmakelists_version(self, filename, version, create_debian_patch = False):

        if create_debian_patch :
            auto_patch_name = "bgversion_auto_codeversion_upgrade.patch"
            
            # If the patch exists we remove it
            if os.path.isfile("debian/patches/"+auto_patch_name):
                # The -r removes the patch file as well (otherwise only the series file would be changed)
                cmdline = "QUILT_PATCHES=debian/patches quilt delete -r %s" % auto_patch_name
                _do_command_or_exit(cmdline)
            
            # First we do quilt push -a if we have a patch
            cmdline = "QUILT_PATCHES=debian/patches quilt series"
            patch_list = _run_command_capture_stdout(cmdline, shell=True)
            if patch_list:
                cmdline = "QUILT_PATCHES=debian/patches quilt push -a"
                _do_command_or_exit(cmdline)

            # Now we start a new patch and mark CMakeLists.txt to be modified
            cmdline = "QUILT_PATCHES=debian/patches quilt new %s" % auto_patch_name
            _do_command_or_exit(cmdline)
            
            cmdline = "QUILT_PATCHES=debian/patches quilt add %s" % filename
            _do_command_or_exit(cmdline)

        else:
            # We do it interactively
            print "About to modify file %s to set the version as %s" % (filename, version)
            try:
                raw_input("Press ENTER to proceed or Ctrl^C to cancel\n")
            except KeyboardInterrupt:
                print "Aborted!"
                sys.exit(0)

        # Second answer in
        # http://stackoverflow.com/questions/39086/search-and-replace-a-line-in-a-file-in-python
        for line in fileinput.FileInput(filename, inplace = 1):
            if BgVersion.regexp_comm.match(line):
                # Comment line
                new_line = line
            elif BgVersion.regexp_ver.search(line) : # search instead of match because it is not 
                                             # anchored at the beginning of the line
                # example_str:  "SET( BULMAGES_VERSION 0.14.0 )"
                # regexp_ver_str = r"BULMAGES_VERSION ([^\s]+)"
                new_line = BgVersion.regexp_ver.sub( BgVersion.regexp_ver_repl % version, line)
            else:
                new_line = line
            print new_line,

        if create_debian_patch:
            cmdline = "QUILT_PATCHES=debian/patches quilt refresh"
            _do_command_or_exit(cmdline)

            cmdline = "QUILT_PATCHES=debian/patches quilt pop -a"
            _do_command_or_exit(cmdline)

    def reset_cmakelists_version(self, filename):
        if not filename:
            print "Filename CMakeLists.txt not specified"
            sys.exit(-1)

        print "About to RESET file %s to git contents" % filename
        try:
            raw_input("Press ENTER to proceed or Ctrl^C to cancel\n")
        except KeyboardInterrupt:
            print "Aborted!"
            sys.exit(0)

        checkout_cmd = "git checkout %s -- %s" % (self.active_commit, filename)
        _do_command_or_exit(checkout_cmd)


# ------------------------------------------------------------------------------------------
# ADAPT THIS TO COMPUTE THE VERSION
# This is a library function not belonging to bg_version class

def assemble_code_and_full_versions(cmakelists_version, ts_date, ts_time, commit_short_hexsha, mode):
    if mode == 'master':
        code_version = "%s.%s.%s" % (cmakelists_version, ts_date, ts_time)
        full_version = "%s-%s" % (code_version, commit_short_hexsha)
    elif mode == "release":
        code_version = cmakelists_version
        full_version = "%s-%s" % (code_version, commit_short_hexsha)
    else:
        print "mode master/release not specified"
        sys.exit(-1)

    return (code_version, full_version)
# -------------------------------------------------------------------------------------------

def _do_command_or_exit(cmdline, verbose=False):
    if verbose:
        print("About to execute: %s" % cmdline)

    status = os.system(cmdline)
    if status != 0 :
        print("Error executing cmdline: %s " % cmdline)
        sys.exit(-1)

# A simple thing that ehemmm PERL has out of the box with the ``
# strings but that python needs to do more carefully
#
# Taken from
# http://stackoverflow.com/questions/4760215/running-shell-command-from-python-and-capturing-the-output
def _run_command_capture_stdout(cmdline, shell=False):
    
    if not shell:
        prog = cmdline.split()
    else:
        prog = cmdline

    p = subprocess.Popen(prog, stdout=subprocess.PIPE,
                         stderr=subprocess.PIPE, shell=shell)

    out, err = p.communicate()

    return out
    

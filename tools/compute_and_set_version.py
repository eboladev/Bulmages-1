#!/usr/bin/env python

import re
import sys
import time
import os
import argparse
import tempfile
import fileinput
import shutil


# From python-git package
import git

# From python standard lib, but only for python2
# python3 has the same name in lowercase letters
import ConfigParser

class opt:
    commit = ""
    force_version = ""
    mode = ""
    print_only = ""


class glb:
    commit = ""
    active_commit = ""
    version = ""
    mode = ""
    repo = None
    active_branch = ""
    filename = ""

    # You can test the regexp with
    # /usr/share/doc/python2.7/examples/Tools/scripts/redemo.py
    # example_str:  "SET( BULMAGES_VERSION 0.14.0 )"
    regexp_ver_str = r"BULMAGES_VERSION ([^\s]+)"
    regexp_ver = re.compile(regexp_ver_str)

    # Just to detect comments
    regexp_comm_str = r'^\s*#'
    regexp_comm = re.compile(regexp_comm_str)



def main():

    parse_argv()

    # Get the date and hash of the latest commit
    # ------------------------------------------
    # See http://packages.python.org/GitPython/0.3.1/tutorial.html
    glb.repo = git.Repo('.')
    consolidate_glb()
    
    if glb.version :
        revision = ""

    if not glb.version:
        # We look at the version in bulmages/CMakeLists.txt
        # (temporary checkout included)
        code_version = obtain_code_version(glb.filename)

        commit = glb.repo.commit(glb.commit)
        timestamp = time.strftime("%Y%m%d.%H%M", time.gmtime(commit.committed_date))
        short_hash = commit.hexsha[:8]

        glb.version, revision = assemble_version_and_revision(code_version, timestamp, short_hash, glb.mode)

    if opt.print_only:
        print glb.version
        if revision:
            print "%s-%s" % (glb.version, revision)
        sys.exit(0)

    if glb.active_commit == glb.commit:
        reset_code_version()
    else:
        print "NOT RESETTING %s because we are looking at a different commit" % glb.filename
        print "Version: %s       Version-Revision: %s-%s" % (glb.version, glb.version, revision)

# ---------------------------------------------------------------------------------------

# ADAPT THIS TO COMPUTE THE VERSION
def assemble_version_and_revision(code_version, commit_timestamp, commit_short_hexsha, mode):
    version = ""
    if mode == 'master':
        version = "%s.%s" % (code_version, commit_timestamp)
        revision = commit_short_hexsha
    elif mode == "release":
        version = code_version
        revision = commit_short_hexsha
    else:
        print "mode master/release not specified"
        sys.exit(-1)

    return (version, revision)

# -----------------------------------------------------

def obtain_code_version(filename):
    """Get version from CMakeLists.txt"""

    tempdir = ""
    if glb.active_commit != glb.commit :
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
        checkout_cmd = "git checkout %s -- %s" % (glb.commit, filename)
        do_command_or_exit(checkout_cmd)

    version = ""

    with open(filename, 'r') as file:
        for line in file:
            if glb.regexp_comm.match(line):
                # Comment line
                continue
            match = glb.regexp_ver.search(line) # search instead of match because it is not 
                                                # anchored at the beginning of the line
            if match :
                version = match.groups()[0]
                break
        file.close()

    # We backtrack the commit and restore a possibly changed work version
    if tempdir:
        checkout_cmd = "git checkout %s -- %s" % (glb.active_commit, filename)
        do_command_or_exit(checkout_cmd)

        temp_filename = os.path.join(tempdir, os.path.basename(filename) )
        shutil.copy2(temp_filename, filename)

        shutil.rmtree(tempdir)

    if not version:
        print("version no encontrada en %s" % filename)
        sys.exit(-1)

    return version

# --------------------------------------------------------------------

def reset_code_version():
    
    print "About to modify file %s to set the version as %s" % (glb.filename, glb.version)
    try:
        raw_input("Press ENTER to proceed or Ctrl^C to cancel\n")
    except KeyboardInterrupt:
        print "Aborted!"
        sys.exit(0)

    # Second answer in
    # http://stackoverflow.com/questions/39086/search-and-replace-a-line-in-a-file-in-python
    for line in fileinput.FileInput(glb.filename, inplace = 1):
        if glb.regexp_comm.match(line):
            # Comment line
            new_line = line
        elif glb.regexp_ver.search(line) : # search instead of match because it is not 
                                         # anchored at the beginning of the line
            # example_str:  "SET( BULMAGES_VERSION 0.14.0 )"
            # regexp_ver_str = r"BULMAGES_VERSION ([^\s]+)"
            new_line = glb.regexp_ver.sub("BULMAGES_VERSION %s" % glb.version, line)
        else:
            new_line = line
        print new_line,

# --------------------------------------------------------------------

def consolidate_glb():
    

    if opt.commit:
        glb.commit = opt.commit

    if opt.force_version:
        glb.version = opt.force_version

    if opt.mode:
        glb.mode = opt.mode

    # git state
    if glb.repo.head.is_detached:
        glb.active_branch = None
    else:
        glb.active_branch = glb.repo.active_branch.name

    glb.active_commit = glb.repo.commit().hexsha

    # First we decide if we are in mode master or mode release
    # --------------------------------------------------------

    # First preference active_branch (unless we are detached)
    if not glb.mode and glb.active_branch:
        if 'release' in glb.active_branch:
            glb.mode = 'release'
        elif 'master' in glb.active_branch:
            glb.mode = 'master'
    else:
        # Default mode
        glb.mode = 'master'

    # If the commit is not specified we look at the current commit
    if not glb.commit:
        glb.commit = glb.active_commit

    # NOTE 1: glb.version is left intentionally unchanged either defined or not
        
# ----------------------------------------------------------------------

# useful tutorial for argparse
# http://docs.python.org/2/howto/argparse.html
# Missing point: nargs='?' for optional position arguments
def parse_argv():

    parser = argparse.ArgumentParser(description='Computes the version and revision and updates CMakeLists.txt.')

    parser.add_argument("commit", 
                        help = "commit reference used in the computation, defaults to the current checked out work-copy",
                        default = "", nargs='?')

    parser.add_argument("--force-version", 
                        help = "force this version to be used in CMakesList")

    parser.add_argument("--mode", choices = ["master", "release"],
                        help = "master or release mode (default: search in active branch or master if not found)")

    parser.add_argument("--filename", default="bulmages/CMakeLists.txt",
                        help = "CMakeLists.txt file to be used for parsing and replacing")

    parser.add_argument('--print-only',
                        action = 'store_true',
                        help = 'Compute the version and revision and print to stdout')

    args = parser.parse_args()

    opt.commit = args.commit
    opt.force_version = args.force_version
    opt.mode = args.mode
    opt.print_only = args.print_only
    glb.filename = args.filename

def do_command_or_exit(cmdline, verbose=False):
    if verbose:
        print("About to execute: %s" % cmdline)

    status = os.system(cmdline)
    if status != 0 :
        print("Unable to checkout the requested commit")
        sys.exit(-1)


if __name__ == '__main__':
    main()




    

            

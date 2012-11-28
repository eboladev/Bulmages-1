#!/usr/bin/env python

import re
import sys
import time
import os
import argparse

# From python-git package
import git

class opt:
    version_only = False

def main():
    parsea_argv()

    # Get the upstream version from CMakelists
    # ----------------------------------------
    #
    # You can test the regexp with
    # /usr/share/doc/python2.7/examples/Tools/scripts/redemo.py
    #
    # example_str:  "SET( BULMAGES_VERSION 0.14.0 )"
    regexp_ver_str = r"BULMAGES_VERSION ([^\s]+)"

    filename = 'bulmages/CMakeLists.txt'
    regexp_ver = re.compile(regexp_ver_str)

    # Just to detect comments
    regexp_comm_str = r'^\s*#'
    regexp_comm = re.compile(regexp_comm_str)

    upstream_version = ""

    with open(filename, 'r') as file:
        for line in file:
            if regexp_comm.match(line):
                # Comment line
                continue
            match = regexp_ver.search(line) # search instead of match because it is not 
                                            # anchored at the beginning of the line
            if match :
                upstream_version = match.groups()[0]
                break
        file.close()

    if not upstream_version:
        print("upstream version no encontrada en %s" % filename)
        sys.exit(-1)

    # Get the date and hash of the latest commit
    # ------------------------------------------
    # See http://packages.python.org/GitPython/0.3.1/tutorial.html
    repo = git.Repo('.')
    commit = repo.commit("HEAD")
    timestamp = time.strftime("%Y%m%d.%H%M", time.gmtime(commit.committed_date))
    hash = commit.hexsha[:8]

    debian_orig_version = upstream_version + "." + timestamp + "." + hash

    if opt.version_only:
        print debian_orig_version
        sys.exit(0)

    print "Version: " + debian_orig_version

    # Now we create the .orig.tar.gz
    # tar --exclude=.git -z -c -v -f ../bulmages_0.14.0.20121128.1132.f070fef1.orig.tar.gz \
    # ../mtelleria-bulmages

    cur_basedir = os.path.basename(os.getcwd())
    cmdline = "tar --exclude=.git -z -c -v -f ../bulmages_%s.orig.tar.gz ../%s" % (debian_orig_version, cur_basedir)

    print "Cmdline: %s" % cmdline
    try:
        raw_input("Press ENTER to proceed or Ctrl^C to cancel\n")
    except KeyboardInterrupt:
        print "Aborted!"
        sys.exit(0)
    
    os.system(cmdline)

def parsea_argv() :

    parser = argparse.ArgumentParser(description='Generates the orig.tar.gz with version info from CMakeLists and git.')

    parser.add_argument('--version-only',
                        action = 'store_true',
                        help = 'print only version number and do not launch tar')

    args = parser.parse_args()

    opt.version_only = args.version_only

if __name__ == '__main__':
    main()




    

            

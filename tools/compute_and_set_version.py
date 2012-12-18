#!/usr/bin/env python

import sys
# Prevent cluttering with bgversion.pyc & co
sys.dont_write_bytecode = True
import argparse
import bgversion

# From python-git package
import git

class opt:
    commit = ""
    force_version = ""
    mode = ""
    print_only = ""
    reset = ""


class glb:
    commit = ""
    version = ""
    mode = ""
    filename = ""
    bg_version = None

def main():

    glb.bg_version = bgversion.BgVersion()

    parse_argv()

    consolidate_glb()
    
    if opt.reset :
        # We reset the value to git's content
        glb.bg_version.reset_code_version(glb.filename)
        sys.exit(0)

    if glb.version :
        full_version = glb.version
    else:
        # We look at the version in bulmages/CMakeLists.txt
        # (temporary checkout included)
        code_version = glb.bg_version.obtain_code_version(glb.filename, glb.commit)
        (ts_date, ts_time, short_hash) = glb.bg_version.obtain_git_version_info(glb.commit)

        glb.version, full_version = bgversion.assemble_version_and_revision(code_version, ts_date, ts_time, short_hash, glb.mode)

    if opt.print_only:
        print glb.version
        print full_version
        sys.exit(0)

    glb.bg_version.set_code_version(glb.filename, glb.version)


def consolidate_glb():
    if opt.commit:
        glb.commit = opt.commit

    if opt.force_version:
        glb.version = opt.force_version

    if opt.mode:
        glb.mode = opt.mode

    (active_branch, active_commit) = glb.bg_version.get_git_info()


    # First we decide if we are in mode master or mode release
    # --------------------------------------------------------

    # First preference active_branch (unless we are detached)
    if not glb.mode and active_branch:
        if 'release' in active_branch:
            glb.mode = 'release'
        elif 'master' in active_branch:
            glb.mode = 'master'
    else:
        # Default mode
        glb.mode = 'master'

    # If the commit is not specified we look at the current commit
    if not glb.commit:
        glb.commit = active_commit

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

    parser.add_argument('--reset',
                        action = 'store_true',
                        help = 'Reset the CMakeLists.txt file to its original git contents')

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
    opt.reset = args.reset
    glb.filename = args.filename


if __name__ == '__main__':
    main()




    

            

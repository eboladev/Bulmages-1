#!/usr/bin/env python

import sys
# Prevent cluttering with bgversion.pyc & co
sys.dont_write_bytecode = True
import os
import argparse

import bgversion

class opt:
    force_version = ""
    mode = ""
    print_only = ""

class glb:
    code_version = ""
    mode = ""
    filename = ""
    bg_version = None

def main():
    
    glb.bg_version = bgversion.BgVersion()
    parsea_argv()
    consolidate_glb()

    if glb.code_version:
        full_version = glb.code_version
    else:
        cmakelists_version = glb.bg_version.obtain_cmakelists_version(glb.filename)
        (ts_date, ts_time, short_hash) = glb.bg_version.obtain_git_version_info()
        glb.code_version, full_version = bgversion.assemble_code_and_full_versions(cmakelists_version, 
                                                                                   ts_date, ts_time, short_hash, glb.mode)

    # Now we create the .orig.tar.gz
    # tar --exclude=.git -z -c -v -f ../bulmages_0.14.0.20121128.1132-f070fef1.orig.tar.gz \
    # ../mtelleria-bulmages


    cur_basedir = os.path.basename(os.getcwd())
    cmdline = "tar --exclude=.git --exclude=debian -z -c -v -f ../bulmages_%s.orig.tar.gz ../%s" % (full_version, cur_basedir)

    if opt.print_only :
        print cmdline
        sys.exit(0)

    print "Cmdline: %s" % cmdline
    try:
        raw_input("Press ENTER to proceed or Ctrl^C to cancel\n")
    except KeyboardInterrupt:
        print "Aborted!"
        sys.exit(0)
    
    os.system(cmdline)

def consolidate_glb() :
    if opt.force_version:
        glb.code_version = opt.force_version

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
    

def parsea_argv() :

    parser = argparse.ArgumentParser(description='Generates the orig.tar.gz with version info from CMakeLists and git.')

    parser.add_argument('--print-only',
                        action = 'store_true',
                        help = 'print only version number and do not launch tar')

    parser.add_argument("--force-version", 
                        help = "force this version to be used as full version in the orig.tar.gz file")

    parser.add_argument("--mode", choices = ["master", "release"],
                        help = "master or release mode (default: search in active branch or master if not found)")

    parser.add_argument("--filename", default="bulmages/CMakeLists.txt",
                        help = "CMakeLists.txt file to be used for parsing and replacing")

    args = parser.parse_args()

    opt.force_version = args.force_version
    opt.mode = args.mode
    opt.print_only = args.print_only
    glb.filename = args.filename

if __name__ == '__main__':
    main()




    

            
